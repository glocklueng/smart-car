#include "adc.h"
#include "gpio.h"
#include "util.h"
#include "usart.h"
#include "control.h"

volatile u16 adcValue[2] = {0, 0};
volatile uint8_t adc_flag = 1;
volatile uint8_t adc_valid_flag = 1;
volatile uint16_t adc_threshold = 1000;

//DMA�����ADC����
void adc_config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	
	gpio_an_no_init(GPIOB, GPIO_Pin_0|GPIO_Pin_1);
	
	DMA_Cmd(DMA2_Stream0, DISABLE);
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)adcValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;	//ADCCLK=PCLK2/4=84/4=21Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 2;				//������������2��ת��
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_480Cycles );	//����ͨ��8����˳��Ϊ1, ����ʱ��Ϊ480������	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_480Cycles );	//����ͨ��9����˳��Ϊ1, ����ʱ��Ϊ480������	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

void adc_start_converte(void)   
{
	ADC_SoftwareStartConv(ADC1);
}

void adc_get_voltage(volatile u16 *adcValue, float *voltage)
{
	voltage[0] = adcValue[0] * 3.0f / 4096.0f;
	voltage[1] = adcValue[1] * 3.0f / 4096.0f;
}

//��ʱ��������ADC����
void adc_tim_trig_config(uint32_t period, uint32_t prescaler)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	gpio_an_no_init(GPIOC, GPIO_Pin_5);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;	//ADCCLK=PCLK2/4=84/4=21Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//�ر�ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;	//�ⲿ����������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;				//������������1��ת��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_84Cycles );	//����ͨ��5����˳��Ϊ1, ����ʱ��Ϊ84������	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	nvic_config(ADC_IRQn, 2);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = period-1;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = period/2;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);    
	TIM_InternalClockConfig(TIM2);  
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_UpdateDisableConfig(TIM2, DISABLE); 
}

void ADC_IRQHandler(void)
{
	static uint16_t adc_val[5] = {0};
	uint16_t ave;
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET){
		adc_val[4] = adc_val[3];
		adc_val[3] = adc_val[2];
		adc_val[4] = adc_val[1];
		adc_val[1] = adc_val[0];
		adc_val[0] = ADC1->DR;
		ave = (adc_val[0]+adc_val[1]+adc_val[2]+adc_val[3]+adc_val[4])/5;
		if(adc_valid_flag == 1){
			if(ave > adc_threshold){
				adc_flag = 0;
			}
			else{
				adc_flag = 1;
			}
		}
	}
}

