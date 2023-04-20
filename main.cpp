// AbstractionWithWeightedCosts.cpp : Defines the entry point for the application.
//

#include "DynamicAbstraction.h"
#include "GridWorld.h"
#include "AStar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Saving states as space delimited. 
void saveStatesToFile(const vector<State*> states, const string filename) {
	ofstream file(filename);
	if (file.is_open()) {
		for (const auto state : states) {
			file << state->currentLocation[0] << "," << state->currentLocation[1]  << "," << state->terrainID << "," << state->regionID << "\n";
		}
		file.close();
		std::cout << "Saved " << states.size() << " states to " << filename << std::endl;
	}
	else {
		std::cerr << "Error: Unable to open file " << filename << std::endl;
	}
}

void PrintPath(State* state)
{
	cout << "Printing path from goal: \n";
	while (state != nullptr)
	{
		if (state == nullptr)
		{
			break;
		}

		cout << "\nNext state: \n";
		cout << "{" << state->currentLocation[0] << "," << state->currentLocation[1] << "}\n|\nV\n";

		state = state->parent;

	}
}

vector<State*> loadStatesFromFile(string filename) {
	vector<State*> states;
	ifstream file(filename);

	string line;

	// Split the line by commas as I am using commas to separate values in the db
	char delimiter = ',';
	while (getline(file, line)) {
		State* state = nullptr;
		string token;
		int elementNum = 0;
		for (auto character : line)
		{
			if (character != delimiter)
			{
				token += character;
			}
			else
			{
				// This was hacked in last minute, but this input reading should be converted to json to make this easier.
				// I modified what I used in TOH project to get something working.
				if (elementNum == 0)
				{
					state->currentLocation[0] = (stoi(token));
					token = "";
				}
				if (elementNum == 1)
				{
					state->currentLocation[1] = (stoi(token));
					token = "";
				}
				if (elementNum == 2)
				{
					state->terrainID = (stoi(token));
					token = "";
				}
				if (elementNum == 3)
				{
					state->regionID = (stoi(token));
					token = "";
				}
			}
		}
		states.push_back(state);
	}
	file.close();
	return states;
}

int main()
{
	vector<State*> stateVector;

	string stateVectorFile = "../../../savedStates.txt";
	ifstream file(stateVectorFile);

	// TODO hardcoding map 1, but should loop through each map here and print the outputs.
	// Initializing grid world with the map and random weights
	GridWorld* gridWorld = new GridWorld("../../../weighted-map/Map1.map");

	// If the states have been saved, don't regenerate them.
	/*if (file.good())
	{
		stateVector = loadStatesFromFile(stateVectorFile);
	}*/
	// Otherwise, generate states and save them to a file.
	if (file.good())
	{
		// Regions are built for abstraction by doing Breadth-first search from each unlabeled state in a sector
		// Limited to states with the same terrain type
		// Label states with their region ID during BFS
		// TODO optimize this. This is such an inefficient way to find the regions. A triple for loop :(
		
		// Tracking time taken to run BFS
		auto bfsTimeStart = steady_clock::now();
		int regionID = 0;
		cout << "Creating regions of areas with the same terrain type.\n";
		for (int i = 0; i < gridWorld->weightedLevelVector.size(); i++)
		{
			for (int j = 0; j < gridWorld->weightedLevelVector[0].size(); j++)
			{
				State* startState = new State({ i, j }, {}, gridWorld->weightedLevelVector[i][j], regionID);
				bool stateExists = false;
				for (auto foundState : stateVector)
				{
					if (startState->currentLocation == foundState->currentLocation)
					{
						stateExists = true;
					}
				}
				if (!stateExists)
				{
					stateVector = DynamicAbstraction::BFS(startState, gridWorld->weightedLevelVector, stateVector, regionID);
					// Once we have iterated through a new state, increment the region ID
					regionID++;
				}
			}
		}
		// End timer
		auto bfsTimeEnd = steady_clock::now();
		auto bfsElapsedTime = duration_cast<milliseconds>(bfsTimeEnd - bfsTimeStart).count() / 1000.0;
		printf("%.2fs taken to generate initial regions using BFS. \n\n", bfsElapsedTime);

		// Tracking time taken to do second pass
		auto edgeTimeStart = steady_clock::now();

		// Do one last iteration to look for neighbors in different regions. Add an edge between them.
		cout << "Doing a second pass to add terrain edges.\n";
		DynamicAbstraction::AddTerrainEdges(stateVector, gridWorld->weightedLevelVector);

		// End timer
		auto edgeTimeEnd = steady_clock::now();
		auto edgeElapsedTime = duration_cast<milliseconds>(edgeTimeEnd - edgeTimeStart).count() / 1000.0;
		printf("%.2fs taken to add terrain edges. \n\n", edgeElapsedTime);

		// After the second pass is done we save it to file so we don't have to generate navmesh again until navmesh updates
		saveStatesToFile(stateVector, stateVectorFile);
	}


	// Initialize state and run A* with abstract graph starting from first region to the region the goal is in
	// TODO eventually load scenarios, but will be using the first one of map 1 for testing.
	//vector<int> startLocation = { 381, 793 };
	//vector<int> goalLocation = { 1724, 889 };
	vector<int> startLocation = { 0, 0 };
	vector<int> goalLocation = { 5, 0 };

	State* startState{};
	for (auto state : stateVector)
	{
		if (state->currentLocation == startLocation)
		{
			startState = state;
			break;
		}
	}
	cout << "Running A Star from start location to next region.\n";

	// Tracking time taken to run A star
	auto aStarTimeStart = steady_clock::now();
	AStar aStar = AStar(goalLocation, gridWorld->randomTerrainWeights);

	// Search until we find a goal state or hit the next terrain type
	auto goalState = aStar.Search(startState);

	// Store A star final nodes for the previous regions
	vector<State*> abstractPath;

	// Found goal don't need to keep iterating through regions
	if (goalState->currentLocation == goalLocation)
	{
		PrintPath(goalState);
	}

	// Run A star again from abstract regions until we find the goal state.
	while (goalState->currentLocation != goalLocation)
	{
		cout << "Running A Star on region  " << goalState->regionID;
		abstractPath.push_back(goalState);
		goalState = aStar.Search(goalState);
	}

	// End timer
	auto aStarTimeEnd = steady_clock::now();
	auto aStarElapsedTime = duration_cast<milliseconds>(aStarTimeEnd - aStarTimeStart).count() / 1000.0;
	printf("%.2fs taken to run A*. \n\n", aStarElapsedTime);

	for (auto state : abstractPath)
	{
		PrintPath(state);
	}
}