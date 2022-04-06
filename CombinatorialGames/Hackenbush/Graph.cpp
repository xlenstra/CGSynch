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
				++blueEdges;
				break;
			case PieceColour::RED:
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

void Vertex::removeEdge(PieceColour colour, NodeId otherNode) {
	reachableNodes.erase(std::make_pair(colour, otherNode));
}

bool Vertex::operator==(const Vertex& other) const {
	return ID == other.ID && reachableNodes == other.reachableNodes;
}


Graph::Graph() = default;

bool Graph::operator==(const Graph& other) const {
	for (size_t i = 0; i < nodeList.size(); ++i) {
		if (nodeList[i] != other.nodeList[i]) return false;
	}
	return true;
}

std::optional<bool> Graph::isIsomorphicWith(const Graph& other) const {
	if (this->nodeList.size() != other.nodeList.size()) return false;
	if (this->nodeList.empty()) return true;
	if (this->getGround().getDegree() != other.getGround().getDegree()) return false;
	if (this->getGround().getEdgeColourCounts() != other.getGround().getEdgeColourCounts()) return false;
	if (this->nodeList.size() == 1) return true;
	if (*this == other) return true;
	if (this->getDegrees() != other.getDegrees()) return false;
	if (this->getAllEdgeColours() != other.getAllEdgeColours()) return false;
	// Easy checks are inconclusive. Harder checks require solving unsolved problems in computer science.
	// So let's not, maybe do backtracking if this turns out to be too slow or something.
	return {};


	// Easy checks are inconclusive. Let's just try a backtracking algorithm.
//	if (this->nodeList.size() > 32) return {}; // Too much work, don't even bother
//	return backtrackingCheckIsomorphic(other);
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

void Graph::addVertex(const Vertex& vertex) {
	addNode(vertex.ID);
	for (const auto& [colour, id] : vertex.reachableNodes) {
		addNode(id);
		addEdge(vertex.ID, id, colour);
	}
}

void Graph::addEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from > nodeList.size() || to > nodeList.size()) {
		throw(std::domain_error("Edge added to non-existent node"));
	}
	nodeList[from].addEdge(edgeColour, to);
	nodeList[to].addEdge(edgeColour, from);
}

void Graph::removeEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from > nodeList.size() || to > nodeList.size()) {
		throw(std::domain_error("Edge removed from non-existent node"));
	}
	nodeList[from].removeEdge(edgeColour, to);
	nodeList[to].removeEdge(edgeColour, from);
}

Graph Graph::getSubGraphConnectedToGround() const {
	Graph subGraph;
	std::deque<NodeId> reachableUnvisitedNodes = {0};
	std::unordered_set<NodeId> visitedNodes;
	while (!reachableUnvisitedNodes.empty()) {
		NodeId nextNodeToVisit = reachableUnvisitedNodes.front();
		reachableUnvisitedNodes.pop_front();
		subGraph.addVertex(nodeList[nextNodeToVisit]);
		visitedNodes.insert(nextNodeToVisit);
		for (const auto& [colour, to] : nodeList[nextNodeToVisit].reachableNodes) {
			if (!visitedNodes.contains(to))
				reachableUnvisitedNodes.push_back(to);
		}
	}
	return subGraph;
}
