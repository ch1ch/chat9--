//
//  ForumHomeScene.cpp
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#include "ForumHomeLayer.h"
#include "Main.h"
#include "CommonDataManager.h"

bool ForumHomeLayer::init ()
{
	if (!BaseHomeLayer::init())
        return false;
	updateTabbarSelectIndex(2);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF *labelTitle = CCLabelTTF::create("论坛", "Thonburi-Bold", 26);
    labelTitle->setPosition(ccp(size.width/2.f, size.height - Memu_Height/2.f));
    addChild(labelTitle,72);
    
    //菜单
    CCMenuItemImage *itemLogin = CCMenuItemImage::create("denglu.png", "denglu.png", this, menu_selector(ForumHomeLayer::loginClickedCallBack));
    CCMenuItemImage *itemSearch = CCMenuItemImage::create("souso.png", "souso.png", this, menu_selector(ForumHomeLayer::searchClickedCallBack));
    CCMenuItemImage *itemDeclare = CCMenuItemImage::create("fatie.png", "fatie.png", this, menu_selector(ForumHomeLayer::declareClickedCallBack));

    itemLogin->setPosition(ccp(size.width*0.7f/8, Memu_Height/2.f));
    itemSearch->setPosition(ccp(size.width*6.2f/8, Memu_Height/2.f));
    itemDeclare->setPosition(ccp(size.width*7.3f/8, Memu_Height/2.f));
    
    CCMenu *menutop = CCMenu::create(itemLogin, itemSearch, itemDeclare, NULL);
    menutop->setContentSize(CCSize(size.width,Memu_Height));
    menutop->setPosition(CCSize(0,size.height - Memu_Height));
    
    addChild(menutop,73);
    
    //头部标签色块
    CCLayerColor *topclassground = CCLayerColor::create(ccc4(238,213,183,255));
    topclassground->setContentSize(CCSize(size.width, ClassMemu_Height));
    topclassground->setPosition(ccp(0, size.height - Memu_Height- ClassMemu_Height));
    addChild(topclassground,80);
	
    //头部标签选项

	CCMenuItemFont::setFontName("Thonburi-Bold");
	CCMenuItemFont::setFontSize(15);

    CCMenuItemFont *hotdeclareItem = CCMenuItemFont::create("热帖", this, menu_selector(ForumHomeLayer::hotdeclareClickedCallBack));
    CCMenuItemFont *developItem = CCMenuItemFont::create("官方开发", this, menu_selector(ForumHomeLayer::developClickedCallBack));
    CCMenuItemFont *communicationItem = CCMenuItemFont::create("玩家交流", this, menu_selector(ForumHomeLayer::communicationClickedCallBack));
    CCMenuItemFont *championItem = CCMenuItemFont::create("势力争霸", this, menu_selector(ForumHomeLayer::championClickedCallBack));
    
    hotdeclareItem->setPosition(ccp(size.width/8.f, ClassMemu_Height/2));
    developItem->setPosition(ccp(size.width*(1.f/4 + 1.f/8), ClassMemu_Height/2));
    communicationItem->setPosition(ccp(size.width*(2.f/4 + 1.f/8), ClassMemu_Height/2));
    championItem->setPosition(ccp(size.width*(3.f/4 + 1.f/8), ClassMemu_Height/2));
    
    CCMenu *menuClass = CCMenu::create(hotdeclareItem, developItem, communicationItem, championItem, NULL);
    menuClass->setPosition(CCSize(0,size.height - Memu_Height - ClassMemu_Height));
    
    addChild(menuClass,81);

    
    addForumContent();
    
    
	return true;
}

void ForumHomeLayer::addForumContent()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // CCScrollView
    scrollView = cocos2d::extension::CCScrollView::create(CCSizeMake(size.width, size.height - Memu_Height*2));
    CCLayer *layer = CCLayer::create();
    
    CCSprite *sprite1 = CCSprite::create("Default.png");
    
    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);
    
    sprite1->setPosition(ccpAdd(CCPointZero, ccp(size.width / 2, size.height / 2)));
    sprite1->setScale(1.f);
    
    
    layer->addChild(sprite1);

    scrollView->setPosition(ccp(0, Memu_Height));
    scrollView->setContentOffset(CCPointZero);
    
    //layer->setContentSize(CCSizeMake(320, 480));
    
    //scrollView->setContentSize(CCSizeMake(320,480));
    
    scrollView->setContainer(layer);
    
    // 设置滚屏属性
    scrollView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
    scrollView->setDelegate(this);
    
    //this->addChild(scrollView,1000);
}

void ForumHomeLayer::hotdeclareClickedCallBack(CCObject* pSender)
{
    
}

void ForumHomeLayer::developClickedCallBack(CCObject* pSender)
{
    
}

void ForumHomeLayer::communicationClickedCallBack(CCObject* pSender)
{
    
}

void ForumHomeLayer::championClickedCallBack(CCObject* pSender)
{
    
}

void ForumHomeLayer::loginClickedCallBack(CCObject* pSender)
{
    
}

void ForumHomeLayer::searchClickedCallBack(CCObject* pSender)
{
    
}

void ForumHomeLayer::declareClickedCallBack(CCObject* pSender)
{
    
}


void ForumHomeLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *view)
{
}

void ForumHomeLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView *view)
{
}