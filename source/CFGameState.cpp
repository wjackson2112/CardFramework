//
// Created by will on 8/8/25.
//

#include "include/CFGameState.h"

void CFGameState::shuffleCards() {
    for (int i = 0; i < 7; i++) {
        std::shuffle(std::begin(cards), std::end(cards), rng);
    }
}