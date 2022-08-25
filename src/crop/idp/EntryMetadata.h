#ifndef CROPCPPENTRYMETADATA_H
#define CROPCPPENTRYMETADATA_H


typedef struct EntryMetadata {
    int pageSize;       // -1 if undefined
    int cardinality;
    int variablesMask;
} EntryMetadata;

EntryMetadata* createMetadata(int cardinality, int pageSize, int variablesMask);

#endif //CROPCPPENTRYMETADATA_H
