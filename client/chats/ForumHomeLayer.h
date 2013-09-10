//
//  ForumHomeScene.h
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#ifndef __chats__ForumHomeScene__
#define __chats__ForumHomeScene__

#include <iostream>
#include "BaseHomeLayer.h"
#include "CCScrollView.h"

#define ClassMemu_Height 40

class ForumHomeLayer: public BaseHomeLayer, public cocos2d::extension::CCScrollViewDelegate
{
private:
	cocos2d::extension::CCScrollView *scrollView;
public:
    
	virtual bool init ();

    void loginClickedCallBack(CCObject* pSender);
    void searchClickedCallBack(CCObject* pSender);
    void declareClickedCallBack(CCObject* pSender);

    void hotdeclareClickedCallBack(CCObject* pSender);
    void developClickedCallBack(CCObject* pSender);
    void communicationClickedCallBack(CCObject* pSender);
    void championClickedCallBack(CCObject* pSender);
    
    void addForumContent();
    
public:
    void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    
    CREATE_FUNC (ForumHomeLayer);
};


#endif /* defined(__chats__ForumHomeScene__) */
