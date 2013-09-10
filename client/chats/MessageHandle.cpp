//
//  MessageHandle.cpp
//  chats
//
//  Created by 付剑超 on 13-8-14.
//
//

#include "MessageHandle.h"
#include "MessageBase.h"


bool SavesendMsgtoDB(const char *dbFilename,const int username,const char *message,const int type,const int fromID){
    
    int des=1;
    SaveMsgdb(dbFilename,username,0, SyncTime(0), message, 0, type, des,fromID);


    return true;
}

bool SavereceMsgtoDB(const char *dbFilename,const int username,const char *message,const int type,const int fromID){
    
    int des=2;

    int msgSvrID=60;
    
    SaveMsgdb(dbFilename,username,msgSvrID, time(NULL), message, 0, type, des,fromID);
    
    return true;
}

bool DelMessage(){
    return true;
}

bool DelChat(){
    return true;
}

messageDate get1Msg(const char *dbFilename,const int username,const int mesLocalID){
    
    char **re=GetOneMessage(dbFilename, username, mesLocalID);
    
    messageDate messagedate;
    
    for (int i=messagecolumn;i<messagecolumn*2;){
               
        messagedate.MeslocalID=re[i++];
        messagedate.MesSvrID=re[i++];
        messagedate.createTime=re[i++];
        messagedate.message=re[i++];
        messagedate.status=re[i++];
        messagedate.type=re[i++];
        messagedate.Des=re[i++];
        messagedate.fromid=re[i++];
    }

    return messagedate;
    
    
}



messageDate gettheLastMsg(const char *dbFilename,const int username){
    
    char **re=GetNumMsg(dbFilename, username,1);
    messageDate messagedate;
    
    for (int i=messagecolumn;i<messagecolumn*2;){
        
        messagedate.MeslocalID=re[i++];
        messagedate.MesSvrID=re[i++];
        messagedate.createTime=re[i++];
        messagedate.message=re[i++];
        messagedate.status=re[i++];
        messagedate.type=re[i++];
        messagedate.Des=re[i++];
        messagedate.fromid=re[i++];
    }
    
    return messagedate;


}
char **get1lastMsg(const char *dbFilename,const int username){
    char **re=GetNumMsg(dbFilename, username, 1);
    return re;

}
char **getsomeMsg(const char *dbFilename,const int username,const int num){
    char **re=GetNumMsg(dbFilename, username, num);
    return re;
    
}
messageDate* GetnumMsg(const char *dbFilename,const int username,const int msgnum){
    

    char **re=GetNumMsg(dbFilename, username,msgnum);
    messageDate *messagedate=new messageDate[msgnum];
    
    for (int i=0,j=msgnum;i<messagecolumn*msgnum;){
        if (i%messagecolumn==0 &&i!=0) j--;
        messagedate[j].MeslocalID=re[i++];
        messagedate[j].MesSvrID=re[i++];
        messagedate[j].createTime=re[i++];
        messagedate[j].message=re[i++];
        messagedate[j].status=re[i++];
        messagedate[j].type=re[i++];
        messagedate[j].Des=re[i++];
        messagedate[j].fromid=re[i++];
    }
    
    return messagedate;
}

int GetmsgCount(const char *dbFilename,const int username){
    
    int num=msgcount(dbFilename,username);
    return num;
    
    
}

bool DelMsglist(const char *dbFilename,const int username){
    DelmsgTable(dbFilename, username);
    
    return true;
}

FriendDate GetOneFriendfromDB(const char *dbFilename,const int FriID){
    char **re=GetOneFriend(dbFilename, FriID);
    FriendDate frienddate;

    for (int i=friendcolumn;i<friendcolumn*2;){
        
        frienddate.FriLocalID=re[i++];
        frienddate.FriID=re[i++];
        frienddate.NickName=re[i++];
        frienddate.Mobile=re[i++];
        frienddate.Type=re[i++];
        frienddate.Sex=re[i++];
        frienddate.Picture=re[i++];
        frienddate.lastMessage=re[i++];
    }
    
    return frienddate;
}
FriendDate GetOneFriendfromDBlocal(const char *dbFilename,const int FrilocID){
    
    char **re=GetOneFriendlocal(dbFilename, FrilocID);
    FriendDate frienddate;
    
    for (int i=friendcolumn;i<friendcolumn*2;){
        
        frienddate.FriLocalID=re[i++];
        frienddate.FriID=re[i++];
        frienddate.NickName=re[i++];
        frienddate.Mobile=re[i++];
        frienddate.Type=re[i++];
        frienddate.Sex=re[i++];
        frienddate.Picture=re[i++];
        frienddate.lastMessage=re[i++];
    }
    
    return frienddate;
}

