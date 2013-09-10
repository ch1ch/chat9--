//
//  GroupUserListLayer.cpp
//  chats
//
//  Created by 王凤全 on 13-9-3.
//
//

#include "GroupUserListLayer.h"
#include "MessageHomeLayer.h"
#include "strutil.h"
#include "AppDelegate.h"
#include "WebSocketHandle.h"
#include "ProtoMeta.pb.h"
#include "MessageLayer.h"
#include "FriendModel.h"
#include "DatabaseManager.h"
#include "CommonDataManager.h"

#define BANNER_HEIGHT		50
#define CELL_HEIGHT			60

#define TAG_BACKGROUND		100

extern WebsocketHandle *webSocketHanle;

GroupUserListLayer::~GroupUserListLayer()
{
	_friendArray->release();
	delete _selectStatus;
}

bool GroupUserListLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	_friendArray =  DatabaseManager::sharedInstance()->getAllFriend();
	_friendArray->retain();
	_selectStatus = new bool[_friendArray->count()];
	memset(_selectStatus, 0, _friendArray->count());
		
	CCSize viewSize = CCDirector::sharedDirector()->getWinSize();
	
	CCLayerColor *background = CCLayerColor::create(ccc4(247,222,166,255));
    addChild(background,1);

    //底部
	CCLayerColor *btmBannerColor = CCLayerColor::create(ccc4(255,165,0,255));
	btmBannerColor->setContentSize(CCSize(viewSize.width, viewSize.width*0.15));
	//addChild(btmBannerColor, 2);
    
    CCSprite *btmbg=CCSprite::create("top-bg.png");
    btmbg->setPosition(CCSize(viewSize.width*1/2.f,viewSize.height*1/20.f));
    btmbg->setScaleX(viewSize.width/btmbg->getContentSize().width);
    btmbg->setScaleY((viewSize.height*1/10.f)/btmbg->getContentSize().height);
    addChild(btmbg,3);
    
	//头部

    
    CCSprite *topbg=CCSprite::create("top-bg.png");
    topbg->setPosition(CCSize(viewSize.width*1/2.f,viewSize.height*19/20.f));
    topbg->setScaleX(viewSize.width/topbg->getContentSize().width);
    topbg->setScaleY((viewSize.height*1/10.f)/topbg->getContentSize().height);
    addChild(topbg,3);
    
	
	CCMenuItem *cancelBtn = CCMenuItemFont::create("取消", this, menu_selector(GroupUserListLayer::onCancelClicked));
	cancelBtn->setPosition(CCPointMake(50, viewSize.width*0.15 / 2));
	CCMenu *cancelMenu = CCMenu::create(cancelBtn, NULL);
	cancelMenu->setPosition(CCPointMake(0, viewSize.height - viewSize.width*0.15));	
	addChild(cancelMenu, 4);

	CCMenuItem *submitBtn = CCMenuItemFont::create("确定", this, menu_selector(GroupUserListLayer::onSubmitClicked));
	submitBtn->setPosition(CCPointMake(viewSize.width - 50, viewSize.width*0.15 / 2));
	CCMenu *submitMenu = CCMenu::create(submitBtn, NULL);
	submitMenu->setPosition(CCPointZero);
	addChild(submitMenu, 5);

	CCLabelTTF *titleLabel = CCLabelTTF::create("选择联系人", "Thonburi-Bold", 18);
    titleLabel->setPosition(CCPointMake(viewSize.width / 2, viewSize.height - viewSize.width*0.15 / 2));
    addChild(titleLabel, 6);
		
	_tableView = CCTableView::create(this, CCSizeMake(viewSize.width, viewSize.height - viewSize.width*0.15 * 2));
	_tableView->setDirection(kCCScrollViewDirectionVertical);
	_tableView->setPosition(CCPointMake(0, viewSize.width*0.15));
	_tableView->setDelegate(this);
	addChild(_tableView, 20);	
	
	int myID = CommonDataManager::getMyUserID();
	webSocketHanle->sendMessage(proto::DataPackage_CmdType_LIST_FRIENDS, myID, 0 , 0, NULL);

	return true;
}

