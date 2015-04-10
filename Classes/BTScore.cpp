//
//  BTScore.cpp
//  Ballthrow
//
//  Created by 李春雨 on 14/12/6.
//
//

#include "BTScore.h"
#include "base/CCUserDefault.h"

USING_NS_CC;

BTScore* BTScore::s_instance = nullptr;

BTScore* BTScore::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new BTScore;
    }
    return s_instance;
}

BTScore::BTScore()
: _score(0)
, _is_new_best(false)
{
    _best = UserDefault::getInstance()->getIntegerForKey("best");
}

int BTScore::getScore() {
    return _score;
}

void BTScore::resetScore() {
    _score = 0;
    _is_new_best = false;
}

int BTScore::addScore(int score) {
    _score += score;
    if (_score > _best) {
        _best = _score;
        _is_new_best = true;
        UserDefault::getInstance()->setIntegerForKey("best", _best);
    }
    return _score;
}

int BTScore::getBest() {
    return _best;
}

bool BTScore::isNewBest() {
    return _is_new_best;
}