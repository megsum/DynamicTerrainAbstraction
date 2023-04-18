#pragma once
#include <vector>

using namespace std;

class State
{
public:
	// Constructor
	State(vector<int> currentLocation, vector<int> previousLocation, int terrainID, int regionID);

	// Destructor
	~State();

	vector<State*> GenerateChildrenBoards(vector<vector<int>> weightedLevelVector, int regionID);

	vector<int> currentLocation;
	vector<int> previousLocation;
	vector<State*> children;

	int regionID;
	int terrainID;
};