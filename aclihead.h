#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

//消息类型
typedef enum type
{
    regist,login,//添加用户用regist
    modifyUserInfo,selectAllUser,deleteUser,ifuser,//用户相关
    addFlower,updateFlower,isFlower,selectAllFlower,//管理花
    selectAllOrder,getoneOrder,produceMsgChat,//员工
    addOrder,selectMyOrder,gettwoOrder,updateOrder,//会员
    leaveComment,chatRoom,vipChatworker,getTempChatTable,//聊天相关
    getNumToUreadmsg,
    updstate,ok,nok,over,
    quit
}TYPE;

//通用消息结构
typedef struct msgment
{
    TYPE msgtype;
    char msgment[1020]; 
}MSGMENT;

//注册结构，等于用户信息
typedef struct regist
{
    int id;
    char permission[128];//权限
    char username[50];
    char pwd[50]; 
    char phone[20];
}REGIST;

//登录结构，用在登录
typedef struct login
{
    char username[128];
    char pwd[128]; 
    char permission[128];//权限
}LOGIN;

//已登录人员信息
typedef struct loginInfo
{
    char username[128];
    int sid;
    int curstate;//当前状态
}LOGINFO;
//当前状态：0：不在线，1：在线，可留言，2.客服正在和其他会员沟通,可留言 
//3.客服在线,可发起聊天

//订单信息结构
typedef struct order
{
    int orid;
    char fname[20];
    int num;
    double price;
    time_t otime;
    char oworker[30];
    int oflag;//1：已下单 2：已发货 2：已收货
} ORDER;

//花信息结构
typedef struct flower
{
    int id;
    char fname[50];
    int num;
    double price;
} FLOWER;

//聊天功能
typedef struct chatInfo
{
    char myname[128];
    int mysid;
    char yourname[128];
    int yoursid;
    char text[500];
    char yourpermis[128];
    time_t tm;
}CHATINFO;

typedef struct upstatef
{
    int orderid;
    int ordersatet;
}UPSTATEF;

//登录注册函数
int registfun(int sid);
void loginpage(int sid);
int logincheck(int sid,char * type);
//登录后的主页面
int vippage(int sid);
int workpage(int sid);
int rootpage(int sid);

//vip客户页面
int AddOrder(int sid);//下单
int talktoworker(int sid);//联系客服
void * pthvip(void *p); //客户线程
int showmyOrder(int sid);//查询个人订单
int getflofun(int sid);//收货函数
int leave_comment(int sid,char * buf);//留言函数
int start_chat(int sid,LOGINFO lworkerinfo);//开始聊天
int talkothervip(int sid);//聊天室

//客户和员工共用
int getchatfun(int sid);//获取临时表信息
int upstatefun(int sid,int state);//更新状态函数
void printLine(int count);

//员工页面
int AddFlower(int sid);// 补货
int showFlower(int sid);//显示库存
int showOrder(int sid);//显示订单
int concessmsgfun(int sid);//群发宣传信息界面
int reserveletter(int sid);//回复信息
int sendflofun(int sid);//发货函数

//管理员页面
int addwandv(int sid,char * permis);//添加用户
int showUser(int sid);//显示用户
int modifyUserinfo(int sid);//修改用户
int delUserfun(int sid);//删用户




// void chatpage(int sid);

// int grochatfun(int sid);
// int singlefun(int sid);
// int getChat(int sid);



