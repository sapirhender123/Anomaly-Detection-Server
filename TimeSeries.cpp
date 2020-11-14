#include <algorithm>
#include <fstream>
#include <sstream>  // std::stringstream
#include <stdexcept>// std::runtime_error
#include <string>
#include <utility>// std::pair
#include <vector>

#include "TimeSeries.h"

TimeSeries::TimeSeries(const char *CSVfileName) { m_CSVfileName = CSVfileName; }

void TimeSeries::load_csv() {
    std::ifstream myFile(m_CSVfileName);
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");
    std::string line, colname;
    // First, read header names into m_features
    if (myFile.good()) {
        // Extract the first line in the file
        std::getline(myFile, line);
        // Create a string stream from line
        std::stringstream ss(line);
        // Extract each column name
        while (std::getline(ss, colname, ',')) { m_features.push_back(colname); }
    }

    while (std::getline(myFile, line)) { parseRow(line); }
    myFile.close();
}

//void TimeSeries::parseRow(const std::string& newLine) {
//    // add a new line to the TimeSeries vector
//    std::stringstream ss(newLine);
//    float time, val;
//    // parse the first column to be the time
//    ss >> time;
//    // create vector for the rest of the file
//    auto current = std::vector<std::pair<std::string, float>>();
//    // begin from the 1st column because the 0st column was for the features
//    int pos = 1;
//    // parse thr rest of the file according to the features
//    while (ss >> val) {
//        current.emplace_back(m_features.at(pos), val);
//        pos++;
//    }
//}

void TimeSeries::parseRow(const std::string &newLine) {
    // add a new line to the TimeSeries vector
    float val;
    std::stringstream ss(newLine);
    for (auto &m_field : m_fields) {
        ss >> val;
        m_field.push_back(val);
    }
}

// Return value at searched when at equals val
float TimeSeries::getFieldAt(const std::string &searched, const std::string &at, float val) {
    int searched_idx = 0, at_idx = 0;
    for (const auto &it : m_features) {
        if (it == searched) { break; }
        searched_idx++;
    }

    for (const auto &it : m_features) {
        if (it == at) { break; }
        at_idx++;
    }

    auto val_it = std::find(m_fields[searched_idx].cbegin(), m_fields[searched_idx].cend(), val);
    return m_fields[at_idx][val_it - m_fields[searched_idx].cbegin()];
}