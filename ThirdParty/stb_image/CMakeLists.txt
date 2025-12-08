cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)

project(stb_image LANGUAGES CXX)

add_library(stb_image STATIC stb_image/stb_image.cpp stb_image/stb_image.h)