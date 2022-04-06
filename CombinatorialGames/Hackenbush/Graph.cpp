//
// Created by ardour on 17-03-22.
//

#include "Graph.h"

std::pair<size_t, size_t> Vertex::getEdgeColourCounts() const {

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
	if (colour != PieceColour::NONE)
		reachableNodes.insert(std::make_pair(colour, otherNode));
}

void Vertex::removeEdge(PieceColour colour, NodeId otherNode) {
	reachableNodes.erase(std::make_pair(colour, otherNode));
}

bool Vertex::operator==(const Vertex& other) const {
	return ID == other.ID && reachableNodes == other.reachableNodes;
}

bool Vertex::isAdjacentTo(NodeId otherNode) {
	return (
		reachableNodes.contains(std::make_pair(PieceColour::BLUE, otherNode))
		|| reachableNodes.contains(std::make_pair(PieceColour::RED, otherNode))
	);
}

PieceColour Vertex::adjacencyColour(NodeId otherNode) {
	if (reachableNodes.contains(std::make_pair(PieceColour::BLUE, otherNode)))
		return PieceColour::BLUE;
	if (reachableNodes.contains(std::make_pair(PieceColour::RED, otherNode)))
		return PieceColour::RED;
	return PieceColour::NONE;
}


Graph::Graph() = default;

bool Graph::operator==(const Graph& other) const {
	return adjacencyMatrix != other.adjacencyMatrix;
}

std::optional<bool> Graph::isIsomorphicWith(const Graph& other) const {
	if (this->adjacencyMatrix.size() != other.adjacencyMatrix.size()) return false;
	if (this->adjacencyMatrix.empty()) return true;
	if (this->getDegree(groundId) != other.getDegree(other.getGround())) return false;
	if (this->getEdgeColourCounts(groundId) != other.getEdgeColourCounts(other.getGround())) return false;
	if (this->adjacencyMatrix.size() == 1) return true;
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

size_t Graph::getDegree(NodeId id) const {
	size_t degree = 0;
	for (const auto& potentialEdge : adjacencyMatrix[id]) {
		if (potentialEdge != PieceColour::NONE)
			++degree;
	}
	return degree;
}

std::multiset<size_t> Graph::getDegrees() const {
	std::multiset<size_t> degrees;
	for (size_t nodeId = 0; nodeId < adjacencyMatrix.size(); ++nodeId) {
		degrees.insert(getDegree(nodeId));
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

std::pair<size_t, size_t> Graph::getEdgeColourCounts(const NodeId& nodeId) const {
	size_t blueEdges = 0;
	size_t redEdges = 0;
	for (const auto& potentialEdge : adjacencyMatrix[nodeId]) {
		switch (potentialEdge) {
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

std::pair<size_t, size_t> Graph::getAllEdgeColours() const {
	std::pair<size_t, size_t> edgeColours;
	for (size_t nodeId = 0; nodeId < adjacencyMatrix.size(); ++nodeId) {
		const std::pair<size_t, size_t>& colours = getEdgeColourCounts(nodeId);
		edgeColours.first += colours.first;
		edgeColours.second += colours.second;
	}
	return edgeColours;
}

void Graph::resizeNodeCount(size_t newSize) {
	if (newSize <= adjacencyMatrix.size()) return;
	std::vector<std::vector<PieceColour>> newAdjacencyMatrix(newSize, std::vector<PieceColour>(newSize, PieceColour::NONE));
	for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
		std::move(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), newAdjacencyMatrix[i].begin());
	}
}

void Graph::addEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from >= adjacencyMatrix.size() || to >= adjacencyMatrix.size()) {
		throw std::domain_error("Edge added to non-existent node");
	}
	adjacencyMatrix[from][to] = edgeColour;
	adjacencyMatrix[to][from] = edgeColour;
}

void Graph::removeEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from >= adjacencyMatrix.size() || to >= adjacencyMatrix.size()) {
		throw std::domain_error("Edge added to non-existent node");
	}
	adjacencyMatrix[from][to] = PieceColour::NONE;
	adjacencyMatrix[to][from] = PieceColour::NONE;
}

std::string Graph::getDisplayString() {
	std::string output;
	output += "{\n";
	for (const auto& row : adjacencyMatrix) {
		output += '{';
		for (const auto& element : row) {
			output += pieceColourToChar(element);
			output += ',';
		}
		output.pop_back();
		output += "}\n";
	}
	output += "}\n";
	return output;
}

Graph Graph::getSubGraphConnectedToGround() const {
	Graph subGraph;
	std::deque<NodeId> reachableUnvisitedNodes = { groundId };
	std::unordered_set<NodeId> visitedNodes;
	std::unordered_map<NodeId, NodeId> nodeIdTranslation;
	//while (!reachableUnvisitedNodes.empty()) {
	//	NodeId nextNodeToVisit = reachableUnvisitedNodes.front();
	//	reachableUnvisitedNodes.pop_front();
	//	subGraph.addVertex(nodeList[nextNodeToVisit], nodeIdTranslation);
	//	visitedNodes.insert(nextNodeToVisit);
	//	for (const auto& [colour, to] : nodeList[nextNodeToVisit].reachableNodes) {
	//		if (!visitedNodes.contains(to))
	//			reachableUnvisitedNodes.push_back(to);
	//	}
	//}
	return subGraph;
}

void Graph::addVertex(const Vertex& vertex, std::unordered_map<NodeId, NodeId>& nodeIdTranslation) {
	//if (!nodeIdTranslation.contains(vertex.ID)) {
	//	nodeIdTranslation[vertex.ID] = nodeList.size();
	//}
	//addNode(nodeIdTranslation[vertex.ID]);
	//
	//for (const auto& [colour, to] : vertex.reachableNodes) {
	//	if (!nodeIdTranslation.contains(to)) {
	//		nodeIdTranslation[to] = nodeList.size();
	//	}
	//	addNode(nodeIdTranslation[to]);
	//	addEdge(nodeIdTranslation[vertex.ID], nodeIdTranslation[to], colour);
	//}
}

// Assumes input as a vector that runs through a square matrix row by row, from top to bottom.
// The rows are the 'from' node, and the columns are the 'to' node.
// E.g., "_RR_" would encode the matrix
// _ R
// R _
// which in turn encodes a graph containing a red edge from the ground to node 1 and back.
Graph graphFromMatrixString(size_t nodeCount, std::vector<PieceColour> input) {
	Graph graph;
	graph.resizeNodeCount(nodeCount);
	for (size_t charIndex = 0; charIndex < input.size(); ++charIndex) {
		if (input[charIndex] != PieceColour::NONE) {
			// If this is an edge from a node to itself
			if (charIndex/nodeCount == charIndex % nodeCount) {
				throw std::logic_error("Invalid vector to build graph out of");
			}
			// If we already have an edge between these two nodes with a different colour
			if (graph.adjacencyMatrix[charIndex/nodeCount][charIndex % nodeCount] == -input[charIndex]) {
				throw std::logic_error("Invalid vector to build graph out of");
			}
			graph.addEdge(charIndex/nodeCount, charIndex % nodeCount, input[charIndex]);
		}
	}
	return graph;
}
