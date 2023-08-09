/*
 * No PLL has been used!
 * 3 Phase SPWM with 3us deadtime, 8kHz PWM, 50Hz sinewave on Timer 1
 * Pin Details:
 * PE8	CH1N
 * PE9	CH1
 * PE10	CH2N
 * PE11	CH2
 * PE12	CH3N
 * PE13	CH3
 */


#include "stm32f4xx.h"
#include <math.h>

#define PI 3.14159
#define RES 160
#define AMP 1000

volatile int SINE_VALUES1[RES];
volatile int SINE_VALUES2[RES];
volatile int SINE_VALUES3[RES];

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


/*

const uint16_t sineArray1[160] ={1000, 1040, 1079, 1118, 1157, 1196, 1235, 1273,
		1311, 1348, 1385, 1421, 1457, 1491, 1525, 1559, 1591, 1622, 1653, 1682,
		1711, 1738, 1764, 1789, 1812, 1835, 1856, 1876, 1894, 1911, 1927, 1941,
		1953, 1965, 1974, 1982, 1989, 1994, 1998, 2000, 2000, 1999, 1996, 1992,
		1986, 1979, 1970, 1959, 1947, 1934, 1919, 1903, 1885, 1866, 1846, 1824,
		1801, 1777, 1751, 1724, 1697, 1668, 1638, 1607, 1575, 1542, 1509, 1474,
		1439, 1403, 1367, 1330, 1292, 1254, 1216, 1177, 1138, 1099, 1059, 1020,
		980,  941,  901,  862,  823,  784,  746,  708,  670,  633,  597,  561,
		526,  491,  458,  425,  393,  362,  332,  303,  276,  249,  223,  199,
		176,  154,  134,  115,   97,   81,   66,   53,   41,   30,   21,   14,
		8,    4,    1,    0,    0,    2,    6,   11,   18,   26,   35,   47,
		59,   73,   89,  106,  124,  144,  165,  188,  211,  236,  262,  289,
		318,  347,  378,  409,  441,  475,  509,  543,  579,  615,  652,  689,
		727,  765,  804,  843,  882,  921,  960, 1000};

const uint16_t sineArray2[160] = {1866, 1846, 1824, 1801, 1777, 1751, 1724, 1697,
		1668, 1638, 1607, 1575, 1542, 1509, 1474, 1439, 1403, 1367, 1330, 1292,
		1254, 1216, 1177, 1138, 1099, 1059, 1020,  980,  941,  901,  862,  823,
		784,  746,  708,  670,  633,  597,  561,  526,  491,  458,  425,  393,
		362,  332,  303,  276,  249,  223,  199,  176,  154,  134,  115,   97,
		81,   66,   53,   41,   30,   21,   14,    8,    4,    1,    0,    0,
		2,    6,   11,   18,   26,   35,   47,   59,   73,   89,  106,  124,
		144,  165,  188,  211,  236,  262,  289,  318,  347,  378,  409,  441,
		475,  509,  543,  579,  615,  652,  689,  727,  765,  804,  843,  882,
		921,  960, 1000, 1040, 1079, 1118, 1157, 1196, 1235, 1273, 1311, 1348,
		1385, 1421, 1457, 1491, 1525, 1559, 1591, 1622, 1653, 1682, 1711, 1738,
		1764, 1789, 1812, 1835, 1856, 1876, 1894, 1911, 1927, 1941, 1953, 1965,
		1974, 1982, 1989, 1994, 1998, 2000, 2000, 1999, 1996, 1992, 1986, 1979,
		1970, 1959, 1947, 1934, 1919, 1903, 1885, 1866};

const uint16_t sineArray3[160] = {134, 115,  97,  81,  66,  53,  41,  30,
		21,  14,   8,   4,   1,   0,   0,   2,   6,  11,  18,  26,  35,
		47,  59,  73,  89, 106, 124, 144, 165, 188, 211, 236, 262, 289,
		318, 347, 378, 409, 441, 475, 509, 543, 579, 615, 652, 689, 727,
		765, 804, 843, 882, 921, 960, 1000, 1040, 1079, 1118, 1157, 1196,
		1235, 1273, 1311, 1348, 1385, 1421, 1457, 1491, 1525, 1559, 1591,
		1622, 1653, 1682, 1711, 1738, 1764, 1789, 1812, 1835, 1856, 1876,
		1894, 1911, 1927, 1941, 1953, 1965, 1974, 1982, 1989, 1994, 1998,
		2000, 2000, 1999, 1996, 1992, 1986, 1979, 1970, 1959, 1947, 1934,
		1919, 1903, 1885, 1866, 1846, 1824, 1801, 1777, 1751, 1724, 1697,
		1668, 1638, 1607, 1575, 1542, 1509, 1474, 1439, 1403, 1367, 1330,
		1292, 1254, 1216, 1177, 1138, 1099, 1059, 1020, 980, 941, 901,
		862, 823, 784, 746, 708, 670, 633, 597, 561, 526, 491, 458, 425,
		393, 362, 332, 303, 276, 249, 223, 199, 176, 154, 134};
*/

