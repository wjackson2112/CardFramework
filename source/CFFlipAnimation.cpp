//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 9/9/25.
//

#include "CFFlipAnimation.h"

#include <iostream>

#include "TranslationAnimation.h"
#include "ScaleAnimation.h"
#include "SpriteComponent2D.h"
#include "CFCard.h"

CFFlipAnimation::CFFlipAnimation(float lengthSeconds, CFCard* animatedCard, IAnimationCompleteReceiver* receiver/* = nullptr*/,
                                 AnimCompleteFunction completeFunction/* = &IAnimationCompleteReceiver::animationCompleteWithId*/,
                                 const std::string& completeIdentifier/* = ""*/)
: CompoundAnimation(lengthSeconds, receiver, completeFunction, completeIdentifier)
{
    SpriteComponent2D* spriteComponent = animatedCard->getComponent<SpriteComponent2D>();
    glm::vec2 spriteSize = spriteComponent->getSize();

    glm::vec3 translation = glm::vec3(spriteSize.x/2, 0.0f, 0.0f);
    glm::vec3 midScale = glm::vec3(0.0, 1.0, 1.0);
    glm::vec3 endScale = glm::vec3(1.0, 1.0, 1.0);

    addAnimation<TranslationAnimation>(0.f, spriteComponent->getTransform(), translation, lengthSeconds / 2);
    addAnimation<ScaleAnimation>(0.f, spriteComponent->getTransform(), midScale, lengthSeconds / 2, animatedCard, static_cast<AnimCompleteFunction>(&CFCard::flipTexture));

    addAnimation<TranslationAnimation>(lengthSeconds / 2, spriteComponent->getTransform(), -translation, lengthSeconds / 2);
    addAnimation<ScaleAnimation>(lengthSeconds / 2, spriteComponent->getTransform(), endScale, lengthSeconds / 2);
}

