/*=========================================================================================
'   Author :  Rian.Liu
'
'   Module :  ModWinscard.cs
'
'   Company:  Alcor Micro, Corp..
'
'   Date   :  Jan 10, 2013
'
'=========================================================================================*/

using System;
using System.Text;
using System.Runtime.InteropServices;

	
	public class ModWinsCard
	{
		public const int SCARD_S_SUCCESS = 0;
		public const int SCARD_ATR_LENGTH = 33;

		/*===============================================================
		' CONTEXT SCOPE
		===============================================================	*/
		public const int SCARD_SCOPE_USER = 0;
		/*===============================================================
		' The context is a user context, and any database operations 
		'  are performed within the domain of the user.
		'===============================================================*/
		public const  int SCARD_SCOPE_TERMINAL = 1;
		/*===============================================================
		' The context is that of the current terminal, and any database 
		'operations are performed within the domain of that terminal.  
		'(The calling application must have appropriate access permissions 
		'for any database actions.)
		'===============================================================*/
		
		public const int  SCARD_SCOPE_SYSTEM = 2;
		/*===============================================================
		' The context is the system context, and any database operations 
		' are performed within the domain of the system.  (The calling
		' application must have appropriate access permissions for any 
		' database actions.)
		'===============================================================*/
		/*=============================================================== 
		' Context Scope
		'===============================================================*/ 
		public const int  SCARD_STATE_UNAWARE = 0x00;
		/*===============================================================
		' The application is unaware of the current state, and would like 
		' to know. The use of this value results in an immediate return
		' from state transition monitoring services. This is represented
		' by all bits set to zero.
		'===============================================================*/
		public const int SCARD_STATE_IGNORE = 0x01;
		/*===============================================================
		' The application requested that this reader be ignored. No other
		' bits will be set.
		'===============================================================*/
		
		public const int SCARD_STATE_CHANGED = 0x02;
		/*===============================================================
		' This implies that there is a difference between the state 
		' believed by the application, and the state known by the Service
		' Manager.When this bit is set, the application may assume a
		' significant state change has occurred on this reader.
		'===============================================================*/

		public const int SCARD_STATE_UNKNOWN = 0x04;
		/*===============================================================
		' This implies that the given reader name is not recognized by
		' the Service Manager. If this bit is set, then SCARD_STATE_CHANGED
		' and SCARD_STATE_IGNORE will also be set.
		'===============================================================*/
		public const int SCARD_STATE_UNAVAILABLE = 0x08;
		/*===============================================================
		' This implies that the actual state of this reader is not
		' available. If this bit is set, then all the following bits are
		' clear.
		'===============================================================*/
		public const int SCARD_STATE_EMPTY = 0x10;
		/*===============================================================
		'  This implies that there is not card in the reader.  If this bit
		'  is set, all the following bits will be clear.
		 ===============================================================*/
		public const int SCARD_STATE_PRESENT = 0x20;
		/*===============================================================
		'  This implies that there is a card in the reader.
		 ===============================================================*/
		public const int SCARD_STATE_ATRMATCH = 0x40;
		/*===============================================================
		'  This implies that there is a card in the reader with an ATR
		'  matching one of the target cards. If this bit is set,
		'  SCARD_STATE_PRESENT will also be set.  This bit is only returned
		'  on the SCardLocateCard() service.
		 ===============================================================*/
		public const int SCARD_STATE_EXCLUSIVE = 0x80;
		/*===============================================================
		'  This implies that the card in the reader is allocated for 
		'  exclusive use by another application. If this bit is set,
		'  SCARD_STATE_PRESENT will also be set.
		 ===============================================================*/
		public const int SCARD_STATE_INUSE = 0x100;
		/*===============================================================
		'  This implies that the card in the reader is in use by one or 
		'  more other applications, but may be connected to in shared mode. 
		'  If this bit is set, SCARD_STATE_PRESENT will also be set.
		 ===============================================================*/
		public const int SCARD_STATE_MUTE = 0x200;
		/*===============================================================
		'  This implies that the card in the reader is unresponsive or not
		'  supported by the reader or software.
		' ===============================================================*/
		public const int SCARD_STATE_UNPOWERED = 0x400;
		/*===============================================================
		'  This implies that the card in the reader has not been powered up.
		' ===============================================================*/

		public const int SCARD_SHARE_EXCLUSIVE = 1;
		/*===============================================================
		' This application is not willing to share this card with other 
		'applications.
		'===============================================================*/
		public const int  SCARD_SHARE_SHARED = 2;
		/*===============================================================
		' This application is willing to share this card with other 
		'applications.
		'===============================================================*/
		public const int SCARD_SHARE_DIRECT = 3;
		/*===============================================================
		' This application demands direct control of the reader, so it 
		' is not available to other applications.
		'===============================================================*/

		/*===========================================================
		'   Disposition
		'===========================================================*/
		public const int SCARD_LEAVE_CARD =   0;   // Don't do anything special on close
		public const int SCARD_RESET_CARD =   1;   // Reset the card on close
		public const int SCARD_UNPOWER_CARD = 2;   // Power down the card on close
		public const int SCARD_EJECT_CARD =   3;   // Eject the card on close

		/*===========================================================
		'   Error Codes
		'===========================================================*/
		public const int SCARD_F_INTERNAL_ERROR	   = -2146435071;  
		public const int SCARD_E_CANCELLED         = -2146435070;      
		public const int SCARD_E_INVALID_HANDLE    = -2146435069;      
		public const int SCARD_E_INVALID_PARAMETER = -2146435068;      
		public const int SCARD_E_INVALID_TARGET    = -2146435067;     
		public const int SCARD_E_NO_MEMORY         = -2146435066;    
		public const int SCARD_F_WAITED_TOO_LONG   = -2146435065;    
		public const int SCARD_E_INSUFFICIENT_BUFFER = -2146435064;   
		public const int SCARD_E_UNKNOWN_READER      = -2146435063;   
		
		
		public const int SCARD_E_TIMEOUT           = -2146435062;      
		public const int SCARD_E_SHARING_VIOLATION = -2146435061;      
		public const int SCARD_E_NO_SMARTCARD      = -2146435060;     
		public const int SCARD_E_UNKNOWN_CARD      = -2146435059;      
		public const int SCARD_E_CANT_DISPOSE      = -2146435058;     
		public const int SCARD_E_PROTO_MISMATCH    = -2146435057;      
		
		
		public const int SCARD_E_NOT_READY          = -2146435056;          
		public const int SCARD_E_INVALID_VALUE      = -2146435055;          
		public const int SCARD_E_SYSTEM_CANCELLED   = -2146435054;       
		public const int SCARD_F_COMM_ERROR         = -2146435053;             
		public const int SCARD_F_UNKNOWN_ERROR	    = -2146435052;          
		public const int SCARD_E_INVALID_ATR		= -2146435051;            
		public const int SCARD_E_NOT_TRANSACTED     = -2146435050;          
		public const int SCARD_E_READER_UNAVAILABLE = -2146435049;    
		public const int SCARD_P_SHUTDOWN			= -2146435048;              
		public const int SCARD_E_PCI_TOO_SMALL	    = -2146435047;          
		
		public const int SCARD_E_READER_UNSUPPORTED = -2146435046;          
		public const int SCARD_E_DUPLICATE_READER   = -2146435045;      
		public const int SCARD_E_CARD_UNSUPPORTED   = -2146435044;     
		public const int SCARD_E_NO_SERVICE		    = -2146435043;     
		public const int SCARD_E_SERVICE_STOPPED    = -2146435042;     

		public const int SCARD_W_UNSUPPORTED_CARD  = -2146435041;       
		public const int SCARD_W_UNRESPONSIVE_CARD = -2146435040;       
		public const int SCARD_W_UNPOWERED_CARD    = -2146435039;           
		public const int SCARD_W_RESET_CARD        = -2146435038;           
		public const int SCARD_W_REMOVED_CARD      = -2146435037; 

        public const int ERR_SLE4442_CARD_LOCKED   = 9853;    

		[DllImport("winscard.dll")]
		public static extern int SCardEstablishContext(int dwScope, int pvReserved1, int pvReserved2, ref int phContext);
		
		[DllImport("winscard.dll")]
		public static extern int SCardReleaseContext(int phContext);
		
		[DllImport("winscard.dll")]
		public static extern int SCardConnect(int hContext, string szReaderName, int dwShareMode, int dwPrefProtocol, ref int phCard, ref int ActiveProtocol);

		[DllImport("winscard.dll")]
		public static extern int SCardDisconnect(int hCard, int Disposition);
		
		[DllImport("winscard.dll")]
		public static extern int SCardListReaderGroups(int hContext, ref string mzGroups, ref int pcchGroups);
		
		[DllImport("winscard.DLL", EntryPoint="SCardListReadersA", CharSet=CharSet.Ansi)]
		public static extern int SCardListReaders(
			int hContext, 
			byte[] Groups,
			byte[] Readers,
			ref int pcchReaders
			);



        //=================================================================================
        //	Alcor Operation Dll
        //==================================================================================

        //Alcor Switch Card Type Mode
        [DllImport("AlcorSLE4442API.dll")]
		public  static extern int Alcor_SwitchCardMode(int lngCard, byte bSlotNum, byte	bCardMode);

        //Read Security Memory
        [DllImport("AlcorSLE4442API.dll")]
        public static extern int SLE4442Cmd_ReadSecurityMemory(int lngCard, byte bSlotNum, byte bReadLen, ref byte pReadData, ref byte pbReturnLen);

        //Read Main Memory
        [DllImport("AlcorSLE4442API.dll")]
        public static extern int SLE4442Cmd_ReadMainMemory(int lngCard, byte bSlotNum, byte bAddress, byte bReadLen, ref byte pReadData, ref byte pbReturnLen);

        //Write Protection Memory
        [DllImport("AlcorSLE4442API.dll")]
        public static extern int SLE4442Cmd_WriteProtectionMemory(int lngCard, byte bSlotNum, byte bAddress, byte bData);

        //Update Main Memory
        [DllImport("AlcorSLE4442API.dll")]
        public static extern int SLE4442Cmd_UpdateMainMemory(int lngCard, byte bSlotNum, byte bAddress, byte bData);

        //Verify PIN Number
        [DllImport("AlcorSLE4442API.dll")]
        public static extern int SLE4442Cmd_Verify(int lngCard, byte bSlotNum, uint lngPinLen, ref byte pPinData);

        //Update Security Memory
        [DllImport("AlcorSLE4442API.dll")]
        public static extern int SLE4442Cmd_UpdateSecurityMemory(int lngCard, byte bSlotNum, byte bAddress, byte bData);

		public static string GetScardErrMsg(int ReturnCode)
		{
			switch(ReturnCode)
			{
				case SCARD_E_CANCELLED:
					return("The action was canceled by an SCardCancel request.");
				case SCARD_E_CANT_DISPOSE:
					return("The system could not dispose of the media in the requested manner.");
				case SCARD_E_CARD_UNSUPPORTED:
					return("The smart card does not meet minimal requirements for support.");
				case SCARD_E_DUPLICATE_READER:
					return("The reader driver didn't produce a unique reader name.");
				case SCARD_E_INSUFFICIENT_BUFFER:
					return("The data buffer for returned data is too small for the returned data.");
				case SCARD_E_INVALID_ATR:
					return("An ATR string obtained from the registry is not a valid ATR string.");
				case SCARD_E_INVALID_HANDLE:
					return("The supplied handle was invalid.");
				case SCARD_E_INVALID_PARAMETER:
					return("One or more of the supplied parameters could not be properly interpreted.");
				case SCARD_E_INVALID_TARGET:
					return("Registry startup information is missing or invalid.");
				case SCARD_E_INVALID_VALUE:
					return("One or more of the supplied parameter values could not be properly interpreted.");
				case SCARD_E_NOT_READY:
					return("The reader or card is not ready to accept commands.");
				case SCARD_E_NOT_TRANSACTED:
					return("An attempt was made to end a non-existent transaction.");
				case SCARD_E_NO_MEMORY:
					return("Not enough memory available to complete this command.");
				case SCARD_E_NO_SERVICE:
					return("The smart card resource manager is not running.");
				case SCARD_E_NO_SMARTCARD:
					return("The operation requires a smart card, but no smart card is currently in the device.");
				case SCARD_E_PCI_TOO_SMALL:
					return("The PCI receive buffer was too small.");
				case SCARD_E_PROTO_MISMATCH:
					return("The requested protocols are incompatible with the protocol currently in use with the card.");
				case SCARD_E_READER_UNAVAILABLE:
					return("The specified reader is not currently available for use.");
				case SCARD_E_READER_UNSUPPORTED:
					return("The reader driver does not meet minimal requirements for support.");
				case SCARD_E_SERVICE_STOPPED:
					return("The smart card resource manager has shut down.");
				case SCARD_E_SHARING_VIOLATION:
					return("The smart card cannot be accessed because of other outstanding connections.");
				case SCARD_E_SYSTEM_CANCELLED:
					return("The action was canceled by the system, presumably to log off or shut down.");
				case SCARD_E_TIMEOUT:
					return("The user-specified timeout value has expired.");
				case SCARD_E_UNKNOWN_CARD:
					return("The specified smart card name is not recognized.");
				case SCARD_E_UNKNOWN_READER:
					return("The specified reader name is not recognized.");
				case SCARD_F_COMM_ERROR:
					return("An internal communications error has been detected.");
				case SCARD_F_INTERNAL_ERROR: 
					return("An internal consistency check failed.");
				case SCARD_F_UNKNOWN_ERROR:
					return("An internal error has been detected, but the source is unknown.");
				case SCARD_F_WAITED_TOO_LONG: 
					return("An internal consistency timer has expired.");
				case SCARD_S_SUCCESS:
					return("No error was encountered.");
				case SCARD_W_REMOVED_CARD:
					return("The smart card has been removed, so that further communication is not possible.");
				case SCARD_W_RESET_CARD:
					return("The smart card has been reset, so any shared state information is invalid.");
				case SCARD_W_UNPOWERED_CARD:
					return("Power has been removed from the smart card, so that further communication is not possible.");
				case SCARD_W_UNRESPONSIVE_CARD:
					return("The smart card is not responding to a reset.");
				case SCARD_W_UNSUPPORTED_CARD:
					return("The reader cannot communicate with the card, due to ATR string configuration conflicts.");
                case ERR_SLE4442_CARD_LOCKED:
                    return ("The smart card is locked.");
                default:
					return("?");
			}
		}

		public ModWinsCard()
		{
			//
			// TODO: Add constructor logic here
			//
		}


	}

