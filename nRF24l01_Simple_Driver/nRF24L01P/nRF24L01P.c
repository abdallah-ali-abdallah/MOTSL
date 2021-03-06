#include "stm32f10x_RCC.h"
#include "stm32f10x_GPIO.h"
#include "stm32f10x_SPI.h"

#include "nRF24l01P.h"

//Define the commands for operate the nRF24L01P
#define READ_nRF_REG        0x00  	// Command for read register
#define WRITE_nRF_REG       0x20 	// Command for read(WRITE) register
#define RD_RX_PLOAD     0x61  	// Command for read Rx payload
#define WR_TX_PLOAD     0xA0  	// Command for write Tx payload
#define FLUSH_TX        0xE1 	// Command for flush Tx FIFO
#define FLUSH_RX        0xE2  	// Command for flush Rx FIFO
#define REUSE_TX_PL     0xE3  	// Command for reuse Tx payload
#define NOP             0xFF  	// Reserve

//Define the register address for nRF24L01P
#define CONFIG          0x00  //  Configurate the status of transceiver, mode of CRC and the replay of transceiver status
#define EN_AA           0x01  //  Enable the atuo-ack in all channels
#define EN_RXADDR       0x02  //  Enable Rx Address
#define SETUP_AW        0x03  // Configurate the address width
#define SETUP_RETR      0x04  //  setup the retransmit
#define RF_CH           0x05  // Configurate the RF frequency
#define RF_SETUP        0x06  // Setup the rate of data, and transmit power
#define NRFRegSTATUS    0x07  //
#define OBSERVE_TX      0x08  //
#define CD              0x09  //    //Carrier detect
#define RX_ADDR_P0      0x0A  //  Receive address of channel 0
#define RX_ADDR_P1      0x0B  // Receive address of channel 1
#define RX_ADDR_P2      0x0C  // Receive address of channel 2
#define RX_ADDR_P3      0x0D  // Receive address of channel 3
#define RX_ADDR_P4      0x0E  // Receive address of channel 4
#define RX_ADDR_P5      0x0F  // Receive address of channel 5
#define TX_ADDR         0x10  //       Transmit address
#define RX_PW_P0        0x11  //  Size of receive data in channel 0
#define RX_PW_P1        0x12  //  Size of receive data in channel 1
#define RX_PW_P2        0x13  //  Size of receive data in channel 2
#define RX_PW_P3        0x14  //  Size of receive data in channel 3
#define RX_PW_P4        0x15  // Size of receive data in channel 4
#define RX_PW_P5        0x16  //  Size of receive data in channel 5
#define FIFO_STATUS     0x17  // FIFO Status
///**************************************************************************************

//define the private constants in this library
//#define TX_ADR_WIDTH 5
//#define RX_ADR_WIDTH 5

unsigned char TxBuf[Buffer_Size] = {0};
unsigned char RxBuf[Buffer_Size] = {0};

unsigned char nRF24L01_Freq = 0;
unsigned char nRF24L01_power_rate = 0;

//define the initial Address
unsigned char  TX_ADDRESS[ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};
unsigned char  RX_ADDRESS[ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};

//Define the layer1:HW operation
unsigned char nRF24L01_SPI_Send_Byte(unsigned char dat);
void nRF24L01_HW_Init(void);
void nRF24L01_SPI_NSS_L(void);
void nRF24L01_SPI_NSS_H(void);

//Define the layer2:Reg operation
unsigned char SPI_WR_Reg(unsigned char reg, unsigned char value);
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len);
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len);
unsigned char SPI_RD_Reg(unsigned char reg);

//Define the layer3:application operation
/****************************************

All the functions is in "nRF24l01P.h"

****************************************/

//Define the other function
void nRF24L01_Delay_us(unsigned long n);

//Define the layer3 functions

void nRF24L01_Set_TX_Address(	unsigned char A,
								unsigned char B,
								unsigned char C,
								unsigned char D,
								unsigned char E)
{
	TX_ADDRESS[0] = A;
	TX_ADDRESS[1] = B;
	TX_ADDRESS[2] = C;
	TX_ADDRESS[3] = D;
	TX_ADDRESS[4] = E;
}
void nRF24L01_Set_RX_Address(	unsigned char A,
								unsigned char B,
								unsigned char C,
								unsigned char D,
								unsigned char E)
{
	RX_ADDRESS[0] = A;
	RX_ADDRESS[1] = B;
	RX_ADDRESS[2] = C;
	RX_ADDRESS[3] = D;
	RX_ADDRESS[4] = E;
}

unsigned char nRF24L01_Config(unsigned char freq, unsigned char power, unsigned char Rate)
{
	nRF24L01_Freq = 0;
	nRF24L01_power_rate = 0;

	if((freq>125)&&(freq<0))
		return 0;
	else
		nRF24L01_Freq = freq;

	if (P0dBm == power)
		nRF24L01_power_rate|=0x06;
	else if (Pm6dBm == power)
		nRF24L01_power_rate|=0x04;
	else if (Pm12dBm == power)
		nRF24L01_power_rate|=0x02;
	else if (Pm18dBm == power)
		nRF24L01_power_rate|=0x00;
	else
		return 0;

	if (R2mbps == Rate)
		{nRF24L01_power_rate|=0x08;}
	else if (Rate == R1mbps)
		{nRF24L01_power_rate|=0x00;}
	else if (Rate == R250kbps)
		nRF24L01_power_rate|=0x20;
	else
		return 0;

	return 1;

}

