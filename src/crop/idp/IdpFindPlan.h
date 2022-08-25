#ifndef CROPCPPIDPFINDPLAN_H
#define CROPCPPIDPFINDPLAN_H

#include <unordered_map>
#include "EntryMetadata.h"
#include "IdpQueryPlan.h"
#include <vector>
#include <bitset>


class IdpFindPlan {
public:
    IdpFindPlan(int size, EntryMetadata **metadatas);
    std::vector<IdpQueryPlanPtr>* findPlan(int k, int t);

private:
    int size;
    int currentNodeIndex;
    EntryMetadata** metadatas;


public: // temporarily public for testing purpose
    static void prunePlans(std::bitset<100> key, std::unordered_map<std::bitset<100>, std::vector<IdpQueryPlanPtr>*>& optPlans, int t);
    std::vector<IdpQueryPlanPtr> joinPlans(std::vector<IdpQueryPlanPtr>* plans1, std::vector<IdpQueryPlanPtr>* plans2);
};


#endif //CROPCPPIDPFINDPLAN_H
