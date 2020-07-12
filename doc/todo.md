# TECH TESTS

- [~] Vulkan simple triangle macos
- [ ] Implement imgui rendrer using Vulkan
- [ ] Cloth simulation
- [ ] Terrain rendering
- [ ] Vehicle/Boat simulation

# CODE QUALITY

- [ ] replace gtest
- [ ] static analyzers
- [ ] Re-enable clang formatting
- [ ] Increase the number of warnings as error (SB_TOOLCHAIN_*_WARNING_IGNORE_LIST)
- [ ] clang sanitizers
- [ ] Use fuzzing for unit tests (libfuzeer)
- [ ] Use clang-tidy

# BUILD SYSTEM

- [ ] Investigate modern cmake best practices
- [ ] Investigate using CMake test command
- [ ] Use cmake format
- [ ] Use pre-compile headers
- [ ] Clean definition of 3rd parties
- [ ] Try writing config.cmake 
- [ ] Try using conan
- [ ] Use ninja

# 3RD::GTEST

- [ ] Rename ObjectTracker to ObjectStats
- [ ] Move AllocatorStats to sb::core e.g. as a composer
- [ ] Check for memory leak before returning from the program

# 3RD::LIBC++

- [ ] Cleanup wstd usage
- [ ] Try using eastl

# SB::CORE

- [ ] introduce byte type to replace ui8
- [ ] typedef the character type used for filesystem path i.e. char vs utf16 vs etc.

