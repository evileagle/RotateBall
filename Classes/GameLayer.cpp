//
//  GameLayer.cpp
//  Ballthrow
//
//  Created by 李春雨 on 14-10-25.
//
//

#include "GameLayer.h"
#include "BTEvent.h"
#include "MainMap.h"
#include "BTScore.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

const char* BTGameLayer::PIN_PICTURE_NAME = "pin.png";

BTGameLayer::BTGameLayer()
: _arrow(nullptr)
, _pin1(nullptr)
, _pin2(nullptr)
, _pin3(nullptr)
, _ball(nullptr)
, _canTouch(true)
{
    
}

BTGameLayer* BTGameLayer::create(BTMainMap *map) {
    BTGameLayer* result = new BTGameLayer;
    if (result != nullptr && result->init(map)) {
        result->autorelease();
        return result;
    } else {
        CC_SAFE_DELETE(result);
        return nullptr;
    }
}

bool BTGameLayer::init(BTMainMap *map) {
    if (!Layer::init()) {
        return false;
    }
    SimpleAudioEngine::getInstance()->preloadEffect("rotate.m4a");
    _map = map;
    _map->retain();
    //VELOCITY = VELOCITY*_map->getScaleX();
    _tileSize = _map->getTileSize();
    _r = _tileSize.width*1.5f;
    _visibleSize = map->getContentSize();
    setContentSize(Size(_visibleSize.width, _visibleSize.height*1.5));
    getEventDispatcher()->addCustomEventListener(EVENT_NAME_GAME_START, CC_CALLBACK_1(BTGameLayer::onGameStart, this));
    return true;
}

void BTGameLayer::onEnter() {
    _scene = getScene();
    reset();
    Layer::onEnter();
}

void BTGameLayer::reset() {
    _throw = false;
    _degree = 0.0f;
    _direction = 1;
    _canTouch = true;
    BTScore::getInstance()->resetScore();
    _pin1Pos.set(_tileSize.width*4+16, _tileSize.height*4+16);
    _pin2Pos.set(_tileSize.width*6+16, _tileSize.height*9+16);
    if (_pin1) {
        removeChild(_pin1);
        _pin1 = nullptr;
    }
    
    if (_pin2) {
        removeChild(_pin2);
        _pin2 = nullptr;
    }
    
    if (_pin3) {
        removeChild(_pin3);
        _pin3 = nullptr;
    }
    
    _pin1 = createPin(_pin1Pos);
    _pin2 = createPin(_pin2Pos);
    
    createBall();
    createArrow();
    createLabel();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    scheduleUpdate();
    _rotateSound = SimpleAudioEngine::getInstance()->playEffect("rotate.m4a", true, 0.5);
}

void BTGameLayer::onGameStart(cocos2d::EventCustom *event) {
    reset();
}

void BTGameLayer::createLabel() {
    _addScore = Label::createWithTTF("", "OCRAStd.ttf", 20);
    _addScore->enableShadow();
    _addScore->setOpacity(0);
    _addScore->setTextColor(Color4B::ORANGE);
    addChild(_addScore);
}

void BTGameLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(BTGameLayer::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    
    DrawPrimitives::setPointSize(10);
    Layer::draw(renderer, transform, flags);
}


void BTGameLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    glLineWidth(2);
    if (_pin3) {
        DrawPrimitives::setDrawColor4B(Color4B::RED.r, Color4B::RED.g, Color4B::RED.b, Color4B::RED.a);
        DrawPrimitives::drawCircle(_pin3Pos, _tileSize.width, 360, 30, false);
    }
    DrawPrimitives::setDrawColor4B(Color4B::BLACK.r, Color4B::BLACK.g, Color4B::BLACK.b, Color4B::BLACK.a);
    DrawPrimitives::drawCircle(_pin2Pos, _r, 360, 30, false);
    if (!_throw) {
        glLineWidth( 5.0f );
        
        DrawPrimitives::drawLine(_pin1Pos, _ball->getPosition());
    }
    kmGLPopMatrix();
}


void BTGameLayer::createArrow() {
    
    if (_arrow == nullptr) {
        _arrow = Sprite::create("arrow.png");
        Blink* blink = Blink::create(1, 1);
        RepeatForever* blinks = RepeatForever::create(blink);
        _arrow->runAction(blinks);
        addChild(_arrow);
    }
    updateArrowPosition();
}

Sprite* BTGameLayer::createPin(Point pos) {
    Sprite* pin = Sprite::create(PIN_PICTURE_NAME);
    pin->cocos2d::Node::setPosition(pos);
    addChild(pin);
    return pin;
}

void BTGameLayer::createBall() {
    _ball = Sprite::create("ball.png");
    _ball->setPosition(Point(_pin1Pos.x+_r, _pin1Pos.y));
    addChild(_ball);
}

