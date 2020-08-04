
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define max 50

int Allocation[max][max];
int Available[max];
int resources[max];
int Max[max][max];
int Need[max][max];
int resourcesSize;//资源总数
int processSize;//进程总数
bool isSecurity;//系统是否安全
bool checkSecurity();

void initialize()
{

    printf("请输入资源总数\n");
    scanf("%d",&resourcesSize);
    printf("输入各个资源的数量：\n");
    for(int i = 0;i<resourcesSize;i++)
    {
        printf("资源%d=",i);
        scanf("%d",&resources[i]);
    }
    printf("输入进程数量\n");
    scanf("%d",&processSize);
    printf("输入每个进程对资源的最大需求\n");
    for(int i = 0;i<processSize;i++)
    {
        for(int j = 0;j<resourcesSize;j++)
        {
            printf("进程%d对资源%d的最大需求=",i,j);
            scanf("%d",&Max[i][j]);
        }
    }
    printf("对每个进程的资源已分配情况：\n");
    for(int i = 0;i<processSize;i++)
    {
        for(int j = 0;j<resourcesSize;j++)
        {
            printf("已对进程%d分配资源%d的数量=",i,j);
            scanf("%d",&Allocation[i][j]);
        }
    }
    for(int i = 0;i<processSize;i++)
    {
        for(int j = 0;j<resourcesSize;j++)
        {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
    for(int j = 0;j<resourcesSize;j++)
    {

        for(int i = 0;i<processSize;i++)
        {
            resources[j] -= Allocation[i][j]; //分配完之后剩下的资源总数
        }
        Available[j] = resources[j];//可用资源数
    }
    checkSecurity();

}
bool checkSecurity()
{
    bool security[processSize];//security[i]表示进程i是否安全
    int securityQueue[processSize];//表示安全队列，将进程的id号加进去
    memset(security,false,processSize); //初始化为不安全的。
    int lastNoSecurity = processSize; //初始化上次不安全进程数为processSize
    int temp[max]; //避免检测时对Available数组进行修改
    for(int i = 0;i<resourcesSize;i++)
    {
        temp[i] = Available[i];
    }
    int k = 0;//添加安全进程到securityQueue中。计数的作用
    while(true)
    {
        int currentNoSecurity = 0;//每次检测都currentNoSecurity都从0开始计数，表示当前不安全的进程数
        int count;
        for(int i = 0;i<processSize;i++)
        {
            count = 0;
            if(security[i]==true) continue; //跳过安全的进程
            for(int j = 0;j<resourcesSize;j++)
            {
                if(Need[i][j]<=temp[j]) //资源数满足进程i对它的需求
                        count++;
                else
                    break;
            }
            if(count==resourcesSize)//所有可利用的资源全部满足进程i+1的需求,将此进程加入安全队列中
            {
                security[i] = true; //进程i是安全的
                //将进程i+1已分配的资源偿还
                printf("进程%d是安全的哦,资源可利用更新为：\n",i);
                for(int j = 0 ;j<resourcesSize;j++)
                {
                    temp[j]+=Allocation[i][j];
                    printf("%d ",temp[j]);
                }
                printf("\n");
                securityQueue[k++] = i;
            }
            //当不满足时，暂时说明此进程是不安全的，当前不安全进程数+1
            else
            {
                currentNoSecurity++;
                printf("进程%d现在还不安全\n",i);
            }
        }
        //检测一遍后，下面判断检测的结果
        if(currentNoSecurity==0) //检测完毕，所有进程都已加入安全队列中。系统可以进行资源分配了。
        {
            printf("当前系统是安全的，可以对进程进行资源分配，安全队列如下：\n");
            for(int i = 0;i< k;i++)
            {
                printf("%d ",securityQueue[i]);
            }
            printf("\n");
            return true;
        }
        //当下检测不是安全队列的进程数如果跟上次一样，就说明扫一遍后找不出符合安全性的进程了。
        if(currentNoSecurity==lastNoSecurity)
        {
            //不安全
            printf("当前系统是不安全的，不能进行资源的分配！");
            return false;
        }
        //至少有一个加入了安全队列中了，所以再扫一遍进程队列，再检测一遍。
        else{
            lastNoSecurity = currentNoSecurity;
        }

    }
    return false;
}
bool answerQuest()
{
    int Request[resourcesSize];
    printf("输入哪个进程的资源请求？\n");
    int process;
    scanf("%d",&process);
    for(int j = 0;j<resourcesSize;j++)
    {
        printf("输入进程%d对资源%d的请求=",process,j);
        scanf("%d",&Request[j]);
    }

    //有多少我就请求多少，这貌似合理，系统也是安全的。但是这是不对的！不能多于最大请求
    for(int j = 0;j<resourcesSize;j++)
    {
        if(Request[j] + Allocation[process][j] > Max[process][j]){
            printf("进程%d对资源%d的请求不合理，超出所宣布的最大值！",process,j);
            return false;
        }
    }
    //没有大于最大请求，但是当前系统可能还没有这么资源分配他们
    for(int j = 0;j<resourcesSize;j++)
    {
            if(Request[j] > Available[j])
            {
                printf("系统还没有足够的资源%d响应进程%d的请求",j,process);
            }
    }
    //尝试分配
    printf("尝试分配资源\n");
    for(int j = 0;j<resourcesSize;j++)
    {
        Allocation[process][j]+=Request[j];
        Need[process][j]-=Request[j];
    }
    for(int j = 0;j<resourcesSize;j++)
    {
        Available[j]-=Request[j];
    }
    if(!checkSecurity()){//如果系统不处于安全状态。则本次分配作废，恢复原来的资源分配状态
        printf("分配作废，恢复原来的状态\n");
            for(int j = 0;j<resourcesSize;j++)
            {

                Allocation[process][j] -= Request[j];
                Need[process][j] += Request[j];
            }
            for(int j = 0;j<resourcesSize;j++)
                {
                        Available[j] += Request[j];
                }
            return false;
    }
    return true;

}
void look()
{
    printf("最大资源需求：\n");
    for(int i = 0;i<processSize;i++)
    {
        for(int j = 0;j<resourcesSize;j++)
        {
            printf("%d\t",Max[i][j]);
        }
        printf("\n");
    }
    printf("已分配资源：\n");
    for(int i = 0;i<processSize;i++)
    {
        for(int j = 0;j<resourcesSize;j++)
        {
            printf("%d\t",Allocation[i][j]);
        }
        printf("\n");
    }
    printf("Need需求：\n");
    for(int i = 0;i<processSize;i++)
    {
        for(int j = 0;j<resourcesSize;j++)
        {
            printf("%d\t",Need[i][j]);
        }
        printf("\n");
    }
    printf("现有资源：\n");
    for(int j = 0;j<resourcesSize;j++)
    {
        printf("%d\t",Available[j]);
    }
    printf("\n");

}
void menu()
{
    printf("\n\n\t1.继续对进程进行资源的分配？\n");
    printf("\t2.查看资源情况\n");
    printf("\t3.exit\n");
}
int main()
{
    printf( "\n\n|-------------------------------------------------------|\n" );
	printf( "|****************** 银行家算法模拟实验 *****************|\n" );
	printf( "|-------------------------------------------------------|\n" );
    printf("\n\t1.请输入系统的初始状态\n");
    initialize();
    int choose;
    while(1)
    {
        menu();
        scanf("%d",&choose);
        switch(choose)
        {
            case 1:
                if(answerQuest())
                    printf("请求成功！");
                else
                    printf("请求失败！");
                    break;
            case 2:look();break;
            case 3:
                printf("再见！");
                exit(0);
        }
    }
    return 0;
}