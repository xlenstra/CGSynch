//
// Created by ardour on 07-04-22.
//

#ifndef CGSYNCH_2_TREENODES_H
#define CGSYNCH_2_TREENODES_H

#include <vector>
#include "CombinatorialGame/CombinatorialGameUtil.h"

namespace parser {
	struct SpiritParserTreeNode {
		SpiritParserTreeNode* rightChild = nullptr;
		virtual ~SpiritParserTreeNode() { delete rightChild; }

		void setRightChild(SpiritParserTreeNode* newChild) { rightChild = newChild; }
		virtual void setLeftChild(AlternatingId newChild) { auto var = newChild; newChild = var; };
		virtual AlternatingId explore(AlternatingId parent) = 0;
	};

	struct SpiritParserTreeNodeBinary : SpiritParserTreeNode {
		void setLeftChild(AlternatingId newChild) override;
		AlternatingId leftChild = -1;
	};

	struct SpiritParserTreeNodeAdd : SpiritParserTreeNodeBinary {
		AlternatingId explore(AlternatingId parent) override;
	};

	struct SpiritParserTreeNodeSubtract : SpiritParserTreeNodeBinary {
		AlternatingId explore(AlternatingId parent) override;
	};

	struct SpiritParserTreeNodeCanonical: SpiritParserTreeNode {
		AlternatingId explore(AlternatingId parent) override;
	};
}

#endif //CGSYNCH_2_TREENODES_H
