//
// Created by ardour on 07-04-22.
//

#ifndef CGSYNCH_2_TREENODES_H
#define CGSYNCH_2_TREENODES_H

#include <vector>
#include "CombinatorialGameUtil.h"

namespace parser {
	struct SpiritParserTreeNode {
		SpiritParserTreeNode* rightChild = nullptr;
		virtual ~SpiritParserTreeNode() { delete rightChild; }

		void setRightChild(SpiritParserTreeNode* newChild) { rightChild = newChild; }
		virtual void setLeftChild(AbstractId newChild) {};
		virtual AbstractId explore(AbstractId parent) = 0;
	};

	struct SpiritParserTreeNodeBinary : SpiritParserTreeNode {
		void setLeftChild(AbstractId newChild) override;
		AbstractId leftChild = -1;
	};

	struct SpiritParserTreeNodeAdd : SpiritParserTreeNodeBinary {
		AbstractId explore(AbstractId parent) override;
	};

	struct SpiritParserTreeNodeSubtract : SpiritParserTreeNodeBinary {
		AbstractId explore(AbstractId parent) override;
	};

	struct SpiritParserTreeNodeCanonical: SpiritParserTreeNode {
		AbstractId explore(AbstractId parent) override;
	};
}

#endif //CGSYNCH_2_TREENODES_H
