#include "AStar.h"
#include <queue>
#include <unordered_set>

// Default constructor
AStar::AStar(vector<int> goalLocation, vector<float> randomTerrainWeights)
{
	this->goalLocation = goalLocation;
	this->randomTerrainWeights = randomTerrainWeights;
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

// I'm new to hashes, but maybe this is unique?
struct StateHash {
    size_t operator()(const State* state) const {
        return hash<long long>()(state->currentLocation[0]) ^ (state->currentLocation[1]);
    }
};

struct StateEqual {
    bool operator()(const State* state1, const State* state2) const {
        return state1->currentLocation == state2->currentLocation;
    }
};

// https://en.wikipedia.org/wiki/A*_search_algorithm
// Too bad I didn't figured out unordered sets for assignment 2 :( 
State* AStar::Search(State* startState)
{
    priority_queue<State*, vector<State*>, Compare> openQueue;
    unordered_set<State*, StateHash, StateEqual> closedSet = { startState };

    // Weighting hCost with the definied random weights
    startState->hCost = ManhattanDistanceHeuristic(startState) * randomTerrainWeights[startState->terrainID];
    startState->gCost = 0;
    startState->fCost = startState->hCost + startState->gCost;
    openQueue.push(startState);

    State* currentState;

    while (!openQueue.empty())
    {
        currentState = openQueue.top();
        openQueue.pop();
        // closedSet.insert(currentState);

        // Iterate through successors
        for (auto child : currentState->children)
        {
            child->hCost = ManhattanDistanceHeuristic(child) * randomTerrainWeights[child->terrainID];
            child->gCost = currentState->gCost + 1;
            child->fCost = child->gCost + child->hCost;
            child->parent = currentState;

            nodeExpansions += 1;

            // If we hit the goal or we found a new terrain type, exit and iterate on the new terrain type
            if (child->currentLocation == goalLocation || child->terrainID != startState->terrainID)
            {
                printf("\nSolution found with an f cost of %f and node expansions of %d \n", child->fCost, nodeExpansions);
                return child;
            }

            // If the child is not in the closed set, add it to the open queue.
            // Couldn't get the closed set working in time.
            //if (closedSet.empty())
            //{
            //    openQueue.push(child);
            //}
            //else if (closedSet.count(child) == 0)
            //{
            openQueue.push(child);
            //}
        }
    }

    printf("No solution found.\n");
    return nullptr;
}