# PILLARS

- Simple
- Coherent

# TECH TESTS

- [] imgui
- [] Vulkan macos

# TODO

## Document

- [] Document thirdparty update & build
- [] Update c++ coding style

## Misc

- [] Rename sandbox to ... sunburst
- [] Delete sandbox & upload sunburst to github
- [] Upload sb_playground to github

## Quality

- [] Try clang sanitizers
- [] Try static analyzers
- [] Re-enable clang formatting

## Code environment

- [] Use pre-compile headers

## thirdparty::gtest

- [] Rename ObjectTracker to ObjectStats
- [] Move AllocatorStats to sb::core e.g. as a composer
- [] Check for memory leak before returning from the program

## thirdparty::libc++

- [] Use std::span and replace sb::Span 
- [x] Move libc++ forwarding which are meant to be used only internally to a specific namespace (e.g. unique_ptr, etc.)
- [x] Make a pass on libc++ forward to make sure everything is used
- [x] Move ext libc++ to thirdparty\ext
- [x] Move libc++ to thirdparty\inc

## sb::core

- [] Check if we can change allocateUnique to rely on Tallocator& instead of reference_wrapper
- [] Move all platform implementation to a single folder e.g. core\_impl\api|platform
- [x] Rework file organization
- [x] Gather platform implemetation in _pimpl special folder
- [x] Re-enable temporarily deactivated unittests

## sb::core::container

- [] Finish SmallVector and there is one test deactivated

## sb::core::filesystem

- [x] FileSystem functions should deal only with FileHdl
- [x] Layer root must start and end with '/' separator
- [x] Possibility to specify multiple layers with the same name
- [x] Layer name must be unique
- [x] Open/CreateFile should try next layers (by name or not) when the first is failing
- [x] Use Generation ID as part of the FileHdl to detect access issue
- [x] Track number of files opened to issue an error if they are not all closed when terminating the FS
