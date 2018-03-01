/*
 * libavs - Avantes interface for Avantes spectrometer library
 *
 *
 */

#ifndef _UNDOCUMENTED_H_
#define _UNDOCUMENTED_H_

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SECURE_VERSION_LEN        4
#define SECURE_SERIAL_LEN         9
#define SECURE_TIMESTAMP_LEN      4
#define SECURE_CUSTOMER_LEN      64
#define SECURE_APPL_LEN          64
#define SECURE_BLACKENED_LEN      1
#define SECURE_TECCONTROL_LEN     2
#define SECURE_NIRSENSITIVITY_LEN 1
#define SECURE_MAC_ADDR_LEN		  6

#define SECURE_WRITE_LEN            (SECURE_VERSION_LEN + SECURE_SERIAL_LEN + SECURE_TIMESTAMP_LEN + SECURE_CUSTOMER_LEN + SECURE_APPL_LEN + (SECURE_BLACKENED_LEN * 2) + (SECURE_TECCONTROL_LEN * 3) + SECURE_NIRSENSITIVITY_LEN + SECURE_MAC_ADDR_LEN)
#define SECURE_WRITE_RESERVED_LEN   (2044 - (SECURE_TOTAL_READ_LENGTH + (SECURE_VERSION_LEN + SECURE_SERIAL_LEN + SECURE_TIMESTAMP_LEN + SECURE_CUSTOMER_LEN + SECURE_APPL_LEN + (SECURE_BLACKENED_LEN * 2) + (SECURE_TECCONTROL_LEN * 3) + SECURE_NIRSENSITIVITY_LEN + SECURE_MAC_ADDR_LEN)))
#define SECURE_VERSION_ADDRESS      SDRAM_SETTINGS_START
#define SECURE_SERIAL_ADDRESS       SDRAM_SETTINGS_START + SECURE_VERSION_LEN

// read only secure registers
#define SECURE_TOTAL_READ_LENGTH  256
#define SECURE_DEVICE_TYPE_LEN    4

#define SECURE_READ_RESERVED_LEN          (SECURE_TOTAL_READ_LENGTH - (SECURE_DEVICE_TYPE_LEN))

typedef enum
{
        APP_UNKNOWN,
        APP_LOCAL,
        APP_EUC
} APPLICATION_NUMBERS;

#pragma pack(push,1)

typedef struct
{
    uint8   m_aSwVersion[SECURE_VERSION_LEN];              // used as ansichar (value '1')
    uint8   m_aSerialId[SECURE_SERIAL_LEN];                // used as ansistring
    uint8   m_aTimeStamp[SECURE_TIMESTAMP_LEN];            // used as float, cast from Delphi's TDateTime, which is a double
    uint8   m_aCustomerID[SECURE_CUSTOMER_LEN];            // used as ansistring
    uint8   m_aAppl[SECURE_APPL_LEN];                      // [2] used for AvaSoft full, '0' for false, '1' for true
                                                           // [3] through 10 used for apps
                                                           // [12] '1' for not dedicated, '2' for dedicated slave, '3' for dedicated master
    uint8   m_aNrOfBlackenedLeft[SECURE_BLACKENED_LEN];
    uint8   m_aNrOfBlackenedRight[SECURE_BLACKENED_LEN];
    uint8   m_aTecCtrl_P[SECURE_TECCONTROL_LEN];           // used as a uint16
    uint8   m_aTecCtrl_I[SECURE_TECCONTROL_LEN];           // used as a uint16
    uint8   m_aTecCtrl_D[SECURE_TECCONTROL_LEN];           // used as a uint16
    uint8   m_aNirSensitivity[SECURE_NIRSENSITIVITY_LEN];  // low noise vs. high sensitivity, ansichar #100 vs. #101
	uint8	m_aMacAddr[SECURE_MAC_ADDR_LEN];               // used as 6 byte number 
    uint8   m_aWriteReserved[SECURE_WRITE_RESERVED_LEN];   // new read/write registers should be added here
    uint8   m_aDeviceType[SECURE_DEVICE_TYPE_LEN];         // read only registers
    uint8   m_aReadReserved[SECURE_READ_RESERVED_LEN];
} SecureType;

typedef enum
{
    HW_TEST_PASSED = 0,
    HW_TEST_FAILED,
    HW_WRONG_HARDWARE,	
    HW_TEST_NOT_IMPLEMENTED 
} HW_TEST_RESULT_T;

