/*
**********************************************************************************
**eps_handle.h
**
**
**
************************************************************************************
*/


#ifndef __EPS_HANDLE_H__
#define __EPS_HANDLE_H__
#include "stm32f4xx.h"

/**
	*@defineoutput
	*/

  //EPS_SVversion
	#define OUT_REG1    0
	#define OUT_REG2    1
	#define OUT_REG3    2	
	#define OUT_REG4    3
	#define OUT_REG5    4
	
	#define OUT_UREG1   5
	#define OUT_UREG2   6
	#define OUT_UREG3   7
	#define OUT_UREG4   8

	#define OUT_DEPLOY  9
	#define OUT_HEAT    10
	#define OUT_POWERS  11
	#define OUT_PSW     12
//define with usage name
	#define OUT_ALL      20
	#define OUT_GPSA    OUT_REG1
	#define OUT_GPSB    OUT_REG2
	#define OUT_MTQ     OUT_UREG1
	#define OUT_WHEELA  OUT_UREG2
	#define OUT_WHEELB  OUT_UREG3
	#define OUT_USB     OUT_UREG4
	#define OUT_FIPEX5V OUT_REG4
	#define OUT_FIPEX3V OUT_REG5
	#define OUT_RES     OUT_REG3

  
#ifdef EPS_4version
	#define REG_NUM          6
	#define UREG_NUM         5
#else
	#define REG_NUM          5
	#define UREG_NUM         4
#endif
	#define SV_NUM           6


#define ADC_REF          2500   //ADC�ο���ѹ
#define ADC_FullScale    4095  //ADC����
#define AD7490_REF       2500   //AD7490 �ο���ѹ
#define AD7490_FullScale 4095  //ad7490����

/* eps adc
*************************************************************************************
*/
typedef struct{
	uint16_t c_sv[SV_NUM];
	uint16_t v_sv[SV_NUM];
	uint16_t c_sva;
	uint16_t c_bus;
	uint16_t v_bus;
	uint16_t v_5v;
	uint16_t v_3v;
	uint16_t c_out[REG_NUM+UREG_NUM];
	int16_t temp_eps[4];
	int16_t temp_bat[2];
} eps_hk_adc_t;

#define hk_adc_num  32
/* ppt converter struct
********************************************************
*/
#define PPT_NUM        6 //ppt converter number
typedef struct{
	uint16_t curr;        //the current input current[mA]
	uint16_t volt;        //the current input voltage[mv]
	uint16_t out_volt;    //the output voltage [mv]
	uint8_t mode;         //mode for ppt[0 = fixed, 1 = auto]
	uint16_t ppt_volt;    //fixed ppt point for converters [mv],only when ppt_mode = 0,it is useful
} eps_ppt_t;

/*battery
********************************************************************************************
*/

//��ص�ѹ״̬
typedef enum{
	BAT_OV,         //8.35v���Ϲ�ѹ
	BAT_NORMAL,     //7.2~8.35 ����
	BAT_UV,         //7.0~7.2Ƿѹ
	BAT_CRITICAL,   //6.75~7.0 critical low 
	BAT_HARDOFF,    //<6.75 Ӳ���ض�
} bat_state_t;

//����жϵ�ѹ�����¶Ȳο�ֵ

#define  bat_full_volt       8350 //bat full charged voltage 
#define  bat_normal_volt     7200 //bat safe voltage range,if bat volt lower than this value,satellite switch to low power mode
#define  bat_critical_volt   7000 //bat critical low voltage ,cut off all the loads
#define  bat_hardoff_volt    6750 //bat hard cutting off voltage,the bus switch will turn on till the batvolt higher than the voltage value plus a  hysteretic voltage
#define  bat_oc_curr         2500 //mode for batheater [0 =manual , 1 = auto]
#define  bat_heater_low      (-5) //turn heater on at [degc]
#define  bat_heater_high     (10) //turn heater off at [degc]

