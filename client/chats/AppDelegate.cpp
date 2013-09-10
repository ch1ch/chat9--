#include "AppDelegate.h"

#include "cocos2d.h"
#include "Main.h"

#include "MessageHomeLayer.h"
#include "ForumHomeLayer.h"
#include "FriendsHomeLayer.h"
#include "MessageLayer.h"
#include "MessageHomeLayer.h"
#include "SquareHomeLayer.h"
#include "CommonDataManager.h"

USING_NS_CC;

int toID=1;

#define DESIGN_WIDTH		320
#define DESIGN_HEIGHT		480


WebsocketHandle *webSocketHanle= new WebsocketHandle();

AppDelegate::AppDelegate()
{

}


AppDelegate::~AppDelegate()
{
    
}

bool AppDelegate::applicationDidFinishLaunching()
{
	int myID = CommonDataManager::getMyUserID();
	
	webSocketHanle->sendMessage(proto::DataPackage_CmdType_NOTIFY, myID, 1, 1, "nofi");

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);

    pEGLView->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, kResolutionFixedHeight);

    //pEGLView->setFrameSize(320, 480);
	

    //pDirector->setContentScaleFactor(pDirector->getWinSize().width / DESIGN_WIDTH);
    

    // turn on display FPS
   // pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
     //WebsocketHandle *webSocketHanle = new WebsocketHandle(1234);

    // create a scene. it's an autorelease object
    CCScene *pScene = CCScene::create();
    MessageHomeLayer *layer=MessageHomeLayer::create();
//  FriendsHomeLayer *layer=FriendsHomeLayer::create();
//	SquareHomeLayer *layer = SquareHomeLayer::create();
    pScene->addChild(layer);
	pDirector->runWithScene(pScene);
	
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
