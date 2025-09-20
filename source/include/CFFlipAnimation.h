//
// Created by will on 9/9/25.
//

#ifndef CF_FLIP_ANIMATION_H
#define CF_FLIP_ANIMATION_H

#include "CompoundAnimation.h"

class CFFlipAnimation : public CompoundAnimation, public IAnimationCompleteReceiver
{
public:
    CFFlipAnimation(float lengthSeconds, Entity* animatedEntity, IAnimationCompleteReceiver* receiver = nullptr,
                    AnimCompleteFunction completeFunction = &IAnimationCompleteReceiver::animationCompleteWithId,
                    const std::string& completeIdentifier = "");

private:
    void flipTexture(std::string identifier, Entity* animatedEntity);
    void animationCompleteWithId(std::string identifier, Entity *animatedEntity) override {};

};

#endif //CF_FLIP_ANIMATION_H
