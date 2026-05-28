# Poker Hand Evaluator - Design Pattern Implementation

Sebuah aplikasi C++ untuk mendeteksi tipe poker hand dan mengkalkulasi skor dengan menggunakan berbagai **Design Patterns** (Chain of Responsibility, Observer, Strategy, Template Method, dan Singleton).

## Dokumentasi Analisis

Berikut adalah tautan dokumen analisis arsitektur proyek:

- **[Analisis Design Pattern](docs/design-pattern-analysis.md)**: Penjelasan lengkap setiap pola desain beserta class diagram (Mermaid).
- **[Sequence Diagram](docs/sequence-diagram.md)**: Alur runtime eksekusi program.
- **[Dokumentasi Otomasi Workspace](AUTOMATION.md)**: Panduan otomatisasi CLI tool.
- **[Changelog](CHANGELOG.md)**: Catatan riwayat perubahan proyek.

## Deskripsi

Proyek ini mensimulasikan sistem penilaian kartu mirip game *Balatro*. Sistem ini mengombinasikan pendeteksian 12 tipe poker hand menggunakan **Chain of Responsibility**, peningkatan skor dinamis menggunakan **Observer** (Joker Cards), fleksibilitas aturan menggunakan **Strategy**, standarisasi alur evaluasi menggunakan **Template Method**, serta kepastian manajemen instance tunggal menggunakan **Singleton**.

## Struktur Filesystem

```
D:\CODE\C++\Kel.DesignPattern\
├── include/                      # Folder Header untuk 8 Subsystem
│   ├── blind/                    # Interface & State Blind (State Pattern)
│   ├── card/                     # Model Kartu
│   ├── hand_selection/           # Hand & Generator
│   ├── joker/                    # Observer & Subject Joker
│   ├── poker_evaluation/         # CoR Checkers & HandHandler
│   ├── reward/                   # Command Reward (Command Pattern)
│   ├── run/                      # GameManager Facade & HandPlayer
│   └── scoring/                  # Template Method & Scoring Rule
├── src/                          # Folder Implementasi untuk 8 Subsystem
│   ├── blind/
│   ├── hand_selection/
│   ├── joker/
│   ├── poker_evaluation/
│   ├── reward/
│   ├── run/
│   ├── scoring/
│   └── main.cpp                  # Entry point minimal (bootstrap GameManager)
├── docs/                         # Diagram & Analisis
├── tests/                        # Unit Tests (Catch2)
│   ├── test_checkers.cpp
│   ├── test_scoring.cpp
│   ├── test_blind_progression.cpp
│   └── test_reward_commands.cpp
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Pattern yang Digunakan

### 1. Chain of Responsibility (Primary Pattern)

Objek chain dari handler - mendeteksi tipe kartu dari yang paling langka ke yang paling umum:
```
1. Five of a Kind  → 2. Royal Flush     → 3. Straight Flush
→ 4. Four of a Kind  → 5. Flush House     → 6. Full House
→ 7. Flush           → 8. Straight        → 9. Three of a Kind
→ 10. Two Pair       → 11. Pair           → 12. High Card (fallback)
```

### 2. Observer Pattern (Joker Cards)

Joker cards mendaftarkan diri sebagai `Observer` ke `GameManager` (sebagai `Subject`). Ketika sesi bermain mengevaluasi tangan, semua Joker terdaftar otomatis dipicu untuk memodifikasi skor pemain.

### 3. Strategy Pattern (Game Rules)

Aturan kalkulasi skor (`ScoringRule`), target skor blind (`BlindRule`), dan perhitungan gold reward (`RewardRule`) dipisahkan menjadi kelas strategi terpisah yang dapat diganti secara dinamis saat runtime (misal: `StandardScoring` vs `DoubleScoring`, `SmallBlind` vs `BossBlind`).

### 4. Template Method Pattern (Score Calculation Flow)

Menggunakan kelas `ScoreCalculator` untuk mendefinisikan langkah tetap perhitungan nilai tangan:
1. Pengecekan jenis tangan (`CheckPokerHand`)
2. Pengambilan base score (`GetBaseScore`)
3. Modifikasi custom score lewat derived class hook (`ModifyScore`)

### 5. State Pattern (Blind Progression)

Blind progression diimplementasikan menggunakan State Pattern tanpa `if-else` bertumpuk. State berpindah secara dinamis dari `SmallBlindState` → `BigBlindState` → `BossBlindState` → `SmallBlindState` (menaikkan nilai Ante).

### 6. Command Pattern (Skip Reward Commands)

Skip reward diimplementasikan menggunakan Command Pattern dengan deferred queue. Perintah seperti `BonusHandCommand` dan `FreePlayingCardCommand` dibungkus sebagai objek perintah yang dieksekusi secara tertunda sesuai timing pemicunya (`NextBlind`, `NextAnte`).

### 7. Singleton Pattern

`GameManager` diimplementasikan sebagai Singleton guna memastikan satu-satunya pengontrol sesi permainan yang diakses secara global via `GameManager::GetInstance()`.


## Cara Menjalankan

Proyek ini dibangun menggunakan **CMake**.

```bash
# 1. Konfigurasi build
cmake -B build -S .

