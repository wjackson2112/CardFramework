//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 1/9/2023.
//

#ifndef CF_CARD_H
#define CF_CARD_H

#include <map>

#include "Entity.h"
#include "IAnimationCompleteReceiver.h"
#include "CFPilable.h"

class CFGameMode;
class CFGameState;


// TODO: This should probably be dynamic
enum CFCardBackColor
{
    CF_BACK_MIN,
    CF_BACK_BLUE=CF_BACK_MIN,
    CF_BACK_RED,
    CF_BACK_GREEN,
    CF_BACK_PURPLE,
    CF_BACK_MAX=CF_BACK_PURPLE
};

enum CFCardSuit
{
    CF_SUIT_MIN,
    CF_SUIT_CLUB=CF_SUIT_MIN,
    CF_SUIT_DIAMOND,
    CF_SUIT_SPADE,
    CF_SUIT_HEART,
    CF_SUIT_MAX=CF_SUIT_HEART
};

const std::string cardSuitStrings[6] = {
        "clubs",
        "diamonds",
        "spades",
        "hearts"
};

enum CFCardRank
{
    CF_RANK_MIN,
    CF_RANK_ACE=CF_RANK_MIN,
    CF_RANK_TWO,
    CF_RANK_THREE,
    CF_RANK_FOUR,
    CF_RANK_FIVE,
    CF_RANK_SIX,
    CF_RANK_SEVEN,
    CF_RANK_EIGHT,
    CF_RANK_NINE,
    CF_RANK_TEN,
    CF_RANK_JACK,
    CF_RANK_QUEEN,
    CF_RANK_KING,
    CF_RANK_MAX=CF_RANK_KING
};

const std::string cardValueStrings[15] = {
        "ace",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "jack",
        "queen",
        "king"
};

class CFCard : public CFPilable//, public IAnimationCompleteReceiver
{
    CFCardSuit suit;
    CFCardRank rank;
    CFCardBackColor color;
    bool faceUp = true;

    CFGameState* gameState;

    glm::vec2 size;

public:
    CFCard(glm::vec2 position, CFCardSuit suit, CFCardRank rank, bool faceUp, CFGameState* gameState);
    ~CFCard();

    CFCardSuit getSuit() { return suit; }
    CFCardRank getRank() { return rank; }

    CFPilable* getClosestOverlap();
    bool containsPoint(glm::vec2 point);
    bool isTopmostAtPoint(glm::vec2 point);
    bool isTopmost();

    void flip(IAnimationCompleteReceiver* receiver = nullptr, AnimCompleteFunction completeFunction = &IAnimationCompleteReceiver::animationComplete, const std::string &completeIdentifier = "");
    void flipTexture(std::string identifier);
    bool isFaceUp() { return faceUp; }
    bool isFaceDown() { return !isFaceUp(); }
    bool hasUnfinishedAnimations();

    void select();
    void deselect();

    void scaleToWidth(float width);
    void scaleToHeight(float height);

    //CFPilable
    void moveTo(glm::vec3 target, IAnimationCompleteReceiver* receiver, const std::string &completeIdentifier, AnimCompleteFunction completeFunction) override;
    void setSize(glm::vec2 size) override;
    glm::vec2 getSize() { return size; }

    //IAnimationCompleteReceiver
    void animationComplete(std::string identifier) override;
};

#endif //CF_CARD_H
