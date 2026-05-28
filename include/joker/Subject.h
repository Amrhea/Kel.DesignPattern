#pragma once
#include "joker/Observer.h"
#include <vector>
#include <algorithm>

class Subject {
protected:
    std::vector<Observer*> observers;

public:
    virtual ~Subject() = default;

    virtual void RegisterObserver(Observer* observer) {
        observers.push_back(observer);
    }

    virtual void RemoveObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    virtual void NotifyObservers(const std::string& handName, int& score) {
        for (auto* observer : observers) {
            if (observer) {
                observer->OnHandPlayed(handName, score);
            }
        }
    }
};
