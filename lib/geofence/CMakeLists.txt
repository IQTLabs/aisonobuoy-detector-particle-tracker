cmake_minimum_required (VERSION 3.2)
project (geofence-test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_STANDARD 11)

enable_testing()

# Global defines for all tests
add_definitions(-DLOG_DISABLE)
add_definitions(-DRELEASE_BUILD)
add_definitions(-DUNIT_TEST)

if (CMAKE_COMPILER_IS_GNUCXX)
  set(GCOV_ENABLE TRUE)
endif()

if (GCOV_ENABLE)
  set(COVERAGE_LIBRARIES gcov)
  set(COVERAGE_CFLAGS -fno-inline -fprofile-arcs -ftest-coverage -O0 -g)
endif()

include_directories(src/ test/)

add_executable(geofence-test test/test.cpp src/Geofence.cpp test/Particle.cpp)
