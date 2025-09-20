//
// Created by Will on 5/27/2024.
//

#include "CFDeck.h"

#include <random>
#include <iostream>
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
            DealQueueElement front = dealQueue.front();
            front.pilable->addToPile(getPileEnd(), false, front.receiver, front.callback);
            dealQueue.pop();
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

    return deckSize;
}

bool CFDeck::isDealing() {
    return !dealQueue.empty();
}

void CFDeck::deal(CFPile* destination, IAnimationCompleteReceiver* receiver/* = nullptr*/, AnimCompleteFunction callback/* = nullptr*/) {
    dealQueue.push(DealQueueElement(destination, receiver, callback));
    shouldUpdate = true;
}


