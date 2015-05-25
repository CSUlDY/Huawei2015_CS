#include "StrategyHoldCard.h"
#include "Global.h"
#include "Player.h"

//����,�յ�������Ϣ֮������鱻��ʼ��

typedef enum
{
    VERY_STRENGTH = 0,   //��ǿ��
    STRENGTH,            //ǿ��
    NORMAL,              //��ͨ��
    SPECULARION,         //Ͷ����
    MIXED,               //�����
    STEAL_CARD,          //͵ע��
    FOLD_CARD,           //������
    CARD_STRENGTH_COUNT
}CARD_STRENGTH;

SingleCard g_holdcard[2];

// static Action s_hold_card_action_tab[CARD_STRENGTH_COUNT][PLAYER_ACTION_COUNT][MY_POSITION_COUNT] = 
// {
//     //��ǿ��
//     {   //��ǰ         //����        //����        //Сäע      //��äע
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//     },
// 
//     //ǿ��
//     {
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_FOLD,  ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_CALL,  ACTION_CALL,  ACTION_CALL,  ACTION_CALL,  ACTION_CALL},
//         {ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//     },
// 
//     //�е���
//     {
//         {ACTION_FOLD, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_FOLD, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_FOLD, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL},
//         {ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL}, //TODO:KQs��ע
//         {ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//     },
// 
//     //ǿͶ����
//     {
//         {ACTION_FOLD, ACTION_FOLD, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_CALL,  ACTION_CALL,  ACTION_CHECK},
//         {ACTION_CALL, ACTION_CALL, ACTION_CALL,  ACTION_CALL,  ACTION_CHECK},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL},
//         {ACTION_CALL, ACTION_CALL, ACTION_CALL,  ACTION_CALL,  ACTION_CALL},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//     },
// 
//     //�����
//     {
//         {ACTION_FOLD, ACTION_FOLD, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_CALL,  ACTION_CALL},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_CALL,  ACTION_CALL,  ACTION_CALL},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CHECK},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CHECK},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//     },
// 
//     //͵ע��
//     {
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_RAISE},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//     },
// 
//     //������
//     {
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//         {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
//     },
// };

