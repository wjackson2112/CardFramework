//
// Created by Will on 5/27/2024.
//

#include "CFDeck.h"

#include <random>
#include <iostream>
#include <algorithm>
#include "AnimationComponent.h"

void CFDeck::update(float deltaTime)
{
    Entity::update(deltaTime);
    // for(auto curr = reinterpret_cast<CFCard*>(getPileChild()); curr != nullptr; curr = reinterpret_cast<CFCard*>(curr->getPileChild()))
    // {
    //     if(curr == getPileEnd())
    //     {
    //         curr->shouldUpdate = true;
    //         curr->shouldDraw = true;
    //         continue;
    //     }
    //
    //     if(curr->getComponent<AnimationComponent>()->hasUnfinishedAnimations()) {
    //         continue;
    //     }
    //
    //     curr->shouldUpdate = false;
    //     curr->shouldDraw = false;
    // }

    if(!dealQueue.empty())
    {
        // Deal the first card immediately
        if(secondsSinceLastDeal == -1.0)
            secondsSinceLastDeal = secondsBetweenDeals;
        else
            secondsSinceLastDeal += deltaTime;

        if(secondsSinceLastDeal >= secondsBetweenDeals)
        {
            getPileEnd()->shouldUpdate = true;
            getPileEnd()->shouldDraw = true;
            getPileEnd()->raiseToFront();
            CFPilable* front = dealQueue.front();
            dealQueue.pop();

            std::string identifier = "deal";
            if(dealQueue.empty())
                identifier = "dealComplete";
            front->addToPile(getPileEnd(), false, identifier);

            secondsSinceLastDeal = 0;
            getPileEnd()->shouldUpdate = true;
            getPileEnd()->shouldDraw = true;
        }
    }

    if(dealQueue.empty())
        shouldUpdate = false;
}

int CFDeck::getDeckSize()
{
    if(getPileChild() == nullptr)
        return 0;

    int deckSize = 0;
    for(CFPilable* curr = getPileChild(); curr != nullptr; curr = curr->getPileChild())
        deckSize++;

    deckSize -= dealQueue.size();

    return deckSize;
}

bool CFDeck::isDealing() {
    return !dealQueue.empty();
}

void CFDeck::shuffle()
{
    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};
    std::vector<CFPilable*> cards;

    for(CFPilable* currPilable = getPileChild(); currPilable != nullptr; currPilable = currPilable->getPileChild())
    {
        currPilable->removeFromPile();
        cards.push_back(currPilable);
    }

    std::shuffle(std::begin(cards), std::end(cards), rng);

    for(CFPilable* card : cards)
    {
        addToPile(card);
    }
}

void CFDeck::deal(CFPile* destination){//, IAnimationCompleteReceiver* receiver/* = nullptr*/, AnimCompleteFunction callback/* = nullptr*/) {
    dealQueue.push(destination);
    shouldUpdate = true;
}


