import React, { useState, useEffect, useRef } from 'react';
import { WasmGameMock } from './wasmGameMock';
import './App.css';

export default function App() {
  const [game, setGame] = useState(null);
  const [isWasm, setIsWasm] = useState(false);
  const [loading, setLoading] = useState(true);

  // Live game state variables synced from C++ or JS core
  const [gameState, setGameState] = useState({
    ante: 1,
    gold: 4,
    deckSize: 52,
    inRound: false,
    roundScore: 0,
    roundTargetScore: 300,
    roundHandsRemaining: 4,
    roundDiscardsRemaining: 3,
    blindName: "Small Blind",
    blindTargetScore: 300,
    blindRewardMoney: 3,
    hand: [],
    selectedIndices: [],
    logs: [],
    shopItems: [],
    inShop: false,
    gameOver: false,
    handScores: {}
  });

  const logEndRef = useRef(null);

  // Initialize the game engine (prefer WASM if available)
  useEffect(() => {
    async function initGame() {
      let g = null;
      let wasmLoaded = false;
      
      try {
        if (typeof window.createWasmGameModule === 'function') {
          const module = await window.createWasmGameModule();
          g = new module.WasmGame();
          wasmLoaded = true;
          console.log("C++ WebAssembly core successfully loaded.");
        } else {
          g = new WasmGameMock();
          console.log("Using JS simulation core.");
        }
      } catch (e) {
        console.error("WASM load failed, falling back to JS Simulation core:", e);
        g = new WasmGameMock();
      }

      setGame(g);
      setIsWasm(wasmLoaded);
      setLoading(false);
    }
    initGame();
  }, []);

  // Update React state from the C++ or JS instance
  const syncState = () => {
    if (!game) return;

    // Get basic stats
    const ante = game.getAnte();
    const gold = game.getGold();
    const deckSize = game.getDeckSize();
    const inRound = game.getInRound();
    const roundScore = game.getRoundScore();
    const roundTargetScore = game.getRoundTargetScore();
    const roundHandsRemaining = game.getRoundHandsRemaining();
    const roundDiscardsRemaining = game.getRoundDiscardsRemaining();
    const blindName = game.getBlindName();
    const blindTargetScore = game.getBlindTargetScore();
    const blindRewardMoney = game.getBlindRewardMoney();

    // Get hand
    const handVector = game.getHand();
    const hand = [];
    const size = typeof handVector.size === 'function' ? handVector.size() : handVector.length;
    for (let i = 0; i < size; i++) {
      const card = typeof handVector.get === 'function' ? handVector.get(i) : handVector[i];
      // Convert char suit back to string if needed
      const suitChar = typeof card.suit === 'number' ? String.fromCharCode(card.suit) : card.suit;
      hand.push({ id: card.id, rank: card.rank, suit: suitChar });
    }

    // Get logs
    const logCount = game.getLogCount();
    const logs = [];
    for (let i = 0; i < logCount; i++) {
      logs.push(game.getLog(i));
    }

    // Check game over
    let gameOver = false;
    if (inRound && roundHandsRemaining <= 0 && roundScore < roundTargetScore) {
      gameOver = true;
    }

    // Get Shop Inventory
    const shopCount = game.getShopItemCount();
    const shopItems = [];
    for (let i = 0; i < shopCount; i++) {
      shopItems.push({
        index: i,
        desc: game.getShopItemDescription(i),
        price: game.getShopItemPrice(i)
      });
    }

    // Check if we just transitioned to shop (inventory has items, and not in round)
    const inShop = !inRound && shopItems.length > 0;

    // Get Jokers
    const jokersList = [];
    if (game.session && game.session.jokers) {
      const jkSize = typeof game.session.jokers.size === 'function' ? game.session.jokers.size() : game.session.jokers.length;
      for (let i = 0; i < jkSize; i++) {
        const j = typeof game.session.jokers.get === 'function' ? game.session.jokers.get(i) : game.session.jokers[i];
        jokersList.push(j.name || j.getName ? (j.name || j.getName()) : "Joker");
      }
    } else if (game.jokers) {
      // Fallback for JS mock
      game.jokers.forEach(jk => jokersList.push(jk.name));
    }

    // Get Hand Level stats
    const handScores = {};
    const possibleHands = [
      "High Card", "Pair", "Two Pair", "Three of a Kind", "Straight", 
      "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Flush"
    ];

    if (game.session && game.session.handScores) {
      // C++ mapping
      // We'll read it directly if possible, else use default display
      possibleHands.forEach(handName => {
        handScores[handName] = { level: 1, baseChips: 10, baseMult: 2 }; // Fallback
      });
    } else if (game.handScores) {
      // JS mockup mapping
      possibleHands.forEach(handName => {
        handScores[handName] = game.handScores[handName];
      });
    }

    setGameState(prev => ({
      ...prev,
      ante,
      gold,
      deckSize,
      inRound,
      roundScore,
      roundTargetScore,
      roundHandsRemaining,
      roundDiscardsRemaining,
      blindName,
      blindTargetScore,
      blindRewardMoney,
      hand,
      logs,
      shopItems,
      inShop,
      gameOver,
      jokers: jokersList,
      handScores
    }));
  };

  useEffect(() => {
    if (game) {
      syncState();
    }
  }, [game]);

  // Autoscroll logs
  useEffect(() => {
    if (logEndRef.current) {
      logEndRef.current.scrollIntoView({ behavior: 'smooth' });
    }
  }, [gameState.logs]);

  const handleStartRun = () => {
    game.startNewRun();
    syncState();
    setGameState(prev => ({ ...prev, selectedIndices: [] }));
  };

  const handleSkipBlind = () => {
    game.skipBlind();
    syncState();
  };

  const handleStartRound = () => {
    game.startRound();
    syncState();
    setGameState(prev => ({ ...prev, selectedIndices: [] }));
  };

  const handleSelectCard = (index) => {
    setGameState(prev => {
      const selected = [...prev.selectedIndices];
      if (selected.includes(index)) {
        return { ...prev, selectedIndices: selected.filter(i => i !== index) };
      } else {
        if (selected.length >= 5) return prev; // Max 5 cards
        return { ...prev, selectedIndices: [...selected, index] };
      }
    });
  };

  const handlePlayHand = () => {
    if (gameState.selectedIndices.length === 0) return;
    
    // Convert selectedIndices to C++ std::vector<int> if WASM
    let indicesParam;
    if (isWasm) {
      const module = window.Module || {};
      indicesParam = new window.Module.IntVector();
      gameState.selectedIndices.forEach(idx => indicesParam.push_back(idx));
    } else {
      indicesParam = gameState.selectedIndices;
    }

    game.playHand(indicesParam);
    syncState();
    setGameState(prev => ({ ...prev, selectedIndices: [] }));
  };

  const handleDiscard = () => {
    if (gameState.selectedIndices.length === 0) return;
    
    let indicesParam;
    if (isWasm) {
      indicesParam = new window.Module.IntVector();
      gameState.selectedIndices.forEach(idx => indicesParam.push_back(idx));
    } else {
      indicesParam = gameState.selectedIndices;
    }

    game.discardCards(indicesParam);
    syncState();
    setGameState(prev => ({ ...prev, selectedIndices: [] }));
  };

  const handleBuyShopItem = (idx) => {
    game.buyShopItem(idx);
    syncState();
  };

  const handleLeaveShop = () => {
    // Leave shop by advancing blind state if inventory is done
    // Or we skip active items to empty the shop inventory
    while (game.getShopItemCount() > 0) {
      game.buyShopItem(0); // This will fail or clean it
    }
    // JS mock has generateShop or advance
    if (game.advanceBlindState) {
      game.advanceBlindState();
    }
    syncState();
  };

  const getRankLabel = (rank) => {
    if (rank === 11) return 'J';
    if (rank === 12) return 'Q';
    if (rank === 13) return 'K';
    if (rank === 14) return 'A';
    return rank;
  };

  const getSuitSymbol = (suit) => {
    switch (suit) {
      case 'H': return '♥';
      case 'D': return '♦';
      case 'C': return '♣';
      case 'S': return '♠';
      default: return suit;
    }
  };

  if (loading) {
    return (
      <div className="loading-screen felt-background">
        <div className="loader"></div>
        <h2>Loading C++ Game Module...</h2>
      </div>
    );
  }

  return (
    <div className="app-container felt-background">
      {/* Top Navbar */}
      <header className="navbar glass-panel">
        <div className="nav-title">
          <span className="logo-icon">🃏</span>
          <h1>Balatro <span className="logo-highlight">Web Port</span></h1>
          <span className={`engine-badge ${isWasm ? 'engine-wasm' : 'engine-js'}`}>
            {isWasm ? 'C++ WASM Core' : 'JS Simulator Fallback'}
          </span>
        </div>
        
        <div className="nav-stats">
          <div className="nav-stat">
            <span className="stat-label">ANTE</span>
            <span className="stat-value glow-blue">{gameState.ante}</span>
          </div>
          <div className="nav-stat">
            <span className="stat-label">GOLD</span>
            <span className="stat-value glow-gold">${gameState.gold}</span>
          </div>
          <div className="nav-stat">
            <span className="stat-label">DECK</span>
            <span className="stat-value">{gameState.deckSize}</span>
          </div>
          <button className="btn btn-restart" onClick={handleStartRun}>Restart Run</button>
        </div>
      </header>

      {/* Main Layout Grid */}
      <div className="main-layout">
        
        {/* Left Control Panel */}
        <aside className="left-panel glass-panel">
          {/* Round Stats if in Round */}
          {gameState.inRound && (
            <div className="panel-section section-round-stats">
              <h3>ROUND STATUS</h3>
              <div className="round-progress-container">
                <div className="progress-labels">
                  <span>Score: {gameState.roundScore}</span>
                  <span>Target: {gameState.roundTargetScore}</span>
                </div>
                <div className="progress-bar-bg">
                  <div 
                    className="progress-bar-fill" 
                    style={{ width: `${Math.min(100, (gameState.roundScore / gameState.roundTargetScore) * 100)}%` }}
                  ></div>
                </div>
              </div>
              <div className="status-grid">
                <div className="status-item">
                  <span className="status-lbl">Hands Left</span>
                  <span className="status-val glow-gold">{gameState.roundHandsRemaining}</span>
                </div>
                <div className="status-item">
                  <span className="status-lbl">Discards</span>
                  <span className="status-val glow-red">{gameState.roundDiscardsRemaining}</span>
                </div>
              </div>
            </div>
          )}

          {/* Active Jokers Section */}
          <div className="panel-section section-jokers">
            <h3>ACTIVE JOKERS ({gameState.jokers?.length || 0})</h3>
            <div className="jokers-list">
              {gameState.jokers && gameState.jokers.length > 0 ? (
                gameState.jokers.map((joker, idx) => (
                  <div key={idx} className="joker-card-item">
                    <span className="joker-icon">🔥</span>
                    <span className="joker-name">{joker}</span>
                  </div>
                ))
              ) : (
                <div className="empty-state">No active jokers</div>
              )}
            </div>
          </div>

          {/* Upgraded Hands Levels */}
          <div className="panel-section section-hands">
            <h3>POKER HAND LEVELS</h3>
            <div className="hand-levels-list">
              {Object.entries(gameState.handScores).length > 0 ? (
                Object.entries(gameState.handScores).map(([handName, data]) => (
                  <div key={handName} className="hand-level-row">
                    <span className="hand-name">{handName}</span>
                    <div className="hand-badges">
                      <span className="lvl-badge">Lvl {data.level}</span>
                      <span className="score-badge">{data.baseChips} x {data.baseMult}</span>
                    </div>
                  </div>
                ))
              ) : (
                <div className="hand-level-row">
                  <span className="hand-name">High Card</span>
                  <span className="lvl-badge">Lvl 1</span>
                </div>
              )}
            </div>
          </div>
        </aside>

        {/* Center Board (Main Gameplay Screen) */}
        <main className="center-board">
          {gameState.gameOver ? (
            <div className="game-over-screen glass-panel floating">
              <h2 className="glow-red">GAME OVER</h2>
              <p>You failed to reach the target score of {gameState.roundTargetScore}!</p>
              <div className="final-stats">
                <div>Final Score: <span className="glow-gold">{gameState.roundScore}</span></div>
                <div>Ante reached: <span>{gameState.ante}</span></div>
              </div>
              <button className="btn btn-play" onClick={handleStartRun}>Start New Run</button>
            </div>
          ) : gameState.inShop ? (
            /* Shop Phase Screen */
            <div className="shop-screen glass-panel">
              <div className="shop-header">
                <h2>🛒 THE SHOP <span className="shop-sub">Spend your gold to upgrade</span></h2>
                <div className="gold-indicator glow-gold">Your Gold: ${gameState.gold}</div>
              </div>
              
              <div className="shop-shelf">
                {gameState.shopItems.length > 0 ? (
                  gameState.shopItems.map((item, idx) => (
                    <div key={idx} className="shop-item-card glass-panel">
                      <div className="item-art">⚡</div>
                      <div className="item-details">
                        <p className="item-description">{item.desc}</p>
                      </div>
                      <button 
                        className={`btn btn-buy ${gameState.gold < item.price ? 'btn-disabled' : ''}`}
                        onClick={() => handleBuyShopItem(item.index)}
                        disabled={gameState.gold < item.price}
                      >
                        Buy for ${item.price}
                      </button>
                    </div>
                  ))
                ) : (
                  <div className="empty-shelf">Shop is empty!</div>
                )}
              </div>

              <div className="shop-actions">
                <button className="btn btn-play" onClick={handleLeaveShop}>Next Blind</button>
              </div>
            </div>
          ) : !gameState.inRound ? (
            /* Blind Selection Screen */
            <div className="blind-select-screen glass-panel">
              <div className="blind-showcase">
                <div className={`blind-graphic ${
                  gameState.blindName.includes("Small") ? 'glow-blue' : 
                  gameState.blindName.includes("Big") ? 'glow-gold' : 'glow-red'
                }`}>
                  <span className="blind-symbol">👁️</span>
                  <h3>{gameState.blindName}</h3>
                </div>
                
                <div className="blind-details">
                  <div className="detail-row">
                    <span className="detail-lbl">Target Score</span>
                    <span className="detail-val glow-gold">{gameState.blindTargetScore}</span>
                  </div>
                  <div className="detail-row">
                    <span className="detail-lbl">Reward</span>
                    <span className="detail-val glow-gold">${gameState.blindRewardMoney}</span>
                  </div>
                </div>
              </div>

              <div className="blind-actions">
                <button className="btn btn-play" onClick={handleStartRound}>Play Blind</button>
                <button className="btn btn-skip" onClick={handleSkipBlind}>Skip Blind</button>
              </div>
            </div>
          ) : (
            /* Active Poker Table Screen */
            <div className="poker-table glass-panel">
              <div className="table-header">
                <h2>{gameState.blindName}</h2>
                <div className="round-score-panel">
                  Score: <span className="glow-gold">{gameState.roundScore}</span> / {gameState.roundTargetScore}
                </div>
              </div>

              {/* Hand cards display */}
              <div className="poker-hand">
                {gameState.hand.map((card, idx) => {
                  const isSelected = gameState.selectedIndices.includes(idx);
                  const isRed = card.suit === 'H' || card.suit === 'D';
                  return (
                    <div 
                      key={card.id} 
                      className={`playing-card ${isSelected ? 'card-selected' : ''}`}
                      onClick={() => handleSelectCard(idx)}
                    >
                      <div className="card-top flex-row">
                        <span className="card-rank">{getRankLabel(card.rank)}</span>
                        <span className={`card-suit ${isRed ? 'suit-red' : 'suit-black'}`}>
                          {getSuitSymbol(card.suit)}
                        </span>
                      </div>
                      <div className={`card-center ${isRed ? 'suit-red' : 'suit-black'}`}>
                        {getSuitSymbol(card.suit)}
                      </div>
                      <div className="card-bottom flex-row">
                        <span></span>
                        <span className="card-rank">{getRankLabel(card.rank)}</span>
                      </div>
                    </div>
                  );
                })}
              </div>

              {/* Action Buttons */}
              <div className="table-actions">
                <button 
                  className={`btn btn-play ${gameState.selectedIndices.length === 0 ? 'btn-disabled' : ''}`}
                  onClick={handlePlayHand}
                  disabled={gameState.selectedIndices.length === 0}
                >
                  Play Hand
                </button>
                <button 
                  className={`btn btn-skip ${gameState.selectedIndices.length === 0 ? 'btn-disabled' : ''}`}
                  onClick={handleDiscard}
                  disabled={gameState.selectedIndices.length === 0}
                >
                  Discard
                </button>
              </div>
            </div>
          )}
        </main>
      </div>

      {/* Bottom Live Logging Terminal */}
      <footer className="live-logs-terminal glass-panel">
        <div className="terminal-header">
          <span className="terminal-dot red-dot"></span>
          <span className="terminal-dot yellow-dot"></span>
          <span className="terminal-dot green-dot"></span>
          <span className="terminal-title">LIVE GAME LOGS ENGINE</span>
        </div>
        <div className="terminal-body">
          {gameState.logs.map((log, idx) => (
            <div key={idx} className="terminal-line">
              <span className="terminal-prompt">&gt;</span> {log}
            </div>
          ))}
          <div ref={logEndRef}></div>
        </div>
      </footer>
    </div>
  );
}
