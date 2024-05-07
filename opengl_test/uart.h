#pragma once
#ifndef __UART_H_
#define __UART_H_

#include <windows.h>


bool openport(LPCTSTR  portname);
bool setupdcb(int rate_arg);
void ReceiveChar(int *x, int *y, int *z);

#endif