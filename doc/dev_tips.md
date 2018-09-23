# Clang fornat

- Dump pre-defined style

    clang-format -dump-config -style=LLVM > .clang-format

- Deactivate clang format in a scope

```c++
// clang-format off
    void    unformatted_code  ;
// clang-format on
```


