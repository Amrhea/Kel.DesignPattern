#pragma once

#include "hand_selection/ChosenHand.h"

class SelectionCommand {
public:
    virtual ~SelectionCommand() = default;
    virtual void Execute(ChosenHand& selection) = 0;
    virtual void Undo(ChosenHand& selection) = 0;
};

class SelectCardCommand : public SelectionCommand {
private:
    int index;
public:
    SelectCardCommand(int idx) : index(idx) {}
    void Execute(ChosenHand& selection) override {
        selection.Select(index);
    }
    void Undo(ChosenHand& selection) override {
        selection.Unselect(index);
    }
};

class UnselectCardCommand : public SelectionCommand {
private:
    int index;
public:
    UnselectCardCommand(int idx) : index(idx) {}
    void Execute(ChosenHand& selection) override {
        selection.Unselect(index);
    }
    void Undo(ChosenHand& selection) override {
        selection.Select(index);
    }
};
