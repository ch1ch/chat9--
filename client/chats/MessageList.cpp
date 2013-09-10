//
//  MessageList.cpp
//  chats
//
//  Created by 付剑超 on 13-8-16.
//
//

#include "MessageList.h"

#include "SimpleAudioEngine.h"
#include "MessageHandle.h"
#include "Main.h"
#include "ProtoMeta.pb.h"

#define testflag 2

using namespace cocos2d;
using namespace CocosDenshion;


CCScene* MessageList::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MessageList *layer=MessageList::create();
    
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MessageList::init()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    //////////////////////////////
    // 1. super init first
    //设置整体背景
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255),size.width,size.height*10) )
    {
        return false;
    }

   // handle = new WebsocketHandle(this);
    messagenum=0;
    //obstacle=cocos2d::CCArray::create();
    
    obstacle=new CCArray(1000);
    
    //设置下边聊天部分背景
    messagelayer=cocos2d::CCLayerColor::create(ccc4(255,255,255,255), size.width, size.height*0.9);
    messagelayer->setPosition(0, 0);
    this->addChild(messagelayer,10);
    
    //设置输入框背景
    CCLayerColor *bottomlayer=cocos2d::CCLayerColor::create(ccc4(0, 0, 0, 100),size.width, size.height*0.1);
    bottomlayer->setPosition(0,0);
    this->addChild(bottomlayer,20);
    
    //对话列表层
    listlayer=CCLayerColor::create(ccc4(255, 255, 255, 255), size.width, size.height*3.0);
    listlayer->setPosition(0,size.height*0.1);
    this->addChild(listlayer,15);
    
    //上方对话目标层
    toplayer=CCLayerColor::create(ccc4(232, 166, 0, 255), size.width, size.height*0.1);
    toplayer->setPosition(ccp(0,size.height*0.9));
    this->addChild(toplayer,20);
    
    AddMessageLayer();
    AddinputLayer();
    
    blackcolor=ccc3(0, 0, 0);
    
    //返回按键
    backbutton = CCSprite::create("back.png");
    backbutton->setPosition( ccp(size.width*0.15, size.height*0.05) );
    //toplayer->addChild(backbutton, 10);

    
    sendmenu=CCMenuItemFont::create("接收", this, menu_selector(MessageList::recivetest));
    sendmenu-> setFontSize(32);
    sendmenu->setFontName("Thonburi");
    sendmenu->setColor(ccc3( 255,255,255));
    sendmenu->setPosition(ccp(size.width*0.1,size.height*0.05));
    toplayer ->addChild(sendmenu,35);
    
    
    char db[namelong]=DbName;
    dbFilename=cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+db;
    
    username="lisi";
    
    char message[1000]="随便来一个";
    int type=1;
    
    
    //对方名字
    CCLabelTTF* userName = CCLabelTTF::create(username.c_str(), "Thonburi", 34);
    userName->setPosition( ccp(size.width / 2, size.height *0.05) );
    toplayer->addChild(userName, 20);
    



   // messageDate onemsg=get1Msg(dbFilename.c_str(), username, locID);
    
    //printf("\n%s\t %s\t %s\t %s\t %s\t %s\t %s\t\n",onemsg.MeslocalID,onemsg.MesSvrID,onemsg.createTime,onemsg.message,onemsg.status,onemsg.type,onemsg.Des);
    recemenu=CCMenuItemFont::create("发送", this, menu_selector(MessageList::recivetest));
    recemenu-> setFontSize(16);
    recemenu->setFontName("Thonburi");
    recemenu->setColor(ccc3( 255,255,255));
    recemenu->setPosition(ccp(size.width*0.8,size.height*0.05));
    toplayer ->addChild(recemenu,35);
    
    /*
    messageDate messageDate1=getLastMsg(dbFilename.c_str(), username.c_str());
    
    int msgnum=atoi(messageDate1.MeslocalID);
    CCLog("msgnum=%d",msgnum);
    
    if (msgnum!=0){
        if (msgnum<10) {
            messageDate *messageDatenum;
            messageDatenum=GetnumMsg(dbFilename.c_str(), username.c_str(), msgnum);
            
            
            for (int i=0;i<msgnum;i++){
                MessageList:: AddMsgtolayer(messageDatenum[i]);
                printf("\n%s\t %s\t %s\t %s\t %s\t %s\t %s\t\n",messageDatenum[i].MeslocalID,messageDatenum[i].MesSvrID,messageDatenum[i].createTime,messageDatenum[i].message,messageDatenum[i].status,messageDatenum[i].type,messageDatenum[i].Des);
            }

            delete messageDatenum;
            }
        
        else{*/
            //读取最新10条数据
            messageDate *messageDate10;
            messageDate10=getLast10Msg(dbFilename.c_str(), username.c_str());
            
            
            for (int i=0;i<9;i++){
                MessageList:: AddMsgtolayer(messageDate10[i]);
                printf("\n%s\t %s\t %s\t %s\t %s\t %s\t %s\t\n",messageDate10[i].MeslocalID,messageDate10[i].MesSvrID,messageDate10[i].createTime,messageDate10[i].message,messageDate10[i].status,messageDate10[i].type,messageDate10[i].Des);
                
            }
            MessageList:: AddMsgtolayer(messageDate10[9]);
            delete messageDate10;
        
    
    CCObject* _object;
    CCARRAY_FOREACH(obstacle,_object)
        {
        CCSprite * msgsprite=(CCSprite *)_object;
        CCPoint tempos=msgsprite->getPosition();
        tempos.y-=size.height*0.1;
        CCLog("pos y=%f  to  y=%f",msgsprite->getPosition().y,tempos.y);
        
        msgsprite->setPosition(tempos);
    }

    setTouchEnabled(true);
    return true;
}

