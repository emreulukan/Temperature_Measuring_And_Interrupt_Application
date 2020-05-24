#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

uint16_t Temp_Value;
float x;

void GPIO_Configuration(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode = 0x03;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = 0x01;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = 0x00;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = 0x01;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = 0x01;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13 |
			GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = 0x01;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void ADC_Configuration(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_6b;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div6;
	ADC_CommonInit(&ADC_CommonInitStruct);
	ADC_Cmd(ADC1, ENABLE);
}

void EXTI_Configuration(){
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = 0x00;
	EXTI_InitStruct.EXTI_Trigger = 0x0C;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

uint16_t ADC_Value(){
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

void delay(uint32_t t){
	while(t--);
}

void EXTI0_IRQHandler(){
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET){
		GPIO_ResetBits(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13 |
				GPIO_Pin_14 | GPIO_Pin_15);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay(2680000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delay(2680000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay(2680000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(2680000);
	}
}

int main(void)
{
	GPIO_Configuration();
	ADC_Configuration();
	EXTI_Configuration();
  while (1)
  {
	  Temp_Value = ADC_Value();
	  x = (10000/255)*Temp_Value;
	  if(x <= 1200){
		  GPIO_ResetBits(GPIOD, GPIO_Pin_1);
		  GPIO_SetBits(GPIOD, GPIO_Pin_0);
	  }
	  else if(x > 1250){
		  GPIO_ResetBits(GPIOD, GPIO_Pin_0);
		  GPIO_SetBits(GPIOD, GPIO_Pin_1);
	  }
  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
