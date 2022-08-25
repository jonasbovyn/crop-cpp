#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "crop/idp/EntryMetadata.h"
#include "crop/idp/IdpQueryPlan.h"

#include <utility>

using namespace std;

namespace {

    typedef struct NodeExpectation {
        IdpQueryPlanPtr plan;
        int height;
        int triples;
        double cost;
        int cardinality;
    } NodeExpectation;

    NodeExpectation createExpectation(IdpQueryPlanPtr node, int height, int triples, double cost, int cardinality) {
        NodeExpectation expectation;
        expectation.plan = std::move(node);
        expectation.height = height;
        expectation.triples = triples;
        expectation.cost = cost;
        expectation.cardinality = cardinality;
        return expectation;
    }


    class IdpQueryPlanTest : public ::testing::Test {

    protected:
        int size;
        EntryMetadata** metadatas;
        vector<IdpQueryPlanPtr>* leafs;

        IdpQueryPlanTest() {
            size = 3;
            metadatas = new EntryMetadata*[size];
            metadatas[0] = createMetadata(4, 100, 1);
            metadatas[1] = createMetadata(5, 100, 1);
            metadatas[2] = createMetadata(2, -1, 1);

            leafs = new vector<IdpQueryPlanPtr>();
            for (int i = 0; i < size; i++) {
                leafs->push_back(make_shared<IdpQueryLeaf>(i, metadatas));
            }
        }

        ~IdpQueryPlanTest() override {
            for (int i = 0; i < size; i++) {
                delete metadatas[i];
            }
            delete metadatas;
            delete leafs;
        }

//        virtual void SetUp() {}
//        virtual void TearDown() {}

    };


    TEST_F(IdpQueryPlanTest, leafValidQueryPlan) {
        EXPECT_EQ((*leafs)[0]->cost, 4);
        EXPECT_EQ((*leafs)[0]->triples, 1);
        EXPECT_EQ((*leafs)[0]->height, 0);
    }

    TEST_F(IdpQueryPlanTest, leafCardinalities) {
        EXPECT_EQ((*leafs)[0]->cardinality, 4);
        EXPECT_EQ((*leafs)[1]->cardinality, 5);
        EXPECT_EQ((*leafs)[2]->cardinality, 2);
    }


    TEST_F(IdpQueryPlanTest, nodesValidQueryPlan) {
        auto node0Hash1 = make_shared<IdpQueryNode>((*leafs)[0], Hash, (*leafs)[1], metadatas);
        auto node2Hash1 = make_shared<IdpQueryNode>((*leafs)[2], Hash, (*leafs)[1], metadatas);
        auto node0NL1 = make_shared<IdpQueryNode>((*leafs)[0], NestedLoop, (*leafs)[1], metadatas);
        auto node0H1NL2 = make_shared<IdpQueryNode>(node0Hash1, NestedLoop, (*leafs)[2], metadatas);
        auto node0NL1H2 = make_shared<IdpQueryNode>(node0NL1, Hash, (*leafs)[2], metadatas);
        auto node2H0NL1 = make_shared<IdpQueryNode>((*leafs)[2], Hash, node0NL1, metadatas);

        NodeExpectation expectations[] = {
                createExpectation(node0Hash1,1,2,2.004,4),
                createExpectation(node2Hash1,1,2,2.002,2),
                createExpectation(node0H1NL2, 2, 3, 2.508, 2),
                createExpectation(node0NL1H2,2,3,3.011,2),
                createExpectation(node2H0NL1,2,3,3.011,2),
        };

        for (NodeExpectation expectation: expectations) {
            EXPECT_EQ(expectation.plan->height, expectation.height);
            EXPECT_EQ(expectation.plan->triples, expectation.triples);
//            EXPECT_NEAR(expectation.plan->cost, expectation.cost, 0.000001); -> altered
            EXPECT_EQ(expectation.plan->cardinality, expectation.cardinality);
        }

    }
}
