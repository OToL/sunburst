# Pillars

- Simple
- Coherent
- Useful

# Physical structure

- Commonly used macros/functions are located in engine/utl/cmake

- Files name containing commonly used macros/functions start with "sb", are lower case, '_' separated and use "cmake" extension
```
sb_utility.cmake
sb_target.cmake
```

# Design

- Functions are preferred to macros

# Style

- Global variables & constants are all capital, starts with "SB" and '_' separated
```
set(SB_TARGET_CPU "x86")
set(SB_TARGET_OS_ARCH "64")
```

- Functions & macros name start with 'sb' and use camel case
```
sbGetProjectTempDir()
sbAddLibrary
```

- CMake built-in functions are all lower-case
```
message("My message");
set(my_var "ny value")
```

- CMake built-in functions named parameters are all capital
```
string(FIND ...)
string(LENGTH ...)
```

- CMake built-it constants (e.g. TRUE) are all capital

- Local variables are lower-case and '_' separated

- Function input parameters are capital and '_' separated

- Function output starts with '_', are capital and '_' separated
