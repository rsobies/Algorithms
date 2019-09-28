#pragma once

/// <summary>
/// class to hold data related with bellman-ford algorithm
/// </summary>
/// <typeparm name="Cost_t">type of cost betwean two nodes</typeparm>
template <typename Cost_t>
class BellmanFordSet
{
public:

	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="size">number of nodes in graph</param>
	BellmanFordSet(id_t size);

	void reset(id_t size);

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
	/// gets path from start node to end node
	/// </summary>
	/// <param name="endNode">id of end node</param>
	/// <returns>list of ids in path</returns>
	deque<id_t> getPath(id_t endNode);

protected:
	/// <summary>
	/// set of all nodes with cost
	/// </summary>
	unordered_map<id_t, Cost_t> costs;
private:
	
	/// <summary>
	/// path from start node to random node
	/// </summary>
	unordered_map<id_t, id_t> prevNodes;

	/// <summary>
	/// mutex to ensure thread safe, getCost, setCost, getPath
	/// </summary>
	mutex mtx;
};

template<typename Cost_t>
inline BellmanFordSet<Cost_t>::BellmanFordSet(id_t size)
{
	for (id_t i = 0; i < size; i++) {
		
		costs[i] = numeric_limits<Cost_t>::max();
	}
}

template<typename Cost_t>
inline void BellmanFordSet<Cost_t>::reset(id_t size)
{
	costs.clear();
	prevNodes.clear();

	for (id_t i = 0; i < size; i++) {
		costs[i] = numeric_limits<Cost_t>::max();
	}
}

template<typename Cost_t>
inline void BellmanFordSet<Cost_t>::setCost(id_t id, Cost_t cost)
{
	lock_guard lock(mtx);
	assert(id < costs.size());
		
	costs[id] = cost;
}

template<typename Cost_t>
inline void BellmanFordSet<Cost_t>::setCost(id_t id, Cost_t cost, id_t prev)
{
	lock_guard lock(mtx);
	costs[id] = cost;
	prevNodes[id] = prev;
}

template<typename Cost_t>
inline Cost_t BellmanFordSet<Cost_t>::getCost(id_t id)
{
	lock_guard lock(mtx);
	return costs[id];
}

template<typename Cost_t>
inline deque<id_t> BellmanFordSet<Cost_t>::getPath(id_t endNode)
{
	lock_guard lock(mtx);
	auto it = prevNodes.find(endNode);

	deque<id_t> path;

	while (it != prevNodes.end()) {
		auto prevId = it->second;
		path.push_front(prevId);
		it = prevNodes.find(prevId);
	}

	path.push_back(endNode);

	return path;
}
