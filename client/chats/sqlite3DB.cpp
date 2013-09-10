//
//  sqlite3DB.cpp
//  chats
//
//  Created by 付剑超 on 13-8-12.
//
//

#include "sqlite3DB.h"

#include "MessageBase.h"

static string databaseName;


SQLiteUtil::SQLiteUtil(){
    
}


SQLiteUtil::~SQLiteUtil(){
    
}

void SQLiteUtil::updateDatabaseName(string dbName)
{
	databaseName = dbName;
}

bool SQLiteUtil::createTableIfNotExist(string tableName, string sql)
{
	sqlite3 *pDB = NULL;
	char *errMsg = NULL;
	int result;
	
	result = sqlite3_open(databaseName.c_str(), &pDB);
	if (result != SQLITE_OK)
		cout<<"open database failed:"<<databaseName<<"\n";

	string checkSQL = sqlite3_mprintf("select * from %s", tableName.c_str());
	result = sqlite3_exec(pDB, checkSQL.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			cout<<"create table failed:" << tableName <<"\n";
	}
	sqlite3_close(pDB);
	
	return result == SQLITE_OK;
}

bool SQLiteUtil::executeUpdate(string sql)
{
	sqlite3 *pDB = NULL;
	char * errMsg = NULL;
	int result;
	
	result = sqlite3_open(databaseName.c_str(), &pDB);
	if (result != SQLITE_OK)
	{
		cout << "open database failed:" << databaseName << "\n";
		
		return false;
	}
	
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	sqlite3_close(pDB);
	
	return result == SQLITE_OK;
}

CCArray * SQLiteUtil::executeQuery(string sql)
{
	sqlite3 *pDB = NULL;
	char * errMsg = NULL;
	int result;
	
	result = sqlite3_open(databaseName.c_str(), &pDB);
	if (result != SQLITE_OK)
	{
		cout << "open database failed:" << databaseName << "\n";
		
		return false;
	}
	
	int row, column;
	char **resultData;
	result = sqlite3_get_table(pDB, sql.c_str(), &resultData, &row, &column, &errMsg);
	
	CCArray *keyArray = new CCArray();
	for (int i = 0; i < column; i ++)
	{
		CCString *str = new CCString(resultData[i]);
		keyArray->addObject(str);
	}

	CCArray *resultArray = new CCArray();
	for (int i = 1; i <= row; i ++)
	{
		CCDictionary *dataDic = new CCDictionary();
		
		for (int j = 0; j < column; j ++)
		{
			printf("%s", resultData[i * column + j]);
			char * tempChar = resultData[i * column + j];
			if (tempChar == NULL)
			{
				tempChar = "";
			}

			CCString *tempStr = new CCString(tempChar);
			string keyString = ((CCString *)keyArray->objectAtIndex(j))->m_sString;
			dataDic->setObject(tempStr, keyString);
			
			tempStr->release();
		}
		
		resultArray->addObject(dataDic);
	}
	
	keyArray->release();
	resultArray->autorelease();
	return resultArray;
}

//bool SQLiteUtil::createTableIfNotExist(std::string sql)
//{
//	sqlite3 *pDB = NULL;
//	char *errMsg = NULL;
//	int result;
//	
//	result = sqlite3_open(<#const char *filename#>, <#sqlite3 **ppDb#>)
//}

sqlite3 *  SQLiteUtil::PrepareMsgDB(const char *dbFilename,const int username){
    sqlite3 *pdb=NULL;
    char * errMsg=NULL;
    
    
    std::string sql;
    int result;
    result=sqlite3_open(dbFilename,&pdb);
    if(result!=SQLITE_OK)
        printf("open database  chat_%d on DB %s failed,  number%d",username,dbFilename,result);
    
    //sql=sqlite3_mprintf("drop table chat_%s",username);
    //result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&errMsg);
    
    sql=sqlite3_mprintf("select * from chat_%d",username);
    
    result=sqlite3_exec( pdb, sql.c_str() ,NULL, NULL, &errMsg );
    
    
    
    if(result!=SQLITE_OK)
    {
        sql=sqlite3_mprintf("create table chat_%d ( MesLocalID integer primary key autoincrement,MesSvrID integer,createTime integer,message text,status integer,type integer,Des integer,Fromid integer)",username);
    
        result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&errMsg);

        if(result!=SQLITE_OK)
            printf("create table failed ,错误原因:%s",errMsg);
    }
    

    
    return pdb;
 

}