void GroupUserListLayer::onCancelClicked()
{
	MessageHomeLayer *messageHomeLayer = MessageHomeLayer::create();
    CCScene *pScene = CCScene::create();
    pScene->addChild(messageHomeLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void GroupUserListLayer::onSubmitClicked()
{
	/*
	 * select count =
	 *		0	no user selected
	 *		1	single user chat
	 *		>1	group chat
	 */
	int sum = 0;
	int dataCount = _friendArray->count();
	for (int i = 0; i < dataCount; i ++)
	{
		sum += _selectStatus[i];
	}
	if (sum == 0)
	{
		onCancelClicked();
	}
	else if(sum == 1)
	{
		int index = 0;
		while (!_selectStatus[index])
		{
			index ++;
		}
		
		FriendModel *model = (FriendModel *)_friendArray->objectAtIndex(index);
		
		int targetID = atoi(model->friendID.c_str());
        
		DatabaseManager::sharedInstance()->addToChatList(" ", model->friendID, eChatTypePersonal);
        
		MessageLayer *msgLayer = MessageLayer::create(targetID, false, model->nickName);
		CCScene *nextScene = CCScene::create();
		nextScene->addChild(msgLayer);
		
		CCDirector::sharedDirector()->replaceScene(nextScene);
	}
	else
	{
		std::string  userString = "";
		int index = 0;
		
		while (index < dataCount)
		{
			if (_selectStatus[index])
			{
				FriendModel *model = (FriendModel *)_friendArray->objectAtIndex(index);
				
				if (userString.length() == 0)
				{
					userString = model->friendID;
				}
				else
				{
					userString += "," + model->friendID;
				}
			}
			
			index ++;
		}
		
		webSocketHanle->sendMessage(proto::DataPackage_CmdType_CREATE_GROUP, 0, 0, 0, userString.c_str());
	}
}

void GroupUserListLayer::onFriendUserListReceived(CCArray *dataArray)
{
	_friendArray->release();
	dataArray->retain();
	
	_friendArray = dataArray;
	
	int dataCount = dataArray->count();
	delete _selectStatus;
	_selectStatus = new bool[dataCount];
	memset(_selectStatus, 0, dataCount);

	_tableView->reloadData();
	_tableView->setContentOffset(CCPointMake(0, _tableView->getViewSize().height - dataCount * CELL_HEIGHT));
}

void GroupUserListLayer::onGroupCreateReceived(int groupID, std::string groupName)
{
	char groupIDString[20];
	sprintf(groupIDString, "%d", groupID);
	DatabaseManager::sharedInstance()->addToChatList(" ", string(groupIDString), eChatTypeGroup);

	MessageLayer *msgLayer = MessageLayer::create(groupID, true, groupName);
	CCScene *nextScene = CCScene::create();
	nextScene->addChild(msgLayer);
	
	CCDirector::sharedDirector()->replaceScene(nextScene);
}

#pragma mark -
#pragma mark CCTableViewDataSource
CCSize GroupUserListLayer::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(CCDirector::sharedDirector()->getWinSize().width, CELL_HEIGHT);
}


CCTableViewCell * GroupUserListLayer::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	CCTableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();
	}
	
	cell->removeAllChildrenWithCleanup(true);
	CCSize viewSize = CCDirector::sharedDirector()->getWinSize();
	FriendModel *model = (FriendModel *)_friendArray->objectAtIndex(idx);
	
	bool isSelected = _selectStatus[idx];
	
	if (isSelected)
	{
		CCLayerColor *background = CCLayerColor::create(ccc4(100,100,100,255));
		background->setContentSize(CCSizeMake(table->getViewSize().width, CELL_HEIGHT));
		cell->addChild(background, 40, TAG_BACKGROUND);
	}
	
	CCMenuItemImage *userImage = CCMenuItemImage::create("head1.png", "head1.png", this, NULL);
    userImage->setPosition(CCPointMake(30, CELL_HEIGHT / 2));
	cell->addChild(userImage, 50);
	
	string showString;
	if (model->nickName.length() == 0)
	{
		showString = model->friendID;
	}
	else
	{
		showString = model->nickName;
	}
	
	CCMenuItemFont *userName = CCMenuItemFont::create(showString.c_str(), this, NULL);
	userName->setFontName("Thonburi"); 
	userName->setColor(ccc3(0, 0, 0));
	userName->setPosition(CCPointMake(viewSize.width / 2, CELL_HEIGHT / 2));
	cell->addChild(userName, 50);

	CCMenuItemImage *seperator = CCMenuItemImage::create("grayline.png", "grayline.png");
	seperator->setPosition(CCPointMake(viewSize.width, 0));
	seperator->setScaleX(viewSize.width / 2);
	seperator->setScaleY(0.3);
	cell->addChild(seperator, 50);
	return cell;
}

unsigned int GroupUserListLayer::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return _friendArray->count();
}

void GroupUserListLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	int index = cell->getIdx();
	_selectStatus[index] = !_selectStatus[index];
	CCLayerColor *background = (CCLayerColor *)cell->getChildByTag(TAG_BACKGROUND);
	if (background)
	{
		ccColor3B color = _selectStatus[index] ? ccc3(100, 100, 100) : ccc3(255, 255, 255);
		background->setColor(color);
	}
	else
	{
		background = CCLayerColor::create(ccc4(100,100,100,255));
		background->setContentSize(CCSizeMake(table->getViewSize().width, CELL_HEIGHT));
		cell->addChild(background, 40, TAG_BACKGROUND);
	}
}