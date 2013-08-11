/*
 *	Header file with GUI ID's, error constants and program version
 */

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <string>
using namespace std;

//------------------------------------------------------------------------------
//Version
#define VERSION "1.0"


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
*/
#define ID_LOGGER		1
#define ID_PAR_ODD		2
#define ID_PAR_EVEN		3
#define ID_PAR_NONE		4
#define ID_GB_PARITY	5
#define ID_GB_TRANSFER	6
#define ID_PRO_XON_OFF	7
#define ID_PRO_DTR_DSR	8
#define ID_PRO_CTS_RTS	9
#define ID_CB_COM_PORT  10
#define ID_CB_COM_BAUD  11
#define ID_MOD_SINGLE	12
#define ID_MOD_DOUBLE	13
#define ID_MOD_MS		14
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

#define ID_BUT			100



//------------------------------------------------------------------------------
//ERROR CODES

#define ERROR_PORT_OPEN		-3
#define ERROR_CLOSE_PORT	-4
#define ERROR_GET_DCB		-5		
#define ERROR_BAUDRATE		-6
#define ERROR_SET_TIMEOUTS	-7
#define ERROR_LOG			-8
#define ERROR_INI			-9
#define ERROR_FILE_NOT_OPEN -13
#define ERROR_EMPTY_FILE	-14
#define ERROR_CFG_HEADER	-15
#define ERROR_SYNTAX		-16
#define ERROR_PARSE			-17

#define DEFAULT_VALUE		-100


//------------------------------------------------------------------------------
//Variables
	struct comPort
	{
		string sMasterPort;
		string sSlavePort;
		int iTransfer;
		int iBaud;
		int iTestMode;
		int iParity;
		int iProtocol;
		int iStopbits;
		
		//constructor
		comPort(void)
		{
			sMasterPort	= "";
			sSlavePort	= "";
			iBaud		= DEFAULT_VALUE;
			iTestMode	= DEFAULT_VALUE;
			iParity		= DEFAULT_VALUE;
			iProtocol	= DEFAULT_VALUE;
			iStopbits	= DEFAULT_VALUE;
			iTransfer	= DEFAULT_VALUE;
		}
	};//struct


#endif