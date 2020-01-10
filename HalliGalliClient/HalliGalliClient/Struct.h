#ifndef Struct_h__
#define Struct_h__

/* 순서: 카드 뒷면 = 0, 체리 = 1, 배 = 2 */
enum FRUIT_ID { FRUIT_BACK, FRUIT_CHEERY, FRUIT_PEAR, FRUIT_END };

typedef struct tagCard
{
	int iFruitID;
	int iFruitCnt;
}CARD;

#endif // Struct_h__