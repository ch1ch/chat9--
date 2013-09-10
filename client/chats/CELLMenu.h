//
//  CELLMenu.h
//  chats
//
//  Created by 付剑超 on 13-8-30.
//
//

#ifndef __chats__CELLMenu__
#define __chats__CELLMenu__

#include <iostream>
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class CELLMenu : public CCMenu
{
private:
    //设置一个点击区域，用于判断点击是否在这个区域内
    CCRect _rect;
    
    //是否滑动了MenuItem
    bool _isScroll;
    
public:
    //重写registerWithTouchDispatcher函数，设置优先级
    virtual void registerWithTouchDispatcher();
    
    //create方法，传进来的是可以响应点击的区域
    static CELLMenu* create(CCRect rect);
    
    //重写三个touch函数
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void ccTouchEnded(CCTouch *touch, CCEvent* event);
    
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
    
    
    
};

#endif /* defined(__Test__CELLMenu__) */



