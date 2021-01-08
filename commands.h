// 208414573 Sapir Hender
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include "HybridAnomalyDetector.h"
#include "timeseries.h"
#include <fstream>
#include <netdb.h>
#include <vector>

#define COMMAND_COUNT (5)

using namespace std;

typedef struct {
    int csv_line_count;
    float threshold;
    vector<AnomalyReport> report;
} CLIData;

typedef struct {
    string description;
    long timeStepBegin;
    long timeStepEnd;
} AnomalyReportRange;

// the classes that will inherit from this class will implement the read and write
class DefaultIO {
public:
    virtual string read() = 0;
    virtual void write(string text) = 0;
    virtual void write(float f) = 0;
    virtual void read(float *f) = 0;
    virtual ~DefaultIO() = default;
};

class StandardIO : public DefaultIO {
public:
    string read() override {
        string input;
        cin >> input;
        return input;
    };
    void write(string text) override { cout << text; }
    void write(float f) override { cout << f; }
    void read(float *f) override { cin >> *f; }
    ~StandardIO() override = default;
};

class SocketIO : public DefaultIO {
    int clientSocket;

public:
    explicit SocketIO(int clientSocket) : clientSocket(clientSocket) {}
    string read() override {
        string res;

        char buf = 0;
        while (true) {
            recv(clientSocket, &buf, sizeof(buf), 0);
            if (buf == '\n') break;
            res += buf;
        }

        return res;
    };
    void write(string text) override { send(clientSocket, text.data(), text.size(), 0); }
    void write(float f) override {
        std::ostringstream ss;
        ss << f;
        std::string s(ss.str());
        send(clientSocket, s.data(), s.size(), 0);
    }
    void read(float *f) override {
        // 1 for float, 1 for \n
        static char buffer[2]{};

        // For small packets that are sent close to each other, the kernel
        // may sometimes combine packets in an unwanted manner. For example:
        // 3\n, 5\n => 3, \n5\n
        // So wait until 2 bytes are received :)
        recv(clientSocket, buffer, sizeof(buffer), MSG_WAITALL);
        std::istringstream ss(buffer);
        ss >> *f;
    }
    ~SocketIO() override = default;
};

class Command {

protected:
    DefaultIO *dio;
    CLIData *data;

public:
    string description;

    Command(DefaultIO *dio, CLIData *data) : dio(dio), data(data) {}
    virtual void execute() = 0;// abstract method
    virtual ~Command() = default;
};

// option 1
class UploadCSVCommand : public Command {
private:
public:
    UploadCSVCommand(DefaultIO *dio, CLIData *data) : Command(dio, data) {
        description = "upload a time series csv file";
    }

    void execute() override {
        std::string line;

        // As the server, return this output
        dio->write("Please upload your local train CSV file.\n");
        std::ofstream anomalyTrain("anomalyTrain.csv");

        // TODO: When we implement the client, he needs to fill the dio with
        //       the data of the csv file.
        do {
            line = dio->read();
            if (line == "done") break;
            anomalyTrain << line << endl;
        } while (true);
        anomalyTrain.close();
        dio->write("Upload complete.\n");

        dio->write("Please upload your local test CSV file.\n");
        std::ofstream anomalyTest("anomalyTest.csv");
        // TODO: When we implement the client, he needs to fill the dio with
        //       the data of the csv file.
        data->csv_line_count = -1;
        do {
            line = dio->read();
            if (line == "done") break;
            anomalyTest << line << endl;
            data->csv_line_count++;
        } while (true);
        anomalyTest.close();
        dio->write("Upload complete.\n");
    }
};

// option 2
class AlgorithmSettingsCommand : public Command {
public:
    AlgorithmSettingsCommand(DefaultIO *dio, CLIData *data) : Command(dio, data) {
        description = "algorithm settings";
    }

