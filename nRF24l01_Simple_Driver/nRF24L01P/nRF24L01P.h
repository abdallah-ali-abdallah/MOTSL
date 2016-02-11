#define ADR_WIDTH 5

#define RX_PLOAD_WIDTH 20
#define TX_PLOAD_WIDTH 20
#define Buffer_Size 32

//Define RF power value
#define P0dBm 0
#define Pm6dBm 1
#define Pm12dBm 2
#define Pm18dBm 3

//#define RF rate
#define R2mbps 0
#define R1mbps 1
#define R250kbps 3


unsigned char nRF24L01_Config(unsigned char freq,unsigned char power,unsigned char rate);
void RX_Mode(void);
void TX_Mode(void);
void nRF24L01_TxPacket(unsigned char * tx_buf);
uint8_t nRF24L01_RxPacket(unsigned char *rx_buf);
void nRF24L01_HW_Init(void);
void nRF24L01_Set_TX_Address(unsigned char A,unsigned char B,unsigned char C,unsigned char D,unsigned char E);
void nRF24L01_Set_RX_Address(unsigned char A,unsigned char B,unsigned char C,unsigned char D,unsigned char E);

