#pragma once
#include <vector>
#include "State.h"

using namespace std;

class DynamicAbstraction
{
public:
	// Constructor
	DynamicAbstraction();

	// Destructor
	~DynamicAbstraction();

	static vector<State*> BFS(State* startState, vector<vector<int>> weightedLevelVector, vector<State*> stateVector, int regionID);
};