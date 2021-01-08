cmake_minimum_required(VERSION 3.0.0)
project(BuildStone1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(BuildStone1 MainTrainEX6.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp minCircle.h minCircle.cpp HybridAnomalyDetector.h HybridAnomalyDetector.cpp CLI.cpp commands.h Server.cpp)
target_link_libraries(BuildStone1 ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} pthread)

set(CXX_STANDARD11)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
