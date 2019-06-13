# Pillars

- Simple
- Coherent
- Useful

# Physical structure

## Projects

- Projects name is ...
    - Lowercase,
    - Starts with a letter indicating its type:
        - w: thirdpaty/middleware wrapper,
        - s: System (filesystem, streaming, network, etc.),
        - g: Game framework (entity system, etc.).
    - Followed by a word describing its purpose e.g. input, core, etc.
```
score
sinput
sphysics
gentity
```

- Root projects directory is located in [engine root]/src
```
[engine root]/src/score
[engine root]/src/sinput
```

## Files

- Allowed extensions ...
    - .cpp
    - .h
    - .hpp

- Files name is made of one or more lowercase words seperated by '_'
```
coding_style.cpp
hash_str.h
```

- UnitTest files name shall be similar to the engine source file(s) they are testing and ends with "_test.cpp"
```
// Engine files implementing hashing functionality
hash_name.h
hash_name.cpp

// File containing hashing unit tests
hash_name_test.cpp
```

## Directories

- Directories name is lower case and does not exceed one word
```
core/memory/debug/
```

- Files not meant to be included by other projects are located in a directory named "_impl" at project's root
```
core/_impl/my_file.cpp
core/_impl/my_file.h
```

- When an implementation file has a public counterpart, it must follow the same directory structure
```
core/aa/bb/cc.h
core/_impl/aa/bb/cc.cpp
```

- Unit tests are located in a directory named "_test" at project's root
```
core/_test/
```

## Platform specific code

- Platform specific code must be in its own file which name ends with the target platform codename
```
filesystem_macos.cpp
filesystem_win.cpp
filesystem_linux.cpp
```

- When some platforms are sharing the same implementation (e.g. steam, posix, etc.), the actual code is in an hpp file included by the different platform sources. 
```c++
/// thread_macos.cpp
#include "thread_posix.hpp"

/// thread_linux.cpp
#include "thread_posix.hpp"
```

- Platform specific files are considered as implementation i.e. they shall be located in "\_impl" project's folder

- Including platform specific file from public headers should be avoided when possible


# Code Design

## struct vs class

- Structure should be used when it makes sense to have all object's data members public (e.g. 3D vector, Texture resource, etc.). Functions operating on the structure can be internal or external.

- Class should be used when the type is ...
    - Purely logical i.e. it only contains static members
```c++
class FileSystem
{
public:
    static b32 initialize();
    static b32 terminate();

    static File openFile(char const * path);
    // ...
};
```
    - Made of complex interdependent members which are not meant to be manipulated directly from outside e.g. containers, allocators, managers, etc.


## Macro

- C/C++ Macros shall be avoided

# Comment

## C-style comments only

- //: Comments explaining non-trivial implementation details e.g. algorithm, constants, etc.
- ///: Documentation e.g. TODO, public interface description, etc.

## Action Item

- TODO: [description]
```c++
// TODO: Add move assignment & ctor
```

- TOFIX: [description]
```c++
/// TOFIX: This vector should be normalized
```

# Namespace

- Engine projects shall only use "sb" namespace
```c++
namespace sb {}
```

- Projects wrapping middlewares shall only declare one namespace using their name
```c++
// [project root]/src/wstd
namespace wstd  {}

// [project root]/src/wgtest
namespace wgtest  {}

// [project root]/src/wglm
namespace wglm  {}
```

# Naming style

- Types (class, struct, type alias, etc.) use Camel case style
```c++
class MyClass {};
struct MyData {};
using MyTypeDef = MyData;
enum class MyEnum {};
```

- Interface classes name starts with a capital "I"
```c++
class IMyInterface {};
```

- Functions & Methods start with a lower case word followed by Camel case
```c++
void computeLambda() {}

class MyClass
{
    void timeOfTheDay() {}
};
```

- Variables are lowercase, '_' separated and start with characters indicating their ...
    1. scope:
        - '': local
        - 'g': global
        - 'm': member
    2. qualifier:
        - 's' : static
```c++
void Foo(int my_param) 
{
    static int s_var;
    int my_var;
}

int g_global_var;

static int gs_global_var;

struct MemberVariable
{
    int m_size; 
    static int ms_y;
};
```

- Constants are capital and '_' separated
```c++
constexpr float MY_PI = 3.14f;
enum class ConstantTypes
{
    VALUE_1,
    VALUE_2
};
```

- Macros used as constants shall be preceeded by "SB_"
```c++
#define SB_MY_CONSTANT 255
```

- Macros used as functions shall be preceeded by "sb"
```c++
#define sbMyMacroFunc(param) sbUnusedVar(param)
```

# Formatting

Customized LLVM style

# Exceptions

- Macros wrapping compiler intrinsics start with "sb_" and are lower-case
```c++
#define sb_inline
#define sb_forceinline
```

- Types defined in "core/stdex" use STL coding style when necessary.

- "detail" namespace can be used to hide implementation in files included from public headers (i.e. headers not located in _impl project folder)
```c++
// FileSystem helper functions
namespace sb { namespace detail {

    File openFile(char const *);

}}
```

- Unittests are located in a sub-namespace named 'test'
```c++
namespace sb { namespace test {}}
```

- Template types name start with T
```c++
template <typename TAllocator, typename TDest>
void foo() {}
```