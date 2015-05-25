#pragma once
#include "tx_poker.h"
#include <string>
#include "CalcCardRate.h"
#include <vector>
#include <map>
//#include "log.h"

#define LOG(...)

typedef struct _Play_Round_Cards
{
    CardMask hand_cards;
    CardMask common_cards;
    CardMask turn_card;
    CardMask river_card;
}Play_Round_Cards;

typedef enum 
{
    PLAYER_IS_NORMAL = 0,
    PLAYER_IS_BUTTON,
    PLAYER_IS_SMALL_BLIND,
    PLAYER_IS_BIG_BLIND,
}PLAYER_TYPE;

typedef struct _Player 
{
    PLAYER_TYPE type; //�������
    int pid;
    unsigned int money;
    std::string jetton; //����
}Player;


typedef enum _Action
{
    ACTION_BLIND,
    ACTION_CHECK,
    ACTION_CALL,
    ACTION_RAISE,
    ACTION_ALL_IN,
    ACTION_FOLD
}Action;

extern Play_Round_Cards gPlayerCards;
extern std::vector<Player> gAllPlayers;

extern uint32 gCurPotTotalbet;
extern int g_small_blind_bet;
extern int g_small_blind_pid;
extern int g_big_blind_bet;
extern int g_big_blind_pid;
extern int g_residual_turn;
extern int g_crruent_round;
extern int g_init_glod;
extern int g_cur_round_glod;

extern bool g_once_flag;

extern bool g_naughties_flag;

extern std::map<int,int> gPersonalityAnalyz;

typedef enum
{
    HOLD_BET_ROUND,      //������ע��
    FLOP_BET_ROUND,      //��������ע��
    TURN_BET_ROUND,      //ת����ע��
    RIVER_BET_ROUND,     //������ע��
    ALL_BET_ROUND_COUNT, //��ע�ּ��� --- �������
    NOT_BET_ROUND,       //����ע��
}BET_ROUND;

extern BET_ROUND gCurBetRound;

typedef struct
{
    int pid;
    int jetton;
    int money;
    int bet;
    std::string action;
}InquirePlayerInfo;

//��һάΪͶע��
//�ڶ�ά���ֵõ���Ϣ����
//����ά������ұ���Ͷע��Ϣ
extern std::vector< std::vector< std::vector<InquirePlayerInfo> > >  gAllInquireMsgInfo;

extern std::map<int,int> gAllCurRoundBet;

typedef enum
{
    ALL_FOLD = 0,           //�����������
    ONE_CALL,               //һ����Ҹ�ע
    MORE_CALL,              //����������Ҹ�ע
    ONE_RAISE_NO_CALL,      //ֻ��һ����Ҽ�ע,����û����Ҹ�ע
    ONE_RAISE_ONEMORE_CALL, //ֻ��һ����Ҽ�ע,����һ����Ҹ�ע
    MORE_RAISE,             //����һ����Ҽ�ע
    PLAYER_ACTION_COUNT
}PLAYER_ACTION;

typedef enum
{
    FRONT_POS = 0,   //ǰ��λ��
    CENTER_POS,      //�м�λ��
    BEHIND_POS,      //����λ��
    SMALL_BLIND_POS, //Сäעλ��
    BIG_BLIND_POS,   //��äעλ��
    MY_POSITION_COUNT
}MY_POSITION;

#define RAND rand()/(RAND_MAX/100)

void InitGlobalData();
void ClearRoundGlobalData();

int CovertMsgPoint(const std::string &msg);
uint32 CovertMsgSuit(const std::string &msg);

void SavePlayer(PLAYER_TYPE type, int pid, uint32 money, std::string &jetton);

void CleanPlayer();

int GetMySelfJetton();
void CleanPlayer();

int GetMinBet();

PLAYER_ACTION GetPlayersAction();

MY_POSITION GetPlayerPosition();
int GetMySelfGlod();

int GetCurrentPlayerNum();

int GetMySelfBet();

PLAYER_ACTION GetPlayersAction();
double GetMadnessRate(std::vector<int> &pids);

int GetMyCurRoundPosition();
//�����ƹ���ע�ĳ�����
int GetTotalBet();

//�Լ�������ʣ�������
int GetRemainBet();

int GetWinJetton(int min_bet);

int GetPlayersTotalMoney(int pid);

int GetGoldGap();

bool MustWin();