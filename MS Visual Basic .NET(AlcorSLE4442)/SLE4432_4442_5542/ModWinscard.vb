Imports System
Imports Microsoft.VisualBasic
Imports System.Runtime.InteropServices

<StructLayout(LayoutKind.Sequential)> _
Public Structure SCARD_IO_REQUEST

    Public dwProtocol As Integer
    Public cbPciLength As Integer

End Structure


<StructLayout(LayoutKind.Sequential)> _
Public Structure SCARD_READERSTATE

    Public RdrName As String
    Public UserData As Integer
    Public RdrCurrState As Integer
    Public RdrEventState As Integer
    Public ATRLength As Integer
    <MarshalAs(UnmanagedType.ByValArray, SizeConst:=37)> _
    Public ATRValue As Byte()
End Structure


Public Class ModWinsCard

    Public Const SCARD_S_SUCCESS = 0
    Public Const SCARD_ATR_LENGTH = 33


    '===============================================================
    ' CONTEXT SCOPE
    '===============================================================
    Public Const SCARD_SCOPE_USER = 0
    '===============================================================
    '  The context is a user context, and any database operations 
    '  are performed within the domain of the user.
    '===============================================================
    Public Const SCARD_SCOPE_TERMINAL = 1
    '===============================================================
    ' The context is that of the current terminal, and any database 
    ' operations are performed within the domain of that terminal.  
    ' (The calling application must have appropriate access 
    ' permissions for any database actions.)
    '===============================================================
    Public Const SCARD_SCOPE_SYSTEM = 2
    '===============================================================
    ' The context is the system context, and any database operations 
    ' are performed within the domain of the system.  (The calling
    ' application must have appropriate access permissions for any 
    ' database actions.)
    '===============================================================
    '=============================================================== 
    ' Context Scope
    '=============================================================== 
    Public Const SCARD_STATE_UNAWARE = &H0
    '===============================================================
    ' The application is unaware of the current state, and would like 
    ' to know. The use of this value results in an immediate return
    ' from state transition monitoring services. This is represented
    ' by all bits set to zero.
    '===============================================================
    Public Const SCARD_STATE_IGNORE = &H1
    '===============================================================
    ' The application requested that this reader be ignored. No other
    ' bits will be set.
    '===============================================================
    Public Const SCARD_STATE_CHANGED = &H2
    '===============================================================
    ' This implies that there is a difference between the state 
    ' believed by the application, and the state known by the Service
    ' Manager.When this bit is set, the application may assume a
    ' significant state change has occurred on this reader.
    '===============================================================
    Public Const SCARD_STATE_UNKNOWN = &H4
    '===============================================================
    ' This implies that the given reader name is not recognized by
    ' the Service Manager. If this bit is set, then SCARD_STATE_CHANGED
    ' and SCARD_STATE_IGNORE will also be set.
    '===============================================================
    Public Const SCARD_STATE_UNAVAILABLE = &H8
    '===============================================================
    ' This implies that the actual state of this reader is not
    ' available. If this bit is set, then all the following bits are
    ' clear.
    '===============================================================
    Public Const SCARD_STATE_EMPTY = &H10
    '===============================================================
    ' This implies that there is not card in the reader. If this bit
    ' is set, all the following bits will be clear.
    '===============================================================
    Public Const SCARD_STATE_PRESENT = &H20
    '===============================================================
    ' This implies that there is a card in the reader.
    '===============================================================
    Public Const SCARD_STATE_ATRMATCH = &H40
    '===============================================================
    ' This implies that there is a card in the reader with an ATR
    ' matching one of the target cards. If this bit is set,
    ' SCARD_STATE_PRESENT will also be set.  This bit is only returned
    ' on the SCardLocateCard() service.
    '===============================================================
    Public Const SCARD_STATE_EXCLUSIVE = &H80
    '===============================================================
    ' This implies that the card in the reader is allocated for 
    ' exclusive use by another application. If this bit is set,
    ' SCARD_STATE_PRESENT will also be set.
    '===============================================================
    Public Const SCARD_STATE_INUSE = &H100
    '===============================================================
    ' This implies that the card in the reader is in use by one or 
    ' more other applications, but may be connected to in shared mode.  
    ' If this bit is set, SCARD_STATE_PRESENT will also be set.
    '===============================================================
    Public Const SCARD_STATE_MUTE = &H200
    '===============================================================
    ' This implies that the card in the reader is unresponsive or not
    ' supported by the reader or software.
    '===============================================================
    Public Const SCARD_STATE_UNPOWERED = &H400
    '===============================================================
    ' This implies that the card in the reader has not been powered up.
    '===============================================================

    Public Const SCARD_SHARE_EXCLUSIVE = 1
    '===============================================================
    ' This application is not willing to share this card with other 
    'applications.
    '===============================================================
    Public Const SCARD_SHARE_SHARED = 2
    '===============================================================
    ' This application is willing to share this card with other 
    ' applications.
    '===============================================================
    Public Const SCARD_SHARE_DIRECT = 3
    '===============================================================
    ' This application demands direct control of the reader, so it 
    ' is not available to other applications.
    '===============================================================

    '===========================================================
    '   Disposition
    '===========================================================
    Public Const SCARD_LEAVE_CARD = 0   ' Don't do anything special on close
    Public Const SCARD_RESET_CARD = 1   ' Reset the card on close
    Public Const SCARD_UNPOWER_CARD = 2 ' Power down the card on close
    Public Const SCARD_EJECT_CARD = 3   ' Eject the card on close


    '===========================================================
    '   Error Codes
    '===========================================================
    Public Const SCARD_F_INTERNAL_ERROR = &H80100001
    Public Const SCARD_E_CANCELLED = &H80100002
    Public Const SCARD_E_INVALID_HANDLE = &H80100003
    Public Const SCARD_E_INVALID_PARAMETER = &H80100004
    Public Const SCARD_E_INVALID_TARGET = &H80100005
    Public Const SCARD_E_NO_MEMORY = &H80100006
    Public Const SCARD_F_WAITED_TOO_Integer = &H80100007
    Public Const SCARD_E_INSUFFICIENT_BUFFER = &H80100008
    Public Const SCARD_E_UNKNOWN_READER = &H80100009
    Public Const SCARD_E_TIMEOUT = &H8010000A
    Public Const SCARD_E_SHARING_VIOLATION = &H8010000B
    Public Const SCARD_E_NO_SMARTCARD = &H8010000C
    Public Const SCARD_E_UNKNOWN_CARD = &H8010000D
    Public Const SCARD_E_CANT_DISPOSE = &H8010000E
    Public Const SCARD_E_PROTO_MISMATCH = &H8010000F
    Public Const SCARD_E_NOT_READY = &H80100010
    Public Const SCARD_E_INVALID_VALUE = &H80100011
    Public Const SCARD_E_SYSTEM_CANCELLED = &H80100012
    Public Const SCARD_F_COMM_ERROR = &H80100013
    Public Const SCARD_F_UNKNOWN_ERROR = &H80100014
    Public Const SCARD_E_INVALID_ATR = &H80100015
    Public Const SCARD_E_NOT_TRANSACTED = &H80100016
    Public Const SCARD_E_READER_UNAVAILABLE = &H80100017
    Public Const SCARD_P_SHUTDOWN = &H80100018
    Public Const SCARD_E_PCI_TOO_SMALL = &H80100019
    Public Const SCARD_E_READER_UNSUPPORTED = &H8010001A
    Public Const SCARD_E_DUPLICATE_READER = &H8010001B
    Public Const SCARD_E_CARD_UNSUPPORTED = &H8010001C
    Public Const SCARD_E_NO_SERVICE = &H8010001D
    Public Const SCARD_E_SERVICE_STOPPED = &H8010001E
    Public Const SCARD_W_UNSUPPORTED_CARD = &H80100065
    Public Const SCARD_W_UNRESPONSIVE_CARD = &H80100066
    Public Const SCARD_W_UNPOWERED_CARD = &H80100067
    Public Const SCARD_W_RESET_CARD = &H80100068
    Public Const SCARD_W_REMOVED_CARD = &H80100069
    Public Const ERR_SLE4442_CARD_LOCKED = &H267D
    '===========================================================
    '   PROTOCOL
    '===========================================================
    Public Const SCARD_PROTOCOL_UNDEFINED = &H0           ' There is no active protocol.
    Public Const SCARD_PROTOCOL_T0 = &H1                  ' T=0 is the active protocol.
    Public Const SCARD_PROTOCOL_T1 = &H2                  ' T=1 is the active protocol.
    Public Const SCARD_PROTOCOL_RAW = &H10000             ' Raw is the active protocol.
    Public Const SCARD_PROTOCOL_DEFAULT = &H80000000      ' Use implicit PTS.
    '===========================================================
    '   READER STATE
    '===========================================================
    Public Const SCARD_UNKNOWN = 0
    '===============================================================
    ' This value implies the driver is unaware of the current 
    ' state of the reader.
    '===============================================================
    Public Const SCARD_ABSENT = 1
    '===============================================================
    ' This value implies there is no card in the reader.
    '===============================================================
    Public Const SCARD_PRESENT = 2
    '===============================================================
    ' This value implies there is a card is present in the reader, 
    'but that it has not been moved into position for use.
    '===============================================================
    Public Const SCARD_SWALLOWED = 3
    '===============================================================
    ' This value implies there is a card in the reader in position 
    ' for use.  The card is not powered.
    '===============================================================
    Public Const SCARD_POWERED = 4
    '===============================================================
    ' This value implies there is power is being provided to the card, 
    ' but the Reader Driver is unaware of the mode of the card.
    '===============================================================
    Public Const SCARD_NEGOTIABLE = 5
    '===============================================================
    ' This value implies the card has been reset and is awaiting 
    ' PTS negotiation.
    '===============================================================
    Public Const SCARD_SPECIFIC = 6
    '===============================================================
    ' This value implies the card has been reset and specific 
    ' communication protocols have been established.
    '===============================================================

    '==========================================================================
    ' Prototypes
    '==========================================================================

    Public Declare Function SCardEstablishContext Lib "Winscard.dll" (ByVal dwScope As Integer, _
                                                                      ByVal pvReserved1 As Integer, _
                                                                      ByVal pvReserved2 As Integer, _
                                                                      ByRef phContext As Integer) As Integer


    Public Declare Function SCardReleaseContext Lib "Winscard.dll" (ByVal hContext As Integer) As Integer


    Public Declare Function SCardConnect Lib "Winscard.dll" Alias "SCardConnectA" (ByVal hContext As Integer, _
                                                                                   ByVal szReaderName As String, _
                                                                                   ByVal dwShareMode As Integer, _
                                                                                   ByVal dwPrefProtocol As Integer, _
                                                                                   ByRef hCard As Integer, _
                                                                                   ByRef ActiveProtocol As Integer) As Integer


    Public Declare Function SCardDisconnect Lib "Winscard.dll" (ByVal hCard As Integer, _
                                                                ByVal Disposistion As Integer) As Integer

    Public Declare Function SCardBeginTransaction Lib "Winscard.dll" (ByVal hCard As Integer) As Integer


    Public Declare Function SCardEndTransaction Lib "Winscard.dll" (ByVal hCard As Integer, _
                                                                   ByVal Disposition As Integer) As Integer


    Public Declare Function SCardState Lib "Winscard.dll" (ByVal hCard As Integer, _
                                                           ByRef State As Integer, _
                                                           ByRef Protocol As Integer, _
                                                           ByRef ATR As Byte, _
                                                           ByRef ATRLen As Integer) As Integer

    Public Declare Function SCardStatus Lib "Winscard.dll" Alias "SCardStatusA" (ByVal hCard As Integer, _
                                                                                 ByVal szReaderName As String, _
                                                                                 ByRef pcchReaderLen As Integer, _
                                                                                 ByRef State As Integer, _
                                                                                 ByRef Protocol As Integer, _
                                                                                 ByRef ATR As Byte, _
                                                                                 ByRef ATRLen As Integer) As Integer

    Public Declare Function SCardTransmit Lib "Winscard.dll" (ByVal hCard As Integer, _
                                                              ByRef pioSendRequest As SCARD_IO_REQUEST, _
                                                              ByRef SendBuff As Byte, _
                                                              ByVal SendBuffLen As Integer, _
                                                              ByRef pioRecvRequest As SCARD_IO_REQUEST, _
                                                              ByRef RecvBuff As Byte, _
                                                              ByRef RecvBuffLen As Integer) As Integer



    Public Declare Function SCardListReaders Lib "Winscard.dll" Alias "SCardListReadersA" (ByVal hContext As Integer, _
                                                                                           ByVal mzGroup As String, _
                                                                                           ByVal ReaderList As String, _
                                                                                           ByRef pcchReaders As Integer) As Integer



    Public Declare Function SCardGetStatusChange Lib "Winscard.dll" Alias "SCardGetStatusChangeA" (ByVal hContext As Integer, _
                                                                                                   ByVal TimeOut As Integer, _
                                                                                                   ByRef ReaderState As SCARD_READERSTATE, _
                                                                                                   ByVal ReaderCount As Integer) As Integer



    Public Declare Function SCardControl Lib "Winscard.dll" (ByVal hCard As Integer, _
                                                              ByVal dwControlCode As Integer, _
                                                              ByRef pvInBuffer As Byte, _
                                                              ByVal cbInBufferSize As Integer, _
                                                              ByRef pvOutBuffer As Byte, _
                                                              ByVal cbOutBufferSize As Integer, _
                                                              ByRef pcbBytesReturned As Integer) As Integer

    '==========================================================================
    ' Alcor SLE4442 API
    '==========================================================================
    Public Declare Function Alcor_SwitchCardMode Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                            ByVal bSlotNum As Byte, _
                                                                            ByVal bCardMode As Byte) As Integer

    Public Declare Function SLE4442Cmd_ReadMainMemory Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                                ByVal bSlotNum As Byte, _
                                                                                ByVal bAddress As Byte, _
                                                                                ByVal bReadLen As Byte, _
                                                                                ByRef pReadData As Byte, _
                                                                                ByRef pbReturnLen As Integer) As Integer

    Public Declare Function SLE4442Cmd_ReadSecurityMemory Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                                ByVal bSlotNum As Byte, _
                                                                                ByVal bReadLen As Byte, _
                                                                                ByRef pReadData As Byte, _
                                                                                ByRef pbReturnLen As Integer) As Integer


    Public Declare Function SLE4442Cmd_Verify Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                                ByVal bSlotNum As Byte, _
                                                                                ByVal lngPinLen As Integer, _
                                                                                ByRef pPinData As Byte) As Integer


    Public Declare Function SLE4442Cmd_UpdateMainMemory Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                                ByVal bSlotNum As Byte, _
                                                                                ByVal bAddress As Byte, _
                                                                                ByVal bData As Byte) As Integer

    Public Declare Function SLE4442Cmd_UpdateSecurityMemory Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                            ByVal bSlotNum As Byte, _
                                                                            ByVal bAddress As Byte, _
                                                                            ByVal bData As Byte) As Integer


    Public Declare Function SLE4442Cmd_WriteProtectionMemory Lib "AlcorSLE4442API.dll" (ByVal hCard As Integer, _
                                                                        ByVal bSlotNum As Byte, _
                                                                        ByVal bAddress As Byte, _
                                                                        ByVal bData As Byte) As Integer



End Class