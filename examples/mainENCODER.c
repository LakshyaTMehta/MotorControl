#include "stm32f4xx.h"

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif

    RCC->CR |= 1ul;
}

void Clocks_Init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;      //TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;      //TIM3
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;     //GPIO A
}

void GPIO_Init()
{
	GPIOA->AFR[1] = 0;
	GPIOC->AFR[0] = 0;

	GPIOA->MODER |= (2UL << 30);      	//AF for PA15
	GPIOA->AFR[1] |= (1UL << 28);		//AF1 for TIM2_ETR

	GPIOA->MODER |= (2UL << 12);		//AF for PA6
	GPIOA->AFR[0] |= (2UL << 24);		//AF2 for TIM3_CH1

	GPIOA->MODER |= (2UL << 14);		//AF for PA7
	GPIOA->AFR[0] |= (2UL << 28);		//AF2 for TIM3_CH2
}

void Timer2_Init()
{
	TIM2->CR1 = 0;
	TIM2->PSC 	= 	1000;           //Prescalar = 0
	TIM2->ARR 	= 	0xFFFF;   		//Period = 65536
	TIM2->SMCR 	|= 	(0UL << 12);	//Trigger Pre-scaler = 0
	TIM2->SMCR	|=	(0xF << 8);		//Trigger Filter

	//TIM2->SMCR 	|=	(0b101 << 4);	//Trigger Channel 1
	TIM2->SMCR 	|=	(0b111 << 4);	//Trigger External

	TIM2->SMCR  |=	(0b110 << 0);	//Slave Trigger Start Mode
	TIM2->CCMR1 |= 	1UL;			//Timer2 Channel1 input TI1 as trigger source
	TIM2->DIER  |=	(1UL << 6);		//Trigger Interrupt
}

void Timer3_Init()
{
	TIM3->CR1 = 0;
	//TIM3->PSC 	= 	1000;           //Prescalar
	TIM3->ARR 	= 	2048;   		//Period = 65536

	//TIM3->SMCR 	|= 	(0UL << 12);	//Trigger Pre-scaler = 0
	//TIM3->SMCR	|=	(0b0011 << 8);	//Trigger Filter
	//TIM3->SMCR 	|=	(0b101 << 4);	//Trigger Channel 1

	//TIM3->SMCR 	|=	(0b101 << 4);	//Trigger Source TI1
	TIM3->SMCR  |=	(0b011 << 0);	//Slave Encoder 3 Mode

	TIM3->CCMR1 |= 	(1UL << 0);		//Timer2 Channel1 input TI1 as trigger source
	TIM3->CCMR1 |= 	(1UL << 8);		//Timer3 Channel2 input TI2 as trigger source

	TIM3->CCMR1	|=	(0UL << 2);		//TI1 Pre-scaler
	TIM3->CCMR1	|=	(0b0000 << 4);	//TI1 Filter

	TIM3->CCMR1	|=	(0UL << 10);	//TI2 Pre-scaler
	TIM3->CCMR1	|=	(0b0000 << 12);	//TI2 Filter

	//TIM3->DIER  |=	(1UL << 6);		//Trigger Interrupt
}

void Timer2_Start()
{
	TIM2->CR1 |= (1UL << 0);
}

void Timer3_Start()
{
	TIM3->CR1 |= (1UL << 0);
}

void Timer2_Stop()
{
	TIM2->CR1 &= ~(1UL << 0);
}

void Timer3_Stop()
{
	TIM3->CR1 &= ~(1UL << 0);
}

void Interrupts_Init()
{
	NVIC->ISER[0] |= (1UL << 28);
}

int trigger = 0;
float speed_in_rpm = 0;
float angle_in_degree = 0;

void TIM2_IRQHandler()
{
	trigger++;
	speed_in_rpm 		= 	(float) 960000 / TIM2->CNT;
	TIM2->CNT	=	0;
	TIM3->CNT	=	0;
	TIM2->SR	&=	~(1UL << 6);
}

int main()
{
	Clocks_Init();
	GPIO_Init();
	Timer2_Init();
	Timer3_Init();
	Interrupts_Init();
	Timer2_Start();
	Timer3_Start();
 	while(1)
 	{
 		angle_in_degree = (float) 360 / 2048 * TIM3->CNT;
 	};

	return 0;
}


