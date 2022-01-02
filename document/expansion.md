# Macro Expansion


## object-like macro

```C++
#define <identifier> <replacement token list> 
#define <宏名> <替换列表> 
```

## function-like macro

```C++
#define <identifier>(<parameter list>) <replacement token list> 
#define <宏名>(<参数列表>) <替换列表> 
```

function-like macro expansion occurs in the following stages:

- Stringification operations are replaced with the textual representation of their argument's replacement list (without performing expansion).
- Parameters are replaced with their replacement list (without performing expansion).
- Concatenation operations are replaced with the concatenated result of the two operands (without expanding the resulting token).
- Tokens originating from parameters are expanded.
- The resulting tokens are expanded as normal.

## Rescans

    每次宏展开的结果会被重复扫描(深度优先)，直到没有任何可展开的宏为止;

```C++
#define FOO A1 B1
#define A1 A2 
#define B1 B2 
#define A2 a2 
#define B2 b2 

FOO → a2 b2
    0→ FOO      // 第一次展开 FOO可展开为'A1 B1'
    1→ A1 B1    // 重复扫描 先处理A1, A1可展开为 'A2'
    2→ A2 B1    // 重复扫描A2 A2可展开为 'a2'
    3→ a2 B1    // 重复扫描a2,不是宏; 再处理1中pending的B1, B1可展开为 'B2'
    4→ a2 B2    // 重复扫描B2,B2可展开为 'b2'
    5→ a2 b2    // 重复扫描b2,不是宏; 完毕!
```

## self-reference macro

> A self-reference macro is one whose name appears in its definition set (DS). 
To prevent infinitely expansion, the self-reference is not considered a macro call. It is passed into the preprocessor output unchanged.


```C++
#define FOO FOO
FOO 
    0→ FOO₁ // 第一次扫描 FOO₁的DS为空{},可展开为'FOO'
    1→ FOO₂ // 第二次扫描 FOO₂的DS为{FOO},已在DS不可再展开; 完毕!
```

```C++
#define FOO 4 + FOO
FOO 
    0→ FOO₁     // 第一次扫描 FOO₁的DS为空{},可展开为'4 + FOO'
    1→ 4 + FOO₂ // 第二次扫描 FOO₂的DS为{FOO},已在DS不可再展开; 完毕!
```

## Join with next

    展开的宏和后面一个仅且一个Token,如果结合成function-like macro,并不在DS中,则使用新DS{}展开该新宏;
    并不是无差别重新扫描。

```C++
#define FOO a FOO BAR
#define BAR() FOO
FOO()()
    1→ FOO₁()₁()₂             // 第一次扫描 FOO₁的DS为空{},可展开为'a FOO BAR'
    2→ a FOO₂ BAR₁()₁()₂      // 第二次扫描 FOO₂的DS为{FOO},是self-reference不可再展开; BAR₁可与()₁结合,并且DS为{FOO},BAR₁()₁可用新的DS{}展开为'FOO'
    3→ a FOO₂ FOO₃()₂         // 第三次扫描 FOO₃的DS为{BAR},可展开为'a FOO BAR'
    4→ a FOO₂ a FOO₄ BAR₂()₂  // 第四次扫描 FOO₄{BAR,FOO}不可再展开; BAR₂{BAR,FOO}BAR₂已在DS,不可再展开; 完毕。
```

```C++
#define BAR() FOO
#define FOO() a BAR()()
FOO() → a FOO()
    1→ FOO₁()₁           // 第一次扫描 FOO₁的DS为空{},可展开为'a BAR()()'
    2→ a BAR₁()₂()₃      // 第二次扫描 BAR₁的DS为{FOO},BAR₁()₂可展开为'FOO' [注意BAR₁()₂不是结合而成]
    3→ a FOO₂()₃         // 第三次扫描 FOO₂的DS为{FOO,BAR}已在DS,不可再展开; 完毕!
```

```C++
#define BAR() FOO
#define FOO() a BAR
FOO()()() → a a BAR
    1→ FOO₁()₁()₂()₃     // 第一次扫描 FOO₁的DS为空{},可展开
    2→ a BAR₁()₂()₃      // 第二次扫描 BAR₁的DS为{FOO},BAR₁可与()₂结合,并且不在DS,BAR₁()₂可用新的DS{}展开
    3→ a FOO₂()₃         // 第三次扫描 FOO₂的DS为{BAR},FOO₂可与()₃结合,并且不在DS,FOO₂()₃可用新的DS{}展开
    4→ a a BAR₂          // 第四次扫描 BAR₂没有()不可展开; 完毕!
```

## Argument Prescan

> Macro arguments are completely macro-expanded before they are substituted into a macro body, unless they are stringized or pasted with other tokens. After substitution, the entire macro body, including the substituted arguments, is scanned again for macros to be expanded. The result is that the arguments are scanned twice to expand macro calls in them.

    参数的DS不会影响`function-like macro`的DS。

```C++
#define FOO(a) a
FOO(FOO(FOO(1)))
    1→ FOO₁(FOO₂(FOO₃(1)))    // 展开FOO₃(1) 为1
    2→ FOO₁(FOO₂(1))          // 展开FOO₂(1) 为1
    3→ FOO₁(1)                // 展开FOO₁(1) 为1
    4→ 1                      // 完毕!
```

    但参数展开后的结果在重扫描时仍然会沿用同一个DS。

```C++
#define BAR() FOO
#define FOO() a BAR()()
FOO() → a FOO()
    1→ FOO₁()₁           // 第一次扫描 FOO₁的DS为空{},可展开为'a BAR()()'
    2→ a BAR₁()₂()₃      // 第二次扫描 BAR₁的DS为{FOO},BAR₁()₂可展开为'FOO' [注意BAR₁()₂不是结合而成]
    3→ a FOO₂()₃         // 第三次扫描 FOO₂的DS为{FOO,BAR}已在DS,不可再展开; 完毕!

#define EVAL(_1) _1
EVAL(FOO())  → a FOO()
    1→ EVAL(FOO₁()₁)     // 先展开参数 
    2→ EVAL(a FOO₂()₃)   // 展开宏函数 EVAL; FOO₂的DS为{FOO,BAR}
    3→ a FOO₂()₃         // 重新扫描 FOO₂的DS为{FOO,BAR}已在DS,不可再展开; 完毕!
```

    参数展开的结果中即使有逗号（,）也不视为参数的分隔符。

```C++
#define FOO(a, b) a, b
FOO(FOO(FOO(1, 2), 3), 4) 
    1→ FOO₁(FOO₂(FOO₃(1, 2), 3), 4) // 展开最内层参数FOO₃(1, 2) 为1, 2; 1, 2被视为1个参数
    2→ FOO₁(FOO₂(1, 2, 3), 4)       // 展开最次层参数FOO₂(1, 2, 3) 为1, 2, 3; 1, 2, 3被视为1个参数
    3→ FOO₁(1, 2, 3, 4)             // 展开FOO₁(1, 2, 3, 4) 为1, 2, 3, 4
    4→ 1, 2, 3, 4                   // 完毕!
```

