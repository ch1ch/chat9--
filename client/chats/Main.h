//
//  Main.h
//  chats
//
//  Created by 付剑超 on 13-8-12.
//
//


#ifndef DbName
#define DbName "db1.db"
#endif

#ifndef namelong
#define namelong 100
#endif

#ifndef messagecolumn
#define messagecolumn 8
#endif

#ifndef chatcolumn
#define chatcolumn 8
#endif

#ifndef __chats__Main__
#define __chats__Main__


#include "cocos2d.h"



#define Memu_Height 60
#define Memu_Width 80



class Main : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    

    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    void messageClickedCallBack(CCObject* pSender);
    void friendsClickedCallBack(CCObject* pSender);
    void forumClickedCallBack(CCObject* pSender);
    void squareClickedCallBack(CCObject* pSender);
    

    CREATE_FUNC(Main);
};


#endif 




