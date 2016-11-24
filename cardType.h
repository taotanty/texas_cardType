/*
  cardType.h
  --牌型判断算法,包含计算过程中记录的牌和类型
 */
#include "cardTypeDefine.h"
#include <map>
#include "assert.h"
#include <vector>

namespace texascardtype{

typedef struct{
  uint8 count;
  uint8 len;
  uint8* num;
}stAnalyseData;

  typedef std::map<uint8,stAnalyseData> mapAnalyseData;

  class cardType{
  public:
    cardType(void);
    ~cardType(void);
    Card_Type countMaxCardType(uint8* hand_cards,uint8 hand_length,uint8* board_cards,uint8 board_length,uint8* max_cards,uint8* high_cards);
    Card_Type countCardType(uint8* five_cards,uint8 len);
    uint8 compareCard(uint8* max_cards,uint8* tmp_cards);
  private:
    void resetAnalyseData();
    void analyse_cards(uint8* five_cards,uint8 len);
    Card_Type countAnalyseCardsType();
    void getAnalyseData(Card_Type card_type,const uint8* five_cards,uint8* high_cards);
    mapAnalyseData*  m_mapAnalyResult;
  };
};