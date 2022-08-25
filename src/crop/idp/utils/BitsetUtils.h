#ifndef CROPCPP_BITSETUTILS_H
#define CROPCPP_BITSETUTILS_H

#include <bitset>

// functions are declared in .h file unfortunately:
// https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function


// https://stackoverflow.com/questions/21245139/fastest-way-to-compare-bitsets-operator-on-bitsets
template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
}

// https://stackoverflow.com/questions/16761472/how-can-i-increment-stdbitset
template<std::size_t N>
void incrementBitset(std::bitset<N>& in) {
    for (size_t i = 0; i < N; ++i) {
        if (in[i] == 0) {
            in[i] = 1;
            break;
        }
        in[i] = 0;
    }
}


#endif //CROPCPP_BITSETUTILS_H