typedef struct
{
    // eui24aa; MAC address chip
    uint8   eui24aa_TestResult;       // HW_TEST_RESULT_T
    uint8   eui24aa_MacAddr[SECURE_MAC_ADDR_LEN]; // SECURE_MAC_ADDR_LEN = 6
    uint32  eui24aa_ErrorCount;
    // adp5050; Power regulator chip
    uint8   adp5050_TestResult;       // HW_TEST_RESULT_T
    uint8   adp5050_Channel1Stat;     // 0=disabled 1=enabled
    uint8   adp5050_Channel2Stat;
    uint8   adp5050_Channel3Stat;
    uint8   adp5050_Channel4Stat;
    uint32  adp5050_ErrorCount;
    // emc1001; Temperature sensor chip
    uint8   emc1001_TestResult;       // HW_TEST_RESULT_T
    uint8   emc1001_ProductId;
    uint8   emc1001_ManufactureId;
    uint8   emc1001_RevNr;
    uint16  emc1101_BoardTemperature; // Degrees Centigrade * 100
    uint32  emc1001_ErrorCount;
    // mp25p80; SPI flash chip 
    uint8   mp25p80_TestResult;       // HW_TEST_RESULT_T - RD/WR to flash result
    uint8   mp25p80_DevStatus;        // UNKNOWN=0, NOT_BUSY=1, BUSY=2
    uint8   mp25p80_ManufacturerId;   // 0x20
    uint8   mp25p80_DevIdMemoryType;  // 0x20
    uint8   mp25p80_DevIdDensity;     // 0x14 = 8 Mbit
    // s25fl256; QUAD SPI flash chip
    uint8   s25fl256_TestResult;      // HW_TEST_RESULT_T - RD/WR to flash result
    uint8   s25fl256_DevStatus;       // OK=0, Programming error=1, Erase error=2 Both=3
    uint8   s25fl256_ManufacturerId;  // 0x01
    uint8   s25fl256_DevIdMemoryType; // 0x02 = S25FL256
    uint8   s25fl256_DevIdDensity;    // 0x19 = 256 Mbit
    // XADC;
    uint8   xadc_TestResult;          // HW_TEST_RESULT_T
    uint16  xadc_NtcValue;            // millivolts
    // GIC; General Interrupt Controller peripheral
    uint8   gic_TestResult;           // HW_TEST_RESULT_T
    // External I/O; XGPIO (FPGA) and XGPIOPS (Cortex) peripheral
    uint8   xgpioFpga_TestResult;     // HW_TEST_RESULT_T
    uint8   xgpioCortex_TestResult;   // HW_TEST_RESULT_T
    // DAC analog out; NO TESTS AVAILABLE
}   HWTestType; // 46 bytes

//----------------------------------------------------------------------------
//
// Name         : AVS_GetRawScopeData
//
// Description  : Returns the values for each pixel
//
// Parameters   : a_hDevice     : device handle
//                a_pTimeLabel  : ticks count last pixel of spectrum is received
//                                by microcontroller, ticks in 10 mS units since
//                                spectrometer started
//                a_pSpectrum   : pointer to array of unsigned int.
//
// Returns      : integer       : 0, successfully started
//                                error code on error
//
// Remark(s)    : array size not checked
//
//----------------------------------------------------------------------------
DLL_INT AVS_GetRawScopeData( AvsHandle a_hDevice, bool a_SendAck, unsigned int* a_pTimeLabel, unsigned short* a_pAvg, uint32* a_pSpectrum );

//----------------------------------------------------------------------------
//
// Name         : AVS_GetMeasPixels
//
// Description  : Returns the number of measuered pixels including dark
//
// Parameters   : a_hDevice     : device handle
//                a_pMeasPixels : pointer to unsigned int.
//
// Returns      : integer       : 0, successfully started
//                                error code on error
//
// Remark(s)    : 
//
//----------------------------------------------------------------------------
DLL_INT AVS_GetMeasPixels( AvsHandle a_hDevice , uint32* a_pMeasPixels );

//----------------------------------------------------------------------------
//
// Name         : AVS_GetVersionNumbers
//
// Description  : Returns the version numbers
//
// Parameters   : AvsIdentityType  a_pDeviceId
//                a_pFPGAVersion, pointer to buffer to store version (32bit)
//                a_pFirmwareVersion, pointer to buffer to store version (32bit)
//            	  a_pDLLVersion pointer to buffer to store version (32bit)
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
// Remark(s)    : Does not check the size of the buffers allocated by the caller.
//
//----------------------------------------------------------------------------
DLL_INT AVS_GetVersionNumbers
(
    AvsIdentityType   a_pDeviceId,
    unsigned int*     a_pHardwareVersion,
    unsigned int*     a_pFPGAVersion,
    unsigned int*     a_pFirmwareVersion
);

//----------------------------------------------------------------------------
//
// Name         : AVS_GetSecureConfig
//
// Description  : Returns the Secure config
//
// Parameters   : AvsIdentityType*  a_pDeviceId
//            	  a_Data pointer to buffer to store Secure data
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
// Remark(s)    : Does not check the size of the buffers allocated by the caller.
//
//----------------------------------------------------------------------------
DLL_INT AVS_GetSecureConfig( AvsIdentityType* a_pDeviceId, SecureType *a_Data );

//----------------------------------------------------------------------------
//
// Name         : AVS_ForceStopMeas
//
// Description  : Send a stop message to the device not checking its state.
//
// Parameters   : AvsIdentityType*  a_pDeviceId
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
// Remark(s)    : 
//
//----------------------------------------------------------------------------
DLL_INT AVS_ForceStopMeas( AvsIdentityType*  a_pDeviceId );


