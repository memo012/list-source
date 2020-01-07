#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/******************************先来先服务********************************/
char b;
typedef struct JCB      				//定义进程控制块
{
	char ID[16];   						//进程号
	char name[16];   					//进程名
	char state;   						//运行状态
	int  arrivetime;  					//到达时间
	int  starttime;  					//进程开始时间
	int  finishtime;  					//进程结束时间
	int  servicetime;   				//服务时间
	float turnaroundtime;				//周转时间
	float weightedturnaroundtime;		//带权周转时间
	struct FCFS *next;  				//指向下个进程
}JCB;

/**
 * 创建进程
 * */
int getId(){
        int pid;
        int pids;
        pid = fork();
        if(pid < 0){
                /* error occurred */
                 printf("Fork Failed!");
                 exit(-1);
        }else{
                pids = getpid();
        }
        return pids;
}



int time;   							//计时器
int n;    								//进程个数
JCB *head=NULL,*p,*q; 					//进程链表指针

void run_fcfs(JCB *p1) 				//运行未完成的进程
{
	time = p1->arrivetime > time? p1->arrivetime:time;
	p1->starttime=time;
	printf("\n现在时间是%d,开始运行作业%s\n",time,p1->name);
	time+=p1->servicetime;
	p1->state='T';
	p1->finishtime=time;
	p1->turnaroundtime=p1->finishtime-p1->arrivetime;
	p1->weightedturnaroundtime=p1->turnaroundtime/p1->servicetime;
	printf("ID  到达时间 开始时间 服务时间 完成时间 周转时间 带权周转时间 \n");
	printf("%s%6d%10d%10d%8d%10.1f%10.2f\n",
		   p1->ID,p1->arrivetime,p1->starttime,p1->servicetime,p1->finishtime,
	       p1->turnaroundtime,p1->weightedturnaroundtime);
}


void run() 								//找到当前未完成的进程
{
	int i,j;
	p=head;
	for(i=0;i<n;i++){
		if(p->state=='F'){
			q=p;    					//标记当前未完成的进程
			run_fcfs(q);
		}
		p=p->next;
	}
	scanf("%c",&b);
	show_select();
}

void fcfs()          					//获得进程信息并创建进程
{
	printf("\n******先来先服务算法模拟******\n");
	int num;
	printf("\n请输入作业个数:");
	scanf("%d",&n);
	for(num=0;num<n;num++){
		p=(JCB *)malloc(sizeof(JCB));
		printf("依次输入：\nID 进程名 到达时间 服务时间\n");
		char string[16] = {0};
		int pid=fork();
       		 // 这是异常情况
       		 if (pid==-1)
       		 {
       		     perror("fork失败!");
        	    exit(1);
       		 }
       		 //循环中，fork函数调用五次，子进程返回0,父进程返回子进程的pid，
       		 //为了避免子进程也fork，需要判断并break
       		 if (pid==0)
       		 {
           		 break;
       		 }else if(pid > 0){
        	        sleep(1);// 延迟父进程程序，等子进程先执行完。
	        }
		sprintf(string, "%d", pid);
		strcpy( p->ID, string);
		strcpy( p->name, string);
		printf("%s\t", string);
		printf("%s\t", string);
		getchar();
		scanf("%d\t%d",&p->arrivetime,&p->servicetime);
		printf("\n");
		if(head==NULL) {head=p;q=p;time=p->arrivetime;}
		if(p->arrivetime < time) time=p->arrivetime;
		q->next=p;
		p->starttime=0;
		p->finishtime=0;
		p->turnaroundtime=0;
		p->weightedturnaroundtime=0;
		p->next=NULL;
		p->state='F';
		q=p;
	}
	if(num == n){
	       	printf("按先来先服务算法调度运行结果如下：\n");
  	     	 p=head;
       		 run();
	}else{
		exit(-1);
	}
//	printf("按先来先服务算法调度运行结果如下：\n");
//	p=head;
//	run();
}
/*******************************先来先服务over*****************************/








/*******************************短作业优先********************************/
/*函数声明*/
struct SJF *creatlist();
void insert(struct JCB *head,struct JCB *s);
struct JCB *searchByAT(struct JCB *head,int AT);

void run1(struct JCB *head);

void del(struct JCB *p);
int getcount(struct JCB *head,int time);

struct SJF  *creatlist()  				/*创建链表，按照进程的到达时间排列*/
{
	int i,MAX;							/*MAX表示进程数*/
	struct JCB *s;
	struct JCB *head=(struct JCB *)malloc(sizeof(struct JCB));
	head->next=NULL;
	printf("\n请输入作业个数:");
	scanf("%d",&MAX);
	for(i=0;i<MAX;i++){
		s=(struct JCB *)malloc(sizeof(struct JCB));
		printf("依次输入：\nID 进程名 到达时间 服务时间\n");
		scanf("%s\t   %s\t    %d\t     %d",&(s->ID),&(s->name),&(s->arrivetime),&(s->servicetime));
		printf("\n");
		s->next=NULL;
		insert(head,s);
	}
	return head;
}

