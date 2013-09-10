//
//  Main.cpp
//  chats
//
//  Created by 付剑超 on 13-8-12.
//
//

#include "Main.h"
#include "MessageHandle.h"
#include "SimpleAudioEngine.h"
#include "MessageHomeLayer.h"
#include "FriendsHomeLayer.h"
#include "ForumHomeLayer.h"
#include "SquareHomeLayer.h"
#include "WebSocketHandle.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Main::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Main *layer = Main::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Main::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }

    return true;
}


void Main::messageClickedCallBack(CCObject* pSender)
{
    MessageHomeLayer *messageHomeLayer = MessageHomeLayer::create();
    CCScene *pScene = CCScene::create();
    pScene->addChild(messageHomeLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void Main::friendsClickedCallBack(CCObject* pSender)
{
    FriendsHomeLayer *friendsHomeLayer = FriendsHomeLayer::create();
    CCScene *pScene = CCScene::create();
	pScene->addChild(friendsHomeLayer, 1, LAYER_TAG_FRIENDHOME);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void Main::forumClickedCallBack(CCObject* pSender)
{
    ForumHomeLayer *forumHomeLayer = ForumHomeLayer::create();
    CCScene *pScene = CCScene::create();
    pScene->addChild(forumHomeLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}


void Main::squareClickedCallBack(CCObject* pSender)
{
    SquareHomeLayer *squareHomeLayer = SquareHomeLayer::create();
    CCScene *pScene = CCScene::create();
    pScene->addChild(squareHomeLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}
