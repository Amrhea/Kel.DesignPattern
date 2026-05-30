#include "session/RuntimeSession.h"
#include "run/AnteManager.h"
#include "blind/BlindManager.h"
#include "card/Card.h"
#include "card/Deck.h"
#include "hand_selection/Hand.h"
#include "hand_selection/HandServices.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include "joker/JokerManager.h"
#include "joker/ConcreteJokers.h"
#include "scoring/ScoreContext.h"
#include "scoring/ConcreteScoreCalculators.h"
#include "scoring/ScoringRule.h"
#include "reward/ShopSystem.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace ftxui;

enum class TUIState {
    MainMenu,
    StartRun,
    SelectBlind,
    PlayHand,
    ResolveScore,
    Reward,
    Shop,
    GameOver
};

Element RenderCardElement(const Card& card, bool selected, bool focused) {
    std::string suit_str = "";
    switch (card.suit) {
        case 'H': suit_str = "♥"; break;
        case 'D': suit_str = "♦"; break;
        case 'C': suit_str = "♣"; break;
        case 'S': suit_str = "♠"; break;
        default: suit_str = std::string(1, card.suit); break;
    }
    
    std::string rank_str = "";
    if (card.rank == 11) rank_str = "J";
    else if (card.rank == 12) rank_str = "Q";
    else if (card.rank == 13) rank_str = "K";
    else if (card.rank == 14) rank_str = "A";
    else rank_str = std::to_string(card.rank);

    auto color_type = (card.suit == 'H' || card.suit == 'D') ? Color::Red : Color::White;

    auto card_content = vbox({
        hbox({ text(rank_str) | bold, filler() }),
        hbox({ text(suit_str) | color(color_type) | bold }) | center | flex,
        hbox({ filler(), text(rank_str) | bold })
    }) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);

    if (focused) {
        if (selected) {
            return card_content | borderDouble | color(Color::Green);
        } else {
            return card_content | border | color(Color::Yellow);
        }
    } else {
        if (selected) {
            return card_content | borderDouble | color(Color::Green);
        } else {
            return card_content | border | color(Color::Blue);
        }
    }
}

class HandComponent : public ComponentBase {
private:
    const Hand& hand_;
    std::vector<bool>& selected_;
    int& active_index_;

public:
    HandComponent(const Hand& hand, std::vector<bool>& selected, int& active_index)
        : hand_(hand), selected_(selected), active_index_(active_index) {}

    Element Render() override {
        Elements card_elements;
        int count = hand_.GetCardCount();
        for (int i = 0; i < count; ++i) {
            bool focused = (i == active_index_ && Focused());
            card_elements.push_back(RenderCardElement(hand_.GetCard(i), selected_[i], focused));
        }
        if (card_elements.empty()) {
            return text("No cards in hand.") | center;
        }
        return hbox(std::move(card_elements)) | center;
    }

    bool OnEvent(Event event) override {
        int count = hand_.GetCardCount();
        if (count == 0) return false;

        if (event == Event::ArrowLeft) {
            active_index_ = (active_index_ - 1 + count) % count;
            return true;
        }
        if (event == Event::ArrowRight) {
            active_index_ = (active_index_ + 1) % count;
            return true;
        }
        if (event == Event::Character(' ') || event == Event::Return) {
            if (active_index_ >= 0 && active_index_ < (int)selected_.size()) {
                selected_[active_index_] = !selected_[active_index_];
                return true;
            }
        }
        return false;
    }

    bool Focusable() const override { return true; }
};

void RunMainMenu(TUIState& state) {
    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;
    std::vector<std::string> entries = {
        "Start New Run",
        "Exit Game"
    };
    
    auto menu = Menu(&entries, &selected);
    
    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("=== BALATRO TUI ===") | bold | center | color(Color::Yellow),
            separator(),
            text("Welcome to the Ultimate Balatro TUI Experience!") | center,
            separator(),
            menu->Render() | center | border,
            separator(),
            text("Use Up/Down Arrow keys to select, and Enter to confirm.") | dim | center
        }) | border | center;
    });

    auto catch_event = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            if (selected == 0) {
                state = TUIState::StartRun;
                screen.ExitLoopClosure()();
                return true;
            } else {
                exit(0);
            }
        }
        return false;
    });

    screen.Loop(catch_event);
}

