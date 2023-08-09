#include "stm32f4xx.h"
#define ARM_MATH_CM4

void TIM2_Init(void);
void configureLED(void);

void TIM4_Init (void)
{

//1. Enable TIM@ and GPIO clock
	RCC->APB1ENR |= (4UL<<0);  // enable TIM4 clock
		RCC->CFGR |= 0<<10; // set APB1 = 16 MHz

//2. Timer pre-scalar and period configuration
	TIM4->CR1 &= ~(0x0010);    		//Set the mode to Count up
		TIM4->PSC = 160-1;					//Set the Prescalar to 1kHz
		TIM4->ARR = 1000-1; 						//Set period (Auto reload) to 400
		//TIM2->SR &= ~(0x0001);				//Clear Update interrupt flag

}
void configureLED(void)
{
	RCC->AHB1ENR |=(1UL<<3);  //Enable GPIOD clock
	GPIOD->MODER |= (2UL<<24); //Alternate function
	GPIOD->AFR[1] |= (2UL<<16); //Timer4_Channel1

}

void configurePWM(void)
{
	TIM4->CCMR1 |=(6UL<<4); //PWM mode
	//TIM4->CCMR1 |=(1UL<<3);
	TIM4->CCER |=(0x1<<0);
}

int main ()
{
int i,brightness,stepSize;
brightness=1;
stepSize=1;
	TIM4_Init ();
	configurePWM();
	configureLED();
	TIM4->CCR1=500;  //Initial duty cycle 50%
	TIM4->CR1 |= 1UL; //Start the timer
while(1)
	{
			if(brightness>=999)
			brightness=1;
			brightness+=stepSize;
			TIM4->CCR1=brightness;
			for(i=0;i<5000;i++);
			}
}
