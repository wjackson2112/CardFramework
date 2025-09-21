//
// Created by will on 9/9/25.
//

#include "CFFlipAnimation.h"

#include <iostream>
#include <bits/ostream.tcc>

#include "TranslationAnimation.h"
#include "ScaleAnimation.h"
#include "SpriteComponent2D.h"
#include "CFCard.h"

CFFlipAnimation::CFFlipAnimation(float lengthSeconds, Entity* animatedEntity, IAnimationCompleteReceiver* receiver/* = nullptr*/,
                                 AnimCompleteFunction completeFunction/* = &IAnimationCompleteReceiver::animationCompleteWithId*/,
                                 const std::string& completeIdentifier/* = ""*/)
: CompoundAnimation(lengthSeconds, animatedEntity, receiver, completeFunction, completeIdentifier)
{
    glm::vec2 spriteSize = animatedEntity->getComponent<SpriteComponent2D>()->getSize();

    glm::vec3 translation = glm::vec3(spriteSize.x/2, 0.0f, 0.0f);
    glm::vec3 midScale = glm::vec3(0.0, 1.0, 1.0);
    glm::vec3 endScale = glm::vec3(1.0, 1.0, 1.0);

    addAnimation<TranslationAnimation>(0.f, animatedEntity, translation, lengthSeconds / 2);
    addAnimation<ScaleAnimation>(0.f, animatedEntity, midScale, lengthSeconds / 2, this, static_cast<AnimCompleteFunction>(&CFFlipAnimation::flipTexture));

    addAnimation<TranslationAnimation>(lengthSeconds / 2, animatedEntity, -translation, lengthSeconds / 2);
    addAnimation<ScaleAnimation>(lengthSeconds / 2, animatedEntity, endScale, lengthSeconds / 2);
}

void CFFlipAnimation::flipTexture(std::string identifier, Entity *animatedEntity)
{
    std::cout << "FLIP" << std::endl;
    dynamic_cast<CFCard*>(animatedEntity)->flipTexture();
}

