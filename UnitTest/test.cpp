#include "pch.h"
#include "../NodeInPath.h"
#include "../Algorithms.h"
#include "../DijstraSet.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class CrtCheckMemory
{
public:
	_CrtMemState state1;
	_CrtMemState state2;
	_CrtMemState state3;
	CrtCheckMemory()
	{		
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		_CrtMemCheckpoint(&state1);
	}
	~CrtCheckMemory()
	{
		_CrtMemCheckpoint(&state2);

		EXPECT_EQ(0, _CrtMemDifference(&state3, &state1, &state2));

		if (_CrtMemDifference(&state3, &state1, &state2))
			_CrtMemDumpStatistics(&state3);
	}
};

class AlgorithmsUnit: public testing::Test{
public:

private:

	CrtCheckMemory check;
};

TEST_F(AlgorithmsUnit, dijsktraSet) {

	int size = 2;
	DijstraSet<int> dset(size);

	ASSERT_FALSE(dset.isEmpty());

	dset.put(0, 2);
	dset.put(1, 5);
	{
		auto [id, cost] = dset.pop();

		ASSERT_EQ(id, 0);
		ASSERT_EQ(cost, 2);
	}

	{
		auto [id, cost] = dset.pop();

		ASSERT_EQ(id, 1);
		ASSERT_EQ(cost, 5);
	}

	ASSERT_TRUE(dset.isEmpty());

}

TEST_F(AlgorithmsUnit, dijsktra) {
	
	vector<shared_ptr<NodeInPath<int>>> graf(6);

	for (unsigned int i = 0; i < graf.size(); i++) {
		graf[i] = make_shared<NodeInPath<int>>(i);
	}
	
	graf[0]->addNeighbour(graf[1], 3);
	graf[0]->addNeighbour(graf[4], 3);

	graf[1]->addNeighbour(graf[2], 1);

	graf[2]->addNeighbour(graf[5], 1);
	graf[2]->addNeighbour(graf[3], 3);

	graf[4]->addNeighbour(graf[5], 2);

	graf[5]->addNeighbour(graf[0], 6);
	graf[5]->addNeighbour(graf[3], 1);
	
	auto cost=findShortestPath(graf, 0, 3);

	ASSERT_EQ(cost, 6);

	cost = findShortestPath(graf, 1, 0);

	ASSERT_EQ(cost, 8);
}

TEST_F(AlgorithmsUnit, dijsktra2) {

	vector<shared_ptr<NodeInPath<int>>> graf(5);

	for (unsigned int i = 0; i < graf.size(); i++) {
		graf[i] = make_shared<NodeInPath<int>>(i);
	}

	graf[0]->addNeighbour(graf[1], 10);
	graf[0]->addNeighbour(graf[4], 5);

	graf[1]->addNeighbour(graf[2], 1);
	graf[1]->addNeighbour(graf[4], 2);

	graf[2]->addNeighbour(graf[3], 4);

	graf[3]->addNeighbour(graf[0], 7);
	graf[3]->addNeighbour(graf[2], 6);
	
	graf[4]->addNeighbour(graf[1], 3);
	graf[4]->addNeighbour(graf[2], 9);
	graf[4]->addNeighbour(graf[3], 2);

	auto cost = findShortestPath(graf, 0, 3);

	ASSERT_EQ(cost, 7);
}