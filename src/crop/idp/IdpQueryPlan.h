#ifndef CROPCPPIDPQUERYPLAN_H
#define CROPCPPIDPQUERYPLAN_H


#include <memory>
#include "EntryMetadata.h"
#include "IdpQueryOperator.h"


class IdpQueryPlan {
public:
    double cost;
    int cardinality;

    int triples;
    int height;

    int creationIndex;

    int variablesMask;

    IdpQueryPlan(double cost, int cardinality, int triples, int height, int creationIndex = 0);
    IdpQueryPlan() = default;

    double compare(const std::shared_ptr<IdpQueryPlan>& other) const;

    virtual ~IdpQueryPlan();

};

typedef std::shared_ptr<IdpQueryPlan> IdpQueryPlanPtr;


class IdpQueryLeaf : public IdpQueryPlan {
public:
    int entryIndex;
    double requestCost;

    IdpQueryLeaf(int entryIndex, EntryMetadata** entryMetadatas, int creationIndex = 0);
};



class IdpQueryNode : public IdpQueryPlan {
public:
    IdpQueryPlanPtr left;
    IdpQueryOperator op;
    IdpQueryPlanPtr right;

    IdpQueryNode(const IdpQueryPlanPtr& left, IdpQueryOperator op, const IdpQueryPlanPtr& right, EntryMetadata** entryMetadatas, int creationIndex = 0);

private:
    double processCost();
    double requestCost(EntryMetadata **entryMetadata);
    double calculateCost(EntryMetadata** entryMetadata);
    int estimateCardinality() const;
};


#endif //CROPCPPIDPQUERYPLAN_H
