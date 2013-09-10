//
//  LoadingPage.h
//  chats
//
//  Created by 付剑超 on 13-8-16.
//
//

#ifndef __chats__LoadingPage__
#define __chats__LoadingPage__

#include <iostream>
#include "cocos2d.h"

typedef enum
{
	TargetScene_Message,
    TargetScene_Friends,
  	TargetScene_Forum,
    TargetScene_Square
} TargetScenes;

class LoadingPage : public cocos2d::CCLayerColor
{
public:
    LoadingPage();
    ~LoadingPage();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
   // virtual bool init();
    
    // there's no 'iiklmiklmiklmijlmijlmijlmijlmjkjkjkjkmjkmkkjlnjlnjlnjlnjlnjlnd' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void sceneWithTargetScene(const TargetScenes targetScene);
    
    void initWithTargetScene(const TargetScenes targetScene);
    
    void Chatupdate(float dt);
    
    TargetScenes targetScene_;
    
    // implement the "static node()" method manually
    CREATE_FUNC(LoadingPage);
};




#endif /* defined(__chats__LoadingPage__) */
