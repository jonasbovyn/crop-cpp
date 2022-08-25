#include "IdpFindPlan.h"
#include "utils/SubsetUtils.h"
#include "utils/BitsetUtils.h"
#include <limits>
#include <algorithm>

using namespace std;

vector<IdpQueryPlanPtr>* IdpFindPlan::findPlan(int k, int t) {
    this->currentNodeIndex = 0;

    std::unordered_map<bitset<100>, vector<IdpQueryPlanPtr>*> optPlans;

    std::bitset<100> bitset1(1);
    for (int i = 0; i < size; i++) {
        auto leaf = make_shared<IdpQueryLeaf>(i, metadatas, this->currentNodeIndex++);
        auto* initialPlans = new vector<IdpQueryPlanPtr>();
        initialPlans->push_back(leaf);

        optPlans[bitset1 << i] = initialPlans;
    }

    int nextSymbol = size;
    vector<int> todoSymbols;
    todoSymbols.reserve(2*size);
    for (int i = 0; i < size; i++) {
        todoSymbols.push_back(i);
    }

    while (todoSymbols.size() > 1) {
        k = min(k, static_cast<int>(todoSymbols.size()));
        for (int i = 2; i <= k; i += 1) {
            // S is the set of currently considered "to_do" symbols
            // indicesS->set map to the index in to_do
            auto indicesS = KSubset(i, static_cast<int>(todoSymbols.size()));

            while (indicesS.set != nullptr) {
                // https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
                bitset<100> maskS = indicesS.maskWithCollection(&todoSymbols[0]);

                if (optPlans.count(maskS) == 0) { // count can only be 0 or 1 in unordered map
                    // S does not exist yet
                    optPlans[maskS] = new vector<IdpQueryPlanPtr>();

                    bitset<100> maskOtoS(1);
                    bitset<100> upperbound = bitset1 << (i - 1);
                    while (maskOtoS < upperbound) {
                        bitset<100> maskO(0);
                        for (int j = 0; j < i; j++) {
                            if (maskOtoS.test(j)) {
                                maskO.set(todoSymbols[indicesS.set[j]]);
                            }
                        }

                        if (optPlans.count(maskO) != 0 && optPlans.count(maskS ^ maskO) != 0) {
                            vector<IdpQueryPlanPtr> newPlans = joinPlans(optPlans[maskO], optPlans[maskS ^ maskO]);

                            if (!newPlans.empty()) {
                                // extending a vector: https://stackoverflow.com/questions/313432/c-extend-a-vector-with-another-vector
                                auto* plansS = optPlans[maskS];
                                plansS->insert(plansS->end(), newPlans.begin(), newPlans.end());

                                prunePlans(maskS, optPlans, newPlans[0]->triples == 2 ? 1 : t);
                            }
                        }

                        incrementBitset(maskOtoS);
                    }
                }

                indicesS.lexSuccessor();
            }
        }


        auto indicesV = KSubset(k, static_cast<int>(todoSymbols.size()));
        int bestV[k];
        bitset<100> bestVmask;
        IdpQueryPlanPtr bestPlan = nullptr;

        while (indicesV.set != nullptr) {
            bitset<100> maskV = indicesV.maskWithCollection(&todoSymbols[0]);

            for (const IdpQueryPlanPtr& plan : *(optPlans[maskV])) {
                if (bestPlan == nullptr || plan->compare(bestPlan) < 0) {
                    bestVmask = maskV;
                    bestPlan = plan;
                    for (int i = 0; i < k; i++) {
                        bestV[i] = todoSymbols[indicesV.set[i]];
                    }
                }
            }

            indicesV.lexSuccessor();
        }


        auto removeFromPlans = vector<bitset<100>>();
        for (auto& it: optPlans) {
            // Do stuff
            if ((it.first & bestVmask).any()) {
                removeFromPlans.push_back(it.first);
            }
        }

        auto* optV = new vector<IdpQueryPlanPtr>();

        if (todoSymbols.size() - k + 1 > 1) { // checks if this is not the last iteration
            optV->push_back(bestPlan);
        }
        else {
            for (const IdpQueryPlanPtr& plan : *(optPlans[bestVmask])) {
                optV->push_back(plan);
            }
        }

        for (auto key : removeFromPlans) {
            delete optPlans[key];   // deletes the allocation of the vector
            optPlans.erase(key);
        }

        optPlans[bitset1 << nextSymbol] = optV;

        // https://stackoverflow.com/questions/21204676/modern-way-to-filter-stl-container
        vector<int> buffer;
        std::copy_if (todoSymbols.begin(), todoSymbols.end(), back_inserter(buffer),[&bestVmask](int i){
            return !bestVmask.test(i); // if to_do symbol is in V, which is masked to the to_do symbols(?), it should be removed
        });
        todoSymbols = buffer;

        todoSymbols.push_back(nextSymbol);
        nextSymbol++;
    }

    // there should only be one plan left, the result

    bitset<100> lastKey = optPlans.begin()->first;
    prunePlans(lastKey, optPlans, t);
    return optPlans[lastKey];
}

struct planCompare{
    bool operator()(const IdpQueryPlanPtr& a,const IdpQueryPlanPtr& b) const{
        return a->compare(b) > 0;
    }
};

void IdpFindPlan::prunePlans(bitset<100> key, unordered_map<bitset<100>, vector<IdpQueryPlanPtr>*>& optPlans, int t) {
    // should never get an empty plans array

    vector<IdpQueryPlanPtr>* vec = optPlans[key];
    std::make_heap(vec->begin(), vec->end(), planCompare());

    int newSize = min(static_cast<int>(vec->size()), t);
    auto* newVector = new vector<IdpQueryPlanPtr>();
    newVector->reserve(newSize);

    for (int i = 0; i < newSize; i++) {
        std::pop_heap(vec->begin(), vec->end(), planCompare());
        IdpQueryPlanPtr min = vec->back();
        vec->pop_back();
        newVector->push_back(min);
    }

    optPlans[key] = newVector;
    delete vec;
}

vector<IdpQueryPlanPtr> IdpFindPlan::joinPlans(vector<IdpQueryPlanPtr>* plans1, vector<IdpQueryPlanPtr>* plans2) {

    vector<IdpQueryPlanPtr> joinedPlans = vector<IdpQueryPlanPtr>();

    for (const IdpQueryPlanPtr& plan1 : *plans1) {
        for (const IdpQueryPlanPtr& plan2 : *plans2) {
            // If the intersection of variables is empty, it is not a useful join operation
            if ((plan1->variablesMask & plan2->variablesMask) == 0) {
                continue;
            }

            // Nested Loop
            if (plan2->triples == 1) {
                auto nodeNL1 = make_shared<IdpQueryNode>(plan1, NestedLoop, plan2, metadatas, this->currentNodeIndex++);
                joinedPlans.push_back(nodeNL1);
            }
            if (plan1->triples == 1) {
                auto nodeNL2 = make_shared<IdpQueryNode>(plan2, NestedLoop, plan1, metadatas, this->currentNodeIndex++);
                joinedPlans.push_back(nodeNL2);
            }

            // Hash
            auto nodeH = make_shared<IdpQueryNode>(plan1, Hash, plan2, metadatas, this->currentNodeIndex++);
            joinedPlans.push_back(nodeH);

        }
    }

    return joinedPlans;
}

IdpFindPlan::IdpFindPlan(int size, EntryMetadata **metadatas) : size(size), metadatas(metadatas) {}


