project(time-tracker)

#**************************************************************************************************
# General cMake settings **************************************************************************
cmake_minimum_required(VERSION 3.5)

#**************************************************************************************************
# Find Package ************************************************************************************
find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(replxx CONFIG REQUIRED)

#**************************************************************************************************
# Include *****************************************************************************************
include_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third-party)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third-party/cxxsubs/include)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

#**************************************************************************************************
# Set compiler ************************************************************************************
set(CMAKE_CXX_FLAGS "-Wall -fPIC -fpermissive -std=c++17")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


#**************************************************************************************************
# Include *****************************************************************************************



#**************************************************************************************************
# Set variable ************************************************************************************
SET(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

#**************************************************************************************************
# Make configuration ******************************************************************************
add_executable(timetracker ${SOURCES})
set_property(TARGET timetracker PROPERTY CXX_STANDARD 17)
target_link_libraries(timetracker -pthread -lstdc++fs fmt::fmt REPLXX::replxx)
