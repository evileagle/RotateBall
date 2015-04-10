//
//  ScoreLayer.cpp
//  Ballthrow
//
//  Created by 李春雨 on 14/11/20.
//
//

#include "ResultLayer.h"
#include "BTEvent.h"
#include "MainMap.h"
#include "BTScore.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

BTResultLayer* BTResultLayer::create(BTMainMap *map) {
    BTResultLayer* result = new BTResultLayer;
    if (result != nullptr && result->init(map)) {
        result->autorelease();
        return result;
    } else {
        CC_SAFE_DELETE(result);
        return nullptr;
    }
}

bool BTResultLayer::init(BTMainMap* map) {
    if (!Layer::init()) {
        return false;
    }

    LayerColor* background = LayerColor::create(Color4B::BLACK);
    background->setOpacity(200);
    addChild(background, 0);
    
    Sprite* gameover = Sprite::create("gameover.png");
    gameover->setPosition(map->getTileSize().width*4.5, map->getTileSize().height*7);
    addChild(gameover);
    
    _diamond = Sprite::create();
    _diamond->setPosition(map->getTileSize().width*4.5, map->getTileSize().height*6);
    gameover->addChild(_diamond);
    
    _scoreTitle = Label::createWithTTF("Score", "CarterOne.ttf", 16);
    _scoreTitle->setPosition(map->getTileSize().width*4, map->getTileSize().height*4.5);
    _scoreTitle->setColor(Color3B::WHITE);
    _scoreTitle->enableOutline(Color4B::BLACK, 2);
    gameover->addChild(_scoreTitle);
    
    _scoreLabel = Label::createWithTTF(std::to_string(BTScore::getInstance()->getScore()),
                                               "OCRAStd.ttf", 16);
    _scoreLabel->setPosition(map->getTileSize().width*7, map->getTileSize().height*4.5);
    _scoreLabel->setColor(Color3B::WHITE);
    _scoreLabel->enableOutline(Color4B::BLACK, 2);
    gameover->addChild(_scoreLabel);
    
    _bestTitle = Label::createWithTTF("Best Score", "CarterOne.ttf", 16);
    _bestTitle->setPosition(map->getTileSize().width*4, map->getTileSize().height*3.5);
    _bestTitle->setColor(Color3B::WHITE);
    _bestTitle->enableOutline(Color4B::BLACK, 2);
    gameover->addChild(_bestTitle);
    
    _bestLabel = Label::createWithTTF(std::to_string(BTScore::getInstance()->getBest()),
                                               "OCRAStd.ttf", 16);
    _bestLabel->setPosition(map->getTileSize().width*7, map->getTileSize().height*3.5);
    _bestLabel->setColor(Color3B::WHITE);
    _bestLabel->enableOutline(Color4B::BLACK, 2);
    gameover->addChild(_bestLabel);
    
    Button* startButton = Button::create("start.png");
    startButton->setTouchEnabled(true);
    startButton->addTouchEventListener(CC_CALLBACK_2(BTResultLayer::onStartTouch, this));
    startButton->setPosition(Point(map->getTileSize().width*4.5, map->getTileSize().height*1.5));
    gameover->addChild(startButton);
    setContentSize(map->getContentSize());
    getEventDispatcher()->addCustomEventListener(EVENT_NAME_GAME_OVER, CC_CALLBACK_1(BTResultLayer::onGameOver, this));
    
    _position = Point(0, map->getContentSize().height);
    setPosition(_position);
    return true;
}

void BTResultLayer::onStartTouch(cocos2d::Ref *ref, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::BEGAN) {
            startNewGame();
    }
}

void BTResultLayer::startNewGame() {
    runAction(JumpTo::create(0.3, _position, 0, 1));
    getEventDispatcher()->dispatchCustomEvent(EVENT_NAME_GAME_START);
}

void BTResultLayer::onGameOver(cocos2d::EventCustom *event) {
    int Level = 3;
    int score = BTScore::getInstance()->getScore();
    if (score < 3) {
        Level = 3;
    } else if (score < 8) {
        Level = 2;
    } else if (score < 20) {
        Level = 1;
    } else {
        Level = 0;
    }
    _diamond->setTexture("diamond.png");
    _diamond->setTextureRect(Rect(0,Level*32, 32, 32));
    _scoreLabel->setString(std::to_string(score));
    _bestLabel->setString(std::to_string(BTScore::getInstance()->getBest()));
    runAction(Sequence::create(JumpTo::create(0.3, Point::ZERO, 0, 1), JumpTo::create(0.2, Point::ZERO, 50, 1), NULL));
}
