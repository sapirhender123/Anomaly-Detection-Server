

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "minCircle.h"
#include <algorithm>
#include <map>
#include <math.h>
#include <string.h>
#include <vector>

#define DEFAULT_THRESHOLD (0.9)

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
    Circle minCircle;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> cf;

//    std::map<std::pair<std::string, std::string>, int> m_correlation = {};
protected:
    vector<correlatedFeatures> minCircle_cf;
    float m_threshold;

public:
	SimpleAnomalyDetector();
    SimpleAnomalyDetector(float t);
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries & ts);
	virtual vector<AnomalyReport> detect(const TimeSeries & ts);

	virtual vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
