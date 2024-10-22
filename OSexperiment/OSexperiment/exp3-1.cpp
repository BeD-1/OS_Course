#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<iomanip>

using namespace std;

#define N 10  //进程数
int wait_total_time; //总等待时间
int currentTime=0;  //当前时间

struct PCB {
	int pid;  //进程标识符
	char state;  //‘r’: 运行状态；‘w’:就绪状态
	int priority;  //进程优先级
	int neededTime;  //进程需要的运行时间
	int totalWaitTime;  //进程累计已等待的CPU时间
	struct PCB* next;  //指向下一个PCB的链表指针
	int arrivalTime;  //到达时间
};


//创建进程
struct PCB* createProcess(int id)
{
	struct PCB* p = new PCB;
	p->pid = id;
	p->neededTime = rand() % 41 + 10; //随机生成10-50之间的整数
	p->arrivalTime = rand() % 20 + 1; //随机生成1-20之间的整数
	p->state = 'w';
	p->priority = 1;
	p->totalWaitTime = 0;
	p->next = NULL;
	cout << "进程 " << id <<  " 创建成功！" << "需要运行时间:" << p->neededTime << "s，到达时间:" << p->arrivalTime << "s" << endl;
	return p;
}


void SJF(struct PCB* head)
{
	struct PCB* p = head->next;  //指向队列中第一个未执行进程的指针
	struct PCB* temp = NULL;  //指向最早到达的进程的指针
	int early = 100000;

	for (struct PCB* t = p; t != NULL; t = t->next)
	{
		if (t->arrivalTime < early)
		{
			temp = t;
			early = t->arrivalTime;
		}
	}
	if (temp != NULL)
	{
		currentTime = temp->arrivalTime;
		temp->state = 'r';
		temp->totalWaitTime = 0;
		cout << "进程 " << temp->pid << " 开始执行！" << "当前时间:" << currentTime << "s, 累计等待时间:" << temp->totalWaitTime << "s" << endl;
		currentTime += temp->neededTime;  //更新当前时间
	}

	while (p != NULL)
	{
		for (struct PCB* q = p; q != NULL; q = q->next)
		{
			if (currentTime >= q->arrivalTime && q->state == 'w')  //找到第一个未被执行且运行时间最短的已到达进程
			{
				q->state = 'r';
				q->totalWaitTime = currentTime - q->arrivalTime;
				wait_total_time += q->totalWaitTime;  //记录总等待时间
				cout << "进程 " << q->pid << " 开始执行！" << "当前时间:" << currentTime << "s, 累计等待时间:" << q->totalWaitTime << "s" << endl;
				currentTime += q->neededTime;  //更新当前时间
				break;
			}
		}
		if (p->state == 'r')
			p = p->next;
	}
}

int main()
{
	srand((unsigned)time(NULL));  //设置随机数种子
	wait_total_time = 0;  //记录总等待时间
	struct PCB* head = new PCB;  //进程排序队列头结点，按运行时间从小到大排序
	head->next = NULL;
	cout << "---------------   进程创建开始  ----------------" << endl;
	for (int i = 0; i < N; i++)
	{
		struct PCB* tmp = createProcess(i);
		if (tmp != NULL)
		{
			struct PCB* p = head;
			//按运行时间从小到大排序，相等则按到达时间从小到大排序
			while(p->next != NULL && (tmp->neededTime > p->next->neededTime || tmp->neededTime == p->next->neededTime && tmp->arrivalTime >= p->next->arrivalTime))
			{
				p = p->next;
			}
			tmp->next = p->next;
			p->next = tmp;   //插入到队列中
		}
	}
	cout << "---------------   进程调度开始  ----------------" << endl;
	SJF(head); //执行SJF调度算法
	cout << "总平均等待时间:" << static_cast<float>(wait_total_time / N) << "s" << endl;
	//释放内存
	while (head != NULL)
	{
		struct PCB* p = head->next;
		delete head;
		head = p;
	}
	return 0;
}