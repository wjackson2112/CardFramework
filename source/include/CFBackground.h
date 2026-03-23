//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 5/25/2024.
//

#ifndef CF_BACKGROUND_H
#define CF_BACKGROUND_H

#include <string>
#include "Entity.h"
#include "Texture2D.h"

#include "IOptionsReceiver.h"

class CFBackground : public Entity, public IOptionsReceiver
{
public:
    CFBackground();
    ~CFBackground();

    void resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes);
};

#endif //CF_BACKGROUND_H
