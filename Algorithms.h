#pragma once
#include "NodeInPath.h"
#include "DijstraSet.h"

/// <summary>
/// finds shortes path in graph using dijstra algorithm
/// </summary>
/// <param name="graph">definition of graph</param>
/// <param name="startNodeId">starting node</param>
/// <param name="endNodeId">last node in searching path</param>
/// <returns>smallest cost of found path</returns>
template <typename T>
auto findShortestPath(const vector<shared_ptr<NodeInPath<T>>>& graph,
	id_t startNodeId, id_t endNodeId)
{
	static_assert(is_arithmetic<T>::value, "type T must be arithmetic");

	DijstraSet<T> dijstraSet(graph.size());

	dijstraSet.setCost(startNodeId, 0);
	
	while (!dijstraSet.isEmpty()) {

		const auto& [processNodeId, cost] = dijstraSet.pop();
		const auto& neigbours = graph[processNodeId]->getNeighbours();

		for (const auto& neigbourAndCost : neigbours) {

			const auto& [weakNeighbour, neigbourCost] = neigbourAndCost;

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

	return dijstraSet.getCost(endNodeId);
}