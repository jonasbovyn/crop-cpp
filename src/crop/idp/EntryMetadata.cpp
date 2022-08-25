#include "EntryMetadata.h"

EntryMetadata* createMetadata(int cardinality, int pageSize, int variablesMask){
    auto* metadata = new EntryMetadata;
    metadata->cardinality = cardinality;
    metadata->pageSize = pageSize;
    metadata->variablesMask = variablesMask;
    return metadata;
};