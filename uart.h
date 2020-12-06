//created by Will Foerster
//date: December 1, 2020

#ifndef UART_H_
#define UART_H_



void uart_init(void);
void confg_uart_wired(void);
void uart_cs(void);
void uart_get(void);
int EUSCIA2_IRQHandler(int target);

#endif
