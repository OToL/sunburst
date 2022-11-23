# CODE QUALITY

- [ ] replace gtest
- [ ] static analyzers
- [ ] Increase the number of warnings as error (SB_TOOLCHAIN_*_WARNING_IGNORE_LIST)
- [ ] clang sanitizers
- [ ] Use fuzzing for unit tests (libfuzeer)
- [ ] Use clang-tidy

# BUILD SYSTEM

- [ ] Use pre-compile headers

# ENGINE

- [ ] memory system interface: malloc/free --> allocate/deallocate, allocateVMem-->reserveAndMapVMem 
- [ ] memory_system proper initialization during static init instead of using magic static
- [ ] Possibility to mount more than just FS (mountLocalFileSystem)
- [ ] move rpmalloc to _impl/extern
- [ ] Implement buffered FileInputStream
- [ ] Integrate an external profiler e.g. tracy
- [ ] Minimize span inclusion in header
- [ ] Minimize type_traits

# SAMPLES 

## IDEAS

- [ ] Implement imgui rendrer using Vulkan
- [ ] Cloth simulation
- [ ] Terrain rendering
- [ ] Vehicle/Boat simulation

## R/PLACE

- 
