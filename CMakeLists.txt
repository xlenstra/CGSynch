cmake_minimum_required(VERSION 3.16)

project(CGSynch)

add_compile_options(-Wall -Wextra -O3)

option(CXX "enable C++ compilation" ON)

if(CXX)
    enable_language(CXX)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

message(STATUS "Gurobi dir: $ENV{GUROBI_HOME}")

find_package(GUROBI REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})
include_directories(AbstractGame CombinatorialGames UI)

# list source files here
set(sources main.cpp
    AbstractGame/CombinatorialGame/CombinatorialGame.cpp
    AbstractGame/CombinatorialGame/CombinatorialGameDatabase.cpp
    AbstractGame/CombinatorialGame/DyadicRational.cpp
    AbstractGame/SynchronousGame/SynchronousGame.cpp
    AbstractGame/SynchronousGame/SynchronousGame.h
    AbstractGame/SynchronousGame/SynchronizedGameDatabase.cpp
    AbstractGame/SynchronousGame/Rational.cpp
    AbstractGame/SynchronousGame/GurobiSolver.cpp

    CombinatorialGames/RulesetUtil.cpp
    CombinatorialGames/Cherries/CherriesUtil.cpp
    CombinatorialGames/Cherries/Cherries.cpp
    CombinatorialGames/Cherries/StackCherries.cpp
    CombinatorialGames/Push-Shove/PushShoveUtil.cpp
    CombinatorialGames/Push-Shove/Push.cpp
    CombinatorialGames/Push-Shove/Shove.cpp
    CombinatorialGames/Math/RulesetMath.cpp
    CombinatorialGames/Hackenbush/NormalGraph.cpp
    CombinatorialGames/Hackenbush/Hackenbush.cpp
    CombinatorialGames/Math/RulesetMathUtil.cpp

    UI/Spirit/SpiritParser.cpp
    UI/Spirit/TreeNodes.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${sources})

if(CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
    target_link_libraries(${CMAKE_PROJECT_NAME} optimized ${GUROBI_CXX_LIBRARY}
            debug ${GUROBI_CXX_DEBUG_LIBRARY})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} ${GUROBI_LIBRARY})

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    include(FeatureSummary)
    feature_summary(WHAT ALL)
endif()