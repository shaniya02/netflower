#include "aclihead.h"

MSGMENT rmsg;

//显示所有用户
int showUser(int sid)
{
    printf("---------显示用户界面---------\n");
    int count=80;
    printf("%-12s %-15s %-10s %-17s %-15s\n",
    "用户id","用户名字","密码","用户权限","电话号码");
    printLine(count);
    rmsg.msgtype=selectAllUser;
    int ret=send(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    while (1)
    {
        ret=recv(sid,&rmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        else
        {
            if(rmsg.msgtype==ok)
            {
                REGIST  userinfo=*(REGIST *)(rmsg.msgment);
                printf("%-10d %-12s %-8s %-13s %-11s \n", 
                userinfo.id,userinfo.username,userinfo.pwd,userinfo.permission,userinfo.phone);
                printLine(count);
            }
            else
            {
                break;
            }
        }
    }
}

//添加员工或客户
int addwandv(int sid,char * permis)
{
    REGIST user;
    printf("请输入要添加的用户名 密码 电话号码：\n");
    scanf("%s %s %s",user.username,user.pwd,user.phone);
    strcpy(user.permission,permis);
    rmsg.msgtype=regist;
    memcpy(rmsg.msgment,&user,sizeof(user));
    int ret=send(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    ret=recv(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    if(rmsg.msgtype == ok)
    {
        printf("%s 添加成功!\n",user.username);
    } 
    else if(rmsg.msgtype == nok)
    {
        printf("%s 添加失败!\n",user.username);
    } 
}

//修改用户信息
int modifyUserinfo(int sid)
{
    REGIST user;
    printf("请输入要修改的用户id：\n");
    scanf("%d",&user.id);
    printf("请输入ta修改后的名字 密码 权限 电话号码：\n");
    scanf("%s %s %s %s",user.username,user.pwd,user.permission,user.phone);
    
    rmsg.msgtype=modifyUserInfo;
    memcpy(rmsg.msgment,&user,sizeof(user));
    int ret=send(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    ret=recv(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    if(rmsg.msgtype == ok)
    {
        printf("%s 修改成功!\n",user.username);
    } 
    else if(rmsg.msgtype == nok)
    {
        printf("%s 修改失败!\n",user.username);
    } 
}

int delUserfun(int sid)
{
    printf("---------删除用户信息界面---------\n");
    printf("请输入您要删除的用户id:\n");
    int userid,flag=0;
    scanf("%d",&userid);
    rmsg.msgtype=deleteUser;
    memcpy(rmsg.msgment,&userid,sizeof(userid));
    int ret=send(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    ret=recv(sid,&rmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    if(rmsg.msgtype == ok)
    {
        printf("删除成功!\n");
    } 
    else if(rmsg.msgtype == nok)
    {
        printf("删除失败!\n");
    } 
}
