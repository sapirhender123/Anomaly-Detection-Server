

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <iostream>
#include <iterator>

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	~HybridAnomalyDetector() override;
    vector<AnomalyReport> detect(const TimeSeries & ts) override;
    vector<correlatedFeatures> getNormalModel() override;
};


#endif /* HYBRIDANOMALYDETECTOR_H_ */
