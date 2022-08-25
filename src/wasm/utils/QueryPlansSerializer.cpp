#include "QueryPlansSerializer.h"

void serializePlans(std::vector<IdpQueryPlanPtr>* queryPlans, ByteBuffer* buffer) {
    buffer->appendInt(static_cast<int>(queryPlans->size()));
    for (const auto& ptr : *queryPlans) {
        serializePlan(ptr, buffer);
    }
}

void serializePlan(const IdpQueryPlanPtr& queryPlan, ByteBuffer* buffer) {
    buffer->appendInt(queryPlan->triples);
    buffer->appendDouble(queryPlan->cost);
    if (dynamic_cast<IdpQueryLeaf*>(queryPlan.get()) != nullptr) {
        buffer->appendByte(0);
        serializeLeaf(dynamic_cast<IdpQueryLeaf*>(queryPlan.get()), buffer);
    }
    else if (dynamic_cast<IdpQueryNode*>(queryPlan.get()) != nullptr) {
        buffer->appendByte(1);
        serializeNode(dynamic_cast<IdpQueryNode*>(queryPlan.get()), buffer);
    }
    else {
        // shouldn't happen
    }
}

void serializeNode(IdpQueryNode *queryNode, ByteBuffer* buffer) {
    serializePlan(queryNode->left, buffer);
    buffer->appendByte(queryNode->op);
    serializePlan(queryNode->right, buffer);
}

void serializeLeaf(IdpQueryLeaf *queryLeaf, ByteBuffer* buffer) {
    buffer->appendInt(queryLeaf->entryIndex);
}
