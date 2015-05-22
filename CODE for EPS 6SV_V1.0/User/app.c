/*
*********************************************************************************************************
*	                                  
*	模块名称 : uCOS-II
*	文件名称 : app.c
*	版    本 : V1.0
*	说    明 : 本实验主要实现消息邮箱
*              实验目的：
*                1. 学习uCOS-II中消息邮箱的使用。
*              实验内容：
*                1. 共创建了如下几个任务，通过按下按键K1可以通过串口打印任务堆栈使用情况
*                    Prio   Used   Free  Per     Taskname
*                     63     17    111   13%   uC/OS-II Idle
*                     62     23    105   17%   uC/OS-II Stat
*                     61     27    101   21%   uC/OS-II Tmr
*                      1     31    993   03%   Start Task
*                      3     65    959   06%   User Interface
*                     60     29    995   02%   LED
*                      4     29    995   02%   COM
*                      2     31    993   03%   KeyScan
*                    串口软件建议使用SecureCRT（V5光盘里面有此软件）查看打印信息。
*                    Start Task任务    ：主要是LED1的闪烁，500ms一次。
*                    User Interface任务：根据用户的按键消息，实现相应的操作。
*                    LED任务           ：实现LED的闪烁。
*                    KeyScan任务       ：主要是实现按键扫描。
*                    COM任务           ：实现LED的闪烁。
*                 2. 按下按键K1可以修改任务AppTaskCom的优先级，建议最后修改此任务的优先级。
*                    因为有按键消息要基于这个任务来实现。
*                 3. 按键K2，K3和五向摇杆按下后，串口打印按下信息
*
*	修改记录 :
*		版本号  日期         作者            说明
*       v1.0    2014-03-23  Eric2013    ST固件库到V1.3.0版本
*                                       BSP驱动包V1.2
*                                       uCOS-II版本V2.92.07
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"

/*
********************************************************************************************************
*                              任务堆栈
********************************************************************************************************
*/

/* 定义每个任务的堆栈空间，app_cfg.h文件中宏定义栈大小 */
static OS_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK AppTaskCOMStk[APP_TASK_COM_STK_SIZE];

static OS_STK AppTaskADCStk[APP_TASK_ADC_STK_SIZE];
static OS_STK AppTaskSASStk[APP_TASK_SAS_STK_SIZE];
static OS_STK AppTaskMPPTStk[APP_TASK_MPPT_STK_SIZE];

static OS_STK AppTaskLEDStk[APP_TASK_LED_STK_SIZE];				
/*
*******************************************************************************************************
*                              函数声明
*******************************************************************************************************
*/

static void AppTaskCreate(void);
static void AppTaskStart(void *p_arg);
static void AppTaskCom(void *p_arg);

static void AppTaskADC(void *p_arg);
static void AppTaskSAS(void *p_arg);
static void AppTaskMPPT(void *p_arg);

static void DispTaskInfo(void);
static void AppTaskLED(void *p_arg);
/*
*******************************************************************************************************
*                               变量
*******************************************************************************************************
*/

/* 定义一个邮箱，    这只是一个邮箱指针，  OSMboxCreate函数会创建邮箱必需的资源 */
OS_EVENT *AppUserIFMbox;
OS_EVENT *COM1_Mbox;



