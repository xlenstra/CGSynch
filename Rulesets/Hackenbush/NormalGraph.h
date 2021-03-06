//
// Created by Xander Lenstra on 17-03-22.
//

#ifndef CGSYNCH_2_NORMALGRAPH_H
#define CGSYNCH_2_NORMALGRAPH_H

#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <optional>

#include "../RulesetUtil.h"
#include "../../Util/Matrix.h"

typedef size_t NodeId;

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
	[[nodiscard]] std::pair<size_t, size_t> getAllEdgeColours() const;

	[[nodiscard]] std::string getDisplayString() const;

	/** Checks if two graphs are isomorphic.
	 * Returns `true` or `false` if they are or are not respectively,
	 * or std::optional(); if checking whether they are isomorphic would take too long.
	 */
	[[nodiscard]] std::optional<bool> isIsomorphicWith(const NormalGraph& other) const;
	[[nodiscard]] NormalGraph getSubGraphConnectedToGround(NodeId from, NodeId to) const;

private:
	/** Get degrees of all nodes in the graph */
	[[nodiscard]] std::multiset<size_t> getDegrees() const;
	[[nodiscard]] size_t getDegree(NodeId id) const;
	[[nodiscard]] std::pair<size_t, size_t> getEdgeColours(const NodeId& nodeId) const;
	[[nodiscard]] inline const NodeId& getGround() const { return groundId; }

	void addVertex(const std::vector<PieceColour>& vertex, NodeId vertexId, const std::vector<NodeId>& nodeIdTranslation);

	[[nodiscard]] bool backtrackingCheckIsomorphic(const NormalGraph& other) const;

	Matrix<PieceColour> adjacencyMatrix = Matrix(1,1, PieceColour::NONE);
	const NodeId groundId = 0;

	friend NormalGraph graphFromMatrixString(size_t nodeCount, std::vector<PieceColour> input);

	friend struct std::hash<NormalGraph>;
};

NormalGraph graphFromMatrixString(size_t nodeCount, std::vector<PieceColour> input);

#endif //CGSYNCH_2_NORMALGRAPH_H
