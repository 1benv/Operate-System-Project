#include "algorithm.h"
int main() {
	srand(0);
	while(1) {
		printf("**********����洢����**********\n");
		printf("��ֱ������ڴ���� �߼���ַλ�� ҳ���Сλ����");
		scanf("%d %d %d",&MemoryBlock,&LogicalAddress,&PageSize);
		PageNumberSize = LogicalAddress - PageSize ;
		PageNumberSize = (1<<PageNumberSize);
		printf("������Ҫ���������г���:");
		scanf("%d",&n);
		printf("���������������:");
		init();
		for(int i = 1 ; i<=n  ; i++) {
			scanf("%d",&work[i]);
		}
		printf("�����Ҫʹ�õ�ҳ���滻�㷨:\n");
		printf("1.OPT 2.LRU 3.FIFO 4.CLOCK 5.�������㷨\n");
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
				printf("������һ����������Kֵ��");
				scanf("%d",&k); 
				WORK();
		}

	}
}
