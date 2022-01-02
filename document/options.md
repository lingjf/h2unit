# Options

```Shell
   ./a.out -h
```

*    `-b` [n=1] *break* test once n (default 1) cases failed
*    `-c` *continues* case asserts despite failure
*    `-C` Console output, black-white *c*olor style
*    `-d` *debug* mode, gdb/lldb attach
*    `-e` {pattern} *exclude* filter, suite name or case name file:line tag name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains, default is ` ` (exclude nothing)
      * `file=` only filter suite or case file:line
      * `suite` only filter suite name
      * `case` only filter case name
      * `tags` only filter tag name
*    `-f` Only test last *failed* cases
*    `-F` [n] *Fold* json object and array
      * `0` do not fold json object and array
      * `1` fold short items
      * `2` fold same items
      * `3` fold single (missing peer)
*    `-i` {pattern} *include* filter, suite name or case name file:line tag name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
      * `file=` only filter suite or case file:line
      * `suite` only filter suite name
      * `case` only filter case name
      * `tags` only filter tag name
*    `-l` [suite case todo tags] *list* out suites and cases todos tags
*    `-m` Disable *memory* check(leak, overflow, trample, double free, asymmetric free)
*    `-o` {path} *output* junit compatible XML output
*    `-p` Disable test percentage *progressing* bar
*    `-r` [n=2] repeat run n *rounds*
*    `-s` *shuffle* test cases no last failed
      * `random` shuffle test cases random, and run in random order (default)
      * `name` sort test cases by suite/case name in alphabet order
      * `file` sort source file in alphabet order
      * `reverse` sort in reverse order
*    `-S` [type] json C/C++ *source* code , *'* single, *"* double, *\\"*
*    `-v` [n=max] *verbose* output
      * `0` quiet
      * `1` compact failed
      * `2` compact warning
      * `2` compact passed
      * `4` normal
      * `5` details
*    `-x` e*x*it code as failed cases count
*    `-W` [type...] Configure failure as *warning* 
      * `exception` consider thrown exception as warning
      * `uncaught` consider uncaught exception as warning
      * `leak` consider memory leak as warning
      * `violate` consider memory violate access as warning
      * `double_free` consider memory double free as warning
      * `asymmetric_free` consider memory asymmetric free as warning
*    `-h/?` [option] show all help tips, or specified option detail help
