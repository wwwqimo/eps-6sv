/*
*********************************************************************************************************
*	                                  
*	ģ������ : uCOS-II
*	�ļ����� : app.c
*	��    �� : V1.0
*	˵    �� : ��ʵ����Ҫʵ����Ϣ����
*              ʵ��Ŀ�ģ�
*                1. ѧϰuCOS-II����Ϣ�����ʹ�á�
*              ʵ�����ݣ�
*                1. �����������¼�������ͨ�����°���K1����ͨ�����ڴ�ӡ�����ջʹ�����
*                    Prio   Used   Free  Per     Taskname
*                     63     17    111   13%   uC/OS-II Idle
*                     62     23    105   17%   uC/OS-II Stat
*                     61     27    101   21%   uC/OS-II Tmr
*                      1     31    993   03%   Start Task
*                      3     65    959   06%   User Interface
*                     60     29    995   02%   LED
*                      4     29    995   02%   COM
*                      2     31    993   03%   KeyScan
*                    �����������ʹ��SecureCRT��V5���������д�������鿴��ӡ��Ϣ��
*                    Start Task����    ����Ҫ��LED1����˸��500msһ�Ρ�
*                    User Interface���񣺸����û��İ�����Ϣ��ʵ����Ӧ�Ĳ�����
*                    LED����           ��ʵ��LED����˸��
*                    KeyScan����       ����Ҫ��ʵ�ְ���ɨ�衣
*                    COM����           ��ʵ��LED����˸��
*                 2. ���°���K1�����޸�����AppTaskCom�����ȼ�����������޸Ĵ���������ȼ���
*                    ��Ϊ�а�����ϢҪ�������������ʵ�֡�
*                 3. ����K2��K3������ҡ�˰��º󣬴��ڴ�ӡ������Ϣ
*
*	�޸ļ�¼ :
*		�汾��  ����         ����            ˵��
*       v1.0    2014-03-23  Eric2013    ST�̼��⵽V1.3.0�汾
*                                       BSP������V1.2
*                                       uCOS-II�汾V2.92.07
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"

/*
********************************************************************************************************
*                              �����ջ
********************************************************************************************************
*/

/* ����ÿ������Ķ�ջ�ռ䣬app_cfg.h�ļ��к궨��ջ��С */
static OS_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK AppTaskCOMStk[APP_TASK_COM_STK_SIZE];

static OS_STK AppTaskADCStk[APP_TASK_ADC_STK_SIZE];
static OS_STK AppTaskSASStk[APP_TASK_SAS_STK_SIZE];
static OS_STK AppTaskMPPTStk[APP_TASK_MPPT_STK_SIZE];

static OS_STK AppTaskLEDStk[APP_TASK_LED_STK_SIZE];				
/*
*******************************************************************************************************
*                              ��������
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
*                               ����
*******************************************************************************************************
*/

/* ����һ�����䣬    ��ֻ��һ������ָ�룬  OSMboxCreate�����ᴴ������������Դ */
OS_EVENT *AppUserIFMbox;
OS_EVENT *COM1_Mbox;



