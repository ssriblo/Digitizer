#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "circular_buffer.h"



/**
  * @brief  This function prepare COBS buffers to send.
  * 	There are two buffers, one used for data acquisition and another for UART sending.
  * 	Then those buffers switched
  * @param  avrg_data - two bytes word
  * @retval None
  */
void cobs_doing(uint16_t avrg_data);


/**
  * @brief This function send out prepared COBS buffer via RS485
  * @param  None
  * @retval None
  */
void rs485_send(void);


/**
  * @brief  This function perform COBS algorithm
  * @param  input byte
  * @retval None
  */
void cobs_encode(uint8_t byte);

/**
  * @brief  This function for COBS buffer reset. Must called at the every new buffer processing
  * @param  dst_buff - pointer to source buffer
  * @retval None
  */
void cobs_reset(uint8_t* dst_buff);

/**
  * @brief  This function for COBS initialization at the start
  * @param  None
  * @retval None
  */
void cobs_init(void);

