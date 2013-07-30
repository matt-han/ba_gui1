#ifndef _CONSTANTS_H
#define _CONSTANTS_H



#define VERSION "1.0"


//GUI Elements
//===================
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

#define ID_BUT			100




//ERROR CODES
//===================
#define ERR_PORT_OPEN	-3
#define ERR_GET_DCB		-4
#define ERROR_FILE_NOT_OPEN -13
#define ERROR_EMPTY_FILE -14
#define ERROR_CFG_HEADER -15
#define ERROR_SYNTAX -16




#endif