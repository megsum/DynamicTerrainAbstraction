#include "AStar.h"
#include <queue>

// Default constructor
AStar::AStar(vector<int> goalLocation, vector<float> randomTerrainWeights)
{
	this->goalLocation = goalLocation;
	this->randomTerrainWeights;
}

// Destructor
AStar::~AStar()
{
}

// Find the distance to the goal
// TODO make a generic heuristic instead of hardcoding Manhattan distance
double AStar::ManhattanDistanceHeuristic(State* state)
{
	return abs(goalLocation[0] - state->currentLocation[0]) + abs(goalLocation[1] - state->currentLocation[1]);
}

// Custom compare for the priority queue
// put lower f cost first, then lower g cost if f cost is a tie
// Source: https://www.geeksforgeeks.org/custom-comparator-in-priority_queue-in-cpp-stl/
class Compare {
public:
	bool operator()(State* a, State* b)
	{
		if (a->fCost == b->fCost)
		{
			// If the f-costs are equal, break ties using the g-costs.
			return a->gCost > b->gCost;
		}
		else
		{
			// Otherwise, order by f-cost.
			return a->fCost > b->fCost;
		}
	}
};

// A* implementation from https://www.geeksforgeeks.org/a-search-algorithm/
State* AStar::Search(State* startState)
{
	vector<State*> openList;
	//priority_queue<State*, vector<State*>, Compare> openQueue;
	vector<State*> closedList;

	// Need to update the h cost of the start state since we aren't doing that by default.
	// Set the start state to visited
	// Weighting the h cost with the random terrain weights we generated
	startState->hCost = ManhattanDistanceHeuristic(startState) * randomTerrainWeights[startState->terrainID];
	startState->gCost = 0;
	startState->fCost = startState->hCost + startState->gCost;
	openList.push_back(startState);

	// Set current state to start state
	State* currentState;

	while (!openList.empty())
	{
		// take the back queue item and set it to our current state and then remove from the queue
		currentState = openList.back();
		openList.pop_back();

		// Generate successors
		for (auto child : currentState->children)
		{
			// Generate successor costs
			child->hCost = ManhattanDistanceHeuristic(child) * randomTerrainWeights[child->terrainID];
			child->gCost = currentState->gCost + 1;
			child->fCost = child->gCost + child->hCost;

			nodeExpansions += 1;
			if (child->currentLocation == goalLocation || child->terrainID != currentState->terrainID)
			{
				// return path
				printf("Solution found with an f cost of %f\n", child->fCost);
				return child;
			}

			// Don't add to open queue if we've already visited this state.
			// Add the new child in sorted f cost order

			for (auto it = openList.begin(); it != openList.end(); ++it) {
				auto state = *it;
				if (state->currentLocation == child->currentLocation) {
					if (child->fCost < state->fCost) {
						*it = child;
					}
					break;
				}
			}

			bool stateFound = false;
			for (auto it = closedList.begin(); it != closedList.end(); ++it) {
				auto state = *it;
				if (state->currentLocation == child->currentLocation) {
					if (state->fCost < child->fCost) {
						*it = child;
						stateFound = true;
					}
					break;
				}
			}

			if (!stateFound) {
				child->parent = currentState;
				openList.push_back(child);
			}
		}

		closedList.push_back(currentState);
	}
	printf("No solution found\n");
	return nullptr;
}

bool AStar::CompareFCost(const State* s1, const State* s2) {
	return s1->fCost < s2->fCost;
}