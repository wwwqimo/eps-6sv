/*
**********************************************************************************************************************
**APP ADC
**
**
*****************************************************************************************************************************
*/
#include "app_adc.h"


extern __IO uint16_t uhADCConvertedValue[NumofConv];


#define size_filter 5    //���廬���˲���������С
//adc map,��Ӧeps_hk_adc_t �ṹ���������ڶ�ȡ����ԭֵ�е�λ�ã�
//����0--15��Ӧ��mcu adc ��16-31��Ӧ��ad7490��ȡ��������
const uint8_t adc_map[]={0,2,4,5,6,7, /*c_s[6]*/
									13,1,3,9,8,15,/*v_s[6]*/
									12,10,11,26,27,/*c_sva,c_bus,v_bus,v_5v,v_3v*/
									29,30,31,20,19,/*c_reg[5]*/
									22,21,28,23,/*c_ureg[4]*/
									24,14,18,25,/*temp_eps[4]*/
									17,16};/*temp_bat[2]*/
//����adc���ݱ�������ѹ��Ϊ��ѹ�ȣ�����ֵ49.9k/10k,ת����ϵΪ V_real = V*Vdiv
//������Ϊ1/(R*G),����ֵΪR=0.051ŷ��G=25V/V;���ߵ�����������R=0.051/2��ת����ϵΪC=V*(1/R*G)
//�¶���ΪV = 0.75+0.01*(T-25);ת����ϵΪT = (V-0.75)/0.01+25
const float  adc_div[]={0.7843,0.7843,0.7843,0.7843,0.7843,0.7843,
									5.99,5.99,5.99,5.99,5.99,5.99,
									1.5686,1.5686,4.30,5.99,5.99,
									0.7843,0.7843,0.7843,0.7843,0.7843,
									0.7843,0.7843,0.7843,0.7843,
									1,1,1,1,1,1};
static uint16_t sf_buffer[size_filter][hk_adc_num];//���廬���˲�������
//adc ���ݴ�����飬�Ѿ������˲�����
uint16_t adc_data[hk_adc_num];							
//����adc���ݽṹ��
eps_hk_adc_t eps_adc_data;	
	

							
void slidingfilter_test(void);
												
									
void ADCSample(void)
{

	//���� uint16_t ����ָ�룬ָ��adc_data[]����
	uint16_t *pdata;
	//���� eps_hk_adc_t���ͽṹ��ָ��ָ��ȫ�ֱ���eps_adc_data
	eps_hk_adc_t *peps_hk_adc;
	
	pdata = &adc_data[0];
	peps_hk_adc = &eps_adc_data;
	//��ȫ�ֱ���eps_adc_data��adc_data�����ڴ�������ʼ��
	memset(peps_hk_adc,0,sizeof(eps_hk_adc_t));
	memset(pdata,0,sizeof(uint16_t)*hk_adc_num);
	
	while(1)
	{
		//��ȡmcu adc �� ad7490 adcֵ������ֵ���л����˲�����
		pdata = adc_acquisition_processing(pdata);
		//��������������ᣬ��ת��Ϊ��ʵֵ
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
**��uint16_t��ʽ��adcֵת��Ϊ��ʵֵ
** ��ѹ����λ mv
** ��������λ ma
** �¶�����λ degc
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
**adc ���ݻ�ȡ�ʹ������
** ��ȡmcu adc ���������ݣ���ȡad7490����
** �����ݽ��л����˲�����������
**adc_dest ���ݽ�������ָ��
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

//�����˲����Գ���
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
** �����˲��㷨
** * adc_value ��������ָ��  
** adc_size�������ݳ���
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
