//
//  MessageLayer.h
//  chats
//
//  Created by 付剑超 on 13-8-30.
//
//

#ifndef __chats__MessageLayer__
#define __chats__MessageLayer__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "WebSocketHandle.h"
#include "MessageHandle.h"


USING_NS_CC;


extern WebsocketHandle *webSocketHanle;
extern int toID;



class MessageLayer: public CCLayer,public cocos2d::extension::CCEditBoxDelegate
{
private:
	int _targetID;
	bool _isGroup;
    std::string _targetName;
	
    cocos2d::extension::CCEditBox *textbox;
    cocos2d::CCLayerColor *toplayer;
    cocos2d::CCLayerColor *inputLyer;
    cocos2d::ccColor3B blackcolor;
    
    std::string dbFilename;
    
    int listHeigth;
    
    int upmsgline;
    
    cocos2d::CCArray *obstacle;
    
    cocos2d::CCSprite *backbutton;
    
    cocos2d::CCLabelTTF *exitgroupbuttom;
    
    cocos2d::extension::CCScrollView *scroll;
    
    CCLayer *layer;
    

    void AddinputLayer();
    
    void GetMsgList();
    

    
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
	
public:
	void backToGroupList();
    
    
	static MessageLayer * create(int targetID, bool isGroup, std::string targetName);
    
	virtual bool init(int targetID, bool isGroup, std::string targetName);

	virtual bool init ();
    
    char *userid=new char[10];
    
    void AddMsgtolayer(const messageDate messagedate);
    
    void ReplacetoFirendScene(CCObject* pSender);
    
    void exitgroup();
    
    int strlength(const std::string &str);
    
    std::string coverstringtoview(std::string str,int length);
    
    void registerWithTouchDispatcher();
    bool CCTouchBegan(cocos2d::CCSet *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchesEnded(cocos2d::CCSet *touch, cocos2d::CCEvent *event);
    void ccTouchesCancelled(cocos2d::CCSet *touch, cocos2d::CCEvent *event);
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    
	CREATE_FUNC (MessageLayer);
    MessageLayer();
    virtual ~MessageLayer();
};

#endif /* defined(__chats__MessageLayer__) */
