#pragma once
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include<bits/stdc++.h>
using namespace std;
extern int MemoryBlock,LogicalAddress,PageSize,PageNumberSize;
extern int n,i_cnt=0 , o_cnt=0;
extern priority_queue<int , vector<int> , greater<int> > belady_q;
extern queue<pair<int,int>> q;
extern set<int> work_set;

 struct  table {
	int PageNumber;
	bool stand;
	int MemoryNumber;
	int ExternalMemoryAddress;
	bool vis;
} ;
extern table *PageTable;

extern int *InnerMemory , *ExternalMemory,*work;
extern int b_cnt=0,k=0;
struct node
{
	int id;
	double lv;
};
extern node belady[100005];
extern void OPT();
extern void LRU();
extern void FIFO();
extern void CLOCK();
extern void WORK();
extern void init();
extern int AddressTranslation(int x);
extern void showmemory();
extern void showtable();
#endif

