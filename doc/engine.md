# PILLARS

- Simple
- Coherent

# TECH TESTS

- [x] imgui basic test
- [ ] Vulkan simple triangle macos
- [ ] Implement imgui rendrer using Vulkan
- [ ] Cloth simulation
- [ ] Vehicle/Boat simulation

# CODE QUALITY

- [ ] clang sanitizers
- [ ] static analyzers
- [ ] Re-enable clang formatting
- [ ] Make SB_TOOLCHAIN_CLANG_WARNING_IGNORE_LIST smaller
- [ ] Use pre-compile headers

# CMAKE

- [ ] Investigate modern cmake best practices
- [ ] Clean definition of 3rd parties

# 3RD::GTEST

- [ ] Rename ObjectTracker to ObjectStats
- [ ] Move AllocatorStats to sb::core e.g. as a composer
- [ ] Check for memory leak before returning from the program

# 3RD::LIBC++

- [ ] Eradicate use of STD lib because of compile times and debug overhead (maybe C++ modules will solve this)
- [x] Use std::span and replace wstd::span 
- [x] Move libc++ forwarding which are meant to be used only internally to a specific namespace (e.g. unique_ptr, etc.)
- [x] Make a pass on libc++ forward to make sure everything is used
- [x] Move ext libc++ to thirdparty\ext
- [x] Move libc++ to thirdparty\inc

# SB::CORE

- [x] Move all platform implementation to a single folder e.g. core\_pimpl\
- [x] Check if we can change allocateUnique to rely on Tallocator& instead of reference_wrapper
- [x] Rework file organization
- [x] Gather platform implemetation in _pimpl special folder
- [x] Re-enable temporarily deactivated unittests
- [x] Finish SmallVector and there is one test deactivated
- [x] FileSystem functions should deal only with FileHdl
- [x] Layer root must start and end with '/' separator
- [x] Possibility to specify multiple layers with the same name
- [x] Layer name must be unique
- [x] Open/CreateFile should try next layers (by name or not) when the first is failing
- [x] Use Generation ID as part of the FileHdl to detect access issue
- [x] Track number of files opened to issue an error if they are not all closed when terminating the FS