int SQLiteUtil::AddMessage(const char *dbFilename,const int username,const int SvrID,const int creatTime,const char *message,const int status,const int type,const int Des,const int fromID){
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    std::string sql;
    int result;
    char * errMsg=NULL;
    

     if(pDB!=NULL){
         sql=sqlite3_mprintf("insert into chat_%d(MesSvrID,createTime,message,status,type,Des,Fromid)  values(%d,%d,'%q',%d,%d,%d,%d)",username,SvrID,creatTime,message,status,type,Des,fromID);
         printf("\n%s\n",sql.c_str());
         result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
         if(result!=SQLITE_OK)
             printf("\ninsert data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
     }
    char **re;
    int r,c;
    sql=sqlite3_mprintf("select MeslocalID from  chat_%d order by MeslocalID DESC limit 1",username,SvrID,creatTime,message,status,type,Des);
    //result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
    sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
    
    //if(result!=SQLITE_OK)
       // printf("set  failed!错误原因:%s..sql:%s\n",errMsg,sql.c_str());
    

    
    sqlite3_close(pDB);
    return atoi(re[1]);
}
bool SQLiteUtil::DelMessage(const char *dbFilename,const int username,const int MesLocalID){
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    std::string sql;
    int result;
    char * errMsg=NULL;
    if(pDB!=NULL){
        sql=sqlite3_mprintf("delete from chat_%d where  MesLocalID = %d",username,MesLocalID);
        result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        if(result!=SQLITE_OK)
        {
            printf("delete data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
            sqlite3_close(pDB);

            return false;
        }
        else{
            sqlite3_close(pDB);

            return true;
        }
    }
    else {

        return false;
    }
    
}




char** SQLiteUtil::GetlastMsg(const char *dbFilename,const int username,const int msgnum){
    

    std::string sql;
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from chat_%d order by MeslocalID DESC limit %d",username,msgnum);
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        sqlite3_close(pDB);

        
        return re;
    }
    else {

        return 0;
    }

    
}

int loadUserRecord(void *para,int n_column,char**column_value,char**column_name){
    
   // printf("id=%s,svrid=%s,creattime=%s,message=%s,status=%s,type=%s,des=%s\n",column_value[0],column_value[1],column_value[2],column_value[3],column_value[4],column_value[5],column_value[6],column_value[7]);
    

    return 0;
}

void SQLiteUtil::ReadFromSQLite(const char *dbFilename,const int username){
    
    std::string sql;
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    char *errMsg;
    if(pDB!=NULL){

        sql=sqlite3_mprintf("select * from chat_%d",username);
        
        sqlite3_exec( pDB, sql.c_str() , loadUserRecord, NULL, &errMsg );
        
        if(errMsg!=NULL){
            printf("exec sql  witherror log:%s",errMsg);
            sqlite3_close(pDB);

        }
        
    }

    sqlite3_close(pDB);
}


char**  SQLiteUtil::MessageallGet(const char *dbFilename,const int username){
    std::string sql;
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from chat_%d",username);
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        sqlite3_close(pDB);

        
        return re;
    }
    else {

        return 0;
    }
    
}
int SQLiteUtil::MessageallRow(const char *dbFilename,const int  username){
    std::string sql;
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    char *errMsg;
    if(pDB!=NULL){
        char **re;
        int r,c;
        sql=sqlite3_mprintf("select * from chat_%d",username);
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        //printf("row is %d ,column is %d ",r,c);
        sqlite3_close(pDB);

        return r;
    }else {
        sqlite3_close(pDB);

        return 0;
    }
    
}
bool SQLiteUtil::UpdateMesdb(const char *dbFilename,const int username,const int mesLocalID,const int SvrID,const int creatTime,const char *message,const int status,const int type,const int Des){
    std::string sql;
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    char *errMsg;
    if(pDB!=NULL){
        sql=sqlite3_mprintf("update chat_%d set MesSvrID=%d,createTime=%d,message=%q,status=%d,type=%d,Des=%d where MesLocalID = %d",username,SvrID,creatTime,message,status,type,Des,mesLocalID);
        char result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        
        if(result!=SQLITE_OK)
        {
            printf("update data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
            sqlite3_close(pDB);

            return false;
        }
        else{
            sqlite3_close(pDB);

            return true;
        }

    }else
        return false;
    
}



char ** SQLiteUtil::GetOnemsg(const char *dbFilename,const int username,const int mesLocalID){
    std::string sql;
    sqlite3* pDB=PrepareMsgDB(dbFilename,username);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from chat_%d where  MesLocalID = %d",username,mesLocalID);
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get id=%d data failed!错误原因:%s..sql:%s",mesLocalID,errMsg,sql.c_str());
        sqlite3_close(pDB);

        return re;
    }
    else{
        sqlite3_close(pDB);

        return 0;
    }
    
}

