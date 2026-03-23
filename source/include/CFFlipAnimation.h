//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 9/9/25.
//

#ifndef CF_FLIP_ANIMATION_H
#define CF_FLIP_ANIMATION_H

#include "CompoundAnimation.h"

class CFCard;

class CFFlipAnimation : public CompoundAnimation
{
public:
    CFFlipAnimation(float lengthSeconds, CFCard* animatedCard, IAnimationCompleteReceiver* receiver = nullptr,
                    AnimCompleteFunction completeFunction = &IAnimationCompleteReceiver::animationComplete,
                    const std::string& completeIdentifier = "");
};

#endif //CF_FLIP_ANIMATION_H
