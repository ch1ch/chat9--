//
//  MessageHomeScene.cpp
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#include "MessageHomeLayer.h"
#include "Main.h"

#include "ProtoMeta.pb.h"
#include "cocos2d.h"
#include "MessageLayer.h"
#include "GroupUserListLayer.h"
#include "WebSocketHandle.h"
#include "DatabaseManager.h"

using namespace cocos2d;

MessageHomeLayer::~MessageHomeLayer()
{
	_chatListArray->release();
}

bool MessageHomeLayer::init ()
{
    if (!BaseHomeLayer::init())
        return false;
    updateTabbarSelectIndex(0);
    
    
    blackcolor=ccc3(0, 0, 0);
    this->setTag(77);

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    //顶部层
    thetoplayer=CCLayerColor::create(ccc4(255,165,0,0));
    thetoplayer->setContentSize(CCSize(size.width,size.height*0.155));
    thetoplayer->setPosition(ccp(0, size.height*0.9));
    this->addChild(thetoplayer,80);
    
    CCLabelTTF* topname = CCLabelTTF::create("聊天", "Thonburi", 34);
    topname->setPosition( ccp(size.width / 2, size.height *0.04) );
    thetoplayer-> addChild(topname, 10);
    
    groupChatMenuItem = CCMenuItemImage::create("addchat.png", "addchat.png",this, menu_selector( MessageHomeLayer::onGroupMessageClicked));
	groupChatMenuItem->setPosition(CCPointMake(size.width*0.85, size.height*0.04));
	CCMenu *groupMenu = CCMenu::create(groupChatMenuItem, NULL);
	//groupMenu->setContentSize(CCSizeMake(100, 50));
	groupMenu->setPosition(CCPointMake(0, 0));
	thetoplayer-> addChild(groupMenu, 40);
    
    
	_chatListArray = DatabaseManager::sharedInstance()->getAllChatList();
	_chatListArray->retain();
	
    //tableview
    tableView = CCTableView::create(this, CCSizeMake(size.width, size.height*0.823));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(0,size.height*0.1));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,21);
    tableView->reloadData();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
    
	return true;
}

void MessageHomeLayer::testtest(){
    
}



void MessageHomeLayer::registerWithTouchDispatcher(){
    CCDirector* pDirector=CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 3, true);
    
    
}


bool MessageHomeLayer::ccTouchBegan(CCTouch *touch, CCEvent *pEvent){
   // CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint location=touch->getLocationInView();
    
    
    CCSize s=groupChatMenuItem->getContentSize();
    CCPoint p=groupChatMenuItem->getPosition();
    CCPoint ap=groupChatMenuItem->getAnchorPoint();
    
    CCRect rect=CCRectMake(p.x-ap.x*s.width, p.y-ap.y*s.height, s.width, s.height);
    
    if (rect.containsPoint(location)) {
       // AddfriendAction();
        // ReplacetoFirendScene(this);
        
        return true;
    }else{
        return false;
    }
    
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    
    return true;
}


void MessageHomeLayer::ccTouchMoved(CCTouch *touch, CCEvent *event){
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint location=touch->getLocationInView();
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint oldlocation=touch->getPreviousLocationInView();
    oldlocation=CCDirector::sharedDirector()->convertToGL(oldlocation);
    
    
    
}

