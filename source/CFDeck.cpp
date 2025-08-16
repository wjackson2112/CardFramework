//
// Created by Will on 5/27/2024.
//

#include "CFDeck.h"

#include <random>
#include <iostream>

void CFDeck::update(float deltaTime)
{
    Entity::update(deltaTime);
    //TODO: Run the card list and disable updates for cards that do not have animations and are not at the end
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


