# Options

```Shell
   ./a.out -h
```

*    `-a` *attend* to last failed cases, only test last failed cases
*    `-b` [n] *break* test once n (default 1) cases failed
*    `-c` Disable *colorful* output, black-white output
*    `-d/D` *debug* mode, -D for gdb attach but requires password
*    `-e` {pattern} *exclude* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains, default is ` ` (exclude nothing)
*    `-f` [n] *fold* json object or array, 0:unfold, 1:fold simple, 2:fold same, 3:fold missing peer
*    `-j` {path} generate *junit* compatible XML output
*    `-i` {pattern} *include* filter, suite name or case name wildcard (?, *) matches, if pattern don't contains ? and *, wildcard change to contains. Default is `*` (include all)
*    `-l` [suite case todo] *list* out suites and cases
*    `-m` Disable *memory* check(leak, overflow, trample, double free, asymmetric free)
*    `-p` Disable test percentage *progressing*
*    `-r` [n] repeat run n *rounds*
*    `-s` *shuffle* cases then test in random order if no last failed
*    `-v` [n] *verbose* output, 0:compact 2:normal 4:acronym default:all
*    `-x` Thrown *exception* is considered as failure
*    `-y` [n] *Copy-paste-able* C/C++ source code formatted JSON, 0:no quote, 1:quote by ', 2:quote by ", 3:quote by \\"
