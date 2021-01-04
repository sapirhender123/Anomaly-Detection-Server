

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <iostream>
#include <iterator>

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
    HybridAnomalyDetector(float t);
	~HybridAnomalyDetector() override;
    vector<AnomalyReport> detect(const TimeSeries & ts) override;
    vector<correlatedFeatures> getNormalModel() override;
//    float getThreshold();
//    void setThreshold(float threshold);
};


#endif /* HYBRIDANOMALYDETECTOR_H_ */
