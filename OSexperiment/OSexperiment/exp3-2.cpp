#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>

using namespace std;

#define N 10  //进程数
int wait_total_time;  //等待时间总和
int currentTime = 0;  //当前时间

struct PCB {
	int pid;  //进程标识符
	char state;  //‘r’: 运行状态；‘w’:就绪状态
	int priority;  //进程优先级=(等待时间+要求服务时间)/要求服务时间
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
	p->totalWaitTime = 0;
	p->state = 'w';
	p->priority = 1;
	p->next = NULL;
	cout << "进程 " << id << " 创建成功！" << "需要运行时间:" << p->neededTime
		<< "s，到达时间:" << p->arrivalTime << "s" << endl;
	return p;
}

//更新优先级
void updatePriority(struct PCB* head)
{
	while (head->next != nullptr)
	{
		if (head->next->state == 'w') //如果进程处于就绪状态,更新优先级
		{
			head->next->totalWaitTime = currentTime - head->next->arrivalTime;
			if (head->next->neededTime > 0)
			{
				head->next->priority = (head->next->totalWaitTime + head->next->neededTime) / head->next->neededTime;//计算优先级
			}
		}
		head = head->next;
	}
}

//HRRN调度算法
void HRRN(struct PCB* head)
{
	struct PCB* temp = NULL;  //指向最早到达的进程的指针
	int early = 100000;
	int cnt = N;  //记录未执行的进程数

	//找到最早到达的进程
	for (struct PCB* t = head->next; t != NULL; t = t->next)
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

	while (cnt > 0)
	{
		updatePriority(head);  //更新优先级
		int min_priority = -1; 
		struct PCB* q = NULL;  //指向优先级最高的进程的指针
		for (struct PCB* p = head->next; p != NULL; p = p->next)
		{
			if (min_priority < p->priority && p->state == 'w')//找到优先级最高且处于就绪状态的进程
			{
				min_priority = p->priority;
				q = p;
			}
		}
		if (q != NULL)
		{
			q->state = 'r';
			q->totalWaitTime = currentTime - q->arrivalTime;
			wait_total_time += q->totalWaitTime;  //记录总等待时间
			cout << "进程 " << q->pid << " 开始执行！" << "当前时间:" << currentTime << "s, 累计等待时间:" << q->totalWaitTime << "s" << endl;
			currentTime += q->neededTime;  //更新当前时间
		}
		cnt--;
	}
}

int main()
{
	srand((unsigned)time(NULL));  //设置随机数种子
	wait_total_time = 0;  //记录总等待时间
	struct PCB* head = new PCB;  //进程排序队列头指针，按运行时间从小到大排序
	head->next = NULL;
	cout << "---------------   进程创建开始  ----------------" << endl;
	for (int i = 0; i < N; i++)
	{
		struct PCB* tmp = createProcess(i);
		tmp->next = head->next;  // 将新进程插入到链表头部
		head->next = tmp;
	}
	cout << "---------------   进程调度开始  ----------------" << endl;
	HRRN(head);
	cout << "所有进程执行完毕！"<< endl <<"总平均等待时间:" << static_cast<float>(wait_total_time) / N << "s" << endl;
	//释放内存
	while (head != nullptr) {
		PCB* p = head->next;
		delete head;
		head = p;
	}
	return 0;
}