//
//  MessageLayer.cpp
//  chats
//
//  Created by 付剑超 on 13-8-30.
//
//

#include "MessageLayer.h"
#include "Main.h"
#include "WebSocketHandle.h"
#include "ProtoMeta.pb.h"
#include "Main.h"
#include "MessageHomeLayer.h"
#include "strutil.h"
#include "GroupUserListLayer.h"
#include "DatabaseManager.h"
#include "CommonDataManager.h"


MessageLayer * MessageLayer::create(int targetID, bool isGroup, std::string targetName)
{
	MessageLayer *msgLayer = new MessageLayer();
	if (msgLayer && msgLayer->init(targetID, isGroup, targetName))
	{
		msgLayer->autorelease();
		return msgLayer;
	}
	else
	{
		delete msgLayer;
		msgLayer = NULL;
		return NULL;
	}
}

bool MessageLayer::init(int targetID, bool isGroup, std::string targetName)
{
	if (!CCLayer::init())
	{
		return false;
	}
    
	int ownid = CommonDataManager::getMyUserID();
    char db[namelong];
    sprintf(db, "db%d.db",ownid);
    dbFilename=cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+db;
    this->setTag(88);
    
	_targetID = targetID;
	_isGroup = isGroup;
	_targetName = targetName;
    toID=_targetID;
    
    blackcolor=ccc3(0, 0, 0);

	CCSize viewSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //char db[namelong]=DbName;

    
    
    sprintf(userid, "%d",targetID);
    obstacle=new CCArray(1000);
    listHeigth=size.height*0.2;
	
    /*
	CCLayerColor *bannerColor = CCLayerColor::create(ccc4(255,165,0,255));
	bannerColor->setContentSize(CCSize(viewSize.width, 50));
	bannerColor->setPosition(CCPointMake(0, viewSize.height - 50));
	addChild(bannerColor, 2);

	std::string tempStirng = "ID:" + google::protobuf::SimpleItoa(targetID);
	CCLabelTTF *idLabel = CCLabelTTF::create(tempStirng.c_str(), "Thonburi-Bold", 18);
	idLabel->setColor(ccc3(255, 0, 0));
	idLabel->setPosition(CCPointMake(160, 150));
	addChild(idLabel,12);
	
	CCLabelTTF *groupLabel = CCLabelTTF::create(_isGroup? "Group Message" : "Personal Message", "Thonburi-Bold", 18);
	groupLabel->setColor(ccc3(255, 0, 0));
	groupLabel->setPosition(CCPointMake(160, 250));
	addChild(groupLabel,12);

	CCLabelTTF *nameLabel = CCLabelTTF::create(("name:" + targetName).c_str(), "Thonburi-Bold", 18);
	nameLabel->setColor(ccc3(255, 0, 0));
	nameLabel->setPosition(CCPointMake(160, 350));
	addChild(nameLabel,12);

	CCMenuItem *tempBackBtn = CCMenuItemFont::create("临时返回按钮", this, menu_selector(MessageLayer::backToGroupList));
	tempBackBtn->setPosition(CCPointMake(50, 50 / 2));
	CCMenu *tempBackMenu = CCMenu::create(tempBackBtn, NULL);
	tempBackMenu->setPosition(CCPointMake(0, viewSize.height - 50));
	addChild(tempBackMenu, 4);
     */

    
    //背景颜色
    CCLayerColor *background = CCLayerColor::create(ccc4(247,222,166,255));
    addChild(background,1);
    
    
    //上部分
    
    toplayer=CCLayerColor::create(ccc4(255,165,0,0));
    toplayer->setContentSize(CCSize(size.width,size.height*0.1));
    toplayer->setPosition(ccp(0, size.height*0.9));
    this->addChild(toplayer,24);
    
    CCSprite *topbg=CCSprite::create("top-bg.png");
    topbg->setPosition(CCSize(viewSize.width*1/2.f,Memu_Height*1/2.f));
    topbg->setScaleX(viewSize.width/topbg->getContentSize().width);
    topbg->setScaleY(Memu_Height/topbg->getContentSize().height);
    toplayer->addChild(topbg,20);
    
    
    
    
    FriendDate onefriend=GetOneFriendfromDB(dbFilename.c_str(), toID);
    
    if (onefriend.FriID==NULL||onefriend.FriID==0) {
        
        AddFriendtoDB(dbFilename.c_str(), toID);
    }

    //onefriend=GetOneFriendfromDB(dbFilename.c_str(), toID);
    
    
    CCLabelTTF* topname = CCLabelTTF::create(userid, "Thonburi", 34);
    topname->setPosition( ccp(size.width / 2, size.height *0.05) );
    toplayer->addChild(topname, 22);
    
    backbutton=CCSprite::create("back.png");
    backbutton->setPosition(ccp(size.width*0.14, size.height *0.05));
    toplayer->addChild(backbutton,22);
    
    CCLabelTTF* backttf = CCLabelTTF::create("返回", "Thonburi", 20);
    backttf->setPosition( ccp(size.width*0.15, size.height *0.05) );
    toplayer->addChild(backttf, 22);
    
    exitgroupbuttom=CCLabelTTF::create("退出群", "Thonburi", 20);

    if (_isGroup) {
    exitgroupbuttom->setPosition(ccp(size.width*0.8,size.height*0.05));
    toplayer->addChild(exitgroupbuttom,22);
    }
    
    //滚动框
    int heigth=size.height;
    scroll = cocos2d::extension::CCScrollView::create(CCSizeMake(size.width, size.height));
    layer = CCLayer::create();
    scroll->setContainer(layer);
    layer->retain();
    layer->setContentSize(CCSizeMake(size.width, heigth));
    scroll->setContentSize(CCSizeMake(size.width, heigth));
    scroll->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
    scroll->setPosition(0, 0);
    this->addChild(scroll,20,99);
    
    //CCScene*  test = CCDirector::sharedDirector()->getRunningScene();
    
    AddinputLayer();
    
    GetMsgList();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
	return true;
}



