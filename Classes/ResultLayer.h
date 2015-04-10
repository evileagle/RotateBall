//
//  ScoreLayer.h
//  Ballthrow
//
//  Created by 李春雨 on 14/11/20.
//
//

#ifndef __Ballthrow__ResultLayer__
#define __Ballthrow__ResultLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace ui;

class BTMainMap;
class ShareNotify;

class BTResultLayer : public Layer {
public:
    static BTResultLayer* create(BTMainMap* map);
    void onStartTouch(Ref* ref, Widget::TouchEventType type);
    void onGameOver(EventCustom* event);
CC_CONSTRUCTOR_ACCESS:
    bool init(BTMainMap* map);
private:
    void startNewGame();
    Point _position;
    Label* _scoreLabel;
    Label* _bestLabel;
    Label* _scoreTitle;
    Label* _bestTitle;
    Sprite* _diamond;
};

#endif /* defined(__Ballthrow__ScoreLayer__) */
