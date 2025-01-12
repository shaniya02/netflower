// #include "aclihead.h"

// extern char curname[128];

// extern MSGMENT msg;

// pthread_t gpt;
// pthread_t spt;
// //群聊线程
// void * pthgroup(void *p)
// {
//     int sid=*(int *)p;

//     MSGMENT msg;
//     while (1)
//     {
//         int ret=recv(sid,&msg,sizeof(msg),0);
//         if(ret==-1)
//         {
//             perror("接受数据出错");
//             return 0;
//         }
//         printf("客户机接收到消息：%s\n",msg.msgment);
//     }
//     return 0;
// }
// //私聊线程
// void * pthsingle(void *p)
// {
//     int sid=*(int *)p;

//     MSGMENT msg;
//     while (1)
//     {
//         int ret=recv(sid,&msg,sizeof(msg),0);
//         if(ret==-1)
//         {
//             perror("接受数据出错");
//             return 0;
//         }
//         CHATINFO chatmsg=*(CHATINFO *)msg.msgment;
//         struct tm * ti=localtime(&(chatmsg.tm));
//         printf("%02d日%02d点%02d分接收到%s发来的消息：%s\n",
//         ti->tm_mday,ti->tm_hour,ti->tm_min,chatmsg.myname,chatmsg.text);
//     }
//     return 0;
// }

// void chatpage(int sid)
// {
//     while (1)
//     {
//          //聊天
//         printf("----------聊天界面-----------\n");
//         printf("请选择您要的操作：1.群聊 2.私聊 3.查看聊天记录\n");
//         char buf[128];
//         scanf("%s",buf);
//         //群聊
//         if(strcmp(buf,"1")==0)
//         {
//             grochatfun(sid);
//         }
//         //私聊
//         else if(strcmp(buf,"2")==0)
//         {
//             singlefun(sid);
//         }
//         //查看聊天记录
//         else if(strcmp(buf,"3")==0)
//         {
//             getChat(sid);
//         }
//     }
// }

// int getChat(int sid)
// {
//     printf("请输入对方名字：");
//     CHATINFO names;
//     scanf("%s",names.yourname);
//     strcpy(names.myname,curname);
//     msg.msgtype=getchat;
//     memcpy(msg.msgment,&names,sizeof(CHATINFO));
//     int ret=send(sid,&msg,sizeof(msg),0);
//     if(ret==-1)
//     {
//         perror("发送数据出错\n");
//         return 0;
//     }

//     while (1)
//     {
//         ret=recv(sid,&msg,sizeof(msg),0);
//         if(ret==-1)
//         {
//             perror("接受数据出错\n");
//             return 0;
//         }
//         else
//         {
//             if(msg.msgtype==ok)
//             {
//                 CHATINFO chatmsg=*(CHATINFO *)(msg.msgment);
//                 struct tm * ti=localtime(&(chatmsg.tm));
//                 printf("%02d日%02d点%02d分,%s->%s：%s\n",
//                 ti->tm_mday,ti->tm_hour,ti->tm_min,chatmsg.myname,chatmsg.yourname,chatmsg.text);
//             }
//             else
//             {
//                 break;
//             }
//         }
//     }
    
// }

// int grochatfun(int sid)
// {
//     printf("----------群聊界面-----------\n");
//     pthread_create(&gpt,NULL,pthgroup,&sid);
//     while(1)
//     {
//         printf("请输入聊天内容：\n");
//         scanf("%s",msg.msgment);
//         if(strcmp(msg.msgment,"quit")==0)
//         {
//             msg.msgtype=quit;
//         }
//         else 
//         {
//             msg.msgtype=groupchat;
//         }
//         int ret1=send(sid,&msg,sizeof(msg),0);

//         if(ret1==-1)
//         {
//             perror("发送数据出错\n");
//             return 0;
//         }
//         if(strcmp(msg.msgment,"quit")==0)
//         {
//             pthread_cancel(gpt);
//             return 0;
//         }
//     }
    
// }

// int singlefun(int sid)
// {
//     printf("----------私聊界面-----------\n");
//     pthread_create(&spt,NULL,pthsingle,&sid);
//     printf("请输入你要私聊的用户名：\n");
//     char buf[128];
//     scanf("%s",buf);

//     msg.msgtype=ifuser;
//     memcpy(msg.msgment,&buf,sizeof(buf));

//     int ret=send(sid,&msg,sizeof(msg),0);
//     if(ret==-1)
//     {
//         perror("发送数据出错\n");
//         return 0;
//     }
//     pthread_cancel(spt);
    
//     ret=recv(sid,&msg,sizeof(msg),0);
//     if(ret==-1)
//     {
//         perror("接受数据出错\n");
//         return 0;
//     }

//     pthread_create(&spt,NULL,pthsingle,&sid);
//     // printf("接受消息类型：%d",msg.msgtype);
//     if(msg.msgtype == ok)
//     {
//         printf("可以开始聊天了：\n");
//         CHATINFO chatmsg;
//         strcpy(chatmsg.myname,curname);
//         strcpy(chatmsg.yourname,buf);
//         while(1)
//         {
//             printf("请输入聊天消息：\n");
//             scanf("%s",chatmsg.text);

//             msg.msgtype=singlechat;
//             memcpy(msg.msgment,&chatmsg,sizeof(CHATINFO));

//             ret=send(sid,&msg,sizeof(msg),0);
//             if(ret==-1)
//             {
//                 perror("发送数据出错\n");
//                 return 0;
//             }
//             if(strcmp(msg.msgment,"quit")==0)
//             {
//                 pthread_cancel(spt);
//                 return 0;
//             }
//         }
        
//     } 
//     else if(msg.msgtype == nok)
//     {
//         printf("未找到用户：%s !\n",buf);
//     } 
// }