#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>

using namespace std;

#define N 10  //������
int wait_total_time;  //�ȴ�ʱ���ܺ�
int currentTime = 0;  //��ǰʱ��

struct PCB {
	int pid;  //���̱�ʶ��
	char state;  //��r��: ����״̬����w��:����״̬
	int priority;  //�������ȼ�=(�ȴ�ʱ��+Ҫ�����ʱ��)/Ҫ�����ʱ��
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
	p->totalWaitTime = 0;
	p->state = 'w';
	p->priority = 1;
	p->next = NULL;
	cout << "���� " << id << " �����ɹ���" << "��Ҫ����ʱ��:" << p->neededTime
		<< "s������ʱ��:" << p->arrivalTime << "s" << endl;
	return p;
}

//�������ȼ�
void updatePriority(struct PCB* head)
{
	while (head->next != nullptr)
	{
		if (head->next->state == 'w') //������̴��ھ���״̬,�������ȼ�
		{
			head->next->totalWaitTime = currentTime - head->next->arrivalTime;
			if (head->next->neededTime > 0)
			{
				head->next->priority = (head->next->totalWaitTime + head->next->neededTime) / head->next->neededTime;//�������ȼ�
			}
		}
		head = head->next;
	}
}

//HRRN�����㷨
void HRRN(struct PCB* head)
{
	struct PCB* temp = NULL;  //ָ�����絽��Ľ��̵�ָ��
	int early = 100000;
	int cnt = N;  //��¼δִ�еĽ�����

	//�ҵ����絽��Ľ���
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
		cout << "���� " << temp->pid << " ��ʼִ�У�" << "��ǰʱ��:" << currentTime << "s, �ۼƵȴ�ʱ��:" << temp->totalWaitTime << "s" << endl;
		currentTime += temp->neededTime;  //���µ�ǰʱ��
	}

	while (cnt > 0)
	{
		updatePriority(head);  //�������ȼ�
		int min_priority = -1; 
		struct PCB* q = NULL;  //ָ�����ȼ���ߵĽ��̵�ָ��
		for (struct PCB* p = head->next; p != NULL; p = p->next)
		{
			if (min_priority < p->priority && p->state == 'w')//�ҵ����ȼ�����Ҵ��ھ���״̬�Ľ���
			{
				min_priority = p->priority;
				q = p;
			}
		}
		if (q != NULL)
		{
			q->state = 'r';
			q->totalWaitTime = currentTime - q->arrivalTime;
			wait_total_time += q->totalWaitTime;  //��¼�ܵȴ�ʱ��
			cout << "���� " << q->pid << " ��ʼִ�У�" << "��ǰʱ��:" << currentTime << "s, �ۼƵȴ�ʱ��:" << q->totalWaitTime << "s" << endl;
			currentTime += q->neededTime;  //���µ�ǰʱ��
		}
		cnt--;
	}
}

int main()
{
	srand((unsigned)time(NULL));  //�������������
	wait_total_time = 0;  //��¼�ܵȴ�ʱ��
	struct PCB* head = new PCB;  //�����������ͷָ�룬������ʱ���С��������
	head->next = NULL;
	cout << "---------------   ���̴�����ʼ  ----------------" << endl;
	for (int i = 0; i < N; i++)
	{
		struct PCB* tmp = createProcess(i);
		tmp->next = head->next;  // ���½��̲��뵽����ͷ��
		head->next = tmp;
	}
	cout << "---------------   ���̵��ȿ�ʼ  ----------------" << endl;
	HRRN(head);
	cout << "���н���ִ����ϣ�"<< endl <<"��ƽ���ȴ�ʱ��:" << static_cast<float>(wait_total_time) / N << "s" << endl;
	//�ͷ��ڴ�
	while (head != nullptr) {
		PCB* p = head->next;
		delete head;
		head = p;
	}
	return 0;
}