#pragma once

template <typename T>
class NodeInPath
{
	using NodeWithCost = tuple<weak_ptr<NodeInPath<T>>, T>;
public:
	
	NodeInPath(id_t id) :id(id) {}

	id_t getId();

	void addNeighbour(shared_ptr<NodeInPath<T>> node, T pathCost);
	
	const vector<NodeWithCost>& getNeighbours(){return neighbours;}
	
private:
	
	id_t id;
	vector<NodeWithCost> neighbours;
};

template<typename T>
inline id_t NodeInPath<T>::getId()
{
	return id;
}

template<typename T>
inline void NodeInPath<T>::addNeighbour(shared_ptr<NodeInPath<T>> node, T pathCost)
{
	neighbours.push_back(NodeWithCost(node, pathCost));
}