sqlite3 *  SQLiteUtil::PrepareFriDB(const char *dbFilename){
    sqlite3 *pdb=NULL;
    char * errMsg=NULL;
    
    
    std::string sql;
    int result;
    result=sqlite3_open(dbFilename,&pdb);
    if(result!=SQLITE_OK)
        printf("open database  friend on DB %s failed,  number%d",dbFilename,result);
    
    //sql=sqlite3_mprintf("drop table friend");
    //result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&errMsg);

    
    sql=sqlite3_mprintf("select * from friend");
    
    result=sqlite3_exec( pdb, sql.c_str() ,NULL, NULL, &errMsg );

    
    if(result!=SQLITE_OK)
    {

        
        sql=sqlite3_mprintf("create table friend ( FriLocalID integer primary key autoincrement,FriID integer,NickName TEXT,Mobile integer,Type integer,Sex integer,Picture TEXT,lastMessage TEXT)");
        
        result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&errMsg);
        
        if(result!=SQLITE_OK)
            printf("create table friend failed ,错误原因:%s",errMsg);
    }
    
    return pdb;
    
}


char ** SQLiteUtil::GetOneFri(const char *dbFilename,const int FriID){
    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from friend where  FriID = %d",FriID);
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get friid=%d data failed!错误原因:%s..sql:%s",FriID,errMsg,sql.c_str());
        sqlite3_close(pDB);
        
        return re;
    }
    else{
        sqlite3_close(pDB);
        
        return 0;
    }
    
}


char ** SQLiteUtil::GetOneFrilocal(const char *dbFilename,const int FrilocalID){
    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from friend where  FriLocalID = %d",FrilocalID);
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get friid=%d data failed!错误原因:%s..sql:%s",FrilocalID,errMsg,sql.c_str());
        sqlite3_close(pDB);
        
        return re;
    }
    else{
        sqlite3_close(pDB);
        
        return 0;
    }
    
}

char **SQLiteUtil::GetAllfri(const char *dbFilename){
    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from friend ");
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get friend table data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
        sqlite3_close(pDB);
        
        return re;
    }
    else{
        sqlite3_close(pDB);
        
        return 0;
    }
    
}

int SQLiteUtil::GetFriRow(const char *dbFilename){
    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        char **re;
        int r,c;
        sql=sqlite3_mprintf("select * from friend");
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        //printf("row is %d ,column is %d ",r,c);
        sqlite3_close(pDB);

        return r;
    }else {
        sqlite3_close(pDB);

        return 0;
    }
}

