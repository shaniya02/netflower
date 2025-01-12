#include "aclihead.h"

MSGMENT wmsg;

pthread_t wpt;

extern char curname[128];
MSGMENT wmsg;
//客服接收消息线程
void * pthworker(void *p)
{
    int sid=*(int *)p;
    while (1)
    {
        int ret=recv(sid,&wmsg,sizeof(MSGMENT),0);
        // printf("客服接收消息线程%d",wmsg.msgtype);
        if(ret==-1)
        {
            perror("接受数据出错");
            return 0;
        }
        // printf("%s\n",wmsg.msgment);
        CHATINFO chatmsg=*(CHATINFO *)wmsg.msgment;
        struct tm * ti=localtime(&(chatmsg.tm));
        printf("客服消息:%02d日%02d点%02d分接收到会员%s发来的消息：%s\n",
        ti->tm_mday,ti->tm_hour,ti->tm_min,chatmsg.myname,chatmsg.text);

    }
    return 0;
}

// 补货
int AddFlower(int sid)
{
    // system("clear");
    printf("---------补货界面---------\n");
    char buf[128];
    printf("请选择您要的操作：\t1.增加库存\t2.新增花种\n");
    scanf("%s",buf);
    if(strcmp(buf,"1")==0)
    {
        printf("请输入您要补货的花的名字 数量:\n");
        FLOWER upflo;
        scanf("%s %d",upflo.fname,&upflo.num);
        wmsg.msgtype=updateFlower;
        memcpy(wmsg.msgment,&upflo,sizeof(FLOWER));
        int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }
    }
    if(strcmp(buf,"2")==0)
    {
        printf("请依次输入花卉名字 数量 价格:\n");
        FLOWER flower;
        scanf("%s %d %lf", flower.fname, &(flower.num), &(flower.price));
        wmsg.msgtype=addFlower;
        memcpy(wmsg.msgment,&flower,sizeof(FLOWER));
        int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }
    }
    int ret=recv(sid,&wmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("接受数据出错");
        return 0;
    }
    // printf("接受消息类型：%d",msg.msgtype);
    if(wmsg.msgtype == ok)
    {
        printf("入库成功!\n");
    } 
    else if(wmsg.msgtype == nok)
    {
       printf("添加花失败!\n");
    } 
    
    // Wait(3);
}

//查看库存
int showFlower(int sid)
{
    // system("clear");
    printf("---------显示库存界面---------\n");
    int count =80;
    printf("\033[0m%-10s %-15s %-10s %-12s\n",
    "商品id","花卉名字","数量","价格");
    printLine(count);
    wmsg.msgtype=selectAllFlower;
    
    int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    while (1)
    {
        
        ret=recv(sid,&wmsg,sizeof(MSGMENT),0);
        // printf("收到的信息类型:%d\n",wmsg.msgtype);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        else
        {
            if(wmsg.msgtype==ok)
            {
                FLOWER  pflo=*(FLOWER *)(wmsg.msgment);
                printf("\033[0m%-8d %-14s %-8d %-10.2lf\n", 
                    pflo.id,pflo.fname,pflo.num,pflo.price);
                if(pflo.num < 30)
                    printf("%s缺货，请尽快补货!\n",pflo.fname);
                printLine(count);
            }
            else if(wmsg.msgtype==over)
            {
                break;
            }
        }
    }
}

