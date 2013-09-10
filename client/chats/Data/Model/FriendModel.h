//
//  FriendModel.h
//  chats
//
//  Created by wangfengquan on 9/8/13.
//
//

#ifndef __chats__FriendModel__
#define __chats__FriendModel__

#include "ModelBase.h"

typedef enum
{
	eGenderTypeMale = 1,
	eGenderTypeFemale
}eGenderType;

class FriendModel : public ModelBase
{
public:
	string localID;
    string friendID;
    string nickName;
    string mobile;
    string type;
    string sex;
    string picture;
	string lastMessage;
		
	static FriendModel *create(CCDictionary *rawDic);

	static string databaseTableName();
	static string databaseCreateSQL();
	static string getSelectAllSQL();
	
	virtual string getInsertSQL();
	virtual string getExistSQL();
	virtual string getUpdateSQL();
};

#endif /* defined(__chats__FriendModel__) */
