cmake_minimum_required(VERSION 3.0)

project(mVM)

set(CMAKE_CXX_STANDARD 17)

set(HEADER_FILES
    ./src/include/mVM.h
    ./src/include/byteCode.h
    ./src/include/parser.h
    ./src/include/macroBase.h
)

set(SOURCE_FILES
    ./src/mVM.cpp
    ./src/main.cpp
    ./src/parser.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})