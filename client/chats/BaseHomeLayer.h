//
//  BaseHomeLayer.h
//  chats
//
//  Created by 王凤全 on 13-9-6.
//
//

#ifndef __chats__BaseHomeLayer__
#define __chats__BaseHomeLayer__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class BaseHomeLayer: public CCLayer
{
	
private:
	CCLayerColor *_selectBgLayer;
	
public:
	virtual bool init();
	void updateTabbarSelectIndex(int index);
	
};

#endif /* defined(__chats__BaseHomeLayer__) */
