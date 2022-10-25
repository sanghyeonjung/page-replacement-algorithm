#include<iostream>
#include<algorithm>
#include<iomanip>

using namespace std;

void FIFO();
void OPT();
void LRU();
void LFU();
void MFU();
void init();
void printfault();
void print(int num, int f);
int check(int num);

int faultcnt = 0; //������ ���� Ƚ��
int arr[100001]; //��������
int page[100]; //���� ������ 
int pagenum = 0; //������ ����
int refernum = 0; //���� ����
int algorithmnum = 0;

int main() {
	cout << "���� ������ ���� : ";
	cin >> pagenum;
	cout << "���� ���� : ";
	cin >> refernum;
	cout << "���� ���� �Է� : ";

	for (int k = 0; k < refernum; k++)
	{
		cin >> arr[k];
	}

	cout << "������ ��ü �˰��� ����\n1.FIFO(First In First Out)\n2.OPT(Optimal)\n3.LRU(Least Recently Used)\n4.LFU(Least Frequently Used)\n5.MFU(Most Frequently used)\n��ȣ �Է� : ";
	cin >> algorithmnum;
	init();

	switch (algorithmnum) {
	case 1:
		FIFO();
		printfault();
		break;
	case 2:
		OPT();
		printfault();
		break;
	case 3:
		LRU();
		printfault();
		break;
	case 4:
		LFU();
		printfault();
		break;
	case 5:
		MFU();
		printfault();
		break;
	default:
		break;
	}

}
void FIFO()
{
	int changeindex = 0; //�ٲ� �ε��� ����
	for (int k = 0; k < pagenum; k++)
	{
		page[k] = arr[k];
		print(k,1);
	}
	for (int k = pagenum; k < refernum; k++)
	{
		int fault = 0;
		if (check(arr[k]) != 1)
		{
			page[changeindex]
				= arr[k];
			fault = 1; // �ߺ��Ǵ� �������� ���ٸ� ������ ���� �߻�
			if (changeindex == pagenum - 1)
			{
				changeindex = 0;
			}
			else
			{
				changeindex++;
			}
		}
		print(k,fault);
	}
}

void OPT()
{
	int put[100001];
	for (int k = 0; k < pagenum; k++)
	{
		page[k] = arr[k];
		print(k, 1);
		put[k] = k;
	}
	for (int k = pagenum; k < refernum; k++)
	{
		int fault = 0;
		if (check(arr[k]) != 1)
		{
			int dis[100001];
			int max = -1;
			for (int i = 0; i < pagenum; i++)
			{
				int cnt = 0;
				for (int j = k+1; j < refernum; j++) // �Ÿ� ����
				{
					if (page[i] == arr[j])
					{
						break;
					}
					cnt++;
				}
				if (max < cnt)
				{
					max = cnt;
				}
				dis[i] = cnt;
			}
			int earlynum = 100000;
			int selectnum = -1;
			for (int i = 0; i < pagenum; i++) //�Ÿ��� ���� �� �� �� ���� ���� ���� ������ ��ü
			{
				if (dis[i] == max)
				{
					if (earlynum > put[i])
					{
						selectnum = i;
						earlynum = put[i];
					}
				}
			}
			page[selectnum] = arr[k];
			put[selectnum] = k;
			fault = 1;
		}
		print(k, fault);
	}
}

void LRU()
{
	int put[100001];
	for (int k = 0; k < pagenum; k++)
	{
		page[k] = arr[k];
		print(k, 1);
		put[k] = k;
	}
	for (int k = pagenum; k < refernum; k++)
	{
		int fault = 0;
		if (check(arr[k]) != 1)
		{
			int earlynum = 100000;
			int selectnum = -1;
			for (int i = 0; i < pagenum; i++) // ���� ���� ���� �������� ã�Ƽ� ��ü
			{
				if (earlynum > put[i])
				{
					selectnum = i;
					earlynum = put[i];
				}
			}
			page[selectnum] = arr[k];
			put[selectnum] = k;
			fault = 1;
		}
		else
		{
			for (int i = 0; i < pagenum; i++)
			{
				if (page[i] == arr[k])
				{
					put[i] = k;
				}
			}
		}
		print(k, fault);
	}
}