/*
*******************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*******************************************************************************************************
*/
int main(void)
{
	INT8U  err;

	/* 初始化"uC/OS-II"内核 */
	OSInit();
  
	/* 创建一个启动任务（也就是主任务）。启动任务会创建所有的应用程序任务 */
	OSTaskCreateExt(AppTaskStart,	/* 启动任务函数指针 */
                    (void *)0,		/* 传递给任务的参数 */
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1], /* 指向任务栈栈顶的指针 */
                    APP_TASK_START_PRIO,	/* 任务的优先级，必须唯一，数字越低优先级越高 */
                    APP_TASK_START_PRIO,	/* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&AppTaskStartStk[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    APP_TASK_START_STK_SIZE, /* 任务栈大小 */
                    (void *)0,	/* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
					
					/*  定义如下：
						OS_TASK_OPT_STK_CHK      使能检测任务栈，统计任务栈已用的和未用的
						OS_TASK_OPT_STK_CLR      在创建任务时，清零任务栈
						OS_TASK_OPT_SAVE_FP      如果CPU有浮点寄存器，则在任务切换时保存浮点寄存器的内容
					*/                  

	/* 指定任务的名称，用于调试。这个函数是可选的 */
	OSTaskNameSet(APP_TASK_START_PRIO, APP_TASK_START_NAME, &err);
	
	/*ucosII的节拍计数器清0    节拍计数器是0-4294967295*/ 
	OSTimeSet(0);	
	
	/* 启动多任务系统，控制权交给uC/OS-II */
	OSStart();
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 这是一个启动任务，在多任务系统启动后，必须初始化滴答计数器(在BSP_Init中实现)
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：1
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg)
{	

    /* 仅用于避免编译器告警，编译器不会产生任何目标代码 */	
    (void)p_arg;   
	 
	/* BSP 初始化。 BSP = Board Support Package 板级支持包，可以理解为底层驱动。*/
	bsp_Init();
	user_bsp_Init();
  CPU_Init();          
	BSP_Tick_Init();
	
	user_app_Init();
//  Mem_Init();          

	/* 检测CPU能力，统计模块初始化。该函数将检测最低CPU占有率 */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif	
		
	/* 创建应用程序的任务 */
 
	AppTaskCreate();
	/* 任务主体，必须是一个死循环 */
	while (1)     
	{   
		//DispTaskInfo();
		led_toggle();
		OSTimeDlyHMSM(0, 0, 0, 1000);	

	}      
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskCom
*	功能说明: LED4的闪烁		
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：4
*********************************************************************************************************
*/
static void AppTaskCom(void *p_arg)
{	

	unsigned char * msg;
	(void)p_arg;
	while(1)
	{		

//		bsp_LedToggle(4);
		OSTimeDlyHMSM(0, 0, 0, 100);	 						  	 	       											  
   } 
	 						  	 	       											  
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskADC
*	功能说明: 			  			  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
* 优 先 级：10
*********************************************************************************************************
*/
static void AppTaskADC(void *p_arg)
{
  (void)p_arg;		/* 避免编译器告警 */
	

	ADCSample();
	
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskSAS
*	功能说明: 			  			  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
* 优 先 级：11
*********************************************************************************************************
*/
static void AppTaskSAS(void *p_arg)
{
  (void)p_arg;		/* 避免编译器告警 */
	
//	SolarArraySim();
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 500);
	}
	
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskSAS
*	功能说明: 			  			  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
* 优 先 级：11
*********************************************************************************************************
*/
static void AppTaskMPPT(void *p_arg)
{
  (void)p_arg;		/* 避免编译器告警 */
	
	app_MPPT();
//	while(1)
//	{
//		OSTimeDlyHMSM(0, 0, 0, 500);
//	}
	
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskLED
*	功能说明: 			  			  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：OS_LOWEST_PRIO - 3
*********************************************************************************************************
*/
static void AppTaskLED(void *p_arg)
{
    (void)p_arg;		/* 避免编译器告警 */
		
	while (1) 
	{

//		bsp_LedToggle(3);
		OSTimeDlyHMSM(0, 0, 0, 500);	
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	INT8U      err;


	/* 创建邮箱(MBOX) */
	//AppUserIFMbox = OSMboxCreate((void *)0);
	//COM1_Mbox = OSMboxCreate((void *)0);
	
	/* 此任务未使用 */
	OSTaskCreateExt(AppTaskCom,
                    (void *)0,
                    (OS_STK *)&AppTaskCOMStk[APP_TASK_COM_STK_SIZE-1],
                    APP_TASK_COM_PRIO,
                    APP_TASK_COM_PRIO,
                    (OS_STK *)&AppTaskCOMStk[0],
                    APP_TASK_COM_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_COM_PRIO, APP_TASK_COM_NAME, &err);
										
		/* 创建ADC任务 */
	OSTaskCreateExt(AppTaskADC,
                    (void *)0,
                    (OS_STK *)&AppTaskADCStk[APP_TASK_ADC_STK_SIZE - 1],
                    APP_TASK_ADC_PRIO,
                    APP_TASK_ADC_PRIO,
                    (OS_STK *)&AppTaskADCStk[0],
                    APP_TASK_ADC_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_ADC_PRIO, APP_TASK_ADC_NAME, &err);										
										
/* 创建SolarArraySim任务 */
	OSTaskCreateExt(AppTaskSAS,
                    (void *)0,
                    (OS_STK *)&AppTaskSASStk[APP_TASK_SAS_STK_SIZE - 1],
                    APP_TASK_SAS_PRIO,
                    APP_TASK_SAS_PRIO,
                    (OS_STK *)&AppTaskSASStk[0],
                    APP_TASK_SAS_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_SAS_PRIO, APP_TASK_SAS_NAME, &err);
										
		/* 创建MPPT任务 */
	OSTaskCreateExt(AppTaskMPPT,
                    (void *)0,
                    (OS_STK *)&AppTaskMPPTStk[APP_TASK_MPPT_STK_SIZE - 1],
                    APP_TASK_MPPT_PRIO,
                    APP_TASK_MPPT_PRIO,
                    (OS_STK *)&AppTaskMPPTStk[0],
                    APP_TASK_MPPT_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_MPPT_PRIO, APP_TASK_MPPT_NAME, &err);
    
	/* 创建LED任务 */
	OSTaskCreateExt(AppTaskLED,
                    (void *)0,
                    (OS_STK *)&AppTaskLEDStk[APP_TASK_LED_STK_SIZE - 1],
                    APP_TASK_LED_PRIO,
                    APP_TASK_LED_PRIO,
                    (OS_STK *)&AppTaskLEDStk[0],
                    APP_TASK_LED_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_LED_PRIO, APP_TASK_LED_NAME, &err);
    
	

   
}

/*
*********************************************************************************************************
*	函 数 名: DispTaskInfo
*	功能说明: 将uCOS-II任务信息打印到串口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
	
	
	OS_TCB      *ptcb;	        /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
	OS_STK_DATA stk;	        /* 用于读取任务栈使用情况 */

	ptcb    = &OSTCBTbl[0];		/* 指向第1个任务控制块(TCB) */

	/* 打印标题 */
	printf("  Prio   Used  Free  Per     CPU  Taskname\r\n");

	OSTimeDly(10);
	
	/* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
	while (ptcb != NULL)
	{
		/* 
			ptcb->OSTCBPrio : 任务控制块中保存的任务的优先级
			ptcb->OSTCBTaskName : 任务控制块中保存的任务名称，需要在创建任务的时候
			调用OSTaskNameSet()指定任务名称，比如：
			OSTaskNameSet(APP_TASK_USER_IF_PRIO, "User I/F", &err);
		*/
		OSTaskStkChk(ptcb->OSTCBPrio, &stk);	/* 获得任务栈已用空间 */
		printf("   %2d  %5d  %5d  %02d%%   %5.2f%%   %s\r\n", ptcb->OSTCBPrio, 
			stk.OSUsed, stk.OSFree, (stk.OSUsed * 100) / (stk.OSUsed + stk.OSFree),
		(float)ptcb->OSTCBCyclesTot / SystemCoreClock,
			ptcb->OSTCBTaskName);		
		ptcb = ptcb->OSTCBPrev;		            /* 指向上一个任务控制块 */
	}
}

/*
*********************************************************************************************************
*                                          App_TaskCreateHook()
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if (OS_APP_HOOKS_EN > 0)
void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    //OSProbe_TaskCreateHook(ptcb);
#else
    (void)ptcb;
#endif
}

/*
*********************************************************************************************************
*                                           App_TaskDelHook()
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                             App_TaskIdleHook()
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK (APPLICATION)
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/
#if OS_VERSION >= 289
void  App_TaskReturnHook (OS_TCB  *ptcb)
{
    (void)ptcb;
}
#endif


/*
*********************************************************************************************************
*                                   App_TaskStatHook()
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Returns    : none
*********************************************************************************************************
*/
void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           App_TaskSwHook()
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    //OSProbe_TaskSwHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                           App_TCBInitHook()
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                               App_TimeTickHook()
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    // OSProbe_TickHook();
#endif
}
#endif

#endif                                       /* End of OS_APP_HOOKS_EN                                   */


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
