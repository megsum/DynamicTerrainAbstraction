# CMakeList.txt : CMake project for AbstractionWithWeightedCosts, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) 

project ("AbstractionWithWeightedCosts")

# Add source to this project's executable.
add_executable (AbstractionWithWeightedCosts "AbstractionWithWeightedCosts.cpp" "AbstractionWithWeightedCosts.h" "GridWorld.cpp" "GridWorld.h" "WeightedAStar.cpp" "WeightedAStar.h" "DynamicAbstraction.cpp" "DynamicAbstraction.h" "State.cpp" "State.h")

# TODO: Add tests and install targets if needed.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)