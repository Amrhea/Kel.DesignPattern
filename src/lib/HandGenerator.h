#pragma once

#include "Hand.h"

class HandGenerator
{
    public:
        HandGenerator();
        ~HandGenerator();

        Hand generateHand();
};