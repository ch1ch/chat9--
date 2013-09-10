//
//  CommonDataManager.h
//  chats
//
//  Created by 王凤全 on 13-9-6.
//
//

#ifndef __chats__CommonDataManager__
#define __chats__CommonDataManager__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

#endif /* defined(__chats__CommonDataManager__) */


class CommonDataManager
{
public:
	static int getMyUserID();
	static int getTestTargetUserID();
	static float getImageScale();
	
};