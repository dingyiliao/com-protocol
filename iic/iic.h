#ifndef __IIC_H
#define __IIC_H

// must to include header file corresponding to the microcontroller, such as #include "reg52.h"

#define sda  // need to bind sda line to this macro definition
#define scl  // need to bind scl line to this macro definition

/// @brief generate a start condition
///
/// @param none
/// @return none
void iic_start(void);
/// @brief generate a stop condition
///
/// @param none
/// @return none
void iic_stop(void);
/// @brief write a byte to the slave
///
/// @param[in] byt - a byte that wait to be sent
/// @return none
void iic_write(unsigned char byt);
/// @brief wait for a acknowledge signal from the slave
///
/// @param none
/// @return ack - acknowledge from the slave
bit iic_wait_ack(void);
/// @brief read a byte from the slave
///
/// @param none
/// @return byt - a byte from the slave
unsigned char iic_read(void);
/// @brief send a acknowledge to the slave
///
/// @param[in] ack - the acknowledge signal that need to be sent to the slave
/// @return none
void iic_send_ack(bit ack);


#endif