static Action s_hold_card_action_tab[CARD_STRENGTH_COUNT][PLAYER_ACTION_COUNT][MY_POSITION_COUNT] = 
{
    //��ǿ��
    {   //��ǰ         //����        //����        //Сäע      //��äע
        {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
        {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
        {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
        {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
        {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
        {ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE, ACTION_RAISE},
    },

    //ǿ��
    {
        {ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_RAISE},
        {ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL},
        {ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL},
        {ACTION_FOLD,  ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL},
        {ACTION_CALL,  ACTION_CALL,  ACTION_CALL,  ACTION_CALL,  ACTION_CALL},
        {ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
        },

        //�е���
        {
            {ACTION_FOLD, ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_RAISE},
            {ACTION_FOLD, ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL},
            {ACTION_FOLD, ACTION_CALL, ACTION_CALL, ACTION_CALL, ACTION_CALL},
            {ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL},
            {ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL}, //TODO:KQs��ע
            {ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
        },

        //ǿͶ����
        {
            {ACTION_FOLD, ACTION_FOLD, ACTION_CALL, ACTION_CALL, ACTION_RAISE},
            {ACTION_FOLD, ACTION_FOLD, ACTION_CALL,  ACTION_CALL,  ACTION_CHECK},
            {ACTION_CALL, ACTION_CALL, ACTION_CALL,  ACTION_CALL,  ACTION_CHECK},
            {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL},
            {ACTION_CALL, ACTION_CALL, ACTION_CALL,  ACTION_CALL,  ACTION_CALL},
            {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
            },

            //�����
            {
                {ACTION_FOLD, ACTION_FOLD, ACTION_CALL, ACTION_CALL, ACTION_RAISE},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_CALL,  ACTION_CALL},
                {ACTION_FOLD, ACTION_FOLD, ACTION_CALL,  ACTION_CALL,  ACTION_CALL},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CHECK},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CHECK},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
            },

            //͵ע��
            {
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_RAISE},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                },

                //������
                {
                    {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_CALL},
                    {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                    {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                    {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                    {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                    {ACTION_FOLD, ACTION_FOLD, ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},
                },
};

//TODO:����֮ǰ�г���ici��ע(Raise)�����,���˳��к�ǿ����,��AA,KK,QQ/AKs/AKo���Լ�����ע����,���������涼Ӧ����
static CARD_STRENGTH GetCardStrength()
{
    //AA,KK,QQ,AKs,AKo
    if (   ((g_holdcard[0].point == "A") && (g_holdcard[1].point == "A"))
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "K")) 
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "Q"))
        || ((g_holdcard[0].point == "A") && (g_holdcard[1].point == "K"))
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "A")))
    {
        return VERY_STRENGTH;
    }

    //JJ, 10-10, 99 / AQs, AQo, AJs 
    if (   ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "J"))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "10")) 
        || ((g_holdcard[0].point == "9") && (g_holdcard[1].point == "9"))
        || ((g_holdcard[0].point == "A") && (g_holdcard[1].point == "Q"))
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "A"))
        || ((g_holdcard[0].point == "A") && (g_holdcard[1].point == "J") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "A") && (g_holdcard[0].color == g_holdcard[1].color)))
    {
        return STRENGTH;
    }

    // AJo, A-10s, A-10o, KQs, KQo
    if (   ((g_holdcard[0].point == "A") && (g_holdcard[1].point == "J"))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "A"))
        || ((g_holdcard[0].point == "A") && (g_holdcard[1].point == "10"))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "A"))
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "Q"))
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "K")) )
    {
        return NORMAL;
    }

    // ��88 �� 22 /KJs, K-10s, QJs, Q-10s, J-10s, 10-9s 
    if (   ((g_holdcard[0].point == g_holdcard[1].point))
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "J") && (g_holdcard[0].color == g_holdcard[1].color)) 
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "10") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "J") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "10") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "10") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "9") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "K") && (g_holdcard[0].color == g_holdcard[1].color)) 
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "K") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "Q") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "Q") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "J") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "9") && (g_holdcard[1].point == "10") && (g_holdcard[0].color == g_holdcard[1].color)) )
    {
        return SPECULARION;
    }

    if (   
           ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "J"))
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "10"))
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "J"))
        || ((g_holdcard[0].point == "Q") && (g_holdcard[1].point == "10"))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "10"))
        || ((g_holdcard[0].point == "A") && (g_holdcard[0].color == g_holdcard[1].color)) 
        || ((g_holdcard[0].point == "K") && (g_holdcard[1].point == "9") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "9") && (g_holdcard[1].point == "8") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "8") && (g_holdcard[1].point == "7") && (g_holdcard[0].color == g_holdcard[1].color))     
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "K"))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "K"))
        || ((g_holdcard[0].point == "J") && (g_holdcard[1].point == "Q"))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "Q"))
        || ((g_holdcard[0].point == "10") && (g_holdcard[1].point == "J"))
        || ((g_holdcard[1].point == "A") && (g_holdcard[0].color == g_holdcard[1].color)) 
        || ((g_holdcard[0].point == "9") && (g_holdcard[1].point == "K") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "8") && (g_holdcard[1].point == "9") && (g_holdcard[0].color == g_holdcard[1].color))
        || ((g_holdcard[0].point == "7") && (g_holdcard[1].point == "8") && (g_holdcard[0].color == g_holdcard[1].color)) )
    {
        return MIXED;
    }

    //AA,KK,QQ,AKs,AKo
    if (   ((g_holdcard[0].point == "A") && ((g_holdcard[1].point == "9") || (g_holdcard[1].point == "8") || (g_holdcard[1].point == "7")))
        || ((g_holdcard[0].point == "K") && ((g_holdcard[1].point == "9") || (g_holdcard[1].point == "8") || (g_holdcard[1].point == "7"))) 
        || ((g_holdcard[0].point == "Q") && ((g_holdcard[1].point == "9") || (g_holdcard[1].point == "8") || (g_holdcard[1].point == "7") ))
        || ((g_holdcard[1].point == "A") && ((g_holdcard[0].point == "9") || (g_holdcard[0].point == "8") || (g_holdcard[1].point == "7")))
        || ((g_holdcard[1].point == "K") && ((g_holdcard[0].point == "9") || (g_holdcard[0].point == "8") || (g_holdcard[1].point == "7")))
        || ((g_holdcard[1].point == "Q") && ((g_holdcard[0].point == "9") || (g_holdcard[0].point == "8") || (g_holdcard[1].point == "7"))) )
    {
        return STEAL_CARD;
    }

    return FOLD_CARD;
}

typedef enum
{
    HAVE_A = 0,
    HAVE_K_OVER_8 = 1,
    HAVE_Q_OVER_8 = 2,
    HAVE_J_OVER_8 = 3,
    HAVE_PAIR = 4,
    OTHER_CARD = 5,
    CARD_COUNT
}LESS_PLAYER_CARD_TYPE;

const static int k_less_players = 3;

