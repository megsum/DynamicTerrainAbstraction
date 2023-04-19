#pragma once
#include "State.h"

using namespace std;

class AStar
{
public:
	// Constructor
	AStar(vector<int> goalLocation, vector<float> randomTerrainWeights);

	// Destructor
	~AStar();

	double ManhattanDistanceHeuristic(State* state);
	State* Search(State* startState);

	bool CompareFCost(const State* s1, const State* s2);

	vector<int> goalLocation;
	int nodeExpansions;
	vector<float> randomTerrainWeights;

};