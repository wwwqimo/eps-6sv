/*
****************************************************************************************************
**GPIO
**
**
******************************************************************************************************
*/

#include "bsp_gpio.h"

static void OUTPUT1(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_2 );else GPIO_ResetBits(GPIOE,GPIO_Pin_2 );}
static void OUTPUT2(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_4 );else GPIO_ResetBits(GPIOE,GPIO_Pin_4 );}
static void OUTPUT3(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_1 );else GPIO_ResetBits(GPIOD,GPIO_Pin_1 );}
static void OUTPUT4(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_2 );else GPIO_ResetBits(GPIOD,GPIO_Pin_2 );}
static void OUTPUT5(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_3 );else GPIO_ResetBits(GPIOD,GPIO_Pin_3 );}
static void OUTPUT6(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_4 );else GPIO_ResetBits(GPIOD,GPIO_Pin_4 );}
static void OUTPUT7(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_5 );else GPIO_ResetBits(GPIOD,GPIO_Pin_5 );}
static void OUTPUT8(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_6 );else GPIO_ResetBits(GPIOD,GPIO_Pin_6 );}
static void OUTPUT9(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_3 );else GPIO_ResetBits(GPIOE,GPIO_Pin_3 );}

static void OUTPS(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_15 );else GPIO_ResetBits(GPIOE,GPIO_Pin_15 );}
static void OUTHS(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOB,GPIO_Pin_10 );else GPIO_ResetBits(GPIOB,GPIO_Pin_10 );}

static void OUTPSW(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOC,GPIO_Pin_6 );else GPIO_ResetBits(GPIOC,GPIO_Pin_6 );}
static void OUTLED(FunctionalState NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOA,GPIO_Pin_12 );else GPIO_ResetBits(GPIOA,GPIO_Pin_12 );}
static void OUTLED_TOGGLE(void) {GPIO_ToggleBits(GPIOA,GPIO_Pin_12);}
static void bsp_InitGPIO_EN(void);//对外保护输出使能脚，0 off 1 on 0-1 fault to on
static void bsp_InitGPIO_LED(void);//LED 使能脚 0 off 1 on
static void bsp_InitGPIO_PSW(void);//硬件问题暂时未使用，默认选择3.3v converter
static void bsp_InitGPIO_DS(void);//帆板释放使能脚 0 off 1 on
static void bsp_InitGPIO_FAULT(void);


static void bsp_InitALLGPIO(void);
void bsp_InitGPIO(void)
{
//	bsp_InitALLGPIO();
	
	bsp_InitGPIO_EN();
	bsp_InitGPIO_LED();
//	bsp_InitGPIO_PSW();
	bsp_InitGPIO_DS();
	bsp_InitGPIO_FAULT();
}
static void bsp_InitALLGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


}
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
	OUTPUT1(DISABLE);
	OUTPUT2(DISABLE);
	OUTPUT3(DISABLE);
  OUTPUT4(DISABLE);
	OUTPUT5(DISABLE);
	OUTPUT6(DISABLE);
	OUTPUT7(DISABLE);
	OUTPUT8(DISABLE);
	OUTPUT9(DISABLE);
	OUTPS(DISABLE);
	OUTHS(DISABLE);
	
	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
**LED 初始化
**
**PA12 0关 1开
*/
static void bsp_InitGPIO_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_ResetBits(GPIOA,GPIO_Pin_12 );//LED
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*
**eps power switch init
**
**PC6 0 3.3V REGULATOR CONVERTER
**    1  battery source through liner converter
**    default 0
*/
static void bsp_InitGPIO_PSW(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_ResetBits(GPIOC,GPIO_Pin_6 );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*
**deploy switch init
**
**PA8  0 OFF 1 ON
** default 0 ,till the deployment command
*/
static void bsp_InitGPIO_DS(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_ResetBits(GPIOA,GPIO_Pin_8 );//LED
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*
**保护输出 fault 脚初始化
**
**PD0 --fault 下降沿触发
**由于使能脚和fault具有直连关系，所以在判断故障时应加入判断逻辑
*/
static void bsp_InitGPIO_FAULT(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* 使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* 连接 EXTI Line0 到 PD0 引脚 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);
	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* 设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* 中断优先级配置 最低优先级 这里一定要分开的设置中断，不能够合并到一个里面设置 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/*
**ocp out put en function
**para    
**     out_chan    使能端口  1---9 对应 端口1---9 ；
**                  0则全部关闭
**                 大于等于10则全部端口打开
**     NewState    端口状态
**
*/
void ocp_out_en(uint8_t out_chan,FunctionalState NewState)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	switch (out_chan){
		case 0:
			OUTPUT1(DISABLE);
			OUTPUT2(DISABLE);
			OUTPUT3(DISABLE);
			OUTPUT4(DISABLE);
			OUTPUT5(DISABLE);
			OUTPUT6(DISABLE);
			OUTPUT7(DISABLE);
			OUTPUT8(DISABLE);
			OUTPUT9(DISABLE);
		break;
		case 1:
			OUTPUT1(NewState);
		break;
		case 2:
			OUTPUT2(NewState);
		break;
		case 3:
			OUTPUT3(NewState);
		break;
		case 4:
			OUTPUT4(NewState);
		break;
		case 5:
			OUTPUT5(NewState);
		break;
		case 6:
			OUTPUT6(NewState);
		break;
		case 7:
			OUTPUT7(NewState);
		break;
		case 8:
			OUTPUT8(NewState);
		break;
		case 9:
			OUTPUT9(NewState);
		break;
		default:
			OUTPUT1(ENABLE);
			OUTPUT2(ENABLE);
			OUTPUT3(ENABLE);
			OUTPUT4(ENABLE);
			OUTPUT5(ENABLE);
			OUTPUT6(ENABLE);
			OUTPUT7(ENABLE);
			OUTPUT8(ENABLE);
			OUTPUT9(ENABLE);
		break;		
	}		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
		
}
void led_on(void)
{
	OUTLED(ENABLE);
}
void led_off(void)
{
	OUTLED(DISABLE);
}
void led_toggle(void)
{
	OUTLED_TOGGLE();
}
/**************************************************************************************************/

