cmake_minimum_required(VERSION 3.22)

project(low_latency_trading_system
    VERSION 0.1
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 17)  # C++ 17 
set(CMAKE_CXX_STANDARD_REQUIRED ON) # don't silently compile with an older C++ standard.
set(CMAKE_CXX_EXTENSIONS OFF)       # use standard C++, rather than compiler-specific GNU extensions.

add_executable(trading_system 
    src/main.cpp
    )   