void BTGameLayer::levelUp(int score) {
    _pin0 = _pin1;
    if (score == 1) {
        _pin1 = _pin2;
        _pin1Pos = _pin2Pos;
        removeChild(_pin3);
        _pin2 = nullptr;
        _pin3 = nullptr;
    } else {
        _pin1 = _pin3;
        _pin1Pos = _pin3Pos;
        removeChild(_pin2);
        _pin2 = nullptr;
        _pin3 = nullptr;
    }
    int rad = rand()%3;
    Point pos((rad*2+2)*_tileSize.width+16, _tileSize.height*14);
    _pin2 = createPin(pos);
    _pin2Pos = pos;
    if (rad == 0 || rad == 2) {
        int pos = 2 - rad;
        _pin3Pos.set((pos*2+2)*_tileSize.width+16, _tileSize.height*14);
        _pin3 = createPin(_pin3Pos);
    }
    if (_arrow) {
        removeChild(_arrow);
        _arrow = nullptr;
    }
    
    _degree = (_ball->getPosition()-_pin1Pos).getAngle();
    float deltaDegree = _velocity.getAngle() - (_pin1Pos-_throwPos).getAngle();
    _direction =  deltaDegree <= 0 ? 1 : -1;
    _ball->setPosition(_r*Point(cosf(_degree), sinf(_degree))+_pin1Pos);
    BTScore::getInstance()->addScore(score);
    Action* action = Sequence::create(MoveBy::create(0.5f, Vec2(0, _tileSize.height*-5)),
                                      CallFunc::create(CC_CALLBACK_0(BTGameLayer::onUpOver, this)),
                                      NULL);
    runAction(action);
    _addScore->setString("+"+ std::to_string(score));
    _addScore->setOpacity(255);
    _addScore->setPosition(_pin1Pos);
    Action* labelAction = Sequence::create(MoveBy::create(0.25, Point(0, _tileSize.width)), FadeOut::create(0.25), NULL);
    _addScore->runAction(labelAction);
    SimpleAudioEngine::getInstance()->playEffect("levelup.m4a");
}

void BTGameLayer::onUpOver() {
    setPosition(Point::ZERO);
    _pin1Pos -= Point(0, _tileSize.height*5);
    _pin2Pos -= Point(0, _tileSize.height*5);
    _pin1->setPosition(_pin1Pos);
    _pin2->setPosition(_pin2Pos);
    if (_pin3) {
        _pin3Pos -= Point(0, _tileSize.height*5);
        _pin3->setPosition(_pin3Pos);
    }
    _ball->setPosition(_ball->getPosition()-Point(0, _tileSize.height*5));
    removeChild(_pin0);
    getEventDispatcher()->dispatchCustomEvent(EVENT_NAME_LEVEL_UP);
    _rotateSound = SimpleAudioEngine::getInstance()->playEffect("rotate.m4a", true, 0.5);
    _canTouch = true;
}

void BTGameLayer::updateArrowPosition() {
    _arrow->setPosition(_pin2Pos-Point(0, _r+_tileSize.height/2));
}

void BTGameLayer::gameOver() {
    getEventDispatcher()->dispatchCustomEvent(EVENT_NAME_GAME_OVER);
    removeChild(_ball);
    _ball = nullptr;
    setTouchEnabled(false);
    unscheduleUpdate();
    SimpleAudioEngine::getInstance()->stopEffect(_rotateSound);
    SimpleAudioEngine::getInstance()->playEffect("gameover.m4a");
}

bool BTGameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    if (_canTouch) {
        _throw = true;
        _canTouch = false;
        float degree = (_ball->getPosition()-_pin1->getPosition()).getAngle()+3.14/2;
        _velocity = _direction*2*3.14*_r*Vec2(cosf(degree), sinf(degree));
        _throwPos = _ball->getPosition();
        SimpleAudioEngine::getInstance()->stopEffect(_rotateSound);
        SimpleAudioEngine::getInstance()->playEffect("throw.m4a");
    }
    return true;
}

void BTGameLayer::update(float delta) {
    if (_throw) {
        Point postion = _velocity*delta+_ball->getPosition();
        _ball->setPosition(postion);
        if ((_ball->getPosition()-_pin2Pos).length() <= _r+16.0f) {
            _throw = false;
            levelUp(1);
        } else if (_pin3 && (_ball->getPosition()-_pin3Pos).length() <= _r) {
            _throw = false;
            levelUp(2);
        } else if (!Rect(0,0,_visibleSize.width,_visibleSize.height).containsPoint(_ball->getPosition())) {
            gameOver();
        }
    } else {
        float deltaDegree = delta*2*3.14*_direction;
        _ball->setPosition(_ball->getPosition().rotateByAngle(_pin1->getPosition(), deltaDegree));
    }
}
