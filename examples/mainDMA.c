#include "stm32f4xx.h"
#include <math.h>

#define PI 3.14159
#define RES 160
#define AMP 1000

const uint16_t LOOKUP_TABLE[10] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

int SINE_VALUES1[RES];
int SINE_VALUES2[RES];
int SINE_VALUES3[RES];

void LookupTables_Init()
{
	float angle;
	for(int i = 0; i < RES; i++)
	{
		angle = 2 * PI * i / (RES - 1);
		SINE_VALUES1[i] = (int) (AMP * sin(angle) + AMP);
		SINE_VALUES2[i] = (int) (AMP * sin(angle + (2 * PI / 3)) + AMP);
		SINE_VALUES3[i] = (int) (AMP * sin(angle - (2 * PI / 3)) + AMP);
	}
}

void Timer4_Init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	RCC->CFGR &= ~(15ul << 13);
	TIM4->PSC = 16000 - 1;
	TIM4->CR1 |= (1UL << 2);
	TIM4->CR2 |= (1UL << 3); // DMA request on update event
	TIM4->CNT = 0;
	TIM4->ARR = 1000 - 1;
	TIM4->CCER |= 0x1;
	TIM4->CCMR1 |=(6ul << 4);
	TIM4->DIER |= (1ul << 8) | (1UL << 0); // UEV DMA Trigger
	NVIC->ISER[0] = (1UL << 30);
	TIM4->DCR |= TIM_DCR_DBL_0;
	TIM4->DCR |= 12UL;
}

void Timer4_Start()
{
	TIM4->CR1 |= 1ul;
}

void Timer4_Stop()
{
	TIM4->CR1 &= ~(1ul);
}

void Timer4_SetDutyCycle(uint16_t duty_cycle)
{
	TIM4->CCR1 = duty_cycle;
}

void GPIOInit()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  //Enable GPIOD clock
	GPIOB->MODER |= (2UL<<12); //Alternate function
	GPIOB->AFR[0] |= (2UL<<24); //Timer1_Channel 1
}

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif

    RCC->CR |= 1ul;
}
/*
void HardFault_Handler()
{
	while(1);
}
*/

void DMA_Init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	DMA1->HISR &= (0UL << 15);
	DMA1->LISR &= (0UL << 15);
	DMA1_Stream6->CR &= (0UL << 15); // clear
	while(DMA1_Stream6->CR & 1UL);
	DMA1_Stream6->CR |= (2UL << 25); // channel select TIM4 Channel 1
	DMA1_Stream6->CR |= (1UL << 13); // memory size half word
	DMA1_Stream6->CR |= (1UL << 11); // peripheral size half word
	DMA1_Stream6->CR |= (1UL << 6);  // memory to peripheral transfer
	DMA1_Stream6->CR |= (1UL << 10); // memory increment mode
	DMA1_Stream6->CR |= (1UL << 8);  // circular mode
	DMA1_Stream6->CR |= (1UL << 4);  // interrupt enable
	DMA1_Stream6->CR |= (3UL << 16);
	DMA1_Stream6->NDTR = 7UL ;
	DMA1_Stream6->PAR = &(TIM4->DMAR);
	DMA1_Stream6->M0AR = LOOKUP_TABLE;
	NVIC->ISER[0] = (1UL << 17);
}

void DMA_Start()
{
	DMA1_Stream6->CR |= 1UL; // enable
}

/*
int pulse_count = 0;                                  // For debugging //
void TIM4_IRQHandler(){                               // View pulse_count and dma_calls in live expressions to monitor the number of DMA requests and Timer 
	pulse_count++;				      // update events
	TIM4->SR &= ~(0x0001);
}

int dma_calls = 0;
void DMA1_Stream6_IRQHandler()
{
	dma_calls++;
	DMA1->HIFCR |= (1UL << 21);
}
*/

int main()
{
	SystemInit();
	LookupTables_Init();
	Timer4_Init();
	DMA_Init();
	//Timer4_SetDutyCycle(500);
	GPIOInit();
	Timer4_Start();
	DMA_Start();
	while(1);
	return 0;
}
