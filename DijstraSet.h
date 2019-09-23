#pragma once
template <typename Cost_t>
using idAndCost_t=tuple<id_t, Cost_t>;

/// <summary>
/// class to hold data related with dijstra algorithm
/// </summary>
/// <typeparm name="Cost_t">type of cost betwean two nodes</typeparm>
template <typename Cost_t>
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
	void setCost(id_t id, Cost_t cost);

	/// <summary>
	/// set a new cost of node, and previous node
	/// </summary>
	/// <param name="id">id of node</param>
	/// <param name="cost">new cost</param>
	/// <param name="prev">id of previous node in path</param>
	void setCost(id_t id, Cost_t cost, id_t prev);

	/// <summary>
	/// returns cost of node
	/// </summary>
	/// <param name="id">selected node</param>
	/// <returns>cost of node</returns>
	Cost_t getCost(id_t id);

	/// <summary>
	/// gets node with smallest cost and removes it from releted lists
	/// </summary>
	/// <returns>tuple with id and cost</returns>
	idAndCost_t<Cost_t> pop();

	/// <summary>
	/// whater the set is empty which means end of iteration
	/// in dijstra algorithm
	/// </summary>
	/// <returns>is set empty</returns>
	bool isEmpty();

	/// <summary>
	/// gets path from start node to end node
	/// </summary>
	/// <param name="endNode">id of end node</param>
	/// <returns>list of ids in path</returns>
	deque<id_t> getPath(id_t endNode);

private:
	
	/// <summary>
	/// set of nodes that was not processed yet
	/// </summary>
	set<id_t> ids;

	/// <summary>
	/// set of all nodes with cost
	/// </summary>
	unordered_map<id_t, Cost_t> costs;

	/// <summary>
	/// path from start node to random node
	/// </summary>
	unordered_map<id_t, id_t> prevNodes;

	/// <summary>
	/// subset of ids list, it holds only nodes with computed cost
	/// this list is used to find node with smallest cost that is still in ids
	/// </summary>
	set<id_t> idWithValue;
};

template<typename Cost_t>
inline DijstraSet<Cost_t>::DijstraSet(id_t size)
{
	for (id_t i = 0; i < size; i++) {
		ids.insert(i);
		costs[i] = numeric_limits<Cost_t>::max();
	}
}

template<typename Cost_t>
inline void DijstraSet<Cost_t>::setCost(id_t id, Cost_t cost)
{
	assert(id < costs.size());
	costs[id] = cost;
	//insert if not duplicated
	idWithValue.insert(id);
}

template<typename Cost_t>
inline void DijstraSet<Cost_t>::setCost(id_t id, Cost_t cost, id_t prev)
{
	setCost(id, cost);
	prevNodes[id] = prev;
}

template<typename Cost_t>
inline Cost_t DijstraSet<Cost_t>::getCost(id_t id)
{
	return costs[id];
}

template<typename Cost_t>
inline idAndCost_t<Cost_t> DijstraSet<Cost_t>::pop()
{	
	auto compFunc = [this](id_t p, id_t q) {return costs[p] < costs[q]; };

	auto itMin=min_element(idWithValue.begin(), idWithValue.end(), compFunc);

	auto minCostId = *itMin;
	auto minCost = costs[minCostId];

	ids.erase(minCostId);
	idWithValue.erase(itMin);

	return idAndCost_t<Cost_t>(minCostId, minCost);
}

template<typename Cost_t>
inline bool DijstraSet<Cost_t>::isEmpty()
{
	return ids.empty();
}

template<typename Cost_t>
inline deque<id_t> DijstraSet<Cost_t>::getPath(id_t endNode)
{
	auto it=prevNodes.find(endNode);

	deque<id_t> path;
	
	while (it != prevNodes.end()) {
		auto prevId = it->second;
		path.push_front(prevId);
		it= prevNodes.find(prevId);
	}

	path.push_back(endNode);

	return path;
}
