#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include "anomaly_detection_util.h"
#include <algorithm>
#include <fstream>
#include <sstream>  // std::stringstream
#include <stdexcept>// std::runtime_error
#include <string>
#include <utility>// std::pair
#include <vector>


class TimeSeries {

public:
    const char *m_CSVfileName = nullptr;
    std::vector<std::string> m_features = std::vector<std::string>();
    std::vector<std::vector<float>> m_fields = std::vector<std::vector<float>>();

    explicit TimeSeries(const char *CSVfileName);
    void load_csv();
    void parseRow(const std::string& newLine);
    float getFieldAt(const std::string &searched, const std::string &at, float val);
    int findIndexOfFeature(const std::string& feature, const std::vector<std::string>&) const;
    Point **createPointArray(const std::string& feature1, const std::string& feature2) const;
};

#endif /* TIMESERIES_H_ */
