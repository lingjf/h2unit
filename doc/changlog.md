# Backlog


## v5.16
1. remove CP primitive, OK instead
1. remove OK 3rd parameter, Matcher/n instead
1. remove IsTrue/IsFalse primitive, true/false instead
1. remove IsNull/NotNull primitive, NULL/Not(NULL) instead
1. Add Catch primitive to check catch throw exception
1. Add Member matcher to check struct/class member
1. Add MaxOf MinOf AvgOf MeanOf MedianOf statistics matcher
1. Add Range IsEven IsOdd IsNaN auxiliary matcher
1. Add TypeEq CastOf matcher
1. Percentage Approximate Eq comparing floating point 
1. Redesign executable options
1. Shuffle alphabet name file:line reverse
1. Filter by tags
1. List by tags
1. Suppport n! C(n,m) P(n,m) nan inf in JSON expression
1. Support STL container adaptors (stack/queue/priority_queue)
1. Has primitive support key value
1. Has item n times in container Has(...) * n
1. Add Every container matcher
1. Specify array size instead of OK 3rd parameter Has(...) / n
1. Specify subset of container Has(...) (start, end) or Has<start, end>(...)
1. Support support wchar_t wstring string_view
1. UNMEM primitive disable memory check
1. Improve trap into debugger 
1. User defined report H2Report primitive
1. no-access-control compiler option (access private and protected class member)
1. Refactoring example and document

## v5.15
1. Squash spaces in JSON and expression
1. Add SpaceLess/* primitive
1. Parenthesis logic expression
1. Redesign option
1. Continue assert option -c
1. Add case name in parameterized test
1. Transfer reference in matcher
1. Rename import main Macro H2UNIT_IMPORT_MAIN
1. Suppress warning Wall Werror
1. Polish progressing bar
1. Refactoring and bugfix

## v5.14
1. Eliminate all warnings
1. Update json compare algorithm
1. Add CP primitive
1. Enumerate progressing
1. Support Chinese CJK
1. Diff string with LCS algorithm and show space as dot to distinguish end
1. Remove In matcher(duplicated with AnyOf)
1. Has/Count change from matcher tuple to single matcher
1. Update OK with native array parameter, get array size
1. Update Me memcmp matcher, remove M1e/M8e/M16e/M32e/M64e, clarify deduce strategy
1. Simplify macro
1. Test c++11/14/17/20/23 versions
1. Fix several bugs
1. Refactoring

## v5.13
1. Cygwin works
1. MinGW works
1. WSL(Windows Subsystem Linux) works
1. MSBuild support
1. Print utf8/control character on windows
1. Safe matcher operator ~ and !
1. Progressing bar
1. Fold json element
1. Update json comparison

## v5.12
1. windows memory allocation override fix
1. windows colorful output
1. windows exception check
1. windows virtual function stub
1. stub function in dynamic library
1. continue assert even if fail
1. STUB/MOCK STUBS/MOCKS
1. linux arm64 x86 clang9/10 gcc8/9 ci

## v5.11
1. arm64 works
1. update MOCK/STUB primitives
1. windows memory violate check

## v5.10
1. Port to windows

## v5.9
1. Add performance benchmark PF
1. Redesign COUT
1. Redesign SOCK, add new Ptx Pij primitive
1. Update macOS mprotect() workaround

## v5.8
1. Capture thrown exception
1. Add Setup Teardown in SUITE
1. Merge memory leaks
1. Show full memory violates
1. Fix basename() usage crash bug and debug
1. List suites and cases without memory check
1. List suites and cases with filter
1. Default repeat 2 rounds with -r option
1. Refactoring

## v5.7
1. Compact console output without failure detail
1. Improve MOCK/STUB by function string name
1. Type-Parameterized test
1. MOCK/STUB private member function
1. UNMOCK/UNSTUB
1. UNMEM function exempt leak check
1. Fix several BUG
1. Refactoring

## v5.6
1. Unify MOCK/STUB primitive
1. Update memcmp matcher output
1. Caseless to json
1. Extend json to javascript
1. Illformed json hint
1. CountOf matcher
1. ListOf without content
1. Specified actual array size in OK
1. Formatted json print for copy paste
1. Execute only last failed cases
1. Coverage utility
1. Update MOCK failure output
1. JSON compare selector
1. Update options
1. Bugfix and refactoring

## v5.5
1. User specified precision when compare floating-point number.
1. MOCK support reference argument and return reference value.
1. MOCK delegate origin function when no returns specified.
1. MOCK STUB support template function, template class member function, and template member function.
1. MOCK STUB support no default constructor and abstract class.
1. MOCK STUB by function string name.
1. Suppress macro expand in OK expression.
1. BLOCK key-value attributes, limit, fill, align,noleak
1. SOCK key-value attributes, from, to
1. COUT attributes can not string type.
1. Odd align memory allocate.
1. Show check count.
1. Memory footprint.
1. Refactoring using effective C++ features.

## v5.4
1. full support STL container
1. memory inspection for overflow illegal access
1. improve failure print
1. refactoring

## v5.3
1. naive support windows msvc
1. improve memcmp matcher: Me(M1e,M8e,M16e,M32e,M64e), for bit/byte/word/dword/qword level
1. asymmetric allocate memory and release
1. access after free detection
1. multiply run round
1. update json diff render
1. safely enumerate case list
1. fix several BUGS
1. refactoring

## v5.2
1. capture syslog in COUT with "syslog" in parameter
1. fix stdio, stub, cout BUGS

## v5.1
1. Obsolete suite setup;
1. Attach user message to OK/JE;
1. More built-in Matcher, and && || ! logic operate for matchers;
1. User defined matcher;
1. Global setup and teardown;
1. Execution progressing;
1. Hijack dns resolve;
1. Hijack TCP/UDP transmission;
1. Capture stdout/err output;
1. Break into debugger;
1. Parameterized value generator;
1. ForEach;
1. ForFullmesh;
1. Type adaptive;
1. SOCK;
1. COUT;

## v4.0
1. Dynamic Mock;
1. Dynamic Stub;
1. Lambda make Mock/Stub fitness;
1. Section mode instead of fixture mode;
1. Non-invasive memory check, leak, overflow, double free;
1. Backtrace;
1. Simple check primitive OK;
1. Suppress some warning (eg. sign compare, writable-strings);
1. Incompatible api with v3/v2/v1;

## v3.0
1. Dynamic Mock and compatible with v2

## v2.0

## v1.0
