#include "algorithm.h"
int main() {
	srand(0);
	while(1) {
		printf("**********虚拟存储管理**********\n");
		printf("请分别输入内存块数 逻辑地址位数 页面大小位数：");
		scanf("%d %d %d",&MemoryBlock,&LogicalAddress,&PageSize);
		PageNumberSize = LogicalAddress - PageSize ;
		PageNumberSize = (1<<PageNumberSize);
		printf("请输入要操作的序列长度:");
		scanf("%d",&n);
		printf("请输入操作的序列:");
		init();
		for(int i = 1 ; i<=n  ; i++) {
			scanf("%d",&work[i]);
		}
		printf("请输出要使用的页面替换算法:\n");
		printf("1.OPT 2.LRU 3.FIFO 4.CLOCK 5.工作集算法\n");
		int op;
		cin>>op;

		switch (op) {
			case 1:
				OPT();
				break;
			case 2 :
				LRU();
				break;
			case 3:
				FIFO();
				break;
			case 4:
				CLOCK();
				break;
			case 5:
				printf("请输入一个工作集的K值：");
				scanf("%d",&k); 
				WORK();
		}

	}
}
