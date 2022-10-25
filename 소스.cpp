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

int faultcnt = 0; //페이지 부재 횟수
int arr[100001]; //참조순서
int page[100]; //현재 페이지 
int pagenum = 0; //페이지 개수
int refernum = 0; //참조 개수
int algorithmnum = 0;

int main() {
	cout << "적재 페이지 개수 : ";
	cin >> pagenum;
	cout << "참조 개수 : ";
	cin >> refernum;
	cout << "참조 순서 입력 : ";

	for (int k = 0; k < refernum; k++)
	{
		cin >> arr[k];
	}

	cout << "페이지 교체 알고리즘 선택\n1.FIFO(First In First Out)\n2.OPT(Optimal)\n3.LRU(Least Recently Used)\n4.LFU(Least Frequently Used)\n5.MFU(Most Frequently used)\n번호 입력 : ";
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
	int changeindex = 0; //바꿀 인덱스 설정
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
			fault = 1; // 중복되는 페이지가 없다면 페이지 부재 발생
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
				for (int j = k+1; j < refernum; j++) // 거리 측정
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
			for (int i = 0; i < pagenum; i++) //거리가 제일 긴 것 중 가장 빨리 들어온 페이지 교체
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
			for (int i = 0; i < pagenum; i++) // 가장 먼저 들어온 페이지를 찾아서 교체
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

void LFU() //참조횟수가 가장 적고 가장 일찍들어온거 교체
{
	int refer[100001];
	int put[100001];
	for (int k = 0; k < pagenum; k++) //참조 횟수 0으로 초기화시키고 언제 들어왔는지 기록
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
		if (check(arr[k]) != 1)  //페이지 부재가 일어날때
		{
			int minrefer = 100000;
			int earlynum = 100000;
			int selectnum = -1;
			for (int i = 0; i < pagenum; i++) //최소 참조횟수 찾기
			{
				if (minrefer > refer[i])
				{
					minrefer = refer[i];
				}
			}
			for (int i = 0; i < pagenum; i++) //참조 횟수가 가장 적은 페이지중 가장 일찍들어온 페이지를 선택하기
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
			page[selectnum] = arr[k]; //페이지 교체
			refer[selectnum] = 0; //참조횟수 초기화
			put[selectnum] = k; //들어온 시간 기록
			fault = 1; //페이지 부재 발생
		}
		else // 이미 해당 페이지가 있다면
		{
			for (int i = 0; i < pagenum; i++)
			{
				if (page[i] == arr[k])
				{
					refer[i]++; //참조 횟수추가
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
	for (int k = 0; k < pagenum; k++) //참조 횟수 0으로 초기화시키고 언제 들어왔는지 기록
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
		if (check(arr[k]) != 1)  //페이지 부재가 일어날때
		{
			int maxrefer = -1;
			int earlynum = 100000;
			int selectnum = -1;
			for (int i = 0; i < pagenum; i++) //최소 참조횟수 찾기
			{
				if (maxrefer < refer[i])
				{
					maxrefer = refer[i];
				}
			}
			for (int i = 0; i < pagenum; i++) //참조 횟수가 가장 적은 페이지중 가장 일찍들어온 페이지를 선택하기
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
			page[selectnum] = arr[k]; //페이지 교체
			refer[selectnum] = 0; //참조횟수 초기화
			put[selectnum] = k; //들어온 시간 기록
			fault = 1; //페이지 부재 발생
		}
		else // 이미 해당 페이지가 있다면
		{
			for (int i = 0; i < pagenum; i++)
			{
				if (page[i] == arr[k])
				{
					refer[i]++; //참조 횟수추가
				}
			}
		}
		print(k, fault);
	}
}


int check(int num) //페이지 부재가 발생하는 코드
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

void init() //페이지 값 초기화
{
	for (int k = 0; k < pagenum; k++)
	{
		page[k] = -1;
	}
}

void print(int num, int f) //페이지 상태 출력
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

void printfault() //페이지 부재 횟수 출력
{
	cout << "페이지 부재 : " << faultcnt << " 번";
}
