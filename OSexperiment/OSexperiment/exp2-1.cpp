#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include "stdlib.h"     //�����������������
#include "time.h"      //��ʱ���йصĺ���ͷ�ļ�
#include "windows.h"    //���Windows����ϵͳ
#include <iostream>
#include <string>
#include <process.h>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>

using namespace std;


#define FAMILY_SIZE 7  //��ͥ����
#define OPERATION 2   //����������

CRITICAL_SECTION cs;  //����ȫ�ֻ�����

struct Operator {
	string name;  //����Ա����
	int money;    //�������
	int op = OPERATION;    //��������
};

int account; //�˻����


//����߳�
DWORD WINAPI Deposit(LPVOID p)
{
	struct Operator* action = (struct Operator*)p;
	EnterCriticalSection(&cs);
	account += action->money;
	cout << left << setw(25) << "���׽����С�����"
		<< setw(10) << (action->name)
		<< setw(5) << "���� ��" 
		<< setw(5) << (action->money)
		<< setw(10) << "�˻����Ϊ ��" << account << endl;
	LeaveCriticalSection(&cs);
	return 0;
}

//ȡ���߳�
DWORD WINAPI Withdraw(LPVOID p)
{
	struct Operator* action = (struct Operator*)p;
	EnterCriticalSection(&cs);
	account -= action->money;
	cout << left << setw(25) << "ȡ��׽����С�����"
		<< setw(10) << (action->name)
		<< setw(5) << "ȡ�� ��" 
		<< setw(5) << (action->money)
		<< setw(10) << "�˻����Ϊ ��" << account << endl;
	LeaveCriticalSection(&cs);
	return 0;
}

int main()
{
	InitializeCriticalSection(&cs);
	account = 10;   //��ʼ�˻����Ϊ10
	srand((unsigned) time(NULL));  //�������������
	//��ʼ��
	HANDLE threads[FAMILY_SIZE * OPERATION];
	struct Operator operators[FAMILY_SIZE] = {
		{"dad",10},
		{"mom",20},
		{"grandma",30},
		{"grandpa",40},
		{"uncle",50},
		{"Mary",50},
		{"Sally",100}
	};
	//�����߳�
	for (int i = 0; i < FAMILY_SIZE * OPERATION; i++)
	{
		int t = rand() % FAMILY_SIZE;
		if (operators[t].op > 0)
		{
			if (t == 5 || t == 6)
				threads[i] = CreateThread(NULL, 0, Withdraw, (void*)&operators[t], 0, NULL);
			else if (t >= 0 && t <= 4)
				threads[i] = CreateThread(NULL, 0, Deposit, (void*)&operators[t], 0, NULL);
			else
			{
				std::cerr << "ERROR����������ɴ��������µ��ԣ�" << endl;
				return 1;
			}
			// ����̴߳����Ƿ�ɹ�
			if (threads[i] == NULL) {
				std::cerr << "ERROR: �����߳�ʧ�ܣ�" << std::endl;
				return 1; 
			}
			operators[t].op--;
		}
		else
			i--;

	}

	//�ȴ������߳̽���
	for (int i = 0; i < FAMILY_SIZE * OPERATION; i++)
	{
		WaitForSingleObject(threads[i], INFINITE);
		CloseHandle(threads[i]);
	}
	DeleteCriticalSection(&cs);
	std::cout << "�����˻����Ϊ  ��" << account << endl;

	return 0;
}
