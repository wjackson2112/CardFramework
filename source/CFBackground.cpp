//
// Created by Will on 5/25/2024.
//

#include "include/CFBackground.h"
#include "AssetManager.h"
#include "Texture2D.h"
#include "SpriteComponent2D.h"
#include "OptionsManager.h"

CFBackground::CFBackground()
{
    this->transform.setPosition(glm::vec3(this->transform.getPosition().x,
                                          this->transform.getPosition().y,
                                          -0.3f));
    Shader shader = AssetManager::getInstance()->loadShader("shaders/sprite.vert",
                                                            "shaders/sprite.frag",
                                                            nullptr,
                                                            "sprite");
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets/green_felt.png", true, "green_felt");
    glm::vec2 viewportRes = OptionsManager::getInstance()->getViewportResolution();
    auto* spriteComponent = new SpriteComponent2D(shader, texture, viewportRes);
    addComponent(spriteComponent);

    OptionsManager::getInstance()->registerReceiver(this);
}

CFBackground::~CFBackground()
{
    OptionsManager::getInstance()->deregisterReceiver(this);
}

void CFBackground::resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes)
{
    getComponent<SpriteComponent2D>()->setSize(newRes);
}