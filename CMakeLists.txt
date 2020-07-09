cmake_minimum_required(VERSION 3.0.0)
project(guarani VERSION 0.1.0)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

enable_testing()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(bmp3 STATIC BMP3-Sensor-API/bmp3.c BMP3-Sensor-API/self-test/bmp3_selftest.c)
target_include_directories(bmp3 PRIVATE "BMP3-Sensor-API" "BMP3-Sensor-API/self-test")

add_library(lsm9ds1 STATIC STMems_Standard_C_drivers/lsm9ds1_STdC/driver/lsm9ds1_reg.c)

add_executable(guarani src/main.cpp src/config_ublox.cpp src/serial.cpp
                        src/recorder.cpp src/gps.cpp)

target_include_directories(guarani PRIVATE "include" "STMems_Standard_C_drivers/lsm9ds1_STdC/driver" "BMP3-Sensor-API")

set_target_properties(guarani PROPERTIES
                          CXX_STANDARD 17
                          CXX_STANDARD_REQUIRED YES
                          CXX_EXTENSIONS NO)

#target_compile_features(guarani PRIVATE cxx_std_17)

target_link_libraries(guarani PRIVATE Threads::Threads bmp3 lsm9ds1)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
