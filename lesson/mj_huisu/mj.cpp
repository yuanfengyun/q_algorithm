#include "stdafx.h"
#include "define.h"

#define MAX_INDEX 42

// 万 筒 条
std::string CARDS_STR[] = {
	"none","1万","2万","3万","4万","5万","6万","7万","8万","9万",

	"1条","2条","3条","4条","5条","6条","7条","8条","9条",

	"1筒","2筒","3筒","4筒","5筒","6筒","7筒","8筒","9筒",

	"东","西","南","北","中","发","白",

	"春","夏","秋","冬","梅","兰","竹","菊"
};

std::string CARAD_COLORS[] = {
	"none","万","筒","条","字","花"
};

// 万 筒 条
char CARDS[] = {
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19, // 1-9万

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29, // 1-9条

	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39, // 1-9筒

	0x41,0x42,0x43,0x44,0x45,0x46,0x47,  //东西南北中发白

	0x51,0x52,0x53,0x54,0x55,0x56,0x67,0x58  // 春夏秋冬 梅兰竹菊
};

int get_card_index(char card)
{
	if (card >= 0x11 && card <= 0x19)
	{
		return card - 0x10;
	}else if (card >= 0x21 && card <= 0x29)
	{
		return card - 0x20 + 9;
	}else if(card >= 0x31 && card <= 0x39)
	{
		return card - 0x30 + 18;
	}else if(card >= 0x41 && card <= 0x47)
	{
		return card - 0x40 + 27;
	}else if (card >= 0x51 && card <= 0x58)
	{
		return card - 0x50 + 27 + 7;
	}
	return 0;
}

std::string get_card_str(char card)
{
	return CARDS_STR[get_card_index(card)];
}

int get_color(char card)
{
	return (card & 0xf0) >> 4;
}

std::string get_color_str(char card)
{
	return CARAD_COLORS[get_color(card)];
}

int get_card_value(char card)
{
	return card & 0x0f;
}

// 吃
int get_chi_type(char* cards, char card_index)
{
	int op = 0;
	if (card_index == 0 || card_index>28) return false;

	if (card_index == 1 || card_index == 10 || card_index == 19)
	{
		if (cards[card_index + 1] > 0 && cards[card_index + 2] > 0) op = OP_left_chi;
		return op;
	}

	if (card_index == 9 || card_index == 18 || card_index == 27)
	{
		if (cards[card_index - 1] > 0 && cards[card_index - 2] > 0) op = OP_right_chi;
		return op;
	}

	if (cards[card_index + 1] > 0 && cards[card_index + 2] > 0) op |= OP_left_chi;
	if (cards[card_index - 1] > 0 && cards[card_index - 2] > 0) op |= OP_right_chi;
	if (cards[card_index + 1] > 0 && cards[card_index - 1] > 0) op |= OP_center_chi;
	return op;
}

void chi(char* cards, std::vector<CHI_PENG_GANG>& chi_peng_gang, char card_index, OP_TYPE op)
{
	CHI_PENG_GANG cpg;
	char center_card = card_index;
	if (op == OP_left_chi)
	{
		--cards[card_index + 1];
		--cards[card_index + 2];
		center_card = card_index + 1;
		cpg.type = "left_chi";

	}else if (op == OP_right_chi)
	{
		--cards[card_index - 1];
		--cards[card_index - 2];
		center_card = card_index - 1;
		cpg.type = "right_chi";
	}if (op == OP_center_chi)
	{
		--cards[card_index + 1];
		--cards[card_index - 1];
		cpg.type = "center_chi";
	}
	
	cpg.center_card = card_index;
	cpg.provider = 0;
	chi_peng_gang.push_back(cpg);
}

// 碰
bool can_peng(char cards[], char card_index)
{
	if (card_index == 0 || card_index>34) return false;

	return cards[card_index] >= 2;
}

void peng(char* cards, std::vector<CHI_PENG_GANG>& chi_peng_gang, char card_index, char provider)
{
	cards[card_index] -= 2;

	CHI_PENG_GANG cpg;
	cpg.center_card = card_index;
	cpg.provider = provider;
	cpg.type = "peng";
	chi_peng_gang.push_back(cpg);
}

//暗杠
bool can_an_gang(char* cards, char card_index, int left_cards_num)
{
	if (left_cards_num == 0) return false;
	if (card_index == 0 || card_index>34) return false;

	return cards[card_index] == 4;
}

void an_gang(char* cards, std::vector<CHI_PENG_GANG>& chi_peng_gang, char card_index)
{
	cards[card_index] = 0;

	CHI_PENG_GANG cpg;
	cpg.center_card = card_index;
	cpg.provider = 0;
	cpg.type = "an_gang";
	chi_peng_gang.push_back(cpg);
}

//点杠
bool can_dian_gang(char* cards, char card_index, int left_cards_num)
{
	if (left_cards_num == 0) return false;
	if (card_index == 0 || card_index>34) return false;

	return cards[card_index] == 3;
}

void dian_gang(char* cards, std::vector<CHI_PENG_GANG>& chi_peng_gang, char card_index, char provider)
{
	cards[card_index] = 0;

	CHI_PENG_GANG cpg;
	cpg.center_card = card_index;
	cpg.provider = provider;
	cpg.type = "dian_gang";
	chi_peng_gang.push_back(cpg);
}

//加杠
bool can_jia_gang(char* cards, std::vector<CHI_PENG_GANG>& chi_peng_gang, char card_index, int left_cards_num)
{
	if (left_cards_num == 0) return false;
	if (card_index == 0 || card_index>34) return false;

	if (cards[card_index] != 1) return false;

	for (std::vector<CHI_PENG_GANG>::iterator it = chi_peng_gang.begin(); it != chi_peng_gang.end();++it)
	{
		if (it->center_card == card_index && it->type == "peng") return true;
	}

	return false;
}

void jia_gang(char* cards, std::vector<CHI_PENG_GANG>& chi_peng_gang, char card_index, char provider)
{
	cards[card_index] = 0;

	for (std::vector<CHI_PENG_GANG>::iterator it = chi_peng_gang.begin(); it != chi_peng_gang.end(); ++it)
	{
		if (it->center_card == card_index && it->type == "peng")
		{
			it->type = "jia_gang";
		}
	}
}

