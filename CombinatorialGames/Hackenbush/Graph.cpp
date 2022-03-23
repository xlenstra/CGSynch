//
// Created by ardour on 17-03-22.
//

#include "Graph.h"

std::pair<size_t, size_t> Vertex::getEdgeColourCounts() const {
	size_t blueEdges = 0;
	size_t redEdges = 0;
	for (const auto& connection : reachableNodes) {
		switch (connection.first) {
			case PieceColour::BLUE:
			case PieceColour::BLACK:
				++blueEdges;
				break;
			case PieceColour::RED:
			case PieceColour::WHITE:
				++redEdges;
				break;
			default:
				break;
		}
	}
	return { blueEdges, redEdges };
}

std::unordered_set<NodeId> Vertex::getNodesReachableViaColour(const PieceColour& colour) const {
	std::unordered_set<NodeId> nodes;
	for (const auto& [edgeColour, id] : reachableNodes) {
		if (edgeColour == colour) {
			nodes.insert(id);
		}
	}
	return nodes;
}

void Vertex::addEdge(PieceColour colour, NodeId otherNode) {
	reachableNodes.insert(std::make_pair(colour, otherNode));
}


Graph::Graph() {}

std::optional<bool> Graph::isIsomorphicWith(const Graph& other) const {
	if (this->nodeList.size() != other.nodeList.size()) return false;
	if (this->nodeList.empty()) return true;
	if (this->getGround().getDegree() != other.getGround().getDegree()) return false;
	if (this->getGround().getEdgeColourCounts() != other.getGround().getEdgeColourCounts()) return false;
	if (this->nodeList.size() == 1) return true;
	if (this->getDegrees() != other.getDegrees()) return false;
	if (this->getAllEdgeColours() != other.getAllEdgeColours()) return false;

	// Easy checks are inconclusive. Let's just try a backtracking algorithm.
	if (this->nodeList.size() > 32) return {}; // Too much work, don't even bother
	return backtrackingCheckIsomorphic(other);
}

std::multiset<size_t> Graph::getDegrees() const {
	std::multiset<size_t> degrees;
	for (const auto& vertex : nodeList) {
		degrees.insert(vertex.getDegree());
	}
	return degrees;
}

bool Graph::backtrackingCheckIsomorphic(const Graph& other) const {
	// Plan: create tree-with-back-edges from this graph using DFS from the ground
	// Use backtracking to try to match nodes of the other graph to this tree
	// Constraints:
	//  - New nodes must be connected via the right colour
	//  - Back-edges must have the correct colour
	//  - node ID must be unused
	// If possible: isomorphic, otherwise not.
	return false;
}

std::pair<size_t, size_t> Graph::getAllEdgeColours() const {
	std::pair<size_t, size_t> edgeColours;
	for (const auto& vertex : nodeList) {
		const std::pair<size_t, size_t>& colours = vertex.getEdgeColourCounts();
		edgeColours.first += colours.first;
		edgeColours.second += colours.second;
	}
	return edgeColours;
}

void Graph::addNode(NodeId nodeId) {
	if (nodeId > nodeList.size()) {
		nodeList.resize(nodeId);
	}
	nodeList[nodeId].ID = nodeId;
}

void Graph::addEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from > nodeList.size() || to > nodeList.size()) {
		throw(std::domain_error("Edge added to non-existent node"));
	}
	nodeList[from].addEdge(edgeColour, to);
	nodeList[to].addEdge(edgeColour, from);
}
