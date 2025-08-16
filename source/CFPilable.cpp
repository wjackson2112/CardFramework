//
// Created by Will on 1/16/2023.
//

#include "CFPilable.h"

#include <iostream>

#include "AnimationComponent.h"

CFPilable::CFPilable()
{
    pileRoot = this;
    pileParent = nullptr;
    pilePrevParent = nullptr;
    pileChild = nullptr;
    pileEnd = this;
}

void CFPilable::update(float deltaTime)
{
    Entity::update(deltaTime);

    if(!getPileChild())
        return;

    glm::vec3 pileOffset = getPileOffset();
    // auto* animComp = getPileChild()->getComponent<AnimationComponent>();
    auto* animComp = getComponent<AnimationComponent>();
    if((!animComp || !animComp->hasUnfinishedAnimations()) &&
       pileOffset.x == 0.f && pileOffset.y == 0.f && pileOffset.z > 0.f)
    {
       shouldUpdate = false;
    }
}

glm::vec3 CFPilable::getRootOffset()
{
    return getPileRoot()->rootOffset;
}

glm::vec3 CFPilable::getPileOffset()
{
    return getPileRoot()->pileOffset;
}

glm::vec3 CFPilable::getInitialRootOffset()
{
    return getPileRoot()->initialRootOffset;
}

glm::vec3 CFPilable::getInitialPileOffset()
{
    return getPileRoot()->initialPileOffset;
}

void CFPilable::setRootOffset(glm::vec3 newOffset)
{
    rootOffset = newOffset;

    if(getPileChild())
        getPileChild()->moveTo(rootOffset, nullptr, "", nullptr);
}

CFPilable* CFPilable::getPileRoot()
{
//    if(!pileRoot)
//    {
//        if (!pileParent)
//            pileRoot = this;
//        else
//            pileRoot = pileParent->getPileRoot();
//    }

    return pileRoot;
}

CFPilable* CFPilable::getPileEnd()
{
//    if(!pileEnd) {
//        if (!pileChild)
//            pileEnd = this;
//        else
//            pileEnd = pileChild->getPileEnd();
//    }

    return pileEnd;
}

void CFPilable::updatePileRoot(CFPilable* pilable)
{
    if(this != pileEnd)
        pileEnd->updatePileRoot(pilable);
    else
        this->updatePileRootRecurse(pilable);
}

void CFPilable::updatePileRootRecurse(CFPilable *pilable)
{
    pileRoot = pilable;
    if(pileParent)
        pileParent->updatePileRootRecurse(pilable);
}

void CFPilable::updatePileEnd(CFPilable* pilable)
{
    if(this != pileRoot)
        pileRoot->updatePileEnd(pilable);
    else
        this->updatePileEndRecurse(pilable);
}

void CFPilable::updatePileEndRecurse(CFPilable *pilable)
{
    pileEnd = pilable;
    if(pileChild)
        pileChild->updatePileEndRecurse(pilable);
}

void CFPilable::raiseToFront()
{
    glm::vec3 origPosition = transform.getPosition();
    transform.setPosition(glm::vec3(origPosition.x, origPosition.y, STACK_MAX));
}

void CFPilable::lowerToBack()
{
    glm::vec3 origPosition = transform.getPosition();
    transform.setPosition(glm::vec3(origPosition.x, origPosition.y, STACK_OFFSET));
}

void CFPilable::addToPile(CFPilable *pilable, bool snap/*= false*/, IAnimationCompleteReceiver* completeReceiver/*= nullptr*/) //bool notify/*= true*/)
{
    pilable->removeFromPile();

    if(this != getPileEnd())
    {
        pileEnd->addToPile(pilable, snap, completeReceiver);
        return;
    }

    glm::vec3 currWorldPosition = pilable->getWorldTransform().getPosition();

    pileChild = pilable;
    updatePileEnd(pileChild->getPileEnd());

    if(pilable->pileParent)
        pilable->pilePrevParent = pilable->pileParent;

    pilable->pileParent = this;
    updatePileRoot(pileRoot);

    addChild(pilable);

    // Attach to the pile without moving the card
    glm::vec3 attachedWorldPosition = pilable->getWorldTransform().getPosition();
    glm::vec3 worldPositionDifference = attachedWorldPosition - currWorldPosition;
    pilable->getTransform()->translate(-worldPositionDifference);

    AnimCompleteFunction complete = &IAnimationCompleteReceiver::animationCompleteWithId;

    // // Move the card to the target location
    // if(pilable->getPileParent() == pileRoot)
    // {
    //     if(snap)
    //     {
    //         pilable->snapTo(getRootOffset());
    //         if(completeReceiver)
    //             completeReceiver->animationCompleteWithId("addToPile", this);
    //     }
    //     else
    //     {
    //         pilable->moveTo(getRootOffset(), completeReceiver, "addToPile", complete);
    //     }
    // }
    // else
    // {
    //     if(snap)
    //     {
    //         pilable->snapTo(getPileOffset());
    //         if(completeReceiver)
    //             completeReceiver->animationCompleteWithId("addToPile", this);
    //     }
    //     else
    //     {
    //         pilable->moveTo(getPileOffset(), completeReceiver, "addToPile", complete);
    //     }
    // }

    // Move the card to the target location
    if(pilable->getPileParent() == pileRoot)
    {
        if(snap)
            pilable->snapTo(getRootOffset());
        else
            pilable->moveTo(getRootOffset(), completeReceiver, "addToPile", complete);
    }
    else
    {
        if(snap)
            pilable->snapTo(getPileOffset());
        else
            pilable->moveTo(getPileOffset(), completeReceiver, "addToPile", complete);
    }

    CFPilable* currPilable = pilable->getPileChild();
    while(currPilable != nullptr)
    {
        // Move the card to the target location
        if(currPilable->getPileParent() == pileRoot)
            currPilable->moveTo(getRootOffset(), completeReceiver, "addToPile", complete);
        else
            currPilable->moveTo(getPileOffset(), completeReceiver, "addToPile", complete);

        currPilable = currPilable->getPileChild();
    }
}

