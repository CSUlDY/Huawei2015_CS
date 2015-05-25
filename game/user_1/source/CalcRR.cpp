#include "CalcRR.h"

#include "CalcRR.h"
#include "CalcHandStrength.h"
#include "StrategySwitch.h"
#include "Global.h"
#include "test_poker.h"
//#include <windows.h>


bool JettonProtect(double hand_strength,int current_bet)
{    
    int myself_remain_remain = GetMySelfJetton();
    //�����-��ע��<(äע*4)���ң�HS<0.65����ô������
    if ( (myself_remain_remain - current_bet) < (g_big_blind_bet * 4)
        && (hand_strength < JettonProcetHS) )
    {
        return true;
    }
    
    return false;
}

double GoldKeepStrategy(double hand_strength)
{
    if (hand_strength >= 0.999999)
    {
        return hand_strength;
    }
    int all_player_num = (int)(gAllPlayers.size());

    if (g_residual_turn < 0)
    {
        g_residual_turn = 0;
    }

    if (MustWin())
    {
        return 0.01;
    }

    if (GetGoldGap() > 0 && gAllPlayers.size() > 2)
    {
        hand_strength -= PLAYER_REDUCE_HAND_STRENGTH;
    }

    return hand_strength;


#if 0
    if (g_small_blind_bet <= CHECKOUT_SMALLBLIND)
    {
        if (all_player_num <= CHECKOUT)
        {
            hand_strength -= CHECKOUT_REDUCE_HAND_STRENGTH;
        }
        else
        {
            //���㵱ǰ���������ҪͶע�Ľ��,Сäע+��äע
            int residua_glod = (g_residual_turn /(CHECKOUT + 1)) * (g_small_blind_bet + g_big_blind_bet);

            bool keep = (myself_total_gold - residua_glod) > ((all_player_num * g_init_glod)/CHECKOUT) ? true : false;
            if (keep)
            {
                hand_strength -= CHECKOUT_REDUCE_HAND_STRENGTH;
            }
        }

    }
    else if (g_small_blind_bet <= RISEINRANKT_SMALLBLIND)
    {
        if (all_player_num <= RISEINRANK)
        {
            hand_strength -= RISEINRANK_REDUCE_HAND_STRENGTH;
        }

        else
        {
            //���㵱ǰ���������ҪͶע�Ľ��,Сäע+��äע
            int residua_glod = (g_residual_turn /(RISEINRANK + 1)) * (g_small_blind_bet + g_big_blind_bet);
            bool keep = (myself_total_gold - residua_glod) > ((all_player_num * g_init_glod)/RISEINRANK) ? true : false;
            if (keep)
            {
                hand_strength -= RISEINRANK_REDUCE_HAND_STRENGTH;
            }
        }
    }
#endif
    
}

//FILE* g_fp = fopen("./RR_test.txt","ab+");

double CalcHandStrength()
{
    CardMask DeadCards;
    CardMask_RESET(DeadCards);
    CardMask common_cards;
    CardMask_OR(common_cards,gPlayerCards.common_cards, gPlayerCards.turn_card);
    CardMask_OR(common_cards,common_cards, gPlayerCards.river_card);
    CardMask_OR(DeadCards,common_cards, gPlayerCards.hand_cards);

    //todo �������
    CardStatCount card_stat_count;
    card_stat_count = CalcPlayerStatCount_E(gPlayerCards.hand_cards,DeadCards,common_cards);
    //printf("win rate:%f.\n", (double)card_stat_count.win_count / card_stat_count.total_count);

    LOG("DeadCards=[%s]",
        Poker_maskString(DeadCards));
    LOG(",hand_cards=[%s]",
        Poker_maskString(gPlayerCards.hand_cards));
    LOG(",common_cards=[%s]\n",
        Poker_maskString(common_cards));
    LOG("TotalBet = %d.\n",GetTotalBet());
    LOG("RemainBet = %d.\n",GetRemainBet());
    


    double hand_strength = CalcHandStrength(
        card_stat_count.total_count,
        card_stat_count.win_count,card_stat_count.tie_count,GetCurrentPlayerNum() - 1);
    return hand_strength;
}

//return RR
double CalcRR(int current_bet,double hand_strength)
{
    
    //bet protect
    if (JettonProtect(hand_strength,current_bet))
    {
        return RR_FLOD;
    }

    //��Ҷౣ�����ԣ���֤����
    hand_strength = GoldKeepStrategy(hand_strength);

    //to do �߳ؽ��
    //����ر���

    
    double pot_odds = (double)(GetMySelfBet() + current_bet)/(GetWinJetton(current_bet) + current_bet);
    double RR = hand_strength / pot_odds;

    //�ж�Сäע���,����Ͷ���ʡ�
    //��̭��
    if (g_small_blind_bet <= CHECKOUT_SMALLBLIND)
    {
        RR -= CHECKOUT_REDUCE_RR;
    }
    //������
    else if (g_small_blind_bet <= RISEINRANKT_SMALLBLIND)
    {
        RR -= RISEINRANK_REDUCE_RR;
    }


    return RR;
}

