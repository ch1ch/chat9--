//
//  ChatListModel.h
//  chats
//
//  Created by 王凤全 on 13-9-10.
//
//

#ifndef __chats__ChatListModel__
#define __chats__ChatListModel__

#include "ModelBase.h"

class ChatListModel : public ModelBase
{
public:
	string localID;
    string chatID;
    string friendID;
    string friendName;
    string type;
    string picture;
    string lastChatTime;
	string lastMessage;
	
	static ChatListModel *create(CCDictionary *rawDic);
	
	static string databaseTableName();
	static string databaseCreateSQL();
	static string getSelectAllSQL();
	
public:
	static string getChatListSQLByFriendID(string friendID);
	virtual string getInsertSQL();
	virtual string getExistSQL();
	virtual string getUpdateSQL();
};

#endif /* defined(__chats__ChatListModel__) */
