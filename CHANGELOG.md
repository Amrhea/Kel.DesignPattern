# Changelog

Semua perubahan penting dalam proyek **Kel.DesignPattern** akan didokumentasikan di sini.

## [Unreleased] - 2026-05-03

### Ditambahkan
- Implementasi `HandGenerator.cpp` untuk pembuatan kartu tangan secara otomatis.
- Class model baru: `Card` dan `ChosenHand` untuk manajemen data yang lebih baik.
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
