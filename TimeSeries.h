#ifndef TIMESERIES_H_
#define TIMESERIES_H_

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
//    std::vector<std::pair<float, std::vector<std::pair<std::string, float>>>> m_fields =
//            std::vector<std::pair<float, std::vector<std::pair<std::string, float>>>>();

    std::vector<std::vector<float>> m_fields = std::vector<std::vector<float>>();

    explicit TimeSeries(const char *CSVfileName);
    void load_csv();
    void parseRow(const std::string& newLine);
    float getFieldAt(const std::string &searched, const std::string &at, float val);
};

#endif /* TIMESERIES_H_ */
