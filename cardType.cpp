/*
  cardType.cpp
  牌型判断算法
 */
#include "cardType.h"

#define MAX_CENTERCOUNT 5
namespace texascardtype{
  cardType::cardType(){

    for(uint8 i=0;i<4;i++){
      stAnalyseData data={};
      data.count=0;
      data.len=0;
      data.num=new uint8[MAX_CENTERCOUNT-1];
      m_mapAnalyResult->insert(std::pair<uint8,stAnalyseData>(i,data));
    }
  }

  cardType::~cardType(){
    mapAnalyseData::iterator iter=m_mapAnalyResult->begin();
    for(;iter!=m_mapAnalyResult->end();iter++){
      delete iter->second.num;
      iter->second.num=NULL;
      m_mapAnalyResult->erase(iter);
    }
    m_mapAnalyResult->clear();
    delete []m_mapAnalyResult;
    m_mapAnalyResult=NULL;

  }

  Card_Type cardType::countMaxCardType(uint8* hand_cards,uint8 hand_length,
                                       uint8* board_cards,uint8 board_length,uint8* max_cards,uint8* high_cards){
    if(hand_cards==NULL or board_cards==NULL) return INVALID_TYPE;
    if(hand_length!=2 or board_length!=5) return INVALID_TYPE;
    //std::vector<uint8> vec_hand_cards;
    //std::vector<uint8> vec_board_cards;
    //for(uint8 i=0;i<hand_length;i++){
    //  vec_hand_cards.push_back(*(hand_cards+i));
    //}
    //for (uint8 i=0;i<board_length;i++){
    //  vec_board_cards.push_back(*(board_cards+i));
    //}
    uint8* tmp_cards=new uint8((hand_length+board_length)*sizeof(uint8));
    memset(tmp_cards,0,sizeof(uint8)*(hand_length+board_length));
    merger_cards(hand_cards,hand_length,board_cards,board_length,tmp_cards);
    sort_cards(tmp_cards,hand_length+board_length);
    memcpy(max_cards,tmp_cards,MAX_CENTERCOUNT);
    Card_Type max_type=countCardType(max_cards,MAX_CENTERCOUNT);
    Card_Type tmp_type=HIGH_CARD;

    max_type=combine(hand_length+board_length,MAX_CENTERCOUNT,tmp_cards,max_cards,max_type);

  }


  Card_Type cardType::combine(uint8 n,uint8 m,uint8* tmp_cards,uint8* max_cards,Card_Type max_type){
    uint8 i=0,j=0;
    uint8* combine_cards=new uint8(m*sizeof(uint8));
    for(i=n;i>m;i--){
      combine_cards[m]=tmp_cards[i];
      if(m>0)
        max_type=combine(i-1,m-1,tmp_cards,max_cards,max_type);
      else{
        Card_Type tmp_card_type=countCardType(combine_cards,MAX_CENTERCOUNT);
        if(compareCard(max_cards,combine_cards)==2){
          memcpy(max,cards,combine_cards,MAX_CENTERCOUNT);
          max_type=tmp_card_type;
          return max_type;
        }
      }
    }
  }

  //retrun value:0-euqal,1-max_cards>five_cards,2-max_cards<five_cards
  uint8 cardType::compareCard(uint8* max_cards,uint8* five_cards){
    Card_Type first_type=
  }

  Card_Type cardType::countCardType(uint8* five_cards,uint8 len){
    bool bsame_color=true;
    bool bline_card=true;
    uint8 first_value=getCardNum(five_cards[1]);
    Card_Suit first_suit=getCardSuit(five_cards[1]);
    for(uint8 i=0;i<len;i++){
      uint8 tmp_value=getCardNum(five_cards[i]);
      uint8 tmp_suit=getCardSuit(five_cards[i]);
      if(tmp_suit!=first_suit){
        bsame_color=false;
      }

      if (first_value-(i-1)!=tmp_value){
        bline_card=false;
      }

      if(bsame_color==false and bline_card==false){
        break;
      }
   }
    //处理最小顺的情况(因为此时它们的数字不是连着的)
    if(bline_card==false and len==MAX_CENTERCOUNT){
      uint8 tmp_value=getCardNum(five_cards[1]);
      if(tmp_value==Card_Num[RANK_CNT-1]){
        bool flag=true;
        for(uint8 i=2;i<len;i++){
          uint8 logic_value=getCardNum(five_cards[i]);
          if(logic_value!=(MAX_CENTERCOUNT-i)){
            flag=false;
            break;
          }
        }
        if(flag==true)
          bline_card==true;
      }
    }
    uint8 second_value=getCardNum(five_cards[2]);
    uint8 second_suit=getCardSuit(five_cards[2]);
    if(bsame_color==true and bline_card==true and second_value==Card_Num[RANK_CNT-2]){
      return ROYALFLUSH;
    }
    //straight
    if(bsame_color==false and bline_card==true){
      return STRAIGHT;
    }
    //flush
    if(bsame_color==true and bline_card==false){
      return FLUSH;
    }

    //straight_flush
    if(bsame_color==true and bline_card==true){
      return STRAIGHT_FLUSH;
    }
    ///------剩下的需要数值分析了
    analyse_cards(five_cards,len);
    return countAnalyseCardsType();
  }

