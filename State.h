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

	void GenerateChildrenBoards(vector<vector<int>> weightedLevelVector);

	vector<int> currentLocation;
	vector<int> previousLocation;
	vector<State*> children;

	int regionID;
	int terrainID;
};