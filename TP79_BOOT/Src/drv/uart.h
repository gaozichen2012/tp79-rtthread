#ifndef __UART_H_
#define __UART_H_


#define UART_RXBUF_LEN_MAX  2048

#define UART3_RX_PORT	GPIOA
#define UART3_RX_PIN		GPIO_Pin_0
#define UART3_TX_PORT	GPIOA
#define UART3_TX_PIN		GPIO_Pin_15

#define UART3_RXPIN_READ()  	 GPIO_ReadInputDataBit(UART3_RX_PORT, UART3_RX_PIN)

typedef enum
{
	UART_STATUS_NORMAL,
	UART_STATUS_RECVING,
	UART_STATUS_RECVED,
	UART_STATUS_SENDING,
}uart_status_t;

typedef struct
{
	uint8_t recv_buf[UART_RXBUF_LEN_MAX];//Ω” ’buf
	uint16_t write_p;
	uint16_t read_p;	
  uart_status_t status;
}device_uart_t,*device_uart_pt;

void uart2_config(void);

void uart1_send_string(const uint8_t *str, uint16_t strlen);
void uart2_send_string(const uint8_t *str, uint16_t strlen);
void uart3_send_string(const uint8_t *str, uint16_t strlen);
void uart3_send_char(const uint8_t str);
#endif
