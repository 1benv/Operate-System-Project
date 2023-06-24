#include<bits/stdc++.h>
using namespace std;
//#include "algorithm.h";
int MemoryBlock,LogicalAddress,PageSize,PageNumberSize;

int n,i_cnt=0 , o_cnt=0;
priority_queue<int , vector<int> , greater<int> > belady_q;
queue<pair<int,int>> q;
set<int> work_set;

struct  table {
	int PageNumber;
	bool stand;
	int MemoryNumber;
	int ExternalMemoryAddress;
	bool vis;
}*PageTable;

int *InnerMemory , *ExternalMemory,*work;
int b_cnt=0,k=0;
struct node
{
	int id;
	double lv;
}belady[100005];

void OPT();
void LRU();
void FIFO();
void CLOCK();
void WORK();
void init();
int AddressTranslation(int x);
void showmemory();
void showtable();

/*********************************************************
* ����������OPT�㷨��ʵ�� ��άvector �ռ�O(N)+ʱ��O(�ڴ��С); 
* ��������� ��
* ��������� ���OPT�㷨�����Ϣ
* ����ֵ�� 0-�ɹ�   ����-ʧ��
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
void OPT() {
	int *last;
	last = new int(n);
	int ci=0;
	double lv=0;
	//��ʼ������ֵ���ȳ��ֵ�ʱ��Ϊ���
	vector<int> v[(1<<PageSize)];
	for(int i = 1 ;i <= (1<<PageSize) ; i++)
	{
		v[i].push_back(n+1);
	}
	for(int i = n ; i>=1 ; i--)
	{
		v[work[i]].push_back(i);
	}
	for(int i =1 ; i<=n ; i++) {
		v[work[i]].pop_back();
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
//					for(int k  = i+1  ; k<=n ; k++) {
//						if(work[i]==work[k]) {
//							last[work[i]]=k;
//							continue;
//						}
//					}
//					if(last[work[j]]>maxx) {
//						maxx=last[work[j]];
//						maxxloc=j;
//					}
				int size=v[work[InnerMemory[j]]].size();
				//printf("maxx %d",v[work[InnerMemory[j]]][size-1]);
				if(v[work[InnerMemory[j]]][size-1]>maxx)
				{
					maxx=v[work[InnerMemory[j]]][size-1];
					maxxloc=j; 
					//printf("maxxloc%d\n",maxxloc);
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
/*********************************************************
* ����������LRU�㷨��ʵ�� ��ͨģ�� ʱ�临�Ӷ�O(�ڴ���С) 
* ��������� ��
* ��������� ���LRU�㷨�����Ϣ
* ����ֵ�� 0-�ɹ�   ����-ʧ��
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
void LRU() {
	int *last;
	int ci=0;
	double lv=0;
	last = new int(n);
	
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
/*********************************************************
* ����������FIFO�㷨��ʵ��  ����+pairֱ��ȡ�� ʱ�临�Ӷ�O(1) 
* ��������� ��
* ��������� ���FIFO�㷨�����Ϣ
* ����ֵ�� 0-�ɹ�   ����-ʧ��
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
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
				printf("ͨ��FIFO�㷨 ���ڴ��%d�е�%d�滻Ϊ%d\n",nowloc,InnerMemory[nowloc],work[i]);
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
/*********************************************************
* ����������CLOCK�㷨��ʵ�� 	
* ��������� ��
* ��������� ���CLOCK�㷨�����Ϣ
* ����ֵ�� 0-�ɹ�   ����-ʧ��
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
void CLOCK() {
	int *last;
	int ci=0;
	double lv=0;
	int *vis;
	vis = new int(n);
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
				
				vis[InnerMemory[zhi]]=1;
				 zhi++;
				 if(zhi>=MemoryBlock+1) zhi=1;
			} else {
				
				while(1)
				{
					if(vis[InnerMemory[zhi]]==1)
					{
						vis[InnerMemory[zhi]]=0;
					}
					else
					{
						vis[InnerMemory[zhi]]=1;
						loc=zhi;
						break;
					}
					zhi++;
					if(zhi>=MemoryBlock+1) zhi=1;
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
/*********************************************************
* �����������������㷨��ʵ�� ˫ָ��+set set O(1)��ѯ  ʱ�临�Ӷ�O(�ڴ���С)  
* ��������� ��
* ��������� ����������㷨�����Ϣ
* ����ֵ�� 0-�ɹ�   ����-ʧ��
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
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
/*********************************************************
* ����������ҳ�� �ڴ� ���Ȳ�����ʼ�� 
* ��������� ��
* ��������� �� 
* ����ֵ�� ��
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
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
/*********************************************************
* ������������ַת�� �ҵ�ҳ�ź�ƫ�Ƶ�ַ 
* ��������� �߼���ַx 
* ��������� ҳ�� 
* ����ֵ�� ��  
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
int AddressTranslation(int x) {
	int pagenumber=x/(1<<PageSize);
	int offset=x%(1<<PageSize);
	int kuai=PageTable[pagenumber].MemoryNumber;
	int ans=kuai*(1<<PageSize)+offset;
	if(ans<0) ans=0;
	return ans;
};
/*********************************************************
* ��������������չʾҳ���ʹ����� 
* ��������� �� 
* ��������� ���ҳ��ǰ״̬ 
* ����ֵ�� �� 
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
void showtable() {
	printf("��ʱҳ������\n");
	printf("ҳ�� ״̬λ �߼���ַ �ڴ��� ����λ\n");
	for(int j = 1 ; j <=PageNumberSize ; j++) {
		printf("%5d %5d %5d %5d %5d",j,PageTable[j].stand,PageTable[j].ExternalMemoryAddress,PageTable[j].MemoryNumber,PageTable[j].vis);
		printf("\n");
	}
}
/*********************************************************
* ��������������չʾ�ڴ�ʹ����� 
* ��������� ��
* �������������ڴ���� 
* ����ֵ����
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
//void showmemory() {
//	printf("\n ��ʱ�ڴ�����\n");
//	for(int j = 1 ; j<=MemoryBlock ; j++) {
//		printf("%5d ",j);
//
//	}
//	printf("\n");
//	for(int j = 1 ; j<=MemoryBlock ; j++) {
//		printf("%5d ",InnerMemory[j]);
//	}
//	printf("\n");
//}

void showmemory() {
	printf("\n ��ʱ�ڴ�����\n");
	printf("�ڴ��� ��Ӧ�Ĺ����� �����ַ\n"); 
	for(int j = 1 ; j<=MemoryBlock ; j++) {
		printf("%5d %5d %10d\n",j,InnerMemory[j],AddressTranslation(PageTable[InnerMemory[j]].ExternalMemoryAddress));
	}
	printf("\n");
//	for(int j = 1 ; j<=MemoryBlock ; j++) {
//		printf("%5d ",InnerMemory[j]);
//	}
//	printf("\n");
}
/*********************************************************
* ���������������ͷ��ڴ� 
* ��������� ��
* ����������� 
* ����ֵ����
* ����˵������Ϣ�ֶ�֮���÷ֺ�(;)�ָ�
************************************************************/
void free()
{
	delete[] PageTable;
	PageTable=nullptr;
	delete[] InnerMemory;
	InnerMemory=nullptr;
	delete[] ExternalMemory;
	ExternalMemory = nullptr;
	delete[] work ;
	work = nullptr;
 } 
