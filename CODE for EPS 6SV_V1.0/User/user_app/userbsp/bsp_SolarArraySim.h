#ifndef __BSP_SOLARARRAYSIM_H__
#define __BSP_SOLARARRAYSIM_H__

#include "stm32f4xx.h"
//#include "user_bsp.h"
#include "bsp.h"

#define        model_num  1
//输出通道数
#define        chan_num   1
//电池片曲线数据细分个数
#define        PVdiv_num 1000


//电池片参数
typedef struct pvpara{
	uint8_t flag;//电池片参数更改标志，初始化为0，若为1表示电池片类型改变，若为2表示电池片输入条件改变
	//电池片类型参�/、
	float a,b,c;//系数
	float Voc,Vm,Isc,Im;	//电池参数
	//输入标准条件
	float Sref;
	float Tref;
	//电池片输入条件参数

	//输入条件
	float S;
	float T;
	
}PVpara_Typedef;

//电池片数量
typedef struct pv_num{
	uint16_t n_p;//电池片并联片数
	uint16_t n_s;//电池片串联片数
	uint16_t model_type;//对应的PV模型
}PVnum_Typedef;
	
//pv 数据
typedef struct pv{
	
	PVpara_Typedef PVpara;//电池片参数
	
	float IV[2][PVdiv_num];//pv曲线参数IV[0]存放电流数据，IV[1]存放电压数据
	float Power_mppt;//最大功率点功率
	uint16_t position_current;//当前数据点位置
	uint16_t position_mppt;//最大功率点位置
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
