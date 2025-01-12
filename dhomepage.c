#include "aclihead.h"

MSGMENT hmsg;
pthread_t vpt;
//函数声明
int msgmentpage(int sid);

extern char curname[128];

//更新消息界面
int initmsgpage(int sid)
{
    //修改当前状态
    int state=0;
    upstatefun(sid,state);

    //获取未读消息数量
    hmsg.msgtype=getNumToUreadmsg;
    memcpy(hmsg.msgment,&curname,sizeof(curname));
    int ret=send(sid,&hmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

    ret=recv(sid,&hmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    int num=*(int *)hmsg.msgment;
    return num;
}

//vip客户消息界面
int msgmentpage(int sid)
{
    while (1)
    {
        printf("----------vip客户消息-----------\n");
        int num=initmsgpage(sid);
        char buf[128];
        printf("请选择您要的操作：\n1.联系客服\n2.聊天室\n3.接收消息(%d)\n\
4.退出\n",num);
        scanf("%s",buf);
        if(strcmp(buf,"1")==0)
        {
            talktoworker(sid);
        }
        if(strcmp(buf,"2")==0)
        {
            talkothervip(sid);
        }
        if(strcmp(buf,"3")==0)
        {
           getchatfun(sid);
        }
        if(strcmp(buf,"4")==0)
        {
           break;
        }
    }
    
}

int vippage(int sid)
{
    while (1)
    {
        printf("----------vip客户主界面-----------\n");
        int state=0;
        upstatefun(sid,state);
        char buf[128];
        printf("请选择您要的操作：\n1.下单\n2.消息界面\n3.查看订单\n4.退出\n");
        scanf("%s",buf);
        if(strcmp(buf,"1")==0)
        {
            AddOrder(sid);
        }
        if(strcmp(buf,"2")==0)
        {
            //进入消息界面
            msgmentpage(sid);
        }
        if(strcmp(buf,"3")==0)
        {
            showmyOrder(sid);
        }
        if(strcmp(buf,"4")==0)
        {
           break;
        }
    }
    
    return 0;
}

int workerroom(int sid)
{
    while (1)
    {
        printf("----------员工工作台界面-----------\n");
        char buf[128];
        printf("请选择您要的操作：\n1.发货\n2.补货\n3.查看订单\n4.查看库存\n5.退出\n");
        scanf("%s",buf);
        if(strcmp(buf,"1")==0)
        {
            sendflofun(sid); 
        }
        if(strcmp(buf,"2")==0)
        {
            AddFlower(sid);
        }
        if(strcmp(buf,"3")==0)
        {
            showOrder(sid);
        }
        if(strcmp(buf,"4")==0)
        {
            showFlower(sid);
        }
        if(strcmp(buf,"5")==0)
        {
            break;
        }
    }
    
    return 0;
}

int workpage(int sid)
{
    while (1)
    {
        printf("----------员工主界面-----------\n");
        int num=initmsgpage(sid);
        int state=0;
        upstatefun(sid,state);
        char buf[128];
        printf("请选择您要的操作：\n1.消息(%d)\n2.工作台\n3.宣传\n4.退出\n",num);
        scanf("%s",buf);
        if(strcmp(buf,"1")==0)
        {
            if(num>0)
            {
                printf("您有%d条未读消息,现在查看吗?yes/no\n",num);
                scanf("%s",buf);
                if(strcmp(buf,"yes")==0)
                {
                    getchatfun(sid);
                    reserveletter(sid);
                }
                else
                {
                    printf("不要忘记还有客户消息未回复!\n");
                }
            }
            else
            {
                printf("暂时没有要回复的消息\n");
            } 
        }
        if(strcmp(buf,"2")==0)
        {
            workerroom(sid);
        }
        if(strcmp(buf,"3")==0)
        {
            //群发宣传信息
            concessmsgfun(sid);
        }
        if(strcmp(buf,"4")==0)
        {
            break;
        }
    }
    return 0;
    
}

int rootpage(int sid)
{
    while (1)
    {
        printf("----------超级管理员主界面-----------\n");
        char buf[128];
        printf("请选择您要的操作：\n1.添加用户\n2.修改用户\n3.查看用户\n4.删除用户\n5.退出\n");
        scanf("%s",buf);
        if(strcmp(buf,"1")==0)
        {
            char type[128];
            printf("请选择您要添加用户的类型：\n1.会员\n2.员工\n");
            scanf("%s",type);
            char permis[20];
            if(strcmp(type,"1")==0)
            {
                strcpy(permis,"customer");
            }
            //2.员工
            else if(strcmp(type,"2")==0)
            {
                strcpy(permis,"workers");
            }
            //添加用户（包括员工和会员）
            addwandv(sid,permis);
        }
        else if(strcmp(buf,"2")==0)
        {
            //修改用户信息
            modifyUserinfo(sid);
        }
        else if(strcmp(buf,"3")==0)
        {
            //查看所有用户
            showUser(sid);
        }
        else if(strcmp(buf,"4")==0)
        {
            //删除用户
            delUserfun(sid);
        }
        else if(strcmp(buf,"5")==0)
        {
            break;
        }
    }
    return 0;
    
}

