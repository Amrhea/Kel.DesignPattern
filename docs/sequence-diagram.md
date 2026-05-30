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
    participant PHE as PokerHandEvaluator
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
            GM->>RM: RunRound(session, blindData)
            Note over RM: Draw/refill player Hand from shuffled Deck
            loop While round.status == PLAYING
                RM->>PHE: Evaluate(playedHand)
                PHE-->>RM: HandEvaluation
                Note over RM,Joker: JokerManager::ApplyJokers(persistentState, context)
                Joker->>RM: Modify chips/mult in ScoreContext
                Note over RM: Add score to RoundState, discard played, draw new
            end
            RM-->>GM: round status (win/loss)
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
3. **Loop** runs while game state progresses:
   - Gets current blind details (name, target score).
   - User inputs action (`play` or `skip`).
   - If **skip**:
     - `RuntimeSession::skipBlind()` queues the skip reward command from `currentBlind->createSkipRewardCommand()`.
     - State transitions via `currentBlind->nextState(ante)`.
     - `executePendingCommands` runs to trigger matching commands (`NextBlind` or `NextAnte`).
   - If **play**:
     - GameManager delegates to `RoundManager::RunRound()` to manage hand play.
     - Hand is drawn from a shuffled `Deck`.
     - Cards played by user are evaluated via `PokerHandEvaluator` (Chain of Responsibility).
     - Final score calculated (Template Method + Observer modifiers from active `Joker` objects on temporary `ScoreContext`).
     - Gold reward added based on blind reward money.
     - `RuntimeSession::playBlind()` advances blind state.
4. Loop completes when `session.ante` increments or game is over.


## Checker Order (Rarest → Commonest)

| Order | Hand | Priority |
|-------|------|----------|
| 1 | Flush Five | Highest |
| 2 | Five of a Kind | |
| 3 | Royal Flush | |
| 4 | Straight Flush | |
| 5 | Four of a Kind | |
| 6 | Flush House | |
| 7 | Full House | |
| 8 | Flush | |
| 9 | Straight | |
| 10 | Three of a Kind | |
| 11 | Two Pair | |
| 12 | Pair | |
| 13 | High Card | Lowest |
