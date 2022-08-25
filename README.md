# crop-cpp


This repository provides the WebAssembly part of the CROP implementation in Comunica.

## Installation

1. Follow the installation instructions for Emscripten https://emscripten.org/docs/getting_started/downloads.html
2. Build the project with the following command: `em++ crop/idp/IdpFindPlan.cpp crop/idp/utils/SubsetUtils.cpp crop/idp/utils/BitsetUtils.cpp crop/idp/EntryMetadata.cpp crop/idp/IdpQueryPlan.cpp crop/idp/IdpQueryOperator.cpp wasm/cropFromWasm.cpp wasm/utils/ByteBuffer.cpp wasm/utils/QueryPlansSerializer.cpp -o wasm/wasm-output/cropcpp.wasm -O3 -s EXPORTED_FUNCTIONS=_findPlanFromWasm,_free -s WASM=1 -s MAXIMUM_MEMORY=4gb -s ALLOW_MEMORY_GROWTH --no-entry` (to be executed within the `src/` folder)
   1. In case you want to start with the minimal amount of memory usage, add the following two flags: `-s TOTAL_STACK=32kb -s TOTAL_MEMORY=64kb`.
3. The .wasm binary file is now located under `src/wasm/wasm-output/cropcpp.wasm`
4. To use the binary file in Comunica with CROP (https://github.com/jonasbovyn/comunica-crop), paste it under `packages/actor-rdf-join-crop/lib/wasm/bin`. The project does not need to be rebuilt.