void insert(struct JCB *head,struct JCB *s) 	 /*插入结点*/
{
	struct JCB *p=searchByAT(head,s->arrivetime);
    s->next=p->next;
    p->next=s;
    return;
}

struct JCB *searchByAT(struct JCB *head,int AT)  /*查找第一个到达时间大于等于AT的节点，返回其前一个指针*/
{
	struct JCB *p,*q;
    p=head;
    q=head->next;
    while(q!=NULL&&q->arrivetime<=AT){
		p=q;
		q=q->next;
    }
    return p;
}


void del(struct JCB *p)  						/*删除P的下一个节点*/
{
	struct JCB *tmp;
	tmp=p->next;
	p->next=tmp->next;
	free(tmp);
	return;
}

int getcount(struct JCB *head,int time)  		/*查看当前就绪队列中的进程数*/
{
	int count=0;
	struct JCB *p,*q;
	p=head;
	q=p->next;
	while(q!=NULL&&q->arrivetime<=time){
		count++;
		p=q;
		q=q->next;
	}
	return count;
}


struct JCB *SJF(struct JCB *head,int count) 	/*在头结点后的count个节点中选择servicetime最小的，返回其前一个节点的指针*/
{
	int min;
	struct JCB *p,*q,*flag;
	p=head;
	q=p->next;
	min=q->servicetime;
	flag=p;    									/*flag记录返回指针*/
	while(count>0){
		if(q->servicetime<min){
			min=q->servicetime;
			flag=p;
		}
		count--;
		p=q;
		q=q->next;
	}
	return flag;
}

void run1(struct JCB *head) 					/*按短作业优先算法调度进程，并输出其运行情况*/
{
	int time=0;
	int count;
	struct JCB *s,*t;
	while(head->next!=NULL){
		count=getcount(head,time);
		if(count==0)    						/*如果当前就绪队列中没有进程，时间自增*/
			time++;
		else if(count==1) 							/*如果就绪队列中只有1个进程，则必定是第一个节点*/
		{
			t=head;
			s=t->next;
			s->starttime=time;
			printf("\n现在时间是%d,开始运行作业%s\n",time,s->name);
			time+=s->servicetime;
			s->finishtime=time;
			s->turnaroundtime=s->finishtime-s->arrivetime;
			s->weightedturnaroundtime=s->turnaroundtime/s->servicetime;
			printf("ID  到达时间 开始时间 服务时间 完成时间 周转时间 带权周转时间 \n");
			printf("%s%6d%10d%10d%8d%10.1f%10.2f\n", s->ID,s->arrivetime,s->starttime,
			s->servicetime,s->finishtime,s->turnaroundtime,s->weightedturnaroundtime);
			del(t);
		}
		else    /*如果就绪队列中的进程数>=2，则在head后的count个节点中进行短作业优先调度*/
		{
			t=SJF(head,count);
			s=t->next;
			s->starttime=time;
			printf("\n现在时间是%d,开始运行作业%s\n",time,s->name);
			time+=s->servicetime;
			s->finishtime=time;
			s->turnaroundtime=s->finishtime-s->arrivetime;
			s->weightedturnaroundtime=s->turnaroundtime/s->servicetime;
			printf("ID  到达时间 开始时间 服务时间 完成时间 周转时间 带权周转时间 \n");
			printf("%s%6d%10d%10d%8d%10.1f%10.2f\n", s->ID,s->arrivetime,s->starttime,
			s->servicetime,s->finishtime,s->turnaroundtime,s->weightedturnaroundtime);
			del(t);
		}
	}
	scanf("%c",&b);
	show_select();
}

void sjf(){
    printf("\n******短作业算法模拟******\n");
	struct SJF *head;
	head=creatlist();
	printf("按短作业优先算法调度运行结果如下:\n");
	run1(head);
}
/******************************短作业优先over*******************************/


/******************************最高响应比***********************************/
struct JCB a[100];

void input(struct JCB *p,int N){
	int i;
	for(i=0;i<=N-1;i++){
		printf("依次输入：\nID 进程名 到达时间 服务时间\n");
		scanf("%s\t%s\t%d\t%d",&p[i].ID,&p[i].name,&p[i].arrivetime,&p[i].servicetime);
		printf("\n");
	}
}

