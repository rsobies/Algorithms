#pragma once

/// <summary>
/// data class describing a node
/// </summary>
/// <typeparm name="Cost_t">type of cost betwean two nodes</typeparm>
template <typename Cost_t>
class NodeInPath
{
	using NodeWithCost = tuple<weak_ptr<NodeInPath<Cost_t>>, Cost_t>;
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
	void addNeighbour(shared_ptr<NodeInPath<Cost_t>> node, Cost_t pathCost);
	
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

template<typename Cost_t>
inline id_t NodeInPath<Cost_t>::getId()
{
	return id;
}

template<typename Cost_t>
inline void NodeInPath<Cost_t>::addNeighbour(shared_ptr<NodeInPath<Cost_t>> node, Cost_t pathCost)
{
	neighbours.push_back(NodeWithCost(node, pathCost));
}