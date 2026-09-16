cmake_minimum_required(VERSION 3.28)
project(easy_guess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(easy_guess
        Movie.cpp
        Movie.h
        RecommendationSystem.h
        RecommendationSystemLoader.h
        User.h
        UsersLoader.h
        GameEngine.h
        Files_Gen.h
        User.cpp
        RecommendationSystem.cpp
        RecommendationSystemLoader.cpp
        UsersLoader.cpp
        GameEngine.cpp
        Files_Gen.cpp
        main.cpp
)
