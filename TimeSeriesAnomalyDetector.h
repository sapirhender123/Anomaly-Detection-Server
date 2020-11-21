//
// Created by s on 14/11/2020.
//

#ifndef BUILDSTONE1_TIMESERIESANOMALYDETECTOR_H
#define BUILDSTONE1_TIMESERIESANOMALYDETECTOR_H


#include "timeseries.h"
#include <vector>

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector();
};

#endif//BUILDSTONE1_TIMESERIESANOMALYDETECTOR_H
