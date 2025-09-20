//
// Created by Will on 1/9/2023.
//

#include "CFCard.h"

#include <iostream>

#include "AssetManager.h"
//#include "EntityManager.h"
//#include "TextComponent.h"
#include "SpriteSheetComponent2D.h"
#include "CollisionComponent2DAABB.h"
#include "AnimationComponent.h"
#include "TransformAnimation.h"
#include "OptionsManager.h"
// #include "SPSnapValidatorFourSuits.h"
#include "CFFlipAnimation.h"
#include "CFGameState.h"
#include "CompoundAnimation.h"
#include "Scene.h"

#include "EventManager.h"

#define CARD_BACK_X_INDEX 13

CFCard::CFCard(glm::vec2 position, CFCardSuit suit, CFCardRank rank, bool faceUp, CFGameState* gameState)
: suit(suit)
, rank(rank)
, color(CF_BACK_BLUE)
, faceUp(faceUp)
, gameState(gameState)
{
    this->transform.setPosition2(position);

    Shader shader = AssetManager::getInstance()->loadShader("shaders/sprite.vert",
                                                            "shaders/sprite.frag",
                                                            nullptr,
                                                            "sprite");
    // Original deck asset is from here: https://www.codeproject.com/Articles/1187548/Video-Poker
    std::string assetName = cardValueStrings[rank] + "_of_" + cardSuitStrings[suit] + ".png";
//    std::cout << assetName << std::endl;
    Texture2D texture = AssetManager::getInstance()->loadTexture(("assets/cards/" + assetName).c_str(), true, assetName);
    Texture2D textureBack = AssetManager::getInstance()->loadTexture("assets/card_back.png", true, "card_back.png");

    size = glm::vec2(texture.width, texture.height);

    SpriteComponent2D* spriteComponent = new SpriteComponent2D(shader, texture, size);

    if(!faceUp)
        spriteComponent->setTexture(textureBack);

    addComponent(spriteComponent);

    auto* animationComponent = new AnimationComponent();
    addComponent(animationComponent);

//    std::string text = "0.0";
//    Shader textShader = AssetManager::getInstance()->loadShader("shaders\\text.vert",
//                                                                "shaders\\text.frag",
//                                                                nullptr,
//                                                                text);
//    TextFont textFont = AssetManager::getInstance()->loadTextFont("assets\\arial.ttf",
//                                                                  "arial12", 14);
//    auto* textComponent = new TextComponent(textShader, textFont, text);
//    textComponent->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
//    textComponent->setTransform(glm::vec3(0, -10.f, 0.001f));
//    addComponent(textComponent);

    shouldUpdate = faceUp;
}

CFCard::~CFCard()
{
    removeFromPile();
    if(pileChild)
        pileChild->removeFromPile();
}

CFPilable* CFCard::getClosestOverlap()
{
    CFPilable *bestPilable = nullptr;
    float bestArea = 0.0f;
    float bestZ = -MAXFLOAT;

//    EntityManager* entityManager = EntityManager::getInstance();
//    std::vector<Entity*> entities = entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this));
    std::vector<Entity*> entities = owningScene->getEntities();
    for(Entity* entity : entities)
    {
        if(CFPilable* newPilable = dynamic_cast<CFPilable*>(entity))
        {
            // We're already piled with this card
            if(isInPile(newPilable))
                continue;

            CFPilable* newParent = newPilable->getPileEnd();

            glm::vec2 thisPosition = getWorldTransform().getPosition();
            glm::vec2 thisSize = this->size;
            glm::vec2 otherPosition = newPilable->getWorldTransform().getPosition();
            glm::vec2 otherSize = newPilable->getSize();

            bool xCollided = thisPosition.x + thisSize.x > otherPosition.x &&
                             otherPosition.x + otherSize.x > thisPosition.x;

            bool yCollided = thisPosition.y + thisSize.y > otherPosition.y &&
                             otherPosition.y + otherSize.y > thisPosition.y;

            if(!xCollided || !yCollided)
                continue;

            glm::vec3 start = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 end = glm::vec3(0.0f, 0.0f, 0.0f);

            if (thisPosition.x < otherPosition.x)
                start.x = otherPosition.x;
            else
                start.x = thisPosition.x;

            if (thisPosition.x + thisSize.x < otherPosition.x + otherSize.x)
                end.x = thisPosition.x + thisSize.x;
            else
                end.x = otherPosition.x + otherSize.x;

            if (thisPosition.y < otherPosition.y)
                start.y = otherPosition.y;
            else
                start.y = thisPosition.y;

            if (thisPosition.y + thisSize.y < otherPosition.y + otherSize.y)
                end.y = thisPosition.y + thisSize.y;
            else
                end.y = otherPosition.y + otherSize.y;

            glm::vec3 overlap = (end - start);

            float newArea = overlap.x * overlap.y;
            float newZ = newPilable->getWorldTransform().getPosition().z;

            if (newArea > bestArea || (newArea == bestArea && newZ > bestZ)) {
                bestPilable = newPilable;
                bestArea = newArea;
                bestZ = newZ;
            }
        }
    }

    return bestPilable;
}

