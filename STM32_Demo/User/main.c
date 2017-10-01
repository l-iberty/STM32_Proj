#include "stm32f10x.h"
#include "bsp_usart.h"

int main()
{
    USART_Config();
    
    printf("STM32 Test!");
    
    while(1);
}