void HandleSelectBlindTUI(RuntimeSession& session, AnteManager& anteManager, TUIState& state, std::vector<std::string>& game_logs) {
    auto screen = ScreenInteractive::TerminalOutput();
    
    std::string blind_name = anteManager.GetBlindName();
    int target_score = anteManager.GetTargetScore();
    int reward_money = anteManager.GetRewardMoney();
    int current_ante = anteManager.GetAnte();
    
    int selected = 0;
    std::vector<std::string> options = {
        "Play Blind",
        "Skip Blind",
        "View Stats & Deck",
        "Abandon Run"
    };
    
    auto menu = Menu(&options, &selected);
    
    auto layout = Renderer(menu, [&] {
        Elements log_elements;
        int start_idx = std::max(0, (int)game_logs.size() - 5);
        for (size_t i = start_idx; i < game_logs.size(); ++i) {
            log_elements.push_back(text(game_logs[i]) | color(Color::YellowLight));
        }
        if (log_elements.empty()) {
            log_elements.push_back(text("No logs yet."));
        }

        auto stats_panel = vbox({
            hbox({ text("Ante: ") | bold, text(std::to_string(current_ante)) | color(Color::Cyan) }),
            hbox({ text("Active Blind: ") | bold, text(blind_name) | color(Color::Magenta) }),
            hbox({ text("Target Score: ") | bold, text(std::to_string(target_score)) | color(Color::Yellow) }),
            hbox({ text("Reward Money: ") | bold, text("$" + std::to_string(reward_money)) | color(Color::Green) }),
            hbox({ text("Your Gold: ") | bold, text("$" + std::to_string(session.gold)) | color(Color::Green) }),
        }) | border | size(WIDTH, EQUAL, 40);

        auto log_panel = vbox({
            text("Game Logs") | bold | center,
            separator(),
            vbox(std::move(log_elements))
        }) | border | flex;

        return vbox({
            text("=== SELECT BLIND ===") | bold | center | color(Color::Yellow),
            separator(),
            hbox({
                stats_panel,
                log_panel
            }),
            separator(),
            menu->Render() | center | border,
            separator(),
            text("Use Up/Down to navigate, Enter to choose.") | dim | center
        }) | border;
    });

    auto catch_event = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            if (selected == 0) { // Play
                state = TUIState::PlayHand;
                screen.ExitLoopClosure()();
                return true;
            } else if (selected == 1) { // Skip
                auto skip_logs = anteManager.Skip();
                game_logs.insert(game_logs.end(), skip_logs.begin(), skip_logs.end());
                blind_name = anteManager.GetBlindName();
                target_score = anteManager.GetTargetScore();
                reward_money = anteManager.GetRewardMoney();
                current_ante = anteManager.GetAnte();
                return true;
            } else if (selected == 2) { // View Stats & Deck
                auto stats_screen = ScreenInteractive::TerminalOutput();
                auto close_btn = Button("Close", stats_screen.ExitLoopClosure());
                
                Elements joker_list;
                for (auto& observer : session.jokers) {
                    if (dynamic_cast<ChipsBoostJoker*>(observer.get())) {
                        joker_list.push_back(text("- ChipsBoostJoker (+20 Chips)"));
                    } else if (dynamic_cast<MultiplierJoker*>(observer.get())) {
                        joker_list.push_back(text("- MultiplierJoker (x2 Mult)"));
                    } else if (dynamic_cast<ConditionalJoker*>(observer.get())) {
                        joker_list.push_back(text("- ConditionalJoker (Flush: +15 Chips)"));
                    } else {
                        joker_list.push_back(text("- Unknown Joker"));
                    }
                }
                if (joker_list.empty()) {
                    joker_list.push_back(text("No Jokers."));
                }

                Elements deck_list;
                for (const auto& card : session.persistentDeck) {
                    std::string s(1, card.suit);
                    deck_list.push_back(text("Rank " + std::to_string(card.rank) + ", Suit " + s));
                }

                auto stats_layout = Renderer(close_btn, [&] {
                    return vbox({
                        text("=== PLAYER STATS & DECK ===") | bold | center | color(Color::Yellow),
                        separator(),
                        hbox({
                            vbox({
                                text("Jokers:") | bold,
                                vbox(joker_list)
                            }) | border | flex,
                            vbox({
                                text("Deck (" + std::to_string(session.persistentDeck.size()) + " cards):") | bold,
                                vbox(std::move(deck_list)) | vscroll_indicator | frame | size(HEIGHT, EQUAL, 10)
                            }) | border | flex
                        }),
                        separator(),
                        close_btn->Render() | center
                    }) | border;
                });
                stats_screen.Loop(stats_layout);
                return true;
            } else if (selected == 3) { // Abandon
                state = TUIState::GameOver;
                screen.ExitLoopClosure()();
                return true;
            }
        }
        return false;
    });

    screen.Loop(catch_event);
}

