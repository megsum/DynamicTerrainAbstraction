#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class GridWorld
{
public:
	// Constructor
	GridWorld(string MapLocation, vector<int> goalLocation);

	// Destructor
	~GridWorld();

	// Random terrain weights from A-Z
	vector<float> randomTerrainWeights;
	vector<vector<int>> weightedLevelVector;
	vector<int> goalLocation;

	vector<vector<int>> LoadMap(string filename);
	vector<float> randomizeTerrainWeights();
};