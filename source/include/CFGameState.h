//
// Created by will on 8/8/25.
//

#ifndef CF_GAME_STATE_H
#define CF_GAME_STATE_H

#define NO_GRAB glm::vec2(-1, -1)
#define DRAG_THRESHOLD 50

#include <random>

#include "IAnimationCompleteReceiver.h"
#include "CFCard.h"

class SPSelectAction;
class SPCancelAction;
class SPDealAction;
class SPUndoAction;

struct CFGameState : public IAnimationCompleteReceiver {
private:
    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};
public:

    std::vector<CFCard*> cards;

    void shuffleCards();
};



#endif //CF_GAME_STATE_H
