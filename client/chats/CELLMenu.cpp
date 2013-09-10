//
//  CELLMenu.cpp
//  chats
//
//  Created by 付剑超 on 13-8-30.
//
//

#include "CELLMenu.h"


void CELLMenu::registerWithTouchDispatcher()
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    
    //重新设置优先级，原来的优先级是-128，CCLayer的优先级是0，我们设置它的优先级为128
    pDispatcher->addTargetedDelegate(this, 10, true);
}

CELLMenu* CELLMenu::create(CCRect rect)
{
    CELLMenu *m = new CELLMenu();
    m->_rect = rect;
    
    if(m && m->init())
    {
        m->autorelease();
        return m;
    }
    
    CC_SAFE_DELETE(m);
    return NULL;
}

bool CELLMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    
    
    //设置滑动为false，初始化
    _isScroll = false;
    
    //获取点击的点，判断是否在我们想要它所在的区域
    CCPoint pt = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    if(!_rect.containsPoint(pt))
    {
        return false;
    }
    
    if (m_eState != kCCMenuStateWaiting || ! m_bVisible || ! m_bEnabled)
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    
    return false;
     
    /*
    if(visibleRect_.size.width&&visibleRect_.size.height){
                if(!visibleRect_.containsPoint(touch->getLocation()))
                         return false;
            }
         setPosition = touch->getLocation();
         return CCMenu::ccTouchBegan(touch,event);
     */
}

void CELLMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    //如果滚动了，那么就设为true，并不做任何操作
    _isScroll = true;
    return;
}

void CELLMenu::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    m_pSelectedItem->unselected();
    
    //判断如果滑动了，就不再触发目的事件了
    if (m_pSelectedItem && !_isScroll)
    {
        m_pSelectedItem->activate();
    }
    m_eState = kCCMenuStateWaiting;
}