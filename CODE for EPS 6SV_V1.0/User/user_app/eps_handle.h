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

#include "user_cfg.h"
#include "user_app.h"

#define EPS_OK         0
#define EPS_MCU_ERR    1
#define EPS_SV_ERR     2
#define EPS_CONV_ERR   3
#define EPS_OUT_ERR    4
#define EPS_DEPLOY_ERR 5
#define EPS_TEMP_ERR   6
#define EPS_BAT_ERR    7
#define EPS_UVP_ERR    8
#define EPS_OVP_ERR    9
#define EPS_HEAT_ERR   10
#define EPS_ADC_ERR    11

#define EPS_OV        100
#define EPS_UV        101
#define EPS_OC        102
#define EPS_SC        103
#define EPS_OT        104
#define EPS_UT        105   





//电池电压状态
typedef enum{
	BAT_OV,         //8.35v以上过压
	BAT_NORMAL,     //7.2~8.35 正常
	BAT_UV,         //7.0~7.2欠压
	BAT_CRITICAL,   //6.75~7.0 critical low 
	BAT_HARDOFF,    //<6.75 硬件关断
} bat_state_t;

//电池判断电压电流温度参考值

#define  bat_full_volt       8300 //bat full charged voltage 
#define  bat_normal_volt     7200 //bat safe voltage range,if bat volt lower than this value,satellite switch to low power mode
#define  bat_critical_volt   7000 //bat critical low voltage ,cut off all the loads
#define  bat_hardoff_volt    6750 //bat hard cutting off voltage,the bus switch will turn on till the batvolt higher than the voltage value plus a  hysteretic voltage
#define  bat_oc_curr         2500//mode for batheater [0 =manual , 1 = auto]
#define  bat_heater_low      (-5)//turn heater on at [degc]
#define  bat_heater_high     (10) //turn heater off at [degc]

typedef enum{
	BAT_DISCH = 0,
	BAT_CHARGE,
	BAT_NOCURR
}bat_char_state_t;
typedef enum{NO,YES= !NO}bat_heater_state_t;
//电池所有状态量结构体
typedef struct{ 
	bat_state_t         bat_state;         //电池当前状态
	char                *bat_status;       //电池当前状态
	bat_char_state_t    bat_charge;        //电池充电状态
	bat_heater_state_t  bat_heater_status; //电池加热状态
	uint32_t            bat_fullcap;       //电池初始能量
	uint16_t            bat_percent;              //当前电量百分比
	uint16_t            bat_total_percent;        //当前电池容量是初始容量的百分比
	uint32_t            bat_off_count;         //电池硬件关断的次数
	uint8_t             bat_heater_mode;        //电池加热模式，0=manual，1=auto
} eps_bat_t;






typedef struct{
	uint8_t ppt_mode;               //mode for ppt[0 = fixed, 1 = auto]
	uint16_t ppt_volt;              //fixed ppt point for converters [mv]
} eps_ppt_t;




void eps_enter_normal(void);
void eps_enter_safe(void);
void eps_entern_critical(void);
void eps_allin_off(void);
void eps_allin_on(void);
void bat_heater_on(void);
void bat_heater_off(void);

void eps_data_Init(void);
void eps_data_handling(void);
void bat_data_processing(void);

#endif


/*******************************************************************************************/

