#!/bin/bash
echo "Compiling C++ to WebAssembly via Emscripten..."

SOURCES=(
    src/web/bindings.cpp
    src/blind/BigBlindState.cpp
    src/blind/BlindRule.cpp
    src/blind/BossBlindState.cpp
    src/blind/SmallBlindState.cpp
    src/hand_selection/Hand.cpp
    src/hand_selection/HandGenerator.cpp
    src/joker/ChipsBoostJoker.cpp
    src/joker/FlushMultJoker.cpp
    src/joker/JokerCard.cpp
    src/joker/JokerManager.cpp
    src/joker/MultBoostJoker.cpp
    src/joker/JokerFactory.cpp
    src/tag/TagFactory.cpp
    src/poker_evaluation/PokerHandEvaluator.cpp
    src/poker_evaluation/checker/FiveOfKindChecker.cpp
    src/poker_evaluation/checker/FlushChecker.cpp
    src/poker_evaluation/checker/FlushFiveChecker.cpp
    src/poker_evaluation/checker/FlushHouseChecker.cpp
    src/poker_evaluation/checker/FourOfKindChecker.cpp
    src/poker_evaluation/checker/FullHouseChecker.cpp
    src/poker_evaluation/checker/HighCardChecker.cpp
    src/poker_evaluation/checker/PairChecker.cpp
    src/poker_evaluation/checker/RoyalFlushChecker.cpp
    src/poker_evaluation/checker/StraightChecker.cpp
    src/poker_evaluation/checker/StraightFlushChecker.cpp
    src/poker_evaluation/checker/ThreeOfKindChecker.cpp
    src/poker_evaluation/checker/TwoPairChecker.cpp
    src/reward/BonusHandCommand.cpp
    src/reward/FreePlayingCardCommand.cpp
    src/reward/RewardManager.cpp
    src/reward/RewardRule.cpp
    src/reward/ShopSystem.cpp
    src/run/AnteManager.cpp
    src/run/HandPlayer.cpp
    src/run/RoundManager.cpp
    src/scoring/ScoringRule.cpp
    src/session/RuntimeSession.cpp
    src/state/BlindRuntimeState.cpp
    src/state/RunSessionState.cpp
)

OUT_DIR="web/public"
mkdir -p "$OUT_DIR"

echo "Running emcc..."
emcc -O3 --bind -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s EXPORT_NAME="createWasmGameModule" -Iinclude "${SOURCES[@]}" -o "$OUT_DIR/wasm_game.js"

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation success! Files written to $OUT_DIR/wasm_game.js and $OUT_DIR/wasm_game.wasm"
