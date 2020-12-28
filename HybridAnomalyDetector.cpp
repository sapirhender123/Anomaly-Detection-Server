
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    // detect circles -> vec report
    auto listOfReports = std::vector<AnomalyReport>();
    std::string feature1, feature2;
    for (int i = 0; i < minCircle_cf.size(); ++i) {
        feature1 = minCircle_cf[i].feature1;
        feature2 = minCircle_cf[i].feature2;
        Point **points = ts.createPointArray(feature1, feature2);

        // iterate circles and detect points that are out of circle
        for (int k = 0; k < ts.m_fields[0].size(); ++k) {
            float devFromCenter = dist(*points[k], minCircle_cf[i].minCircle.center);
            // if it's bigger than the threshold - add to list of reports
            // this report include const string description, const long timeStep (the row of the detection anomaly;
            if (devFromCenter > minCircle_cf[i].threshold) {
                std::string name = feature1 + "-" + feature2;
                AnomalyReport report = AnomalyReport(name, k + 1);
                listOfReports.push_back(report);
            }
        }
    }

    auto cf_report = SimpleAnomalyDetector::detect(ts);
    std::copy(cf_report.begin(), cf_report.end(), std::back_inserter(listOfReports));

    return listOfReports;
}

vector<correlatedFeatures> HybridAnomalyDetector::getNormalModel() {
    vector<correlatedFeatures> res;
    auto cf = SimpleAnomalyDetector::getNormalModel();
    std::copy(cf.begin(), cf.end(), std::back_inserter(res));
    std::copy(minCircle_cf.begin(), minCircle_cf.end(), std::back_inserter(res));
    return res;
}
