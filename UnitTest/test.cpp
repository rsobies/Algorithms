#include "pch.h"
#include "../Algorithms.h"
#include "../ThreadPool.h"
#include <algorithm> 
#include "MemoryLeakDetector.h"


class AlgorithmsUnit: public testing::Test{
public:

private:
	/// <summary>
	/// memoryCheck is created at the beginnig of each test method
	/// and is destroyed at the end of test method
	/// </summary>
	MemoryLeakDetector memoryCheck;
};

TEST_F(AlgorithmsUnit, dijsktraSet) {

	int size = 2;
	DijskstraSet<int> dset(size);

	ASSERT_FALSE(dset.isEmpty());

	dset.setCost(0, 2);
	dset.setCost(1, 5);
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

TEST_F(AlgorithmsUnit, dijkstra) {
	
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
	{
		const auto& [path, cost] = dijstraShortestPath(graf, 0, 3);

		ASSERT_EQ(cost, 6);

		ASSERT_EQ(path.size(), 4);

		ASSERT_EQ(path[0], 0);
		ASSERT_EQ(path[1], 4);
	}

	const auto& [path, cost] = dijstraShortestPath(graf, 1, 0);

	ASSERT_EQ(cost, 8);

	ASSERT_EQ(path.size(), 4);

	ASSERT_EQ(path[0], 1);
	ASSERT_EQ(path[1], 2);
	ASSERT_EQ(path[2], 5);
	ASSERT_EQ(path[3], 0);
}

TEST_F(AlgorithmsUnit, dijkstra2) {

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

	const auto& [path, cost] = dijstraShortestPath(graf, 0, 3);

	ASSERT_EQ(cost, 7);

	ASSERT_EQ(path.size(), 3);

	ASSERT_EQ(path[0], 0);
	ASSERT_EQ(path[1], 4);
	ASSERT_EQ(path[2], 3);
}

TEST_F(AlgorithmsUnit, bellmanford) {
	vector<shared_ptr<NodeInPath<int>>> graf(6);

	for (unsigned int i = 0; i < graf.size(); i++) {
		graf[i] = make_shared<NodeInPath<int>>(i);
	}

	graf[0]->addNeighbour(graf[1], 5);
	
	graf[1]->addNeighbour(graf[4], 9);
	graf[1]->addNeighbour(graf[3], 3);

	graf[2]->addNeighbour(graf[0], 4);
	graf[2]->addNeighbour(graf[1], -4);

	graf[3]->addNeighbour(graf[4], 3);
	graf[3]->addNeighbour(graf[5], 2);

	graf[4]->addNeighbour(graf[2], -1);
	graf[4]->addNeighbour(graf[5], -5);

	graf[5]->addNeighbour(graf[0], 9);
	graf[5]->addNeighbour(graf[2], 8);

	const auto& [path, cost] = bellmanFordShortestPath(graf, 0, 3);

	ASSERT_EQ(cost, 8);

	ASSERT_EQ(path.size(), 3);

	ASSERT_EQ(path[0], 0);
	ASSERT_EQ(path[1], 1);
	ASSERT_EQ(path[2], 3);
}

TEST_F(AlgorithmsUnit, blockingqueue) {
	BlockingQueue<int> myq;

	myq.push(2);
	myq.push(3);

	ASSERT_EQ(myq.pop(), 2);
	ASSERT_EQ(myq.pop(), 3);
	
}


TEST_F(AlgorithmsUnit, threadpool) {
	ThreadPool pool;
	
		deque<future<void>> tasks;
		for (int i = 0; i < 10; i++) {
			tasks.push_back(pool.submit([i]() {
				cout << "in the thread " << i << endl;
				this_thread::sleep_for(chrono::milliseconds(800));
				}));
		}

		for (auto& task : tasks) {
			task.wait();
		}
	
}