    void execute() override {
        dio->write("The current correlation threshold is ");
        dio->write(data->threshold);
        dio->write("\n");
        dio->write("Type a new threshold\n");

        float newThreshold = -1;

        do {
            dio->read(&newThreshold);
            // Check if threshold is legal
            if (newThreshold < 0 || newThreshold > 1) {
                dio->write("please choose a value between 0 and 1\n");
            }
        } while (newThreshold < 0 || newThreshold > 1);
        data->threshold = newThreshold;
    }
};

// option 3
class DetectCommand : public Command {
private:
public:
    DetectCommand(DefaultIO *dio, CLIData *data) : Command(dio, data) {
        description = "detect anomalies";
    }

    void execute() override {
        TimeSeries ts("anomalyTrain.csv");
        HybridAnomalyDetector ad = HybridAnomalyDetector(data->threshold);

        ad.learnNormal(ts);
        TimeSeries ts2("anomalyTest.csv");
        data->report = ad.detect(ts2);
        dio->write("anomaly detection complete.\n");
    }
};

// option 4
class DisplayResultsCommand : public Command {
private:
public:
    DisplayResultsCommand(DefaultIO *dio, CLIData *data) : Command(dio, data) {
        description = "display results";
    }
    void execute() override {
        for (const auto &r : data->report) {
            dio->write((float) r.timeStep);
            dio->write("\t" + r.description + "\n");
        }
        dio->write("Done.\n");
    }
};

// option 5
class AnomaliesCommand : public Command {
private:
public:
    AnomaliesCommand(DefaultIO *dio, CLIData *data) : Command(dio, data) {
        description = "upload anomalies and analyze results";
    }

    void execute() override {
        // First - create vector of AnomalyReportRange according to the data
        vector<AnomalyReportRange> report_ranges;
        for (int i = 0; i < data->report.size(); ++i) {
            AnomalyReportRange rr;
            rr.description = data->report[i].description;
            rr.timeStepBegin = data->report[i].timeStep;
            i++;
            int range_length = 1;
            while (i < data->report.size() && data->report[i].description == rr.description &&
                   data->report[i].timeStep == rr.timeStepBegin + range_length) {
                i++;
                range_length++;
            }
            // subtract the index for the last iteration
            rr.timeStepEnd = data->report[--i].timeStep;
            report_ranges.emplace_back(rr);
        }

        int FP = 0;
        int TP = 0;
        float false_alarm_rate;  // FP / N (= number of time steps without anomalies)
        float true_positive_rate;// TP / P (= number ranges in input report file)

        int P = 0, N = data->csv_line_count;

        dio->write("Please upload your local anomalies file.\n");
        vector<AnomalyReportRange> user_ranges;

        do {
            string line = dio->read();
            if (line == "done") break;// if it is the end of the file

            std::stringstream ss(line);
            int begin, end;
            ss >> begin;
            if (ss.peek() == ',') ss.ignore();
            ss >> end;

            P++;                   // another range of anomalies
            N -= (end - begin + 1);// +1 in order to include all the range

            AnomalyReportRange r = {"", begin, end};
            user_ranges.emplace_back(r);
        } while (true);

        for (auto &report_range : report_ranges) {
            bool isTP = false;
            for (auto &r : user_ranges) {
                // if there any intersection between the ranges
                if (r.timeStepBegin <= report_range.timeStepEnd &&
                    report_range.timeStepBegin <= r.timeStepEnd) {
                    TP++;
                    isTP = true;
                }
            }

            if (!isTP) { FP++; }
        }

        dio->write("Upload complete.\n");
        true_positive_rate = (float) TP / (float) P;
        dio->write("True Positive Rate: ");
        dio->write(((int) (true_positive_rate * 1000)) / 1000.0f);

        false_alarm_rate = (float) FP / (float) N;
        dio->write("\nFalse Positive Rate: ");
        dio->write(((int) (false_alarm_rate * 1000)) / 1000.0f);
        dio->write("\n");
    }
};

#endif /* COMMANDS_H_ */
