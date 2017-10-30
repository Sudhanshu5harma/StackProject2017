/*
 * UART_C_file.h
 *
 * http://www.electronicwings.com
 */

#ifndef __UART_H_file_H__
#define __UART_H_file_H__

#include<reg51.h>

void UART_Init();
void UART_TxChar(char);
char UART_RxChar();
void UART_SendString(char *);

#endif