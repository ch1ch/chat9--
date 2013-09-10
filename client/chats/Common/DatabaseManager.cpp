//
//  DatabaseManager.cpp
//  chats
//
//  Created by wangfengquan on 9/8/13.
//
//

#include "DatabaseManager.h"
#include "CommonDataManager.h"
#include "sqlite3DB.h"
#include "FriendModel.h"
#include "ChatListModel.h"

static DatabaseManager *sharedDatanaseManager = NULL;


DatabaseManager * DatabaseManager::sharedInstance(void)
{
	if (!sharedDatanaseManager)
	{
		sharedDatanaseManager = new DatabaseManager();
		sharedDatanaseManager->init();
	}
	
	return sharedDatanaseManager;
}

bool DatabaseManager::init()
{
	int userID = CommonDataManager::getMyUserID();
    char fileChar[100];
	sprintf(fileChar, "db%d.db", userID);
	string databaseName = CCFileUtils::sharedFileUtils()->getWritablePath() + fileChar;
	SQLiteUtil::updateDatabaseName(databaseName);
	createDatabaseTablesIfNotExist();

	return true;
}

void DatabaseManager::createDatabaseTablesIfNotExist()
{
	bool tableExist = false;

	string friendTableName = FriendModel::databaseTableName();
	tableExist = SQLiteUtil::executeUpdate("select * from " + friendTableName);
	if (!tableExist)
	{
		string sql = FriendModel::databaseCreateSQL();
		SQLiteUtil::executeUpdate(sql);
	}
	
	string chatlistTableName = ChatListModel::databaseTableName();
	tableExist = SQLiteUtil::executeUpdate("select * from " + chatlistTableName);
	if (!tableExist)
	{
		string sql = ChatListModel::databaseCreateSQL();
		SQLiteUtil::executeUpdate(sql);
	}
}

CCArray * DatabaseManager::getAllFriend()
{
	string tableName = FriendModel::databaseTableName();
	string sql = "select * from " + tableName + ";";
	CCArray *rawArray = SQLiteUtil::executeQuery(sql);
	
	CCArray *friendArray = new CCArray();
	
	for (int i = 0; i < rawArray->count(); i ++)
	{
		CCDictionary *dataDic = (CCDictionary *)rawArray->objectAtIndex(i);
		FriendModel *model =  FriendModel::create(dataDic);
		friendArray->addObject(model);
	}
	
	friendArray->autorelease();
	return friendArray;
}

bool DatabaseManager::updateAllFriend(CCArray *friendArray)
{
	string tableName = FriendModel::databaseTableName();
	string deleteSQL = "delete from "+ tableName + ";";
	SQLiteUtil::executeUpdate(deleteSQL);
	for (int i = 0; i < friendArray->count(); i ++)
	{
		FriendModel *model = (FriendModel *)friendArray->objectAtIndex(i);
		string insertSQL = model->getInsertSQL();
		SQLiteUtil::executeUpdate(insertSQL);
	}
	
	return true;
}

bool DatabaseManager::addFriends(CCArray *friendArray)
{
	for (int i = 0; i < friendArray->count(); i ++)
	{
		FriendModel *model = (FriendModel *)friendArray->objectAtIndex(i);
		CCArray *resultArray = SQLiteUtil::executeQuery(model->getExistSQL());
		string sql;
		if (resultArray->count())
		{
			sql = model->getUpdateSQL();
		}
		else
		{
			sql = model->getInsertSQL();
		}
		SQLiteUtil::executeUpdate(sql);
	}
	
	return true;
}

CCArray * DatabaseManager::getAllChatList()
{
	string sql =ChatListModel::getSelectAllSQL();
	CCArray *rawArray = SQLiteUtil::executeQuery(sql);
	
	CCArray *chatListArray = new CCArray();
	
	for (int i = 0; i < rawArray->count(); i ++)
	{
		CCDictionary *dataDic = (CCDictionary *)rawArray->objectAtIndex(i);
		ChatListModel *model =  ChatListModel::create(dataDic);
		chatListArray->addObject(model);
	}
	
	chatListArray->autorelease();
	return chatListArray;
}

ChatListModel * DatabaseManager::getChatByFriendID(string friendID)
{
	string tableName = ChatListModel::databaseTableName();
	string sql = ChatListModel::getChatListSQLByFriendID(friendID);
	CCArray *resultArray = SQLiteUtil::executeQuery(sql);
	
	if (resultArray->count())
	{
		CCDictionary *dataDic = (CCDictionary *)resultArray->objectAtIndex(0);
		ChatListModel *model = ChatListModel::create(dataDic);
		return model;
	}
	else
	{
		return NULL;
	}
}

bool DatabaseManager::addToChatList(string message, string targetID, eChatType messageType)
{
	string sql = ChatListModel::getChatListSQLByFriendID(targetID);
	CCArray *resultArray = SQLiteUtil::executeQuery(sql);
	if (resultArray->count())
	{
		CCDictionary *dataDic = (CCDictionary *)resultArray->objectAtIndex(0);
		ChatListModel *model = ChatListModel::create(dataDic);
		char timeString[20];
		sprintf(timeString, "%ld", time(NULL));
		model->lastMessage	= message;
		model->lastChatTime = timeString;
		sql = model->getUpdateSQL();
		SQLiteUtil::executeUpdate(sql);
	}
	else
	{
		char timeString[20], typeString[10];
		sprintf(timeString, "%ld", time(NULL));
		sprintf(typeString, "%d", messageType);
				
		ChatListModel *model = new ChatListModel();
		model->chatID		= "10";
		model->friendID		= targetID;
		model->type			= string(typeString);
		model->picture		= "head3.png";
		model->lastChatTime = string(timeString);
		model->lastMessage	= message;
		sql = model->getInsertSQL();
		SQLiteUtil::executeUpdate(sql);
	}
	
	return true;
}