bool MessageLayer::init ()
{
	if (!CCLayer::init())
        return false;
   
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //char db[namelong]=DbName;
	int ownid = CommonDataManager::getMyUserID();
    char db[namelong];
    sprintf(db, "db%d.db",ownid);
    dbFilename=cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+db;
    
    sprintf(userid, "%d",toID);
    // printf("%s",userid);
    obstacle=new CCArray(1000);
    listHeigth=size.height*0.2;
    
    
    //背景颜色
    CCLayerColor *background = CCLayerColor::create(ccc4(255,255,255,255));
    addChild(background,1);
    
    
    
    //上部分
    toplayer=CCLayerColor::create(ccc4(255,165,0,255));
    toplayer->setContentSize(CCSize(size.width,size.height*0.1));
    toplayer->setPosition(ccp(0, size.height*0.9));
    this->addChild(toplayer,24);

    FriendDate onefriend=GetOneFriendfromDB(dbFilename.c_str(), toID);
    if (onefriend.FriID==NULL||onefriend.FriID==0) {
        
        AddFriendtoDB(dbFilename.c_str(), toID);
    }
    //AddFriendtoDB(dbFilename.c_str(), toID);
    onefriend=GetOneFriendfromDB(dbFilename.c_str(), toID);
    
    CCLabelTTF* topname = CCLabelTTF::create(userid, "Thonburi", 34);
    topname->setPosition( ccp(size.width / 2, size.height *0.05) );
    toplayer->addChild(topname, 20);
    
    backbutton=CCSprite::create("back.png");
    backbutton->setPosition(ccp(size.width*0.14, size.height *0.05));
    toplayer->addChild(backbutton,20);
    
    
    

    //滚动框
    int heigth=size.height;
    scroll = cocos2d::extension::CCScrollView::create(CCSizeMake(size.width, size.height));
    layer = CCLayer::create();
    scroll->setContainer(layer);
    layer->setContentSize(CCSizeMake(size.width, heigth));
    scroll->setContentSize(CCSizeMake(size.width, heigth));
    scroll->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
    scroll->setPosition(0, 0);
    this->addChild(scroll,20,99);

    //CCScene*  test = CCDirector::sharedDirector()->getRunningScene();
    
    AddinputLayer();
    
    GetMsgList();
    
CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);

	return true;
}

void MessageLayer::backToGroupList()
{
	GroupUserListLayer *groupLayer = GroupUserListLayer::create();
	groupLayer->setTag(LAYER_TAG_GROUPUSER);
	CCScene *nextScene = CCScene::create();
	nextScene->addChild(groupLayer);
	
	CCDirector::sharedDirector()->replaceScene(nextScene);
	int ownid = CommonDataManager::getMyUserID();
	webSocketHanle->sendMessage(proto::DataPackage_CmdType_LIST_FRIENDS, ownid, 0 , 0, NULL);
}

