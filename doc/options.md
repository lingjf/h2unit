# Options

```Shell
   ./a.out -h
```

*    `-b` [n=1] *break* test once n (default 1) cases failed
*    `-c` *continues* case asserts despite failure
*    `-d` *debug* mode, gdb attach
*    `-e` {pattern} *exclude* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains, default is ` ` (exclude nothing)
*    `-f` Only test last *failed* cases
*    `-F` [n] *Fold* json object or array, 0:unfold, 1:fold short, 2:fold same, 3:fold single (missing peer)
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-l` [suite case todo tags] *list* out suites and cases todos tags
*    `-m` Disable *memory* check(leak, overflow, trample, double free, asymmetric free)
*    `-o` {path} *output* junit compatible XML output
*    `-p` Disable test percentage *progressing* bar
*    `-q` *Quit" exit code as failed cases count
*    `-r` [n=2] repeat run n *rounds*
*    `-s` *shuffle* test cases in random/name/file/reverse order if no last failed, default is random, name is suite/case name alphabet order
*    `-S` [type] json C/C++ *source* code , *'* single, *"* double, *\\"*
*    `-t` include/exclude filter with tags
*    `-v` [n=max] *verbose* output
      * `0` quiet
      * `1` compact failed
      * `2` compact warning
      * `2` compact passed
      * `4` normal
      * `5` details
*    `-w` Console output, *black-white* color style
*    `-x` Thrown *exception* is considered as failure
