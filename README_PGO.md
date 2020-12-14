git clone --recurse-submodules https://github.com/carlopi/wasp/
cd wasp
git checkout PGOprototype
mkdir build
cd build
cmake .. -G ninja
make -j8

// move to your work directory
// take arbitrary file_name.js and file_name.wasm

~/path/to/wasp/build/src/tools/wasp wasm2wasm file_name.wasm -o file_name_pgo.wasm
cp file_name.js file_name_annotated.js

// in the JS, modify the called wasm from file_name.wasm to file_name_annotated.js
// in the JS, add a bunch of random utility functions from pgo.js

nodejs file_name_annotated.js

// in the output you should now find a list of hints to be added to if and br_if, one for each hint, encoded as:
// 2 -> no hint, 1 -> hint taken, 0 -> hint not taken
// currently the strategy is this: less than 20 visit -> no hint, >90% success -> taken, <10% success -> not taken, otherwise no hint

// feel free to modify / amend the JS utility functions (to change how to choose hint or change the output format)

// add pgo

// compare file_name.js with file_name_pgo.js
