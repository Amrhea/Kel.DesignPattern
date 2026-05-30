cmake_minimum_required(VERSION 3.10)
project(KelDesignPattern)

# Gunakan standar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tambahkan direktori header agar bisa di-include tanpa path panjang
include_directories(include)
include_directories(tests)

# Mencari semua file .cpp di folder src dan sub-direktorinya secara otomatis
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Hapus main.cpp dan main_tui.cpp dari daftar SOURCES untuk target lain
list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/main_tui.cpp")

# Option untuk build TUI secara opsional
option(BUILD_TUI "Build TUI version with FTXUI" OFF)

# Tentukan nama executable dan file sumbernya
add_executable(PokerGame src/main.cpp ${SOURCES})

# Target TUI (Opsional)
if(BUILD_TUI)
    # Fetch FTXUI
    include(FetchContent)
    FetchContent_Declare(
      ftxui
      GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
      GIT_TAG v5.0.0
    )
    set(FTXUI_BUILD_TESTS OFF CACHE INTERNAL "")
    set(FTXUI_BUILD_EXAMPLES OFF CACHE INTERNAL "")
    FetchContent_MakeAvailable(ftxui)

    add_executable(PokerGameTUI src/main_tui.cpp ${SOURCES})
    target_link_libraries(PokerGameTUI PRIVATE ftxui::screen ftxui::dom ftxui::component)
    set_target_properties(PokerGameTUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

# Tambahkan target Testing
add_executable(TestRunner 
    tests/test_card.cpp 
    tests/test_hand_selection.cpp 
    tests/test_poker_evaluation.cpp 
    tests/test_scoring.cpp 
    tests/test_joker.cpp 
    tests/test_blind.cpp 
    tests/test_reward.cpp
    tests/test_run_integration.cpp
    tests/test_persistent_state.cpp
    tests/test_runtime_state.cpp
    tests/test_score_context.cpp
    tests/test_boundary_enforcement.cpp
    ${SOURCES}
)

# Mengatur output direktori agar rapi
set_target_properties(PokerGame TestRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


