#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include "stdlib.h"     //包含随机数产生函数
#include "time.h"      //与时间有关的函数头文件
#include "windows.h"    //针对Windows操作系统
#include <iostream>
#include <string>
#include <process.h>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>

using namespace std;


#define FAMILY_SIZE 7  //家庭人数
#define OPERATION 2   //最多操作次数

CRITICAL_SECTION cs;  //定义全局互斥锁

struct Operator {
	string name;  //操作员姓名
	int money;    //操作金额
	int op = OPERATION;    //操作次数
};

int account; //账户余额


//存款线程
DWORD WINAPI Deposit(LPVOID p)
{
	struct Operator* action = (struct Operator*)p;
	EnterCriticalSection(&cs);
	account += action->money;
	cout << left << setw(25) << "存款交易进行中···"
		<< setw(10) << (action->name)
		<< setw(5) << "存入 ￥" 
		<< setw(5) << (action->money)
		<< setw(10) << "账户余额为 ￥" << account << endl;
	LeaveCriticalSection(&cs);
	return 0;
}

//取款线程
DWORD WINAPI Withdraw(LPVOID p)
{
	struct Operator* action = (struct Operator*)p;
	EnterCriticalSection(&cs);
	account -= action->money;
	cout << left << setw(25) << "取款交易进行中···"
		<< setw(10) << (action->name)
		<< setw(5) << "取出 ￥" 
		<< setw(5) << (action->money)
		<< setw(10) << "账户余额为 ￥" << account << endl;
	LeaveCriticalSection(&cs);
	return 0;
}

int main()
{
	InitializeCriticalSection(&cs);
	account = 10;   //初始账户余额为10
	srand((unsigned) time(NULL));  //设置随机数种子
	//初始化
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
	//创建线程
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
				std::cerr << "ERROR：随机数生成错误！请重新调试！" << endl;
				return 1;
			}
			// 检查线程创建是否成功
			if (threads[i] == NULL) {
				std::cerr << "ERROR: 创建线程失败！" << std::endl;
				return 1; 
			}
			operators[t].op--;
		}
		else
			i--;

	}

	//等待所有线程结束
	for (int i = 0; i < FAMILY_SIZE * OPERATION; i++)
	{
		WaitForSingleObject(threads[i], INFINITE);
		CloseHandle(threads[i]);
	}
	DeleteCriticalSection(&cs);
	std::cout << "最终账户余额为  ￥" << account << endl;

	return 0;
}
