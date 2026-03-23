//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 1/16/2023.
//

#ifndef CF_PILE_H
#define CF_PILE_H

#include "CFCard.h"
#include "CFPilable.h"
//#include "SPSnapValidator.h"
#include "Entity.h"
#include "ICollisionReceiver.h"

class CFPile;

class ICFPileChangeReceiver
{
    int myVal = 1;
public:
    virtual void cardAdded(CFPile* pile, Entity* pilable) = 0;
    virtual void cardAddedToStart(CFPile* pile, CFPilable* pilable) = 0;
    virtual void cardRemoved(CFPile* pile, CFPilable* pilable) = 0;
};

class CFPile : public CFPilable//, public ICollisionReceiver
{
protected:
    glm::vec2 size;

public:
    //SPPilable
    glm::vec2 getSize() { return size; }
    void setSize(glm::vec2 size);
    void scaleToWidth(float width);
    void scaleToHeight(float height);

    int getPileLength();

    CFPile(glm::vec2 position, glm::vec3 rootOffset, glm::vec3 pileOffset);
    ~CFPile();

    virtual void shuffle();
};

#endif //CF_PILE_H
