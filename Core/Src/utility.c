#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "main.h"
#include "circular_buffer.h"
#include "utility.h"

/*****************************************************************************/
// **** GLOBALS VARIABLES: *******/
uint8_t cobs_buff_1[COBS_BUFF_SIZE+2] = {0}; // NOTE: fill all elements to ZERO
uint8_t cobs_buff_2[COBS_BUFF_SIZE+2] = {0}; // NOTE: fill all elements to ZERO
uint32_t switch_status = 0; // to point one of the COBS buffers
uint8_t* p_cobs_buff_acquisition = cobs_buff_1;		// will be switched later
uint8_t* p_cobs_buff_uart = cobs_buff_2;					// will be switched later
uint32_t cobs_counter = 0;
//uint8_t test_data[] = {
//		01, 00, 03, 04, 05, 06, 00,  8,
//		 9, 10, 11, 12, 13, 14, 15, 16,
//		17, 18, 19, 20, 21, 22, 23, 24,
//		25, 26, 27, 28, 29, 30, 31, 32,
//		33, 34, 35, 36, 37, 38, 39, 40,
//		41, 42, 43, 44, 45, 46, 47, 48
//};
/***********COBS Vars *********************************************************/
uint8_t* dst = cobs_buff_1 + 1;
uint8_t* code_ptr = cobs_buff_1;
uint8_t code = 0x01;
/*****************************************************************************/
extern UART_HandleTypeDef huart1;

/*****************************************************************************/
void cobs_doing(uint16_t avrg_data){
//	static uint32_t i = 0;
	cobs_encode(avrg_data & 0xFF);
	cobs_encode((avrg_data >> 8) & 0xFF);
//	cobs_encode(test_data[i++]);
//	cobs_encode(test_data[i++]);
	cobs_counter = cobs_counter + 2; // Every COBS buff contains COBS_BUFF_SIZE/2 WORDS
	if(COBS_BUFF_SIZE == cobs_counter){
		cobs_counter = 0;
		*code_ptr = (code);

		// Here need to add ZERO at the END !!!!!!!!!!!

		if(0 == switch_status){
			p_cobs_buff_acquisition = cobs_buff_2;
			p_cobs_buff_uart = cobs_buff_1;
			switch_status = 1;
		}else{
			p_cobs_buff_acquisition = cobs_buff_1;
			p_cobs_buff_uart = cobs_buff_2;
			switch_status = 0;
		}
		cobs_reset(p_cobs_buff_acquisition);
		rs485_send();
	} // if(COBS_BUFF_SIZE ==

}

/*****************************************************************************/
void rs485_send(void){
  GPIOA->BSRR = RS485_DERE__Pin; // Set High
	HAL_UART_Transmit_IT(&huart1, p_cobs_buff_uart, COBS_BUFF_SIZE+2);
  GPIOA->BSRR = RS485_DERE__Pin <<16u; // Set LOW

}
/*****************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
          if(huart == &huart1)
          {
                  // можно установить какой-то флаг, сообщающий об окончании отправки
          }
}

/*****************************************************************************/
void cobs_reset(uint8_t* dst_buff){
	dst = dst_buff;
	code_ptr = dst++;
	code = 0x01;
}

void cobs_encode(uint8_t byte){
	/* dst, code and code_ptr must be sets to start at code above  */
		if (byte == 0){
			*code_ptr = (code);
 			code_ptr = dst++;
 			code = 0x01;
		}
		else{
			*dst++ = byte;
			code++;
			//if(code == 0xFF) ... >>> not used because packet < 255
		}
}

// For reference, not used:
#define FinishBlock(X) (*code_ptr = (X), \
 code_ptr = dst++, \
 code = 0x01 )

void StuffData(const unsigned char *ptr,
unsigned long length, unsigned char *dst)
{
	const unsigned char *end = ptr + length;
	unsigned char *code_ptr = dst++;
	unsigned char code = 0x01;
	while (ptr < end)
	{
		if (*ptr == 0) FinishBlock(code);
		else
		{
			*dst++ = *ptr;
			code++;
			if (code == 0xFF) FinishBlock(code);
		}
		ptr++;
	}
	FinishBlock(code);
}


/*****************************************************************************/