bool CFCard::containsPoint(glm::vec2 point)
{
    glm::vec2 cardPosition = getWorldTransform().getPosition2();
    return  point.x >= cardPosition.x && point.x <= cardPosition.x + size.x &&
            point.y >= cardPosition.y && point.y <= cardPosition.y + size.y;
}

bool CFCard::isTopmostAtPoint(glm::vec2 point)
{
    if(!containsPoint(point))
        return false;

//    EntityManager* entityManager = EntityManager::getInstance();
//    std::vector<Entity*> entities = entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this));
    std::vector<Entity*>entities = owningScene->getEntities();
    for(Entity* entity : entities)
    {
        if (CFCard *otherCard = dynamic_cast<CFCard *>(entity))
        {
            if (otherCard->containsPoint(point) &&
                otherCard->getWorldTransform().getPosition().z > getWorldTransform().getPosition().z)
            {
                return false;
            }
        }
    }

    return true;
}

void CFCard::flip(IAnimationCompleteReceiver* receiver/* = nullptr*/, AnimCompleteFunction completeFunction/* = ""*/, const std::string &completeIdentifier/* = &IAnimationCompleteReceiver::animationCompleteWithId*/)
{
    auto* animComp = getComponent<AnimationComponent>();

    animComp->addAndStart<CFFlipAnimation>(.15f, this);


    // Start receiving updates when card turned face up
    // NOTE: Face down disable is handled in the animation callback
    // if(isFaceUp())
    shouldUpdate = true;


};

void CFCard::flipTexture()
{
    faceUp = !faceUp;

    std::string assetName = cardValueStrings[rank] + "_of_" + cardSuitStrings[suit] + ".png";
    Texture2D texture = AssetManager::getInstance()->getTexture(assetName);
    Texture2D textureBack = AssetManager::getInstance()->getTexture("card_back.png");

    if(faceUp)
        getComponent<SpriteComponent2D>()->setTexture(texture);
    else
        getComponent<SpriteComponent2D>()->setTexture(textureBack);
}

bool CFCard::hasUnfinishedAnimations() {
    return getComponent<AnimationComponent>()->hasUnfinishedAnimations();
}

void CFCard::select()
{
    CFPilable::select();
//    getComponent<SpriteComponent2D>()->setColor3(glm::vec3(0.75f, 1.0f, .75f));
}

void CFCard::deselect()
{
    CFPilable::deselect();
//    getComponent<SpriteComponent2D>()->setColor3(glm::vec3(1.0f, 1.0f, 1.0f));
}

void CFCard::scaleToWidth(float width)
{
    auto* spriteComp = getComponent<SpriteComponent2D>();
    Transform spriteTrans = spriteComp->getWorldTransform();
    float scaleFactor = width / spriteTrans.getScale().x;
    this->transform.scaleBy(glm::vec3(scaleFactor, scaleFactor, 1.f));
}

void CFCard::scaleToHeight(float height)
{
    auto* spriteComp = getComponent<SpriteComponent2D>();
    Transform spriteTrans = spriteComp->getWorldTransform();
    float scaleFactor = height / spriteTrans.getScale().y;
    this->transform.scaleBy(glm::vec3(scaleFactor, scaleFactor, 1.f));
}


void CFCard::moveTo(glm::vec3 target, IAnimationCompleteReceiver* receiver, const std::string &completeIdentifier, AnimCompleteFunction completeFunction)
{
    glm::vec3 translation = target - transform.getPosition();

    auto* animComp = getComponent<AnimationComponent>();

    // Determine where the card is already going, so we can just add enough animation
    // to get to the new target without overshooting
    for(auto transformAnim : animComp->getAnimations<TransformAnimation>())
        if(!transformAnim.hasFinished())
            translation -= transformAnim.getRemainingTranslation();

    Transform targetTransform = Transform();
    targetTransform.translate(translation);

    if(translation.x != 0.f || translation.y != 0.f || translation.z != 0.f)
        animComp->addAndStart<TransformAnimation>(this, targetTransform, 0.3f, receiver, completeFunction, completeIdentifier);

    shouldUpdate = true;
}

void CFCard::setSize(glm::vec2 size)
{
    this->size = size;
    getComponent<SpriteComponent2D>()->setSize(size);
}

void CFCard::animationCompleteWithId(std::string identifier, Entity* entity)
{
    // Turn off updates for face down cards after they're all settled
    if(!getComponent<AnimationComponent>()->hasAnimations() && isFaceDown())
        shouldUpdate = false;

    // TODO: Bring this back after establishing the gamestate type
    gameState->animationCompleteWithId(identifier, this);
}