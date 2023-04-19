#include "State.h"

// Default constructor
State::State(vector<int> currentLocation, vector<int> previousLocation, int terrainID, int regionID)
{
	this->currentLocation = currentLocation;
	this->previousLocation = previousLocation;
	this->children = children;

	// Terrain ID of the locations current terrain type
	this->terrainID = terrainID;

	// The abstracted ID of a connected region (Grouped regions of the same type)
	this->regionID = regionID;
}

// Destructor
State::~State()
{
}

// Generate moves of up / down / left / right if possible
// TODO Make work for diagonal moves
void State::GenerateChildrenBoards(vector<vector<int>> weightedLevelVector)
{
	vector<int> tempLocation = currentLocation;

	// Left Child
	tempLocation = { currentLocation[0] + 1, currentLocation[1] };
	if (tempLocation != previousLocation && tempLocation[0] >= 0 && tempLocation[0] < weightedLevelVector.size() && tempLocation[1] >= 0 && tempLocation[1] < weightedLevelVector[1].size())
	{
		State* leftChild = new State(tempLocation, currentLocation, weightedLevelVector[tempLocation[0]][tempLocation[1]], regionID);
		if (leftChild->terrainID == terrainID)
		{
			children.push_back(leftChild);
		}
	}

	// Right Child
	tempLocation = { currentLocation[0] - 1, currentLocation[1] };
	if (tempLocation != previousLocation && tempLocation[0] >= 0 && tempLocation[0] < weightedLevelVector.size() && tempLocation[1] >= 0 && tempLocation[1] < weightedLevelVector[1].size())
	{
		State* rightChild = new State(tempLocation, currentLocation, weightedLevelVector[tempLocation[0]][tempLocation[1]], regionID);
		if (rightChild->terrainID == terrainID)
		{
			children.push_back(rightChild);
		}
	}

	// Up Child
	tempLocation = { currentLocation[0], currentLocation[1] + 1 };
	if (tempLocation != previousLocation && tempLocation[0] >= 0 && tempLocation[0] < weightedLevelVector.size() && tempLocation[1] >= 0 && tempLocation[1] < weightedLevelVector[1].size())
	{
		State* upChild = new State(tempLocation, currentLocation, weightedLevelVector[tempLocation[0]][tempLocation[1]], regionID);
		if (upChild->terrainID == terrainID)
		{
			children.push_back(upChild);
		}
	}

	// Down Child
	tempLocation = { currentLocation[0], currentLocation[1] - 1 };
	if (tempLocation != previousLocation && tempLocation[0] >= 0 && tempLocation[0] < weightedLevelVector.size() && tempLocation[1] >= 0 && tempLocation[1] < weightedLevelVector[1].size())
	{
		State* downChild = new State(tempLocation, currentLocation, weightedLevelVector[tempLocation[0]][tempLocation[1]], regionID);
		if (downChild->terrainID == terrainID)
		{
			children.push_back(downChild);
		}
	}
}