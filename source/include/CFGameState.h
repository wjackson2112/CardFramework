//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/8/25.
//

#ifndef CF_GAME_STATE_H
#define CF_GAME_STATE_H

#define NO_GRAB glm::vec2(-1, -1)
#define DRAG_THRESHOLD 50

#include <random>
#include <string>

#include "IAnimationCompleteReceiver.h"
#include "CFCard.h"
#include "CFGameMode.h"

class SPSelectAction;
class SPCancelAction;
class SPDealAction;
class SPUndoAction;

class CFGameState {
public:
    CFGameMode* gameMode;
    std::vector<CFCard*> cards;

    void cardAnimationComplete(CFCard* card, std::string identifier)
    {
        gameMode->cardAnimationComplete(card, identifier);
    }
};



#endif //CF_GAME_STATE_H