void HandlePlayRoundTUI(RuntimeSession& session, AnteManager& anteManager, TUIState& state, std::vector<std::string>& game_logs) {
    auto screen = ScreenInteractive::TerminalOutput();
    
    Deck deck;
    for (const auto& card : session.persistentDeck) {
        deck.AddCard(card);
    }
    deck.Shuffle();

    std::string blind_name = anteManager.GetBlindName();
    int target = anteManager.GetTargetScore();
    int reward = anteManager.GetRewardMoney();
    
    BlindType type = BlindType::SMALL;
    if (blind_name == "Big Blind") type = BlindType::BIG;
    else if (blind_name == "Boss Blind") type = BlindType::BOSS;

    BlindData blindData(type, blind_name, target, session.remainingPlays, 3, reward);
    RoundState round(blindData, session.remainingPlays);
    Hand hand;
    std::vector<Card> discardPile;

    DrawService::DrawToHand(deck, hand, 8);
    
    std::vector<bool> selected(hand.GetCardCount(), false);
    int active_card_idx = 0;
    std::vector<std::string> round_logs;
    round_logs.push_back("Round started: " + blind_name);

    auto hand_comp = std::make_shared<HandComponent>(hand, selected, active_card_idx);

    auto play_hand_cb = [&]() {
        std::vector<int> selectedIndices;
        for (int i = 0; i < hand.GetCardCount(); ++i) {
            if (selected[i]) {
                selectedIndices.push_back(i);
            }
        }

        if (selectedIndices.empty()) {
            round_logs.push_back("No cards selected!");
            return;
        }

        Hand playedHand;
        for (int index : selectedIndices) {
            playedHand.AddCard(hand.GetCard(index));
        }

        PokerHandEvaluator handEvaluator;
        HandEvaluation eval = handEvaluator.Evaluate(playedHand);
        if (!eval.isValid()) {
            round_logs.push_back("Not a valid hand!");
            return;
        }

        JokerManager& jokerManager = JokerManager::GetInstance();
        jokerManager.ClearObservers();
        for (auto& joker : session.jokers) {
            jokerManager.RegisterObserver(joker.get());
        }

        auto scoringRule = std::make_unique<ScoringRule>(std::make_unique<BaseScoringRule>());
        PlayedHandResult baseResult = scoringRule->calculateScore(eval.handType, session.handScores);
        ScoreContext context(eval.handType, eval.handName, baseResult.chips, baseResult.mult, playedHand.GetCards());

        round_logs.push_back("Played: " + context.handName + " (Lvl " + std::to_string(baseResult.level) + ")");
        round_logs.push_back("Base: " + std::to_string(context.chips) + " x " + std::to_string(context.mult));

        jokerManager.NotifyObservers(context);
        int finalScore = context.calculateFinalScore();
        round_logs.push_back("Final: " + std::to_string(context.chips) + " x " + std::to_string(context.mult) + " = " + std::to_string(finalScore));

        round.UseHand();
        round.AddScore(finalScore);

        std::vector<Card> dummyDiscard;
        DiscardService::Discard(hand, selectedIndices, dummyDiscard);
        DrawService::DrawToHand(deck, hand, 8 - hand.GetCardCount());

        selected.assign(hand.GetCardCount(), false);
        active_card_idx = 0;

        if (round.GetStatus() == RoundStatus::WON) {
            game_logs.push_back("Defeated " + blind_name + "!");
            state = TUIState::ResolveScore;
            screen.ExitLoopClosure()();
        } else if (round.GetStatus() == RoundStatus::LOST) {
            game_logs.push_back("Failed " + blind_name + "...");
            state = TUIState::GameOver;
            screen.ExitLoopClosure()();
        }
    };

    auto discard_cb = [&]() {
        if (round.GetDiscardsRemaining() <= 0) {
            round_logs.push_back("No discards left!");
            return;
        }

        std::vector<int> selectedIndices;
        for (int i = 0; i < hand.GetCardCount(); ++i) {
            if (selected[i]) {
                selectedIndices.push_back(i);
            }
        }

        if (selectedIndices.empty()) {
            round_logs.push_back("Select cards to discard!");
            return;
        }

        round.UseDiscard();
        DiscardService::Discard(hand, selectedIndices, discardPile);
        DrawService::DrawToHand(deck, hand, 8 - hand.GetCardCount());

        round_logs.push_back("Discarded " + std::to_string(selectedIndices.size()) + " cards.");

        selected.assign(hand.GetCardCount(), false);
        active_card_idx = 0;
    };

    auto give_up_cb = [&]() {
        game_logs.push_back("Gave up round.");
        state = TUIState::GameOver;
        screen.ExitLoopClosure()();
    };

    auto play_btn = Button("Play Hand", play_hand_cb);
    auto discard_btn = Button("Discard", discard_cb);
    auto give_up_btn = Button("Give Up", give_up_cb);

    auto buttons_container = Container::Horizontal({
        play_btn,
        discard_btn,
        give_up_btn
    });

    auto main_container = Container::Vertical({
        hand_comp,
        buttons_container
    });

    auto layout = Renderer(main_container, [&] {
        double score_ratio = (double)round.GetCurrentScore() / (double)round.GetTargetScore();
        if (score_ratio > 1.0) score_ratio = 1.0;
        
        auto score_gauge = gauge(score_ratio) | color(Color::Green);
        
        auto left_panel = vbox({
            text("ROUND DETAILS") | bold | center | color(Color::Cyan),
            separator(),
            hbox({ text("Active Blind: "), text(blind_name) | color(Color::Magenta) | bold }),
            separator(),
            hbox({ text("Hands Left: "), text(std::to_string(round.GetHandsRemaining())) | color(Color::Yellow) | bold }),
            hbox({ text("Discards Left: "), text(std::to_string(round.GetDiscardsRemaining())) | color(Color::Yellow) | bold }),
            separator(),
            hbox({ text("Target Score: "), text(std::to_string(round.GetTargetScore())) | color(Color::Red) | bold }),
            hbox({ text("Current Score: "), text(std::to_string(round.GetCurrentScore())) | color(Color::Green) | bold }),
            vbox({
                text("Progress:") | dim,
                score_gauge
            }) | size(HEIGHT, EQUAL, 2)
        }) | border | size(WIDTH, EQUAL, 35);

        Elements joker_elements;
        for (auto& observer : session.jokers) {
            if (dynamic_cast<ChipsBoostJoker*>(observer.get())) {
                joker_elements.push_back(text("- ChipsBoostJoker") | color(Color::Cyan));
            } else if (dynamic_cast<MultiplierJoker*>(observer.get())) {
                joker_elements.push_back(text("- MultiplierJoker") | color(Color::Magenta));
            } else if (dynamic_cast<ConditionalJoker*>(observer.get())) {
                joker_elements.push_back(text("- ConditionalJoker") | color(Color::Yellow));
            } else {
                joker_elements.push_back(text("- Unknown Joker"));
            }
        }
        if (joker_elements.empty()) {
            joker_elements.push_back(text("No Jokers") | dim);
        }

        Elements log_elements;
        int start_log = std::max(0, (int)round_logs.size() - 8);
        for (size_t i = start_log; i < round_logs.size(); ++i) {
            log_elements.push_back(text(round_logs[i]) | color(Color::White));
        }

        auto right_panel = vbox({
            text("ACTIVE JOKERS") | bold | center | color(Color::Magenta),
            separator(),
            vbox(std::move(joker_elements)),
            separator(),
            text("ROUND LOGS") | bold | center | color(Color::Yellow),
            separator(),
            vbox(std::move(log_elements)) | flex
        }) | border | size(WIDTH, EQUAL, 40);

        auto middle_panel = vbox({
            text("YOUR HAND") | bold | center,
            separator(),
            hand_comp->Render() | center | flex,
            separator(),
            text("Select: [Space/Enter] | Navigate: [Left/Right] | Actions: [Down]") | dim | center
        }) | border | flex;

        auto bottom_panel = vbox({
            text("ACTIONS") | bold | center,
            separator(),
            buttons_container->Render() | center
        }) | border;

        return vbox({
            hbox({
                left_panel,
                middle_panel,
                right_panel
            }) | flex,
            bottom_panel
        });
    });

    screen.Loop(layout);
}

