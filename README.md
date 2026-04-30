# Poker Hand Evaluator - Design Pattern Implementation

Sebuah aplikasi C++ untuk mendeteksi tipe poker hand menggunakan **Chain of Responsibility Pattern**.

## Deskripsi

Proyek ini mengimplementasikan desain pattern **Chain of Responsibility** untuk mengecek dan mendeteksi 12 tipe poker hand secara berurutan, dari yang paling langka ke yang paling umum.

## Struktur Filesystem

```
D:\CODE\C++\Kel.DesignPattern\
├── src/
│   ├── checker/                  # Implementasi Checker (Concrete Handlers)
│   │   ├── FiveOfKindChecker.cpp/h
│   │   ├── FlushChecker.cpp/h
│   │   ├── FlushHouseChecker.cpp/h
│   │   ├── FourOfKindChecker.cpp/h
│   │   ├── FullHouseChecker.cpp/h
│   │   ├── HighCardChecker.cpp/h
│   │   ├── PairChecker.cpp/h
│   │   ├── RoyalFlushChecker.cpp/h
│   │   ├── StraightChecker.cpp/h
│   │   ├── StraightFlushChecker.cpp/h
│   │   ├── ThreeOfKindChecker.cpp/h
│   │   └── TwoPairChecker.cpp/h
│   ├── lib/                      # Header files & Utilities
│   │   ├── checker/              # Interface & Abstract Classes
│   │   │   └── *.h
│   │   ├── GameManage.h/cpp      # GameManager class
│   │   ├── Hand.h/cpp            # Hand class (data model)
│   │   ├── HandGenerator.h       # Generator interface (placeholder)
│   │   ├── HandHandler.h/cpp     # Chain of Responsibility Handler
│   │   ├── HandPlayer.h          # Player interface (placeholder)
│   │   ├── IPokerHandChecker.h   # Abstract Checker interface
│   │   ├── PokerHandUtils.h      # Utility functions for card analysis
│   │   ├── ScoringRule.h         # Scoring interface (placeholder)
│   │   ├── BlindRule.h           # Blind interface (placeholder)
│   │   └── RewardRule.h          # Reward interface (placeholder)
│   ├── GameManage.cpp            # Main game logic
│   └── main.cpp                  # Application entry point
├── .vscode/                      # VS Code configuration
├── .gitignore
└── README.md
```

## Pattern yang Digunakan

### 1. Chain of Responsibility (Primary Pattern)

**Descripsi:** Objek chain dari handler - masing-masing menangani permintaan yang berbeda. Permintaan dilewatkan sepanjang chain sampai handler menangkapnya.

**Implementasi:**

```
IPokerHandChecker (Abstract Handler)
    └── IPokerHandChecker* nextChecker
    └── virtual bool Check(hand) = 0
    └── bool Handle(hand) - meneruskan ke nextChecker

HandHandler (Concrete Handler)
    └── IPokerHandChecker* head
    └── void AddChecker(checker)
    └── bool Handle(hand)

└── Concrete Checkers (12 types):
    ├── FiveOfKindChecker (rarest)
    ├── RoyalFlushChecker
    ├── StraightFlushChecker
    ├── FourOfKindChecker
    ├── FlushHouseChecker
    ├── FullHouseChecker
    ├── FlushChecker
    ├── StraightChecker
    ├── ThreeOfKindChecker
    ├── TwoPairChecker
    ├── PairChecker
    └── HighCardChecker (most common)
```

**Chain Order (dari paling rare ke common):**
```
1.  Five of a Kind    → 2.  Royal Flush       → 3.  Straight Flush
→ 4.  Four of a Kind  → 5.  Flush House       → 6.  Full House
→ 7.  Flush           → 8.  Straight          → 9.  Three of a Kind
→ 10. Two Pair        → 11. Pair              → 12. High Card
```

### 2. Template Pattern (Implicit)

Setiap checker memiliki struktur yang sama:
- `Check()` - method utama yang dipanggil
- `Is<Type>()` - helper method untuk pengecekan spesifik

### 3. Singleton (Potential)

`GameManager` dirancang sebagai entry point tunggal untuk mengelola game session.

### 4. Interface/Abstract Class Pattern

- `IPokerHandChecker` - Abstract base class untuk semua checker
- `ScoringRule`, `BlindRule`, `RewardRule` - Interface untuk extensibility

## Cara Menjalankan

```bash
# Kompilasi
g++ -std=c++17 -o poker_main.exe src/main.cpp src/GameManage.cpp src/Hand.cpp src/HandHandler.cpp src/checker/*.cpp

# Jalankan
./poker_main.exe
```

Atau gunakan build system seperti CMake atau Makefile.

## Penggunaan

1. Jalankan program
2. Program akan menampilkan daftar checker dari paling mudah ke paling sulit muncul
3. Input nomor checker (1-12) untuk melihat hasil deteksi
4. Program akan menjalankan chain of responsibility dan menampilkan hasilnya

## Utility Functions (`PokerHandUtils.h`)

```cpp
// Mendapatkan rank dari setiap kartu
std::vector<int> GetRanks(const Hand& hand)

// Mendapatkan suit dari setiap kartu
std::vector<int> GetSuits(const Hand& hand)

// Menghitung jumlah kemunculan setiap rank
std::array<int, 13> GetRankCounts(const Hand& hand)

// Cek apakah ada rank dengan jumlah kemunculan tertentu
bool HasCount(const Hand& hand, int targetCount)

// Menghitung jumlah rank dengan kemunculan tertentu
int CountRanksWithOccurrences(const Hand& hand, int targetCount)

// Cek flush (5 kartu dengan suit sama)
bool IsFlush(const Hand& hand)

// Cek straight (5 kartu berurutan)
bool IsStraight(const Hand& hand)

// Cek royal flush
bool IsRoyalFlush(const Hand& hand)
```

## Card Representation

Kartu direpresentasikan sebagai integer 0-51:
- **Suit:** `card / 13` (0: Clubs, 1: Diamonds, 2: Hearts, 3: Spades)
- **Rank:** `card % 13` (0-12: Ace, 2-10, Jack, Queen, King)

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

Proyek ini dirancang untuk mudah dikembangkan:

1. **Menambah Checker Baru:**
   - Buat class baru mewarisi `IPokerHandChecker`
   - Implementasikan `Check()` method
   - Tambahkan ke chain di `HandHandler()` constructor

2. **Kustomisasi Rules:**
   - Modifikasi `ScoringRule`, `BlindRule`, `RewardRule` untuk aturan kustom

## Dependencies

- C++17 atau lebih baru
- Standard Library: `<vector>`, `<array>`, `<algorithm>`, `<iostream>`

## Author

- **syahrandywaskito** - Initial work

## License

This project is for educational purposes.
