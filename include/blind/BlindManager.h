#pragma once

#include <string>
#include <vector>
#include <memory>

enum class RoundStatus {
    PLAYING,
    WON,
    LOST
};

enum class BlindType {
    SMALL,
    BIG,
    BOSS
};

struct BlindData {
    BlindType type;
    std::string name;
    int targetScore;
    int handsLimit;
    int discardsLimit;
    int reward;

    BlindData(BlindType t, std::string n, int target, int hands, int discards, int r)
        : type(t), name(n), targetScore(target), handsLimit(hands), discardsLimit(discards), reward(r) {}
};

class Blind {
public:
    virtual ~Blind() = default;
    virtual BlindData GetData(int ante) const = 0;
};

class SmallBlindVariant : public Blind {
public:
    BlindData GetData(int ante) const override {
        return BlindData(BlindType::SMALL, "Small Blind", ante * 300, 4, 3, 3);
    }
};

class BigBlindVariant : public Blind {
public:
    BlindData GetData(int ante) const override {
        return BlindData(BlindType::BIG, "Big Blind", ante * 450, 4, 3, 4);
    }
};

class BossBlindVariant : public Blind {
public:
    BlindData GetData(int ante) const override {
        return BlindData(BlindType::BOSS, "Boss Blind", ante * 900, 4, 3, 5);
    }
};

class RoundState {
private:
    int targetScore;
    int currentScore;
    int handsRemaining;
    int discardsRemaining;
    RoundStatus status;

public:
    RoundState(const BlindData& data)
        : targetScore(data.targetScore), currentScore(0),
          handsRemaining(data.handsLimit), discardsRemaining(data.discardsLimit),
          status(RoundStatus::PLAYING) {}

    void AddScore(int score) {
        if (status != RoundStatus::PLAYING) return;
        currentScore += score;
        if (currentScore >= targetScore) {
            status = RoundStatus::WON;
        }
    }

    bool UseHand() {
        if (status != RoundStatus::PLAYING) return false;
        if (handsRemaining > 0) {
            handsRemaining--;
            if (currentScore < targetScore && handsRemaining == 0) {
                status = RoundStatus::LOST;
            }
            return true;
        }
        return false;
    }

    bool UseDiscard() {
        if (status != RoundStatus::PLAYING) return false;
        if (discardsRemaining > 0) {
            discardsRemaining--;
            return true;
        }
        return false;
    }

    int GetCurrentScore() const { return currentScore; }
    int GetTargetScore() const { return targetScore; }
    int GetHandsRemaining() const { return handsRemaining; }
    int GetDiscardsRemaining() const { return discardsRemaining; }
    RoundStatus GetStatus() const { return status; }
};

class BlindManager {
private:
    int ante;
    int blindIndex;
    std::vector<std::unique_ptr<Blind>> blinds;

public:
    BlindManager() : ante(1), blindIndex(0) {
        blinds.push_back(std::make_unique<SmallBlindVariant>());
        blinds.push_back(std::make_unique<BigBlindVariant>());
        blinds.push_back(std::make_unique<BossBlindVariant>());
    }

    BlindData GetCurrentBlindData() const {
        return blinds[blindIndex]->GetData(ante);
    }

    void NextBlind() {
        blindIndex++;
        if (blindIndex >= static_cast<int>(blinds.size())) {
            blindIndex = 0;
            ante++;
        }
    }

    int GetAnte() const { return ante; }
};
