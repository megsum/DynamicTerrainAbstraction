// AbstractionWithWeightedCosts.cpp : Defines the entry point for the application.
//

#include "DynamicAbstraction.h"
#include "GridWorld.h"

using namespace std;



int main()
{
	// TODO hardcoding map 1, but should loop through each map here and print the outputs.
	// Initializing grid world with the map and random weights
	vector<int> goalLocation = { 1724, 889 };
	GridWorld* gridWorld = new GridWorld("../../..//weighted-map/Map1.map", goalLocation);

	vector<State*> stateVector;

	// Regions are built for abstraction by doing Breadth-first search from each unlabeled state in a sector
	// Limited to states with the same terrain type
	// Label states with their region ID during BFS
	// TODO optimize this. This is such an inefficient way to find the regions. A triple for loop :(
	int regionID = 0;
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

	// Do one last iteration to look for neighbors in different regions. Add an edge between them

	// Initialize state and run A* with abstract graph
	// TODO eventually load scenarios, but will be using the first one of map 1 for testing.
	// State* startState = new State({ 381, 793 }, {}, gridWorld->weightedLevelVector[381][793], 0);

}
