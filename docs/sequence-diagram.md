# Sequence Diagram - Poker Hand Evaluator

## Overview
This diagram shows the flow of execution when evaluating a poker hand using the Chain of Responsibility pattern.

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Main
    participant GM as GameManager
    participant Session as RuntimeSession
    participant Blind as BlindState
    participant HH as HandHandler
    participant HG as HandGenerator
    participant Joker as JokerCard
    participant Cmd as RewardCommand

    Main->>GM: RunSession()
    Note over GM,Session: Initialize RuntimeSession (SmallBlindState active)

    loop While session.ante == 1
        GM->>Session: get currentBlind details
        Session->>Blind: getName(), getTargetScore(ante)
        Blind-->>GM: name, target score

        alt Skip Blind
            GM->>Session: skipBlind()
            Session->>Blind: createSkipRewardCommand()
            Blind-->>Session: Command instance (queued)
            Session->>Blind: nextState(ante)
            Blind-->>Session: Next BlindState
            Note over Session: executePendingCommands("NextBlind")
            Session->>Cmd: execute() (if timing matches)
            Session-->>GM: execution and transition logs
        else Play Blind
            GM->>HG: generateHand()
            HG-->>GM: Hand
            GM->>HH: evaluate(hand)
            HH-->>GM: ChosenHand
            Note over GM,Joker: NotifyObservers(handName, score)
            Joker->>GM: Modify score (Jokers)
            GM->>Session: playBlind()
            Session->>Blind: nextState(ante)
            Blind-->>Session: Next BlindState
            Note over Session: executePendingCommands("NextBlind")
            Session-->>GM: transition logs
        end
    end
```

## Execution Flow

1. **Main** → `GameManager::RunSession()`
2. **GameManager** initializes `RuntimeSession` (with `SmallBlindState` active).
3. **Loop** runs while `session.ante` is 1:
   - Gets current blind details (name, target score).
   - User inputs action (`play` or `skip`).
   - If **skip**:
     - `RuntimeSession::skipBlind()` queues the skip reward command from `currentBlind->createSkipRewardCommand()`.
     - State transitions via `currentBlind->nextState(ante)`.
     - `executePendingCommands` runs to trigger matching commands (`NextBlind` or `NextAnte`).
   - If **play**:
     - Hand is generated and evaluated via Chain of Responsibility.
     - Final score calculated (Template Method + Observer modifiers from Joker cards).
     - Gold reward added based on blind reward money.
     - `RuntimeSession::playBlind()` advances blind state.
4. Loop completes when `session.ante` increments.


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
