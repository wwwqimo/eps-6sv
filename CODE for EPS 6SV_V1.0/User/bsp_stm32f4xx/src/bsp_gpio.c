/*
****************************************************************************************************
**GPIO
**
**
******************************************************************************************************
*/

#include "bsp_gpio.h"
#include "user_cfg.h"

static void bsp_InitGPIO_EN(void);//对外保护输出使能脚，0 off 1 on 0-1 fault to on
#if EPS_PS_USED > 0u
static void bsp_InitGPIO_PS(void);
#endif
#if EPS_HS_USED > 0u
static void bsp_InitGPIO_HS(void);
#endif
#if USER_DEBUG_EN > 0u
static void bsp_InitGPIO_LED(void);//LED 使能脚 0 off 1 on
#endif
#if EPS_PSW_USED > 0u
static void bsp_InitGPIO_PSW(void);//硬件问题暂时未使用，默认选择3.3v converter
#endif
#if EPS_DS_USED > 0u
static void bsp_InitGPIO_DS(void);//帆板释放使能脚 0 off 1 on
#endif
#if EPS_FAULT_USED > 0u
static void bsp_InitGPIO_FAULT(void);
#endif
static void bsp_InitALLGPIO(void);


void bsp_InitGPIO(void)
{
//	bsp_InitALLGPIO();

	#if EPS_FAULT_USED > 0u
	out_fault_en(DISABLE);
	#endif
	bsp_InitGPIO_EN();
	#if USER_DEBUG_EN > 0u
  bsp_InitGPIO_LED();
	#endif
	#if EPS_PS_USED > 0u
	bsp_InitGPIO_PS();
	#endif
	#if EPS_HS_USED > 0u
	bsp_InitGPIO_HS();
	#endif
	#if EPS_PSW_USED > 0u
	bsp_InitGPIO_PSW();
	#endif
	#if EPS_DS_USED > 0u
	bsp_InitGPIO_DS();
	#endif
	#if EPS_FAULT_USED > 0u
	bsp_InitGPIO_FAULT();
	#endif
}
//static void bsp_InitALLGPIO(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	/* 打开GPIO时钟 */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;		/* 设为输出口 */
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//}
/**
*@bref    eps battery board heater switch gpio init
*@para
*@reteval
*/
#if EPS_HS_USED >0u
static void bsp_InitGPIO_HS(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	OUTPUT_HS(DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
#endif
/**
*@bref    eps battery board power switch gpio init
*@para
*@reteval
*/
#if EPS_PS_USED >0u
static void bsp_InitGPIO_PS(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	OUTPS(DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
#endif
/*
**输出使能脚初始化函数
**默认0关断，1打开，发生故障需要完成0-1变换才能打开输出
**EN1--PE2;EN2--PE4;EN3--PD1;EN4--PD2;EN5--PD3;EN6--PD4;EN7--PD5;EN8--PD6;EN9--PD3
**PS--PE15;HS--PB10
*/
static void bsp_InitGPIO_EN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//初始拉低
	OUTPUT_REG1(DISABLE);
	OUTPUT_REG2(DISABLE);
	OUTPUT_REG3(DISABLE);
  OUTPUT_REG4(DISABLE);
	OUTPUT_REG5(DISABLE);
	OUTPUT_UREG1(DISABLE);
	OUTPUT_UREG2(DISABLE);
	OUTPUT_UREG3(DISABLE);
	OUTPUT_UREG4(DISABLE);

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}
/*
**LED 初始化
**
**PA12 0关 1开
*/
#if USER_DEBUG_EN > 0u
static void bsp_InitGPIO_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	OUTPUT_LED(DISABLE);//LED
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#endif
/*
**eps power switch init
**
**PC6 0 3.3V REGULATOR CONVERTER
**    1  battery source through liner converter
**    default 0
*/
#if EPS_PSW_USED > 0u
static void bsp_InitGPIO_PSW(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	OUTPUT_PSW(DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
#endif
/*
**deploy switch init
**
**PA8  0 OFF 1 ON
** default 0 ,till the deployment command
*/
#if EPS_DS_USED >0u
static void bsp_InitGPIO_DS(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	OUTPUT_DS(DISABLE);// DEPLOY EN DISABLE DEFAULT
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#endif
/*
**保护输出 fault 脚初始化
**
**PD0 --fault 下降沿触发
**由于使能脚和fault具有直连关系，所以在判断故障时应加入判断逻辑
*/
#if EPS_FAULT_USED >0u
static void bsp_InitGPIO_FAULT(void)
{
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
//	
//	/* 使能SYSCFG时钟 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	/* 连接 EXTI Line0 到 PD0 引脚 */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);
//	/* 配置 EXTI LineXXX */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	/* 设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级 */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	
//	/* 中断优先级配置 最低优先级 这里一定要分开的设置中断，不能够合并到一个里面设置 */
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
}
#endif
/**
*@bref external interruption state set
*/
void out_fault_en(FunctionalState NewState)
{
//	NVIC_InitTypeDef   NVIC_InitStructure;

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
//	NVIC_Init(&NVIC_InitStructure);
}



/**
*@bref eps power select
*@detail    
*@para  NewState   0 = 3.3v converter powered(default) ; 1 = battery powered directly 
*@reteval none
*/

void led_on(void)
{
	OUTPUT_LED(ENABLE);
}
void led_off(void)
{
	OUTPUT_LED(DISABLE);
}
void led_toggle(void)
{
	OUTPUT_LED_TOGGLE();
}
/**************************************************************************************************/