void CFPilable::addToPileStart(CFPilable *pilable, bool snap/*= false*/, IAnimationCompleteReceiver* completeReceiver/*= nullptr*/)//bool notify/*= true*/)
{
    pilable->removeFromPile();

    if(this != getPileRoot())
    {
        pileRoot->addToPileStart(pilable, snap, completeReceiver);
        return;
    }

    glm::vec3 currWorldPosition = pilable->getWorldTransform().getPosition();

    CFPilable* oldChild = pileChild;
    glm::vec3 oldChildOldPosition = oldChild->getTransform()->getPosition();
    glm::vec3 oldChildOldWorldPosition = oldChild->getWorldTransform().getPosition();

    pilable->addToPile(oldChild, true, nullptr);
    addToPile(pilable, true, nullptr);

    // Attach to the pile without moving the card
    glm::vec3 attachedWorldPosition = pilable->getWorldTransform().getPosition();
    glm::vec3 worldPositionDifference = attachedWorldPosition - currWorldPosition;
    pilable->getTransform()->translate(-worldPositionDifference);

    glm::vec3 oldChildAttachedWorldPosition = oldChild->getWorldTransform().getPosition();
    oldChild->getTransform()->translate(-(oldChildAttachedWorldPosition - oldChildOldWorldPosition));

    AnimCompleteFunction complete = nullptr;

    // Move the card to the target location
    if(snap)
    {
        pilable->snapTo(getRootOffset());
        if(completeReceiver)
            completeReceiver->animationCompleteWithId("addToPileStart", this);
    }
    else
    {
        pilable->moveTo(getRootOffset(), completeReceiver, "addToPileStart", complete);
        oldChild->moveTo(oldChild->getPileParent()->getPileOffset(), nullptr, "", nullptr);
        oldChild->getPileChild()->moveTo(oldChild->getPileOffset(), nullptr, "", nullptr);
    }
}

bool CFPilable::isInPile(CFPilable *pilable)
{
    CFPilable* pileRoot = getPileRoot();

    CFPilable* currPilable = pileRoot;
    while(currPilable != pilable && currPilable != nullptr)
        currPilable = currPilable->pileChild;

    return currPilable == pilable;
}

void CFPilable::removeFromPile(bool notify)
{
    if(pileParent == nullptr)
        return;

    Transform worldTransform = this->getWorldTransform();

    pileParent->shouldDraw = true;
    pileParent->shouldUpdate = true;

    shouldDraw = true;
    shouldUpdate = true;

    pileParent->removeChild(this);
    pileParent->pileChild = nullptr;
    pileParent->pileEnd = pileParent;
    pileParent->updatePileEnd(pileParent);
    if(pileParent)
        pilePrevParent = pileParent;
    pileParent = nullptr;
    updatePileRoot(this);

    transform = worldTransform;
}

void CFPilable::moveTo(glm::vec3 target, IAnimationCompleteReceiver* receiver, const std::string &completeIdentifier, AnimCompleteFunction completeFunction)
{
    transform.setPosition(target);
    if(receiver && completeFunction)
        (receiver->*completeFunction)(completeIdentifier, this);
}

void CFPilable::snapTo(glm::vec3 target)
{
    transform.setPosition(target);
}

void CFPilable::select()
{
    selected = true;
}

void CFPilable::deselect()
{
    selected = false;
}

void swap(CFPilable& left, CFPilable& right) {
    std::cout << "swapping " << &left << " and " << &right << std::endl;
    CFPilable* origLeftParent = left.getPileParent();
    CFPilable* origRightParent = right.getPileParent();
    CFPilable* origLeftChild = left.getPileChild();
    CFPilable* origRightChild = right.getPileChild();

    left.pileParent = origRightParent;
    right.pileParent = origLeftParent;
    left.pileChild = origRightChild;
    right.pileChild = origLeftChild;
}
