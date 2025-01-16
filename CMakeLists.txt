cmake_minimum_required(VERSION 3.6.0)
project(xmlParser)

add_executable(xmlParser src/main.cpp src/xml.cpp src/xml.hpp)

set_property(TARGET xmlParser PROPERTY CXX_STANDARD 17)