void RX_Mode(void)
{unsigned char buf[5]={0};


	SPI_Read_Buf(TX_ADDR, buf, ADR_WIDTH);

	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH);

	SPI_WR_Reg(WRITE_nRF_REG + EN_AA, 0);
	SPI_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01);
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x1a);
	SPI_WR_Reg(WRITE_nRF_REG + RF_CH,nRF24L01_Freq);
	SPI_WR_Reg(WRITE_nRF_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	SPI_WR_Reg(WRITE_nRF_REG + RF_SETUP, nRF24L01_power_rate);

	SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x03);

	nRF24L01_Delay_us(200);
}

void TX_Mode(void)
{


	SPI_Write_Buf(WRITE_nRF_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);
	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH);

	SPI_WR_Reg(WRITE_nRF_REG + EN_AA, 0);
	SPI_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01);
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x1a);
	SPI_WR_Reg(WRITE_nRF_REG + RF_CH,nRF24L01_Freq);
	SPI_WR_Reg(WRITE_nRF_REG + RF_SETUP,  nRF24L01_power_rate);
	SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x02);

}

void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE_high(); //Set CE pin
	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, TX_ADDRESS, ADR_WIDTH);
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
	CE_low(); //Reset CE pin

}

void CE_high()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}

void CE_low()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

uint8_t nRF24L01_RxPacket(unsigned char *rx_buf){
	unsigned char flag=0;
	unsigned char status;

	status=SPI_RD_Reg(NRFRegSTATUS);

	if(status & 0x40)
	{

		 SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
		 flag =1;
	}
	SPI_WR_Reg(WRITE_nRF_REG+NRFRegSTATUS, status);
	return flag;
}

//Define the layer2 functions
unsigned char SPI_RD_Reg(unsigned char reg)
{
	unsigned char reg_val;

	nRF24L01_SPI_NSS_L();                // CSN low, initialize SPI communication...
	nRF24L01_SPI_Send_Byte(reg);            // Select register to read from..
	reg_val = nRF24L01_SPI_Send_Byte(0);    // ..then read register value
	nRF24L01_SPI_NSS_H();                // CSN high, terminate SPI communication

	return(reg_val);        // return register value
}

unsigned char SPI_WR_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;

	nRF24L01_SPI_NSS_L();                  // CSN low, init SPI transaction
	status = nRF24L01_SPI_Send_Byte(reg);// select register
	nRF24L01_SPI_Send_Byte(value);             // ..and write value to it..
	nRF24L01_SPI_NSS_H();                   // CSN high again

	return(status);            // return nRF24L01 status unsigned char
}

unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	nRF24L01_SPI_NSS_L();                    		// Set CSN low, init SPI tranaction
	status = nRF24L01_SPI_Send_Byte(reg);       		// Select register to write to and read status unsigned char

  for(i=0;i<Len;i++)
  {
     pBuf[i] = nRF24L01_SPI_Send_Byte(0);
  }

	nRF24L01_SPI_NSS_H();

	return(status);                    // return nRF24L01 status unsigned char
}

unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	nRF24L01_SPI_NSS_L();
	status = nRF24L01_SPI_Send_Byte(reg);
	for(i=0; i<Len; i++) //
	{
		nRF24L01_SPI_Send_Byte(*pBuf);
		 pBuf ++;
	}
	nRF24L01_SPI_NSS_H();
	return(status);
}



//Define the layer1 functions
unsigned char nRF24L01_SPI_Send_Byte(unsigned char dat)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI2, dat);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

void nRF24L01_SPI_NSS_H(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void nRF24L01_SPI_NSS_L(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

void nRF24L01_HW_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	/* Configure SPI2 pins: NSS, SCK, MISO and MOSI */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI1 configuration */
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_High=Ä£Ê½3£¬Ê±ÖÓ¿ÕÏÐÎª¸ß //SPI_CPOL_Low=Ä£Ê½0£¬Ê±ÖÓ¿ÕÏÐÎªµÍ
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_2Edge;//SPI_CPHA_1Edge, SPI_CPHA_2Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Soft;//SPI_NSS_Hard
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//SPI_BaudRatePrescaler_2=18M;//SPI_BaudRatePrescaler_4=9MHz
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//Êý¾Ý´Ó¸ßÎ»¿ªÊ¼·¢ËÍ
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI2, &SPI_InitStructure);

	//Config the NSS pin
  	SPI_SSOutputCmd(SPI2, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable SPI2  */
  	SPI_Cmd(SPI2, ENABLE);

	 //CE
  	 // GPIO configuration : OUTPUT : pin_11 : PORTA :
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef cePin;
	cePin.GPIO_Pin = GPIO_Pin_11;
	cePin.GPIO_Speed = GPIO_Speed_50MHz;
	cePin.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &cePin);
}

void nRF24L01_Delay_us(unsigned long n)
{
	unsigned long i;

	while(n--)
	{
 	   i=2;
 	   while(i--);
  }
}

