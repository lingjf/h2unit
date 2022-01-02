# Benchmark


## Compile performance

|               |   h2unit.h    | h2unit.hpp/cpp |
| ------------- | ------------- | -------------- |
| Linux gcc     |            62 |             18 |
| macOS clang   |            49 |             21 |
| Windows msvc  |            67 |             35 |

data from [benchmark testing](https://github.com/lingjf/h2unit/actions/workflows/benchmark.yml)

In order to speed up compile progress, split header-only-single-file [h2unit.h](../h2unit.h) into two files: [h2unit.cpp](../build/h2unit.cpp) and [h2unit.hpp](../build/h2unit.hpp). Refer to [`benchmark`](../benchmark/) or [`example`](../example/) for sample usage.

twofiles speed up 2~3 times than onefile.
