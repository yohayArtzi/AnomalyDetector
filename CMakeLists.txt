cmake_minimum_required(VERSION 3.21)
project(Advanced_Programming_1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")


add_executable(Advanced_Programming_1 MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h
        AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h
        timeseries.cpp timeseries.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h
        minCircle.h minCircle.cpp CLI.h CLI.cpp commands.h Server.h Server.cpp)