  Card_Type cardType::countAnalyseCardsType(){
    mapAnalyseData::iterator iter=m_mapAnalyResult->find(3);
    if(iter!=m_mapAnalyResult->end()){
      if(iter->second.count==1){
        return FOUR_OF_A_KIND;
      }
    }

   iter=m_mapAnalyResult->find(2);
   mapAnalyseData::iterator iter1=m_mapAnalyResult->find(1);
   if(iter!=m_mapAnalyResult->end() and iter1!=m_mapAnalyResult->end()){
      if(iter1->second.count==2){
        return TWO_PAIR;
      }
      if(iter->second.count==1 and iter1->second.count==1){
        return FULL_HOUSE;
      }
      if(iter->second.count==1){
        return THREE_OF_A_KIND;
      }
      if(iter1->second.count==1){
        return ONE_PAIR;
      }
    }
   return HIGH_CARD;
  }


  void cardType::resetAnalyseData(){
    mapAnalyseData::iterator iter=m_mapAnalyResult->begin();
    for(;iter!=m_mapAnalyResult->end();iter++){
      iter->second.count=0;
      iter->second.len=0;
      memset(iter->second.num,0,(MAX_CENTERCOUNT-1)*sizeof(uint8));
    }
  }

  void cardType::getAnalyseData(Card_Type card_type,const uint8* five_cards,uint8* high_cards){
    switch(card_type){
    case ROYALFLUSH:
    case STRAIGHT_FLUSH:
    case FLUSH:
    case STRAIGHT:
    case FULL_HOUSE:
    {
      memcpy(high_cards,five_cards,MAX_CENTERCOUNT);
    }
      break;
    case FOUR_OF_A_KIND:{
      mapAnalyseData::iterator iter=m_mapAnalyResult->find(3);
      assert(iter!=m_mapAnalyResult->end());
      memcpy(high_cards,iter->second.num,iter->second.len);
      break;
    }
    case THREE_OF_A_KIND:{
      mapAnalyseData::iterator iter=m_mapAnalyResult->find(2);
      assert(iter!=m_mapAnalyResult->end());
      memcpy(high_cards,iter->second.num,iter->second.len);
    }
      break;
    case TWO_PAIR:{
      mapAnalyseData::iterator iter=m_mapAnalyResult->find(1);
      assert(iter!=m_mapAnalyResult->end());
      memcpy(high_cards,iter->second.num,iter->second.len);
    }
      break;
    case ONE_PAIR:{
      mapAnalyseData::iterator iter=m_mapAnalyResult->find(1);
      assert(iter!=m_mapAnalyResult->end());
      memcpy(high_cards,iter->second.num,iter->second.len);
    }
      break;
    deault:
      printf("cardType::getAnalyseData card_type error");
    }
  }

  void cardType::analyse_cards(uint8* five_cards,uint8 len){
    uint8 i=0;
    resetAnalyseData();
    while(i<len){
      uint8 bsame_count=1;
      uint8 same_cards[]={five_cards[i],0,0,0};
      uint8 logic_value=getCardNum(five_cards[i]);

      for(uint j=i+1;j<len;j++){
        uint8 tmp_value=getCardNum(five_cards[j]);
        if(tmp_value!=logic_value) break;
        bsame_count+=1;
        same_cards[bsame_count-1]=five_cards[bsame_count-1];
      }
      mapAnalyseData::iterator iter=m_mapAnalyResult->find(bsame_count-1);
      if(iter!=m_mapAnalyResult->end()){
        iter->second.count+=1;
        iter->second.len+=bsame_count;
        iter->second.num[iter->second.count-1]=five_cards[iter->second.count-1];
      }
      i+=bsame_count;
    }
  }
};
