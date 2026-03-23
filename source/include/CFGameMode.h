//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/20/25.
//

#ifndef CF_GAME_MODE_H
#define CF_GAME_MODE_H

#include <string>

#include "IAnimationCompleteReceiver.h"
#include "CFPilable.h"
#include "CFCard.h"
#include "Scene.h"

class CFGameMode : public IAnimationCompleteReceiver
{
public:
    virtual void initialSetup(Scene* scene) = 0;
    virtual void updateLayout() = 0;

    virtual void dealComplete(std::string identifier) = 0;
    virtual void resetGameComplete() = 0;

    virtual void cardAnimationComplete(CFCard* card, std::string identifier) = 0;
};

#endif //CF_GAME_MODE_H
