//
//  MainMap.cpp
//  Ballthrow
//
//  Created by 李春雨 on 14/12/1.
//
//

#include "MainMap.h"
#include "GameLayer.h"
#include "ScoreLayer.h"
#include "ResultLayer.h"

BTMainMap::BTMainMap() {
    
}

BTMainMap::~BTMainMap() {
    CC_SAFE_RELEASE_NULL(_mapinfo);
}

BTMainMap* BTMainMap::create(const std::string &tmxFile) {

    BTMainMap* map = new BTMainMap;
    if (map != nullptr && map->initWithTMXFile(tmxFile)) {
        map->autorelease();
        return map;
    } else {
        delete map;
        return nullptr;
    }
}

bool BTMainMap::initWithTMXFile(const std::string &tmxFile) {
    _mapinfo = TMXMapInfo::create(tmxFile);
    if (_mapinfo) {
        _mapinfo->retain();
        //setContentSize(Size::ZERO);
        TMXTiledMap::buildWithMapInfo(_mapinfo);
        setScale(Director::getInstance()->getVisibleSize().width/getContentSize().width);
        BTGameLayer* gameLayer = BTGameLayer::create(this);
        addChild(gameLayer, 2);
        BTScoreLayer* scoreLayer = BTScoreLayer::create(this);
        addChild(scoreLayer, 2);
        BTResultLayer* resultLayer = BTResultLayer::create(this);
        addChild(resultLayer, 2);
        return true;
    }
    return false;
}