uint8_t i = 0;

void Clocks_Init(void);
void GPIO_Pin_Init(void);
void PWM_Init(void);		//TIM1 for PWM
void TIM2_Init (void);		//TIM2 for ISR

void Clocks_Init(void)
{
		RCC->APB2ENR |= (1UL<<0);  //For TIM1
		RCC->AHB1ENR |= (17UL<<0); //For GPIOA and GPIOE
}

void GPIO_Pin_Init(void)
{
	//TIM1CH1	PE9
	GPIOE->MODER |= (2UL<<18); //Alternate function
	GPIOE->AFR[1] |= (1UL<<4); //Timer1_Channel1

	//TIM1CH1N	PE8
	GPIOE->MODER |= (2UL<<16); //Alternate function
	GPIOE->AFR[1] |= (1UL<<0); //Timer1_Channel1N

	//TIM1CH2	PE11
	GPIOE->MODER |= (2UL<<22); //Alternate function
	GPIOE->AFR[1] |= (1UL<<12); //Timer1_Channel2

	//TIM1CH2N	PE10
	GPIOE->MODER |= (2UL<<20); //Alternate function
	GPIOE->AFR[1] |= (1UL<<8); //Timer1_Channel2N

	//TIM1CH3	PE13
	GPIOE->MODER |= (2UL<<26); //Alternate function
	GPIOE->AFR[1] |= (1UL<<20); //Timer1_Channel3

	//TIM1CH3N	PE12
	GPIOE->MODER |= (2UL<<24); //Alternate function
	GPIOE->AFR[1] |= (1UL<<16); //Timer1_Channel3N
}

void PWM_Init(void)
{
	TIM1->CR1 &= ~(0x0010); 			//Upcounter mode
	TIM1->PSC = 1-1;					//Set the Prescalar so freq = 16MHz
	TIM1->ARR = 2000-1;					//Set ARR for 2000 steps
	TIM1->CCMR1 |=(6UL<<4); 			//PWM mode 1
	TIM1->CCMR1 |=(6UL<<12); 			//PWM mode 1
	TIM1->CCMR2 |=(6UL<<4); 			//PWM mode 1

	//RM0090 pg 544 for steps
	//RM0090 Table 95 pg. 579 for
	TIM1->CCER |= (5UL<<0);				//CC1P, CC1NP are 0, CC1E, CC1NE are 1 to configure as output
	TIM1->CCER |= (5UL<<4);				//CC2P, CC2NP are 0, CC2E, CC2NE are 1 to configure as output
	TIM1->CCER |= (5UL<<8);				//CC3P, CC3NP are 0, CC3E, CC3NE are 1 to configure as output

	TIM1->BDTR |= (1UL<<15);			//MOE = 1, when it falls to 0 asynchronously by hardware, break-circuit
										//is enabled. We think break-circuit IS responsible for the break-time

	TIM1->CR2 |=(3UL<<8);				//Refer the output example on pg. 545
	TIM1->CR2 |=(3UL<<10);				//After period a, OCxN is 1
	TIM1->CR2 |=(3UL<<12);				//After period b, OCx is 1
										//Therefore OISx and OISxN are both 1

	TIM1->BDTR |= (1UL<<11);			//OSSR is 1
	TIM1->BDTR |= (1UL<<14);			//AOE = 1, Automatically make MOE 1 after Update Event i.e. break input is not be active

										//Now for Delay Time Generator make DTG as 48
	TIM1->BDTR |= (48UL<<0);

}

void TIM2_Init (void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->CFGR |= 0<<10;


	TIM2->CR1 &= ~(0x0010);
	TIM2->PSC = 1-1;
	TIM2->ARR = 2000-1;
	TIM2->SR &= ~(0x0001);
	NVIC->ISER[0] |= 1<<28;
	TIM2->DIER |=(1<<0);
}

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif

    RCC->CR |= 1ul;
}

int main(void)
{
	SystemInit();
	LookupTables_Init();
	Clocks_Init();
	GPIO_Pin_Init();
	PWM_Init();
	TIM2_Init ();

	TIM1->CCR1 = SINE_VALUES1[0];
	TIM1->CCR2 = SINE_VALUES2[0];
	TIM1->CCR3 = SINE_VALUES3[0];

	TIM2->CR1 |= (1<<0);
	TIM1->CR1 |= (1<<0);

	while(1)
	{

	}
}

void TIM2_IRQHandler( )
{
	if(i==160)
	{
		i = 0;
	}
	else
	{
		i++;
	}

	TIM1->CCR1 = SINE_VALUES1[i];
	TIM1->CCR2 = SINE_VALUES2[i];
	TIM1->CCR3 = SINE_VALUES3[i];

	TIM2->SR &= ~(0x0001);
}

/* ****************************************       NOTES FOR SNEHA      *********************************************************
 * to configure PLLs just change the SystemInit() and ClocksInit() functions and not the whole thing or i will kill you :)
 * also...RES and AMP are exactly what you think they are
 *
 */
