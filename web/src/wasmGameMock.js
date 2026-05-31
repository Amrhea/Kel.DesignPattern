// WasmGameMock.js
// Mock C++ WasmGame module for client-side execution when WASM is not loaded.

export class WasmGameMock {
    constructor() {
        this.ante = 1;
        this.gold = 4;
        this.deck = [];
        this.jokers = [
            { name: "ChipsBoostJoker", bonus: 20 },
            { name: "MultiplierJoker", bonus: 2 },
            { name: "ConditionalJoker", target: "Flush", bonus: 15 }
        ];
        this.handScores = {
            "High Card": { level: 1, baseChips: 5, baseMult: 1 },
            "Pair": { level: 1, baseChips: 10, baseMult: 2 },
            "Two Pair": { level: 1, baseChips: 20, baseMult: 2 },
            "Three of a Kind": { level: 1, baseChips: 30, baseMult: 3 },
            "Straight": { level: 1, baseChips: 30, baseMult: 4 },
            "Flush": { level: 1, baseChips: 35, baseMult: 4 },
            "Full House": { level: 1, baseChips: 40, baseMult: 4 },
            "Four of a Kind": { level: 1, baseChips: 60, baseMult: 7 },
            "Straight Flush": { level: 1, baseChips: 100, baseMult: 8 },
            "Royal Flush": { level: 1, baseChips: 100, baseMult: 8 }
        };
        this.inRound = false;
        this.roundScore = 0;
        this.roundTargetScore = 0;
        this.roundHandsRemaining = 4;
        this.roundDiscardsRemaining = 3;
        this.currentHand = [];
        this.roundDeck = [];
        this.logs = [];
        this.shopInventory = [];

        this.startNewRun();
    }

    startNewRun() {
        this.ante = 1;
        this.gold = 4;
        this.inRound = false;
        this.roundScore = 0;
        this.roundHandsRemaining = 4;
        this.roundDiscardsRemaining = 3;
        
        // Generate persistent deck (52 cards)
        this.persistentDeck = [];
        let id = 1;
        const suits = ['H', 'D', 'C', 'S']; // Hearts, Diamonds, Clubs, Spades
        for (let suit of suits) {
            for (let rank = 2; rank <= 14; rank++) {
                this.persistentDeck.push({ id: id++, rank, suit });
            }
        }
        
        this.jokers = [
            { name: "ChipsBoostJoker", bonus: 20 },
            { name: "MultiplierJoker", bonus: 2 },
            { name: "ConditionalJoker", target: "Flush", bonus: 15 }
        ];

        this.logs = [];
        this.addLog("New run started! Ante 1.");
    }

    addLog(msg) {
        this.logs.push(msg);
        if (this.logs.length > 50) {
            this.logs.shift();
        }
    }

    getAnte() { return this.ante; }
    getGold() { return this.gold; }
    getDeckSize() { return this.persistentDeck.length; }
    getInRound() { return this.inRound; }
    getRoundScore() { return this.roundScore; }
    getRoundTargetScore() { return this.roundTargetScore; }
    getRoundHandsRemaining() { return this.roundHandsRemaining; }
    getRoundDiscardsRemaining() { return this.roundDiscardsRemaining; }

    getBlindName() {
        // Balatro small, big, boss blind logic
        const phases = ["Small Blind", "Big Blind", "Boss Blind"];
        const pIdx = (this.ante - 1) % 3;
        return phases[pIdx];
    }

    getBlindTargetScore() {
        // Scale with ante
        const base = 300;
        const multiplier = Math.pow(2, this.ante - 1);
        const pIdx = (this.ante - 1) % 3;
        const factor = pIdx === 0 ? 1 : (pIdx === 1 ? 1.5 : 2.0);
        return Math.floor(base * multiplier * factor);
    }

    getBlindRewardMoney() {
        const pIdx = (this.ante - 1) % 3;
        return pIdx === 0 ? 3 : (pIdx === 1 ? 4 : 5);
    }

    getHandLevel(name) {
        return this.handScores[name] ? this.handScores[name].level : 1;
    }

