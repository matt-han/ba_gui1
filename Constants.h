/*
 *	Header file with GUI ID's, error constants and program version
 */

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <string>
#include <vector>


using namespace std;

//------------------------------------------------------------------------------
//Version
#define VERSION "0.70"


//------------------------------------------------------------------------------
//GUI Elements

/*ABREVIATIONS
*	ID	-> Identification
*	PAR	-> Parity
*	GB	-> GroupBox
*	PRO	-> Protocol for transfer
*	CB	-> ComboBox
*	MOD	-> Mode
*	SB	-> StopBits
*	LB	-> Label
*	BT	-> Button
*	DB	-> Databits
*/
#define ID_LOGGER		1
#define ID_PAR_ODD		2
#define ID_PAR_EVEN		3
#define ID_PAR_NONE		4
#define ID_GB_PARITY	5
#define ID_GB_TRANSFER	6
#define ID_PRO_XON_OFF	7
#define ID_PRO_HARDWARE	8
#define ID_PRO_NONE		9
#define ID_CB_COM_PORT  10
#define ID_CB_COM_BAUD  11
#define ID_MOD_SINGLE	12
#define ID_MOD_DOUBLE	13
#define ID_MOD_MASTER	14
#define ID_SB_ONE		15
#define ID_SB_ONE5		16
#define ID_SB_TWO		17
#define ID_LB_LOAD		18
#define ID_BT_LOAD		19
#define ID_BT_START		20
#define ID_BT_CLOSE		21
#define ID_GB_STOPBITS	22
#define ID_GB_TESTMODE	23
#define ID_TM_AUTO		24
#define ID_TM_WOBB		25
#define ID_TM_FIXED		26
#define ID_LB_TEXT		27
#define ID_BT_TEXT		28
#define ID_CB_SLV_PORT  29
#define ID_MOD_SLAVE	30
#define ID_LB_SLV		31
#define ID_LB_MIN		32
#define ID_LB_MAX		33
#define ID_LB_BAUD		34
#define ID_CB_MAX		35
#define ID_CB_BAUD_MAX	36
#define ID_BT_SAVE		37
#define ID_BT_LOADINI	38
#define ID_LB_REPEATER	39
#define ID_DB_7			40
#define ID_DB_8			41
#define ID_GB_DATABITS	42
#define ID_BT_STOP		43
#define ID_BT_HELP		44
#define ID_GB_PROTOCOL	45

#define ID_THREAD_MAIN	98
#define ID_THREAD_SUB	99

#define ID_BUT			100



//------------------------------------------------------------------------------
//ERROR CODES
#define ERROR_CMD_SYNTAX	-1
#define ERROR_CREATE_GUI	-2
#define ERROR_PORT_OPEN		-3
#define ERROR_CLOSE_PORT	-4
#define ERROR_GET_DCB		-5		
#define ERROR_BAUDRATE		-6
#define ERROR_SET_TIMEOUTS	-7
#define ERROR_LOG			-8
#define ERROR_INI			-9
#define ERROR_SET_DCB		-10
#define ERROR_INPUT			-11
#define ERROR_NO_FILE		-12
#define ERROR_FILE_NOT_OPEN -13
#define ERROR_EMPTY_FILE	-14
#define ERROR_BAUD_MINMAX	-15
#define ERROR_MISSING_PAR	-16
#define ERROR_PARSE			-17
#define ERROR_READ_PORT		-18
#define ERROR_WRITE_PORT	-19
#define ERROR_CMP_STR		-20
#define ERROR_WAIT_SLAVE	-21
#define ERROR_WAIT_MASTER	-22
//#define						-23
//#define 						-24
//#define						-25
//#define						-26


#define ERROR_TRANSMITION "ERROR_TRANSMITION"
#define DEFAULT_VALUE		-100
#define ERROR_TODO			-666


//------------------------------------------------------------------------------
//Baud rates
//can NOT trust baud rates defined in WinBase.h

#define B_075			 75
#define B_110			 110
#define B_134_5			 134.5
#define B_150			 150
#define B_300			 300
#define B_600			 600
#define B_1200			 1200
#define B_1800			 1800
#define B_2400			 2400
#define B_4800			 4800
#define B_7200			 7200
#define B_9600			 9600
#define B_14400			 14400
#define B_19200			 19200
#define B_38400			 38400
#define B_56K			 56000
#define B_57600			 57600
#define B_115200		 115200
#define B_128K			 128000
#define B_256K			 256000



//------------------------------------------------------------------------------
//Variables
	struct TestStruct
	{
		string sMasterPort;
		string sSlavePort;
		string sTextToTransfer;
		string sFilePath;
		int iTransfer;
		int iBaud;
		int iBaudrateMax;
		int iTestMode;
		int iParity;
		int iProtocol;
		int iStopbits;
		int iDatabits;
		int iTransTextMode;
		int iRepeater;
		bool bLoggerState;

		vector<string> svBaudrates;

		//constructor
		TestStruct(void)
		{
			sMasterPort		= "";
			sSlavePort		= "";
			sTextToTransfer = "";
			sFilePath		= "";
			iBaud			= DEFAULT_VALUE;
			iBaudrateMax	= DEFAULT_VALUE;
			iTestMode		= DEFAULT_VALUE;
			iParity			= DEFAULT_VALUE;
			iProtocol		= DEFAULT_VALUE;
			iStopbits		= DEFAULT_VALUE;
			iDatabits		= DEFAULT_VALUE;
			iTransfer		= DEFAULT_VALUE;
			iTransTextMode  = DEFAULT_VALUE;
			iRepeater		= DEFAULT_VALUE;
			bLoggerState	= true;
		}
	};//struct


#endif