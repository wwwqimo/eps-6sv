#ifndef __BSP_SOLARARRAYSIM_H__
#define __BSP_SOLARARRAYSIM_H__

#include "stm32f4xx.h"
//#include "user_bsp.h"
#include "bsp.h"

#define        model_num  1
//���ͨ����
#define        chan_num   1
//���Ƭ��������ϸ�ָ���
#define        PVdiv_num 1000


//���Ƭ����
typedef struct pvpara{
	uint8_t flag;//���Ƭ�������ı�־����ʼ��Ϊ0����Ϊ1��ʾ���Ƭ���͸ı䣬��Ϊ2��ʾ���Ƭ���������ı�
	//���Ƭ���Ͳ��/��
	float a,b,c;//ϵ��
	float Voc,Vm,Isc,Im;	//��ز���
	//�����׼����
	float Sref;
	float Tref;
	//���Ƭ������������

	//��������
	float S;
	float T;
	
}PVpara_Typedef;

//���Ƭ����
typedef struct pv_num{
	uint16_t n_p;//���Ƭ����Ƭ��
	uint16_t n_s;//���Ƭ����Ƭ��
	uint16_t model_type;//��Ӧ��PVģ��
}PVnum_Typedef;
	
//pv ����
typedef struct pv{
	
	PVpara_Typedef PVpara;//���Ƭ����
	
	float IV[2][PVdiv_num];//pv���߲���IV[0]��ŵ������ݣ�IV[1]��ŵ�ѹ����
	float Power_mppt;//����ʵ㹦��
	uint16_t position_current;//��ǰ���ݵ�λ��
	uint16_t position_mppt;//����ʵ�λ��
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
