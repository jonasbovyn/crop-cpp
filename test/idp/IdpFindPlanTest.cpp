#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "crop/idp/EntryMetadata.h"
#include "crop/idp/IdpFindPlan.h"
#include "crop/idp/IdpQueryPlan.h"

using namespace std;

EntryMetadata* m1[] = {
        createMetadata(4, 100, 1),
        createMetadata(5, 100, 1),
        createMetadata(2, -1, 1),
        createMetadata(8, 200, 1),
        createMetadata(7, 150, 1),
};



TEST(IdpFindPlan, prunePlans) {
    IdpQueryPlanPtr l[] = {
            make_shared<IdpQueryLeaf>(0, m1),
            make_shared<IdpQueryLeaf>(1, m1),
            make_shared<IdpQueryLeaf>(2, m1),
            make_shared<IdpQueryLeaf>(3, m1),
            make_shared<IdpQueryLeaf>(4, m1),
    };

    unordered_map<bitset<100>, vector<IdpQueryPlanPtr>*> optPlans;
    auto* plans = new vector<IdpQueryPlanPtr>();

    auto node = make_shared<IdpQueryNode>(l[2], Hash, l[3], m1);
    plans->push_back(l[0]);
    plans->push_back(l[1]);
    plans->push_back(node);
    plans->push_back(l[4]);

    bitset<100> key = bitset<100>(5);

    optPlans[key] = plans;
    IdpFindPlan::prunePlans(key, optPlans, 2);
    ASSERT_EQ(optPlans[key]->size(), 2);
    delete optPlans[key];
}

TEST(IdpFindPlan, findPlanSmall) {
    IdpFindPlan planfinder = IdpFindPlan(5, m1);
    auto* v = planfinder.findPlan(2, 5);

    IdpQueryPlanPtr bestPlan = v->front();

//    EXPECT_EQ(v->size(), 5); -> not necessarily
    EXPECT_EQ(bestPlan->triples, 5);
    delete v;
}


int rand_between(int min, int max) {
    return rand()%(max-min + 1) + min;
}

IdpQueryPlanPtr findPlanRandom(unsigned int seed, int k, int size) {
    srand(seed);
    EntryMetadata* metadatas[size];

    for (int i = 0; i < size; i++) {
        metadatas[i] = createMetadata(
                rand_between(0, 10000),
                rand_between(0,3) == 0 ? -1 : rand_between(100, 5000),
                3
        );
    }

    IdpFindPlan planfinder = IdpFindPlan(size, metadatas);
    auto* v = planfinder.findPlan(k, 5);
    IdpQueryPlanPtr bestPlan = v->front();
    delete v;

    for (int i = 0; i < size; i++) {
        delete metadatas[i];
    }
    return bestPlan;
}


TEST(IdpFindPlan, findPlanLarge) {
    int size = 25;
    int k = 4;
    IdpQueryPlanPtr bestPlan = findPlanRandom(0, k, size);
    EXPECT_EQ(bestPlan->triples, size);
}

//// todo remove
//TEST(IdpFindPlan, smallestCrash) {
//    IdpQueryPlanPtr bestPlan = findPlanRandom(0, 2, 17);
//    EXPECT_EQ(bestPlan->triples, 17);
//    delete bestPlan;
//}

TEST(IdpFindPlan, lotsOfFindPlans) {
    for (int k = 2; k <= 4; k++) {
        for (int size = 10; size < 20; size++) {
            for (unsigned int seed = 0; seed < 2; seed++) {
                std::cout << "k: " << k << "size: " << size << "seed: " << seed << endl;
                IdpQueryPlanPtr bestPlan = findPlanRandom(seed, k, size);
                EXPECT_EQ(bestPlan->triples, size);
            }
        }
    }
}


//TEST(IdpFIndPLan, heavyPlans) {
//    for (int k = 3; k <= 8; k++) {
//        for (int size = 20; size < 30; size++) {
//            for (unsigned int seed = 0; seed < 10; seed++) {
//                std::cout << "k: " << k << ", size: " << size << ", seed: " << seed << endl;
//                IdpQueryPlanPtr bestPlan = findPlanRandom(seed, k, size);
//                EXPECT_EQ(bestPlan->triples, size);
//            }
//        }
//    }
//}

TEST(IdpFindPLan, heavyPlans) {
    IdpQueryPlanPtr bestPlan = findPlanRandom(5, 4, 30);
    EXPECT_EQ(bestPlan->triples, 30);
}

TEST(IdpFindPlan, specificTestPlan) {
    int size = 3;

    EntryMetadata* metadatas[size];

    metadatas[0] = createMetadata(51, -1, 1);
    metadatas[1] = createMetadata(219, -1, 1);
    metadatas[2] = createMetadata(1, -1, 1);

    IdpFindPlan planfinder = IdpFindPlan(size, metadatas);
    auto* v = planfinder.findPlan(4, 5);
    IdpQueryPlanPtr bestPlan = v->front();
    delete v;

    for (auto & metadata : metadatas) {
        delete metadata;
    }

    EXPECT_EQ(bestPlan->triples, 3);
}