#include "SubsetUtils.h"
#include <bitset>

using namespace std;

KSubset::KSubset(int k, int bound)  : size(k), bound(bound) {
    set = new int[k];
    for (int i = 0; i < k; i++) {
        set[i] = i;
    }
}

KSubset::KSubset(int bound, int *set, int k) : bound(bound), size(k), set(set) {}



void KSubset::lexSuccessor() {
    int i = size - 1;
    while (i >= 0 && set[i] == bound - size + i) {
        i -= 1;
    }

    if (i == -1) {
        delete set;
        set = nullptr;
        return;
    }

    int buffer = set[i];
    for (int j = i; j < size; j += 1) {
        set[j] = buffer + j - i + 1;
    }
}

bitset<100> KSubset::mask() {
    bitset<100> result(0);
    for (int i = 0; i < size; i++) {
        int n = set[i];
        result.set(n);
    }
    return result;
}

bitset<100> KSubset::maskWithCollection(const int *collection) {
    bitset<100> result(0);
    for (int i = 0; i < size; i++) {
        int n = set[i];
        result.set(collection[n]);
    }
    return result;
}

KSubset::~KSubset() {
    delete set;
}
