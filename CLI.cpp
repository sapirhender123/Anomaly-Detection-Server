#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;

    data.threshold = DEFAULT_THRESHOLD;
    data.report = vector<AnomalyReport>();

    commands = new Command *[COMMAND_COUNT];
    commands[0] = new UploadCSVCommand(dio, &data);
    commands[1] = new AlgorithmSettingsCommand(dio, &data);
    commands[2] = new DetectCommand(dio, &data);
    commands[3] = new DisplayResultsCommand(dio, &data);
    commands[4] = new AnomaliesCommand(dio, &data);
}

void CLI::printOptions() {
    dio->write("Welcome to the Anomaly Detection Server.\n"
               "Please choose an option:\n");
    int i;
    for (i = 0; i < COMMAND_COUNT; ++i) {
        dio->write((float) (i + 1));
        dio->write("." + commands[i]->description + "\n");
    }
    dio->write((float) (i + 1));
    dio->write(".exit\n");
}

void CLI::start() {
    float option = 0;
    do {
        printOptions();
        dio->read(&option);
        if (option > 6) {
            dio->write("illegal choice, try again");
            break;
        }
        classifyOpt(option);
    } while (option != 6);
}

void ::CLI::classifyOpt(int option) {
    switch (option) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            commands[option - 1]->execute();
            break;
        default:
            break;
    }
}

CLI::~CLI() = default;
