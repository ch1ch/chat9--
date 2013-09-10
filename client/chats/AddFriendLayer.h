//
//  AddFriendLayer.h
//  chats
//
//  Created by 付剑超 on 13-8-29.
//
//

#ifndef __chats__AddFriendLayer__
#define __chats__AddFriendLayer__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "WebSocketHandle.h"
#include "BaseHomeLayer.h"

USING_NS_CC;

extern WebsocketHandle *webSocketHanle;

class AddFriendLayer: public BaseHomeLayer,public cocos2d::extension::CCEditBoxDelegate
{
private:
    void AddinputLayer();
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
	
public:
    cocos2d::extension::CCEditBox *textbox;
    
	virtual bool init ();
    
    
	CREATE_FUNC (AddFriendLayer);
};

#endif /* defined(__chats__AddFriendLayer__) */
