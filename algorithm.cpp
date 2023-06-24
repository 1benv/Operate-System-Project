
#include "algorithm.h"

//#include <bits/stdc++.h>
using namespace std;
int MemoryBlock,LogicalAddress,PageSize,PageNumberSize;
int n,i_cnt=0 ,o_cnt=0;
priority_queue<int , vector<int> , greater<int> > belady_q;
queue<pair<int,int>> q;
set<int> work_set;
int *InnerMemory , *ExternalMemory,*work;
int b_cnt=0,k=0;

void OPT() {
	int *last;
	last = new int(n);
	int ci=0;
	double lv=0;
	//��ʼ������ֵ���ȳ��ֵ�ʱ��Ϊ���
	for(int i = 1 ; i<=n ; i++) {
		last[i]=n+1;
	}
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("����ҳ��%d����ҳ���С\n",work[i]); 
		return ;	
		}
		
		if(PageTable[work[i]].stand==0) {
			printf("��ҵ%d�����ڴ��У�����ȱҳ�жϣ�\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("��ҵ%d�ѷ�������ڴ��%d\n",work[i],i_cnt);
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=i_cnt;
			} else {
				int maxx=0,maxxloc=0;
				for(int j = 1; j<=MemoryBlock ; j++) {
					for(int k  = i+1  ; k<=n ; k++) {
						if(work[i]==work[k]) {
							last[work[i]]=k;
							continue;
						}
					}
					if(last[work[j]]>maxx) {
						maxx=last[work[j]];
						maxxloc=j;
					}
				}
				printf("ͨ��opt�㷨 ���ڴ��%d�е�%d�滻Ϊ%d\n",maxxloc,InnerMemory[maxxloc],work[i]);
				
				
				//cout<<	PageTable[work[i]].MemoryNumber<<"	PageTable[work[i]].MemoryNumber";
				
				PageTable[InnerMemory[maxxloc]].stand=0;
				PageTable[InnerMemory[maxxloc]].vis=0;
				PageTable[InnerMemory[maxxloc]].MemoryNumber=0;
				PageTable[work[i]].vis=0;
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=maxxloc;
				InnerMemory[maxxloc]=work[i];
				//cout<<	PageTable[InnerMemory[maxxloc]].MemoryNumber<<"	PageTable[InnerMemory[maxxloc]].MemoryNumber";

			}
		} else {
			printf("�ڴ��%d�Ѿ����ڴ���\n",work[i]);
			PageTable[work[i]].vis=1;
		}
		showmemory();
		showtable();
		
	}
	lv=1.0*ci/n;
	printf("opt�㷨��ȱҳ����Ϊ%d , ȱҳ��Ϊ %lf\n",ci,lv);
};
void LRU() {
	int *last;
	int ci=0;
	double lv=0;
	last = new int(n);
	//��ʼ������ֵ���ȳ��ֵ�ʱ��Ϊ���
	
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("����ҳ��%d����ҳ���С\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("��ҵ%d�����ڴ��У�����ȱҳ�жϣ�\n",work[i]);

			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("��ҵ%d�ѷ�������ڴ��%d\n",work[i],i_cnt);
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=i_cnt;
				last[work[i]]=i;
			} else {
				int minn=INT_MIN,minnloc=0;
				for(int j = 1; j<=MemoryBlock ; j++) {
					if(last[work[j]]<minn) {
						minn=last[work[j]];
						minnloc=j;
					}
				}
				printf("ͨ��LRU�㷨 ���ڴ��%d�е�%d�滻Ϊ%d\n",minnloc,InnerMemory[minnloc],work[i]);
				PageTable[InnerMemory[minnloc]].stand=0;
				PageTable[InnerMemory[minnloc]].vis=0;
				PageTable[InnerMemory[minnloc]].MemoryNumber=0;
				InnerMemory[minnloc]=work[i];
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=minnloc;
				PageTable[work[i]].vis=0;

				last[work[i]]=i;
			}
		} else {
			printf("�ڴ��%d�Ѿ����ڴ���\n",work[i]);
			PageTable[work[i]].vis=1;
			last[work[i]]=i;
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("LRU�㷨��ȱҳ����Ϊ%d , ȱҳ��Ϊ %lf\n",ci,lv);
};
void FIFO() {
	int *last;
	last = new int(n);
	//��ʼ������ֵ���ȳ��ֵ�ʱ��Ϊ���
	int ci=0;
	double lv=0.0;
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("����ҳ��%d����ҳ���С\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("��ҵ%d�����ڴ��У�����ȱҳ�жϣ�\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("��ҵ%d�ѷ�������ڴ��%d\n",work[i],i_cnt);
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=i_cnt;
				q.push({work[i],i_cnt});
			} else {
				
				 int nowwork=q.front().first;
				 int nowloc=q.front().second; 
				 q.pop();
				printf("ͨ��LRU�㷨 ���ڴ��%d�е�%d�滻Ϊ%d\n",nowloc,InnerMemory[nowloc],work[i]);
				PageTable[InnerMemory[nowloc]].stand=0;
				PageTable[InnerMemory[nowloc]].vis=0;
				PageTable[InnerMemory[nowloc]].MemoryNumber=0;
				InnerMemory[nowloc]=work[i];
				q.push({work[i],nowloc});
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=nowloc;
				PageTable[work[i]].vis=0;

			}
		} else {
			printf("�ڴ��%d�Ѿ����ڴ���\n",work[i]);
			PageTable[work[i]].vis=1;
			
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("FIFO�㷨��ȱҳ����Ϊ%d , ȱҳ��Ϊ %lf\n",ci,lv);
	belady[++b_cnt].lv=lv;
	belady[b_cnt].id=MemoryBlock;
	int f=0;
	for(int i = 1 ; i<b_cnt ;i++)
	{
		if(lv>belady[i].lv&&MemoryBlock>belady[i].id)
		{
			f=1;
			printf("FIFO������belady����!!!\n"); 
			break;
		}
	}
	if(f==0)
	printf("FIFO��δ����belady����\n"); 
};
void CLOCK() {
	int *last;
	int ci=0;
	double lv=0;
	last = new int(n);
	int zhi=1,loc;
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("����ҳ��%d����ҳ���С\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("��ҵ%d�����ڴ��У�����ȱҳ�жϣ�\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("��ҵ%d�ѷ�������ڴ��%d\n",work[i],i_cnt);
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=i_cnt;
				last[work[i]]=i;
			} else {
				
				while(1)
				{
					if(PageTable[work[zhi]].vis==1)
					{
						PageTable[work[zhi]].vis=0;
					}
					else
					{
						PageTable[work[zhi]].vis=1;
						loc=zhi;
						break;
					}
					zhi++;
					if(zhi==n+1) zhi=1;
				}
				printf("ͨ��CLOCK�㷨 ���ڴ��%d�е�%d�滻Ϊ%d\n",loc,InnerMemory[loc],work[i]);
				PageTable[InnerMemory[loc]].stand=0;
				PageTable[InnerMemory[loc]].vis=0;
				PageTable[InnerMemory[loc]].MemoryNumber=0;
				InnerMemory[loc]=work[i];
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=loc;
				PageTable[work[i]].vis=0;

			}
		} else {
			printf("�ڴ��%d�Ѿ����ڴ���\n",work[i]);
			PageTable[work[i]].vis=1;
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("CLOCK�㷨��ȱҳ����Ϊ%d , ȱҳ��Ϊ %lf\n",ci,lv);
};
void WORK()
{
	int *last;
	int ci=0;
	double lv=0;
	last = new int(n);
	int l = 0,r=0,loc;
	
	set<int>::iterator it;
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("����ҳ��%d����ҳ���С\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("��ҵ%d�����ڴ��У�����ȱҳ�жϣ�\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("��ҵ%d�ѷ�������ڴ��%d\n",work[i],i_cnt);
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=i_cnt;
				if(r+1<=n)r++;
				if(r-l>k) 
				{
				work_set.erase(work[l]); 
				l++;
				} 
				work_set.insert(work[r]); 
			} else {
				if(r+1<=n)r++;
				if(r-l>k) 
				{
				work_set.erase(work[l]); 
				l++;
				} 
				for(int j = 1 ; j <= MemoryBlock ; j++)
				{
					if((it=work_set.find(InnerMemory[j]))==work_set.end())
					{
						loc=j;
						break;
					}
				}
				printf("ͨ���������㷨 ���ڴ��%d�е�%d�滻Ϊ%d\n",loc,InnerMemory[loc],work[i]);
				PageTable[InnerMemory[loc]].stand=0;
				PageTable[InnerMemory[loc]].vis=0;
				PageTable[InnerMemory[loc]].MemoryNumber=0;
				InnerMemory[loc]=work[i];
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=loc;
				PageTable[work[i]].vis=0;
				work_set.insert(work[i]);
			}
		} else {
			if(r+1<=n)r++;
				if(r-l>k) 
				{
				work_set.erase(work[l]); 
				l++;
				} 
			printf("�ڴ��%d�Ѿ����ڴ���\n",work[i]);
			PageTable[work[i]].vis=1;
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("�������㷨��ȱҳ����Ϊ%d , ȱҳ��Ϊ %lf\n",ci,lv);
};
void init() {
	srand((unsigned)time(NULL));
	PageTable = new table[PageNumberSize+5]();
	InnerMemory = new int[MemoryBlock+5]();
	ExternalMemory = new int[PageNumberSize+5]();
	work = new int[PageNumberSize+5]();
	i_cnt=0;
	for(int i =1 ; i<=PageNumberSize ; i++) {

		int OffsetAddress = rand()%(1<<PageSize)+1;
		PageTable[i].PageNumber=i;
		PageTable[i].stand=0;
		PageTable[i].ExternalMemoryAddress=i*(1<<PageSize)+OffsetAddress;
		PageTable[i].MemoryNumber=0;
		PageTable[i].vis=0;
	}
	while(!q.empty())
	{
		q.pop();
	}
	work_set.clear();
}
int AddressTranslation(int x) {
	int pagenumber=x/PageSize;
	int offset=x%PageSize;
	return pagenumber;
};

void showtable() {
	printf("��ʱҳ������\n");
	printf("ҳ�� ״̬λ �߼���ַ �ڴ��� ����λ\n");
	for(int j = 1 ; j <=PageNumberSize ; j++) {
		printf("%5d %5d %5d %5d %5d",j,PageTable[j].stand,PageTable[j].ExternalMemoryAddress,PageTable[j].MemoryNumber,PageTable[j].vis);
		printf("\n");
	}
}

void showmemory() {
	printf("\n ��ʱ�ڴ�����\n");
	for(int j = 1 ; j<=MemoryBlock ; j++) {
		printf("%5d ",j);

	}
	printf("\n");
	for(int j = 1 ; j<=MemoryBlock ; j++) {
		printf("%5d ",InnerMemory[j]);
	}
	printf("\n");
}
