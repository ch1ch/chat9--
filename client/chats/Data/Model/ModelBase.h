//
//  ModelBase.h
//  chats
//
//  Created by 王凤全 on 13-9-9.
//
//

#ifndef __chats__ModelBase__
#define __chats__ModelBase__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

typedef enum
{
	eChatTypePersonal = 1,
	eChatTypeGroup
}eChatType;

class ModelBase : public CCObject
{
public:
	static string generateCreateSQL(string tableName, string primaty, CCDictionary *dataDic);
	static string generateInsertSQL(string tableName, CCDictionary *dataDic);
	static bool	addToCCDictionary(CCDictionary *dic, string key, string value);
	
	virtual string getInsertSQL() = 0;
	virtual string getExistSQL() = 0;
	virtual string getUpdateSQL() = 0;
};


#endif /* defined(__chats__ModelBase__) */
