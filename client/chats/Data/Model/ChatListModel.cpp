//
//  ChatListModel.cpp
//  chats
//
//  Created by 王凤全 on 13-9-10.
//
//

#include "ChatListModel.h"

#define DATABASE_TABLE		"chatlist2"

#define CHAT_LOCALID		"ChatLocalID"
#define CHAT_ID				"ChatID"
#define CHAT_FRIENDID		"friID"
#define CHAT_NICKNAME		"NickName"
#define CHAT_TYPE			"Type"
#define CHAT_PICTURE		"Picture"
#define CHAT_LASTTIME		"LastChatTime"
#define CHAT_LASTMESSAGE	"LastMsg"

ChatListModel * ChatListModel::create(CCDictionary *rawDic)
{
	ChatListModel *model = new ChatListModel();
	if (model)
	{
		model->localID		= ((CCString *)rawDic->objectForKey(CHAT_LOCALID))->m_sString;
		model->chatID		= ((CCString *)rawDic->objectForKey(CHAT_ID))->m_sString;
		model->friendID		= ((CCString *)rawDic->objectForKey(CHAT_FRIENDID))->m_sString;
		model->friendName	= ((CCString *)rawDic->objectForKey(CHAT_NICKNAME))->m_sString;
		model->type			= ((CCString *)rawDic->objectForKey(CHAT_TYPE))->m_sString;
		model->picture		= ((CCString *)rawDic->objectForKey(CHAT_PICTURE))->m_sString;
		model->lastChatTime	= ((CCString *)rawDic->objectForKey(CHAT_LASTTIME))->m_sString;
		model->lastMessage	= ((CCString *)rawDic->objectForKey(CHAT_LASTMESSAGE))->m_sString;
		
		model->autorelease();
		return model;
	}
	else
	{
		delete model;
		model = NULL;
		return NULL;
	}
}

string ChatListModel::databaseTableName()
{
	return DATABASE_TABLE;
}

string ChatListModel::databaseCreateSQL()
{
	CCDictionary *dataDic = new CCDictionary();
	
	addToCCDictionary(dataDic, CHAT_ID, "integer");
	addToCCDictionary(dataDic, CHAT_FRIENDID, "integer");
	addToCCDictionary(dataDic, CHAT_NICKNAME, "TEXT");
	addToCCDictionary(dataDic, CHAT_TYPE, "integer");
	addToCCDictionary(dataDic, CHAT_PICTURE, "TEXT");
	addToCCDictionary(dataDic, CHAT_LASTTIME, "integer");
	addToCCDictionary(dataDic, CHAT_LASTMESSAGE, "TEXT");
	
	string sql = generateCreateSQL(DATABASE_TABLE, CHAT_LOCALID, dataDic);
	return sql;
}

string ChatListModel::getSelectAllSQL()
{
	char temp[100];
	sprintf(temp, "SELECT * FROM %s ORDER BY %s DESC", DATABASE_TABLE, CHAT_LASTTIME);

	return string(temp);
}

string ChatListModel::getChatListSQLByFriendID(string friendID)
{
	char tempSrt[100];
	sprintf(tempSrt, "SELECT * FROM %s WHERE %s=%s", DATABASE_TABLE, CHAT_FRIENDID, friendID.c_str());
	
	return string(tempSrt);
}



string ChatListModel::getInsertSQL()
{
	CCDictionary *insertDic = new CCDictionary();
	addToCCDictionary(insertDic, CHAT_LOCALID, this->localID);
	addToCCDictionary(insertDic, CHAT_ID, this->chatID);
	addToCCDictionary(insertDic, CHAT_FRIENDID, this->friendID);
	addToCCDictionary(insertDic, CHAT_NICKNAME, this->friendName);
	addToCCDictionary(insertDic, CHAT_TYPE, this->type);
	addToCCDictionary(insertDic, CHAT_PICTURE, this->picture);
	addToCCDictionary(insertDic, CHAT_LASTTIME, this->lastChatTime);
	addToCCDictionary(insertDic, CHAT_LASTMESSAGE, this->lastMessage);
	string sql = ChatListModel::generateInsertSQL(DATABASE_TABLE, insertDic);
	insertDic->release();
	
	return sql;
}

string ChatListModel::getExistSQL()
{
	char tempSrt[100];
	sprintf(tempSrt, "SELECT * FROM %s WHERE %s=%s", DATABASE_TABLE, CHAT_FRIENDID, this->friendID.c_str());
	
	return string(tempSrt);
}


string ChatListModel::getUpdateSQL()
{
	CCDictionary *updateDic = new CCDictionary();
	addToCCDictionary(updateDic, CHAT_FRIENDID, this->friendID);
	addToCCDictionary(updateDic, CHAT_NICKNAME, this->friendName);
	addToCCDictionary(updateDic, CHAT_TYPE, this->type);
	addToCCDictionary(updateDic, CHAT_PICTURE, this->picture);
	addToCCDictionary(updateDic, CHAT_LASTTIME, this->lastChatTime);
	addToCCDictionary(updateDic, CHAT_LASTMESSAGE, this->lastMessage);
	
	if (updateDic->allKeys()->count() == 0) return NULL;
	
	char temp[100];
	sprintf(temp, "UPDATE %s SET ", DATABASE_TABLE);
	string sql = string(temp);
	
	CCArray *allKeys = updateDic->allKeys();
	string keyValue = ((CCString *)allKeys->objectAtIndex(0))->m_sString;
	string value = ((CCString *)updateDic->objectForKey(keyValue))->m_sString;
	sql += keyValue + "='" + value + "' ";
	for (int i = 1; i < allKeys->count(); i ++)
	{
		string keyValue = ((CCString *)allKeys->objectAtIndex(i))->m_sString;
		string value = ((CCString *)updateDic->objectForKey(keyValue))->m_sString;
		sql +=  ", " + keyValue + "='" + value + "' ";
	}
	
	char whereChar[100];
	sprintf(whereChar, "WHERE %s=%s", CHAT_FRIENDID, this->friendID.c_str());
	
	sql += string(whereChar);
	
	return sql;
}