//查看订单
int showOrder(int sid)
{
    // system("clear");
    printf("---------查看订单界面---------\n");
    int jump;
    int count =80;
    printf("\033[0m%-10s %-15s %-10s %-12s %-15s %-14s\n",
    "订单id","花卉名字","数量","价格","顾客帐号","状态");
    printLine(count);
    wmsg.msgtype=selectAllOrder;
    int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    while (1)
    {
        ret=recv(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        else
        {
            if(wmsg.msgtype==ok)
            {
                ORDER  porder=*(ORDER *)(wmsg.msgment);
                char state[128];
                if(porder.oflag ==1)
                {
                    jump=1;
                    strcpy(state,"已下单");
                }
                else if(porder.oflag ==2)
                {
                    strcpy(state,"已发货");
                }
                else if(porder.oflag ==3)
                {
                    strcpy(state,"已收货");
                }
                printf("\033[0m%-8d %-14s %-8d %-10.2lf %-11s %-15s \n", 
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
        printf("您有未发货的订单,是否现在发货?yes/no\n");
        char buf[128];
        scanf("%s",buf);
        if(strcmp(buf,"yes")==0)
        {
            sendflofun(sid);
        }
        else
        {
            return 0;
        }
    }
}

void printLine(int count)
{
     for (size_t i = 0; i < count; i++)
     {
          printf("-");
     }
     printf("\n");
}

//发货函数
int sendflofun(int sid)
{
    printf("---------发货界面---------\n");

    int count = 80;
    printf("\033[45m以下是未发货的订单:\033[0m \n");
    printLine(count);
    printf("\033[0m%-10s %-15s %-10s %-12s %-15s %-14s\n",
    "订单id","花卉名字","数量","价格","顾客帐号","状态");
    printLine(count);
    wmsg.msgtype=getoneOrder;
    int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
    while (1)
    {
        ret=recv(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错\n");
            return 0;
        }
        if(wmsg.msgtype==ok)
        {
            ORDER  porder=*(ORDER *)(wmsg.msgment);
            
            char state[128];
            if(porder.oflag ==1)
            {
                strcpy(state,"已下单");
            }
            else
            {
                strcpy(state,"未下单");
            }
            printf("\033[0m%-8d %-14s %-8d %-10.2lf %-11s %-15s \n", 
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
        printf("请输入你要发货的订单编号:(输入0,退出界面)\n");
        UPSTATEF info;
        scanf("%d",&info.orderid);
        if(info.orderid==0)
        {
            break;
        }
        info.ordersatet=2;
        wmsg.msgtype=updateOrder;
        memcpy(wmsg.msgment,&info,sizeof(UPSTATEF));
        int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }

        ret=recv(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("接受数据出错");
            return 0;
        }
        // printf("接受消息类型：%d",msg.msgtype);
        if(wmsg.msgtype == ok)
        {
            printf("发货成功!\n");
        } 
        else if(wmsg.msgtype == nok)
        {
            printf("发货失败!\n");
        } 
    }
    
    
}

//群发宣传信息界面
int concessmsgfun(int sid)
{
    printf("----------群发宣传信息界面-----------\n");
    CHATINFO postmsg;
    strcpy(postmsg.myname,curname);
    strcpy(postmsg.yourpermis,"customer");
    printf("请输入宣传内容：\n");
    scanf("%s",postmsg.text);
    memcpy(wmsg.msgment,&postmsg,sizeof(CHATINFO));
    wmsg.msgtype=produceMsgChat;
    int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }

}

//更新状态函数
int upstatefun(int sid,int state)
{
    wmsg.msgtype=updstate;
    memcpy(wmsg.msgment,&state,sizeof(state));
    int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
    if(ret==-1)
    {
        perror("发送数据出错\n");
        return 0;
    }
}
//回复消息
int reserveletter(int sid)
{
    printf("----------回复消息界面-----------\n");
    pthread_create(&wpt,NULL,pthworker,&sid);
    int state=1;
    upstatefun(sid,state);
    printf("请输入你要回复的用户名：\n");
    char buf[128];
    scanf("%s",buf);
    CHATINFO chatmsg;
    strcpy(chatmsg.myname,curname);
    strcpy(chatmsg.yourname,buf);
    while(1)
    {
        printf("请输入聊天消息：\n");
        scanf("%s",chatmsg.text);

        wmsg.msgtype=vipChatworker;
        memcpy(wmsg.msgment,&chatmsg,sizeof(CHATINFO));

        int ret=send(sid,&wmsg,sizeof(MSGMENT),0);
        if(ret==-1)
        {
            perror("发送数据出错\n");
            return 0;
        }
        if(strcmp(chatmsg.text,"quit")==0)
        {
            pthread_cancel(wpt);
            return 0;
        }
    }

}


