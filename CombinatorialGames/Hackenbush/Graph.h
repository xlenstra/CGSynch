//
// Created by ardour on 17-03-22.
//

#ifndef CGSYNCH_2_GRAPH_H
#define CGSYNCH_2_GRAPH_H

#include <unordered_set>
#include <boost/container_hash/hash.hpp>

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
};

class Graph {
public:
	Graph();
	Graph(const Graph& other) = default;
private:
	std::vector<Vertex> adjacencyList;
};

#endif //CGSYNCH_2_GRAPH_H
