#ifndef CROPCPP_PARENT_BYTEBUFFER_H
#define CROPCPP_PARENT_BYTEBUFFER_H

#include <vector>


class ByteBuffer {
private:
    std::vector<char> byteVector;

public:
    ByteBuffer();
    void appendByte(char value);
    void appendDouble(double value);
    void appendInt(int value);

    char* getStartPointer();
    int size();

    ~ByteBuffer();
};


#endif //CROPCPP_PARENT_BYTEBUFFER_H
