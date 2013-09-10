//
//  ModelBase.cpp
//  chats
//
//  Created by 王凤全 on 13-9-9.
//
//

#include "ModelBase.h"

string ModelBase::generateCreateSQL(string tableName, string primaty, cocos2d::CCDictionary *dataDic)
{
	char temp[100];
	sprintf(temp, "CREATE TABLE %s ( %s integer primary key autoincrement ", tableName.c_str(), primaty.c_str());
	
	string sql = string(temp);
		
	CCArray *keyArray = dataDic->allKeys();
	for (int i = 0; i < keyArray->count(); i ++)
	{
		string key = ((CCString *)keyArray->objectAtIndex(i))->m_sString;
		string value = ((CCString *)dataDic->objectForKey(key))->m_sString;
		sql += ", " + key + " " + value;
	}
	sql += ");";
	
	return sql;
}

string ModelBase::generateInsertSQL(string tableName, CCDictionary *dataDic)
{
	string pre = "INSERT INTO " + tableName + " ";
	CCArray *keyArray = dataDic->allKeys();
	if (keyArray->count() == 0) return NULL;
	
	string columnKey = ((CCString *)keyArray->objectAtIndex(0))->m_sString;
	
	string columns = "(" + columnKey;
	string values = "('" + ((CCString *)dataDic->objectForKey(columnKey))->m_sString + "'";
	for (int i = 1; i < keyArray->count(); i ++)
	{
		columnKey = ((CCString *)keyArray->objectAtIndex(i))->m_sString;
		columns += "," + columnKey;
		values += ",'" + ((CCString *)dataDic->objectForKey(columnKey))->m_sString + "'";
	}
	columns += ")";
	values += ")";
	
	string sql = "INSERT INTO " + tableName + " " + columns + " VALUES " + values + ";";
	
	return sql;
}

bool ModelBase::addToCCDictionary(CCDictionary *dic, string key, string value)
{
	if (key.length() == 0 || value.length() == 0)
	{
		return false;
	}
	
	CCString *str = new CCString(value);
	dic->setObject(str, key);
	str->release();
	
	return true;
}