//----------------------------------------------------------------------------
//
// Name         : AVS_SetSecureConfig
//
// Description  : Send the Secure config to the device
//
// Parameters   : a_hDevice     : device handle
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
//
//----------------------------------------------------------------------------
DLL_INT AVS_SetSecure
(
    AvsHandle       a_hDevice,
    char*           a_PasswordNr,
    SecureType*     a_pSrc
);

//----------------------------------------------------------------------------
// Name         : AVS_Start_SourceUpdate
//
// Description  : Start transfer of data to flash memory on AS71010
//
// Parameters   : a_hDevice     : device handle
//                a_SourceType  : type of flash memory to update (FX3, Cortex FW, FPGA)
//                a_Length      : total length of data that will be transferred
//
// Returns      : integer       : 0, ok
//                                <0 on error
//
//
//----------------------------------------------------------------------------

DLL_INT AVS_Start_SourceUpdate
(
    AvsHandle      a_hDevice,
    unsigned char  a_SourceType,
    unsigned int   a_Length
);

//----------------------------------------------------------------------------
//
// Name         : AVS_EnterMaintenanceMode
//
// Description  : Forces the device into maintenance mode
//                (AvaSpec Mini only)
//
// Parameters   : a_hDevice     : device handle
//
// Returns      : integer       : 0, ok
//                                <0 on error
//
//
//----------------------------------------------------------------------------
DLL_INT AVS_EnterMaintenanceMode
(
    AvsHandle       a_hDevice
);

//----------------------------------------------------------------------------
//
// Name         : AVS_TransferFW
//
// Description  : Transfers firmware data in order to update device firmware
//                (both for AvaSpec Mini and AS71010)
//
// Parameters   : a_hDevice     : device handle
//                a_Address     : address where the data block is to be positioned
//                a_Size        : size of the data block 
//                a_pData       : pointer to buffer that stores the data
//                a_Crc         : pointer to uint32, the firmware will calculate 
//                                the CRC code of the transfered data and set the 
//                                result value in this variable.
//
// Returns      : integer       : 0, ok
//                                <0 on error
//
//
//----------------------------------------------------------------------------

DLL_INT AVS_TransferFW
(
    AvsHandle      a_hDevice,
    unsigned int   a_Address,
    unsigned int   a_Size,
    unsigned char  *a_pData,
	unsigned int   *a_Crc
);

//----------------------------------------------------------------------------
//
// Name         : AVS_WriteFWToROM
//
// Description  : Start the transfer of the firmware data form SDRAM to EEPROM 
//                (AvaSpec Mini only)
//
// Parameters   : a_hDevice     : device handle
//
// Returns      : integer       : 0, ok
//                                <0 on error
//
//----------------------------------------------------------------------------
DLL_INT AVS_StartWriteFWToROM
(
    AvsHandle      a_hDevice
);

//----------------------------------------------------------------------------
//
// Name         : AVS_UpdateFWToROMFinished
//
// Description  : Will give the number of block that still needs to be written.
//                (AvaSpec Mini only)
//                (The block size is 256 bytes).
//
// Parameters   : a_hDevice     : device handle
//                a_pNrActions  : Number of blocks left, on error this value 
//                                will be max uint32
//
// Returns      : integer       : 0, ok
//                                <0 on error
//
//----------------------------------------------------------------------------
DLL_INT AVS_UpdateFWToROMFinished
(
    AvsHandle      a_hDevice,
    uint32         *a_pNrActions
);

//----------------------------------------------------------------------------
//
// Name         : AVS_UpdateFWToROMErrors
//
// Description  : Will give the number of byte errors found after comparing the SDRAM and ROM data.
//                (AvaSpec Mini only)
//
// Parameters   : a_hDevice     : device handle
//                a_pErrors     : Number of byte that are different after write to ROM
//                                on a communication error the value will be max uint32
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
//----------------------------------------------------------------------------
DLL_INT AVS_UpdateFWToROMErrors
(
    AvsHandle      a_hDevice,
    uint32         *a_pErrors
); 

//----------------------------------------------------------------------------
//
// Name         : AVS_ResetDevice
//
// Description  : After this command is replied the device will do a hard reset.
//                 
//
// Parameters   : a_hDevice     : device handle
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
//----------------------------------------------------------------------------
DLL_INT AVS_ResetDevice
(
    AvsHandle      a_hDevice
);

//----------------------------------------------------------------------------
//
// Name         : AVS_HW_TestReport
//
// Description  : This command will collect a hardware test report from the AS7010
//                Implemented only for the Ethernet interface of the AS71010
//
// Parameters   : a_hDevice     : device handle
//                a_Data        : pointer to buffer containing the hardware test report 
//
// Returns      : integer         : 0, ok
//                                  <0 on error
//
//----------------------------------------------------------------------------
DLL_INT AVS_HW_TestReport
(
    AvsHandle       a_hDevice,
    HWTestType      *a_Data
);

//----------------------------------------------------------------------------

#pragma pack(pop)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

