//
//  ScoreLayer.h
//  Ballthrow
//
//  Created by 李春雨 on 14/11/23.
//
//

#ifndef __Ballthrow__ScoreLayer__
#define __Ballthrow__ScoreLayer__

#include "cocos2d.h"

class BTMainMap;

USING_NS_CC;

class BTScoreLayer : public Layer {
public:
    static BTScoreLayer* create(BTMainMap* map);
    void setMap(BTMainMap* map);
    void onLevelUp(EventCustom* event);
    void onReset(EventCustom* event);
    void onGameOver(EventCustom* event);
CC_CONSTRUCTOR_ACCESS:
    bool init(BTMainMap* map);
private:
    Label* _scoreLabel;
};

#endif /* defined(__Ballthrow__ScoreLayer__) */
