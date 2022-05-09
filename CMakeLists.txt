cmake_minimum_required(VERSION 3.16)
project(CGSynch_2)
add_compile_options(-Wall -Wextra -O3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

include_directories(AbstractGame CombinatorialGames UI)

add_executable(CGSynch_2
        main.cpp
        AbstractGame/CombinatorialGame/CombinatorialGame.cpp
        AbstractGame/SynchronousGame/SynchronousGame.cpp
        AbstractGame/CombinatorialGame/CombinatorialGameDatabase.cpp
        AbstractGame/CombinatorialGame/DyadicRational.cpp
        AbstractGame/SynchronousGame/SynchronizedGameDatabase.cpp

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

#        UI/CombinatorialGameCommandLine.cpp
#        UI/TreeNodes/AbstractUITreeNode.cpp
#        UI/TreeNodes/UITreeNodeInEquality.cpp
#        UI/TreeNodes/UITreeNodeFunction.cpp UI/TreeNodes/UITreeNodeFunction.h UI/TreeNodes/UITreeNodeFunctions.h UI/TreeNodes/TypeCheckFunctions.h

        UI/Spirit/SpiritParser.cpp UI/Spirit/SpiritParser.h
        UI/Spirit/TreeNodes.cpp UI/Spirit/TreeNodes.h CombinatorialGames/Math/RulesetMathUtil.cpp CombinatorialGames/Math/RulesetMathUtil.h AbstractGame/SynchronousGame/SynchronizedGameUtil.h AbstractGame/SynchronousGame/Rational.cpp AbstractGame/SynchronousGame/Rational.h)