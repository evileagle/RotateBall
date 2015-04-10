//
//  MainMap.h
//  Ballthrow
//
//  Created by 李春雨 on 14/12/1.
//
//

#ifndef __Ballthrow__MainMap__
#define __Ballthrow__MainMap__

#include "cocos2d.h"

USING_NS_CC;

class BTMainMap : public TMXTiledMap {
public:
    static BTMainMap* create(const std::string& tmxFile);
    
CC_CONSTRUCTOR_ACCESS:
    BTMainMap();
    ~BTMainMap();
    bool initWithTMXFile(const std::string& tmxFile);
private:
    TMXMapInfo* _mapinfo;
};

#endif /* defined(__Ballthrow__MainMap__) */
