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

# Hapus main.cpp dari daftar SOURCES untuk target Testing
list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp")

# Tentukan nama executable dan file sumbernya
add_executable(PokerGame src/main.cpp ${SOURCES})

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
