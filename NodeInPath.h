#pragma once

/// <summary>
/// data class describing a node
/// </summary>
template <typename T>
class NodeInPath
{
	using NodeWithCost = tuple<weak_ptr<NodeInPath<T>>, T>;
public:
	
	/// <summary>
	/// initialize new node
	/// </summary>
	/// <param name="id">id of node</param>
	NodeInPath(id_t id) :id(id) {}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>node id</returns>
	id_t getId();

	/// <summary>
	/// add a neigbour node to current node
	/// </summary>
	/// <param name="node">neighbour node</param>
	/// <param name="pathCost">cost connected with this neigbour</param>
	void addNeighbour(shared_ptr<NodeInPath<T>> node, T pathCost);
	
	/// <summary>
	/// 
	/// </summary>
	/// <returns>all neigbours of this node</returns>
	const vector<NodeWithCost>& getNeighbours(){return neighbours;}
	
private:
	/// <summary>
	/// id of node
	/// </summary>
	id_t id;

	/// <summary>
	/// all neighbours, and cost related to them
	/// </summary>
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