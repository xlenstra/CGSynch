//
// Created by ardour on 17-03-22.
//

#ifndef CGSYNCH_2_GRAPH_H
#define CGSYNCH_2_GRAPH_H

#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <optional>

#include "RulesetUtil.h"

typedef size_t nodeId;

namespace std {
	template<>
	struct hash<std::pair<PieceColour,nodeId>> {
		size_t operator()(const std::pair<PieceColour,nodeId>& pair) const {
			size_t seed = 0;
			boost::hash_combine(seed, boost::hash<PieceColour>()(pair.first));
			boost::hash_combine(seed, boost::hash<nodeId>()(pair.second));
			return seed;
		}
	};
}

struct Vertex {
	std::unordered_set<std::pair<PieceColour,nodeId>> reachableNodes;
	nodeId ID = -1ul;

	Vertex(const Vertex& other) = default;

	size_t getDegree() const { return reachableNodes.size(); }
	std::pair<size_t, size_t> getEdgeColourCounts() const;
};

class Graph {
public:
	Graph();
	Graph(const Graph& other) = default;

	[[nodiscard]] const Vertex& getGround() const { return nodeList[0]; }

	/** Checks if two graphs are isomorphic.
	 * Returns `true` or `false` if they are or are not,
	 * or std::optional(); if checking whether they are isomorphic would take too long.
	 */
	[[nodiscard]] std::optional<bool> isIsomorphicWith(const Graph& other) const;

private:
	/** Get degrees of all nodes in the graph */
	[[nodiscard]] std::multiset<size_t> getDegrees() const;
	[[nodiscard]] std::pair<size_t, size_t> getAllEdgeColours() const;
	[[nodiscard]] bool backtrackingCheckIsomorphic(const Graph& other) const;

	std::vector<Vertex> nodeList;
};

#endif //CGSYNCH_2_GRAPH_H
