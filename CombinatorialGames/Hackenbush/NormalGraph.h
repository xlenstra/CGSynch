//
// Created by ardour on 17-03-22.
//

#ifndef CGSYNCH_2_NORMALGRAPH_H
#define CGSYNCH_2_NORMALGRAPH_H

#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <optional>

#include "RulesetUtil.h"

typedef size_t NodeId;
typedef std::vector<std::vector<PieceColour>> PieceMatrix;

class NormalGraph;

namespace std {
	template<>
	struct hash<NormalGraph> {
		size_t operator()(const NormalGraph& position) const;
	};
}

// Class for a graph without multiple edges not permitting loops, of which the first node is uniquely labeled.
class NormalGraph {
public:
	NormalGraph();
	NormalGraph(const NormalGraph& other) = default;

	bool operator==(const NormalGraph& other) const;

	void resizeNodeCount(size_t newSize);
	void addEdge(NodeId from, NodeId to, PieceColour edgeColour);
	void removeEdge(NodeId from, NodeId to);
	std::set<std::tuple<NodeId, NodeId, PieceColour>> getAllEdges();

	[[nodiscard]] std::string getDisplayString() const;

	/** Checks if two graphs are isomorphic.
	 * Returns `true` or `false` if they are or are not respectively,
	 * or std::optional(); if checking whether they are isomorphic would take too long.
	 */
	[[nodiscard]] std::optional<bool> isIsomorphicWith(const NormalGraph& other) const;
	[[nodiscard]] NormalGraph getSubGraphConnectedToGround() const;

private:
	/** Get degrees of all nodes in the graph */
	[[nodiscard]] std::multiset<size_t> getDegrees() const;
	[[nodiscard]] size_t getDegree(NodeId id) const;
	[[nodiscard]] std::pair<size_t, size_t> getEdgeColours(const NodeId& nodeId) const;
	[[nodiscard]] std::pair<size_t, size_t> getAllEdgeColours() const;
	[[nodiscard]] inline const NodeId& getGround() const { return groundId; }

	void addVertex(const std::vector<PieceColour>& vertex, NodeId vertexId, std::unordered_map<NodeId, NodeId>& nodeIdTranslation);

	[[nodiscard]] bool backtrackingCheckIsomorphic(const NormalGraph& other) const;

	PieceMatrix adjacencyMatrix = {{PieceColour::NONE}};
	const NodeId groundId = 0;

	friend NormalGraph graphFromMatrixString(size_t nodeCount, std::vector<PieceColour> input);

	// Unneeded warning, this function does exist, but clang can't find it or something.
	friend size_t std::hash<NormalGraph>::operator()(const NormalGraph& position) const;
};

NormalGraph graphFromMatrixString(size_t nodeCount, std::vector<PieceColour> input);

#endif //CGSYNCH_2_NORMALGRAPH_H
