//
//  SquareHomeLayer.cpp
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#include "SquareHomeLayer.h"
#include "Main.h"
#include "CommonDataManager.h"

bool SquareHomeLayer::init ()
{
	if (!BaseHomeLayer::init())
        return false;
    updateTabbarSelectIndex(3);
	
    //test
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite *sprite1 = CCSprite::create("guangchang.png");
    sprite1->setPosition(ccpAdd(ccp(0, 0), ccp(size.width / 2, size.height / 2)));
    sprite1->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
    addChild(sprite1,20);
    	
	return true;
}