//0 1 2 -> 1,2,3
Action less_players_tab[CARD_COUNT][k_less_players] = 
{
    //1������      2������       3������
    {ACTION_CHECK, ACTION_CHECK, ACTION_CHECK}, // ��A
    {ACTION_CHECK, ACTION_CHECK, ACTION_FOLD},  // ��K,���������Ƴ���8
    {ACTION_CHECK, ACTION_CHECK, ACTION_FOLD},  // ��Q,���������Ƴ���8
    {ACTION_CHECK, ACTION_FOLD,  ACTION_FOLD},  // ��J,���������Ƴ���8
    {ACTION_CHECK, ACTION_CHECK, ACTION_CHECK}, // �ж�
    {ACTION_FOLD,  ACTION_FOLD,  ACTION_FOLD},  // ������
};

bool IsBiggerThan8(const std::string& point)
{
    return (point == "A") || (point == "K") || (point == "Q") || (point == "J") || (point == "10") || (point == "9") || (point == "8");
}

//��С���ӷ���true
bool IsLessPair()
{
    if (g_holdcard[0].point != g_holdcard[1].point)
    {
        return false;
    }

    if (IsBiggerThan8(g_holdcard[0].point))
    {
        return false;
    }

    return true;
}

LESS_PLAYER_CARD_TYPE GetLessCardType()
{
    if ((g_holdcard[0].point == "A") || (g_holdcard[1].point == "A"))
    {
        return HAVE_A;
    }

    if (((g_holdcard[0].point == "K") && IsBiggerThan8(g_holdcard[1].point))
        || ((g_holdcard[1].point == "K") && IsBiggerThan8(g_holdcard[0].point)))
    {
        return HAVE_K_OVER_8;
    }

    if (((g_holdcard[0].point == "Q") && IsBiggerThan8(g_holdcard[1].point))
        || ((g_holdcard[1].point == "Q") && IsBiggerThan8(g_holdcard[0].point)))
    {
        return HAVE_Q_OVER_8;
    }

    if (((g_holdcard[0].point == "J") && IsBiggerThan8(g_holdcard[1].point))
        || ((g_holdcard[1].point == "J") && IsBiggerThan8(g_holdcard[0].point)))
    {
        return HAVE_J_OVER_8;
    }

    if (g_holdcard[0].point == g_holdcard[1].point)
    {
        return HAVE_PAIR;
    }

    return OTHER_CARD;
}

Action LessPlayersAction(int players)
{
    LESS_PLAYER_CARD_TYPE card_type = GetLessCardType();

    return less_players_tab[card_type][players - 1];  // 1,2,3 --> 0,1,2
}

void GetRaisePids(std::vector<int>& raise_pids)
{
    raise_pids.clear();

    const std::vector<InquirePlayerInfo>& player_inquare = gAllInquireMsgInfo[gCurBetRound].back();

    for (std::vector<InquirePlayerInfo>::const_iterator iter = player_inquare.begin(); 
        iter != player_inquare.end();
        ++iter)
    {
        if (iter->pid == GetPlayerId())
        {
            continue;
        }

        if ((iter->action != "fold") && (iter->action != "blind"))
        {
            raise_pids.push_back(iter->pid);
        }
    }
}

StrategyInof GetHoldCardAction()
{
    //��һ��action��ȡ,���ڻ�����
    Action action = ACTION_FOLD;
    CARD_STRENGTH card_strength = GetCardStrength();
    PLAYER_ACTION player_action = GetPlayersAction();
    MY_POSITION postion = GetPlayerPosition();
    action = s_hold_card_action_tab[card_strength][player_action][postion];

    // Ѻע���̫��ֱ������.
    int min_bet = GetMinBet();

    //2�����ʱ,��Щ������ҲҪ����
    if (ACTION_FOLD == action) 
    {
        if ((gAllPlayers.size() == 2) && (GetGoldGap() < 10000))
        {
            action = LessPlayersAction(gAllPlayers.size() - 1);
        }
    }
    
    //���ʣ���������Ѿ���ע����,��ʾ���г����Ѿ�����.����All in
    if (GetRemainBet() == GetTotalBet())
    {
        action = ACTION_CHECK;
        LOG("no jetton!\n");
    }
    
    if (MustWin())
    {
        action = ACTION_FOLD;
    }

    if (ACTION_FOLD != action)
    {
        if ((GetTotalBet() + min_bet) / (GetTotalBet() + GetRemainBet()) > 0.5)
        {
            action = ACTION_CHECK;
            LOG("Jetton Protected!\n");
        }
    }

    //Ӧ�ö�����ʱ��,��������ѿ��ƻ���,��Ҫ����.
    if (ACTION_FOLD == action)
    {
        if (0 == min_bet)
        {
            action = ACTION_CHECK;
            LOG("min_bet = 0.\n");
        }
    }

    LOG("Hold result :%d. %d.\n", action, min_bet);
    StrategyInof strategy;
    strategy.action = action;
    strategy.money = min_bet;

    return strategy;
}