# τ-balanced-clustering

**Paper:** Generating Clusters of Similar Sizes by Constrained Balanced Clustering

> Releasing the source code is in progress, the GPUs-enhanced version will be released in the future.

## Build

In Linux, under the project directory, generate an executable file through the following command.

```shell
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Run

the executable file is under the `bin` folder

```shell
./bin/tbc <input> <output> <k> <tau> [seed]
```

 + `input`: input file path
 + `output`: output file path
 + `k`: number of clusters
 + `tau`: tau balanced factor, set it 0 if don't need
 + `seed` (optional): for shuffle the raw data

