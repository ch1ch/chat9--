//
//  GroupUserListLayer.h
//  chats
//
//  Created by 王凤全 on 13-9-3.
//
//

#ifndef __chats__GroupUserListLayer__
#define __chats__GroupUserListLayer__

#include <iostream>
#include "cocos2d.h"
#include "MessageHandle.h"
#include "CCTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

#endif /* defined(__chats__GroupUserListLayer__) */


class GroupUserListLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
private:
	CCArray *_friendArray;
	
	bool *_selectStatus;
	CCTableView *_tableView;
		
public:
	virtual bool init();
	CREATE_FUNC (GroupUserListLayer);
	virtual ~GroupUserListLayer();
	
	void onCancelClicked();
	void onSubmitClicked();	
	void onFriendUserListReceived(CCArray *dataArray);
	void onGroupCreateReceived(int groupID, std::string groupName);
	
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	virtual void tableCellTouched(CCTableView *table, CCTableViewCell *cell);
	
	virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
};
