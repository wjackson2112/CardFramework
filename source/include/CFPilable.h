//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 1/16/2023.
//

#ifndef CF_PILABLE_H
#define CF_PILABLE_H

#include "glm/glm.hpp"
#include "Entity.h"
#include "Animation.h"

#define STACK_MIN 0.0f
#define STACK_MAX 0.1f
#define STACK_OFFSET 0.0001f

class CFDeck;

class CFPilable : public Entity, public IAnimationCompleteReceiver
{
protected:
    CFPilable* pileRoot = nullptr;
    CFPilable* pilePrevParent = nullptr;
    CFPilable* pileParent = nullptr;
    CFPilable* pileChild = nullptr;
    CFPilable* pileEnd = nullptr;

    bool selected = false;

    glm::vec3 rootOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 initialRootOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 pileOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 initialPileOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);

public:
    CFPilable();
    void update(float deltaTime) override;

    glm::vec3 getRootOffset();
    glm::vec3 getPileOffset();

    glm::vec3 getInitialRootOffset();
    glm::vec3 getInitialPileOffset();

    void setRootOffset(glm::vec3 newOffset);
    void setPileOffset(glm::vec3 newOffset) { pileOffset = newOffset; };

    void setInitialRootOffset(glm::vec3 newOffset) { initialRootOffset = newOffset; };
    void setInitialPileOffset(glm::vec3 newOffset) { initialPileOffset = newOffset; };

    CFPilable* getPileRoot();
    CFPilable* getPileEnd();

    void updatePileRoot(CFPilable* pilable);
    void updatePileEnd(CFPilable* pilable);

private:
    void updatePileRootRecurse(CFPilable* pilable);
    void updatePileEndRecurse(CFPilable* pilable);
public:

    CFPilable* getPileParent() { return pileParent; }
    CFPilable* getPilePrevParent() { return pilePrevParent; }
    CFPilable* getPileChild() { return pileChild; }

    virtual void raiseToFront();
    virtual void lowerToBack();
    virtual void addToPile(CFPilable *pilable, bool snap = false, std::string identifier = "addToPile"/*IAnimationCompleteReceiver* completeReceiver = nullptr, AnimCompleteFunction completeFunction = &IAnimationCompleteReceiver::animationComplete*/);
    virtual void addToPileStart(CFPilable* pilable, bool snap = false, IAnimationCompleteReceiver* completeReceiver = nullptr);
    virtual bool isInPile(CFPilable* pilable);
    virtual void removeFromPile(bool notify = false);
    virtual ~CFPilable() = default;
    virtual void moveTo(glm::vec3 target, IAnimationCompleteReceiver* receiver, const std::string &completeIdentifier, AnimCompleteFunction completeFunction);
    virtual void snapTo(glm::vec3 target);

    virtual glm::vec2 getSize() { return glm::vec2(0,0); };
    virtual void setSize(glm::vec2 size) {};

    virtual void select();
    virtual void deselect();
    virtual bool isSelected() { return selected; }

    void animationComplete(std::string identifier) override {};

    // Swap supported to allow CFPile to implement the shuffle algorithm
    friend void swap(CFPilable& left, CFPilable& right);
};


#endif //CF_PILABLE_H
