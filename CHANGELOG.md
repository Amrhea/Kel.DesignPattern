# Changelog

Semua perubahan penting dalam proyek **Kel.DesignPattern** akan didokumentasikan di sini.

## [Released] - 2026-05-30

### Ditambahkan (PR #37)
- **Subsystem H (Run & CLI Machine)**: Implementasi `GameManager` state machine facade, `AnteManager`, `RoundManager`, dan alur CLI state machine loop.
- **Subsystem I (Runtime State Separation)**: Pembagian state terpisah menjadi persistent (`RunPersistentState`), runtime (`BlindRuntimeState`), temporary (`ScoreContext`), dan composite root (`RunSessionState`).
- File pengujian test suite subsystem baru: `test_card.cpp`, `test_hand_selection.cpp`, `test_poker_evaluation.cpp`, `test_scoring.cpp`, `test_joker.cpp`, `test_blind.cpp`, `test_reward.cpp`, dan `test_run_integration.cpp` (menggantikan unit test lama).

### Diubah
- Refaktorisasi `RuntimeSession` untuk mengomposisi state layers dengan backward compatibility via reference fields.
- Sinkronisasi modifier Joker (`ChipsBoostJoker`, `MultBoostJoker`, `FlushMultJoker`) dengan warisan interface `Joker`.
- Pembaruan diagram sequence dan diagram kelas di folder `docs/`.

## [Released] - 2026-05-29

### Ditambahkan (PR #35)
- **Subsystem A (Deck & Card Lifecycle)**: Implementasi siklus dek, penambahan ID kartu unik, serta pemisah servis draw/discard.
- **Subsystem B (Hand Selection)**: Sistem seleksi kartu tangan berbasis Command pattern dengan fungsionalitas Undo dan validasi batas maksimal 5 kartu.
- **Subsystem C (Poker Hand Evaluation)**: Refaktorisasi deteksi kombinasi kartu poker ke Chain of Responsibility (CoR) terisolasi dengan class evaluasi `HandEvaluation`, serta penambahan deteksi tipe kombinasi kartu `Flush Five` (prioritas tertinggi).
- **Subsystem F (Blind & Round State)**: Implementasi pengelola progresi blind (`BlindManager`) dan pelacak kondisi ronde (`RoundState`) terpisah.
- File pengujian unit test baru: `tests/test_deck_hand.cpp`, `tests/test_selection.cpp`, dan `tests/test_blind_round.cpp`.

### Diubah
- Peningkatan diagram kelas dan dokumentasi visual pada [design-pattern-analysis.md](file:///D:/CODE/C++/Kel.DesignPattern/docs/design-pattern-analysis.md) dan [runtime-state-separation.md](file:///D:/CODE/C++/Kel.DesignPattern/docs/runtime-state-separation.md).
- Penghapusan kode `HandHandler` lama dan pembaruan struktur file di `README.md`.

## [Released] - 2026-05-28

### Ditambahkan
- Pola desain **State** untuk progression blind (`BlindState`, `SmallBlindState`, `BigBlindState`, `BossBlindState`).
- Pola desain **Command** untuk skip reward (`RewardCommand`, `BonusHandCommand`, `FreePlayingCardCommand`) beserta mekanisme deferred command queue.
- Unit test untuk progression blind dan reward command (`tests/test_blind_progression.cpp` dan `tests/test_reward_commands.cpp`).

### Diubah
- Restrukturisasi proyek menjadi **8 subsystem terisolasi**: `card`, `hand_selection`, `poker_evaluation`, `scoring`, `joker`, `blind`, `reward`, dan `run`.
- Folder header dipisah ke dalam `include/` dan folder implementasi ke `src/` untuk semua subsystem.
- Pembersihan include path di seluruh codebase menggunakan clean include path format (e.g., `#include "card/Card.h"`).
- Logika game loop didecouple penuh dari `main.cpp` dan diisolasi ke dalam `GameManager` (Subsystem `run`).
- `CMakeLists.txt` diperbarui untuk mendukung struktur direktori baru dan unit test baru.

## [Released] - 2026-05-20

### Ditambahkan
- Pola desain **Observer** untuk Joker Cards (`Observer.h`, `Subject.h`, `JokerCard.h/cpp`) yang memodifikasi skor poker secara dinamis.
- Pola desain **Strategy** untuk aturan permainan (`ScoringRule.h/cpp` memakai `IScoringStrategy`, `BlindRule.h/cpp` memakai `IBlindStrategy`, `RewardRule.h/cpp` memakai `IRewardStrategy`).
- Pola desain **Template Method** untuk kalkulasi skor (`ScoreCalculator.h`, `ConcreteScoreCalculators.h`).
- Pola desain **Singleton** untuk kelas `GameManager`.
- Kelas `HandPlayer` (`HandPlayer.h/cpp`) untuk mengelola gold dan daftar kartu Joker.
- Unit test komprehensif untuk memverifikasi keempat pola desain baru di `tests/test_checkers.cpp`.

## [Unreleased] - 2026-05-03

### Ditambahkan
- Implementasi `HandGenerator.cpp` untuk pembuatan kartu tangan secara otomatis.
- Class model baru: `Card` dan `HandEvaluation` untuk manajemen data yang lebih baik.
- Sistem build **CMake** (`CMakeLists.txt`) menggantikan sistem lama.
- Framework pengujian **Catch2** (`tests/catch.hpp`) dan unit test awal di `tests/test_checkers.cpp`.
- Dokumentasi `AUTOMATION.md` untuk panduan workflow dan otomatisasi workspace.
- Analisis pola desain mendalam di `docs/design-pattern-analysis.md`.

### Ditambahkan (PR #10)
- `std::unique_ptr` untuk manajemen memori `GameManager` (migrasi dari raw pointer).

### Diubah
- Refactor besar pada arsitektur *Chain of Responsibility* di `HandHandler`.
- Pembaruan logika pengecekan kartu di seluruh `src/checker/*.cpp` untuk efisiensi dan akurasi.
- Restrukturisasi folder: memisahkan header (`src/lib/`) dan implementasi (`src/`).
- Refactor `PokerHandUtils.h` untuk memusatkan fungsi bantuan kartu.
- `README.md` diperbarui dengan struktur proyek dan analisis pola desain yang komprehensif.

### Diubah (PR #10)
- `GameManager` sekarang menggunakan `HandGenerator` untuk pembuatan tangan secara acak (sebelumnya hardcoded `BuildSampleHand`).
- `HandGenerator::generateHand()` dikonversi dari metode `static` ke instance method.
- `BuildSampleHand` helper function dihapus.
- Optimisasi RNG: `random_device` dan `mt19937` dijalankan sekali (static), bukan recreating setiap panggilan.

### Diperbaiki
- Sinkronisasi antara `GameManage` dan mekanisme *Hand Handling*.
- Perbaikan berbagai bug kecil pada logika `HighCardChecker` dan `StraightChecker`.

### Diperbaiki (PR #10)
- API `HandGenerator` konsisten (instance method, bukan static).
- Fragile aggregate initialization di `HandGenerator.cpp:16` diperbaiki dengan inisialisasi eksplisit field.

---
*Dibuat otomatis berdasarkan sejarah commit git.*