void HandleRewardTUI(RuntimeSession& session, AnteManager& anteManager, TUIState& state, std::vector<std::string>& game_logs) {
    auto screen = ScreenInteractive::TerminalOutput();
    int rewardMoney = anteManager.GetRewardMoney();
    session.addGold(rewardMoney);
    
    std::string reward_msg = "[Reward] Earned $" + std::to_string(rewardMoney) + " for completing the blind.";
    std::string total_gold = "[Reward] Total Gold: $" + std::to_string(session.gold);
    game_logs.push_back(reward_msg);
    game_logs.push_back(total_gold);
    
    auto btn = Button("Go to Shop", screen.ExitLoopClosure());
    auto layout = Renderer(btn, [&] {
        return vbox({
            text("=== BLIND CLEARED ===") | bold | center | color(Color::Green),
            separator(),
            text(reward_msg) | center,
            text(total_gold) | center,
            separator(),
            btn->Render() | center
        }) | border | center;
    });
    
    screen.Loop(layout);
    state = TUIState::Shop;
}

void HandleShopTUI(RuntimeSession& session, AnteManager& anteManager, TUIState& state, std::vector<std::string>& game_logs) {
    auto screen = ScreenInteractive::TerminalOutput();
    
    ShopSystem shop;
    shop.GenerateInventory();
    
    int selected = 0;
    
    auto run_shop = [&]() {
        while (true) {
            std::vector<std::string> options;
            const auto& inventory = shop.GetInventory();
            for (size_t i = 0; i < inventory.size(); ++i) {
                options.push_back(inventory[i].description + " ($" + std::to_string(inventory[i].price) + ")");
            }
            options.push_back("Leave Shop");
            
            if (selected >= (int)options.size()) {
                selected = std::max(0, (int)options.size() - 1);
            }
            
            auto menu = Menu(&options, &selected);
            
            auto layout = Renderer(menu, [&] {
                return vbox({
                    text("=== THE SHOP ===") | bold | center | color(Color::Green),
                    separator(),
                    hbox({ text("Your Gold: ") | bold, text("$" + std::to_string(session.gold)) | color(Color::Yellow) | bold }) | center,
                    separator(),
                    text("Items For Sale:") | bold,
                    menu->Render() | border,
                    separator(),
                    text("Select item and press Enter to buy. Select 'Leave Shop' to advance.") | dim | center
                }) | border | center;
            });
            
            bool exit_shop = false;
            auto catch_event = CatchEvent(layout, [&](Event event) {
                if (event == Event::Return) {
                    const auto& current_inv = shop.GetInventory();
                    if (selected == (int)current_inv.size()) {
                        exit_shop = true;
                        screen.ExitLoopClosure()();
                        return true;
                    } else {
                        if (shop.BuyItem(selected, session)) {
                            JokerManager& jokerManager = JokerManager::GetInstance();
                            jokerManager.ClearObservers();
                            for (auto& joker : session.jokers) {
                                jokerManager.RegisterObserver(joker.get());
                            }
                            game_logs.push_back("Purchased item.");
                        } else {
                            game_logs.push_back("Could not buy item.");
                        }
                        screen.ExitLoopClosure()();
                        return true;
                    }
                }
                return false;
            });
            
            screen.Loop(catch_event);
            
            if (exit_shop) {
                break;
            }
        }
    };
    
    run_shop();
    
    game_logs.push_back("Leaving shop.");
    auto advance_logs = anteManager.Advance();
    game_logs.insert(game_logs.end(), advance_logs.begin(), advance_logs.end());
    state = TUIState::SelectBlind;
}

