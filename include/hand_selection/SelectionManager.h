#pragma once

#include "hand_selection/ChosenHand.h"
#include "hand_selection/SelectionCommand.h"
#include "hand_selection/SelectionValidator.h"
#include <memory>
#include <vector>

class SelectionManager {
private:
    ChosenHand currentSelection;
    std::vector<std::unique_ptr<SelectionCommand>> history;

public:
    void Select(int index) {
        auto cmd = std::make_unique<SelectCardCommand>(index);
        cmd->Execute(currentSelection);
        history.push_back(std::move(cmd));
    }

    void Unselect(int index) {
        auto cmd = std::make_unique<UnselectCardCommand>(index);
        cmd->Execute(currentSelection);
        history.push_back(std::move(cmd));
    }

    void Undo() {
        if (!history.empty()) {
            history.back()->Undo(currentSelection);
            history.pop_back();
        }
    }

    bool Confirm(const Hand& hand) {
        return SelectionValidator::IsValid(hand, currentSelection);
    }

    const ChosenHand& GetSelection() const {
        return currentSelection;
    }

    void Clear() {
        currentSelection.Clear();
        history.clear();
    }
};
