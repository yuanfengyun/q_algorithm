#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>

#include "PlayerHuTips2.h"

using namespace std;

void print_cards(char* cards)
{
	for (int i = 0; i<9; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 9; i<18; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");
	for (int i = 18; i<27; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");
	for (int i = 27; i<34; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\n");
}

#define MAX_COUNT 100000
static BYTE s_HuCardAll[136];

void main()
{

	CPlayerHuTips2 stTssss;
	stTssss.TrainAll();

		
	cin >> n1;
}