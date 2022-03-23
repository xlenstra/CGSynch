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
