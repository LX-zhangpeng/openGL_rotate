#include <stdio.h>
#include <iostream>
#include <windows.h>

using namespace std;

HANDLE hComm;
OVERLAPPED m_ov;
COMSTAT comstat;

//打开一个串口
bool openport(LPCTSTR  portname)
{
	hComm = CreateFile(portname,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hComm == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}

bool setupdcb(int rate_arg)
{
	DCB dcb;
	int rate = rate_arg;
	memset(&dcb, 0, sizeof(dcb));
	if (!GetCommState(hComm, &dcb))//获取当前DCB配置
	{
		return FALSE;
	}

	/* -------------------------------------------------------------------- */
	// set DCB to configure the serial port
	dcb.DCBlength = sizeof(dcb);
	/* ---------- Serial Port Config ------- */
	dcb.BaudRate = rate;
	dcb.Parity = NOPARITY;
	dcb.fParity = 0;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = 0;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	/* ----------------- misc parameters ----- */
	dcb.fErrorChar = 0;
	dcb.fBinary = 1;
	dcb.fNull = 0;
	dcb.fAbortOnError = 0;
	dcb.wReserved = 0;
	dcb.XonLim = 2;
	dcb.XoffLim = 4;
	dcb.XonChar = 0x13;
	dcb.XoffChar = 0x19;
	dcb.EvtChar = 0;

	// set DCB
	if (!SetCommState(hComm, &dcb))
	{
		return false;
	}
	else
		return true;
}

//从串口获取信息
void ReceiveChar(int *x,int *y,int *z)
{
	char temp_buf = 0;
	DWORD readlen = 0;
	char recive_buf[100];
	int flag = 0;
	int i = 0;

	while(ReadFile(hComm, &temp_buf, 1, &readlen, NULL))
	{
		if(temp_buf == 's')
		{
			flag = 1;
		}
		else if((flag == 1) && (temp_buf == 'e'))
		{
			flag = 0;
			i = 0;
			*x = (recive_buf[2] - 0x30) * 1000 + (recive_buf[3] - 0x30)*100 + (recive_buf[4] - 0x30)*10 + (recive_buf[5] - 0x30);
			*y = (recive_buf[8] - 0x30) * 1000 + (recive_buf[9] - 0x30)*100 + (recive_buf[10] - 0x30)*10 + (recive_buf[11] - 0x30);
			*z = (recive_buf[14] - 0x30) * 1000 + (recive_buf[15] - 0x30)*100 + (recive_buf[16] - 0x30)*10 + (recive_buf[17] - 0x30);


			*x = *x / 1000 ? -1*(*x % 1000) : *x;
			*y = *y / 1000 ? -1*(*y % 1000) : *y;
			*z = *z / 1000 ? -1*(*z % 1000) : *z;

			//printf("hhhhhh %d  %d  %d\n", *x, *y, *z);
			break;
		}
		else
		{
			recive_buf[i] = temp_buf;
			i++;
		}
	}
}