# Benchmark


## Compile performance

|               |   h2unit.h    | h2unit.hpp/cpp |
| ------------- | ------------- | -------------- |
| Linux         |           113 |             36 |
| macOS         |            81 |             42 |
| Windows       |            43 |             23 |


In order to speed up compile progress, split header-only-single-file [h2unit.h](../h2unit.h) into 
two files: [h2unit.cpp](../build/h2unit.cpp) and [h2unit.hpp](../build/h2unit.hpp). Refer to [`benchmark`](../benchmark/) or [`print`](../print/) for sample usage.

twofiles speed up 2~3 times than onefile.
