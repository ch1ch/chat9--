//
//  LoadingPage.cpp
//  chats
//
//  Created by 付剑超 on 13-8-16.
//
//

#include "LoadingPage.h"
#include "Main.h"



using namespace cocos2d;

LoadingPage:: LoadingPage(){
    
}
LoadingPage::~LoadingPage(){
    
}

void LoadingPage:: sceneWithTargetScene(const TargetScenes targetScene)
{

    printf("replaceScene to %d\n",targetScene);
    targetScene_=targetScene;
    
    LoadingPage::initWithTargetScene(targetScene);

    
}


void LoadingPage:: initWithTargetScene(const TargetScenes targetScene)
{

    targetScene_ = targetScene;
   this-> schedule (schedule_selector(LoadingPage::Chatupdate)) ;
   
    switch (targetScene_) {
        case TargetScene_Message:
        {

            //MessageList *thelayer=MessageList::create();
            
            //CCScene *theScene=CCScene::create();
           // theScene->addChild(thelayer,120);
            
          //  CCDirector::sharedDirector()->replaceScene(theScene);
            break;
        }
        case TargetScene_Forum:
        {
            cocos2d::CCSprite *backbutton = cocos2d::CCSprite::create("back.png");
            cocos2d::CCSize size(100,100);
            backbutton->setPosition(size);
            addChild(backbutton, 10);
//            cocos2d::CCSize size(100,100);
//            ForumLayer *forumlayer = ForumLayer::create(size);
//            addChild(forumlayer,110);
        }
        default:
            //CCDirector::sharedDirector()->replaceScene(MessageList::scene());
            break;
    }

    
}

void LoadingPage::Chatupdate(float dt){
       printf("========chup========"); 
}



