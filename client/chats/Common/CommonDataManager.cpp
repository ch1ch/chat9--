//
//  CommonDataManager.cpp
//  chats
//
//  Created by 王凤全 on 13-9-6.
//
//

#include "CommonDataManager.h"


int CommonDataManager::getMyUserID()
{
	return 12;
}

int CommonDataManager::getTestTargetUserID()
{
	return 1;
}

float CommonDataManager::getImageScale()
{
	return CCDirector::sharedDirector()->getContentScaleFactor();
}
