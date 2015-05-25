#include "MsgProc.h"
#include <string>
#include<vector>
//#include <WinSock2.h>
#include <sys/socket.h>
#include "Player.h"
#include "StrategyHoldCard.h"
#include "Global.h"
#include "test_poker.h"
#include "SendAction.h"

typedef enum {
    HOLD_CARD = 0, //0: ������
    FLOP_CARD, //1����3�Ź���
    TURN_CARD, //2����һ��ת��
    RIVER_CARD, //3����һ�ź���
    REV_CARD
}PROC_CARD_MSG_TYPE;

int imp_ProcCardMsg(std::string msg, PROC_CARD_MSG_TYPE call_type);

void OneTurnClear()
{
    g_small_blind_bet = 0;
    g_small_blind_pid = -1;
    g_big_blind_bet = 0;
    g_big_blind_pid = -1;
    g_crruent_round = 0;
    g_once_flag = true;
    g_init_glod = 0;
    g_naughties_flag = false;
}

/*void MsgPrint(std::string content)
{
    printf("\n---------------\n");
    printf("%s",content.c_str());
}*/

void SplitStringByChar(const std::string& str, char seperator, std::vector<std::string>& substrList)
{
    substrList.clear();

    std::string strTmp1 = trim(str);
    if (strTmp1.empty()) 
    {
        return;
    }
    std::string::size_type posSubstring = 0; // Position of substring
    std::string::size_type posSeperator = strTmp1.find(seperator, posSubstring); // position of seperator
    if (std::string::npos == posSeperator)
    {
        substrList.push_back(trim(strTmp1));
        return;
    }
    while(std::string::npos != posSeperator) 
    {
        std::string strTmp = trim(strTmp1.substr(posSubstring, posSeperator - posSubstring));
        if (!strTmp.empty())
        {
            substrList.push_back(strTmp);
        }
        
        posSubstring = posSeperator + 1;
        posSeperator = strTmp1.find(seperator, posSubstring);
    }
    strTmp1 = trim(str.substr(posSubstring));
    if (!strTmp1.empty())
    {
         substrList.push_back(strTmp1);
    }
   
}

static bool IsSpace(char c) 
{
    return (' ' == c) || ('\t' == c) || ('\r' == c) || ('\n' == c);
}

std::string trim(const std::string& str)
{
    size_t len = str.length();
    if (0 == len) 
    {
        return "";
    }

    const char *p = str.c_str();

    size_t beginPos = 0;
    for (; (IsSpace(p[beginPos]) && (beginPos < len)); ++beginPos) 
    {
    }

    if (beginPos == len) 
    {
        return "";
    }

    size_t endPos = len - 1;
    // ��ʱendPos�����ܵ���0��-1��λ�ã������������� endPos > 0
    for (; IsSpace(p[endPos]) ; --endPos) 
    {
    }

    return str.substr(beginPos, (endPos - beginPos) + 1);
}

std::string FindMsgContent(std::string msg)
{
    std::string rcvMsg = msg;

    std::string::size_type first = rcvMsg.find_first_of('/');
    std::string::size_type last = rcvMsg.find_last_of('/');
    
    if ((first == std::string::npos) || (last == std::string::npos) || (last == first))
    {
        return "";//��Ϣ����
    }
    return trim(rcvMsg.substr(first+1,last-first-1));
}


//��ȡһ�е����ݣ���������λ�ø���
std::string GetLineField(std::string::size_type &fieldBegin,std::string msgContent)
{   
    if (fieldBegin == std::string::npos)
    {
        return "";
    }
    std::string::size_type firstNotEol = msgContent.find_first_not_of(SPACE_CODE,fieldBegin);

    if (firstNotEol == std::string::npos)
    {
        return "";
    }

    std::string::size_type fieldEnd = msgContent.find(EOL,firstNotEol);

    std::string line = trim(msgContent.substr(firstNotEol,fieldEnd-firstNotEol));    
    
    fieldBegin = fieldEnd;

    return line;
}

/*
seat-info-msg := seat/ eol
button: pid jetton eol
small blind: pid jetton eol
big blind: pid jetton eol
pid jetton eol
pid jetton eol
pid jetton eol
/seat eol

server->player
�����ֵ�������Ϣ�������ƺͺ�ע����
jetton(����)Ϊ0��ѡ�ֽ������뱾����Ϸ

*/

