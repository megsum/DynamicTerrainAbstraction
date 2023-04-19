#include "GridWorld.h"
using namespace std;


// Default constructor
GridWorld::GridWorld(string MapLocation)
{
	this->goalLocation = goalLocation;

	// Initialize the random weights
	randomTerrainWeights = randomizeTerrainWeights();

	weightedLevelVector = LoadMap(MapLocation);
}

// Destructor
GridWorld::~GridWorld()
{
}

// Read the map files into a list of lists with the terrain letter converted into the weight of that location
// Use random weights for each terrain letter.
vector<vector<int>> GridWorld::LoadMap(string filename)
{
	vector<vector<int>> levelVector;
	ifstream mapFile(filename);

	string line;
	int count = 0;
	while (getline(mapFile, line))
	{
		// Split the line by each new character
		vector<int> currentVals = {};
		// Hacky way to skip the first 3 lines as they are metadata
		if (count > 3 && count <= 10)
		{
			for (auto character : line)
			{
				// Adding the random weight associated with the terrain type
				auto weight = tolower(character) - 'a';
				currentVals.push_back((int)weight);
			}

			levelVector.push_back(currentVals);
		}
		count++;
	}

	return levelVector;
}

vector<float> GridWorld::randomizeTerrainWeights()
{
	// Setting the seed to zero for consistency
	std::srand(0);
	std::vector<float> tempVector(26);

	// Loop through the vector and assign a random value to each element
	// TODO using random ints for simplicity, but should convert to float later on.
	for (int i = 0; i < tempVector.size(); i++) {
		tempVector[i] = std::rand() % 5 + 1; // generate a random number between 1 and 5
	}

	return tempVector;
}
