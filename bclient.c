#include "aclihead.h"

//定义客户端地址
struct sockaddr_in clientsin;
//客户端套接字
int sid=0;

void initclisock()
{
    //AF_INET:IPV4
    clientsin.sin_family=AF_INET;
    //端口通过htons()函数转为网络字节序
    clientsin.sin_port=htons(10002);

    //把字符串ip地址转为网络字节序ip
    unsigned int ip;
    char bdip[]="192.168.50.222";
    inet_pton(AF_INET,bdip,&ip);
    clientsin.sin_addr.s_addr=ip;

    //创建套接字
    //SOCK_STREAM代表是TCP，SOCK_DGRAM代表UDP
    sid=socket(AF_INET,SOCK_STREAM,0);
}

int welcomepage()
{
    printf("---------欢迎界面---------\n");
    char buf[128];
    while (1)
    {
        printf("请选择您要的操作：\n1.登陆\n2.注册\n3.退出\n");
        scanf("%s",buf);
        //登陆
        if(strcmp(buf,"1")==0)
        {
            loginpage(sid);
        }
        //注册
        else if(strcmp(buf,"2")==0)
        {
            registfun(sid);
        }
        //退出
        else if(strcmp(buf,"3")==0)
        {
            return 0;
        }
        else
        {
            printf("输入错误,请重新输入：\n");
        }
    }
}

int main()
{
    //初始化
    initclisock();

    //连接服务器
    int ret=-1;
    while(ret==-1)
    {
        ret=connect(sid,(struct sockaddr *)&clientsin,sizeof(clientsin));
        printf("服务器链接中.....\n");
        sleep(1);
    }

    //初始界面
    welcomepage();
    // rootpage(sid);
    // workpage(sid);
    // workerroom(sid);

    //关闭套接字
    close(sid);
    return 0;
}

//初始化客户端地址

