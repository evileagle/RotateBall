//
//  GameLayer.h
//  Ballthrow
//
//  Created by 李春雨 on 14-10-25.
//
//

#ifndef __Ballthrow__GameLayer__
#define __Ballthrow__GameLayer__

#include <cocos2d.h>

USING_NS_CC;

class BTMainMap;

class BTGameLayer : public Layer {
public:
    static BTGameLayer* create(BTMainMap* map);
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onUpOver();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    void update(float delta) override;
    void reset();
    void onEnter() override;
    void onGameStart(EventCustom* event);
CC_CONSTRUCTOR_ACCESS:
    BTGameLayer();
    bool init(BTMainMap* map);
private:
    Sprite* createPin(Point pos);
    void createBall();
    void createArrow();
    void createLabel();
    void levelUp(int score);
    void gameOver();
    void updateArrowPosition();
private:
    BTMainMap* _map;
    Scene* _scene;
    Sprite* _pin0;
    Sprite* _pin1;
    Sprite* _pin2;
    Sprite* _pin3;
    Sprite* _ball;
    Sprite* _arrow;
    Point _pin1Pos;
    Point _pin2Pos;
    Point _pin3Pos;
    bool _throw;
    Point _throwPos;
    bool _canTouch;
    Size _tileSize;
    Size _visibleSize;
    Vec2 _velocity;
    Label* _addScore;
    int _direction;
    float _r;
    CustomCommand _customCommand;
    static const char* PIN_PICTURE_NAME;
    float _degree;
    unsigned int _rotateSound;
};

#endif /* defined(__Ballthrow__GameLayer__) */
