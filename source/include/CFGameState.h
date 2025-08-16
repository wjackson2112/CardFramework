//
// Created by will on 8/8/25.
//

#ifndef CF_GAME_STATE_H
#define CF_GAME_STATE_H

#include "IAnimationCompleteReceiver.h"
#include "CFPilable.h"

struct CFGameState : public IAnimationCompleteReceiver {
    virtual void animationCompleteWithId(std::string identifier, Entity *entity) {};
};



#endif //CF_GAME_STATE_H