char** GetOneFriendFDB(const char *dbFilename,const int FriID){
    char **re=GetOneFriend(dbFilename, FriID);
    return re;
}



char **getallFDB(const char *dbFilename){
    char **re=GetAllFriend(dbFilename);
    return re;
    
}
char GetonefriendFDB(const char *dbFilename,const int FriID);


FriendDate* GetAllFriendfromDB(const char *dbFilename){
    
    char **re=GetAllFriend(dbFilename);
    
    int friendnum=FriendNum(dbFilename);

    FriendDate *frienddate=new FriendDate[friendnum];
    
    for (int i=friendcolumn,j=friendnum;i<friendcolumn*(friendnum+1);){
        if (i%friendnum==0 &&i!=0) j--;
        
        frienddate[j].FriLocalID=re[i++];
        frienddate[j].FriID=re[i++];
        frienddate[j].NickName=re[i++];
        frienddate[j].Mobile=re[i++];
        frienddate[j].Type=re[i++];
        frienddate[j].Sex=re[i++];
        frienddate[j].Picture=re[i++];
        frienddate[j].lastMessage=re[i++];
        printf("==id=%s===nickname=%s=pic=%s=\n",frienddate[j].FriID,frienddate[j].NickName,frienddate[j].Picture);
        
    }
    
    return frienddate;
    
}

bool AddFriendtoDB(const char *dbFilename,const int FriID){
    
    int findnum=FriendFindCount(dbFilename, FriID);
    
    
    if (findnum==0) {
        
        AddFriend(dbFilename,FriID);
        
        char *nickname="张三";
        int mobile=13512345678;
        int type=1;
        int sex=1;
        std::string picture="head2.png";
        
        UpdateFriend(dbFilename, FriID, nickname, mobile, type, sex, picture.c_str());
    }
    return true;
}

int GetFriendCount(const char *dbFilename){
    int num=FriendNum(dbFilename);
    return num;
}

int FriendFindCount(const char *dbFilename,const int FriID){
    int num=FriendFindNum(dbFilename, FriID);
    return num;
    
}

bool DelFriendtoDB(const char *dbFilename,const int FriID){
    DelFriendtoDB(dbFilename, FriID);
    return true;
    
}

bool SaveChattoDB(const char *dbFilename,const int FriID,const char *lastmsg,const int type){
    
    int chatid=20;
    char *nickname="zhangsan";
    //int type=1;
    char *picture="head3.png";
    
    int find=getchatfindnum(dbFilename, FriID);
    
    bool result;
    if (0==find) {
    
        addChattoDb(dbFilename,  chatid, FriID, nickname, type, picture, time(NULL), lastmsg);
    }else{
        updatemsgtochat(dbFilename, FriID, lastmsg);
    }
    
    return true;
    
}


char **GetoneChat(const char *dbFilename,const int FriID){
    char **temp=GetOneChat(dbFilename, FriID);
    return temp;
    
}

char **GetallChats(const char *dbFilename){
    char **temp=GetAllChats(dbFilename);
    return temp;
}

int GetchatNum(const char *dbFilename){
    int temp=ChatNum(dbFilename);
    return temp;
}

int GetchatfindCount(const char *dbFilename,const int FriID){
    
    int temp=getchatfindnum(dbFilename,FriID);
    return temp;
    
}

int GetChatType(const char *dbFilename,const int FriID){
    
    int temp=getChattype(dbFilename, FriID);
    return temp;
}


bool DelchatTable(const char *dbFilename){
    bool temp=DelChatTable(dbFilename);
    return temp;
}

bool updatemsgtochat(const char *dbFilename,const int FriID,const char *lastmsg){
    
    int chatid=20;
    char *nickname="zhangsan";
    int type=1;
    char *picture="head3.png";
    //int time=time_t();
    
    bool result=UpdateoneChat(dbFilename,  chatid, FriID, nickname, type, picture, time(NULL), lastmsg);
    
    return result;
}