int ProcSeatInfoMsg(std::string msg)
{
    //���м����Ϣ���ҳ���
    std::string info = FindMsgContent(msg);
    if (info.empty())
    {
        return 0;
    }

    std::vector<std::string> contentVec;

    //������ÿһ���ֶ�
    std::string::size_type location = 0;

    std::string fieldContent = GetLineField(location,info);//button
    if (fieldContent.empty())
    {
        return 0;
    }

    ClearRoundGlobalData();  //���֮ǰ�����һ����Ϣ

    std::string::size_type begin = fieldContent.find(':');   
    if (std::string::npos == begin)
    {
        return 0;
    }

    fieldContent = fieldContent.substr(begin+1,std::string::npos);
    SplitStringByChar(fieldContent,' ',contentVec);
    if (contentVec.size() < 3)
    {
        return 0;
    }
   
    SavePlayer(PLAYER_IS_BUTTON, atoi(contentVec[0].c_str()), atoi(contentVec[2].c_str()), contentVec[1]);

    fieldContent = GetLineField(location,info);//small blind
    if (fieldContent.empty())
    {
        return 0;
    }
    begin = fieldContent.find(':');
    if (std::string::npos == begin)
    {
        return 0;
    }
    fieldContent = fieldContent.substr(begin+1,std::string::npos);
    SplitStringByChar(fieldContent,' ',contentVec);
	if (contentVec.size() < 3)
    {
        return 0;
    }
    SavePlayer(PLAYER_IS_SMALL_BLIND, atoi(contentVec[0].c_str()), atoi(contentVec[2].c_str()), contentVec[1]);
    g_small_blind_pid = atoi(contentVec[0].c_str());

    fieldContent = GetLineField(location,info);//big blind
    if (!fieldContent.empty())
    {
        begin = fieldContent.find(':');
        fieldContent = fieldContent.substr(begin+1,std::string::npos);
        SplitStringByChar(fieldContent,' ',contentVec);
        if (contentVec.size() < 3)
        {
        	return 0;
        }
        SavePlayer(PLAYER_IS_BIG_BLIND, atoi(contentVec[0].c_str()), atoi(contentVec[2].c_str()), contentVec[1]);
        g_big_blind_pid = atoi(contentVec[0].c_str());
        fieldContent = GetLineField(location,info);    
    }

    while (!fieldContent.empty())
    {
        SplitStringByChar(fieldContent,' ',contentVec);
        if (contentVec.size() < 3)
        {
        	return 0;
        }
        SavePlayer(PLAYER_IS_NORMAL, atoi(contentVec[0].c_str()), atoi(contentVec[2].c_str()), contentVec[1]);

        fieldContent = GetLineField(location,info);
    }
    
    g_once_flag = false;

    g_residual_turn--;

    return 0;
}

/*
blind-msg := blind/ eol
pid: bet eol
pid: bet eol
/blind eol

server���Զ�äע
äע����Զ��۳�äע���
��äע���ΪСäע��2��

*/
int ProcBlindMsg(std::string msg)
{
    //���м����Ϣ���ҳ���
    std::string info = FindMsgContent(msg);
    if (info.empty())
    {
        return 0;
    }
    std::vector<std::string> contentVec;

    //������ÿһ���ֶ�
    std::string::size_type location = 0;
    std::string fieldContent = GetLineField(location,info);
    if (fieldContent.empty())
    {
        return 0;
    }
    std::string::size_type begin = fieldContent.find(':');
    std::string pid = fieldContent.substr(0,begin);
    std::string bet = trim(fieldContent.substr(begin+1,std::string::npos));
    if (g_small_blind_pid == atoi(pid.c_str()))
    {
        g_small_blind_bet = atoi(bet.c_str());
    }
    else if (g_big_blind_pid == atoi(pid.c_str()))
    {
        g_big_blind_bet = atoi(bet.c_str());
    }
    /*MsgPrint(fieldContent);
    MsgPrint(pid);
    MsgPrint(bet);*/

    fieldContent = GetLineField(location,info);
    if (fieldContent.empty())
    {
        return 0;
    }
    begin = fieldContent.find(':');
    pid = fieldContent.substr(0,begin);
    bet = trim(fieldContent.substr(begin+1,std::string::npos));
    /*MsgPrint(fieldContent);
    MsgPrint(pid);
    MsgPrint(bet);*/
    if (g_small_blind_pid == atoi(pid.c_str()))
    {
        g_small_blind_bet = atoi(bet.c_str());
    }
    else if (g_big_blind_pid == atoi(pid.c_str()))
    {
        g_big_blind_bet = atoi(bet.c_str());
    }
    

    return 0;
}


