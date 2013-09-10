//
//  MessageList.h
//  chats
//
//  Created by 付剑超 on 13-8-16.
//
//

#ifndef __chats__MessageList__
#define __chats__MessageList__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include <iostream>
#include "MessageHandle.h"
#include "WebSocket.h"
#include "WebSocketHandle.h"
#ifndef namelong
#define namelong 100
#endif

using namespace cocos2d;
using namespace CocosDenshion;


static int messagenum;
static cocos2d::CCLayerColor *listlayer;
static cocos2d::CCLayerColor *toplayer;
static cocos2d::CCLayerColor *inputLyer;

static cocos2d::ccColor3B blackcolor;

static std::string dbFilename;
static std::string username;

static cocos2d::extension::CCEditBox *box2;

static cocos2d::CCArray *obstacle;

static bool msglistinit;
static int isControl;
static cocos2d::CCMenuItemFont *recemenu;
static cocos2d::CCMenuItemFont *sendmenu;

static cocos2d::CCSprite *backbutton;
//static WebsocketHandle *handle;
class MessageList : public cocos2d::CCLayerColor,public cocos2d::extension::CCEditBoxDelegate
{
    
private:

public:
    
    CCLayerColor *messagelayer;
    void AddMessageLayer();
    void AddMsgtolayer(const messageDate messagedate);
    void AddinputLayer();
    
    

public:
   
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    void recivetest();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void registerWithTouchDispatcher();
    bool CCTouchBegan(cocos2d::CCSet *ptouches, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchesEnded(cocos2d::CCSet *ptouches, cocos2d::CCEvent *event);
    void ccTouchesCancelled(cocos2d::CCSet *ptouches, cocos2d::CCEvent *event);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);

    
    // implement the "static node()" method manually
    CREATE_FUNC(MessageList);
    
};

#endif
