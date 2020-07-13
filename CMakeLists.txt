cmake_minimum_required(VERSION 3.0.0)
project(guarani VERSION 0.1.0)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

enable_testing()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(bmp280 STATIC BMP280_driver/bmp280.c)
target_include_directories(bmp280 PRIVATE "BMP280_driver")

add_library(i2ctools STATIC i2c-tools/lib/smbus.c)
target_include_directories(i2ctools PRIVATE "i2c-tools/include")

add_library(lsm9ds1 STATIC STMems_Standard_C_drivers/lsm9ds1_STdC/driver/lsm9ds1_reg.c)

add_executable(guarani  src/main.cpp
                        src/config_ublox.cpp
                        src/serial.cpp
                        src/recorder.cpp src/gps.cpp
                        src/i2c_fd.cpp
                        src/bmp280_t.cpp
                        src/i2c_aquisition.cpp
                        src/lsm9ds1_t.cpp)

target_include_directories(guarani PRIVATE  "include"
                                            "STMems_Standard_C_drivers/lsm9ds1_STdC/driver" 
                                            "BMP280_driver"
                                            "i2c-tools/include")

set_target_properties(guarani PROPERTIES
                          CXX_STANDARD 17
                          CXX_STANDARD_REQUIRED YES
                          CXX_EXTENSIONS NO)

target_link_libraries(guarani PRIVATE   Threads::Threads 
                                        bmp280 
                                        lsm9ds1
                                        i2ctools)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