# 2. Build proyek
cmake --build build

# 3. Jalankan Game utama
.\build\bin\PokerGame.exe

# 4. Jalankan Unit Test suite
.\build\bin\TestRunner.exe
```

## Tipe Poker Hand yang Didukung

| No | Tipe | Deskripsi | Contoh |
|----|------|-----------|--------|
| 1 | Five of a Kind | 5 kartu dengan rank sama (dengan wildcard) | A♠ A♥ A♦ A♣ A♠ |
| 2 | Royal Flush | 5 kartu berurutan, suit sama, dari 10-Ace | 10♠ J♠ Q♠ K♠ A♠ |
| 3 | Straight Flush | 5 kartu berurutan, suit sama (bukan Royal) | 7♠ 8♠ 9♠ 10♠ J♠ |
| 4 | Four of a Kind | 4 kartu dengan rank sama | K♠ K♥ K♦ K♣ 9♠ |
| 5 | Flush House | Flush + Full House kombinasi | 5♥ 5♠ 5♦ 5♣ 5♣ |
| 6 | Full House | 3 kartu sama + 2 kartu sama | 3♠ 3♥ 3♦ K♣ K♠ |
| 7 | Flush | 5 kartu dengan suit sama | 2♠ 5♠ 7♠ 9♠ K♠ |
| 8 | Straight | 5 kartu berurutan | 4♥ 5♦ 6♠ 7♣ 8♠ |
| 9 | Three of a Kind | 3 kartu dengan rank sama | 8♠ 8♥ 8♦ 2♣ 5♠ |
| 10 | Two Pair | 2 pasang kartu dengan rank sama | 9♠ 9♥ 3♦ 3♣ K♠ |
| 11 | Pair | 1 pasang kartu dengan rank sama | 7♠ 7♥ 2♦ 5♣ 9♠ |
| 12 | High Card | Tidak ada kombinasi khusus | 2♠ 5♦ 8♥ 10♣ K♠ |

## Extensibility

Proyek ini dirancang agar mudah diperluas:
1. **Menambah Joker Baru:** Buat subclass baru dari `Observer` / `JokerCard`.
2. **Menambah Strategi Rule Baru:** Buat implementasi konkret baru dari `IScoringStrategy`, `IBlindStrategy`, atau `IRewardStrategy`.
3. **Menambah Checker Tangan Baru:** Buat checker mewarisi `IPokerHandChecker`, daftarkan ke urutan chain di `HandHandler`.

## Dependencies

- C++17 atau lebih baru
- CMake 3.15 atau lebih baru
- Catch2 (untuk unit testing, disertakan otomatis)

## Author

- **syahrandywaskito** - System Programmer
- **Amrhea** - Mechanic Programmer