void LFU() //����Ƚ���� ���� ���� ���� ������°� ��ü
{
	int refer[100001];
	int put[100001];
	for (int k = 0; k < pagenum; k++) //���� Ƚ�� 0���� �ʱ�ȭ��Ű�� ���� ���Դ��� ���
	{
		refer[k] = 0;
	}
	for (int k = 0; k < pagenum; k++) 
	{
		page[k] = arr[k];
		put[k] = k;
		print(k, 1);
	}
	for (int k = pagenum; k < refernum; k++)
	{
		int fault = 0;
		if (check(arr[k]) != 1)  //������ ���簡 �Ͼ��
		{
			int minrefer = 100000;
			int earlynum = 100000;
			int selectnum = -1;
			for (int i = 0; i < pagenum; i++) //�ּ� ����Ƚ�� ã��
			{
				if (minrefer > refer[i])
				{
					minrefer = refer[i];
				}
			}
			for (int i = 0; i < pagenum; i++) //���� Ƚ���� ���� ���� �������� ���� ������� �������� �����ϱ�
			{
				if (refer[i] == minrefer)
				{
					if (put[i] < earlynum)
					{
						selectnum = i;
						earlynum = put[i];
					}
				}
			}
			page[selectnum] = arr[k]; //������ ��ü
			refer[selectnum] = 0; //����Ƚ�� �ʱ�ȭ
			put[selectnum] = k; //���� �ð� ���
			fault = 1; //������ ���� �߻�
		}
		else // �̹� �ش� �������� �ִٸ�
		{
			for (int i = 0; i < pagenum; i++)
			{
				if (page[i] == arr[k])
				{
					refer[i]++; //���� Ƚ���߰�
				}
			}
		}
		print(k, fault);
	}
}

void MFU()
{
	int refer[100001];
	int put[100001];
	for (int k = 0; k < pagenum; k++) //���� Ƚ�� 0���� �ʱ�ȭ��Ű�� ���� ���Դ��� ���
	{
		refer[k] = 0;
	}
	for (int k = 0; k < pagenum; k++)
	{
		page[k] = arr[k];
		put[k] = k;
		print(k, 1);
	}
	for (int k = pagenum; k < refernum; k++)
	{
		int fault = 0;
		if (check(arr[k]) != 1)  //������ ���簡 �Ͼ��
		{
			int maxrefer = -1;
			int earlynum = 100000;
			int selectnum = -1;
			for (int i = 0; i < pagenum; i++) //�ּ� ����Ƚ�� ã��
			{
				if (maxrefer < refer[i])
				{
					maxrefer = refer[i];
				}
			}
			for (int i = 0; i < pagenum; i++) //���� Ƚ���� ���� ���� �������� ���� ������� �������� �����ϱ�
			{
				if (refer[i] == maxrefer)
				{
					if (put[i] < earlynum)
					{
						selectnum = i;
						earlynum = put[i];
					}
				}
			}
			page[selectnum] = arr[k]; //������ ��ü
			refer[selectnum] = 0; //����Ƚ�� �ʱ�ȭ
			put[selectnum] = k; //���� �ð� ���
			fault = 1; //������ ���� �߻�
		}
		else // �̹� �ش� �������� �ִٸ�
		{
			for (int i = 0; i < pagenum; i++)
			{
				if (page[i] == arr[k])
				{
					refer[i]++; //���� Ƚ���߰�
				}
			}
		}
		print(k, fault);
	}
}


int check(int num) //������ ���簡 �߻��ϴ� �ڵ�
{
	for (int k = 0; k < pagenum; k++) 
	{
		if (page[k] == num)
		{
			return 1;
		}
	}
	return -1;
}

void init() //������ �� �ʱ�ȭ
{
	for (int k = 0; k < pagenum; k++)
	{
		page[k] = -1;
	}
}

void print(int num, int f) //������ ���� ���
{
	cout << num+1 << " : ";
	for (int k = 0; k < pagenum; k++)
	{
		if (page[k] == -1)
		{
			cout << setw(3) << "N";
		}
		else
		{
			cout << setw(3) << page[k];
		}
	}
	if (f == 1)
	{
		faultcnt++;
		cout << setw(3) << "   PAGE FAULT";
	}
	cout << "\n";
}

void printfault() //������ ���� Ƚ�� ���
{
	cout << "������ ���� : " << faultcnt << " ��";
}