void MessageLayer::GetMsgList(){
    
    //messageDate onemesg;
    
    int msgnum=GetmsgCount(dbFilename.c_str(), toID);
    if (msgnum>20)msgnum=20;
    
    if (msgnum>0) {
    
    char **re=getsomeMsg(dbFilename.c_str(), toID, msgnum);
    
    messageDate *messagedate=new messageDate[msgnum+1];
    
    for (int i=0,j=msgnum;i<(messagecolumn*msgnum+1);j--){
        
        
        messagedate[j].MeslocalID=re[i++];
        messagedate[j].MesSvrID=re[i++];
        messagedate[j].createTime=re[i++];
        messagedate[j].message=re[i++];
        messagedate[j].status=re[i++];
        messagedate[j].type=re[i++];
        messagedate[j].Des=re[i++];
        messagedate[j].fromid=re[i++];
        
        
        
    }
        
       // delete re;
    for (int i=0; i<msgnum; i++) {

        AddMsgtolayer(messagedate[i]);
    }
    }
    
}

void MessageLayer::AddinputLayer(){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::extension::CCScale9Sprite *sacel9SprG=cocos2d::extension::CCScale9Sprite::create("whitebg.png");
    textbox=cocos2d::extension::CCEditBox::create(CCSizeMake(size.width*0.9,size.height*0.1), sacel9SprG);
    textbox->setPlaceHolder("输入聊天内容");
    textbox->setPosition(ccp(size.width*0.5,size.height*0.065));
    textbox->setReturnType(cocos2d::extension::kKeyboardReturnTypeSend);
    textbox->setMaxLength(100);
    textbox->setColor(ccc3(0,0,0));
    addChild(textbox,50);
    //获取编辑框内的文字
    //printf("text=== %s ===\n",box2->getText());
    //CCLOG("Text:===%s====",textbox->getText());
    //
    textbox->setDelegate(this);
    
}


