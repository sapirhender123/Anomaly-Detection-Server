////
////
//#include "AnomalyDetector.h"
//#include "HybridAnomalyDetector.h"
//#include <fstream>
//#include <iostream>
//#include <math.h>
//#include <stdlib.h> /* srand, rand */
//#include <time.h>   /* time */
//#include <vector>
////
////using namespace std;
////
////// this is a simple test to put you on the right track
//void generateTrainCSV(float a1, float b1, float a2, float b2) {
//    ofstream out("trainFile1.csv");
//    out << "A,B,C,D" << endl;
//    Line ac(a1, b1);
//    Line bd(a2, b2);
//    for (int i = 1; i <= 100; i++) {
//        float a = i;
//        float b = rand() % 40;
//        out << a << "," << b << "," << ac.f(a) - 0.02 + (rand() % 40) / 100.0f << ","
//            << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << endl;
//    }
//    out.close();
//}
//
//void generateTestCSV(float a1, float b1, float a2, float b2, int anomaly) {
//    ofstream out("testFile1.csv");
//    out << "A,B,C,D" << endl;
//    Line ac(a1, b1);
//    Line bd(a2, b2);
//    for (int i = 1; i <= 100; i++) {
//        float a = i;
//        float b = rand() % 40;
//        if (i != anomaly)
//            out << a << "," << b << "," << ac.f(a) - 0.02 + (rand() % 40) / 100.0f << ","
//                << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << endl;
//        else
//            out << a << "," << b << "," << ac.f(a) + 1 << ","
//                << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << endl;
//    }
//    out.close();
//}
//
//void checkCorrelationTrain(correlatedFeatures c, string f1, string f2, float a, float b) {
//    if (c.feature1 == f1) {
//        if (c.feature2 != f2) cout << "wrong correlated feature of " << f1 << " (-20)" << endl;
//        else {
//            if (c.corrlation < 0.99)
//                cout << f1 << "-" << f2 << " wrong correlation detected (-5)" << endl;
//            if (c.lin_reg.a < a - 0.5f || c.lin_reg.a > a + 0.5f)
//                cout << f1 << "-" << f2 << " wrong value of line_reg.a (-5)" << endl;
//            if (c.lin_reg.b < b - 0.5f || c.lin_reg.b > b + 0.5f)
//                cout << f1 << "-" << f2 << " wrong value of line_reg.b (-5)" << endl;
//            if (c.threshold > 0.3)
//                cout << f1 << "-" << f2 << " wrong threshold detected (-5) " << c.threshold << endl;
//        }
//    }
//}
//
//int main() {
//    srand(time(NULL));
//    for (int k = 0; k < 1000; k++) {
//        float a1 = 1 + rand() % 10, b1 = -50 + rand() % 100;
//        float a2 = 1 + rand() % 20, b2 = -50 + rand() % 100;
//
//        // test the learned model: (40 points)
//        // expected correlations:
//        //	A-C: y=a1*x+b1
//        //	B-D: y=a2*x+b2
//        std::cout << a1 << " " << b1 << " " << a2 << " " << b2 << std::endl;
////        generateTrainCSV(a1, b1, a2, b2);
////        TimeSeries ts("trainFile1.csv");
//        TimeSeries ts("/home/s/CLionProjects/BuildStone/trainFile.csv");
//        //    std::cout<<ts.findIndexOfFeature("C", ts.m_features);
//        //    for (auto & i : ts.m_features)
//        //        std::cout << i << ' ';
//        //    std::cout<<ts.getFieldAt("D","C",341.06);
//        //SimpleAnomalyDetector ad;
//        HybridAnomalyDetector ad;
//        ad.learnNormal(ts);
//        vector<correlatedFeatures> cf = ad.getNormalModel();
//
//        if (cf.size() != 2) {
//            cout << "wrong size of correlated features (-40)" << endl;
//        } else {
//            for_each(cf.begin(), cf.end(), [&a1, &b1, &a2, &b2](correlatedFeatures c) {
//                checkCorrelationTrain(c, "A", "C", a1, b1);// 20 points
//                checkCorrelationTrain(c, "B", "D", a2, b2);// 20 points
//            });
//        }
//
//        // test the anomaly detector: (60 points)
//        // one simply anomaly is injected to the data
//        int anomaly=5+rand()%90; // one anomaly injected in a random time step
////        generateTestCSV(a1, b1, a2, b2, anomaly);
////        TimeSeries ts2("testFile1.csv");
//        TimeSeries ts2("/home/s/CLionProjects/BuildStone/testFile.csv");
//        vector<AnomalyReport> r = ad.detect(ts2);
//
//        bool anomlyDetected = false;
//        int falseAlarms = 0;
//        for_each(r.begin(), r.end(), [&anomaly, &anomlyDetected, &falseAlarms](AnomalyReport ar) {
//            if (ar.description == "A-C" && ar.timeStep == anomaly) anomlyDetected = true;
//            else
//                falseAlarms++;
//        });
//
//        if (!anomlyDetected) {
//            cout << "the anomaly was not detected (-30)" << endl;
//        }
//
//        if (falseAlarms > 0) {
//            cout << "you have " << falseAlarms << " false alarms (-" << min(30, falseAlarms * 3)
//                 << ")" << endl;
//        }
//        cout << "done" << endl;
//    }
//    return 0;
//}
