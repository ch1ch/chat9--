//
//  FriendModel.cpp
//  chats
//
//  Created by wangfengquan on 9/8/13.
//
//

#include "FriendModel.h"

#define DATABASE_TABLE		"friend2"

#define FRIEND_LOCALID		"FriLocalID"
#define FRIEND_ID			"FriID"
#define FRIEND_NICKNAME		"NickName"
#define FRIEND_MOBILE		"Mobile"
#define FRIEND_TYPE			"Type"
#define FRIEND_SEX			"Sex"
#define FRIEND_PICTURE		"Picture"
#define FRIEND_LASTMESSAGE	"lastMessage"

FriendModel * FriendModel::create(CCDictionary *rawDic)
{
	FriendModel *model = new FriendModel();
	if (model)
	{
		model->localID		= ((CCString *)rawDic->objectForKey(FRIEND_LOCALID))->m_sString;
		model->friendID		= ((CCString *)rawDic->objectForKey(FRIEND_ID))->m_sString;
		model->nickName		= ((CCString *)rawDic->objectForKey(FRIEND_NICKNAME))->m_sString;
		model->mobile		= ((CCString *)rawDic->objectForKey(FRIEND_MOBILE))->m_sString;
		model->type			= ((CCString *)rawDic->objectForKey(FRIEND_TYPE))->m_sString;
		model->sex			= ((CCString *)rawDic->objectForKey(FRIEND_SEX))->m_sString;
		model->picture		= ((CCString *)rawDic->objectForKey(FRIEND_PICTURE))->m_sString;
		model->lastMessage	= ((CCString *)rawDic->objectForKey(FRIEND_LASTMESSAGE))->m_sString;
		
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

string FriendModel::databaseTableName()
{
	return DATABASE_TABLE;
}

string FriendModel::databaseCreateSQL()
{
	CCDictionary *dataDic = new CCDictionary();
	
	addToCCDictionary(dataDic, FRIEND_ID, "integer");
	addToCCDictionary(dataDic, FRIEND_NICKNAME, "TEXT");
	addToCCDictionary(dataDic, FRIEND_MOBILE, "integer");
	addToCCDictionary(dataDic, FRIEND_TYPE, "integer");
	addToCCDictionary(dataDic, FRIEND_SEX, "integer");
	addToCCDictionary(dataDic, FRIEND_PICTURE, "TEXT");
	addToCCDictionary(dataDic, FRIEND_LASTMESSAGE, "TEXT");
	
	string sql = generateCreateSQL(DATABASE_TABLE, FRIEND_LOCALID, dataDic);
	return sql;
}

string FriendModel::getSelectAllSQL()
{
	return "select * from " + databaseTableName();
}

string FriendModel::getInsertSQL()
{
	CCDictionary *insertDic = new CCDictionary();
	addToCCDictionary(insertDic, FRIEND_LOCALID, this->localID);
	addToCCDictionary(insertDic, FRIEND_ID, this->friendID);
	addToCCDictionary(insertDic, FRIEND_NICKNAME, this->nickName);
	addToCCDictionary(insertDic, FRIEND_MOBILE, this->mobile);
	addToCCDictionary(insertDic, FRIEND_TYPE, this->type);
	addToCCDictionary(insertDic, FRIEND_SEX, this->sex);
	addToCCDictionary(insertDic, FRIEND_PICTURE, this->picture);
	addToCCDictionary(insertDic, FRIEND_LASTMESSAGE, this->lastMessage);
	string sql = FriendModel::generateInsertSQL(DATABASE_TABLE, insertDic);
	insertDic->release();
	
	return sql;
}

string FriendModel::getExistSQL()
{
	char tempSrt[100];
	sprintf(tempSrt, "SELECT * FROM %s WHERE %s=%s", DATABASE_TABLE, FRIEND_ID, this->friendID.c_str());
	
	return string(tempSrt);
}


string FriendModel::getUpdateSQL()
{
	CCDictionary *updateDic = new CCDictionary();
	addToCCDictionary(updateDic, FRIEND_NICKNAME, this->nickName);
	addToCCDictionary(updateDic, FRIEND_MOBILE, this->mobile);
	addToCCDictionary(updateDic, FRIEND_TYPE, this->type);
	addToCCDictionary(updateDic, FRIEND_SEX, this->sex);
	addToCCDictionary(updateDic, FRIEND_PICTURE, this->picture);
	addToCCDictionary(updateDic, FRIEND_LASTMESSAGE, this->lastMessage);
	
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
	sprintf(whereChar, "WHERE %s=%s", FRIEND_ID, this->friendID.c_str());
	
	sql += string(whereChar);
	
	return sql;
}
