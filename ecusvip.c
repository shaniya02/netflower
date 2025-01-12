#include "aclihead.h"

MSGMENT cmsg;

extern pthread_t vpt;

extern char curname[128];

//会员接收消息线程
void * pthvip(void *p)
{
    int sid=*(int *)p;

    MSGMENT msg;
    while (1)
    {
        int ret=recv(sid,&msg,sizeof(msg),0);
        if(ret==-1)
        {
            perror("接受数据出错");
            return 0;
        }
        // printf("%s\n",msg.msgment);
        CHATINFO chatmsg=*(CHATINFO *)msg.msgment;
        struct tm * ti=localtime(&(chatmsg.tm));
        printf("会员消息:%02d日%02d点%02d分接收到%s发来的消息：%s\n",
        ti->tm_mday,ti->tm_hour,ti->tm_min,chatmsg.myname,chatmsg.text);
        
    }
    return 0;
}

// 下单
int AddOrder(int sid)
{
    // system("clear");
    printf("---------下单界面---------\n");
    printf("请输入您要下单的花的名字 数量\n");
    ORDER order;
    scanf("%s %d",order.fname, &(order.num));
    strcpy(order.oworker,curname);
    cmsg.msgtype=addOrder;
    memcpy(cmsg.msgment,&order,sizeof(ORDER));
    int ret=send(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

    ret=recv(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    // printf("接受消息类型：%d",msg.msgtype);
    if(cmsg.msgtype == ok)
    {
        printf("%s\n",cmsg.msgment);
    } 
    else if(cmsg.msgtype == nok)
    {
        printf("%s\n",cmsg.msgment);
    } 
    
}

//查看订单
int showmyOrder(int sid)
{
    // system("clear");
    printf("---------查看我的订单界面---------\n");
    int count =80;
    printf("\033[0m%-10s %-15s %-10s %-12s %-15s %-14s\n",
    "订单id","花卉名字","数量","价格","下单帐号","状态");
    printLine(count);
    cmsg.msgtype=selectMyOrder;
    int jump=0;
    memcpy(cmsg.msgment,&curname,sizeof(curname));
    int ret=send(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

    while (1)
    {
        ret=recv(sid,&cmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        else
        {
            if(cmsg.msgtype==ok)
            {
                ORDER  porder=*(ORDER *)(cmsg.msgment);
                char state[128];
                if(porder.oflag ==1)
                {
                    strcpy(state,"已下单");
                }
                else if(porder.oflag ==2)
                {
                    jump=1;
                    strcpy(state,"已发货");
                }
                else if(porder.oflag ==3)
                {
                    strcpy(state,"已收货");
                }
                printf("\033[0m%-8d %-14s %-8d %-11.2lf %-11s %-15s \n", 
                porder.orid,porder.fname,porder.num,porder.price,porder.oworker,state);
                printLine(count);
            }
            else
            {
                break;
            }
        }
    }
    if (jump==1)
    {
        printf("您有未收货的订单,是否现在收货?yes/no\n");
        char buf[128];
        scanf("%s",buf);
        if(strcmp(buf,"yes")==0)
        {
            getflofun(sid);
        }
        else
        {
            return 0;
        }
    }
    
}

//收货函数
int getflofun(int sid)
{
    printf("---------收货界面---------\n");
    int count =80;
    printf("\033[45m以下是未收货的订单:\033[0m \n");
    printLine(count);
    printf("\033[0m%-10s %-15s %-10s %-12s %-15s %-14s\n",
    "订单id","花卉名字","数量","价格","下单帐号","状态");
    printLine(count);
    cmsg.msgtype=gettwoOrder;
    memcpy(cmsg.msgment,&curname,sizeof(curname));
    int ret=send(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    while (1)
    {
        ret=recv(sid,&cmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        if(cmsg.msgtype==ok)
        {
            ORDER  porder=*(ORDER *)(cmsg.msgment);
            
            char state[128];
            if(porder.oflag ==2)
            {
                strcpy(state,"已发货");
            }
            printf("\033[0m%-8d %-14s %-8d %-11.2lf %-11s %-15s \n", 
            porder.orid,porder.fname,porder.num,porder.price,porder.oworker,state);
            printLine(count);
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("请输入你要收货的订单编号:(输入0,退出界面)\n");
        UPSTATEF info;
        scanf("%d",&info.orderid);
        if(info.orderid==0)
        {
            break;
        }
        info.ordersatet=3;
        cmsg.msgtype=updateOrder;
        memcpy(cmsg.msgment,&info,sizeof(UPSTATEF));
        int ret=send(sid,&cmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }

        ret=recv(sid,&cmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错");
            return 0;
        }
        // printf("接受消息类型：%d",msg.msgtype);
        if(cmsg.msgtype == ok)
        {
            printf("收货成功!\n");
        } 
        else if(cmsg.msgtype == nok)
        {
            printf("收货失败!\n");
        } 
    }
    
    
}

//联系客服
int talktoworker(int sid)
{
    printf("请输入你要联系的客服名字(yyy/mmm/sss)：\n");
    char buf[128];
    scanf("%s",buf);

    cmsg.msgtype=ifuser;
    memcpy(cmsg.msgment,&buf,sizeof(buf));

    int ret=send(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    
    ret=recv(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错\n");
        return 0;
    }
    // pthread_create(&spt,NULL,pthsingle,&sid);
    // printf("接受消息类型：%d",msg.msgtype);
    if(cmsg.msgtype == ok)
    {
        LOGINFO lworkerinfo=*(LOGINFO *)cmsg.msgment;

        if(lworkerinfo.curstate==1)
        {
            printf("%s客服在线,可留言\n",buf);
            printf("请问是否留言：yes/no\n");
            char isletter[128];
            scanf("%s",isletter);
            if(strcmp(isletter,"yes")==0)
            {
                leave_comment(sid,buf);
            }
        }
        else if (lworkerinfo.curstate==2)
        {
            printf("%s客服正在和其他会员沟通,可留言\n",buf);
            printf("请问是否留言：yes/no");
            char isletter[128];
            scanf("%s",isletter);
            if(strcmp(isletter,"yes")==0)
            {
                leave_comment(sid,buf);
            }
        }
        else if (lworkerinfo.curstate==3)
        {
            printf("%s客服在线,可发起聊天\n",buf);
            start_chat(sid,lworkerinfo);
        }
    } 
    else if(cmsg.msgtype == nok)
    {
        printf("您要联系的客服%s不在线!\n",buf);
    } 
}

//给客服留言函数
int leave_comment(int sid,char * buf)
{
    CHATINFO tempchat;
    strcpy(tempchat.myname,curname);
    strcpy(tempchat.yourname,buf);
    while(1)
    {
        printf("请输入留言消息：\n");
        scanf("%s",tempchat.text);

        cmsg.msgtype=leaveComment;
        memcpy(cmsg.msgment,&tempchat,sizeof(CHATINFO));

        int ret=send(sid,&cmsg,sizeof(cmsg),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }
        if(strcmp(tempchat.text,"quit")==0)
        {
            pthread_cancel(vpt);
            return 0;
        }
    }

}

//开始和客服聊天
int start_chat(int sid,LOGINFO lworkerinfo)
{
    printf("可以开始聊天了：\n");
    pthread_create(&vpt,NULL,pthvip,&sid);

    CHATINFO chatmsg;
    strcpy(chatmsg.myname,curname);
    strcpy(chatmsg.yourname,lworkerinfo.username);
    chatmsg.yoursid=lworkerinfo.sid;

    while(1)
    {
        printf("请输入聊天消息：\n");
        scanf("%s",chatmsg.text);

        cmsg.msgtype=vipChatworker;
        memcpy(cmsg.msgment,&chatmsg,sizeof(CHATINFO));

        int ret=send(sid,&cmsg,sizeof(cmsg),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }
        if(strcmp(chatmsg.text,"quit")==0)
        {
            pthread_cancel(vpt);
            return 0;
        }
    }
}

//获取消息临时表中的消息
int getchatfun(int sid)
{
    printf("----------查看未查看消息界面-----------\n");
    //发送接收临时消息的信号
    cmsg.msgtype=getTempChatTable;
    memcpy(cmsg.msgment,&curname,sizeof(curname));
    int ret=send(sid,&cmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

    while (1)
    {
        ret=recv(sid,&cmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        else
        {
            if(cmsg.msgtype==ok)
            {
                CHATINFO chatmsg=*(CHATINFO *)cmsg.msgment;
                struct tm * ti=localtime(&(chatmsg.tm));
                printf("%02d日%02d点%02d分接收到%s发来的消息：%s\n",
                ti->tm_mday,ti->tm_hour,ti->tm_min,chatmsg.myname,chatmsg.text);

            }
            else if(cmsg.msgtype==over)
            {
                break;
            }
        }
    }
}

//聊天室
int talkothervip(int sid)
{
    printf("----------欢迎进入聊天室-----------\n");
    pthread_create(&vpt,NULL,pthvip,&sid);
    int state =1;
    upstatefun(sid,state);
    CHATINFO postmsg;
    strcpy(postmsg.myname,curname);
    strcpy(postmsg.yourpermis,"customer");
    while(1)
    {
        printf("请输入聊天内容：\n");
        scanf("%s",postmsg.text);
        cmsg.msgtype=chatRoom;
        memcpy(cmsg.msgment,&postmsg,sizeof(CHATINFO));
        int ret1=send(sid,&cmsg,sizeof(cmsg),0);

        if(ret1==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }
        if(strcmp(postmsg.text,"quit")==0)
        {
            pthread_cancel(vpt);
            return 0;
        }
    }
    
}

