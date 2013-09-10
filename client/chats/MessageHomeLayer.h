//
//  MessageHomeScene.h
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#ifndef __chats__MessageHomeScene__
#define __chats__MessageHomeScene__

#include <iostream>

#include "cocos2d.h"
USING_NS_CC;
#include "CCScrollView.h"
#include "WebSocketHandle.h"
#include "CELLMenu.h"
#include "MessageHandle.h"
#include "BaseHomeLayer.h"

#ifndef chatcolumn
#define chatcolumn 8
#endif

extern WebsocketHandle *webSocketHanle;
extern int toID;


#include "MessageHandle.h"

class MessageHomeLayer: public cocos2d::extension::CCTableViewDataSource,cocos2d::extension::CCTableViewDelegate,public BaseHomeLayer
{
private:

	CCArray *_chatListArray;

    cocos2d::ccColor3B blackcolor;
    CCLayerColor *thetoplayer;
    
    //cocos2d::CCMenuItemImage *addmenu;
    
    CCMenuItem *groupChatMenuItem;
    
    ChatDate *chatdate;


    
public:
    ~MessageHomeLayer();
	virtual bool init ();
    
    cocos2d::extension::CCTableView* tableView;
    
    void testtest();
    
   // void AddChattolayer(const ChatDate onechatdate);
    
    //void Chatlist();
    int strlength(const std::string &str);
    
    void registerWithTouchDispatcher();
    bool CCTouchBegan(cocos2d::CCSet *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchesEnded(cocos2d::CCSet *touch, cocos2d::CCEvent *event);
    void ccTouchesCancelled(cocos2d::CCSet *touch, cocos2d::CCEvent *event);
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    //void insertCellAtIndex(unsigned int idx);

    
	CREATE_FUNC (MessageHomeLayer);

	void onGroupMessageClicked();
};

#endif /* defined(__chats__MessageHomeScene__) */
