//#include <algorithm>
#include <algorithm>
#include "IdpQueryPlan.h"
#include "EntryMetadata.h"

using namespace std;

IdpQueryPlan::IdpQueryPlan(double cost, int cardinality, int triples, int height, int creationIndex) :
    cost(cost), cardinality(cardinality), triples(triples), height(height), creationIndex(creationIndex), variablesMask(0) {}

double IdpQueryPlan::compare(const IdpQueryPlanPtr &other) const {
    if (this->cost == other->cost) {
        return this->creationIndex - other->creationIndex;
    }
    return this->cost - other->cost;
}

IdpQueryPlan::~IdpQueryPlan() = default;


IdpQueryLeaf::IdpQueryLeaf(int entryIndex, EntryMetadata **entryMetadatas, int creationIndex) :
                            entryIndex(entryIndex), IdpQueryPlan(0, -1, 1, 0, creationIndex) {
    EntryMetadata *metadata = entryMetadatas[entryIndex];
    cardinality = metadata->cardinality;
    cost = cardinality;
    int pageSize = metadata->pageSize;
    double requests = pageSize == -1 ? 1.0 : (cardinality*1.0)/pageSize;
    requestCost = max(requests, 1.0);
    variablesMask = metadata->variablesMask;
}



IdpQueryNode::IdpQueryNode(const IdpQueryPlanPtr& left, IdpQueryOperator op, const IdpQueryPlanPtr& right, EntryMetadata** entryMetadatas, int creationIndex) :
                            left(left), op(op), right(right), IdpQueryPlan() {
    this->creationIndex = creationIndex;
    variablesMask = left->variablesMask | right->variablesMask;
    height = std::max(left->height, right->height) + 1;
    triples = left->triples + right->triples;
    cardinality = estimateCardinality();
    cost = calculateCost(entryMetadatas);

}

double IdpQueryNode::processCost() {
    double opMultiplier = opCost(op);
    if (op == Hash) {
        return opMultiplier * this->cardinality;
    }

    // NestedLoop
    return opMultiplier * (this->cardinality + this->right->cardinality);
}

double IdpQueryNode::requestCost(EntryMetadata **entryMetadatas) {
    double totalRequestCost = 0;
    if (left->triples == 1) {
        totalRequestCost += (dynamic_pointer_cast<IdpQueryLeaf>(left))->requestCost;
    }

    if (op == Hash) {
        if (right->triples == 1) {
            totalRequestCost += (dynamic_pointer_cast<IdpQueryLeaf>(right))->requestCost;
        }
    }
    else {
        // NestedLoop
        double heightFactor = max(1, 4*height);

        int pageSize = entryMetadatas[(dynamic_pointer_cast<IdpQueryLeaf>(right))->entryIndex]->pageSize;
        double requests = pageSize == -1 ? 1.0 : (cardinality*1.0)/pageSize;

        totalRequestCost += max(1.0 * left->cardinality, max(10 * requests, 1.0)) / heightFactor;
    }

    return totalRequestCost;
}

double IdpQueryNode::calculateCost(EntryMetadata **entryMetadatas) {
    return processCost() + requestCost(entryMetadatas) +
            (left->triples == 1 ? 0 : left->cost) + (right->triples == 1 ? 0 : right->cost);
}

int IdpQueryNode::estimateCardinality() const {
    return std::min(left->cardinality, right->cardinality);
}

