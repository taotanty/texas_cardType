/*
  牌型基本定义
 */
#include <iostream>
#include <string>
#include <vector>

namespace texascardtype{
#define MIN_CARD 0
#define MAX_CARD 51
#define INVALID_CARD 52
#define RANK_CNT 13
  typedef unsigned short uint8;

typedef  enum{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYALFLUSH,
    INVALID_TYPE
  } Card_Type;

  typedef enum{
    SPADE,//黑桃
    HEART,//红桃
    DIAMOND,//♦️
    CLUB,//♣️
  }Card_Suit;

  std::string Card_Type_Str_EN[]={"High Card","One Pair","Two Pair","Three of a Kind","Straight","Flush","Full House","Four of a Kind","Four of a Kind","Straight Flush","Royal Straight Flush"};
  std::string Card_Suit_Str[]={"s","h","d","c"};//spade,heart,diamond,club
  std::string Card_Num_Str[]={"2","3","4","5","6","7","8","9","T","J","Q","K","A"};
  uint8 Card_Num[]={0,1,2,3,4,5,6,7,8,9,10,11,12};
  std::string Card_Type_Str_CH[]={"高牌","一对","两对","三条","顺子","同花","满堂红","四条","同花顺","皇家同花顺"};

  void merger_cards(uint8* arr1,uint8 len1,uint8 *arr2,uint8 len2,uint8* m_arr){
    memcpy(m_arr,arr1,len1);
    memcpy(m_arr+len1,arr2,len2);
  }




    uint8 getCardNum(uint8 card){
    return card/RANK_CNT;
  }

  Card_Suit getCardSuit(uint8 card){
    return static_cast<Card_Suit>(card%RANK_CNT);
  }
  bool comps(uint8 card1,uint8 card2){
    uint8 logic1=getCardNum(card1);
    uint8 logic2=getCardNum(card2);
    return logic1<logic2;

  }


  bool check_card(uint8 card){
    if(card<MIN_CARD or card>MAX_CARD)
      return false;
    else
      return true;
  }

  uint8 make_card(uint8 num,Card_Suit suit){
    return suit*RANK_CNT+num;
  }
  std::string card_to_str(uint8 card){
    uint8 num=getCardNum(card);
    Card_Suit suit=getCardSuit(card);
    std::string str=Card_Num_Str[num]+Card_Suit_Str[suit];
    return str;
  }

  void sort_cards(uint8* cards,uint8 length){
    std::vector<uint8> vec(cards,cards+length);
    std::sort(vec.begin(),vec.end(),comps);
    std::vector<uint8>::iterator iter;
    uint8 i=0;
    for(iter=vec.begin();iter!=vec.end();iter++,i++){
      cards[i]=*iter;
    }
};