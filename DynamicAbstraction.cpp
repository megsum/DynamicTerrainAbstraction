#include "DynamicAbstraction.h"
#include <queue>

// Default constructor
DynamicAbstraction::DynamicAbstraction()
{
}

// Destructor
DynamicAbstraction::~DynamicAbstraction()
{
}

vector<State*> DynamicAbstraction::BFS(State* startState, vector<vector<int>> weightedLevelVector, vector<State*> stateVector, int regionID)
{
	// BTS in original state space
	// Write depth in abstract state space
	// For additive database, only count cost of actions in the pattern
	priority_queue<State*> queue;

	queue.push(startState);

	State* currentState;

	while (!queue.empty())
	{
		// Get states in order of their depth
		// Want to take states from the front and add them to the back of the queue.
		currentState = queue.top();
		queue.pop();

		// Generate successors in the original state space
		// Take the depth of parent + 1 (unit costs). Abstract children
		// If child doesn't exists, store the child
		// If we haven't seen it before, put s in the queue
		currentState->GenerateChildrenBoards(weightedLevelVector);
		for (auto child : currentState->children)
		{
			bool childExists = false;
			for (auto foundState : stateVector)
			{
				if (child->currentLocation == foundState->currentLocation)
				{
					childExists = true;
				}
			}
			if (!childExists)
			{
				stateVector.push_back(child);
				queue.push(child);
			}
		}
	}
	return stateVector;
}

// TODO: Should probably make this add children use the same check as the generate children above
void DynamicAbstraction::AddTerrainEdges(vector<State*> stateVector, vector<vector<int>> weightedLevelVector)
{
	for (auto state : stateVector)
	{
		for (int i = 0; i < stateVector.size(); i++)
		{
			// Left
			vector<int> tempLocation = { state->currentLocation[0] + 1, state->currentLocation[1] };
			if (stateVector[i]->currentLocation == tempLocation)
			{
				if (stateVector[i]->terrainID != state->terrainID)
				{
					state->children.push_back(stateVector[i]);
				}
			}

			// Right
			tempLocation = { state->currentLocation[0] - 1, state->currentLocation[1] };
			if (stateVector[i]->currentLocation == tempLocation)
			{
				if (stateVector[i]->terrainID != state->terrainID)
				{
					state->children.push_back(stateVector[i]);
				}
			}

			// Up
			tempLocation = { state->currentLocation[0], state->currentLocation[1] + 1 };
			if (stateVector[i]->currentLocation == tempLocation)
			{
				if (stateVector[i]->terrainID != state->terrainID)
				{
					state->children.push_back(stateVector[i]);
				}
			}

			// Down
			tempLocation = { state->currentLocation[0], state->currentLocation[1] - 1 };
			if (stateVector[i]->currentLocation == tempLocation)
			{
				if (stateVector[i]->terrainID != state->terrainID)
				{
					state->children.push_back(stateVector[i]);
				}
			}
		}

	}
}
