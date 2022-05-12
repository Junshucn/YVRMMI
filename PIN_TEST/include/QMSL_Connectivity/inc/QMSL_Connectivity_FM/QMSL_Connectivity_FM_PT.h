/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Connectivity/inc/QMSL_Connectivity_FM/QMSL_Connectivity_FM_PT.h#2 $
 * $DateTime: 2017/08/26 19:55:41 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#if !defined(_QLIB_FM_PT_H)
#define _QLIB_FM_PT_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/**
   Establish whether the library is being compiled into a DLL (exporting),
   or being included from a client (importing)

   When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

QLIB_API unsigned char QLIB_FTM_FM_PT_HCI_USER_CMD_WithEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pFMPTResponseBuffer, unsigned short* iFMPTResponseLength );

QLIB_API unsigned char QLIB_FTM_FM_PT_HCI_USER_CMD( HANDLE hResourceContext, unsigned char* pBuffer, unsigned char iLength);

/**
   Factory Test Mode FM Pass Through Commands, Enable Receiver

   This function will enable the receiver and return the current configuration.
   Only after successfully calling this function can many of the FM receiver interfaces be used.
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iStatus = 0 if success, else fail
   \param  iEmphasis =
        \code
                0 = FM_DE_EMP75                         75us time constant (the pre-emphasis/de-emphasis timing typically used in the US and Japan)
                1 = FM_DE_EMP50                         50us time constant (the pre-emphasis/de-emphasis timing typically used outside of the US and Japan)
        \endcode
   \param  iChannelSpacing =
        \code
                0 = FM_CHSPACE_200_KHZ
                1 = FM_CHSPACE_100_KHZ
                2 = FM_CHSPACE_50_KHZ
        \endcode
   \param  iRdsStd =
        \code
                0 = FM_RDS_STD_RBDS                     RBDS mode (Typically used in the United States).
                1 = FM_RDS_STD_RDS                      RDS mode (Typically used outside of the United States).
                2 = FM_RDS_STD_NONE                     No RDS/RBDS decoding.
        \endcode
   \param  iHighLowInjection =
         \code
               0 = AUTO_HIGH_SIDE_AND_LOW_SIDE
               1 = LOW_SIDE_INJECTION
               2 = HIGH_SIDE_INJECTION
         \endcode
   \param  iBandLowLimit = min freq (in KHz) for FM_USER_DEFINED_BAND, must be >= 76000
   \param  iBandHighLimit = max freq (in KHz) for FM_USER_DEFINED_BAND, must be <= 108000 AND > iUserDefinedBandFreqMin

   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_RX_ENABLE_RECEIVER( HANDLE hResourceContext, 
      unsigned char *iStatus, 
      unsigned char *iEmphasis, 
      unsigned char *iChannelSpacing,
      unsigned char *iRdsStd,
      unsigned char *iHighLowInjection,
      unsigned int *iBandLowLimit,
      unsigned int *iBandHighLimit );

/**
   Factory Test Mode FM Pass Through Commands

   This method downloads the nvm from the desired location.
  
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param invmFileLocation [in] = the nvm file location for downloading

   \return true if successful, false if fail.

   \warning
*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_NVM_DOWNLOAD(HANDLE hResourceContext,
      char *invmFileLocation);

/**
   Factory Test Mode FM Pass Through Commands, Disable Receiver

   This function will disable the receiver and return the status of this operation.
   While powered off, most functionality offered by the FM receiver will also be disabled until
        the client re-enables the receiver again via QLIB_FTM_FM_PT_RX_ENABLE_RECEIVER.
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iStatus = 0 if success, else fail
*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_DISABLE_RECEIVER( HANDLE hResourceContext, 
         unsigned char *iStatus );
   
/**
   Factory Test Mode FM Pass Through Commands, Get Receiver Configuration

   This function will return the current receiver configuration.
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iStatus = 0 if success, else fail
   \param  iEmphasis =
        \code
                0 = FM_DE_EMP75                         75us time constant (the pre-emphasis/de-emphasis timing typically used in the US and Japan)
                1 = FM_DE_EMP50                         50us time constant (the pre-emphasis/de-emphasis timing typically used outside of the US and Japan)
        \endcode
   \param  iChannelSpacing =
        \code
                0 = FM_CHSPACE_200_KHZ
                1 = FM_CHSPACE_100_KHZ
                2 = FM_CHSPACE_50_KHZ
        \endcode
   \param  iRdsStd =
        \code
                0 = FM_RDS_STD_RBDS                     RBDS mode (Typically used in the United States).
                1 = FM_RDS_STD_RDS                      RDS mode (Typically used outside of the United States).
                2 = FM_RDS_STD_NONE                     No RDS/RBDS decoding.
        \endcode
   \param  iHighLowInjection =
         \code
               0 = AUTO_HIGH_SIDE_AND_LOW_SIDE
               1 = LOW_SIDE_INJECTION
               2 = HIGH_SIDE_INJECTION
         \endcode
   \param  iBandLowLimit = min freq (in KHz) for FM_USER_DEFINED_BAND, must be >= 76000
   \param  iBandHighLimit = max freq (in KHz) for FM_USER_DEFINED_BAND, must be <= 108000 AND > iUserDefinedBandFreqMin

   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_GET_RECEIVER_CONFIGURATION( HANDLE hResourceContext, 
      unsigned char *iStatus, 
      unsigned char *iEmphasis, 
      unsigned char *iChannelSpacing, 
      unsigned char *iRDSStandard, 
      unsigned char *iHighAndLowSideInjection, 
      unsigned int *iBandLowLimit, 
      unsigned int *iBandHighLimit );
   
/**
   Factory Test Mode FM Pass Through Commands, Set Receiver Configuration
   
   This function will update  the current receiver configuration.
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iStatus = 0 if success, else fail
   \param  iEmphasis =
        \code
                0 = FM_DE_EMP75                         75us time constant (the pre-emphasis/de-emphasis timing typically used in the US and Japan)
                1 = FM_DE_EMP50                         50us time constant (the pre-emphasis/de-emphasis timing typically used outside of the US and Japan)
        \endcode
   \param  iChannelSpacing =
        \code
                0 = FM_CHSPACE_200_KHZ
                1 = FM_CHSPACE_100_KHZ
                2 = FM_CHSPACE_50_KHZ
        \endcode
   \param  iRdsStd =
        \code
                0 = FM_RDS_STD_RBDS                     RBDS mode (Typically used in the United States).
                1 = FM_RDS_STD_RDS                      RDS mode (Typically used outside of the United States).
                2 = FM_RDS_STD_NONE                     No RDS/RBDS decoding.
        \endcode
   \param  iHighLowInjection =
         \code
               0 = AUTO_HIGH_SIDE_AND_LOW_SIDE
               1 = LOW_SIDE_INJECTION
               2 = HIGH_SIDE_INJECTION
         \endcode
   \param  iBandLowLimit = min freq (in KHz) for FM_USER_DEFINED_BAND, must be >= 76000
   \param  iBandHighLimit = max freq (in KHz) for FM_USER_DEFINED_BAND, must be <= 108000 AND > iUserDefinedBandFreqMin

   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_RECEIVER_CONFIGURATION( HANDLE hResourceContext, 
      unsigned char iEmphasis, 
      unsigned char iChannelSpacing, 
      unsigned char iRDSStandard, 
      unsigned char iHighAndLowSideInjection, 
      unsigned int iBandLowLimit, 
      unsigned int iBandHighLimit, 
      unsigned char *iStatus );
   
/**
   Factory Test Mode FM Pass Through Commands, Set FM Mute Mode

   Allows the muting and un-muting of the audio coming from the FM receiver.
        This is an asynchronous command used to mute or un-mute the FM audio.
        This command mutes the audio coming from the FM device. It is important to note
        that this only affects the FM audio and not any other audio system being used.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iHardMute
         \code
               0 = FM_RX_NOMUTE     Unmute both audio channels
               1 = FM_RX_MUTELEFT   Mute only the left audio channel
               2 = FM_RX_MUTERIGHT  Mute only the right audio channel
               3 = FM_RX_MUTEBOTH   Mute both audio channels
         \endcode
   \param iSoftMute
         \code
               0 = SoftMuteDisabled
               1 = SoftMuteEnabled
         \endcode
   \param  iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_PT_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/ 
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_MUTE_MODE( HANDLE hResourceContext, 
      unsigned char iHardMute, 
      unsigned char iSoftMute, 
      unsigned char *iStatus );
   
/**
   Factory Test Mode FM Pass Through Commands, Set Stereo Mode


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStereoMode
         \code
               0 = FM_RX_AUTO_MONO_STEREO_MODE  Both mono and stereo reception is possible;  The FM hardware will transition to stereo when possible.
               1 = FM_RX_FORCE_MONO_ONLY_MODE   Force mono only reception.
         \endcode
   \param iSignalBlending
         \code
               0 = DISABLE_SIG_BLEND
               1 = ENABLE_SIG_BLEND
         \endcode
   \param iInterferenceBlending
         \code
               0 = DISABLE_INTF_BLEND
               1 = ENABLE_INTF_BLEND
         \endcode
   \param iMonoStereoSwitch
         \code
               0 = DISABLE_MONO_STEREO_SWITCH   Do not allow for switching between mono and stereo reception
               1 = ENABLE_MONO_STEREO_SWITCH    Allow for switching between mono and stereo reception. FM hardware will transition to stereo when possible.
         \endcode
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_STEREO_MODE( HANDLE hResourceContext, 
      unsigned char iStereoMode, 
      unsigned char iSignalBlending, 
      unsigned char iInterferenceBlending, 
      unsigned char iMonoStereoSwitch, 
      unsigned char *iStatus );
   
/**
   Factory Test Mode FM Pass Through Commands, Set Antenna


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAntenna
         \code
               0 = FM_ANTENNA_WIRED_HEADSET   Force selection of wired headset FM antenna.
               1 = FM_ANTENNA_PWB             Force selection of PWB FM antenna.
         \endcode
   
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_ANTENNA( HANDLE hResourceContext, 
      unsigned char iAntenna, 
      unsigned char *iStatus );

/**
   Factory Test Mode FM Pass Through Commands, Set FM station

    Tunes the FM receiver to the supplied FM frequency.
        This is an asynchronous command which tunes the FM receiver to a station as specified
        by the supplied frequency. Only valid frequencies within the band set by
        QLIB_FTM_FM_PT_SET_RECEIVER_CONFIGURATION can be tuned by this function.
        Attempting to tune to frequencies outside of the set band will result in an error.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iTuneFrequency =
        \code
                The frequency (in KHz) must be a multiple of 50 KHz and within the band ranges defined below

                FM_US_EUROPE_BAND:
                        US/Europe FM band (87500 KHz <-> 108000 KHz)

                FM_JAPAN_STANDARD_BAND:
                        Japan Standard Band (76000 KHz <-> 90000 KHz)

                FM_JAPAN_WIDE_BAND
                        Japan Wide Band (76000 KHz <-> 108000 KHz)

                FM_USER_DEFINED_BAND
                        User defined band ( x KHz <-> y KHz where x < y and x >= 76000 KHz and y <= 108000 KHz)
        \endcode
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_TUNE_STATION( HANDLE hResourceContext, unsigned int iTuneFrequency, 
         unsigned char *iStatus );

/**
   Factory Test Mode FM Pass Through Commands, Get FM station parameters

   Returns various statistics related to the currently tuned station.
        This is an asynchronous command which returns various statistics related to the currently
        tuned station. Included in these statistics are the currently tuned frequency,
        the RDS/RBDS sync status, the RSSI level, current mute settings and the stereo/mono status.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFrequencyTuned = Returns the currently tuned frequency in KHz
   \param iServiceAvailable = Returns the current service available indicator for the current station
        \code
                0 = FM_RX_SERVICE_NOT_AVAILABLE - No stations were found in the FM band.
                1 = FM_RX_SERVICE_AVAILABLE - At least one station was found in the FM band.
        \endcode
   \param iRSSI = Returns the current signal strength level (0-100 range).
        \code
                0 = -120 dBm
                1 = -119 dBm
                2 = -118 dBm
                ...
                100 = -20 dBm
   \param iStereoProgram = Returns the current mono/stereo indicator for this station.
        \code
                0 = FM_RX_MONO_PROGRAM - The currently received audio is mono
                1 = FM_RX_STEREO_PROGRAM - The currently received audio is stereo.
        \endcode
   \param iRDSSyncStatus = Returns the current RDS/RBDS synchronization status
        \code
                0 = FM_RX_NO_RDS_SYNC - No RDS/RBDS decoding possible.
                1 = FM_RX_RDS_SYNC - RDS/RBDS decoding.
        \endcode
   \param iMuteMode = Returns the current FM mute mode.
        \code
                0 = FM_RX_NOMUTE  Un-mute both audio channels
                1 = FM_RX_MUTELEFT - Mute only the left audio channel
                2 = FM_RX_MUTERIGHT - Mute only the right audio channel
                3 = FM_RX_MUTEBOTH - Mute both audio channels
        \endcode
   \param iSINR = Returns the measured sinr in dBm
   \param iInterferenceDetectorThreshold = Returns the measured interference detector threshold
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_GET_STATION_PARAMETERS( HANDLE hResourceContext, 
         unsigned char *iStatus, 
         unsigned int *iFrequencyTuned, 
         unsigned char *iServiceAvailable, 
         unsigned char *iRSSI, 
         unsigned char *iStereoProgram, 
         unsigned char *iRDSSyncStatus, 
         unsigned char *iMuteMode, 
         unsigned char *iSINR, 
         unsigned char *iInterferenceDetectorThreshold );

/**
   Factory Test Mode FM Pass Through Commands, Set Soft Mute

   This function enables/disables the soft-mute feature.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iType = 
         \code
               0 = CONSIDER_SINR_ONLY
               1 = CONSIDER_BOTH_SINR_AND_RMSSI
         \endcode
   \param iRampRate = Signed value
        \code
                0 = disable ramp rate
                Positive value = Limit X index change
                Negative value = wait based on the RX channel condition tiemr X time(s) and limit 1 index change.
        \endcode
   \param iSINRHigh = SINR corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F])
   \param iRMSSIHigh = RMSSI corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F]); ignored if Mute Type is 0x00
   \param iIndexHigh = Highest index being used in soft mute table
   \param softMuteTable
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_SOFT_MUTE( HANDLE hResourceContext, 
         unsigned char iType, 
         unsigned char iRampRate, 
         unsigned char iSINRHigh, 
         unsigned char iRMSSIHigh, 
         unsigned char iIndexHigh,
         unsigned char *softMuteTable,  
         unsigned char *iStatus );   
/**
   Factory Test Mode FM Pass Through Commands, Get Soft Mute

   This function returns the state of soft-mute feature configuration.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail
   \param iType = 
         \code
               0 = CONSIDER_SINR_ONLY
               1 = CONSIDER_BOTH_SINR_AND_RMSSI
         \endcode
   \param iRampRate = Signed value
        \code
                0 = disable ramp rate
                Positive value = Limit X index change
                Negative value = wait based on the RX channel condition tiemr X time(s) and limit 1 index change.
        \endcode
   \param iSINRHigh = SINR corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F])
   \param iRMSSIHigh = RMSSI corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F]); ignored if Mute Type is 0x00
   \param iIndexHigh = Highest index being used in soft mute table
   \param softMuteTable
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_GET_SOFT_MUTE( HANDLE hResourceContext, 
         unsigned char *iStatus, 
         unsigned char *iType, 
         unsigned char *iRampRate, 
         unsigned char *iSINRHigh, 
         unsigned char *iRMSSIHigh, 
         unsigned char *iIndexHigh );   

/**
   Factory Test Mode FM Pass Through Commands, Set Blend

   This function configures signal blending parameters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iType = 
         \code
               0 = CONSIDER_BOTH_SINR_AND_RMSSI
               1 = CONSIDER_SINR_ONLY
               2 = CONSIDER_RMSSI_ONLY
         \endcode
   \param iRampRateUp = Signed value
        \code
                0 = disable ramp rate
                Positive value = Limit X index change to higher index direction.
        \endcode
   \param iDebounceNumberOfSamplesUp = Need to see higher or equal to the value in the index Debounce Index Up this number of times in a row before applying higher index blend value.  0 disables this feature.
   \param iDebounceIndexUp = The new blend value which shall be higher or equal to the value in the index Debounce Index Up will be updated if the previously applied blend value is lower than the value in this index.  0 disables this feature.
   \param iSINRIndexSkipStep = The index step which will spread out SINR change slowly throughout blend table from the index Index High to the index 0 in table.  0 disables this feature.
   \param iSINRHigh = SINR corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F])
   \param iRMSSIHigh = RMSSI corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F]); ignored if Mute Type is 0x00
   \param iIndexHigh = Highest index being used in soft mute table
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_BLEND( HANDLE hResourceContext, 
         unsigned char iType, 
         unsigned char iRampRateUp, 
         unsigned char iDebounceNumberOfSamplesUp, 
         unsigned char iDebounceIndexUp, 
         unsigned char iSINRIndexSkipStep, 
         unsigned char iSINRHigh, 
         unsigned char iRMSSIHigh, 
         unsigned char iIndexHigh, 
         unsigned char *iStatus );   

/**
   Factory Test Mode FM Pass Through Commands, Get Blend

   This function returns the signal blending feature configuration.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail
   \param iType = 
         \code
               0 = CONSIDER_BOTH_SINR_AND_RMSSI
               1 = CONSIDER_SINR_ONLY
               2 = CONSIDER_RMSSI_ONLY
         \endcode
   \param iRampRateUp = Signed value
        \code
                0 = disable ramp rate
                Positive value = Limit X index change to higher index direction.
        \endcode
   \param iDebounceNumberOfSamplesUp = Need to see higher or equal to the value in the index Debounce Index Up this number of times in a row before applying higher index blend value.  0 disables this feature.
   \param iDebounceIndexUp = The new blend value which shall be higher or equal to the value in the index Debounce Index Up will be updated if the previously applied blend value is lower than the value in this index.  0 disables this feature.
   \param iSINRIndexSkipStep = The index step which will spread out SINR change slowly throughout blend table from the index Index High to the index 0 in table.  0 disables this feature.
   \param iSINRHigh = SINR corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F])
   \param iRMSSIHigh = RMSSI corresponding to the last active index item of the soft mute table based on Mute Index High (signed, -128 [0x80] to 127 [0x7F]); ignored if Mute Type is 0x00
   \param iIndexHigh = Highest index being used in soft mute table
   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_RX_ENABLE_RECEIVER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_GET_BLEND( HANDLE hResourceContext, 
         unsigned char *iStatus, 
         unsigned char *iType, 
         unsigned char *iRampRateUp, 
         unsigned char *iDebounceNumberOfSamplesUp, 
         unsigned char *iDebounceIndexUp, 
         unsigned char *iSINRIndexSkipStep, 
         unsigned char *iSINRHigh, 
         unsigned char *iRMSSIHigh, 
         unsigned char *iIndexHigh );

/**
   Factory Test Mode FM Pass Through Commands, FM Peek Byte

   This function will read the content of a register.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAddress = Memory address to read
   \param iDataLength = Length of data to read
   \param iStatus = 0 if success, else fail
   \param iAddressOut = Memory address read
   \param iReturnDataLength = Length of data read from memory
   \param returnData = Array of bytes containing data read from memory
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_PEEK_BYTE( HANDLE hResourceContext, 
         unsigned int iAddress, 
         unsigned char iDataLength, 
         unsigned char *iStatus, 
         unsigned int *iAddressOut, 
         unsigned char *iReturnDataLength,
         unsigned char *returnData[] );
   
/**
   Factory Test Mode FM Pass Through Commands, FM Peek Word

   This function will read the content of a register.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAddress = Memory address to read
   \param iDataLength = Length of data to read
   \param iStatus = 0 if success, else fail
   \param iAddressOut = Memory address read
   \param iReturnDataLength = Length of data read from memory
   \param returnData = Array of bytes containing data read from memory
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_PEEK_WORD( HANDLE hResourceContext, 
      unsigned int iAddress, 
      unsigned char iDataLength, 
      unsigned char *iStatus, 
      unsigned int *iAddressOut, 
      unsigned char *iReturnDataLength,
      unsigned int *returnData );
   
/**
   Factory Test Mode FM Pass Through Commands, FM Poke Byte

   This function will write the content of a register.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAddress = Memory address to write
   \param iDataLength = Length of data to write
   \param data = Array of bytes containing data to write to memory
   \param iStatus = 0 if success, else fail
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_POKE_BYTE( HANDLE hResourceContext, 
      unsigned int iAddress, 
      unsigned char iDataLength,
      unsigned char data[],
      unsigned char *iStatus );
   
/**
   Factory Test Mode FM Pass Through Commands, FM Poke Word

   This function will write the content of a register.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAddress = Memory address to write
   \param iDataLength = Length of data to write
   \param data = Array of bytes containing data to write to memory
   \param iStatus = 0 if success, else fail
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_POKE_WORD( HANDLE hResourceContext, 
      unsigned int iAddress, 
      unsigned char iDataLength, 
      unsigned int data,
      unsigned char *iStatus );

/**
   Factory Test Mode FM Pass Through Commands, FM Read Group Counters

   This function provides statistics related to incoming RDS data.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iResetCounters = 
         \code
               0 = Do NOT reset counters
               1 = Reset Counters
         \endcode
   \param iStatus = 0 if success, else fail
   \param iTotalRDSBlockErrors = Returns the Total count of RDS block errors received.
   \param iTotalRDSGroupsReceived = Returns the Total count of RDS groups received. 
   \param iTotalRDSGroup0 = Returns the Total count of RDS group type 0A/0B received. 
   \param iTotalRDSGroup2 = Returns the Total count of RDS group type 2A/2B received. 
   \param iTotalRDSBlockB = Returns the Total count of RDS groups which matched Block-B match condition.
   \param iTotalRDSProcessedGroup0 = Returns the Total count of interrupts from processing RDS group type 0A/0B. 
   \param iTotalRDSProcessedGroup2 = Returns the Total count of interrupts from processing RDS group type 2A/2B.
   \param iTotalRDSGroupFiltered = Returns the Total count of RDS groups which passed the group filter.
   \param iTotalRDSChangeFiltered = Returns the Total count of RDS groups which passed the change filter.
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_READ_GROUP_COUNTERS( HANDLE hResourceContext, 
      unsigned char iResetCounters, 
      unsigned char *iStatus, 
      unsigned int *iTotalRDSBlockErrors, 
      unsigned int *iTotalRDSGroupsReceived, 
      unsigned int *iTotalRDSGroup0, 
      unsigned int *iTotalRDSGroup2, 
      unsigned int *iTotalRDSBlockB, 
      unsigned int *iTotalRDSProcessedGroup0, 
      unsigned int *iTotalRDSProcessedGroup2, 
      unsigned int *iTotalRDSGroupFiltered, 
      unsigned int *iTotalRDSChangeFiltered );

/**
   Factory Test Mode FM Pass Through Commands, FM Read EXT Group Counters

   This function provides statistics related to incoming RDS data.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iResetCounters = 
         \code
               0 = Do NOT reset counters
               1 = Reset Counters
         \endcode
   \param iStatus = 0 if success, else fail
   \param iTotalRDSSyncLoss = Returns the Total count RDS groups which lost synchronization.
   \param iTotalRDSNotSync = Returns the Total count RDS groups which didn't sync. 
   \param iTotalRDSSyncInt = Returns the Total count RDS groups which triggered synchronization interrupt. 
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_READ_EXT_GROUP_COUNTERS( HANDLE hResourceContext, 
      unsigned char iResetCounters, 
      unsigned char *iStatus, 
      unsigned int *iTotalRDSSyncLoss, 
      unsigned int *iTotalRDSNotSync, 
      unsigned int *iTotalRDSSyncInt );

/**
   Factory Test Mode FM Pass Through Commands, Set RDS Group options

   This function enables or disables various RDS/RBDS group filtering and buffering features. 
   Included in these features are the RDS group enable mask, RDS/RBDS group change filter, and the RDS/RBDS group buffer size. 
   This is an asynchronous function used to set or unset various Rx RDS/RBDS group filtering and buffering options in the FM driver.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRDSGroupEnableMask = A bitmask of RDS/RBDS groups to enable. 
   \param iRDSBufferSize = The number of RDS/RBDS groups to buffer before sending to the client. Max is 21 groups
   \param iEnableRDSChangeFilter = Flag used to determine whether the RDS/RBDS change filter should be enabled
         \code
               0 = FTM_FM_RX_RDS_CHANGE_FILTER_DISABLED
               1 = FTM_FM_RX_RDS_CHANGE_FILTER_ENABLED
         \endcode
   \param iStatus = 0 if success, else fail
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_RDS_GROUPS( HANDLE hResourceContext, 
      unsigned int iRDSGroupEnableMask, 
      unsigned int iRDSBufferSize, 
      unsigned char iEnableRDSChangeFilter, 
      unsigned char *iStatus );

/**
   Factory Test Mode FM Pass Through Commands, Search Station List

   This function initiates basic seek and scan operations.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iSearchListMode = This setting describes different search modes offered by the FM driver based on received signal level for FM stations.
         \code
               0 = RETURN_ALL_MEETING_SIGNAL_LEVEL    FM driver will return a list of stations that meet a signal-level requirement (arranged in descending order by strength)
               1 = RETURN_ALL_NOT_MEETING_SIGNAL_LEVEL   FM driver will return a list of stations that meet a signal-level requirement (arranged in descending order by strength)
               2 = RETURN_STRONGEST_MEETING_SIGNAL_LEVEL    FM driver will return a list of the strongest stations in the band that meet a signal-level requirement (arranged in descending order by strength)
               3 = RETURN_WEAKEST_MEETING_SIGNAL_LEVEL      FM driver will return a list of the weakest stations in the band that meet a signal-level requirement (arranged in ascending order by strength)
               4 = RETURN_ALL_MEETING_SIGNAL_LEVEL_AND_MATCHING_PROGRAM_TYPE     FM driver will return a list of stations in the band that meet a signal-level requirement and are broadcasting the specified RDS/RBDS program type (arranged in descending order by strength)
         \endcode
   \param iSearchListDirection = This setting decribes whether searches will increment or decrement frequencies while searching the band
         \code
               0 = UP   Each frequency searched will be greater than the previous frequency (except when wrapping around the band 88.1 MHz -> 88.3 MHz)
               1 = DOWN Each frequency searched will be less than the previous frequency (except when wrapping around the band 88.1 MHz -> 87.9 MHz)
         \endcode
   \param iSearchListMax = Maximum number of stations found in the return list. 0x14 = Maximum number of stations.  0x00 = Invalid stations.
   \param iProgramType = The RDS/RBDS program type.
   \param numStationsFound = The total number of stations found in the search
   \param foundStations = Array containing the list of frequencies found in search
   \param iStatus = 0 if success, else fail
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SEARCH_STATION_LIST( HANDLE hResourceContext, 
      unsigned char iSearchListMode, 
      unsigned char iSearchListDirection, 
      unsigned int iSearchListMax, 
      unsigned char iProgramType,
      unsigned int *numStationsFound,
      unsigned int foundStations[],
      unsigned char *iStatus );

/**
   Factory Test Mode FM Pass Through Commands, Enable Notch Filter Control

   This function enables/disables a low pass or high pass notch filter.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iNotchEnable = This setting describes different search modes offered by the FM driver based on received signal level for FM stations.
         \code
               0 = DISABLED
               1 = ENABLE_LOW_PASS_NOTCH_FILTER
               2 = ENABLE_HIGH_PASS_NOTCH_FITLER
         \endcode
   \param iStatus = 0 if success, else fail
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_ENABLE_NOTCH_CONTROL( HANDLE hResourceContext, 
      unsigned char iNotchEnable, 
      unsigned char *iStatus );

/**
   Factory Test Mode FM Pass Through Commands, Set Get Reset AGC

   TThis function provides control of the AGC.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iucCtrl = 
         \code
               0x00 = Set AGC Gain State with "GainState" parameter.
               0x01 = Get Current AGC gain State and past 3 gain state changes. 
               0x02 = Reset AGC
         \endcode
   \param iucGainState =  
         \code
               0x00 = G0 state (HIGH)
               0x01 = G1 state (MIDPLUS)
               0x02 = G2 state (MID)
               0x03 = G3 state (MIDMIN)
               0x04 = G4 state (LOW)
         \endcode
   \param iStatus = 0 if success, else fail
   \param iucCurrentGainState = 
         \code
               0x00 = G0 state (HIGH)
               0x01 = G1 state (MIDPLUS)
               0x02 = G2 state (MID)
               0x03 = G3 state (MIDMIN)
               0x04 = G4 state (LOW)
         \endcode
   \param iucGainStateChange1 = 
         \code
               0x00 = G0 state (HIGH)
               0x01 = G1 state (MIDPLUS)
               0x02 = G2 state (MID)
               0x03 = G3 state (MIDMIN)
               0x04 = G4 state (LOW)
         \endcode
   \param iucGainStateChange2 = 
         \code
               0x00 = G0 state (HIGH)
               0x01 = G1 state (MIDPLUS)
               0x02 = G2 state (MID)
               0x03 = G3 state (MIDMIN)
               0x04 = G4 state (LOW)
         \endcode
   \param iucGainStateChange3 = 
         \code
               0x00 = G0 state (HIGH)
               0x01 = G1 state (MIDPLUS)
               0x02 = G2 state (MID)
               0x03 = G3 state (MIDMIN)
               0x04 = G4 state (LOW)
         \endcode
   
   \return true if successful, false if fail.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_GET_RESET_AGC( HANDLE hResourceContext, 
         unsigned char iucCtrl, 
         unsigned char iucGainState, 
         unsigned char *iStatus, 
         unsigned char *iucCurrentGainState, 
         unsigned char *iucGainStateChange1, 
         unsigned char *iucGainStateChange2, 
         unsigned char *iucGainStateChange3 );

/**
   Factory Test Mode FM Pass Through Commands, GET STATIONS INTERNAL DEBUG PARAMETERS

   This function returns the current station interference debug parameters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail
   \param iBlend =
         \code
               0x00 = OFF  The currently received audio is mono
               0x01 = ON
         \endcode
   \param iSoftMute = 
         \code
               0x00 = OFF Soft mute off
               0x01 = ON  Soft mute on
         \endcode
   \param iInterferenceBlend = Interference Blend
         \code
               0x00 = OFF  Interference mitigated blending off
               0x01 = ON   Interference mitigated blending on
         \endcode
   \param iInterferenceSoftMute = Interference Soft Mute
         \code
               0x00 = OFF  Interference mitigated soft mute off
               0x01 = ON   Interference mitigated soft mute on
         \endcode
   \param iPilotPLL = 
         \code
               0x00 = OFF  Pilot PLL lock off
               0x01 = ON   Pilot PLL lock on
         \endcode
   \param iInterferenceToCarrierRatio = Estimated interference to carrier ratio; two's complement value in dB
   \param iInterferenceDetectorOut = Zero crossing result
   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_GET_STATIONS_INTERNAL_DEBUG_PARAMETERS( HANDLE hResourceContext, 
         unsigned char *iStatus, 
         unsigned char *iBlend, 
         unsigned char *iSoftMute, 
         unsigned char *iInterferenceBlend, 
         unsigned char *iInterferenceSoftMute, 
         unsigned char *iPilotPLL, 
         unsigned char *iInterferenceToCarrierRatio, 
         unsigned char *iInterferenceDetectorOut );

/**
   Factory Test Mode FM Pass Through Commands, GET ALTERNATIVE RADIO FREQUENCY LIST

   This function returns the alternative frequency list for the current tuned station.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail
   \param iTuneFrequency = Currently tuned frequency
   \param iProgramIdentificationCode = Current Program Identification code
   \param iNumberOfAlternativeFrequencies = Number of alternative frequencies set for the tuned station
   \param altFreqList = Array with the first 20 stations in the alternative frequencies list
   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_GET_ALTERNATIVE_RADIO_FREQUENCY_LIST( HANDLE hResourceContext, 
         unsigned char *iStatus, 
         unsigned int *iTuneFrequency, 
         unsigned short *iProgramIdentificationCode, 
         unsigned char *iNumberOfAlternativeFrequencies,
         unsigned int altFreqList[] );

/**
   Factory Test Mode FM Pass Through Commands, SET RX RDS GROUP PROCESSING OPTIONS

   This function enables or disables RDS/RBDS group processing features.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iGroupsToProcess = Bit mask to determine what RDS processing the FM driver/hardware will do
      \code
         0x01 = FM_RX_RDS_GRP_RT_EBL         (RDS/RBDS Radio Text processing enable bit)
         0x02 = FM_RX_RDS_GRP_PS_EBL         (RDS/RBDS Program Service processing enable bit)
         0x04 = FM_RX_RDS_GRP_PS_SIMPLE_EBL   (RDS/RBDS Simple Program Service processing enable bit)
         0x08 = FM_RX_RDS_GRP_AF_EBL         (RDS/RBDS Alternative Frequency processing enable bit)
         0x10 = FM_RX_RDS_GRP_AF_JUMP_EBL   (RDS/RBDS Alternative Frequency Auto Jump enable bit)
         0x20 = FM_RX_RDS_GRP_ECC_EBL        (RDS/RBDS Extended Country Code enable bit)
         0x40 = FM_RX_RDS_GRP_PTNY_EBL       (RDS/RBDS Program Type Name enable bit)
        0x80 = FM_RX_RDS_GRP_RTPLUS_EBL       (RDS/RBDS RT PLUS (RT+) enable bit)
      \endcode
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_RDS_GROUP_PROCESSING( HANDLE hResourceContext, 
         unsigned int iGroupsToProcess, 
         unsigned char *iStatus );
/**
   Factory Test Mode FM Pass Through Commands, SEARCH STATIONS

   This function triggers an FM station search.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iSearchMode = Type of search to trigger
      \code
         0 = Seek
         1 = Scan
      \endcode
   \param iScanTime = Time (seconds) the FM driver will remain tune to a station during search.
   \param iSearchDirection = Direction in which to perform the search.
      \code
         0 = Up
         1 = Down
      \endcode
   \param iStatus = 0 if success, else fail
   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SEARCH_STATIONS(HANDLE hResourceContext, 
         unsigned char iSearchMode, 
         unsigned char iScanTime, 
         unsigned char iSearchDirection,
         unsigned char *iStatus);

/**
   Factory Test Mode FM Pass Through Commands, CANCEL SEARCH

   This function cancels any ongoing search operation (including seek, scan, and searchlist)

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail

   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_CANCEL_SEARCH(HANDLE hResourceContext,
         unsigned char *iStatus);


/**
   Factory Test Mode FM Pass Through Commands, Enable Transmitter

   Powers on FM transmitter.
   This is an asynchronous command used to power on the FM-RDS transmitter.
   
   This function will not initialize the transmitter. Initialization must be done throug the use of the 
   function QLIB_FTM_FM_TX_CONFIGURE_TRANSMITTER.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail
   \param  iEmphasis = Configuration currently set in the transmitter
      \code
         0 = FM_DE_EMP75            (pre-emphasis/de-emphasis typically used in the United States and Japan.)
         1 = FM_DE_EMP50            (pre-emphasis/de-emphasis typically used outside of the United states and Japan.)
      \endcode
   \param  iRdsStd = Configuration currently set in the transmitter
      \code
         0 = FM_RDS_STD_RBDS         RBDS mode (Typically used in the United States).
         1 = FM_RDS_STD_RDS          RDS mode (Typically used outside of the United States).
         2 = FM_RDS_STD_NONE         No RDS/RBDS decoding.
      \endcode

   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_ENABLE_TRANSMITTER(HANDLE hResourceContext,
         unsigned char *iStatus,
         unsigned char *iEmphasis,
         unsigned char *iRDSStandard);

/**
   Factory Test Mode FM Pass Through Commands, Disable Transmitter

   Powers off and disables the FM transmitter.
   This is an asynchronous command used to power-off and disable the FM transmitter.
   This function is expected to be used when the client no longer needs use of the FM transmitter.
   While powered off, most functionality offered by the FM transmitter will also be disabled until
   the client re-enables the device again via QLIB_FTM_FM_TX_ENABLE_TRANSMITTER.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStatus = 0 if success, else fail

   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_DISABLE_TRANSMITTER(HANDLE hResourceContext,
         unsigned char *iStatus);

/**
   Factory Test Mode FM Pass Through Commands, Configure Transmitter

   Configures the transmitters regional settings (FM Band, Pre-emphasis, Channel Spacing, RDS/RBDS mode).
   This is an asynchronous command used to reconfigure various settings of the transmitter.
   These settings typically differ from one geographical region to another.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iEmphasis =
      \code
         0 = FM_DE_EMP75            (pre-emphasis/de-emphasis typically used in the United States and Japan.)
         1 = FM_DE_EMP50            (pre-emphasis/de-emphasis typically used outside of the United states and Japan.)
      \endcode
   \param  iRdsStd =
      \code
         0 = FM_RDS_STD_RBDS         RBDS mode (Typically used in the United States).
         1 = FM_RDS_STD_RDS         RDS mode (Typically used outside of the United States).
         2 = FM_RDS_STD_NONE         No RDS/RBDS decoding.
      \endcode
   \param  iUserDefinedBandFreqMin = min freq (in KHz) for FM_USER_DEFINED_BAND, must be >= 76000
   \param  iUserDefinedBandFreqMax = max freq (in KHz) for FM_USER_DEFINED_BAND, must be <= 108000 AND > iUserDefinedBandFreqMin
   \param iStatus = 0 if success, else fail

   \return true if successful, false if fail.

   \warning

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_TRANSMITTER_CONFIGURATION(HANDLE hResourceContext, 
         unsigned char iEmphasis, 
         unsigned char iRDSStandard, 
         unsigned int iBandLowLimit, 
         unsigned int iBandHighLimit,
         unsigned char *iStatus);

/**
   Factory Test Mode FM Pass Through Commands, Select TX Internal Audio Tone

   This function selects between various audio tones to modulate the FM Transmitter

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTone
      \code
         0 = FM_TX_AUDIO_TONE_NOTONE            no audio tone
         1 = FM_TX_AUDIO_TONE_1KHZ_0DBFS_LR      1 kHz tone @ 0 dBFS applied equally to Left & Right
         2 = FM_TX_AUDIO_TONE_1KHZ_0DBFS_LEFT   1 kHz tone @ 0 dBFS applied to Left only
         3 = FM_TX_AUDIO_TONE_1KHZ_0DBFS_RIGHT   1 kHz tone @ 0 dBFS applied to Right only
         4 = FM_TX_AUDIO_TONE_1KHZ_N18DBFS_LR   1 kHz tone @ -18 dBFS applied equally to Left & Right
         5 = FM_TX_AUDIO_TONE_15KHZ_N18DBFS_LR   15 kHz tone @ -18 dBFS applied equally to Left & Right
      \endcode
   \param iStatus = 0 if success, else fail

   \return true if successful, false if fail.

   \warning QLIB_FTM_FM_TX_ENABLE_TRANSMITTER must be called before this.

*******************************************************************************/
QLIB_API unsigned char QLIB_FTM_FM_PT_SET_INTERNAL_TONE_GENERATOR(HANDLE hResourceContext, 
         unsigned char iTone,
         unsigned char *iStatus);

#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_FM_PT_H)