void MessageHomeLayer::ccTouchesEnded(CCSet *ptouches, CCEvent *event){
 //   CCSize size = CCDirector::sharedDirector()->getWinSize();
  //  CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}

void MessageHomeLayer::ccTouchesCancelled(CCSet *ptouches, CCEvent *event){
   // CCSize size = CCDirector::sharedDirector()->getWinSize();
   // CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}


void MessageHomeLayer::onGroupMessageClicked()
{
	GroupUserListLayer *nextLayer = GroupUserListLayer::create();
	nextLayer->setTag(LAYER_TAG_GROUPUSER);
    CCScene *pScene = CCScene::create();
    pScene->addChild(nextLayer);
    
    CCDirector::sharedDirector()->replaceScene(pScene);
}




void MessageHomeLayer::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{
    
    printf("\ntouch me~~at index : %d\n",cell->getIdx());
	int index = cell->getIdx();
	if (index > 0)
	{
		ChatListModel *model = (ChatListModel *)_chatListArray->objectAtIndex(index - 1);
		
		int targetID = atoi(model->friendID.c_str());
		bool isgroup = atoi(model->type.c_str()) == eChatTypeGroup;
		
		
		MessageLayer *msgLayer = MessageLayer::create(targetID, isgroup, "");
		CCScene *pScene = CCScene::create();
		pScene->addChild(msgLayer);
		
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
};

cocos2d::CCSize MessageHomeLayer::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    switch (idx) {
        case 0:
        {
            return CCSizeMake(size.width, 100);
            break;
        }
        default:
        {
            return CCSizeMake(size.width, 70);
            break;
        }
    }//end switch
    
};

cocos2d::extension::CCTableViewCell* MessageHomeLayer::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    CCSize size = CCDirector::sharedDirector()->getWinSize();


    
    if (!cell)
	{
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);

    switch (idx) {
        case 0:
        {
            CCMenuItemImage *groupimage = CCMenuItemImage::create("groupico.png", "groupico.png", this, menu_selector(MessageHomeLayer::testtest));
            groupimage->setPosition(ccp(size.width*0.12, 45));
            cell->addChild(groupimage);
            
            
            CCMenuItemFont *groupname=CCMenuItemFont::create("提醒", this, menu_selector(MessageHomeLayer::testtest));
            groupname->setFontName("Thonburi");
            groupname->setColor(blackcolor);
            groupname->setPosition(ccp(size.width*4/10.f, 45));
            groupname-> setFontSize(14);
            cell ->addChild(groupname);
            
            CCMenuItemImage *grayimage = CCMenuItemImage::create("grayline.png", "grayline.png", this, menu_selector(MessageHomeLayer::testtest));
            grayimage->setPosition(ccp(size.width*0.5, 10));
            grayimage->setScaleX(size.width/2);
            grayimage->setScaleY(0.3);
            cell->addChild(grayimage);
            break;
        }
            
        default:
        {
            
			ChatListModel *model = (ChatListModel *)_chatListArray->objectAtIndex(idx - 1);
			
            CCMenuItemImage *chatimage = CCMenuItemImage::create("head1.png", "head1.png", this, NULL);
            chatimage->setPosition(ccp(size.width*0.12, 45));
            cell->addChild(chatimage);
            
            CCMenuItemFont *chatname=CCMenuItemFont::create(model->friendID.c_str(), this, NULL);
            chatname->setFontName("Thonburi");
            chatname->setColor(blackcolor);
            chatname->setAnchorPoint(CCPointZero);
            chatname->setPosition(ccp(size.width*0.25, 50));
            chatname-> setFontSize(17);
            cell ->addChild(chatname);
            
            
            std::string strmsg=model->lastMessage.c_str();
            int lastmsgcutlong=strlength(strmsg);
            char *thelastmsg;
            strncpy(thelastmsg, model->lastMessage.c_str(),lastmsgcutlong);
            thelastmsg[lastmsgcutlong]='\0';


            CCMenuItemFont *lastmsg=CCMenuItemFont::create(thelastmsg, this, NULL);
            lastmsg->setFontName("Thonburi");
            lastmsg->setColor(blackcolor);
            lastmsg->setAnchorPoint(ccp(0, 0));
            lastmsg->setPosition(ccp(size.width*0.22, 25));
            lastmsg-> setFontSize(17);
            cell ->addChild(lastmsg);
            
            
            CCMenuItemImage *grayimage = CCMenuItemImage::create("grayline.png", "grayline.png", this, NULL);
            grayimage->setPosition(ccp(size.width*0.5,10));
            grayimage->setScaleX(size.width/2);
            grayimage->setScaleY(0.3);
            cell->addChild(grayimage);
            
            struct tm chattimetm;
            time_t timeint=atoi(model->lastChatTime.c_str());
            chattimetm=*localtime(&timeint);
            //printf("\n%d-%d-%d\n",1900+chattimetm.tm_year,1+chattimetm.tm_mon,chattimetm.tm_mday);
            
            char chattime[15];
            sprintf(chattime, "%d-%d %d:%d",1+chattimetm.tm_mon,chattimetm.tm_mday,chattimetm.tm_hour,chattimetm.tm_min);
            
            CCLabelTTF *monthLabel = CCLabelTTF::create(chattime, "Thonburi-Bold", 15);
            monthLabel->setColor(blackcolor);
            monthLabel->setPosition(ccp(size.width*0.83, 60));
            cell->addChild(monthLabel, 6);
            
            
            
            /*
             printf("\n%s\n",frienddate[idx-2].FriID);
             
             CCSprite *sprite = CCSprite::create("head2.png");
             sprite->setAnchorPoint(CCPointZero);
             sprite->setPosition(ccp(0, 0));
             cell->addChild(sprite);
             
             CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
             label->setPosition(CCPointZero);
             label->setAnchorPoint(CCPointZero);
             label->setTag(123);
             cell->addChild(label);
             */
            break;
        }
    }//end switch
    
    
    return cell;
    
    
};

unsigned int MessageHomeLayer::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return _chatListArray->count() + 1;
};


int MessageHomeLayer::strlength(const std::string &str){
    int len=21;
    if (str.length()<len) len=str.length();
    //printf("\nthe %s len is %d\n",str.c_str(),len);
    //len = str.length();
    std::vector<std::string>dump;
    int i = 0;
    while(i < len) {
        if (~(str.at(i) >> 8) == 0) {
            dump.push_back(str.substr(i, 3));
            dump.push_back(str.substr(i, 3));
            dump.push_back(str.substr(i, 3));
            i = i + 3;
        } else {
            dump.push_back(str.substr(i, 1));
            i = i + 1;
        }
    }
     //printf("\nthe %s size is %d\n",str.c_str(),dump.size());
    return dump.size();
}




