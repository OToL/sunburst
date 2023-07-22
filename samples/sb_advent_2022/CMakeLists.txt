project(sb_advent_2022 LANGUAGES CXX)

add_executable(sb_advent_2022
    src/advent_day_01.cpp
    src/advent_day_02.cpp
    src/advent_day_03.cpp
    src/advent_day_04.cpp
    src/advent_day_05.cpp
    src/advent_day_06.cpp
    src/advent_day_07.cpp
    src/advent_day_08.cpp
    src/advent_day_09.cpp
    src/advent_day_10.cpp
    src/advent_day_11.cpp
    src/advent_day_12.cpp
    src/advent_day_13.cpp
    src/advent_day_14.cpp
    src/advent_day_15.cpp
    src/advent_day_16.cpp
    src/advent_day_17.cpp
    src/advent_day_18.cpp
    src/advent_day_19.cpp
    src/advent_day_20.cpp
    src/advent_day_21.cpp
    src/advent_day_22.cpp
    src/utility.cpp
    src/main.cpp
    ${SB_ENGINE_MEMORY_HOOK_FILE_PATH})
  
target_include_directories(sb_advent_2022 PRIVATE 
    extern/inc) 

target_link_libraries(sb_advent_2022 
    PRIVATE
        sb::core
        sb::cxx_common_private)

