
unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(sdk_folder /home/gui/Projects/sdk/cross-pi-gcc-10.1.0-0)

set(CMAKE_C_COMPILER ${sdk_folder}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${sdk_folder}/bin/arm-linux-gnueabihf-g++)
set(CMAKE_C_FLAGS "-mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard ${COMMON_FLAGS}" CACHE STRING "Flags for Raspberry PI 1 B+ Zero")


SET(CMAKE_FIND_ROOT_PATH ${sdk_folder}/rasp-pi-rootfs)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_definitions(-Wall)
