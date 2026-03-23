//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/20/25.
//

#ifndef CF_ACTION_H
#define CF_ACTION_H

#include "Action.h"
#include "CFCard.h"

template<typename T>
class CFAction : public Action<T>
{
public:
    CFAction(T* gameMode) : Action<T>(gameMode) {}
    ~CFAction() {}
protected:
    CFCard* getTopmostCardAtPosition(glm::vec2 position){
        for(CFCard* card : this->gameMode->gameState->cards)
        {
            if(!card)
                continue;

            if(card->isTopmostAtPoint(position))
                return card;
        }

        return nullptr;
    }
};

#endif //CF_ACTION_H