/*
*******************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************************************
*/
int main(void)
{
	INT8U  err;

	/* ��ʼ��"uC/OS-II"�ں� */
	OSInit();
  
	/* ����һ����������Ҳ���������񣩡���������ᴴ�����е�Ӧ�ó������� */
	OSTaskCreateExt(AppTaskStart,	/* ����������ָ�� */
                    (void *)0,		/* ���ݸ�����Ĳ��� */
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1], /* ָ������ջջ����ָ�� */
                    APP_TASK_START_PRIO,	/* ��������ȼ�������Ψһ������Խ�����ȼ�Խ�� */
                    APP_TASK_START_PRIO,	/* ����ID��һ����������ȼ���ͬ */
                    (OS_STK *)&AppTaskStartStk[0],/* ָ������ջջ�׵�ָ�롣OS_STK_GROWTH ������ջ�������� */
                    APP_TASK_START_STK_SIZE, /* ����ջ��С */
                    (void *)0,	/* һ���û��ڴ�����ָ�룬����������ƿ�TCB����չ����
                       ���������л�ʱ����CPU����Ĵ��������ݣ���һ�㲻�ã���0���� */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* ����ѡ���� */
					
					/*  �������£�
						OS_TASK_OPT_STK_CHK      ʹ�ܼ������ջ��ͳ������ջ���õĺ�δ�õ�
						OS_TASK_OPT_STK_CLR      �ڴ�������ʱ����������ջ
						OS_TASK_OPT_SAVE_FP      ���CPU�и���Ĵ��������������л�ʱ���渡��Ĵ���������
					*/                  

	/* ָ����������ƣ����ڵ��ԡ���������ǿ�ѡ�� */
	OSTaskNameSet(APP_TASK_START_PRIO, APP_TASK_START_NAME, &err);
	
	/*ucosII�Ľ��ļ�������0    ���ļ�������0-4294967295*/ 
	OSTimeSet(0);	
	
	/* ����������ϵͳ������Ȩ����uC/OS-II */
	OSStart();
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(��BSP_Init��ʵ��)
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����1
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg)
{	

    /* �����ڱ���������澯����������������κ�Ŀ����� */	
    (void)p_arg;   
	 
	/* BSP ��ʼ���� BSP = Board Support Package �弶֧�ְ����������Ϊ�ײ�������*/
	bsp_Init();
	user_bsp_Init();
  CPU_Init();          
	BSP_Tick_Init();
	
	user_app_Init();
//  Mem_Init();          

	/* ���CPU������ͳ��ģ���ʼ�����ú�����������CPUռ���� */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif	
		
	/* ����Ӧ�ó�������� */
 
	AppTaskCreate();
	/* �������壬������һ����ѭ�� */
	while (1)     
	{   
		//DispTaskInfo();
		led_toggle();
		OSTimeDlyHMSM(0, 0, 0, 1000);	

	}      
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskCom
*	����˵��: LED4����˸		
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����4
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
*	�� �� ��: AppTaskADC
*	����˵��: 			  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
* �� �� ����10
*********************************************************************************************************
*/
static void AppTaskADC(void *p_arg)
{
  (void)p_arg;		/* ����������澯 */
	

	ADCSample();
	
}
/*
*********************************************************************************************************
*	�� �� ��: AppTaskSAS
*	����˵��: 			  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
* �� �� ����11
*********************************************************************************************************
*/
static void AppTaskSAS(void *p_arg)
{
  (void)p_arg;		/* ����������澯 */
	
//	SolarArraySim();
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 500);
	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskSAS
*	����˵��: 			  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
* �� �� ����11
*********************************************************************************************************
*/
static void AppTaskMPPT(void *p_arg)
{
  (void)p_arg;		/* ����������澯 */
	
	app_MPPT();
//	while(1)
//	{
//		OSTimeDlyHMSM(0, 0, 0, 500);
//	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: AppTaskLED
*	����˵��: 			  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_LOWEST_PRIO - 3
*********************************************************************************************************
*/
static void AppTaskLED(void *p_arg)
{
    (void)p_arg;		/* ����������澯 */
		
	while (1) 
	{

//		bsp_LedToggle(3);
		OSTimeDlyHMSM(0, 0, 0, 500);	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	INT8U      err;


	/* ��������(MBOX) */
	//AppUserIFMbox = OSMboxCreate((void *)0);
	//COM1_Mbox = OSMboxCreate((void *)0);
	
	/* ������δʹ�� */
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
										
		/* ����ADC���� */
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
										
/* ����SolarArraySim���� */
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
										
		/* ����MPPT���� */
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
    
	/* ����LED���� */
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
*	�� �� ��: DispTaskInfo
*	����˵��: ��uCOS-II������Ϣ��ӡ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
	
	
	OS_TCB      *ptcb;	        /* ����һ��������ƿ�ָ��, TCB = TASK CONTROL BLOCK */
	OS_STK_DATA stk;	        /* ���ڶ�ȡ����ջʹ����� */

	ptcb    = &OSTCBTbl[0];		/* ָ���1��������ƿ�(TCB) */

	/* ��ӡ���� */
	printf("  Prio   Used  Free  Per     CPU  Taskname\r\n");

	OSTimeDly(10);
	
	/* ����������ƿ��б�(TCB list)����ӡ���е���������ȼ������� */
	while (ptcb != NULL)
	{
		/* 
			ptcb->OSTCBPrio : ������ƿ��б������������ȼ�
			ptcb->OSTCBTaskName : ������ƿ��б�����������ƣ���Ҫ�ڴ��������ʱ��
			����OSTaskNameSet()ָ���������ƣ����磺
			OSTaskNameSet(APP_TASK_USER_IF_PRIO, "User I/F", &err);
		*/
		OSTaskStkChk(ptcb->OSTCBPrio, &stk);	/* �������ջ���ÿռ� */
		printf("   %2d  %5d  %5d  %02d%%   %5.2f%%   %s\r\n", ptcb->OSTCBPrio, 
			stk.OSUsed, stk.OSFree, (stk.OSUsed * 100) / (stk.OSUsed + stk.OSFree),
		(float)ptcb->OSTCBCyclesTot / SystemCoreClock,
			ptcb->OSTCBTaskName);		
		ptcb = ptcb->OSTCBPrev;		            /* ָ����һ��������ƿ� */
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


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
