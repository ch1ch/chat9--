//
//  FriendsHomeLayer.h
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#ifndef __chats__FriendsHomeLayer__
#define __chats__FriendsHomeLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "WebSocketHandle.h"
#include "CELLMenu.h"
#include "MessageHandle.h"
#include "BaseHomeLayer.h"

#ifndef friendcolumn
#define friendcolumn 8
#endif

USING_NS_CC;
USING_NS_CC_EXT;

extern WebsocketHandle *webSocketHanle;
extern int ownid;


class FriendsHomeLayer:public cocos2d::extension::CCTableViewDataSource,cocos2d::extension::CCTableViewDelegate,public BaseHomeLayer
{
private:
	CCArray *_friendArray;
	CCLayerColor *thetoplayer;
	cocos2d::CCMenuItemImage *addmenu;
	cocos2d::ccColor3B blackcolor;
	CCTableView *_friendTableView;
	
public:
	virtual bool init ();
	~FriendsHomeLayer();
   
	void onRequestFriendDataReceived(CCArray *friendArray);
	
	void ReplacetoFirendScene(CCObject* pSender);
    void ReplacetoMsgScene(CCObject* pSender);
    
    void registerWithTouchDispatcher();
    bool CCTouchBegan(cocos2d::CCSet *ptouches, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchesEnded(cocos2d::CCSet *ptouches, cocos2d::CCEvent *event);
    void ccTouchesCancelled(cocos2d::CCSet *ptouches, cocos2d::CCEvent *event);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	CREATE_FUNC (FriendsHomeLayer);
};


#endif /* defined(__chats__FriendsHomeLayer__) */
