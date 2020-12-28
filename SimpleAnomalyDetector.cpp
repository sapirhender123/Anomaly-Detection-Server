
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

#include <iostream>


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    // calculate the correlation between the pairs, and taking the high correlation
    //    for (auto vector1 : ts.m_fields) {
    string featureI;
    string featureJ;
    float threshold = 0.9;
    for (int idx1 = 0; idx1 < ts.m_fields.size(); ++idx1) {
        float maxDev = 0;
        float maxCor = 0;
        featureI = "";
        featureJ = "";
        Line regLine;
        Circle minCircleFromPoints = Circle(Point(0,0), 0);
        for (int idx2 = idx1 + 1; idx2 < ts.m_fields.size(); ++idx2) {
            float arrVec1[ts.m_fields.size()];
            float arrVec2[ts.m_fields.size()];
            for (int i = 0; i < ts.m_fields.size(); i++) {
                // conversion to array in order to use the function pearson (accept float *)
                arrVec1[i] = ts.m_fields[idx1][i];
                arrVec2[i] = ts.m_fields[idx2][i];
            }
            // calculate the pearson between two headers
            float pearsonCor = fabs(pearson(arrVec1, arrVec2, ts.m_fields.size()));

            // taking the highest result for correlation and saving the pair
            if (pearsonCor > maxCor && pearsonCor > 0.5) {
                if (idx1 < idx2) {
                    featureI = ts.m_features[idx1];
                    featureJ = ts.m_features[idx2];
                } else {
                    featureJ = ts.m_features[idx1];
                    featureI = ts.m_features[idx2];
                }

                maxCor = pearsonCor;

                /* convert the value of the vectors to array of values according to the function
                 getFieldAt in Timeseries */
                // create an array of points
                //Point *points[ts.m_fields[0].size()];
                auto **points = new Point*[ts.m_fields[0].size()];
                // check what index is first
                int xCoord = idx1 < idx2 ? idx1 : idx2;
                int yCoord = idx1 < idx2 ? idx2 : idx1;
                // create array of points with the two features
                for (int j = 0; j < ts.m_fields[0].size(); j++) {
                    points[j] = new Point(ts.m_fields[xCoord][j], ts.m_fields[yCoord][j]);
                }

                if (pearsonCor > threshold) {
                    // create linear regression line
                    regLine = linear_reg(points, ts.m_fields[0].size());

                    // calculate the deviation from the regression line
                    for (int i = 0; i < ts.m_fields[0].size(); i++) {
                        float devFromLine = dev(*points[i], regLine);
                        maxDev = devFromLine > maxDev ? devFromLine : maxDev;
                    }
                } else {
                    minCircleFromPoints = findMinCircle(points, ts.m_fields[0].size());
                }
            }
        }

        // Now I have the max result
        if (maxCor > threshold) {
            struct correlatedFeatures cfval = {.feature1 = featureI,
                                               .feature2 = featureJ,
                                               .corrlation = maxCor,
                                               .lin_reg = regLine,
                                               .threshold = maxDev * 1.11f,
                                               .minCircle = Circle(Point(0,0),0)};
            cf.push_back(cfval);
        }
        // If bigger than 0.5, then save in list of mincircle_cf
        else if (maxCor > 0.5) {
            struct correlatedFeatures cf_minCircle = {.feature1 = featureI,
                                                      .feature2 = featureJ,
                                                      .corrlation = maxCor,
                                                      .lin_reg = Line(0,0),
                                                      .threshold = minCircleFromPoints.radius * 1.1f,
                                                      .minCircle = minCircleFromPoints};
            minCircle_cf.push_back(cf_minCircle);
        }
    }
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    //create array of points from the rows of correlation features
    auto listOfReports = std::vector<AnomalyReport>();
    std::string feature1, feature2;
    for (int i = 0; i < cf.size(); ++i) {
        feature1 = cf[i].feature1;
        feature2 = cf[i].feature2;
        Point **points = ts.createPointArray(feature1, feature2);

        // for each point (relevant according to the correlation features),
        // calculate the distance from the regression line
        for (int k = 0; k < ts.m_fields[0].size(); ++k) {
            float devFronRegLin = dev(*points[k], cf[i].lin_reg);
            // if it's bigger than the threshold - add to list of reports
            // this report include const string description, const long timeStep (the row of the detection anomaly;
            if (devFronRegLin > cf[i].threshold) {
                std::string name = feature1 + "-" + feature2;
                AnomalyReport report = AnomalyReport(name, k + 1);
                listOfReports.push_back(report);
            }
        }
    }
    return listOfReports;
}