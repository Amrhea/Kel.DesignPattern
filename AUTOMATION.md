# Workspace Automation Documentation

Dokumentasi ini merangkum semua fitur otomasi yang tersedia dalam repositori **Kel.DesignPattern** untuk memudahkan proses pengembangan, pembuatan build, dan pengujian.

## 1. Build System (CMake)
Repositori ini menggunakan **CMake** sebagai sistem build utama. Otomasi build dikonfigurasi dalam file `CMakeLists.txt`.

- **Project Name:** `KelDesignPattern`
- **C++ Standard:** C++17
- **Targets:**
  - `PokerGame`: Executable utama game.
  - `TestRunner`: Executable untuk menjalankan unit tests.
- **Output Directory:** Semua binary executable akan diletakkan di folder `build/bin/`.

### Perintah Manual:
```bash
# Konfigurasi project
cmake -S . -B build -G "MinGW Makefiles"

# Build project
cmake --build build
```

---

## 2. VS Code Tasks
Untuk pengguna Visual Studio Code, tersedia integrasi task otomasi yang dapat diakses melalui menu `Terminal -> Run Task` atau shortcut `Ctrl+Shift+B`.

| Task Label | Deskripsi | Shortcut |
|------------|-----------|----------|
| **CMake: Configure** | Menyiapkan environment build menggunakan CMake. | - |
| **CMake: Build** | Mengompilasi kode sumber menjadi executable. | `Ctrl+Shift+B` (Default) |
| **CMake: Run Tests** | Menjalankan unit tests menggunakan `TestRunner.exe`. | - |

---

## 3. Testing Automation (Catch2)
Otomasi pengujian diimplementasikan menggunakan framework **Catch2**.

- **File Pengujian:** `tests/test_checkers.cpp`
- **Cara Menjalankan:**
  - Melalui VS Code Task: `CMake: Run Tests`
  - Melalui Terminal: `./build/bin/TestRunner.exe`

---

## 4. Code Quality & Formatting
Repositori ini menerapkan standardisasi gaya penulisan kode (style guide).

- **Clang-Format:** Konfigurasi tersedia di file `.clang-format`.
- **VS Code Integration:** Otomasi format saat simpan (*Format on Save*) diaktifkan melalui `.vscode/settings.json`.
  - Menggunakan formatter: `ms-vscode.cpptools`.
  - Memerlukan `clang-format.exe` (Path terkonfigurasi di `D:/Msys2/...`).

---

## 5. IDE Settings Automation
Pengaturan workspace VS Code diotomatisasi melalui:
- **`.vscode/settings.json`**: Mengatur default formatter dan perilaku editor.
- **`.vscode/tasks.json`**: Mendefinisikan workflow build dan test.
- **`.claude/settings.local.json`**: Mengatur izin akses shell untuk asisten AI (Claude/Gemini).

---

## Ringkasan Workflow
Untuk memulai pengembangan:
1. Jalankan task **CMake: Configure**.
2. Modifikasi kode (Format akan otomatis diterapkan saat simpan).
3. Tekan `Ctrl+Shift+B` untuk **Build**.
4. Jalankan task **CMake: Run Tests** untuk memverifikasi perubahan.
