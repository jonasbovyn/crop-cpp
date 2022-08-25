#include "ByteBuffer.h"

ByteBuffer::ByteBuffer() = default;
ByteBuffer::~ByteBuffer() = default;

void ByteBuffer::appendByte(char value) {
    byteVector.push_back(value);
}

void ByteBuffer::appendDouble(double value) {
    byteVector.insert(byteVector.end(), ((char*) &value), ((char*) &value) + 8);
}

void ByteBuffer::appendInt(int value) {
    byteVector.insert(byteVector.end(), ((char*) &value), ((char*) &value) + 4);
}

char *ByteBuffer::getStartPointer() {
    return &byteVector[0];
}

int ByteBuffer::size() {
    return static_cast<int>(byteVector.size());
}
