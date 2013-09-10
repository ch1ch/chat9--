//
//  AddFriendLayer.cpp
//  chats
//
//  Created by 付剑超 on 13-8-29.
//
//

#include "AddFriendLayer.h"
#include "Main.h"
#include "FriendsHomeLayer.h"
#include "WebSocketHandle.h"
#include "ProtoMeta.pb.h"

//#include "AppDelegate.h"


bool AddFriendLayer::init ()
{
	if (!BaseHomeLayer::init())
        return false;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCLabelTTF* topname = CCLabelTTF::create("添加好友", "Thonburi", 34);
    topname->setPosition( ccp(size.width / 2, size.height *0.93) );
    addChild(topname, 60);
    
    AddinputLayer();
    
    
	return true;
}

void AddFriendLayer::AddinputLayer(){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::extension::CCScale9Sprite *sacel9SprG=cocos2d::extension::CCScale9Sprite::create("whitebg.png");
    textbox=cocos2d::extension::CCEditBox::create(CCSizeMake(size.width*0.9,size.height*0.09), sacel9SprG);
    textbox->setPlaceHolder("输入好友ID");
    textbox->setPosition(ccp(size.width*0.5,size.height*0.5));
    textbox->setReturnType(cocos2d::extension::kKeyboardReturnTypeSend);
    textbox->setInputMode(cocos2d::extension::kEditBoxInputModeNumeric);
    textbox->setMaxLength(10);
    textbox->setColor(ccc3(0,0,0));
    addChild(textbox,50);
    //获取编辑框内的文字
    //printf("text=== %s ===\n",box2->getText());
    //CCLOG("Text:===%s====",textbox->getText());
    //
    textbox->setDelegate(this);
    
}



void AddFriendLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox)
{
    CCLOG("start edit");
}
void AddFriendLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox)
{
    
    
    //int type=1;
    
    
    // handle->sendMessage(box2->getText());
    
    //WebsocketHandle
    //textbox->setText("");
    //messageDate onemsg=getLastMsg(dbFilename.c_str(), username.c_str());
    webSocketHanle->sendMessage(proto::DataPackage_CmdType_ADD_FRIEND,atoi(textbox->getText()), 0, 0," ");
    
    FriendsHomeLayer *friendLayer = FriendsHomeLayer::create();
    CCScene *pScene = CCScene::create();
	pScene->addChild(friendLayer, 1, LAYER_TAG_FRIENDHOME);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
    
    //AddMsgtolayer(onemsg);
    
    
}

void AddFriendLayer::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text)
{
   // CCLOG("textchanged");
}
void AddFriendLayer::editBoxReturn(cocos2d::extension::CCEditBox *editBox)
{
    CCLOG("editboxreturn");
}


