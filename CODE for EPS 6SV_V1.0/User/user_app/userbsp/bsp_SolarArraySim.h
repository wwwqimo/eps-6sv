#ifndef __BSP_SOLARARRAYSIM_H__
#define __BSP_SOLARARRAYSIM_H__

#include "stm32f4xx.h"
//#include "user_bsp.h"
#include "bsp.h"

#define        model_num  1
//Êä³öÍ¨µÀÊı
#define        chan_num   1
//µç³ØÆ¬ÇúÏßÊı¾İÏ¸·Ö¸öÊı
#define        PVdiv_num 1000


//µç³ØÆ¬²ÎÊı
typedef struct pvpara{
	uint8_t flag;//µç³ØÆ¬²ÎÊı¸ü¸Ä±êÖ¾£¬³õÊ¼»¯Îª0£¬ÈôÎª1±íÊ¾µç³ØÆ¬ÀàĞÍ¸Ä±ä£¬ÈôÎª2±íÊ¾µç³ØÆ¬ÊäÈëÌõ¼ş¸Ä±ä
	//µç³ØÆ¬ÀàĞÍ²ÎÊ/¡¢
	float a,b,c;//ÏµÊı
	float Voc,Vm,Isc,Im;	//µç³Ø²ÎÊı
	//ÊäÈë±ê×¼Ìõ¼ş
	float Sref;
	float Tref;
	//µç³ØÆ¬ÊäÈëÌõ¼ş²ÎÊı

	//ÊäÈëÌõ¼ş
	float S;
	float T;
	
}PVpara_Typedef;

//µç³ØÆ¬ÊıÁ¿
typedef struct pv_num{
	uint16_t n_p;//µç³ØÆ¬²¢ÁªÆ¬Êı
	uint16_t n_s;//µç³ØÆ¬´®ÁªÆ¬Êı
	uint16_t model_type;//¶ÔÓ¦µÄPVÄ£ĞÍ
}PVnum_Typedef;
	
//pv Êı¾İ
typedef struct pv{
	
	PVpara_Typedef PVpara;//µç³ØÆ¬²ÎÊı
	
	float IV[2][PVdiv_num];//pvÇúÏß²ÎÊıIV[0]´æ·ÅµçÁ÷Êı¾İ£¬IV[1]´æ·ÅµçÑ¹Êı¾İ
	float Power_mppt;//×î´ó¹¦ÂÊµã¹¦ÂÊ
	uint16_t position_current;//µ±Ç°Êı¾İµãÎ»ÖÃ
	uint16_t position_mppt;//×î´ó¹¦ÂÊµãÎ»ÖÃ
	uint16_t position_prev;//reserve
	uint16_t position_nxt;//reserve

}PV_Typedef;
	


void bsp_PV_Init(void);
void PV_Change(PV_Typedef *PV,PVpara_Typedef *SCpara);
void PV_ChangeCondition(PV_Typedef *PV,PVpara_Typedef *SCpara);
void PV_ChangeType(PV_Typedef *PV,PVpara_Typedef *SCpara);

uint8_t IV_generate(PV_Typedef *PV);

void SAS_EN(uint16_t chan, FunctionalState State);

#endif
/****************************************************************************************************************/
