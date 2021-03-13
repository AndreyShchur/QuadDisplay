#include "stm32f10x.h"


// Init SPI1
// Pin config:
//     CLK  - GPIOA5
//     MOSI - GPIOA7
//     MISO - GPIOA6
static void Init_SPI(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// CLK and MOSI Alternative function Push-pull
	GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_MODE7;
	GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF7_0);
	
	// MISO floating input
	GPIOA->CRL &= ~GPIO_CRL_MODE6;
	GPIOA->CRL |= GPIO_CRL_CNF6_0;
	 
	
	// CS Output Push-pull
	GPIOA->CRL |= GPIO_CRL_MODE4;
	GPIOA->CRL &= ~GPIO_CRL_CNF4; 
	
	// Set clock SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	//Baud rate = Fpclk/256
	SPI1->CR1 |= SPI_CR1_BR;
	
	//8 bit data
	SPI1->CR1 &= ~SPI_CR1_DFF;
	
	//MSB will be first
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	
	//Software slave management & Internal slave select
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	
	//Mode Master
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	//Enable SPI1
	SPI1->CR1 |= SPI_CR1_SPE;
}


//---------------------MAIN FUNC--------------------------
int main()
{
	Init_SPI();
	
	uint8_t counter = 0;
	uint8_t i = 0;
	long j = 0;
	
	// SPI1 ready for work
	while(!(SPI1->SR & SPI_SR_TXE));
	
	while(1)
	{
		// CS in 0
		GPIOA->ODR &= ~GPIO_ODR_ODR4;
		// Write data
		SPI1->DR = i;
		// Wait writing
		while(!(SPI1->SR & SPI_SR_TXE));
		
		// CS in 1
		GPIOA->ODR |= GPIO_ODR_ODR4;
	
		// Shift left 1 bit
		i = 1U << counter;
		
		// Check shift counter
		counter = (counter == 8) ? 0 : counter + 1;
		
		// Delay
		while(j < 2000000)
		{
			j++;
		}
		
		j = 0;
	}
	
	return 0;
}
