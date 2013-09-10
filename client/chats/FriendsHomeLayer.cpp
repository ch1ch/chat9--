//
//  FriendsHomeLayer.cpp
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#include "FriendsHomeLayer.h"
#include "Main.h"
#include "MessageHandle.h"
#include "ProtoMeta.pb.h"
#include "AppDelegate.h"
#include "AddFriendLayer.h"
#include "CELLMenu.h"
#include "MessageLayer.h"
#include "CommonDataManager.h"
#include "DatabaseManager.h"
#include "FriendModel.h"


FriendsHomeLayer::~FriendsHomeLayer()
{
	_friendArray->release();
}

bool FriendsHomeLayer::init ()
{
    if (!BaseHomeLayer::init())
        return false;
    updateTabbarSelectIndex(1);
    
	_friendArray = DatabaseManager::sharedInstance()->getAllFriend();
	_friendArray->retain();
    
	CCPoint pt = ccpCompMult(ccp(100,100), ccp(.5,2.5));
	printf("%lf %lf", pt.x, pt.y);
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    blackcolor=ccc3(0, 0, 0);
    
    this->setTag(99);
    
    
    
    //顶部层
   thetoplayer=CCLayerColor::create(ccc4(255,165,0,0));
   thetoplayer->setContentSize(CCSize(size.width,size.height*0.1));
    thetoplayer->setPosition(ccp(0, size.height*0.9));
    this->addChild(thetoplayer,80);
    
    CCLabelTTF* topname = CCLabelTTF::create("好友", "Thonburi", 34);
    topname->setPosition( ccp(size.width / 2, size.height *0.04) );
    thetoplayer-> addChild(topname, 10);
    
    //添加好友链接
    addmenu=CCMenuItemImage::create("addfriend.png", "addfriend.png",this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));

    //addmenu=CCMenuItemImage::create("addfriend.png", "addfriend.png",this, menu_selector(FriendsHomeLayer::AddfriendAction));
    addmenu->setPosition(ccp(size.width*0.85,size.width*0.05));
    thetoplayer-> addChild(addmenu,10);
    
	int ownid = CommonDataManager::getMyUserID();
    webSocketHanle->sendMessage(proto::DataPackage_CmdType_LIST_FRIENDS, ownid,1 , 1, "list friend");

    //tableview
	_friendTableView = CCTableView::create(this, CCSizeMake(size.width, size.height*0.823));
    _friendTableView->setDirection(kCCScrollViewDirectionVertical);
    _friendTableView->setPosition(ccp(0,size.height*0.1));
    _friendTableView->setDelegate(this);
    _friendTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(_friendTableView,21);
    _friendTableView->reloadData();
        
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
	return true;
}

void FriendsHomeLayer::onRequestFriendDataReceived(cocos2d::CCArray *friendArray)
{
	_friendArray->release();
	_friendArray = friendArray;
	_friendArray->retain();
	_friendTableView->reloadData();
}

