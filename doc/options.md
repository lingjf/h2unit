# Options

```Shell
   ./a.out -h
```

*    `-b` [n=1] *break* test once n (default 1) cases failed
*    `-c` *continues* case asserts despite failure
*    `-d` *debug* mode, gdb attach
*    `-e` {pattern} *exclude* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains, default is ` ` (exclude nothing)
*    `-f` Only test last *failed* cases
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-j` [n] Fold *json* object or array, 0:unfold, 1:fold short, 2:fold same, 3:fold single (missing peer)
*    `-l` [suite case todo] *list* out suites and cases
*    `-m` Disable *memory* check(leak, overflow, trample, double free, asymmetric free)
*    `-n` [n=2] repeat run n *rounds*
*    `-o` {path} *output* junit compatible XML output
*    `-p` Disable test percentage *progressing* bar
*    `-q` *Quit" exit code as failed cases count
*    `-s` *shuffle* test cases in random/name/file/reverse order if no last failed, default is random, name is suite/case name alphabet order
*    `-S` [type] JSON C/C++ source code , *'* single, *"* double, *\\"*
*    `-v` [n=max] *verbose* output, 0:quiet 1:compact failed 2:compact passed 3:normal 4:details
*    `-w` Console output, *black-white* color style
*    `-x` Thrown *exception* is considered as failure
