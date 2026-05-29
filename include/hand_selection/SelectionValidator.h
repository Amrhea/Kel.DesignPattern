#pragma once

#include "hand_selection/ChosenHand.h"
#include "hand_selection/Hand.h"

class SelectionValidator {
public:
    static bool IsValid(const Hand& hand, const ChosenHand& selection) {
        if (selection.Count() > 5) return false;
        if (selection.Count() == 0) return false;

        const auto& indices = selection.GetIndices();
        for (int index : indices) {
            if (index < 0 || index >= hand.GetCardCount()) {
                return false;
            }
        }
        return true;
    }
};
