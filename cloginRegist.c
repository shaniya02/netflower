#include "aclihead.h"

char curname[128]={0};
int loginNum=0;

MSGMENT msg;

//注册
int registfun(int sid)
{
    printf("----------会员注册界面-----------\n");
    REGIST user;
    printf("请依次输入您的用户名 密码 联系电话：\n");
    scanf("%s %s %s",user.username,user.pwd,user.phone);
    strcpy(user.permission,"customer");

    msg.msgtype=regist;
    memcpy(msg.msgment,&user,sizeof(user));

    int ret=send(sid,&msg,sizeof(msg),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

    ret=recv(sid,&msg,sizeof(msg),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    if(msg.msgtype == ok)
    {
        printf("%s 注册成功!\n",user.username);
    } 
    else if(msg.msgtype == nok)
    {
        printf("%s 注册失败!\n",user.username);
    } 
}

//登录检查
int logincheck(int sid,char * type)
{
    LOGIN luser;
    printf("请输入您的用户名 密码:\n");
    scanf("%s %s",luser.username,luser.pwd);
    //1.会员
    if(strcmp(type,"1")==0)
    {
        strcpy(luser.permission,"customer");
    }
    //2.员工
    else if(strcmp(type,"2")==0)
    {
        strcpy(luser.permission,"workers");
    }
    //3.超级管理员
    else if(strcmp(type,"3")==0)
    {
        strcpy(luser.permission,"root");
    }

    msg.msgtype=login;
    memcpy(msg.msgment,&luser,sizeof(luser));

    int ret=send(sid,&msg,sizeof(msg),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

    ret=recv(sid,&msg,sizeof(msg),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }

    if(msg.msgtype == ok)
    {
        strcpy(curname,luser.username);
        return 1;
    } 
    else if(msg.msgtype == nok)
    {
        return 2;
    } 
}

void loginpage(int sid)
{
    printf("----------登陆界面-----------\n");
    char type[128];
    printf("请选择您要登录的类型：\n1.会员\n2.员工\n3.超级管理员\n");
    scanf("%s",type);
    int flag=logincheck(sid,type);
    if(flag == 1)
    {
        printf("登陆成功!\n");
        //1.会员
        if(strcmp(type,"1")==0)
        {
            vippage(sid);
        }
        //2.员工
        else if(strcmp(type,"2")==0)
        {
            workpage(sid);
        }
        //3.超级管理员
        else if(strcmp(type,"3")==0)
        {
            rootpage(sid);
        }
        
    }
    else
    {
        printf(" 登陆失败! %d\n",flag);
    }
}