    getHandBaseChips(name) {
        return this.handScores[name] ? this.handScores[name].baseChips : 0;
    }

    getHandBaseMult(name) {
        return this.handScores[name] ? this.handScores[name].baseMult : 0;
    }

    skipBlind() {
        if (this.inRound) return;
        const reward = "Double Tag";
        this.addLog(`Skipping blind: ${this.getBlindName()}`);
        this.addLog(`[Reward queued: Skip reward - ${reward}]`);
        
        // Advance blind
        this.advanceBlindState();
        this.generateShop();
    }

    advanceBlindState() {
        this.ante++;
        this.addLog(`Advancing to Ante ${this.ante}...`);
    }

    startRound() {
        if (this.inRound) return;
        this.inRound = true;
        this.roundScore = 0;
        this.roundTargetScore = this.getBlindTargetScore();
        this.roundHandsRemaining = 4;
        this.roundDiscardsRemaining = 3;

        // Shuffle deck
        this.roundDeck = [...this.persistentDeck];
        this.shuffle(this.roundDeck);

        // Draw initial 8 cards
        this.currentHand = [];
        this.refillHand();
        this.addLog(`Round started! Target score: ${this.roundTargetScore}`);
    }

    shuffle(array) {
        for (let i = array.length - 1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i + 1));
            [array[i], array[j]] = [array[j], array[i]];
        }
    }

    refillHand() {
        const cardsNeeded = 8 - this.currentHand.length;
        for (let i = 0; i < cardsNeeded; i++) {
            if (this.roundDeck.length > 0) {
                this.currentHand.push(this.roundDeck.pop());
            }
        }
    }

    getHand() {
        return this.currentHand;
    }

    playHand(indices) {
        if (!this.inRound) return "Not in round";
        if (!indices || indices.length === 0) return "No cards selected";

        const selectedCards = indices.map(idx => this.currentHand[idx]).filter(Boolean);
        if (selectedCards.length === 0) return "Invalid selection";

        const evaluation = this.evaluatePokerHand(selectedCards);
        
        // Base scoring from handScores
        const handType = evaluation.handType;
        const scoreData = this.handScores[handType] || { baseChips: 5, baseMult: 1, level: 1 };
        
        let chips = scoreData.baseChips;
        let mult = scoreData.baseMult;

        // Apply jokers
        for (let joker of this.jokers) {
            if (joker.name === "ChipsBoostJoker") {
                chips += joker.bonus;
            } else if (joker.name === "MultiplierJoker") {
                mult += joker.bonus;
            } else if (joker.name === "ConditionalJoker") {
                if (handType === joker.target) {
                    mult += joker.bonus;
                }
            }
        }

        const scoreGained = chips * mult;
        this.roundScore += scoreGained;
        this.roundHandsRemaining--;

        // Remove played cards
        this.currentHand = this.currentHand.filter((_, i) => !indices.includes(i));
        this.refillHand();

        const resLog = `Played: ${handType} (Level ${scoreData.level}). Scored: ${chips} x ${mult} = ${scoreGained}`;
        this.addLog(resLog);

        if (this.roundScore >= this.roundTargetScore) {
            this.inRound = false;
            const reward = this.getBlindRewardMoney();
            this.gold += reward;
            this.addLog(`Round won! Gained $${reward}`);
            this.advanceBlindState();
            this.generateShop();
        } else if (this.roundHandsRemaining <= 0) {
            this.inRound = false;
            this.addLog(`Game Over! Failed to reach target score.`);
        }

        return resLog;
    }

    discardCards(indices) {
        if (!this.inRound) return;
        if (this.roundDiscardsRemaining <= 0) {
            this.addLog("No discards remaining!");
            return;
        }
        if (!indices || indices.length === 0) return;

        this.roundDiscardsRemaining--;
        this.currentHand = this.currentHand.filter((_, i) => !indices.includes(i));
        this.refillHand();
        this.addLog(`Discarded ${indices.length} cards.`);
    }

    // Very simple poker hand evaluator for the client UI
    evaluatePokerHand(cards) {
        if (cards.length === 0) return { handType: "High Card" };

        const sorted = [...cards].sort((a, b) => a.rank - b.rank);
        const ranks = sorted.map(c => c.rank);
        const suits = sorted.map(c => c.suit);

        const isFlush = suits.every(s => s === suits[0]) && cards.length >= 5;
        
        let isStraight = false;
        if (cards.length >= 5) {
            // Check consecutive unique ranks
            const uniqueRanks = [...new Set(ranks)].sort((a, b) => a - b);
            if (uniqueRanks.length >= 5) {
                for (let i = 0; i <= uniqueRanks.length - 5; i++) {
                    if (uniqueRanks[i+4] - uniqueRanks[i] === 4) {
                        isStraight = true;
                    }
                }
                // Ace low straight (A-2-3-4-5) -> 14, 2, 3, 4, 5
                if (uniqueRanks.includes(14) && uniqueRanks.includes(2) && uniqueRanks.includes(3) && uniqueRanks.includes(4) && uniqueRanks.includes(5)) {
                    isStraight = true;
                }
            }
        }

        // Frequencies
        const rankCounts = {};
        for (let r of ranks) {
            rankCounts[r] = (rankCounts[r] || 0) + 1;
        }
        const freq = Object.values(rankCounts).sort((a, b) => b - a);

        if (isFlush && isStraight) {
            if (ranks.includes(14) && ranks.includes(13) && ranks.includes(12) && ranks.includes(11) && ranks.includes(10)) {
                return { handType: "Royal Flush" };
            }
            return { handType: "Straight Flush" };
        }

        if (freq[0] >= 4) return { handType: "Four of a Kind" };
        if (freq[0] === 3 && freq[1] >= 2) return { handType: "Full House" };
        if (isFlush) return { handType: "Flush" };
        if (isStraight) return { handType: "Straight" };
        if (freq[0] === 3) return { handType: "Three of a Kind" };
        if (freq[0] === 2 && freq[1] === 2) return { handType: "Two Pair" };
        if (freq[0] === 2) return { handType: "Pair" };

        return { handType: "High Card" };
    }

    generateShop() {
        this.shopInventory = [];
        const possibleHands = Object.keys(this.handScores);
        for (let i = 0; i < 3; i++) {
            const r = Math.random();
            if (r < 0.5) {
                const hand = possibleHands[Math.floor(Math.random() * possibleHands.length)];
                this.shopInventory.push({
                    type: "Planet",
                    name: `Planet: ${hand}`,
                    price: 3,
                    description: `Upgrade ${hand} (+15 Chips, +2 Mult)`,
                    targetHand: hand
                });
            } else {
                const multVal = Math.floor(Math.random() * 3) + 2;
                this.shopInventory.push({
                    type: "Joker",
                    name: `Joker x${multVal}`,
                    price: 4,
                    description: `Add MultBoostJoker (+${multVal} Mult)`,
                    multBonus: multVal
                });
            }
        }
    }

    getShopItemCount() {
        return this.shopInventory.length;
    }

    getShopItemDescription(idx) {
        const item = this.shopInventory[idx];
        if (!item) return "";
        return `${item.name} - ${item.description} (Price: $${item.price})`;
    }

    getShopItemPrice(idx) {
        const item = this.shopInventory[idx];
        return item ? item.price : 999;
    }

    buyShopItem(idx) {
        const item = this.shopInventory[idx];
        if (!item) return false;

        if (this.gold >= item.price) {
            this.gold -= item.price;
            if (item.type === "Planet") {
                const handScore = this.handScores[item.targetHand];
                if (handScore) {
                    handScore.level++;
                    handScore.baseChips += 15;
                    handScore.baseMult += 2;
                }
            } else if (item.type === "Joker") {
                this.jokers.push({
                    name: "MultiplierJoker",
                    bonus: item.multBonus
                });
            }
            this.addLog(`Bought: ${item.name}`);
            this.shopInventory.splice(idx, 1);
            return true;
        }
        return false;
    }

    getLogCount() { return this.logs.length; }
    getLog(idx) { return this.logs[idx] || ""; }
    clearLogs() { this.logs = []; }
}
