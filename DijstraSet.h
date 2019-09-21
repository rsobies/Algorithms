#pragma once
template <typename T>
using idAndCost_t=tuple<id_t, T>;

/// <summary>
/// class to hold data related with dijstra algorithm
/// </summary>
template <typename T>
class DijstraSet
{
	
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="size">number of nodes in graph</param>
	DijstraSet(id_t size);

	/// <summary>
	/// set new cost of node
	/// </summary>
	/// <param name="id">id of node</param>
	/// <param name="cost">new cost</param>
	void setCost(id_t id, T cost);

	/// <summary>
	/// set a new cost of node, and previous node
	/// </summary>
	/// <param name="id">id of node</param>
	/// <param name="cost">new cost</param>
	/// <param name="prev">id of previous node in path</param>
	void setCost(id_t id, T cost, id_t prev);

	/// <summary>
	/// returns cost of node
	/// </summary>
	/// <param name="id">selected node</param>
	/// <returns>cost of node</returns>
	T getCost(id_t id);

	/// <summary>
	/// get node with smallest cost
	/// </summary>
	/// <returns>tuple with id and cost</returns>
	idAndCost_t<T> pop();

	/// <summary>
	/// whater the set is empty which means end of iteration
	/// in dijstra algorithm
	/// </summary>
	/// <returns>is set empty</returns>
	bool isEmpty();

private:
	
	/// <summary>
	/// set of nodes that was not processed yet
	/// </summary>
	list<id_t> ids;

	/// <summary>
	/// set of all nodes with cost
	/// </summary>
	unordered_map<id_t, T> costs;

	/// <summary>
	/// path from start node to random node
	/// </summary>
	unordered_map<id_t, id_t> prevNodes;

	/// <summary>
	/// subset of ids list, it holds only nodes with computed cost
	/// this list is used to find node with smallest cost in costs map
	/// </summary>
	set<id_t> idWithValue;
};

template<typename T>
inline DijstraSet<T>::DijstraSet(id_t size)
{
	for (id_t i = 0; i < size; i++) {
		ids.push_back(i);
		costs[i] = numeric_limits<T>::max();
	}
}

template<typename T>
inline void DijstraSet<T>::setCost(id_t id, T cost)
{
	assert(id < costs.size());
	costs[id] = cost;
	//insert if not duplicated
	idWithValue.insert(id);
}

template<typename T>
inline void DijstraSet<T>::setCost(id_t id, T cost, id_t prev)
{
	setCost(id, cost);
	prevNodes[id] = prev;
}

template<typename T>
inline T DijstraSet<T>::getCost(id_t id)
{
	return costs[id];
}

template<typename T>
inline idAndCost_t<T> DijstraSet<T>::pop()
{	
	auto minCostId = *idWithValue.begin();
	auto minCost = costs[minCostId];

	for (auto id : idWithValue) {
		if (costs[id] < minCost) {
			minCost = costs[id];
			minCostId = id;
		}
	}

	ids.remove(minCostId);
	idWithValue.erase(idWithValue.find(minCostId));

	return idAndCost_t<T>(minCostId, minCost);
}

template<typename T>
inline bool DijstraSet<T>::isEmpty()
{
	return ids.empty();
}