/*
hold-cards-msg := hold/ eol
color point eol
color point eol
/hold eol
server��2�ŵ���
*/
int ProcHoldCardMsg(std::string msg)
{
    LOG("Hold Card msg: %s.\n", msg.c_str());
    gAllInquireMsgInfo.clear();
    gAllInquireMsgInfo.resize(ALL_BET_ROUND_COUNT);

    gCurBetRound = HOLD_BET_ROUND;
    g_crruent_round = 1;
    return imp_ProcCardMsg(msg, HOLD_CARD);
}


/*
0: ������
1����3�Ź���
2����һ��ת��
3����һ�ź���
*/
int imp_ProcCardMsg(std::string msg, PROC_CARD_MSG_TYPE call_type)
{
    //���м����Ϣ���ҳ���
    std::string info = FindMsgContent(msg);
    if (info.empty())
    {
        return 0;
    }
    std::vector<std::string> contentVec;

    //������ÿһ���ֶ�
    std::string::size_type location = 0;

    std::string fieldContent = GetLineField(location,info);

    int count = 0;
    while(!fieldContent.empty())
    {

        SplitStringByChar(fieldContent,' ',contentVec);
		if (contentVec.size() < 2)
        {
            return 0;
        }

        unsigned int suit = CovertMsgSuit(contentVec[0]);
        int rank = CovertMsgPoint(contentVec[1]);  

        if (call_type == HOLD_CARD) //������
        {
            g_holdcard[count].color = contentVec[0];
            g_holdcard[count].point = contentVec[1];

            if(rank >= 0)
            {
                int cardIndex = MAKE_CARD(rank, suit);
                CardMask_SET(gPlayerCards.hand_cards, cardIndex);
            }

            ++count;
            if (2 == count)
            {
                break;
            }
        }
        else if (call_type == FLOP_CARD)    //��3�Ź���
        {
            if(rank >= 0)
            {
                int cardIndex = MAKE_CARD(rank, suit);
                CardMask_SET(gPlayerCards.common_cards, cardIndex);
            }
        }
        else if (call_type == TURN_CARD)    //��ת��
        {
            if(rank >= 0)
            {
                int cardIndex = MAKE_CARD(rank, suit);
                CardMask_SET(gPlayerCards.turn_card, cardIndex);
            }
        }
        else if (call_type == RIVER_CARD)   //������
        {
            if(rank >= 0)
            {
                int cardIndex = MAKE_CARD(rank, suit);
                CardMask_SET(gPlayerCards.river_card, cardIndex);
            }
        }



        fieldContent = GetLineField(location,info);
    }
/*
    printf("\n(%d) Fa CardMsg: %s \n", call_type, Poker_maskString(gPlayerCards.hand_cards));
    printf("(%d) Fa CardMsg: %s \n", call_type, Poker_maskString(gPlayerCards.common_cards));
    printf("(%d) Fa CardMsg: %s \n", call_type, Poker_maskString(gPlayerCards.turn_card));
    printf("(%d) Fa CardMsg: %s \n", call_type, Poker_maskString(gPlayerCards.river_card));
*/

    return 0;
}

