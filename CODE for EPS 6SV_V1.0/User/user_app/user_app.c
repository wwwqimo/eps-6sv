#include "user_app.h"
#include "user_cfg.h"
#include "includes.h"



#define eps_ppt ppt_conv

void user_app_Init(void)
{
	eps_data_Init();
}

void user_data_handling(void)
{
	eps_data_handling();
}

void user_app(void)
{
	uint8_t time_fill=0;
	user_app_Init();
	outall_en(ENABLE);
	
	while(1)
	{
		while(time_fill++ > 10)
		{
			time_fill = 10;
			user_data_handling();
		}
		
		OSTimeDlyHMSM(0, 0, 0, USER_TIME_DLY);
	}
}




/********************************************************************************************/

