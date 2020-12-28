
#include "timeseries.h"

TimeSeries::TimeSeries(const char *CSVfileName) {
    m_CSVfileName = CSVfileName;
    load_csv();
}

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

    m_fields = std::vector<std::vector<float>>(m_features.size());
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

        // If the next token is a comma, ignore it and move on
        if(ss.peek() == ',') ss.ignore();
    }
}

//
//void TimeSeries::addRow(std::string newLine) {
//
//
//}

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

int TimeSeries::findIndexOfFeature(const std::string& feature, const std::vector<std::string>& vector) const{
    for (int i=0; i < m_features.size(); ++i) {
        auto val_it = std::find(m_features.cbegin(), m_features.cend(), feature);
        int index = -1;
        if (val_it != vector.end()) {
            index = val_it - vector.begin();
        }
        return index;
    }

    return -1;
}

Point** TimeSeries::createPointArray(const std::string& feature1, const std::string& feature2) const {
    // find the index of feature 1 and feature 2
    int index1 = findIndexOfFeature(feature1, m_features);
    int index2 = findIndexOfFeature(feature2, m_features);

    auto points = new Point*[m_fields[0].size()];
    for (int j = 0; j < m_fields[0].size(); j++) {
        points[j] = new Point(m_fields[index1][j], m_fields[index2][j]);
    }

    return points;
}

