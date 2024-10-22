#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<iomanip>

using namespace std;

#define N 10  //������
int wait_total_time; //�ܵȴ�ʱ��
int currentTime=0;  //��ǰʱ��

struct PCB {
	int pid;  //���̱�ʶ��
	char state;  //��r��: ����״̬����w��:����״̬
	int priority;  //�������ȼ�
	int neededTime;  //������Ҫ������ʱ��
	int totalWaitTime;  //�����ۼ��ѵȴ���CPUʱ��
	struct PCB* next;  //ָ����һ��PCB������ָ��
	int arrivalTime;  //����ʱ��
};


//��������
struct PCB* createProcess(int id)
{
	struct PCB* p = new PCB;
	p->pid = id;
	p->neededTime = rand() % 41 + 10; //�������10-50֮�������
	p->arrivalTime = rand() % 20 + 1; //�������1-20֮�������
	p->state = 'w';
	p->priority = 1;
	p->totalWaitTime = 0;
	p->next = NULL;
	cout << "���� " << id <<  " �����ɹ���" << "��Ҫ����ʱ��:" << p->neededTime << "s������ʱ��:" << p->arrivalTime << "s" << endl;
	return p;
}


void SJF(struct PCB* head)
{
	struct PCB* p = head->next;  //ָ������е�һ��δִ�н��̵�ָ��
	struct PCB* temp = NULL;  //ָ�����絽��Ľ��̵�ָ��
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
		cout << "���� " << temp->pid << " ��ʼִ�У�" << "��ǰʱ��:" << currentTime << "s, �ۼƵȴ�ʱ��:" << temp->totalWaitTime << "s" << endl;
		currentTime += temp->neededTime;  //���µ�ǰʱ��
	}

	while (p != NULL)
	{
		for (struct PCB* q = p; q != NULL; q = q->next)
		{
			if (currentTime >= q->arrivalTime && q->state == 'w')  //�ҵ���һ��δ��ִ��������ʱ����̵��ѵ������
			{
				q->state = 'r';
				q->totalWaitTime = currentTime - q->arrivalTime;
				wait_total_time += q->totalWaitTime;  //��¼�ܵȴ�ʱ��
				cout << "���� " << q->pid << " ��ʼִ�У�" << "��ǰʱ��:" << currentTime << "s, �ۼƵȴ�ʱ��:" << q->totalWaitTime << "s" << endl;
				currentTime += q->neededTime;  //���µ�ǰʱ��
				break;
			}
		}
		if (p->state == 'r')
			p = p->next;
	}
}

int main()
{
	srand((unsigned)time(NULL));  //�������������
	wait_total_time = 0;  //��¼�ܵȴ�ʱ��
	struct PCB* head = new PCB;  //�����������ͷ��㣬������ʱ���С��������
	head->next = NULL;
	cout << "---------------   ���̴�����ʼ  ----------------" << endl;
	for (int i = 0; i < N; i++)
	{
		struct PCB* tmp = createProcess(i);
		if (tmp != NULL)
		{
			struct PCB* p = head;
			//������ʱ���С������������򰴵���ʱ���С��������
			while(p->next != NULL && (tmp->neededTime > p->next->neededTime || tmp->neededTime == p->next->neededTime && tmp->arrivalTime >= p->next->arrivalTime))
			{
				p = p->next;
			}
			tmp->next = p->next;
			p->next = tmp;   //���뵽������
		}
	}
	cout << "---------------   ���̵��ȿ�ʼ  ----------------" << endl;
	SJF(head); //ִ��SJF�����㷨
	cout << "��ƽ���ȴ�ʱ��:" << static_cast<float>(wait_total_time / N) << "s" << endl;
	//�ͷ��ڴ�
	while (head != NULL)
	{
		struct PCB* p = head->next;
		delete head;
		head = p;
	}
	return 0;
}