#include <winscard.h>

#ifdef __cplusplus
extern "C" {
#endif

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ALCORSLE4442API_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ALCORSLE4442API_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ALCORSLE4442API_EXPORTS
#define ALCORSLE4442API_API __declspec(dllexport)
#else
#define ALCORSLE4442API_API __declspec(dllimport)
#endif

/* Alcor vendor command */
#define CMD_SWITCH_CARD_MODE					0x50
#define CMD_ALCOR_HEADER_LENGTH					0x08
#define CMD_ALCOR_OP_CODE						0x40
// SLE4442
#define CMD_SLE4442_CARD_HEADER_LENGTH			0x03
#define	CMD_SLE4442_CARD_COMMAND				0x80
#define	CMD_SLE4442_CARD_BREAK					0x81
//Code for SLE4432/42
#define CODE_READ_MAIN_MEMORY					0x30
#define CODE_UPDATE_MAIN_MEMORY					0x38
#define CODE_READ_PROTECTION_MEMORY				0x34
#define CODE_WRITE_PROTECTION_MEMORY			0x3C
#define CODE_READ_SECURITY_MEMORY				0x31
#define CODE_UPDATE_SECURITY_MEMORY				0x39
#define CODE_COMPARE_VERIFICATION_DATA			0x33

#define ERR_SLE4442_CARD_LOCKED					0x267D //Error value of Card Locked
	
#define	IOCTL_CCID_ESCAPE		  		   CTL_CODE(FILE_DEVICE_SMARTCARD,  \
	3500,\
	METHOD_BUFFERED,  \
	FILE_ANY_ACCESS)
	
	
	ALCORSLE4442API_API LONG SLE4442Cmd_ReadMainMemory(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bAddress,
		IN	UCHAR	bReadLen,
		OUT	LPVOID	pReadData,
		OUT	ULONG	*pbReturnLen
		);
	
	ALCORSLE4442API_API LONG SLE4442Cmd_UpdateMainMemory(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bAddress,
		IN	UCHAR	bData
		);
	
	ALCORSLE4442API_API LONG SLE4442Cmd_ReadProtectionMemory(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bReadLen,
		OUT	LPVOID	pReadData,
		OUT	ULONG	*pbReturnLen
		);
	
	ALCORSLE4442API_API LONG SLE4442Cmd_WriteProtectionMemory(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bAddress,
		IN	UCHAR	bData
		);
	
	ALCORSLE4442API_API LONG SLE4442Cmd_ReadSecurityMemory(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bReadLen,
		OUT	LPVOID	pReadData,
		OUT	ULONG	*pbReturnLen
		);
	
	ALCORSLE4442API_API LONG SLE4442Cmd_UpdateSecurityMemory(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bAddress,
		IN	UCHAR	bData
		);
	
	ALCORSLE4442API_API LONG SLE4442Cmd_CompareVerificationData(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bAddress,
		IN	UCHAR	bData
		);

	ALCORSLE4442API_API LONG  SLE4442Cmd_Verify(		
								IN	LONG	lngCard,
								IN	UCHAR	bSlotNum,
								IN	ULONG	lngPinLen,
								IN	UCHAR	*pPinData  
								) ;

	
	ALCORSLE4442API_API LONG  Alcor_SwitchCardMode(
		IN	LONG	lngCard,
		IN	UCHAR	bSlotNum,
		IN	UCHAR	bCardMode
		);

#ifdef __cplusplus
}
#endif