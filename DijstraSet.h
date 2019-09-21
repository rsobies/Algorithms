#pragma once
template <typename T>
using idAndCost_t=tuple<id_t, T>;

template <typename T>
class DijstraSet
{
	
public:
	DijstraSet(id_t size);
	void put(id_t id, T cost);
	void put(id_t id, T cost, id_t prev);
	T getCost(id_t id);
	idAndCost_t<T> pop();

	bool isEmpty();

private:
	
	/**
		set of nodes that was not prossed yet
	*/
	list<id_t> ids;

	/**
	 set of all nodes with cost
	*/
	unordered_map<id_t, T> costs;

	/**
		path from start node to random node
	*/
	unordered_map<id_t, id_t> prevNodes;

	/**
		subset of ids, only nodes with computed cost
		used to find node with smalest cost
	*/	
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
inline void DijstraSet<T>::put(id_t id, T cost)
{
	assert(id < costs.size());
	costs[id] = cost;
	//insert if not duplicated
	idWithValue.insert(id);
}

template<typename T>
inline void DijstraSet<T>::put(id_t id, T cost, id_t prev)
{
	put(id, cost);
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
