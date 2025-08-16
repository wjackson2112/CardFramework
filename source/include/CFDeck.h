//
// Created by Will on 5/27/2024.
//

#ifndef CF_DECK_H
#define CF_DECK_H

#include "CFPile.h"

class CFDeck : public CFPile {

public:
    CFDeck(glm::vec2 position)
    : CFPile(position, glm::vec3(0.f, 0.f, 0.001f), glm::vec3(0.f, 0.f, 0.001f))
    {}

    // Entity Interface
    void update(float deltaTime) override;

    int getDeckSize();
};

#endif //CF_DECK_H
