//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 5/27/2024.
//

#ifndef CF_DECK_H
#define CF_DECK_H

#include "CFPile.h"
#include "Animation.h"
#include "IAnimationCompleteReceiver.h"

#include <queue>

class CFDeck : public CFPile {
    std::queue<CFPilable*> dealQueue;
    float secondsSinceLastDeal = -1.0;
    float secondsBetweenDeals;

public:
    CFDeck(glm::vec2 position, float secondsBetweenDeals)
    : CFPile(position, glm::vec3(0.f, 0.f, STACK_OFFSET), glm::vec3(0.f, 0.f, STACK_OFFSET))
    {
        this->secondsBetweenDeals = secondsBetweenDeals;
    }

    // Entity Interface
    void update(float deltaTime) override;

    int getDeckSize();

    bool isDealing();

    void shuffle() override;
    void deal(CFPile* destination);//, IAnimationCompleteReceiver* receiver = nullptr, AnimCompleteFunction callback = nullptr);
};

#endif //CF_DECK_H
