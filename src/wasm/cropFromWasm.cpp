#include "../crop/idp/EntryMetadata.h"
#include "../crop/idp/IdpFindPlan.h"
#include "utils/ByteBuffer.h"
#include "utils/QueryPlansSerializer.h"
#include <emscripten/emscripten.h>

using namespace std;

EntryMetadata** parseMetadatas(int metadatasSize, int* metadatasFields) {
    auto** m1 = new EntryMetadata*[metadatasSize];
    for (int i = 0; i < metadatasSize; i += 1) {
        m1[i] = createMetadata(metadatasFields[i*3], metadatasFields[i*3+1], metadatasFields[i*3+2]);
    }
    return m1;
}

void destroyMetadatas(int metadatasSize, EntryMetadata** metadatas) {
    for (int i = 0; i < metadatasSize; i++) {
        delete metadatas[i];
    }
    delete metadatas;
}


// avoids Name mengling of c++ functions, so they can be called from webAssembly
#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE char* findPlanFromWasm(int k, int t, int metadatasSize, int* metadataFields) {
    EntryMetadata** m1 = parseMetadatas(metadatasSize, metadataFields);

    IdpFindPlan planfinder = IdpFindPlan(metadatasSize, m1);
    auto* v = planfinder.findPlan(k, t);

    auto buffer = ByteBuffer();
    serializePlans(v, &buffer);

    char* startPointer = buffer.getStartPointer();

    char* byteArr = (char*) malloc(buffer.size());
    std::copy(startPointer, startPointer + buffer.size(), byteArr);

    delete v;
    destroyMetadatas(metadatasSize, m1);

    // byteArr needs to be freed from WASM, after using
    return byteArr; // gives error in Clion but its actually not
}

#ifdef __cplusplus
}
#endif