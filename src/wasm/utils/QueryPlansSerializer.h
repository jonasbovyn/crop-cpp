#ifndef CROPCPP_PARENT_QUERYPLANSSERIALIZER_H
#define CROPCPP_PARENT_QUERYPLANSSERIALIZER_H


#include "ByteBuffer.h"
#include "../../crop/idp/IdpQueryPlan.h"
#include <vector>

void serializePlans(std::vector<IdpQueryPlanPtr>* queryPlans, ByteBuffer* buffer);

void serializePlan(const IdpQueryPlanPtr& queryPlan, ByteBuffer* buffer);

void serializeNode(IdpQueryNode* queryNode, ByteBuffer* buffer);

void serializeLeaf(IdpQueryLeaf* queryLeaf, ByteBuffer* buffer);

#endif //CROPCPP_PARENT_QUERYPLANSSERIALIZER_H
