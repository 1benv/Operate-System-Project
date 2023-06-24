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
* 功能描述：OPT算法的实现 二维vector 空间O(N)+时间O(内存大小); 
* 输入参数： 无
* 输出参数： 输出OPT算法相关信息
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
void OPT() {
	int *last;
	last = new int(n);
	int ci=0;
	double lv=0;
	//初始化各个值最先出现的时间为最大
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
		printf("错误：页号%d超过页表大小\n",work[i]); 
		return ;	
		}
		
		if(PageTable[work[i]].stand==0) {
			printf("作业%d不在内存中，发生缺页中断！\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("作业%d已放入空闲内存块%d\n",work[i],i_cnt);
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
				printf("通过opt算法 将内存块%d中的%d替换为%d\n",maxxloc,InnerMemory[maxxloc],work[i]);
				
				
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
			printf("内存块%d已经在内存中\n",work[i]);
			PageTable[work[i]].vis=1;
		}
		showmemory();
		showtable();
		
	}
	lv=1.0*ci/n;
	printf("opt算法的缺页次数为%d , 缺页率为 %lf\n",ci,lv);
};
/*********************************************************
* 功能描述：LRU算法的实现 普通模拟 时间复杂度O(内存块大小) 
* 输入参数： 无
* 输出参数： 输出LRU算法相关信息
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
void LRU() {
	int *last;
	int ci=0;
	double lv=0;
	last = new int(n);
	
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("错误：页号%d超过页表大小\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("作业%d不在内存中，发生缺页中断！\n",work[i]);
			ci++; 
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("作业%d已放入空闲内存块%d\n",work[i],i_cnt);
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
				printf("通过LRU算法 将内存块%d中的%d替换为%d\n",minnloc,InnerMemory[minnloc],work[i]);
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
			printf("内存块%d已经在内存中\n",work[i]);
			PageTable[work[i]].vis=1;
			last[work[i]]=i;
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("LRU算法的缺页次数为%d , 缺页率为 %lf\n",ci,lv);
};
/*********************************************************
* 功能描述：FIFO算法的实现  队列+pair直接取出 时间复杂度O(1) 
* 输入参数： 无
* 输出参数： 输出FIFO算法相关信息
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
void FIFO() {
	int *last;
	last = new int(n);
	//初始化各个值最先出现的时间为最大
	int ci=0;
	double lv=0.0;
	for(int i =1 ; i<=n ; i++) {
		if(PageTable[work[i]].PageNumber==0)
		{
		printf("错误：页号%d超过页表大小\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("作业%d不在内存中，发生缺页中断！\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("作业%d已放入空闲内存块%d\n",work[i],i_cnt);
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=i_cnt;
				q.push({work[i],i_cnt});
			} else {
				
				 int nowwork=q.front().first;
				 int nowloc=q.front().second; 
				 q.pop();
				printf("通过FIFO算法 将内存块%d中的%d替换为%d\n",nowloc,InnerMemory[nowloc],work[i]);
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
			printf("内存块%d已经在内存中\n",work[i]);
			PageTable[work[i]].vis=1;
			
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("FIFO算法的缺页次数为%d , 缺页率为 %lf\n",ci,lv);
	belady[++b_cnt].lv=lv;
	belady[b_cnt].id=MemoryBlock;
	int f=0;
	for(int i = 1 ; i<b_cnt ;i++)
	{
		if(lv>belady[i].lv&&MemoryBlock>belady[i].id)
		{
			f=1;
			printf("FIFO发生了belady现象!!!\n"); 
			break;
		}
	}
	if(f==0)
	printf("FIFO还未发生belady现象\n"); 
};
/*********************************************************
* 功能描述：CLOCK算法的实现 	
* 输入参数： 无
* 输出参数： 输出CLOCK算法相关信息
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
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
		printf("错误：页号%d超过页表大小\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("作业%d不在内存中，发生缺页中断！\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("作业%d已放入空闲内存块%d\n",work[i],i_cnt);
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
				printf("通过CLOCK算法 将内存块%d中的%d替换为%d\n",loc,InnerMemory[loc],work[i]);
				PageTable[InnerMemory[loc]].stand=0;
				PageTable[InnerMemory[loc]].vis=0;
				PageTable[InnerMemory[loc]].MemoryNumber=0;
				InnerMemory[loc]=work[i];
				PageTable[work[i]].stand=1;
				PageTable[work[i]].MemoryNumber=loc;
				PageTable[work[i]].vis=0;

			}
		} else {
			printf("内存块%d已经在内存中\n",work[i]);
			PageTable[work[i]].vis=1;
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("CLOCK算法的缺页次数为%d , 缺页率为 %lf\n",ci,lv);
};
/*********************************************************
* 功能描述：工作集算法的实现 双指针+set set O(1)查询  时间复杂度O(内存块大小)  
* 输入参数： 无
* 输出参数： 输出工作集算法相关信息
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
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
		printf("错误：页号%d超过页表大小\n",work[i]); 
		return ;	
		}
		if(PageTable[work[i]].stand==0) {
			printf("作业%d不在内存中，发生缺页中断！\n",work[i]);
			ci++;
			if(i_cnt<MemoryBlock) {
				InnerMemory[++i_cnt]=work[i];
				printf("作业%d已放入空闲内存块%d\n",work[i],i_cnt);
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
				printf("通过工作集算法 将内存块%d中的%d替换为%d\n",loc,InnerMemory[loc],work[i]);
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
			printf("内存块%d已经在内存中\n",work[i]);
			PageTable[work[i]].vis=1;
		}
		showmemory();
		showtable();
	}
		lv=1.0*ci/n;
	printf("工作集算法的缺页次数为%d , 缺页率为 %lf\n",ci,lv);
};
/*********************************************************
* 功能描述：页表 内存 外存等参数初始化 
* 输入参数： 无
* 输出参数： 无 
* 返回值： 无
* 其它说明：消息字段之间用分号(;)分隔
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
* 功能描述：地址转换 找到页号和偏移地址 
* 输入参数： 逻辑地址x 
* 输出参数： 页号 
* 返回值： 无  
* 其它说明：消息字段之间用分号(;)分隔
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
* 功能描述：用于展示页表的使用情况 
* 输入参数： 无 
* 输出参数： 输出页表当前状态 
* 返回值： 无 
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
void showtable() {
	printf("此时页表的情况\n");
	printf("页号 状态位 逻辑地址 内存块号 访问位\n");
	for(int j = 1 ; j <=PageNumberSize ; j++) {
		printf("%5d %5d %5d %5d %5d",j,PageTable[j].stand,PageTable[j].ExternalMemoryAddress,PageTable[j].MemoryNumber,PageTable[j].vis);
		printf("\n");
	}
}
/*********************************************************
* 功能描述：用于展示内存使用情况 
* 输入参数： 无
* 输出参数：输出内存情况 
* 返回值：无
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
//void showmemory() {
//	printf("\n 此时内存块情况\n");
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
	printf("\n 此时内存块情况\n");
	printf("内存块号 对应的工作号 物理地址\n"); 
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
* 功能描述：用于释放内存 
* 输入参数： 无
* 输出参数：无 
* 返回值：无
* 其它说明：消息字段之间用分号(;)分隔
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
