#include "stm32f10x.h"

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
	
	SPI1->CR1 |= SPI_CR1_BR;                 //Baud rate = Fpclk/256
  //SPI1->CR1 &= ~SPI_CR1_CPOL;              //Polarity cls signal CPOL = 0;
  //SPI1->CR1 &= ~SPI_CR1_CPHA;              //Phase cls signal    CPHA = 0;
  //SPI1->CR1 |= SPI_CR1_DFF;                //16 bit data
	//SPI1->CR1 &= ~SPI_CR1_LSBFIRST;          //MSB will be first
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;  //Software slave management & Internal slave select
	
  SPI1->CR1 |= SPI_CR1_MSTR;               //Mode Master
  SPI1->CR1 |= SPI_CR1_SPE;                //Enable SPI2
	
}

int main()
{
	Init_SPI();

	uint8_t data[2U] = {255,0};
	
	while(!(SPI1->SR & SPI_SR_TXE));
	
	// Sec CS in 0
	GPIOA->ODR &= ~GPIO_ODR_ODR4;   
  for (int i = 0; i < 2; i++)
	{
		SPI1->DR = data[i];  
		while(!(SPI1->SR & SPI_SR_RXNE));
    
		data[i] = SPI1->DR;  
	}
	// Sec CS in 1
	GPIOA->ODR |= GPIO_ODR_ODR4;
	
	return 0;
}