void MessageLayer:: AddMsgtolayer(const messageDate messagedate){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
     //FriendDate onefriend=GetOneFriendfromDB(dbFilename.c_str(), toID);
    
    CCObject* _object;
    
    if(obstacle!=NULL){
        
        int i=obstacle->count();
        
        if (i>=0)
        {

            CCARRAY_FOREACH(obstacle,_object){
                CCSprite * msgsprite=(CCSprite *)_object;
                CCPoint tempos=msgsprite->getPosition();
                tempos.y+=size.height*0.14;
                msgsprite->setPosition(tempos);
            }
        }

        CCSprite *msghead=NULL;
        
        std::string strmsg=messagedate.message;
        
        int strlen=strlength(strmsg);
        
        int strfontlinenum=12;
        
        int strline=strlen/strfontlinenum;
        if (strlen%strfontlinenum!=0) strline++;
        int fontheigh=strline*30;
        int fontweiht=strfontlinenum*13;
        if (1==strline) fontweiht=strlen*14;
        
        int bglen=strlen*size.width*0.05;
        if(strline>1) bglen=strfontlinenum*size.width*0.04+size.width*0.05;
        int bghigh=strline*size.width*0.1;

        if (atoi(messagedate.Des)==1)
        {
            msghead=cocos2d::CCSprite::create();
            msghead->initWithFile("head.png");
            msghead->setPosition(ccp(size.width*0.9,size.height*0.2));
            layer->addChild(msghead,20);
            obstacle->addObject(msghead);
            
            //CCLabelTTF* LocID = CCLabelTTF::create(messagedate.MeslocalID, "Thonburi", 24);
           // LocID->setPosition( ccp(size.width*0.5, size.height*0.2)) ;
           // LocID->setColor(blackcolor);
            //layer->addChild(LocID, 20);
            //obstacle->addObject(LocID);
            
            printf("the msg is %s strlen is %d the line is %d  hei is %d   wei is %d\n",messagedate.message,strlen,strline,fontheigh,fontweiht);
            
            CCLabelTTF* message = CCLabelTTF::create(messagedate.message, "Thonburi", 24, CCSizeMake(fontweiht, fontheigh), kCCTextAlignmentLeft);
            //CCLabelTTF* message = CCLabelTTF::create(messagedate.message, "Thonburi", 24);
            message->setPosition( ccp(size.width*0.79, size.height*0.25)) ;
            message->setAnchorPoint(ccp(1,1));
            message->setColor(blackcolor);
            layer->addChild(message, 20);
            obstacle->addObject(message);
            
            /*
            int msglen;//=strlen(messagedate.message);
            
            for(msglen=0;messagedate.message[msglen]!='\0';msglen++);
            
            size_t len=strlen(messagedate.message)+1;
            
            wchar_t *Wstr;
            
            Wstr=(wchar_t*)malloc(len*sizeof(wchar_t));
            
            mbstowcs(Wstr, messagedate.message, len);
             */
            
            
           // printf("\nthe %s '%s' len=%d   strlen=%d\n",messagedate.MeslocalID,messagedate.message,strlen);


            cocos2d::extension::CCScale9Sprite* labBg1 = cocos2d::extension::CCScale9Sprite::create("msg_send_bg.png");
            labBg1->setAnchorPoint(ccp(1,1));
            labBg1->setPreferredSize(CCSizeMake(bglen, bghigh));
            labBg1->setPosition(ccp(size.width*0.79, size.height*0.25));
            layer->addChild(labBg1);
            obstacle->addObject(labBg1);
            upmsgline=strline;

        }else if (atoi(messagedate.Des)==2)
        {
           
            msghead=CCSprite::create("head.png");
            msghead->setPosition(ccp(size.width*0.1,size.height*0.2));
            layer->addChild(msghead,20);
            obstacle->addObject(msghead);
            
            CCLabelTTF* message = CCLabelTTF::create(messagedate.message, "Thonburi", 24, CCSizeMake(fontweiht, fontheigh), kCCTextAlignmentLeft);
            //CCLabelTTF* message = CCLabelTTF::create(messagedate.message, "Thonburi", 24);
            message->setPosition( ccp(size.width*0.22, size.height*0.25)) ;
            message->setAnchorPoint(ccp(0,1));
            message->setColor(blackcolor);
            layer->addChild(message, 20);
            obstacle->addObject(message);
            
            cocos2d::extension::CCScale9Sprite* labBg1 = cocos2d::extension::CCScale9Sprite::create("msg_rece_bg.png");
            labBg1->setAnchorPoint(ccp(0,1));
            labBg1->setPreferredSize(CCSizeMake(bglen, bghigh));
            labBg1->setPosition(ccp(size.width*0.21, size.height*0.25));
            layer->addChild(labBg1);
            obstacle->addObject(labBg1);
            upmsgline=strline;

        }
        
        int uphigh=0;
        
        if (upmsgline>1)
        {
            
            uphigh=upmsgline*size.height*0.02+size.height*0.02;
            
            CCARRAY_FOREACH(obstacle,_object){
                CCSprite * msgsprite=(CCSprite *)_object;
                CCPoint tempos=msgsprite->getPosition();
                
                tempos.y+=(uphigh);
                
                msgsprite->setPosition(tempos);
            }
            upmsgline=1;
        }
    
    }
}

void MessageLayer::ReplacetoFirendScene(CCObject* pSender){
    
    MessageHomeLayer *chatLayer = MessageHomeLayer::create();
    
    CCScene *pScene = CCScene::create();
    
    pScene->addChild(chatLayer);
    
    
    CCDirector::sharedDirector()->replaceScene(pScene);

    
}

int MessageLayer::strlength(const std::string &str){
    int len = str.length();
    std::vector<std::string>dump;
    int i = 0;
    while(i < len) {
        if (~(str.at(i) >> 8) == 0) {
            dump.push_back(str.substr(i, 3));
            dump.push_back(str.substr(i, 3));
            i = i + 3;
        } else {
            dump.push_back(str.substr(i, 1));
            i = i + 1;
        }
    }
    return dump.size();
}


void MessageLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox)
{
    CCLOG("start edit");
}
void MessageLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox)
{
    int type=1;
    const char *tem=textbox->getText();
    
	const char *message = textbox->getText();
	
    int len=strlen(tem);
	char targetID[20];
	sprintf(targetID, "%d", toID);
    
    if (len!=0)
	{
		int ownid = CommonDataManager::getMyUserID();
        
        if (_isGroup) {
            SavesendMsgtoDB(dbFilename.c_str(), toID, textbox->getText(), type,ownid);
            
			DatabaseManager::sharedInstance()->addToChatList(string(message), string(targetID), eChatTypeGroup);
            
            webSocketHanle->sendMessage(proto::DataPackage_CmdType_SEND, toID, proto::DataPackage_Message_MessageType_GROUP, 1, textbox->getText());
            
        }
        else{
        
            //单聊处理，处理
            SavesendMsgtoDB(dbFilename.c_str(), toID, textbox->getText(), type,ownid);
			
			char targetString[50];
			sprintf(targetString, "%d", toID);
			
			DatabaseManager::sharedInstance()->addToChatList(string(textbox->getText()), string(targetString), eChatTypePersonal);
			
            webSocketHanle->sendMessage(proto::DataPackage_CmdType_SEND, toID, proto::DataPackage_Message_MessageType_PERSON_TO_PERSON, 1, textbox->getText());
            
        }
    textbox->setText("");

   
    //显示到层
    char ** re=get1lastMsg(dbFilename.c_str(), toID);
    messageDate messagedate;
    
    for (int i=messagecolumn;i<messagecolumn*2;){
        
        messagedate.MeslocalID=re[i++];
        messagedate.MesSvrID=re[i++];
        messagedate.createTime=re[i++];
        messagedate.message=re[i++];
        messagedate.status=re[i++];
        messagedate.type=re[i++];
        messagedate.Des=re[i++];
        messagedate.fromid= re[i++];
        break;
    }
    
    AddMsgtolayer(messagedate);


    }
    
}

void MessageLayer::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text)
{
    CCLOG("textchanged");
}
void MessageLayer::editBoxReturn(cocos2d::extension::CCEditBox *editBox)
{
    CCLOG("editboxreturn");
}



void MessageLayer::registerWithTouchDispatcher(){
    CCDirector* pDirector=CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 3, true);
    
    
}


bool MessageLayer::ccTouchBegan(CCTouch *touch, CCEvent *pEvent){
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint location=touch->getLocationInView();
    
    
    CCSize s=backbutton->getContentSize();
    CCPoint p=backbutton->getPosition();
    CCPoint ap=backbutton->getAnchorPoint();
    
    CCRect rect=CCRectMake(p.x-ap.x*s.width, p.y-ap.y*s.height, s.width, s.height);
    //返回
    if (rect.containsPoint(location)) {
        // AddfriendAction();
        ReplacetoFirendScene(this);
        
        return true;
    }
    
    if (_isGroup){
        //退出群
        s=exitgroupbuttom->getContentSize();
        p=exitgroupbuttom->getPosition();
        ap=exitgroupbuttom->getAnchorPoint();
        rect=CCRectMake(p.x-ap.x*s.width, p.y-ap.y*s.height, s.width, s.height);
        if (rect.containsPoint(location)) {
            // AddfriendAction();
            exitgroup();
            return true;
        }else{
            return false;
        }
    }else
        return false;
    
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    
    return true;
}


void MessageLayer::ccTouchMoved(CCTouch *touch, CCEvent *event){
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint location=touch->getLocationInView();
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint oldlocation=touch->getPreviousLocationInView();
    oldlocation=CCDirector::sharedDirector()->convertToGL(oldlocation);
    
    
    
}

void MessageLayer::ccTouchesEnded(CCSet *ptouches, CCEvent *event){
   // CCSize size = CCDirector::sharedDirector()->getWinSize();
    //CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}

void MessageLayer::ccTouchesCancelled(CCSet *ptouches, CCEvent *event){
   // CCSize size = CCDirector::sharedDirector()->getWinSize();
   // CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}

MessageLayer::MessageLayer(){
   
    
}

MessageLayer::~MessageLayer(){
    
}

std::string MessageLayer::coverstringtoview(std::string str, int length){
    unsigned int before = 0;
    std::vector<std::string > subStrs;
    do
    {
        subStrs.push_back(str.substr(before,length));
        if (before+length >str.size())
        {
            break;
        }
        else
        {
            before = before+length;
        }
        
    } while (true);
    std::string resultStr;
    for (unsigned int i = 0;i<subStrs.size();++i)
    {
        resultStr.append(subStrs[i]).append("\n");
    }
    //resultStr.pop_back();
    //resultStr.pop_back();
    return resultStr;
    
}

void MessageLayer::exitgroup(){
    printf("\nexit group~\n");
}