void MessageList::recivetest(){
    receiveMessage(dbFilename.c_str(), username.c_str());
    

    
}

void MessageList::AddMessageLayer(){
    
    
}

void MessageList:: AddMsgtolayer(const messageDate messagedate){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCObject* _object;
    CCARRAY_FOREACH(obstacle,_object){
        CCSprite * msgsprite=(CCSprite *)_object;
        CCPoint tempos=msgsprite->getPosition();
        tempos.y+=size.height*0.1;
        
        msgsprite->setPosition(tempos);
    }
    if (atoi(messagedate.Des)==1)
    {
        CCSprite *msghead=CCSprite::create("head.png");
        msghead->setPosition(ccp(size.width*0.9,size.height*0.1));
        //listlayer->addChild(msghead,20);
        obstacle->addObject(msghead);
        
        CCLabelTTF* LocID = CCLabelTTF::create(messagedate.MeslocalID, "Thonburi", 24);
        LocID->setPosition( ccp(size.width*0.5, size.height*0.1)) ;
        LocID->setColor(blackcolor);
        //listlayer->addChild(LocID, 20);
        obstacle->addObject(LocID);
        
        CCLabelTTF* message = CCLabelTTF::create(messagedate.message, "Thonburi", 24);
        message->setPosition( ccp(size.width*0.7, size.height*0.1)) ;
        message->setColor(blackcolor);
        listlayer->addChild(message, 20);
        obstacle->addObject(message);
    }else if (atoi(messagedate.Des)==2)
    {

        CCSprite *msghead=CCSprite::create("head.png");
        msghead->setPosition(ccp(size.width*0.1, size.height*0.1));
        //listlayer->addChild(msghead,20);
        obstacle->addObject(msghead);
            
        CCLabelTTF* LocID = CCLabelTTF::create(messagedate.MeslocalID, "Thonburi", 24);
        LocID->setPosition( ccp(size.width*0.6,size.height*0.1)) ;
        LocID->setColor(blackcolor);
        //listlayer->addChild(LocID, 20);
        obstacle->addObject(LocID);
            
        CCLabelTTF* message = CCLabelTTF::create(messagedate.message, "Thonburi", 24);
        message->setPosition( ccp(size.width*0.3, size.height*0.1)) ;
        message->setColor(blackcolor);
        listlayer->addChild(message, 20);
        obstacle->addObject(message);
    }
    
    //CCPoint listpos=listlayer->getPosition();
    //listpos.y=size.height*(messagenum*0.1-0.9);
    
    //listlayer->setPosition(listpos.x, listpos.y);
}


