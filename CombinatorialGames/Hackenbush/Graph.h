//
// Created by ardour on 17-03-22.
//

#ifndef CGSYNCH_2_GRAPH_H
#define CGSYNCH_2_GRAPH_H

#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <optional>

#include "RulesetUtil.h"

typedef size_t NodeId;

namespace std {
	template<>
	struct hash<std::pair<PieceColour,NodeId>> {
		size_t operator()(const std::pair<PieceColour,NodeId>& pair) const {
			size_t seed = 0;
			boost::hash_combine(seed, boost::hash<PieceColour>()(pair.first));
			boost::hash_combine(seed, boost::hash<NodeId>()(pair.second));
			return seed;
		}
	};
}

struct Vertex {
	Vertex() = default;
	Vertex(const Vertex& other) = default;

	[[nodiscard]] size_t getDegree() const { return reachableNodes.size(); }
	[[nodiscard]] std::pair<size_t, size_t> getEdgeColourCounts() const;
	[[nodiscard]] std::unordered_set<NodeId> getNodesReachableViaColour(const PieceColour& colour) const;

	void addEdge(PieceColour colour, NodeId otherNode);
	void removeEdge(PieceColour colour, NodeId otherNode);

	bool operator==(const Vertex& other) const;

	std::set<std::pair<PieceColour,NodeId>> reachableNodes;
	NodeId ID = -1ul;
};

// Class for a graph without multiple edges not permitting loops, of which the first node is uniquely labeled.
class Graph {
public:
	Graph();
	Graph(const Graph& other) = default;

	bool operator==(const Graph& other) const;

	void addNode(NodeId);
	void addVertex(const Vertex& vertex);
	void addEdge(NodeId from, NodeId to, PieceColour edgeColour);
	void removeEdge(NodeId from, NodeId to, PieceColour edgeColour);

	/** Checks if two graphs are isomorphic.
	 * Returns `true` or `false` if they are or are not respectively,
	 * or std::optional(); if checking whether they are isomorphic would take too long.
	 */
	[[nodiscard]] std::optional<bool> isIsomorphicWith(const Graph& other) const;
	[[nodiscard]] Graph getSubGraphConnectedToGround() const;

private:
	/** Get degrees of all nodes in the graph */
	[[nodiscard]] std::multiset<size_t> getDegrees() const;
	[[nodiscard]] std::pair<size_t, size_t> getAllEdgeColours() const;
	[[nodiscard]] const Vertex& getGround() const { return nodeList[0]; }

	[[nodiscard]] bool backtrackingCheckIsomorphic(const Graph& other) const;

	std::vector<Vertex> nodeList;
};

#endif //CGSYNCH_2_GRAPH_H