/*

inquire-msg := inquire/ eol
(pid jetton money bet blind | check | call | raise | all_in | fold eol)2-6
total pot: num eol
/inquire eol

server��playerѯ���ж�����
playerֻ���յ���ѯ����Ϣ֮��, ���ܷ���action��Ϣ
ѯ����Ϣ�л�����������ݣ�
������ң�������ѯ���ߺ�äע�������г��룬��ǰͶע��������һ����Чaction��˳���ɽ���Զ���У��ϼ����ڵ�һ��
��ǰ�׳��ܽ�����Ͷע���ۼƣ�


*/
int ProcInquireMsg(std::string msg)
{
    LOG("Inquire:%s.\n", msg.c_str());
    //���м����Ϣ���ҳ���
    std::string info = FindMsgContent(msg);
    if (info.empty())
    {
        return 0;
    }
    std::string::size_type totalLocation = info.find("total pot");

    std::string player = info.substr(0,totalLocation);
    std::vector<std::string> contentVec;
    //������ÿһ���ֶ�
    std::string::size_type location = 0;
    std::string fieldContent;
    fieldContent = GetLineField(location,player);

    std::vector<InquirePlayerInfo> player_infos;
    while(!fieldContent.empty())
    {
        SplitStringByChar(fieldContent,' ',contentVec);
        if (contentVec.size() < 5)
        {
            return 0;
        }
        
        InquirePlayerInfo tmp_player_info;
        tmp_player_info.pid = atoi(contentVec[0].c_str());
        tmp_player_info.jetton = atoi(contentVec[1].c_str());
        tmp_player_info.money = atoi(contentVec[2].c_str());
        tmp_player_info.bet = atoi(contentVec[3].c_str());
        tmp_player_info.action = contentVec[4];
        player_infos.push_back(tmp_player_info);

        //printf("inquire: pid<%d> jetton:%d,money:%d,bet:%d,action:%s\n", tmp_player_info.pid, tmp_player_info.jetton,tmp_player_info.money,tmp_player_info.bet,tmp_player_info.action.c_str());

        fieldContent = GetLineField(location,player);
    }     
    
    gAllInquireMsgInfo[gCurBetRound].push_back(player_infos);

    std::string total = info.substr(totalLocation,std::string::npos);
    std::string::size_type pos = total.find(':');

    std::string num = trim(total.substr(pos+1,std::string::npos));
    gCurPotTotalbet = atoi(num.c_str());
    //printf("total pot:%d\n", gCurPotTotalbet);

#if 0
    char buff[100];
    memset(buff,0,100);
    printf("put you action [check | call | raise num | all_in | fold ]: ");
    int size = scanf("%s",buff);

    std::string sendMsg = buff;
    printf("you action is :%s,len:%d\n",sendMsg.c_str(),(int)strlen(sendMsg.c_str()));

    sendMsg += "\n";    

    send(my_sock_client, sendMsg.c_str(),(int)strlen(sendMsg.c_str())+1, 0);
#else
    if (HOLD_BET_ROUND == gCurBetRound)
    {
        SendAction(GetHoldCardAction());
    }
    else
    {
        SendAction(ChooseStrategy());
    }
#endif    
    return 0;
}

/*
flop-msg := flop/ eol
color point eol
color point eol
color point eol
/flop eol
server�������Ź�����
�㲥��Ϣ������playerͬʱ�յ�

*/
int ProcFolpMsg(std::string msg)
{
	gCurBetRound = FLOP_BET_ROUND;
    g_crruent_round = 2;
    return imp_ProcCardMsg(msg, FLOP_CARD);
}

/*
turn-msg := turn/ eol
color point eol
/turn eol
server����һ��ת��
�㲥��Ϣ������playerͬʱ�յ�

*/
int ProcTurnMsg(std::string msg)
{
    g_crruent_round = 3;
    gCurBetRound = TURN_BET_ROUND;
    return imp_ProcCardMsg(msg, TURN_CARD);
}

/*
river-msg := river/ eol
color point eol
/river eol

server����һ�ź���
�㲥��Ϣ������playerͬʱ�յ�

*/

int ProcRiverMsg(std::string msg)
{
    g_crruent_round = 4;
    gCurBetRound = RIVER_BET_ROUND;
    return imp_ProcCardMsg(msg, RIVER_CARD);
}


