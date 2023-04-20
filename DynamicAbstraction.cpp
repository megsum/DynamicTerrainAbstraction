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
	priority_queue<State*> queue;

	queue.push(startState);
	stateVector.push_back(startState);
	
	State* currentState;

	while (!queue.empty())
	{
		// Want to take states from the front and add them to the back of the queue.
		currentState = queue.top();
		queue.pop();

		// Generate successors
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