void FriendsHomeLayer::ReplacetoFirendScene(CCObject* pSender){
    
    AddFriendLayer *addfriendLayer = AddFriendLayer::create();
    CCScene *pScene = CCScene::create();
    pScene->addChild(addfriendLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void FriendsHomeLayer::registerWithTouchDispatcher(){
    CCDirector* pDirector=CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 3, true);
}


bool FriendsHomeLayer::ccTouchBegan(CCTouch *touch, CCEvent *pEvent){
   // CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCPoint location=touch->getLocationInView();
    
    
    CCSize s=addmenu->getContentSize();
    CCPoint p=addmenu->getPosition();
    CCPoint ap=addmenu->getAnchorPoint();

    CCRect rect=CCRectMake(p.x-ap.x*s.width, p.y-ap.y*s.height, s.width, s.height);
    
    if (rect.containsPoint(location)) {
       // AddfriendAction();
        ReplacetoFirendScene(this);
        
        return true;
    }else{
        return false;
    }
    
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    
    return true;
}

void FriendsHomeLayer::ccTouchMoved(CCTouch *touch, CCEvent *event){
   // CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint location=touch->getLocationInView();
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint oldlocation=touch->getPreviousLocationInView();
    oldlocation=CCDirector::sharedDirector()->convertToGL(oldlocation);
    


}

void FriendsHomeLayer::ccTouchesEnded(CCSet *ptouches, CCEvent *event){
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
   // CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}

void FriendsHomeLayer::ccTouchesCancelled(CCSet *ptouches, CCEvent *event){
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
   // CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}




void FriendsHomeLayer::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){

    printf("\ntouch me~~at index : %d\n",cell->getIdx());
	int index = cell->getIdx();
	if (index >= 2)
	{
		FriendModel *model = (FriendModel *)_friendArray->objectAtIndex(index - 2);
		MessageLayer *addfriendLayer = MessageLayer::create(atoi(model->friendID.c_str()), false, model->nickName);
		CCScene *pScene = CCScene::create();
		pScene->addChild(addfriendLayer);
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
};

cocos2d::CCSize FriendsHomeLayer::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    switch (idx) {
        case 0:
        {
            return CCSizeMake(size.width, 100);
            break;
        }
        case 1:
        {
            return CCSizeMake(size.width, 90);
            break;
        }
        default:
        {
            return CCSizeMake(size.width, 75);
            break;
        }
    }//end switch

};

cocos2d::extension::CCTableViewCell* FriendsHomeLayer::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    //CCString *string = CCString::createWithFormat("%d", idx);
    CCTableViewCell *cell = table->dequeueCell();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    

    
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildrenWithCleanup(true);
 
    switch (idx) {
        case 0:
        {
            CCMenuItemImage *searchimage = CCMenuItemImage::create("search.png", "search.png", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            //searchimage->setContentSize(CCSize(size.width, size.height*0.5));
            searchimage->setScaleX(size.width/searchimage->getContentSize().width);
            searchimage->setScaleY(40/searchimage->getContentSize().height);
            searchimage->setPosition(ccp(size.width*0.5, 60));
            cell->addChild(searchimage);
            
            CCMenuItemFont *newfriend=CCMenuItemFont::create("新朋友", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            newfriend->setFontName("Thonburi");
            newfriend->setColor(blackcolor);
            newfriend->setPosition(ccp(size.width*0.3, 22));
            newfriend-> setFontSize(13);
            cell->addChild(newfriend);
            
            CCMenuItemImage *grayimage = CCMenuItemImage::create("grayline.png", "grayline.png", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            grayimage->setPosition(ccp(size.width*0.5, 0));
            grayimage->setScaleX(size.width/2);
            grayimage->setScaleY(0.3);
            cell->addChild(grayimage);
            
            break;
        }
        case 1:
        {
            CCMenuItemImage *groupimage = CCMenuItemImage::create("groupico.png", "groupico.png", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            groupimage->setPosition(ccp(size.width*1/7.f, 55));
            cell->addChild(groupimage);
            
            CCMenuItemImage *friendimage = CCMenuItemImage::create("friendline.png", "friendline.png", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            friendimage->setScaleX(size.width/friendimage->getContentSize().width);
            friendimage->setPosition(ccp(size.width*0.5, 10));
            cell->addChild(friendimage);
            
            
            
            CCMenuItemFont *groupname=CCMenuItemFont::create("群", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            groupname->setFontName("Thonburi");
            groupname->setColor(blackcolor);
            groupname->setPosition(ccp(size.width*4/10.f, 57));
            groupname-> setFontSize(16);
            cell ->addChild(groupname);
            break;
        }
        default:
        {
            FriendModel *model = (FriendModel *)_friendArray->objectAtIndex(idx - 2);
            
            
            CCMenuItemImage *friendimage = CCMenuItemImage::create("head1.png", "head1.png", this, NULL);
            friendimage->setPosition(ccp(size.width*1/7.f, 42));
            cell->addChild(friendimage);
            
            CCMenuItemFont *friendname=CCMenuItemFont::create(model->friendID.c_str(), this, NULL);
            friendname->setFontName("Thonburi");
            friendname->setColor(blackcolor);
            friendname->setPosition(ccp(size.width*0.4, 45));
            friendname-> setFontSize(17);
            cell ->addChild(friendname);
            
            CCMenuItemImage *grayimage = CCMenuItemImage::create("grayline.png", "grayline.png", this, menu_selector(FriendsHomeLayer::ReplacetoFirendScene));
            grayimage->setPosition(ccp(size.width*0.5,10));
            grayimage->setScaleX(size.width/2);
            grayimage->setScaleY(0.3);
            cell->addChild(grayimage);
            break;
        }
    }//end switch
    return cell;

    
};

unsigned int FriendsHomeLayer::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return _friendArray->count() + 2;
};