void MessageList::AddinputLayer(){
    CCSize size = CCDirector::sharedDirector()->getWinSize();

            cocos2d::extension::CCScale9Sprite *sacel9SprG=cocos2d::extension::CCScale9Sprite::create("whitebg.png");
            box2=cocos2d::extension::CCEditBox::create(CCSizeMake(size.width*0.9,size.height*0.09), sacel9SprG);
            box2->setPlaceHolder("输入聊天内容");
            box2->setPosition(ccp(size.width*0.5,size.height*0.05));
            box2->setReturnType(cocos2d::extension::kKeyboardReturnTypeSend);
            box2->setColor(ccc3(0,0,0));
            addChild(box2,50);
            //获取编辑框内的文字
            //printf("text=== %s ===\n",box2->getText());
            CCLOG("Text:===%s====",box2->getText());
            box2->setDelegate(this);
    
}

void MessageList::registerWithTouchDispatcher(){
    CCDirector* pDirector=CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 3, true);
    //pDirector->getTouchDispatcher()->addTargetedDelegate(listlayer, 3, false);
    //pDirector->getTouchDispatcher()->addTargetedDelegate(toplayer, 5, false);
    
}


bool MessageList::ccTouchBegan(CCTouch *touch, CCEvent *pEvent){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint listpos=listlayer->getPosition();
    CCPoint location=touch->getLocationInView();
    
    
    CCSize s=recemenu->getContentSize();
    CCPoint p=recemenu->getPosition();
    CCPoint ap=recemenu->getAnchorPoint();
    //CCLog("\n~~size x= %f,y=%f,point x=%f,y=%f\n",s.width,s.height,p.x,p.y);
    CCRect rect=CCRectMake(p.x-ap.x*s.width, p.y-ap.y*s.height, s.width, s.height);
    
    if (rect.containsPoint(location)) {
        receiveMessage(dbFilename.c_str(), username.c_str());
        messageDate onemsg=getLastMsg(dbFilename.c_str(), username.c_str());
        AddMsgtolayer(onemsg);
        

       //handle->sendMessage("");

        return true;
    }

    
    location=CCDirector::sharedDirector()->convertToGL(location);
    
   
    if (listpos.y<size.height*0.15) {
        isControl=1;
           }else isControl=0;
    
    return true;
}

void MessageList::ccTouchMoved(CCTouch *touch, CCEvent *event){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint location=touch->getLocationInView();
    location=CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint oldlocation=touch->getPreviousLocationInView();
    oldlocation=CCDirector::sharedDirector()->convertToGL(oldlocation);
    
    CCPoint listpos=listlayer->getPosition();

    if (isControl) {
        
        float x=listpos.x;       
        float y=listpos.y+ location.y-oldlocation.y;
        
        if (y>size.height*0.11) {
            y=listpos.y;
        }
        int uplim=obstacle->count();

        
         printf("list pos y= %f  <   %f     up=%d\n",y,(size.height*0.8-size.height*0.1*uplim/3),uplim);
                
        if (y<(size.height*0.8-size.height*0.1*uplim/3)) {
            y=size.height*0.8-size.height*0.1*uplim/3;
        }
        
        listlayer->setPosition(x, y);
    }    
}

void MessageList::ccTouchesEnded(CCSet *ptouches, CCEvent *event){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}

void MessageList::ccTouchesCancelled(CCSet *ptouches, CCEvent *event){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCTouch *touch=dynamic_cast<CCTouch*>(ptouches->anyObject());
}


void MessageList::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox)
{
    CCLOG("start edit");
}
void MessageList::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox)
{

    int type=1;
    
    
   // handle->sendMessage(box2->getText());
    
    
    box2->setText("");
    messageDate onemsg=getLastMsg(dbFilename.c_str(), username.c_str());
    
    
    
    AddMsgtolayer(onemsg);
    

}
void MessageList::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text)
{
    CCLOG("textchanged");
}
void MessageList::editBoxReturn(cocos2d::extension::CCEditBox *editBox)
{
    CCLOG("editboxreturn");
}


void MessageList::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}




