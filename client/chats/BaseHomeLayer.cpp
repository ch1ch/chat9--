//
//  BaseHomeLayer.cpp
//  chats
//
//  Created by 王凤全 on 13-9-6.
//
//

#include "BaseHomeLayer.h"
#include "Main.h"
#include "CommonDataManager.h"
#include "DatabaseManager.h"

bool BaseHomeLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
		
	DatabaseManager::sharedInstance();
	
	CCSize viewSize = CCDirector::sharedDirector()->getWinSize();
	
    //背景颜色
    CCLayerColor *background = CCLayerColor::create(ccc4(247,222,166,255));
    addChild(background,1);
	
    //菜单条颜色
    /*
    CCLayerColor *buttomground = CCLayerColor::create(ccc4(255,165,0,255));
    buttomground->setContentSize(CCSize(viewSize.width, Memu_Height));
    addChild(buttomground,30);
    */
    
    CCSprite *menubg=CCSprite::create("menu-bg.png");
    menubg->setPosition(CCSize(viewSize.width*1/2.f,Memu_Height*1/2.f));
    menubg->setScaleX(viewSize.width/menubg->getContentSize().width);
    menubg->setScaleY(Memu_Height/menubg->getContentSize().height);
    addChild(menubg,30);
    
    
    //选中菜单项色块
	//_selectBgLayer = CCLayerColor::create(ccc4(238,213,183,255));
    _selectBgLayer = CCLayerColor::create(ccc4(255,255,255,100));
    _selectBgLayer->setContentSize(CCSize(viewSize.width/4.f, Memu_Height));
    _selectBgLayer->setPosition(CCPointZero);
    addChild(_selectBgLayer,30);
    
    //菜单
    CCMenuItemImage *itemMessage = CCMenuItemImage::create("xiaoxi.png", "xiaoxi.png", this, menu_selector(Main::messageClickedCallBack));
    CCMenuItemImage *itemFriends = CCMenuItemImage::create("haoyou.png", "haoyou.png", this, menu_selector(Main::friendsClickedCallBack));
    CCMenuItemImage *itemForum = CCMenuItemImage::create("luntan.png", "luntan.png", this, menu_selector(Main::forumClickedCallBack));
    CCMenuItemImage *itemSquare = CCMenuItemImage::create("guangchang.png", "guangchang.png", this, menu_selector(Main::squareClickedCallBack));

    itemMessage->setPosition(ccp(viewSize.width/8.f, Memu_Height*1/2.f));
    itemFriends->setPosition(ccp(viewSize.width*(1.f/4 + 1.f/8), Memu_Height*1/2.f));
    itemForum->setPosition(ccp(viewSize.width*(2.f/4 + 1.f/8), Memu_Height*1/2.f));
    itemSquare->setPosition(ccp(viewSize.width*(3.f/4 + 1.f/8), Memu_Height*1/2.f));
	
	CCMenu *menu = CCMenu::create(itemMessage, itemFriends, itemForum, itemSquare, NULL);
    menu->setContentSize(CCSize(viewSize.width,Memu_Height));
    menu->setPosition(CCPointZero);
	
    addChild(menu,40);
    
    //头部
    CCSprite *topbg=CCSprite::create("top-bg.png");
    topbg->setPosition(CCSize(viewSize.width*1/2.f,viewSize.height -Memu_Height*1/2.f));
    topbg->setScaleX(viewSize.width/topbg->getContentSize().width);
    topbg->setScaleY(Memu_Height/topbg->getContentSize().height);
    addChild(topbg,30);
    
    /*
    
    //菜单文字
    CCLabelTTF *labelMessage = CCLabelTTF::create("消息", "Thonburi-Bold", 18);
    labelMessage->setPosition(ccp(viewSize.width*(0.f/4 + 1.f/8), Memu_Height/4.f));
    addChild(labelMessage,50);
    
    CCLabelTTF *labelFriends = CCLabelTTF::create("好友", "Thonburi-Bold", 18);
    labelFriends->setPosition(ccp(viewSize.width*(1.f/4 + 1.f/8), Memu_Height/4.f));
    addChild(labelFriends,51);
    
    CCLabelTTF *labelForum = CCLabelTTF::create("论坛", "Thonburi-Bold", 18);
    labelForum->setPosition(ccp(viewSize.width*(2.f/4 + 1.f/8), Memu_Height/4.f));
    addChild(labelForum,52);
    
    CCLabelTTF *labelSquare = CCLabelTTF::create("广场", "Thonburi-Bold", 18);
    labelSquare->setPosition(ccp(viewSize.width*(3.f/4 + 1.f/8), Memu_Height/4.f));
    addChild(labelSquare,53);
    */
    
    //头部色块
    //选中菜单项色块
    /*
    CCLayerColor *topground = CCLayerColor::create(ccc4(255,165,0,255));
    topground->setContentSize(CCSize(viewSize.width, Memu_Height));
    topground->setPosition(ccp(0, viewSize.height - Memu_Height));
    addChild(topground,50);
	
*/

         
    
	return true;
}

void BaseHomeLayer::updateTabbarSelectIndex(int index)
{
	float viewWidth = CCDirector::sharedDirector()->getWinSize().width;
	_selectBgLayer->setPosition(ccp(index * viewWidth / 4, 0));
}