void Print(struct JCB *p,int arrivetime,int servicetime,int starttime,
		   int finishtime,float turnaroundtime,float weightedturnaroundtime,int N){
	int k;
    printf("按最高响应比优先算法调度运行结果如下：\n");
    for(k=0;k<=N-1;k++){
		printf("\n现在时间是%d,开始运行作业%s\n",p[k].starttime,p[k].name);
		printf("ID  到达时间 开始时间 服务时间 完成时间 周转时间 带权周转时间 \n");
        printf("%s%6d%10d%10d%8d%10.1f%10.2f\n",p[k].ID,p[k].arrivetime,p[k].starttime,
		p[k].servicetime,p[k].finishtime,p[k].turnaroundtime,p[k].weightedturnaroundtime);
	}
}
//按到达时间排序
void sort(struct JCB *p,int N)
{
	int i,j;
    for(i=0;i<=N-1;i++)
        for(j=0;j<=i;j++)
            if(p[i].arrivetime<p[j].arrivetime){
                struct JCB temp;
                temp=p[i];
                p[i]=p[j];
                p[j]=temp;
            }
}
//yun xing jieduan
void deal(struct JCB *p,int arrivetime,int servicetime,
		  int starttime,int finishtime,float turnaroundtime,float weightedturnaroundtime,int N){
	int k;
    for(k=0;k<=N-1;k++){
        if(k==0){
			p[k].starttime=p[k].arrivetime;
            p[k].finishtime=p[k].arrivetime+p[k].servicetime;
		}
		else{
			p[k].starttime=p[k-1].finishtime;
			p[k].finishtime=p[k-1].finishtime+p[k].servicetime;
		}
	}
    for(k=0;k<=N-1;k++){
		p[k].turnaroundtime=p[k].finishtime-p[k].arrivetime;
		p[k].weightedturnaroundtime=p[k].turnaroundtime/p[k].servicetime;
    }
}

void ZGXYB(struct JCB *p,int N)
{
    int arrivetime=0,servicetime=0,starttime=0,finishtime=0;
	float turnaroundtime=0,weightedturnaroundtime=0;
    int m,i,n,follow,k;
    float max;
    struct JCB temp;
    sort(p,N);
    for(m=0;m<N-1;m++){
        if(m==0)
            p[m].finishtime=p[m].arrivetime+p[m].servicetime;
        else
            p[m].finishtime=p[m-1].finishtime+p[m].servicetime;
		i=0;
		for(n=m+1;n<=N-1;n++){
            if(p[n].arrivetime<=p[m].finishtime)
			i++;
        }
        max=(p[m].finishtime-p[m+1].arrivetime)/p[m+1].servicetime;
        follow=m+1;
        for(k=m+1;k<m+i;k++){
            if(max<=(p[m].finishtime-p[k+1].arrivetime)/p[k+1].servicetime){
                max=(p[m].finishtime-p[k+1].arrivetime)/p[k+1].servicetime;
                follow=k+1;
            }
        }
        temp=p[m+1];
        p[m+1]=p[follow];
        p[follow]=temp;
    }
    deal(p,arrivetime,servicetime,starttime,finishtime,turnaroundtime,weightedturnaroundtime,N);
    Print(p,arrivetime,servicetime,starttime,finishtime,turnaroundtime,weightedturnaroundtime,N);
	scanf("%c",&b);
	show_select();
}

void hrn()
{
	int N;
	struct HRN *c;
	printf("\n******最高响应比算法模拟******\n");
	printf("\n请输入作业个数:");
	scanf("%d",&N);
	input(a,N);
	c=a;
	ZGXYB(c,N);
}
/*****************************最高响应比over********************************/

void show_welcome()
{
    printf("\n");
    printf("\t\t\t*********************************\n");
    printf("\t\t\t*                               *\n");
    printf("\t\t\t*   欢迎来到进程调度模拟算法    *\n");
    printf("\t\t\t*                               *\n");
    printf("\t\t\t*                               *\n");
    printf("\t\t\t*         李泽轩   雷强         *\n");
    printf("\t\t\t*                               *\n");
    printf("\t\t\t*                               *\n");
	printf("\t\t\t*         王皓煊   王鹏         *\n");
	printf("\t\t\t*                               *\n");
    printf("\t\t\t*                               *\n");
    printf("\t\t\t*********************************\n");
}

void show_select()
{
	void sel();
        printf("\n\n");
		printf("\t\t********************请选择算法********************\n");
		printf("\t\t*                                                *\n");
        printf("\t\t*              1.先来先服务算法                  *\n");
        printf("\t\t*                                                *\n");
		printf("\t\t*              2.短作业优先算法                  *\n");
		printf("\t\t*                                                *\n");
        printf("\t\t*              3.最高相应比算法                  *\n");
		printf("\t\t*                                                *\n");
		printf("\t\t*              4.退出！                          *\n");
		printf("\t\t*                                                *\n");
		printf("\t\t**************************************************\n\n");
		sel();
}
void sel()
{
	char a;
    printf("请选择：");
	scanf("%c",&a);
    switch(a)
	{
        case '1':fcfs();
        case '2':sjf();
		case '3':hrn();
		case '4':{printf("谢谢使用！");exit(0);}
		default:{printf("输入有误，请重新输入!");show_select();}
	}
}

void main()
{
	show_welcome();
    show_select();
}
