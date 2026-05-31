# Balatro Web - C++ WebAssembly Port

A premium interactive web interface for playing the Balatro C++ game directly in the browser using WebAssembly.

## 🚀 Live Deployment
Tautan deploy game Anda di GitHub Pages:
**`https://Amrhea.github.io/Kel.DesignPattern/`**

## Getting Started

### 1. Run the Web Application
No configuration required to play out-of-the-box using the high-fidelity JS Engine fallback:
```bash
# Navigate to web folder
cd web

# Start local server
npm run dev
```

### 2. Compile WebAssembly Core (Optional)
If you have **Emscripten (emsdk)** installed:
- **Windows**: Run `..\compile_wasm.bat` (from the `web` folder) or `compile_wasm.bat` from root.
- **Linux/macOS/Git Bash**: Run `bash ../compile_wasm.sh`.

Once compiled, the output `wasm_game.js` and `wasm_game.wasm` will be written to `public/`, and the web application will automatically switch to using the compiled C++ engine.

## Features
- **Curated Color Palettes**: Neon-green felt felt, neon blue, gold, and red accents.
- **Premium Glassmorphic UI**: High-end transparent blur panels.
- **Micro-animations**: Lift-off effects when selecting playing cards, pulsars for active blinds.
- **Live Terminal Logging**: Visual terminal at the bottom showing real-time feedback from the WASM/JS engine.
