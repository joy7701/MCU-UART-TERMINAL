#include "stm32f10x.h" // Заголовочный файл устройства 
#include "string.h"
char massive[200];
char commandi[20];
int flagEnd = 0;
char param;
int Led1ON = 0;
int Led2ON = 0;
int Led1BLINK = 0;
int Led2BLINK = 0;
int resault;
int a=0;
void temp_delay(uint32_t delayTime)
{
	uint32_t counter = 0;
	for(;counter < delayTime;counter++)
	{}
}
void doCommand(void);
int main()
{
	
	// ИНИЦИАЛИЗАЦИЯ ДИОДА 
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH = 0;
	GPIOC->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1;	//РЕЖИМ ВЫХОДА СКОРОСТЬ 2 МГЦ
	GPIOC->CRH &=~(GPIO_CRH_CNF9_0 | GPIO_CRH_CNF9_1);
	GPIOC->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1;	//РЕЖИМ ВЫХОДА СКОРОСТЬ 2 МГЦ
	GPIOC->CRH &=~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF8_1);
	// ИНИЦИАЛИЗАЦИЯ UART
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //ВКЛЮЧЕНИЕ ПОРТА А
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //ВКЛЮЧЕНИЕ АЛЬТЕРНАТИВНЫХ ФУНКЦИЙ НА ШИНЕ
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // ВКЛЮЧЕНИЕ ТАКТИРОВАНИЯ ДЛЯ USART1 
	// PA9 ПЕРЕДАЮЩАЯ НОЖКА
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9); //  СБРОС НАСТРОЕК ПОРТА А
	GPIOA->CRH |= (GPIO_CRH_CNF9_1 |GPIO_CRH_MODE9_1); /// ДЛЯ 9 ПИНА ЗАДАЕМ МОД ПУШ-ПУЛ С ЧАСТОТОЙ 2МГЦ
	// PA10 ПО УМОЛЧАНИЮ ОСТАВЛЯЕМ
	USART1->BRR = 0x9C4; // НАСТРОЙКА ЧАСТОТЫ ТАКТИРОВАНИЯ 
	USART1->CR1 |= USART_CR1_UE; //ВКЛЮЧЕНИЕ USART В БИТ UE ПОДАЕМ ЕДИНИЦУ
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE); // ВКЛЮЧЕНИЕ ПЕРЕДАЧИ И ПРИЕМА
	USART1->CR1 |= USART_CR1_RXNEIE; //ВКЛЮЧЕНИЕ ПРЕРЫВАНИЯ ПО ПОСТУПЛЕНИЮ ДАННЫХ IE INPUT ENABLE
	NVIC_EnableIRQ(USART1_IRQn); // сообщаем КОНТРОЛЛЕРУ ПРЕРЫВАНИЙ ПРЕРЫВАНИЕ USART1
	
	while(1)
	{
		if(flagEnd == 1)
	{
		doCommand();
		flagEnd = 0;
		
		
	}
	
		if(Led1ON ==1)
			{
				
				GPIOC->BSRR = GPIO_BSRR_BS9;
		
			}
		if(Led1ON == 0)
			{
				
				GPIOC->BSRR = GPIO_BSRR_BR9;
			}
		if (Led2ON == 1)
			{
				
				GPIOC->BSRR = GPIO_BSRR_BS8;
			}
		if (Led2ON == 0)
			{
				GPIOC->BSRR = GPIO_BSRR_BR8;
			}
		if(Led1BLINK == 1)
			{
				
				GPIOC->BSRR = GPIO_BSRR_BS9;
					for(a = 0;a<=2000000;a++);
				GPIOC->BSRR = GPIO_BSRR_BR9;
					for(a = 0;a<=2000000;a++);
			}

		if(Led2BLINK == 1)
			{
				GPIOC->BSRR = GPIO_BSRR_BS8;
					for(a = 0;a<=2000000;a++);
				GPIOC->BSRR = GPIO_BSRR_BR8;
					for(a = 0;a<=2000000;a++);
			}
	}
	return 1;
}
void USART1_IRQHandler(void) // обработчик событий по usart1
{
	
	char temp; // буфер сообщений
	if ((USART1->SR & USART_SR_RXNE) > 0) // проверяем регистр прибития
		{
			temp  = USART1->DR;
			if (temp == 0x0A)
				{
					if(massive[strlen(massive)-1] ==0x0D)
						{
							flagEnd = 1;				
							return;

						}
				}
					massive[strlen(massive)] = temp;
		
		}
}
void doCommand(void)
{
	resault = strcmp(massive,"LED1 ON\r");
	if( resault == 0)
		{
			Led1ON = 1;
			Led1BLINK = 0;
		}
	if (strcmp(massive,"LED1 OFF\r") == 0)
		{
			Led1ON = 0;
			Led1BLINK = 0;
		}
	if (strcmp(massive,"LED2 ON\r") == 0)
		{
			Led2ON = 1;
			Led2BLINK = 0;
		}
	if (strcmp(massive,"LED2 OFF\r") == 0)
		{
			Led2ON = 0;
			Led2BLINK = 0;
		}
	if (strcmp(massive,"LED1 BLINK\r") == 0)
	{
		Led1ON = 0;
		Led1BLINK = 1;
	}
	if (strcmp(massive,"LED2 BLINK\r") == 0)
	{
		Led2ON = 0;
		Led2BLINK = 1;
	}
	memset(massive,0,sizeof(massive));
}