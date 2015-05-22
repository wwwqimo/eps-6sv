/*
**********************************************************************************************************************
**APP ADC
**
**
*****************************************************************************************************************************
*/
#include "app_adc.h"


extern __IO uint16_t uhADCConvertedValue[NumofConv];


#define size_filter 5    //定义滑动滤波缓存区大小
//adc map,对应eps_hk_adc_t 结构体中数据在读取到的原值中的位置，
//其中0--15对应于mcu adc ，16-31对应于ad7490读取到的数据
const uint8_t adc_map[]={0,2,4,5,6,7, /*c_s[6]*/
									13,1,3,9,8,15,/*v_s[6]*/
									12,10,11,26,27,/*c_sva,c_bus,v_bus,v_5v,v_3v*/
									29,30,31,20,19,/*c_reg[5]*/
									22,21,28,23,/*c_ureg[4]*/
									24,14,18,25,/*temp_eps[4]*/
									17,16};/*temp_bat[2]*/
//定义adc数据比例，电压量为分压比，典型值49.9k/10k,转换关系为 V_real = V*Vdiv
//电流量为1/(R*G),典型值为R=0.051欧，G=25V/V;总线电流采样电阻R=0.051/2，转换关系为C=V*(1/R*G)
//温度量为V = 0.75+0.01*(T-25);转换关系为T = (V-0.75)/0.01+25
const float  adc_div[]={0.7843,0.7843,0.7843,0.7843,0.7843,0.7843,
									5.99,5.99,5.99,5.99,5.99,5.99,
									1.5686,1.5686,4.30,5.99,5.99,
									0.7843,0.7843,0.7843,0.7843,0.7843,
									0.7843,0.7843,0.7843,0.7843,
									1,1,1,1,1,1};
static uint16_t sf_buffer[size_filter][hk_adc_num];//定义滑动滤波缓存区
//adc 数据存放数组，已经经过滤波处理
uint16_t adc_data[hk_adc_num];							
//定义adc数据结构体
eps_hk_adc_t eps_adc_data;	
	

							
void slidingfilter_test(void);
												
									
void ADCSample(void)
{

	//定义 uint16_t 类型指针，指向adc_data[]数组
	uint16_t *pdata;
	//定义 eps_hk_adc_t类型结构体指针指向全局变量eps_adc_data
	eps_hk_adc_t *peps_hk_adc;
	
	pdata = &adc_data[0];
	peps_hk_adc = &eps_adc_data;
	//对全局变量eps_adc_data，adc_data分配内存区并初始化
	memset(peps_hk_adc,0,sizeof(eps_hk_adc_t));
	memset(pdata,0,sizeof(uint16_t)*hk_adc_num);
	
	while(1)
	{
		//获取mcu adc 和 ad7490 adc值，并对值进行滑动滤波处理，
		pdata = adc_acquisition_processing(pdata);
		//将数据填入机构提，并转换为真实值
		peps_hk_adc = adc_to_real(pdata,peps_hk_adc);
		#if USER_DEBUG_EN > 0
//		eps_print(print_adc);
    #endif
		eps_data_handling();
		OSTimeDlyHMSM(0, 0, 0, 1000);	
	}
}


/**********************************************************************************************************************/
/*
**将uint16_t格式的adc值转换为真实值
** 电压量单位 mv
** 电流量单位 ma
** 温度量单位 degc
*/
static eps_hk_adc_t * adc_to_real(uint16_t *adc_uint,eps_hk_adc_t *adc_dest)
{
	uint8_t i;
	uint16_t * adc_dest1;
	int16_t * adc_dest2;
	adc_dest1 = (uint16_t*)adc_dest;
	adc_dest2 = (int16_t*)(&(adc_dest->temp_eps[0]));
	for(i=0;i<hk_adc_num-6;i++)
	{
		*(adc_dest1+i) = (uint16_t)(((float)(*(adc_uint+adc_map[i])))*adc_div[i]*ADC_REF/ADC_FullScale);
	}
	for(i=0;i<6;i++)
	{
		*(adc_dest2+i) = (int16_t)(((((float)(*(adc_uint+adc_map[i+26])))*ADC_REF/ADC_FullScale-750)/10)+25);
	}
	
	return adc_dest;
}
/*
**adc 数据获取和处理程序
** 获取mcu adc 数组内数据，读取ad7490数据
** 对数据进行滑动滤波处理，并返回
**adc_dest 数据接收数组指针
*/
static uint16_t * adc_acquisition_processing(uint16_t *adc_dest)
{
	uint16_t *ptr_ad7490;
	uint16_t i;
	ptr_ad7490 = ad7490_read_NoIntSeq();
	for (i=0;i<16;i++)
	{
		*(adc_dest+i) = SlidingFilter(uhADCConvertedValue[i],i);
		*(adc_dest+i+16) = SlidingFilter(*(ptr_ad7490+i),i+16);
	}
	return adc_dest;
}

//滑动滤波测试程序
void slidingfilter_test(void)
{
	uint16_t i,j;
	uint16_t temp;
	uint16_t tempadc[32];
	for (j=0;j<32;j++)
	{
		tempadc[j] = j;
		temp = SlidingFilter(tempadc[j],j);
		printf("temp[%2d]=%4d\r\n",j,temp);
	}

}


/*
**********************************************************************************************************************
** 滑动滤波算法
** * adc_value 输入数据指针  
** adc_size输入数据长度
**
******************************************************************************************************************
*/
static uint16_t  SlidingFilter(uint16_t adc_value,uint16_t adc_count)
{
	static uint16_t ptr[hk_adc_num] = {0};
	uint16_t k;
	uint32_t sum =0;

	sf_buffer[ptr[adc_count]++][adc_count] = adc_value;
	
	if(ptr[adc_count] >=size_filter)	ptr[adc_count] = 0;
	
	for(k=0;k<size_filter;k++)
	{
		sum += sf_buffer[k][adc_count];
	}
	return (uint16_t)sum/size_filter;
}

/********************************************************************************************************************/
