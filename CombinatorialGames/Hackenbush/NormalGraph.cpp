//
// Created by ardour on 17-03-22.
//

#include "NormalGraph.h"

NormalGraph::NormalGraph() = default;

bool NormalGraph::operator==(const NormalGraph& other) const {
	return adjacencyMatrix != other.adjacencyMatrix;
}

std::optional<bool> NormalGraph::isIsomorphicWith(const NormalGraph& other) const {
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
	if (this->adjacencyMatrix.size() > 32) return {}; // Too much work, don't even bother
	return backtrackingCheckIsomorphic(other);
}

size_t NormalGraph::getDegree(NodeId id) const {
	size_t degree = 0;
	for (const auto& potentialEdge : adjacencyMatrix[id]) {
		if (potentialEdge != PieceColour::NONE)
			++degree;
	}
	return degree;
}

std::multiset<size_t> NormalGraph::getDegrees() const {
	std::multiset<size_t> degrees;
	for (size_t nodeId = 0; nodeId < adjacencyMatrix.size(); ++nodeId) {
		degrees.insert(getDegree(nodeId));
	}
	return degrees;
}

bool NormalGraph::backtrackingCheckIsomorphic(const NormalGraph& other) const {
	// Plan: create tree-with-back-edges from this graph using DFS from the ground
	// Use backtracking to try to match nodes of the other graph to this tree
	// Constraints:
	//  - New nodes must be connected via the right colour
	//  - Back-edges must have the correct colour
	//  - node ID must be unused
	// If possible: isomorphic, otherwise not.
	return false;
}

std::pair<size_t, size_t> NormalGraph::getEdgeColourCounts(const NodeId& nodeId) const {
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

std::pair<size_t, size_t> NormalGraph::getAllEdgeColours() const {
	std::pair<size_t, size_t> edgeColours;
	for (size_t nodeId = 0; nodeId < adjacencyMatrix.size(); ++nodeId) {
		const std::pair<size_t, size_t>& colours = getEdgeColourCounts(nodeId);
		edgeColours.first += colours.first;
		edgeColours.second += colours.second;
	}
	return edgeColours;
}

void NormalGraph::resizeNodeCount(size_t newSize) {
	if (newSize <= adjacencyMatrix.size()) return;
	std::vector<std::vector<PieceColour>> newAdjacencyMatrix(newSize, std::vector<PieceColour>(newSize, PieceColour::NONE));
	for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
		std::move(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), newAdjacencyMatrix[i].begin());
	}
}

void NormalGraph::addEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from >= adjacencyMatrix.size() || to >= adjacencyMatrix.size()) {
		throw std::domain_error("Edge added to non-existent node");
	}
	adjacencyMatrix[from][to] = edgeColour;
	adjacencyMatrix[to][from] = edgeColour;
}

void NormalGraph::removeEdge(NodeId from, NodeId to, PieceColour edgeColour) {
	if (from >= adjacencyMatrix.size() || to >= adjacencyMatrix.size()) {
		throw std::domain_error("Edge added to non-existent node");
	}
	adjacencyMatrix[from][to] = PieceColour::NONE;
	adjacencyMatrix[to][from] = PieceColour::NONE;
}

std::string NormalGraph::getDisplayString() const {
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

NormalGraph NormalGraph::getSubGraphConnectedToGround() const {
	NormalGraph subGraph;
	std::deque<NodeId> reachableUnvisitedNodes = { groundId };
	std::unordered_set<NodeId> visitedNodes;
	std::unordered_map<NodeId, NodeId> nodeIdTranslation;
	while (!reachableUnvisitedNodes.empty()) {
		NodeId nextNodeToVisit = reachableUnvisitedNodes.front();
		reachableUnvisitedNodes.pop_front();
		subGraph.addVertex(adjacencyMatrix[nextNodeToVisit], nextNodeToVisit, nodeIdTranslation);
		visitedNodes.insert(nextNodeToVisit);
		for (size_t i = 0; i < adjacencyMatrix[nextNodeToVisit].size(); ++i) {
			if (adjacencyMatrix[nextNodeToVisit][i] == PieceColour::NONE) continue;
			if (!visitedNodes.contains(i))
				reachableUnvisitedNodes.push_back(i);
		}
	}
	return subGraph;
}

void NormalGraph::addVertex(const std::vector<PieceColour>& vertex, NodeId vertexId, std::unordered_map<NodeId, NodeId>& nodeIdTranslation) {
	if (!nodeIdTranslation.contains(vertexId)) {
		nodeIdTranslation[vertexId] = adjacencyMatrix.size();
	}
	resizeNodeCount(nodeIdTranslation[vertexId]);

	for (size_t i = 0; i < vertex.size(); ++i) {
		PieceColour colour = vertex[i];
		if (colour == PieceColour::NONE) continue;
		if (!nodeIdTranslation.contains(i)) {
			nodeIdTranslation[i] = adjacencyMatrix.size();
		}
		resizeNodeCount(nodeIdTranslation[i]);
		addEdge(nodeIdTranslation[vertexId], nodeIdTranslation[i], colour);
	}
}

// Assumes input as a vector that runs through a square matrix row by row, from top to bottom.
// The rows are the 'from' node, and the columns are the 'to' node.
// E.g., "_RR_" would encode the matrix
// _ R
// R _
// which in turn encodes a graph containing a red edge from the ground to node 1 and back.
NormalGraph graphFromMatrixString(size_t nodeCount, std::vector<PieceColour> input) {
	NormalGraph graph;
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

size_t std::hash<NormalGraph>::operator()(const NormalGraph& position) const {
	return boost::hash_range(position.adjacencyMatrix.begin(), position.adjacencyMatrix.end());
}
