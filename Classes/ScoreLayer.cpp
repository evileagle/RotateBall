//
//  ScoreLayer.cpp
//  Ballthrow
//
//  Created by 李春雨 on 14/11/23.
//
//

#include "ScoreLayer.h"
#include "BTEvent.h"
#include <string>
#include "MainMap.h"
#include "BTScore.h"


BTScoreLayer* BTScoreLayer::create(BTMainMap *map) {
    BTScoreLayer* result = new BTScoreLayer;
    if (result != nullptr && result->init(map)) {
        result->autorelease();
        return result;
    } else {
        CC_SAFE_DELETE(result);
        return nullptr;
    }
}

bool BTScoreLayer::init(BTMainMap *map) {
    if (!Layer::init()) {
        return false;
    }
    _scoreLabel = Label::createWithTTF(std::to_string(BTScore::getInstance()->getScore()), "OCRAStd.ttf", 48);
    _scoreLabel->setTextColor(Color4B::WHITE);
    _scoreLabel->enableOutline(Color4B::BLACK, 2);
    _scoreLabel->setPosition(map->getTileSize().width*4+16, map->getTileSize().height*7);
    addChild(_scoreLabel);
    getEventDispatcher()->addCustomEventListener(EVENT_NAME_LEVEL_UP, CC_CALLBACK_1(BTScoreLayer::onLevelUp, this));
    getEventDispatcher()->addCustomEventListener(EVENT_NAME_GAME_START, CC_CALLBACK_1(BTScoreLayer::onReset, this));
    getEventDispatcher()->addCustomEventListener(EVENT_NAME_GAME_OVER, CC_CALLBACK_1(BTScoreLayer::onGameOver, this));
    return true;
}

void BTScoreLayer::onLevelUp(EventCustom* event) {
    _scoreLabel->setString(std::to_string(BTScore::getInstance()->getScore()));
}

void BTScoreLayer::onReset(EventCustom* event) {
    _scoreLabel->setString(std::to_string(BTScore::getInstance()->getScore()));
    _scoreLabel->setVisible(true);
}

void BTScoreLayer::onGameOver(EventCustom* event) {
    _scoreLabel->setVisible(false);
}
