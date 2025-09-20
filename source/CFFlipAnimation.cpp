//
// Created by will on 9/9/25.
//

#include "CFFlipAnimation.h"

#include <iostream>
#include <bits/ostream.tcc>

#include "TransformAnimation.h"
#include "SpriteComponent2D.h"
#include "CFCard.h"

CFFlipAnimation::CFFlipAnimation(float lengthSeconds, Entity* animatedEntity, IAnimationCompleteReceiver* receiver/* = nullptr*/,
                                 AnimCompleteFunction completeFunction/* = &IAnimationCompleteReceiver::animationCompleteWithId*/,
                                 const std::string& completeIdentifier/* = ""*/)
: CompoundAnimation(lengthSeconds, animatedEntity, receiver, completeFunction, completeIdentifier)
{
    glm::vec2 spriteSize = animatedEntity->getComponent<SpriteComponent2D>()->getSize();

    Transform midTransform = Transform();
    midTransform.translate(glm::vec3(spriteSize.x/2, 0.0f, 0.0f));
    midTransform.scaleTo(glm::vec3(0.0, 1.0, 1.0));

    Transform endTransform = Transform();
    endTransform.translate(glm::vec3(-spriteSize.x/2, 0.0f, 0.0f));

    // Slim to zero
    addAnimation<TransformAnimation>(0.f, animatedEntity, midTransform, lengthSeconds / 2, this, static_cast<AnimCompleteFunction>(&CFFlipAnimation::flipTexture));

    // Widen back up
    addAnimation<TransformAnimation>(lengthSeconds / 2, animatedEntity, endTransform, lengthSeconds / 2);
}

void CFFlipAnimation::flipTexture(std::string identifier, Entity *animatedEntity)
{
    std::cout << "FLIP" << std::endl;
    dynamic_cast<CFCard*>(animatedEntity)->flipTexture();
}

