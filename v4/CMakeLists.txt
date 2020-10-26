cmake_minimum_required(VERSION 3.10)
project(Itimer)

set(CMAKE_CXX_STANDARD 14)

add_executable(Itimer main.cpp TimerTask.h TimerManager.cpp TimerManager.h)