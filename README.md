# τ-balance-clustering

## Build

```shell
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Run

the executable file is under the bin folder

```shell
./bin/tbc <input> <output> <k> <tau> [seed]
```

 + `input`: input file path
 + `output`: output file path
 + `k`: number of clusters
 + `tau`: tau
 + `seed` (optional): for shuffle the raw data
