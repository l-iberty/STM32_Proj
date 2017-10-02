#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_spi_flash.h"

typedef enum { FAILED = 0, SUCCEEDED = !FAILED} TestStatus;

#define BufferSize              SPI_FLASH_PageSize

#define FLASH_WriteAddress      0x00000
#define FLASH_ReadAddress       FLASH_WriteAddress
#define FLASH_SectorToErase     FLASH_WriteAddress

     

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[BufferSize] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);

// main entry
int main(void)
{ 	
	/* 配置串口为：115200 8-N-1 */
	USART_Config();
	printf("\r\n 8M serial flash(W25Q64) test \r\n");
    
	/* 8M serial flash W25Q64 初始化 */
	SPI_FLASH_Init();
	
	/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	Delay( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X, Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n serial flash W25Q64 detected!\r\n");
        
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
		SPI_FLASH_SectorErase(FLASH_SectorToErase);
        
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		printf("\r\n data written: %s \r\n", Tx_Buffer);
		
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
        printf("\r\n data read: %s \r\n", Rx_Buffer);
		
        float x = -128.75;
        float y;
        SPI_FLASH_WriteFloat(x);
        y = SPI_FLASH_ReadFloat();
        printf("\r\n x = %f, y = %f", x, y);
    }
    else // if (FlashID != sFLASH_ID)
	{ 
		printf("\r\n cannot detect W25Q64 ID!\n\r");
	}
	
	while(1);  
}

/**
 * @breif   比较两个缓冲区内容是否相等
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }
        pBuffer1++;
        pBuffer2++;
    }
    
    return SUCCEEDED;
}

void Delay(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
