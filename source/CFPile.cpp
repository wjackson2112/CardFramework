//
// Created by Will on 1/16/2023.
//

#include "CFPile.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <list>

#include "AssetManager.h"
#include "SpriteComponent2D.h"
#include "CollisionComponent2DAABB.h"

CFPile::CFPile(glm::vec2 position, glm::vec3 inRootOffset, glm::vec3 inPileOffset)
: size(49.f, 64.f)
{
    rootOffset = inRootOffset;
    pileOffset = inPileOffset;

    initialRootOffset = inRootOffset;
    initialPileOffset = inPileOffset;

    this->transform.setPosition2(position);

    Shader shader = AssetManager::getInstance()->loadShader("shaders/sprite.vert",
                                                            "shaders/sprite.frag",
                                                            nullptr,
                                                            "sprite");
    // Deck asset is from here: https://www.codeproject.com/Articles/1187548/Video-Poker
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets/pile_border.png", true, "pile_border");

    auto* spriteComponent = new SpriteComponent2D(shader, texture, size);
    addComponent(spriteComponent);

    shouldUpdate = true;
}

CFPile::~CFPile()
{
    removeFromPile();
    if(pileChild)
        pileChild->removeFromPile();
}

void CFPile::setSize(glm::vec2 size)
{
    this->size = size;
    getComponent<SpriteComponent2D>()->setSize(size);
}

void CFPile::scaleToWidth(float width)
{
    auto* spriteComp = getComponent<SpriteComponent2D>();
    Transform spriteTrans = spriteComp->getWorldTransform();
    float scaleFactor = width / spriteTrans.getScale().x;
    this->transform.scaleBy(glm::vec3(scaleFactor, scaleFactor, 1.f));
}

void CFPile::scaleToHeight(float height)
{
    auto* spriteComp = getComponent<SpriteComponent2D>();
    Transform spriteTrans = spriteComp->getWorldTransform();
    float scaleFactor = height / spriteTrans.getScale().y;
    this->transform.scaleBy(glm::vec3(scaleFactor, scaleFactor, 1.f));
}

void CFPile::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<CFPilable*> pileList;

    for(CFPilable* currPilable = getPileRoot()->getPileChild(); currPilable != nullptr; currPilable = currPilable->getPileChild())
        pileList.push_back(currPilable);

    std::shuffle(pileList.begin(), pileList.end(), g);

    getPileChild()->removeFromPile();
    addToPile(pileList[0], false);

    for(int i = 0; i < pileList.size() - 1; i++)
    {
        if(pileList[i]->getPileChild())
            pileList[i]->getPileChild()->removeFromPile();
        pileList[i]->addToPile(pileList[i+1], false);
    }
}

//TODO: Buffer this value so it doesn't have to be reevaluated every call
int CFPile::getPileLength()
{
    int length = 0;
    for(CFPilable* currPilable = this; currPilable != nullptr; currPilable = currPilable->getPileChild())
        length++;
    return length;
}
