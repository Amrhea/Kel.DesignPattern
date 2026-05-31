# Dokumentasi Build & Penggunaan Balatro Web

Panduan kompilasi C++ ke WebAssembly (Wasm) dan build frontend aplikasi React + Vite.

---

## 📋 Prasyarat (Prerequisites)

Sebelum melakukan build, pastikan sistem Anda telah memiliki:
1. **Node.js** (v18 ke atas) & **npm** untuk aplikasi React.
2. **Emscripten SDK (emsdk)** jika ingin mengompilasi core logic C++ ke Wasm.
   - Unduh & instal emsdk dari [https://emscripten.org](https://emscripten.org).
   - Pastikan `emcc` dapat diakses dari terminal/PATH Anda.

---

## 🛠️ Langkah-Langkah Build

### Opsi A: Jalankan Web Frontend Langsung (Out-of-the-Box)
Aplikasi web dilengkapi dengan simulasi engine game berbasis Javascript ([wasmGameMock.js](file:///D:/CODE/C++/Kel.DesignPattern/web/src/wasmGameMock.js)). Anda dapat langsung menjalankannya tanpa mengompilasi Wasm:

```bash
# 1. Masuk ke direktori web
cd web

# 2. Instal dependensi npm
npm install

# 3. Jalankan server lokal
npm run dev
```
Buka peramban di URL yang ditampilkan (contoh: `http://localhost:5173`).

---

### Opsi B: Kompilasi C++ ke WebAssembly (Wasm)

Untuk menggunakan core logic C++ asli yang dikompilasi ke Wasm:

#### 1. Kompilasi Kode Sumber C++
Jalankan skrip kompilasi yang sesuai dengan sistem operasi Anda dari root folder proyek:

- **Windows (Command Prompt / PowerShell)**:
  ```cmd
  compile_wasm.bat
  ```
- **Linux / macOS / Git Bash**:
  ```bash
  chmod +x compile_wasm.sh
  ./compile_wasm.sh
  ```

Skrip ini akan memanggil `emcc` untuk memproduksi berkas `wasm_game.js` dan `wasm_game.wasm` langsung ke folder `web/public/`.

#### 2. Jalankan dan Verifikasi di Browser
Jalankan perintah berikut di folder `web`:
```bash
npm run dev
```
Aplikasi web secara otomatis mendeteksi kehadiran file `wasm_game.js` di folder `public` dan mengganti engine simulasi menjadi **C++ WASM Core** (ditandai dengan badge emas di pojok kanan atas layar).

---

### Opsi C: Build untuk Produksi (Production Build)
Untuk melakukan kompilasi bundel aplikasi web statis siap-pakai:

```bash
cd web
npm run build
```
Hasil build akhir akan berada di folder `web/dist/`, berisi HTML, CSS, JS, dan file `.wasm` yang siap di-deploy ke GitHub Pages atau hosting statis lainnya.
