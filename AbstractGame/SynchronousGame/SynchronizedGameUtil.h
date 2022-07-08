//
// Created by Xander Lenstra on 03-05-22.
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
	std::vector<SynchedId> leftOptions;
	std::vector<SynchedId> rightOptions;

	bool operator==(const SynchedMatrix& other) const = default;
};


namespace std {
	template<>
	struct hash<SynchedMatrix> {
		size_t operator()(const SynchedMatrix& synchedMatrix) const {
			size_t matrixHash = boost::hash_range(synchedMatrix.matrix.begin(), synchedMatrix.matrix.end());
			boost::hash_combine(matrixHash, boost::hash<std::vector<SynchedId>>()(synchedMatrix.leftOptions));
			boost::hash_combine(matrixHash, boost::hash<std::vector<SynchedId>>()(synchedMatrix.rightOptions));
			return matrixHash;
		}
	};
}

class SGDatabase;
class SynchronizedGame;

#endif //CGSYNCH_2_SYNCHRONIZEDGAMEUTIL_H
