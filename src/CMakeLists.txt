cmake_minimum_required(VERSION 3.20)
project(cropcpp_parent)

set(CMAKE_CXX_STANDARD 14 -std=c++11)

set(IDP_DIR crop/idp)
set(CROP_SOURCE_FILES
        ${IDP_DIR}/IdpFindPlan.cpp
        ${IDP_DIR}/IdpFindPlan.h
        ${IDP_DIR}/utils/SubsetUtils.cpp
        ${IDP_DIR}/utils/SubsetUtils.h
        ${IDP_DIR}/utils/BitsetUtils.cpp
        ${IDP_DIR}/utils/BitsetUtils.h
        ${IDP_DIR}/EntryMetadata.cpp
        ${IDP_DIR}/EntryMetadata.h
        ${IDP_DIR}/IdpQueryPlan.cpp
        ${IDP_DIR}/IdpQueryPlan.h
        ${IDP_DIR}/IdpQueryOperator.cpp
        ${IDP_DIR}/IdpQueryOperator.h)

set(WASM_CROP_SOURCE_FILES
        wasm/trash_tests/wasmTest1.cpp
        wasm/trash_tests/wasmTest2.cpp
        wasm/cropFromWasm.cpp
        wasm/utils/ByteBuffer.h
        wasm/utils/ByteBuffer.cpp
        wasm/utils/QueryPlansSerializer.cpp
        wasm/utils/QueryPlansSerializer.h)

add_executable(crop
        ${CROP_SOURCE_FILES}
        ${WASM_CROP_SOURCE_FILES}
        )
