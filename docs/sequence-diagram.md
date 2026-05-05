# Sequence Diagram - Poker Hand Evaluator

## Overview
This diagram shows the flow of execution when evaluating a poker hand using the Chain of Responsibility pattern.

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Main
    participant GM as GameManager
    participant HG as HandGenerator
    participant HH as HandHandler
    participant HF as FiveOfKindChecker
    participant RF as RoyalFlushChecker
    participant SF as StraightFlushChecker
    participant FK as FourOfKindChecker
    participant FH as FlushHouseChecker
    participant FU as FullHouseChecker
    participant FL as FlushChecker
    participant ST as StraightChecker
    participant TK as ThreeOfKindChecker
    participant TP as TwoPairChecker
    participant PA as PairChecker
    participant HC as HighCardChecker
    participant Score as ScoringRule
    participant Blind as BlindRule
    participant Reward as RewardRule

    Main->>GM: RunSession()
    GM->>HG: generateHand()
    HG-->>GM: Hand (5 cards)
    GM->>HH: ShowCards(hand)
    HH-->>GM: Display cards
    
    GM->>HH: evaluate(hand)
    HH->>HF: Handle(hand) -> invalid
    HH->>RF: Handle(hand) -> invalid
    HH->>SF: Handle(hand) -> invalid
    HH->>FK: Handle(hand) -> invalid
    HH->>FH: Handle(hand) -> invalid
    HH->>FU: Handle(hand) -> invalid
    HH->>FL: Handle(hand) -> invalid
    HH->>ST: Handle(hand) -> invalid
    HH->>TK: Handle(hand) -> invalid
    HH->>TP: Handle(hand) -> invalid
    HH->>PA: Handle(hand) -> invalid
    HH->>HC: Handle(hand) -> valid
    
    HH-->>GM: ChosenHand
    GM->>Score: calculateScore()
    Score-->>GM: finalScore
    GM->>Blind: getRequiredScore()
    Blind-->>GM: required
    GM->>Reward: calculateReward()
    Reward-->>GM: reward
    GM-->>Main: Print results
```

## Execution Flow

1. **Main** → `GameManager::RunSession()`
2. **HandGenerator** generates random 5-card hand
3. **HandHandler** displays cards
4. **Chain of Responsibility** evaluates hand:
   - Checks from rarest (Five of Kind) to commonest (High Card)
   - Each checker returns invalid → passes to next
   - First valid match returns result
5. **Scoring** calculates final score
6. **Blind** gets required score for level
7. **Reward** calculates money earned
8. Results printed to console

## Checker Order (Rarest → Commonest)

| Order | Hand | Priority |
|-------|------|----------|
| 1 | Five of a Kind | Highest |
| 2 | Royal Flush |
| 3 | Straight Flush |
| 4 | Four of a Kind |
| 5 | Flush House |
| 6 | Full House |
| 7 | Flush |
| 8 | Straight |
| 9 | Three of a Kind |
| 10 | Two Pair |
| 11 | Pair |
| 12 | High Card | Lowest |
