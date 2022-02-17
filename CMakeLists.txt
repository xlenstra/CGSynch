cmake_minimum_required(VERSION 3.19)
project(CGSynch_2)
add_compile_options(-Wall -Wextra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

add_executable(CGSynch_2
        main.cpp
        AbstractGame/CombinatorialGame.cpp
        AbstractGame/SynchronousGame.cpp
        AbstractGame/CombinatorialGame.h
        AbstractGame/SynchronousGame.h
        AbstractGame/CombinatorialGameDatabase.cpp
        AbstractGame/CombinatorialGameDatabase.h
        AbstractGame/util.h
        CombinatorialRuleset/RulesetMath.h
        CombinatorialRuleset/RulesetMath.cpp
        CombinatorialRuleset/AbstractRuleset.h
        CombinatorialRuleset/RulesetDatabase.h
        CombinatorialRuleset/RulesetDatabase.cpp
    )