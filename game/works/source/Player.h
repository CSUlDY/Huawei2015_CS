
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define GAME_OVER 0xffee

void SetPlayerId(int id);

#define WSADESCRIPTION_LEN 128
#define WSASYSSTATUS_LEN 128

typedef struct WSAData_ {
   int wVersion;
   int wHighVersion;
   char szDescription[WSADESCRIPTION_LEN+1];
   char szSystemStatus[WSASYSSTATUS_LEN+1];
   unsigned short iMaxSockets;
   unsigned short iMaxUdpDg;
   char * lpVendorInfo;
} WSADATA;

/*����������, Ӧ����GAME_OVER*/
//int RoundEntry(int& socket);
int GameOver(void);
void GameStart(void);
int GetPlayerId(void);
int PlayerMsgProc(char* recv_buf);
int RoundEntry(int socket);

/*
1.	player��serverע���Լ���id��name��reg-msg�� 
2.	while������2����������Ҳ���δ������������
a)	����������Ϣ��seat-info-msg��������ׯ��
b)	ǿ��Ѻäע��blind-msg
c)	Ϊÿλ���ַ����ŵ��ƣ�hold-cards-msg
d)	����ǰ��ע�� inquire-msg/action-msg����Σ�
e)	�������Ź����ƣ�flop-msg 
f)	����Ȧ��ע��inquire-msg/action-msg����Σ�
g)	����һ�Ź����ƣ�ת�ƣ���turn-msg
h)	ת��Ȧ��ע�� inquire-msg/action-msg����Σ�
i)	����һ�Ź����ƣ����ƣ���river-msg
j)	����Ȧ��ע��inquire-msg/action-msg����Σ�
k)	������������δ������̯�Ʊȴ�С��showdown-msg
l)	�����ʳط�������pot-win-msg
3.	��������������game-over-msg��
*/
typedef enum MSG_TYPES_
{
    MSG_TYPE_none = 0,
    // ��������player����Ϣ
    MSG_TYPE_reg,
    MSG_TYPE_seat_info,
    MSG_TYPE_blind,
    MSG_TYPE_hold_cards,
    MSG_TYPE_inquire,
    //
    MSG_TYPE_flop,
    MSG_TYPE_turn,
    MSG_TYPE_river,
    MSG_TYPE_showdown,
    MSG_TYPE_pot_win,
    MSG_TYPE_game_over,
    
    // player������������Ϣ
    MSG_TYPE_action_check,
    MSG_TYPE_action_call, 
    MSG_TYPE_action_raise,
    MSG_TYPE_action_all_in,
    MSG_TYPE_action_fold,
    //
} MSG_TYPES;

/* ������Ϣ�ṹ */
typedef struct GAME_MSG_
{
    
} GAME_MSG;

/* ��Ƭ��ɫ */
typedef enum CARD_COLOR_
{
    CARD_COLOR_Unknow,          /* ���ڱ�ʶ���ֵ�δ֪��Ƭ */
    CARD_COLOR_SPADES = 1,
    CARD_COLOR_HEARTS,
    CARD_COLOR_CLUBS,
    CARD_COLOR_DIAMONDS,
} CARD_COLOR;

/* ��Ƭ��С */
typedef enum CARD_POINT_
{
    CARD_POINT_Unknow,      /* ���ڱ�ʶ���ֵ�δ֪��Ƭ */
    CARD_POINT_2 = 2,
    CARD_POINT_3,
    CARD_POINT_4, 
    CARD_POINT_5,
    CARD_POINT_6,
    CARD_POINT_7,
    CARD_POINT_8,
    CARD_POINT_9,
    CARD_POINT_10,
    CARD_POINT_J,
    CARD_POINT_Q,
    CARD_POINT_J,
    CARD_POINT_A,
} CARD_POIN;

/* play card */
typedef struct CARD_
{
    CARD_POIN Point;
    CARD_COLOR Color;
} CARD;

/* ��ҵĴ������ */
typedef struct PLAYER_Action_
{
    ACTION_check,       /* ���ƣ�����ǰ�����ң�ʲôҲ�������ѻ������������ */
    ACTION_call,        /* ��������ǰ������raise������re-raise��Ҳ�����ƣ���call�� */
    
} PLAYER_Action;

/* �����Ϣ */
typedef struct PLAYER_
{
    /* ��̬���� */    
    int PlayerID;
    char PlayerName[32];    /* player name, length of not more than 20 bytes */

    /* ��̬player ��Ϣ */    
    int Status;             /* ��ǰ�ֵĵ�ǰ״̬ */
    int SeatIndex;          /* ��ǰ�ֵ�λ�� */
    CARD HoldCards[2];      /* ѡ�ֵ����ŵ��ƣ���ȷ���� */

} PLAYER;

/* ÿһ����Ϣ */
typedef struct RoundInfo_
{
    int PlayerCount;        /* ��Ҹ��� */
    PLAYER * Players[8];    /* 8����ң���ʱ��ָ�� */
    CARD FlopCards[3];      /* 3�Ź��� */
    CARD TurnCards[1];      /* 1��ת�� */
    CARD RiverCards[1];     /* 1�ź��� */
} RoundInfo;

#endif

