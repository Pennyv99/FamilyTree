//
//  header.h
//  familytree
//
//  Created by 王潘怡 on 2023/2/25.
//
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#define MAXSIZE 10000

typedef struct FTree
{
    char name[MAXSIZE];  // 姓名
    int level = 0;  //  辈分b **TODO 王潘怡 不确定要不要加这个属性
    struct FTree *brother; // 兄弟指针
    struct FTree *child;   // 孩子指针
}FMember;
FTree *St[MAXSIZE];
int top=-1;
// 读入家谱
void ReadFile(char str[])
{
    FILE *fp;
    if((fp = fopen("familytree.txt","r"))==NULL)
    {
        cout << "Can't open file!"<<endl;
        exit(0);
    }
    fgets(str,MAXSIZE,fp);
    fclose(fp);
}

//创建家树
void CreateFTree(FMember* &b,char *str)
{
        FTree *p=NULL;
    int k,j=0,startName = 0,lengthName = 0,i = 0,m = 0;//名字在字符串中的起始位置及长度
        char ch,NAME[MAXSIZE];
        b=NULL;                         //建立的二叉树初始时为空
        ch=str[j];
        while (ch!='\0')      //str未扫描完时循环
        {
            switch(ch)
            {
                case '(':
                    k=1; break;        //为孩子节点
                case ')':
                        while(top>=1 && St[top] != St[top-1]->child)
                        {
                            top--;
                        }
                        top--;                       //将当前括号对包括的节点从栈中移去
                        break;
                case ',':k=2; break;                              //为兄弟节点
                default:
                    p=(FTree *)malloc(sizeof(FTree));
                    top++;
                    St[top]=p;
                    startName = j;lengthName = 1;j++;ch = str[j];   //从字符串中读取姓名
                    while (ch!='(' && ch!=')' && ch!=','  && ch!='\0')
                    {
                        lengthName++;
                        j++;
                        ch = str[j];
                    }
                    j--;
                    m = 0;
                    for(i = startName;i<startName+lengthName;i++)
                    {
                        NAME[m] = str[i];m++;
                    }
                    NAME[m] = '\0';
                    strcpy(p->name,NAME);
                    p->child=p->brother=NULL;
                    if (b==NULL)                             //*p为二叉树的根节点
                        b=p;
                    else                                  //已建立二叉树根节点
                    {
                        switch(k)
                        {
                        case 1:St[top-1]->child=p;break;
                        case 2:St[top-1]->brother=p;break;
                        }
                    }
            }
            j++;
            ch=str[j];
        }
    top=-1;
}

//输出家树
void OutputFTree(FMember* b)
{
    FMember *fp = b;
    if(fp!=NULL)
    {
        top++;St[top] = fp;     //父入栈
        fp = b->child;
        if(fp!=NULL)            //只有在有孩子的情况下需要输出一行
        {   cout<<St[top]->name;
            cout<<'(';
            cout<<fp->name;
            top++;St[top] = fp; //第一个孩子入栈
            fp = St[top]->brother;
            while(fp!=NULL)
            {
            cout<<',';
            cout<<fp->name;
            fp = fp->brother;
            }
            cout<<')'<<endl;
            top--;              //第一个孩子出栈
        }
        fp = St[top]->brother;OutputFTree(fp);
        fp = St[top]->child;OutputFTree(fp);
        top--;                  //父出栈
    }
    
}

//求出家树的高度
int GetHeightFTree(FMember *b)
{
    if(b == NULL)
        return 0;
    int leftheight = GetHeightFTree(b->child);
    int rightheight = GetHeightFTree(b->brother);
    return leftheight >= rightheight? leftheight+1:rightheight;
    
}

int CountSum(FMember *b)
{
    if(b == NULL)
        return 0;
    return CountSum(b->brother)+1;
} 

// 统计指定辈分的人数并显示
int count1 = 0, level[MAXSIZE] = {0};
void OutputHeightNumber(FMember *b , int height)
{
    FMember *fp = b;    //保存传过来的b
    while(b != NULL)    //父节点存在
    {
        OutputHeightNumber(b->child, height-1); b = b->brother;
    }
    level[height] = level[height]+CountSum(fp);
}

void FindLevelNum(FMember *b, int height, int target)
// height为b节点辈分，target为目标辈分.target = 1,表示叶子结点，家谱辈分最小的人
{
    OutputHeightNumber(b, height);  //  统计所有辈分人数
    cout<<"指定辈分为第"<<GetHeightFTree(b)-target+1<<"代人数为："<<level[target]<<endl;
}

// 查找指定人，并显示其祖先和所有后代
//TODO 王潘怡 万一有重名的怎么办；没有找到将什么都不显示;祖先是指父亲还是包括比父亲辈分高的所有人
void OutputPerson(FMember *b, char target[])
{
    FMember *fp = b;     // 保存父结点
    FMember *fb;        // 保存当前结点
    b = fp->child;
    while(b!=NULL && strcmp(b->name, target) != 0)     // 从父结点的孩子中找指定人
    {
        b = b->brother;
    }
    if(b != NULL && !strcmp(b->name, target))       // 找到了，输出指定人的父亲和所有孩子
    {
        fb = b;
        cout<<"找到了！"<<b->name<<"的父亲为"<<fp->name<<" 后代有:";
        fb = b->child;
        while(fb!=NULL)
        {
            cout<<fb->name<<" ";fb = fb->brother;
        }
        cout<<endl;
    }
    else
    {
        if(fp->brother != NULL)OutputPerson(fp->brother, target);  //  没有找到，从父结点的兄弟那找
        if(fp->child != NULL)OutputPerson(fp->child, target);      //  没有找到，从父结点的孩子往下找
    }
}

// 求出两个人最近的祖先
