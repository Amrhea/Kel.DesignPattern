#pragma once

#include <vector>
#include <algorithm>

class ChosenHand {
private:
    std::vector<int> selectedIndices;

public:
    void Select(int index) {
        if (std::find(selectedIndices.begin(), selectedIndices.end(), index) == selectedIndices.end()) {
            selectedIndices.push_back(index);
        }
    }

    void Unselect(int index) {
        selectedIndices.erase(std::remove(selectedIndices.begin(), selectedIndices.end(), index), selectedIndices.end());
    }

    void Clear() {
        selectedIndices.clear();
    }

    const std::vector<int>& GetIndices() const {
        return selectedIndices;
    }

    int Count() const {
        return static_cast<int>(selectedIndices.size());
    }
};
