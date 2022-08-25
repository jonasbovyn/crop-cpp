#ifndef CROPCPPSUBSETUTILS_H
#define CROPCPPSUBSETUTILS_H

#include <bitset>

class KSubset {

private:
    int bound;

public:
    KSubset(int k, int bound);
    KSubset(int bound, int *set, int k);
    ~KSubset();
    void lexSuccessor(); // returns nullptr if it ended, returns the set otherwise
    std::bitset<100> mask();
    std::bitset<100> maskWithCollection(const int* collection);

    int* set;
    int size; // length of set
};


void kSubsetLexSuccessor(int bound, int* set);

#endif //CROPCPPSUBSETUTILS_H