int SQLiteUtil::GetFrifindRow(const char *dbFilename,const int FriID){
    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from friend where  FriID = %d",FriID);
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get friid=%d data failed!错误原因:%s..sql:%s",FriID,errMsg,sql.c_str());
        sqlite3_close(pDB);
        
        return r;
    }
    else{
        sqlite3_close(pDB);
        
        return 0;
    }
}



bool SQLiteUtil::AddFri(const char *dbFilename,const int FriID){
    sqlite3* pDB=PrepareFriDB(dbFilename);
    std::string sql;
    int result;
    char * errMsg=NULL;
    

    if(pDB!=NULL){
        sql=sqlite3_mprintf("insert into friend(FriID) values(%d)",FriID);
        
        result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        
        printf("\naddfri %s\n",sql.c_str());
        if(result!=SQLITE_OK)
            printf("insert data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
    }
    
    sqlite3_close(pDB);
    return true;
}

bool SQLiteUtil::DelFri(const char *dbFilename,const int FriID){
    sqlite3* pDB=PrepareFriDB(dbFilename);
    std::string sql;
    int result;
    char * errMsg=NULL;
    if(pDB!=NULL){
        sql=sqlite3_mprintf("delete from friend where  FriID = %d",FriID);
        result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        if(result!=SQLITE_OK)
        {
            printf("delete data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
            sqlite3_close(pDB);
            
            return false;
        }
        else{
            sqlite3_close(pDB);
            
            return true;
        }
    }
    else {
        return false;
    }
}

bool SQLiteUtil::UpdateFri(const char *dbFilename,const int friID,const char *nickName,const int mobile,const int type,const int sex,const char *picture){

    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        sql=sqlite3_mprintf("update friend set NickName='%q',Mobile=%d,Type=%d,Sex=%d,Picture='%q' where FriID=%d",nickName,mobile,type,sex,picture,friID);
        
        //printf("\nupdate friend~ %s\n",sql.c_str());
        char result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        
        if(result!=SQLITE_OK)
        {
            printf("\nupdate fri data failed!错误原因:%s..sql:%s======\n",errMsg,sql.c_str());
            sqlite3_close(pDB);

            return false;
        }
        else{
            printf("update data ok!..sql:%s======\n",sql.c_str());
            sqlite3_close(pDB);

            return true;
        }
        
    }else
        return false;
    
}


bool  SQLiteUtil::DeleteTable(const char *dbFilename,const char *tablename){
    std::string sql;
    sqlite3* pDB=PrepareFriDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        sql=sqlite3_mprintf("drop table %s",tablename);
        char result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        if(result!=SQLITE_OK)
        {
            printf("update data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
            sqlite3_close(pDB);

            return false;
        }
        else{
            sqlite3_close(pDB);

            return false;
        }
    }
    else return false;
    
}


sqlite3 *SQLiteUtil::PrepareChatDB(const char *dbFilename){
    
    sqlite3 *pdb=NULL;
    char * errMsg=NULL;
    
    
    std::string sql;
    int result;
    result=sqlite3_open(dbFilename,&pdb);
    if(result!=SQLITE_OK)
        printf("open database  chatlist  DB %s failed,  number%d",dbFilename,result);
    
    //sql=sqlite3_mprintf("drop table chatlist");
    //result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&errMsg);
    
    
    sql=sqlite3_mprintf("select * from chatlist");
    
    result=sqlite3_exec( pdb, sql.c_str() ,NULL, NULL, &errMsg );
    
    
    if(result!=SQLITE_OK)
    {
        
        
        sql=sqlite3_mprintf("create table chatlist(ChatLocalID integer primary key autoincrement,ChatID integer,friID integer,NickName TEXT,Type integer,Picture TEXT,LastChatTime integer,LastMsg TEXT)");
        
        result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&errMsg);
        
        if(result!=SQLITE_OK)
            printf("create table chat failed ,错误原因:%s",errMsg);
    }
    
    return pdb;
    
}