#define  bat_volt_drop        100  //battery constant charge set voltage= bat_full_volt + bat_volt_drop [mV]
#define  bat_volt_hyst_up     50   //battery nearly full charge dropout voltage [mV]
#define  bat_volt_hyst_down   100  //battery exit from full charge dropout voltage [mV]
typedef enum{
	BAT_NOCURR = 0,
	BAT_DISCH,
	BAT_DISCH_PRE,
	BAT_SV_CHARGE,
	BAT_EXT_CHARGE,
	BAT_EXT_CHARGE_PRE,
}bat_charge_state_t;
typedef enum{NO,YES= !NO}bat_heater_state_t;
//�������״̬���ṹ��
typedef struct{ 
	bat_state_t         bat_state;         //��ص�ǰ״̬
	char                *bat_status;       //��ص�ǰ״̬
	bat_charge_state_t    bat_charge;        //��س��״̬
	bat_heater_state_t  bat_heater_status; //��ؼ���״̬
	uint32_t            bat_fullcap;       //��س�ʼ����
	uint16_t            bat_percent;              //��ǰ�����ٷֱ�
	uint16_t            bat_total_percent;        //��ǰ��������ǳ�ʼ�����İٷֱ�
	uint32_t            bat_off_count;         //���Ӳ���ضϵĴ���
	uint8_t             bat_heater_mode;        //��ؼ���ģʽ��0=manual��1=auto
} eps_bat_t;


/*
*********************************************************************************************
*/

//sv inputs state enum
typedef enum{
	SV_OFF = 0,
	SV_ON,
	SV_ERR,
	SV_HDERR,
}sv_state_t;
//CONVERTER state enum
typedef enum{
	CONV_OFF,
	CONV_ON,
	CONV_ERR,
	CONV_UV,           //converter output voltage too low
	CONV_OV,           //converter output voltage too high
	CONV_OC,           //converter output current too large
}conv_state_t;
//output state enum
typedef enum{
	OUTPUT_OFF = 0, //output off     output is turned off
	OUTPUT_ON,      //output on      output is turned on
	OUTPUT_ERR,     //output err     OUTPUT_TRY �� OUTPUT_HDTRY ֻ����OUT_ERR֮��Ż����
	OUTPUT_OFFERR,  //��output ����off״̬ʱ�����ڲ���֪���ص���ͨ�����ڿ�ͨ״̬
	OUTPUT_SFTRY,     //mannual autotry
	OUTPUT_HDTRY,    //hardware autotry
	OUTPUT_TRYOK,   //output aytotry ok,֮�����OUTPUT_ON
	OUTPUT_HDERR,   //output hardware err,this channel output will not be opened
}output_state_t;
//eps power select enum
typedef enum{
	POWER_REG = 0,   //eps powered from 3.3v converter
	POWER_BAT,       //eps powered from battery output
}power_state_t;


//eps sates struct
typedef struct eps_hk_state_s{
	sv_state_t sv_state[SV_NUM];     //eps solar input state
	conv_state_t conv_state[2];   //eps regulator converter state
	output_state_t out_state[REG_NUM + UREG_NUM];    //eps regulated and unregulated ouputs states
	uint16_t out_faults;      //eps ALL outputs fault count number 
	uint16_t out_ocp[REG_NUM + UREG_NUM];//eps out over current soft detecct count number
	uint16_t out_Ton[REG_NUM + UREG_NUM];      // outputs set n seconds to turn on the output
	uint16_t out_Toff[REG_NUM + UREG_NUM];     // outputs set n second to turn off the output
	uint16_t out_Thderr[REG_NUM + UREG_NUM];   /*��HDERR״̬�ָ���ʱ�� ��λ[ticks]*/
	uint16_t out_fault[REG_NUM + UREG_NUM];    //eps every regulated and unregulated outputs fault count number
	power_state_t eps_power_mode;  //eps power select
	uint8_t eps_state_reserved; //eps state reserved
} eps_hk_state_t;



/*
*********************************************************************************************
*/
void eps_enter_safe(void);
void eps_enter_normal(void);
void eps_enter_critical(void);

void eps_allin_off(void);
void eps_allin_on(void);
uint8_t bat_heater_on(void);
uint8_t bat_heater_off(void);
void out_state_setforce(uint8_t chan,FunctionalState NewState);
void out_state_clear(uint8_t chan);
static void out_channel(uint8_t chan,FunctionalState NewState);
output_state_t out_en(uint8_t chan,FunctionalState NewState);
int8_t outall_en(FunctionalState NewState);

void eps_data_Init(void);
void eps_data_handling(void);

static void temp_data_processing(eps_hk_adc_t * eps_adc, eps_hk_state_t * eps_state);
static void conv_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state);
static void output_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state);
static void bat_data_processing(eps_hk_adc_t *eps_adc,eps_bat_t *eps_bat);
static uint16_t batSOCpermanent(eps_bat_t *bat,uint16_t volt,uint16_t curr);
static uint16_t batSOC(eps_bat_t *bat,uint16_t volt,uint16_t curr);
#endif


/*******************************************************************************************/

