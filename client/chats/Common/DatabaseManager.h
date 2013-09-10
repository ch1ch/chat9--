//
//  DatabaseManager.h
//  chats
//
//  Created by wangfengquan on 9/8/13.
//
//

#ifndef __chats__DatabaseManager__
#define __chats__DatabaseManager__

#include <iostream>
#include "cocos2d.h"
#include "MessageHandle.h"
#include "ChatListModel.h"

USING_NS_CC;
using namespace std;

class DatabaseManager
{
private:
	string _databaseName;
	
public:
	static DatabaseManager *sharedInstance();
	bool init();
	void createDatabaseTablesIfNotExist();
	
	//user
	CCArray *getAllFriend();
	bool updateAllFriend(CCArray *friendArray);
	bool addFriends(CCArray *friendArray);
	
	//chatList
	CCArray *getAllChatList();
	ChatListModel * getChatByFriendID(string friendID);
	bool addToChatList(string message, string targetID, eChatType messageType);
};


#endif /* defined(__chats__DatabaseManager__) */
