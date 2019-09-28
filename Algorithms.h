#pragma once
#include "NodeInPath.h"
#include "DijskstraSet.h"
#include "ThreadPool.h"

/// <summary>
/// finds shortes path in graph using dijstra algorithm
/// </summary>
/// <param name="graph">definition of graph</param>
/// <param name="startNodeId">starting node</param>
/// <param name="endNodeId">last node in searching path</param>
/// <typeparm name="Cost_t">must be numeric type, type of cost betwean two nodes</typeparm>
/// <returns>tuple: shortest path(deque) and cost of the path</returns>
template <typename Cost_t>
auto dijstraShortestPath(const vector<shared_ptr<NodeInPath<Cost_t>>>& graph,
	id_t startNodeId, id_t endNodeId)
{
	static_assert(is_arithmetic<Cost_t>::value, "type T must be arithmetic");

	DijskstraSet<Cost_t> dijstraSet(graph.size());

	dijstraSet.setCost(startNodeId, 0);
	
	while (!dijstraSet.isEmpty()) {

		const auto& [processNodeId, cost] = dijstraSet.pop();
		const auto& neigbours = graph[processNodeId]->getNeighbours();

		for (const auto& neigbourAndCost : neigbours) {

			const auto& [weakNeighbour, neigbourCost] = neigbourAndCost;

			assert(neigbourCost >= 0);

			auto neighbour = weakNeighbour.lock();
			if (neighbour != nullptr) {
				auto newNeigbourCost = cost + neigbourCost;

				auto neigbourId = neighbour->getId();
				if (dijstraSet.getCost(neigbourId) > newNeigbourCost) {
					dijstraSet.setCost(neigbourId, newNeigbourCost, processNodeId);
				}
			}
		}
	}
	auto path=dijstraSet.getPath(endNodeId);
	auto minCost = dijstraSet.getCost(endNodeId);

	return tuple<decltype(path), decltype(minCost)>(path, minCost);
}

/// <summary>
///  recursive function used only in bellman-ford algorithm
/// </summary>
/// <param name="bellSet"></param>
/// <param name="processedNode"></param>
/// <param name="threadPool"></param>
/// <typeparm name="Cost_t">must be numeric type, type of cost betwean two nodes</typeparm>
///<remarks>recursion uses threads, todo: use thread pool</remarks>
template <typename Cost_t>
void processNode(ThreadPool& threadPool, BellmanFordSet<Cost_t>& bellSet, shared_ptr<NodeInPath<Cost_t>> processedNode) {

	const auto& neigbours = processedNode->getNeighbours();
	auto processNodeId = processedNode->getId();

	//this node will be processed in the current thread
	shared_ptr<NodeInPath<Cost_t>> firstNeigbour;

	deque <future<void>> pendingTasks;

	for (const auto& neigbourAndCost : neigbours) {

		const auto& [weakNeighbour, edgeCost] = neigbourAndCost;

		auto neighbour = weakNeighbour.lock();
		
		if (neighbour != nullptr) {
			auto newNeigbourCost = bellSet.getCost(processNodeId) + edgeCost;
			
			assert(newNeigbourCost >= 0);

			auto neigbourId = neighbour->getId();

			if (bellSet.getCost(neigbourId) > newNeigbourCost) {
				bellSet.setCost(neigbourId, newNeigbourCost, processNodeId);

				if (firstNeigbour == nullptr) {
					firstNeigbour = neighbour;
				}
				else {
					
					pendingTasks.push_back(threadPool.submit(
						[&threadPool, &bellSet, &neighbour]()
						{
							processNode(threadPool, bellSet, neighbour);
						}
					));
					/*
					pendingTasks.push_back(async(launch::async,
						[&bellSet, &neighbour]() 
						{
							processNode(bellSet, neighbour);
						}));
					*/
				}
			}
		}
	}

	for (const auto& task : pendingTasks) {
		task.wait();
	}

	//and now process the first neigbour
	if (firstNeigbour != nullptr) {
		processNode(threadPool, bellSet, firstNeigbour);
	}
}

/// <summary>
/// finds shortes path in graph using bellman-ford algorithm
/// </summary>
/// <param name="graph">definition of graph</param>
/// <param name="startNodeId">starting node in the path</param>
/// <param name="endNodeId">last node in searching path</param>
/// <typeparm name="Cost_t">must be numeric type, type of cost betwean two nodes</typeparm>
/// <returns>tuple: shortest path(deque) and cost of the path</returns>
template <typename Cost_t>
auto bellmanFordShortestPath(const vector<shared_ptr<NodeInPath<Cost_t>>>& graph,
	id_t startNodeId, id_t endNodeId) {
	
	static_assert(is_arithmetic<Cost_t>::value, "type T must be arithmetic");
	
	BellmanFordSet<Cost_t> bellFordSet(graph.size());
	ThreadPool threadPool;

	bellFordSet.setCost(startNodeId, 0);
	processNode(threadPool, bellFordSet, graph[startNodeId]);
	
	auto path = bellFordSet.getPath(endNodeId);
	auto minCost = bellFordSet.getCost(endNodeId);

	return tuple<decltype(path), decltype(minCost)>(path, minCost);
}