bool SQLiteUtil:: addchat(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg){
    
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    int result;
    //int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("insert into chatlist(FriID,Type,LastMsg,NickName,Picture,LastChatTime,ChatID) values(%d,%d,'%q','%q','%q',%d,%d)",FriID,type,lastmsg,nickname,picture,lastchattime,chatID);
        
        printf("\n%s\n",sql.c_str());
        
        result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        if(result!=SQLITE_OK)
            printf("insert data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
        
    }
    return true;
    
}


char **SQLiteUtil::GetOneChat(const char *dbFilename,const int FriID){
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from chatlist where  FriID = %d",FriID);
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get chatid=%d data failed!错误原因:%s..sql:%s",FriID,errMsg,sql.c_str());
        sqlite3_close(pDB);
        
        return re;
    }
    else{
        sqlite3_close(pDB);
        
        return 0;
    }
    
}


char **SQLiteUtil::GetAllChat(const char *dbFilename){
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    char **re;
    int r,c;
    if(pDB!=NULL){
        
        sql=sqlite3_mprintf("select * from chatlist order by LastChatTime DESC");
        char result=sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        if(result!=SQLITE_OK)
            printf("get chatlist table data failed!错误原因:%s..sql:%s",errMsg,sql.c_str());
        sqlite3_close(pDB);
        
        
        return re;
    }
    else{
        sqlite3_close(pDB);
        
        return 0;
    }
    
}

int SQLiteUtil::GetChatRow(const char *dbFilename){
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        char **re;
        int r,c;
        sql=sqlite3_mprintf("select * from chatlist");
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        //printf("row is %d ,column is %d ",r,c);
        sqlite3_close(pDB);
        
        return r;
    }else {
        sqlite3_close(pDB);
        
        return 0;
    }
}

int SQLiteUtil::GetchatfindNum(const char *dbFilename,const int FriID){
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        char **re;
        int r,c;
        sql=sqlite3_mprintf("select * from chatlist where FriID=%d",FriID);
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        //printf("row is %d ,column is %d ",r,c);
        sqlite3_close(pDB);
        
        return r;
    }else {
        sqlite3_close(pDB);
        
        return 0;
    }
    
}


int SQLiteUtil::Getchattype(const char *dbFilename,const int FriID){
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        char **re;
        int r,c;
        sql=sqlite3_mprintf("select Type from chatlist where FriID=%d",FriID);
        sqlite3_get_table(pDB, sql.c_str(), &re, &r, &c, &errMsg);
        //printf("row is %d ,column is %d ",r,c);
        sqlite3_close(pDB);
        if(re[1]==NULL)
        {
            return 1;
        }else
            return atoi(re[1]);
    }
    else {
        sqlite3_close(pDB);
        
        return 0;
    }
    
}


bool SQLiteUtil:: UpdateChat(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg){
    
    std::string sql;
    sqlite3* pDB=PrepareChatDB(dbFilename);
    char *errMsg;
    if(pDB!=NULL){
        sql=sqlite3_mprintf("update chatlist set ChatID=%d, NickName='%q',Type=%d,Picture='%q',LastChatTime=%d ,LastMsg='%q' where FriID=%d",chatID,nickname,type,picture,lastchattime,lastmsg,FriID);
        char result=sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
        
        if(result!=SQLITE_OK)
        {
            printf("\nupdate  chat data failed!错误原因:%s..sql:%s======\n",errMsg,sql.c_str());
            sqlite3_close(pDB);
            
            return false;
        }
        else{
            printf("update chat data ok!..sql:%s======\n",sql.c_str());
            sqlite3_close(pDB);
            
            return true;
        }
        
    }else
        return false;

    
    return true;
}
