//
// Created by ardour on 03-05-22.
//

#ifndef CGSYNCH_2_SYNCHRONIZEDGAMEUTIL_H
#define CGSYNCH_2_SYNCHRONIZEDGAMEUTIL_H

#include <cstddef>
#include <vector>
#include <boost/functional/hash/hash.hpp>
#include "../../Util/Matrix.h"

typedef size_t SynchedId;

struct SynchedMatrix {
	Matrix<SynchedId> matrix;
	size_t leftMoveCount = 0;
	size_t rightMoveCount = 0;

	bool operator==(const SynchedMatrix& other) const = default;
};


namespace std {
	template<>
	struct hash<SynchedMatrix> {
		size_t operator()(const SynchedMatrix& synchedMatrix) const {
			size_t matrixHash = boost::hash_range(synchedMatrix.matrix.begin(), synchedMatrix.matrix.end());
			boost::hash_combine(matrixHash, synchedMatrix.leftMoveCount);
			boost::hash_combine(matrixHash, synchedMatrix.rightMoveCount);
			return matrixHash;
		}
	};
}

class SGDatabase;
class SynchronizedGame;

#endif //CGSYNCH_2_SYNCHRONIZEDGAMEUTIL_H