/*
showdown-msg := showdown/ eol
common/ eol
(color point)5 eol
/common eol
(rank: pid color point color point eol)2-6
/showdown eol

��2�˻�2������δ���ƣ���server̯��
�㲥��Ϣ������playerͬʱ�յ�
������ҵĵ��ƽ���������

*/
int ProcShowdownMsg(std::string msg)
{
    //���м����Ϣ���ҳ���
    std::string info = FindMsgContent(msg);
    if (info.empty())
    {
        return 0;
    }
    
    //ȡ��common
    std::string common = FindMsgContent(info);
    if (common.empty())
    {
        return 0;
    }

    LOG("shutdown: %s.\n", msg.c_str());
    //������ÿһ���ֶ�
    std::string::size_type location = 0;
    std::string fieldContent;
    std::vector<std::string> contentVec;
    fieldContent = GetLineField(location,common);
    while(!fieldContent.empty())
    {
        SplitStringByChar(fieldContent,' ',contentVec);
        if (contentVec.size() < 2)
        {
            return 0;
        }
        std::string color = contentVec[0];
        std::string point = contentVec[1];
        contentVec.clear();
        fieldContent = GetLineField(location,common);
    }  

    location = info.find("rank:");
    if (location == std::string::npos)
    {
        return 0;
    }

    std::string rankMsg = trim(info.substr(location));

    location = 0;
    fieldContent = GetLineField(location,rankMsg);
    while(!fieldContent.empty())
    {
        SplitStringByChar(fieldContent,' ',contentVec);
        if (contentVec.size() < 6)
        {
            return 0;
        }
        std::string rank = contentVec[0].erase(':');
        std::string pid = contentVec[1];
        std::string color1 = contentVec[2];
        std::string point1 = contentVec[3];
        std::string color2 = contentVec[4];
        std::string point2 = contentVec[5];
        contentVec.clear();
        LOG("rank:%s,pid:%s, color1:%s,point1:%s,color2:%s,point2:%s. \n", rank.c_str(), pid.c_str(), color1.c_str(), point1.c_str(),
            color2.c_str(), point2.c_str());
        fieldContent = GetLineField(location,rankMsg);
    }     

    return 0;
}


void PersonalityAnalyz()
{
    std::map<int,int> temp_ids;

    std::vector< std::vector<InquirePlayerInfo> >::iterator it = gAllInquireMsgInfo[HOLD_BET_ROUND].begin();
    for (; it != gAllInquireMsgInfo[HOLD_BET_ROUND].end(); ++it)
    {
        std::vector<InquirePlayerInfo>::iterator iter = it->begin();

        for (; iter != it->end(); ++iter)
        {
            if ("fold" != iter->action && "blind" != iter->action)
            {
                temp_ids[iter->pid] = 1;

                if( gPersonalityAnalyz.end() != gPersonalityAnalyz.find(iter->pid))
                {
                    gPersonalityAnalyz[iter->pid] = gPersonalityAnalyz[iter->pid] + 1;
                }
                else
                {
                    gPersonalityAnalyz[iter->pid] = 1;
                }

            }
        }

    }

    std::vector< std::vector<InquirePlayerInfo> >::iterator it1 = gAllInquireMsgInfo[FLOP_BET_ROUND].begin();
    for (; it1 != gAllInquireMsgInfo[FLOP_BET_ROUND].end(); ++it1)
    {
        std::vector<InquirePlayerInfo>::iterator iter = it1->begin();

        for (; iter != it1->end(); ++iter)
        {
            if ("fold" != iter->action && "blind" != iter->action)
            {
                if (temp_ids.end() == temp_ids.find(iter->pid))
                {
                    //��һ������û��
                    if( gPersonalityAnalyz.end() != gPersonalityAnalyz.find(iter->pid))
                    {
                        gPersonalityAnalyz[iter->pid] = gPersonalityAnalyz[iter->pid] + 1;
                    }
                    else
                    {
                        gPersonalityAnalyz[iter->pid] = 1;
                    }
                }
            }
        }
    }


}

/*
pot-win-msg := pot-win/ eol
(pid: num eol)+
/pot-win eol
server�����ʳط�����
�㲥��Ϣ������playerͬʱ�յ�

*/
int ProcPotWinMsg(std::string msg)
{
    /*PersonalityAnalyz();

    std::map<int,int>::iterator it = gPersonalityAnalyz.begin();
    for (; it != gPersonalityAnalyz.end(); ++it)
    {
        //printf("~~~~~~~~~~~pid=[%d],num=[%d]~~~~~~~~\n",it->first,it->second);
    }*/

    //���м����Ϣ���ҳ���
    std::string info = FindMsgContent(msg);
    if (info.empty())
    {
        return 0;
    }    
   
    //������ÿһ���ֶ�
    std::string::size_type location = 0;
    std::string fieldContent;
    fieldContent = GetLineField(location,info);
    while(!fieldContent.empty())
    {
        std::string::size_type pos = fieldContent.find(':');
        
        if (std::string::npos == pos)
        {
            return 0;
        }
        std::string pid = trim(fieldContent.substr(0,pos));
        std::string num = trim(fieldContent.substr(pos+1,std::string::npos));
        LOG("Win:%s = %s.\n", pid.c_str(), num.c_str());
        fieldContent = GetLineField(location,info);
    }

    OneTurnClear();

    return 0;
}



