//
// Created by will on 8/20/25.
//

#ifndef CF_GAME_MODE_H
#define CF_GAME_MODE_H

#include "IAnimationCompleteReceiver.h"
#include "CFPilable.h"
#include "Scene.h"

class CFGameMode : public IAnimationCompleteReceiver
{
public:
    virtual void initialSetup(Scene* scene) = 0;
    virtual void updateLayout() = 0;
};

#endif //CF_GAME_MODE_H