void HandleGameOverTUI(TUIState& state) {
    auto screen = ScreenInteractive::TerminalOutput();
    auto btn = Button("Return to Main Menu", screen.ExitLoopClosure());
    
    auto layout = Renderer(btn, [&] {
        return vbox({
            text("=== GAME OVER ===") | bold | center | color(Color::Red),
            separator(),
            text("You lost all your hands or gave up.") | center,
            separator(),
            btn->Render() | center
        }) | border | center;
    });
    
    screen.Loop(layout);
    state = TUIState::MainMenu;
}

int main() {
    std::srand(std::time(nullptr));
    TUIState state = TUIState::MainMenu;
    
    std::unique_ptr<RuntimeSession> session;
    std::unique_ptr<AnteManager> anteManager;
    std::vector<std::string> game_logs;

    bool running = true;
    while (running) {
        switch (state) {
            case TUIState::MainMenu: {
                RunMainMenu(state);
                break;
            }
            case TUIState::StartRun: {
                session = std::make_unique<RuntimeSession>();
                anteManager = std::make_unique<AnteManager>(*session);
                
                session->addJoker(std::make_shared<ChipsBoostJoker>(20));
                session->addJoker(std::make_shared<MultiplierJoker>(2));
                session->addJoker(std::make_shared<ConditionalJoker>(PokerHandType::Flush, 15));

                JokerManager& jokerManager = JokerManager::GetInstance();
                jokerManager.ClearObservers();
                for (auto& joker : session->jokers) {
                    jokerManager.RegisterObserver(joker.get());
                }

                game_logs.clear();
                game_logs.push_back("Started new run.");
                state = TUIState::SelectBlind;
                break;
            }
            case TUIState::SelectBlind: {
                HandleSelectBlindTUI(*session, *anteManager, state, game_logs);
                break;
            }
            case TUIState::PlayHand: {
                HandlePlayRoundTUI(*session, *anteManager, state, game_logs);
                break;
            }
            case TUIState::ResolveScore: {
                state = TUIState::Reward;
                break;
            }
            case TUIState::Reward: {
                HandleRewardTUI(*session, *anteManager, state, game_logs);
                break;
            }
            case TUIState::Shop: {
                HandleShopTUI(*session, *anteManager, state, game_logs);
                break;
            }
            case TUIState::GameOver: {
                HandleGameOverTUI(state);
                session.reset();
                anteManager.reset();
                break;
            }
            default:
                running = false;
                break;
        }
    }
    return 0;
}
