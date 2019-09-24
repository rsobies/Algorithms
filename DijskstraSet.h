#pragma once
#include "BellmanFordSet.h"

template <typename Cost_t>
using idAndCost_t=tuple<id_t, Cost_t>;

/// <summary>
/// class to hold data related with dijstra algorithm
/// </summary>
/// <typeparm name="Cost_t">type of cost betwean two nodes</typeparm>
template <typename Cost_t>
class DijskstraSet : public BellmanFordSet<Cost_t>
{
public:

	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="size">number of nodes in graph</param>
	DijskstraSet(id_t size);

	/// <summary>
	/// whater the set is empty which means end of iteration
	/// in dijstra algorithm
	/// </summary>
	/// <returns>is set empty</returns>
	bool isEmpty();

	/// <summary>
	/// set new cost of node
	/// </summary>
	/// <param name="id">id of node</param>
	/// <param name="cost">new cost</param>
	void setCost(id_t id, Cost_t cost);

	/// <summary>
	/// set a new cost of node, and previous node
	/// </summary>
	/// <param name="id">id of node</param>
	/// <param name="cost">new cost</param>
	/// <param name="prev">id of previous node in path</param>
	void setCost(id_t id, Cost_t cost, id_t prev);

	/// <summary>
	/// gets node with smallest cost and removes it from releted lists
	/// </summary>
	/// <returns>tuple with id and cost</returns>
	idAndCost_t<Cost_t> pop();

private:
	/// <summary>
	/// set of nodes that was not processed yet
	/// </summary>
	set<id_t> ids;

	/// <summary>
	/// subset of ids list, it holds only nodes with computed cost
	/// this list is used to find node with smallest cost that is still in ids
	/// </summary>
	set<id_t> idWithValue;
};

template<typename Cost_t>
inline DijskstraSet<Cost_t>::DijskstraSet(id_t size) :BellmanFordSet(size)
{
	for (id_t i = 0; i < size; i++) {
		ids.insert(i);
	}
}

template<typename Cost_t>
inline bool DijskstraSet<Cost_t>::isEmpty()
{
	return ids.empty();
}
template<typename Cost_t>
inline void DijskstraSet<Cost_t>::setCost(id_t id, Cost_t cost)
{
	BellmanFordSet::setCost(id, cost);
	//insert if not duplicated
	idWithValue.insert(id);
}

template<typename Cost_t>
inline void DijskstraSet<Cost_t>::setCost(id_t id, Cost_t cost, id_t prev)
{
	BellmanFordSet::setCost(id, cost, prev);
	//insert if not duplicated
	idWithValue.insert(id);
}

template<typename Cost_t>
idAndCost_t<Cost_t> DijskstraSet<Cost_t>::pop()
{
	
	auto compFunc = [this](id_t p, id_t q) {return BellmanFordSet::costs[p] < BellmanFordSet::costs[q]; };

	auto itMin = min_element(idWithValue.begin(), idWithValue.end(), compFunc);

	auto minCostId = *itMin;
	auto minCost = BellmanFordSet::costs[minCostId];

	ids.erase(minCostId);
	idWithValue.erase(itMin);

	return idAndCost_t<Cost_t>(minCostId, minCost);
}
