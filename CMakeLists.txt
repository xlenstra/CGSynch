cmake_minimum_required(VERSION 3.19)
project(CGSynch_2)
add_compile_options(-Wall -Wextra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

add_executable(CGSynch_2
        main.cpp
        AbstractGame/CombinatorialGame.h
        AbstractGame/CombinatorialGame.cpp
        AbstractGame/SynchronousGame.h
        AbstractGame/SynchronousGame.cpp
        AbstractGame/CombinatorialGameDatabase.h
        AbstractGame/CombinatorialGameDatabase.cpp
        AbstractGame/DyadicRational.h
        AbstractGame/DyadicRational.cpp
        CombinatorialGames/AbstractGame.h
        CombinatorialGames/GameDatabase.h
        CombinatorialGames/RulesetUtil.h
        CombinatorialGames/CherriesUtil.h
        CombinatorialGames/CherriesUtil.cpp
        CombinatorialGames/Cherries.h
        CombinatorialGames/Cherries.cpp
        CombinatorialGames/StackCherries.h
        CombinatorialGames/StackCherries.cpp
        CombinatorialGames/PushShoveUtil.h
        CombinatorialGames/PushShoveUtil.cpp
        CombinatorialGames/Push.h
        CombinatorialGames/Push.cpp
        CombinatorialGames/RulesetMath.h
        CombinatorialGames/RulesetMath.cpp
        )