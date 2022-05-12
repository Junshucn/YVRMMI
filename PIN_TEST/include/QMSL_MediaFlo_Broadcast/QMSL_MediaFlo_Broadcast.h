/******************************************************************************
 * $File: //depot/HTE/QDART/QMSL6/inc/QMSL_MediaFlo_Broadcast/QMSL_MediaFlo_Broadcast.h $
 *
 * DESCRIPTION: QMSL_MediaFlo_Broadcast
 ******************************************************************************
 *
 * Copyright (c) 2014 QUALCOMM Technologies Incorporated.
 * All rights reserved.
 * Qualcomm Confidential and Proprietary
 *
 ******************************************************************************
 *
 * $Header: //depot/HTE/QDART/QMSL6/inc/QMSL_MediaFlo_Broadcast/QMSL_MediaFlo_Broadcast.h#2 $
 * $DateTime: 2014/01/15 10:29:33 $
 *
 */
#if !defined(_QMSL_MEDIAFLO_BROADCAST_H)
#define _QMSL_MEDIAFLO_BROADCAST_H

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
//#define QLIB_API __cdecl
#endif


#ifdef __cplusplus
extern "C" {
#endif   
   /******************************************************************************
                     MediaFLO FTM
   *******************************************************************************/

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.10 Subcommand 400 - Get RSSI Cal Point

         This function returns a 16-bit signed RSSI cal point for the requested gain state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iGainState = the gain state (0 = highest gain, 3 = lowest gain)
   \param iInputPower = the receiver input power, 1 dB resolution
   \param piCalPoint = the RSSI cal point for the requested gain state, 1/256 dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_GET_RSSI_CAL_POINT(HANDLE hResourceContext, unsigned short iGainState,
         short iInputPower, short *piCalPoint);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.9 Subcommand 401 - Get Rx RSSI

         This function returns a 16-bit signed receiver signal strength indicator (RSSI) value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piRssiValue = RSSI value in 1/100 dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_GET_RX_RSSI( HANDLE hResourceContext, short *piRssiValue );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.7 Subcommand 402 - Get AGC State

         This function returns the current gain state and 15-bit signed DVGA accumulator value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piGainState = current gain state (0 = highest gain state, 3 = lowest gain state)
   \param piDvga = current DVGA accumulator value, 15-bit signed value

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_GET_AGC_STATE(HANDLE hResourceContext, unsigned short *piGainState, short *piDvga);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.12 Subcommand 403 - Write Cal Data

         This function stores the current state of the RF Calibration to NV.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iBand = the MediaFLO band to write to NV

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_WRITE_CAL_DATA(HANDLE hResourceContext, unsigned short iBand);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.2 Subcommand 406 - LNA Select

         This function enables the desired LNA in the RBR device.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iWhichLna = which LNA to enable (1=LNA1, 2=LNA2)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_LNA_SELECT( HANDLE hResourceContext, unsigned short iWhichLna );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.3 Subcommand 407 - RF Switch Config

         This function sets the state of the LNA RF Switch Control pins SW_CTL1 and SW_CTL2.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iState = the state of the LNA RF Switch Control pins
                  0 - SW_CTL1 Low, SW_CTL2 Low
                  1 - SW_CTL1 Low, SW_CTL2 High
                  2 - SW_CTL1 High, SW_CTL2 Low
                  3 - SW_CTL1 High, SW_CTL2 High

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_RF_SWITCH_CONFIG( HANDLE hResourceContext, unsigned short iState );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.6 Subcommand 408 - Synth Lock

         This function returns the lock state of the synthesizer.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pbLockState = the lock state of the synthesizer (0=unlocked, 1=locked)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_SYNTH_LOCK( HANDLE hResourceContext, unsigned char* pbLockState );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.8 Subcommand 411 - Set Gain State

         This function sets the gain state of the currently selected LNA.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iGainState = the desired gain state (0=highest gain state, 3=lowest gain state)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_SET_GAIN_STATE(HANDLE hResourceContext, unsigned short iGainState);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.5 Subcommand 413 - Tune PLL

         This function tunes the PLL to the requested FLO Air Interface Specification channel.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iChannel = the desired channel (0 - 8191)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_TUNE_PLL( HANDLE hResourceContext, unsigned short iChannel );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO, 3.11 Subcommand 415 - Get IM2

         This function gets the MediaFlo IM2 calibration values.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piSuppression = characteristic IM2 performance improvement vs. uncalibrated in dB
   \param piI_Value = I channel IM2 calibration value
   \param piQ_Value = Q channel IM2 calibration value
   \param piTransConductance_Value = Transconductor IM2 calibration value

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MF_GET_IM2(	HANDLE hResourceContext,
         short* piSuppression,
         unsigned char* piI_Value,
         unsigned char* piQ_Value,
         unsigned char* piTransConductance_Value);

   /******************************************************************************
                     MediaFLO FTM UBM
   *******************************************************************************/

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.l Subcommand 0 - Get RSSI Cal Point

         This function returns a 16-bit signed RSSI cal point for the requested gain state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iGainState = the gain state (0 = highest gain, 3 = lowest gain)
   \param iInputPower = the receiver input power, 1 dB resolution
   \param iNumberOfSamples = the number of times to sample the cal point
   \param piCalPoint = the RSSI cal point for the requested gain state, 1/256 dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_GET_RSSI_CAL_POINT(HANDLE hResourceContext,
         unsigned char iGainState,
         unsigned short iInputPower,
         unsigned short iNumberOfSamples,
         short* piCalPoint);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.2.2 Subcommand 1 - Get Rx RSSI

         This function returns a 16-bit signed receiver signal strength indicator (RSSI) value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piIsValid = Returns whether the RSSI value is valid (1) or not (0).
   \param piRssiValue = RSSI value in 1/100 dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_GET_RSSI(HANDLE hResourceContext,
         unsigned char *piIsValid,
         short *piRssiValue );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.3 Subcommand 2 - Get AGC State

         This function returns the current gain state and 15-bit signed DVGA accumulator value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piGainState = current gain state (0 = highest gain state, 3 = lowest gain state)
   \param piDvga = current DVGA accumulator value, 15-bit signed value

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_GET_AGC_STATE(HANDLE hResourceContext,
         short *piDvga,
         unsigned char  *piGainState);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.4 Subcommand 3 - Write Cal Data to NV

         This function stores the current state of the RF Calibration to NV.

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()
   \param iitemCode=			either 0, for the channel list, or 1 for the RSSI calibration point
   \param iGainState=			when writing the calibration point, this specifes the coresponding gains state.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_WRITE_CAL_DATA_TO_NV(HANDLE hResourceContext,
         unsigned long iItemCode,
         unsigned long iGainState);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.5 Subcommand 4 - Set RF On.

         This command allows the MBP RF_ON signal to be set to the active or inactive state

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()
   \param bRFOn=			    either 0, to set RF_ON to inactive, or 1 to set RF_ON to active.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_SET_RF_ON(HANDLE hResourceContext, unsigned char bRFOn);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.6 Subcommand 5 - Powerup.

         This command enables the RF power supply.

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_RF_POWER_UP_RF(HANDLE hResourceContext);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.7 Subcommand 6 - Select LNA.

         This command allows the LNA selection to be modified
         independent of the FTM_MF_RF_INITIALIZE command.

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLNA	 =				LNA value to select.
      \code
      MFLO UHF_LNA =0,
      MFLO_US_HIGH_BAND_LNA = 1,
      MFLO_US_LOW_BAND_LNA = 2,
      MFLO_L_BAND_LNA = 3,
      \endcode

   \return true if successful, false if fail.

   \warning Recommended for MBP1600/MBP1610 only
          Not supported on MBP2600.
          Not recommended for new MBP2700 applications (see QLIB_FTM_MF_LNA_SELECT_2)

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_LNA_SELECT(HANDLE hResourceContext, unsigned char iLNA);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.7 Subcommand 21 - Select LNA.

         This command allows the LNA selection to be modified
         independent of the FTM_MF_RF_INITIALIZE command.

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRfMode = The RF mode to initialise.
   Defined by ftm_mflo_rf_mode in QLib_Defines.h:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iLNASelect = LNA value to select.
   Defined by ftm_mflo_rf_lna_type in QLib_Defines.h:
      \code
      MFLO_T_UHF1		= 0,	( MBP2600 )
      MFLO_F_LBAND	= 1,	( MBP2700 LBAND )
      MFLO_F_LNA1		= 2,	( MBP2700 LNA1 )
      MFLO_F_LNA2		= 3,	( MBP2700 LNA2 )
      \endcode
   \param iStartFreq = The start frequency (ignored if both start and end frequencies are zero).
   \param iEndFreq = The end frequency (ignored if both start and end frequencies are zero).

   \return true if successful, false if fail.

   \warning MBP2700 only

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_LNA_SELECT_2(HANDLE hResourceContext,
         unsigned char iRfMode,
         unsigned char iLNASelect,
         unsigned long iStartFreq,
         unsigned long iEndFreq);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.8 Subcommand 7 - Set Antenna tune switch.

         This command allows the antenna tune selection to be modified independent
         of the FTM_MF_RF_INITIALIZE command

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()
   \param iValue	 =			6 LSB are written to the Antenna tune switch.

   \return true if successful, false if fail.

   \warning Recommended for MBP1600/MBP1610 only
          Not supported on MBP2600.
          Not recommended for new MBP2700 applications (see QLIB_FTM_UBM_MF_RF_SET_ANT_TUNE_2)

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_RF_SET_ANT_TUNE(HANDLE hResourceContext, unsigned char iValue);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.8 Subcommand 7 - Set Antenna tune switch.

         This command allows the antenna tune selection to be modified independent
         of the FTM_MF_RF_INITIALIZE command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRfMode = The RF mode to initialise.
   Defined by ftm_mflo_rf_mode in QLib_Defines.h:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iAntTuneMask = The mask to apply to the Tune Settings.
   \param iAntTuneSettings = The Antenna Tune Settings (only bits with a corresponding 1 in the Tune Mask will actually be set)
   \param iStartFreq = The start frequency (ignored if both start and end frequencies are zero).
   \param iEndFreq = The end frequency (ignored if both start and end frequencies are zero).

   \return true if successful, false if fail.

   \warning MBP2700 Only

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_RF_SET_ANT_TUNE_2(HANDLE hResourceContext,
         unsigned char iRfMode,
         unsigned char iAntTuneMask,
         unsigned char iAntTuneSettings,
         unsigned long iStartFreq,
         unsigned long iEndFreq);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.9 Subcommand 8 - Synth Lock

         This function returns the lock state of the synthesizer.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pbLockState = the lock state of the synthesizer (0=unlocked, 1=locked)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_SYNTH_LOCK( HANDLE hResourceContext, unsigned short* pbLockState );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.10 Subcommand 9 - Enable/Disable LNA

         This command allows the LNA to be enabled/disabled independent of the FTM_MF_RF_INITIALIZE command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param bLNAState = either 0, to disable LNA update, 1 to enable LNA update.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_SET_LNA ( HANDLE hResourceContext, unsigned char bLNAState );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, Subcommand 10 - Tune RF In Hz

      This function tunes the PLL to the requested frequency.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param bfreqHz = Required frequency in Hz.

   \return true if successful, false if fail.

   \warning Supported in MBP2600/MBP2700 only.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_TUNE_RF_IN_HZ ( HANDLE hResourceContext, unsigned long iFreq );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.11 Subcommand 11 - Set Gain State

         This function sets the gain state of the currently selected LNA.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iGainState = the desired gain state (0=highest gain state, 3=lowest gain state)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_SET_GAIN_STATE(HANDLE hResourceContext, unsigned char iGainState);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.12 Subcommand 12 - Pulse MBP TCXO_ON signal.

          This command allows the MBP TCXO_ON signal to be pulsed.  The pulse width is 100 microsecnds.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_TOGGLE_TCXO_ON( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.13 Subcommand 13 - Tune PLL

         This function tunes the PLL to the requested FLO Air Interface Specification channel.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iChannel = the desired channel (0 - 8191)

   \return true if successful, false if fail.

   \warning Supported in MBP1600/MBP1610 only.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_TUNE_PLL( HANDLE hResourceContext, unsigned short iChannel );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.14 Subcommand 14 - Load RSSI Cal Point

         Loads a RSSI Calibration point in to phone memory

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iChanFreq = for MBP1600/MBP1610: the desired channel (0 - 8191)
                  for MBP2600/MBP2700: the desired frequency in MHz
   \param iGainState = the phone gain state
   \param uCalPoint = the calibration point (dB/256)
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_LOAD_RSSI_CAL_POINT(HANDLE hResourceContext,
         unsigned short iChanFreq,
         unsigned char iGainState,
         short iCalPoint);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.15 Subcommand 16 - Set AGC Default

         Restores the AGC to the default state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_SET_AGC_DEFAULT(HANDLE hResourceContext);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.16 Subcommand 17 -RF Initialise

         Initialises the RF Mode, Bandwidth and Frequency

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRfMode = The RF mode to initialise.
   Defined by ftm_mflo_rf_mode in QLib_Defines.h:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iFreqKhz = The RF frequency to initialise in KHz
   \param iBandwidth = The RF bandwidth to use.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_RF_INITIALIZE(HANDLE hResourceContext,
         unsigned char iRfMode,
         unsigned long iFreqKhz,
         unsigned char iBandwidth);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.17 Subcommand 18 -Load AGC Cal Channel List

         Loads the AGC Calibration channel list to the phone memory.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iChannelsFreqs = MBP1600/MBP1610: List of up to 32 channel numbers to load to the phone
                     MBP2600/MBP2700: List of up to 32 frequencies in MHz to load to the phone
   \param iNumberOfChannelsFreqs = The number of channels/frequencies to load.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_LOAD_AGC_CAL_CHAN_LIST( HANDLE hResourceContext,
         unsigned short *iChannelsFreqs,
         unsigned short iNumberOfChannelsFreqs);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO UBM, 3.3.18 Subcommand 19 -Powerdown.

         This command turns off the RF power supply.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_POWER_DOWN_RF(HANDLE hResourceContext);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 2.1.3.16 Subcommand 20 - Set Demod Config Options.

      This command sets the demodulation configuration.
      For Pilot Pattern (0, 3, 6) the DMTT mode must obviously be set to TPC based.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFftSize = FFT Size
      Defined by ftm_mflo_fft_size:
      \code
      FTM_MFLO_2K_FFT = 0,
      FTM_MFLO_4K_FFT = 1,
      FTM_MFLO_8K_FFT = 2
      \endcode
   \param iCpLength = CP Length
      Defined by ftm_mflo_cp_length:
      \code
      FTM_MFLO_1_16_CP_LENGTH = 0,
      FTM_MFLO_1_8_CP_LENGTH = 1,
      FTM_MFLO_3_16_CP_LENGTH = 2,
      FTM_MFLO_1_4_CP_LENGTH = 3
      \endcode
   \param iPpc = 0 - No PPC Symbols, 1 - PPC Symbols
   \param iPilotPattern = 0 - (2,6), 1 - (0,3,6)
   \param iAdaptiveThresh = Adaptive Thresholding (0 - Enabled, 1 - Disabled)
   \param iIcMode = IC Mode
      Defined by ftm_mflo_ic_mode:
      \code
      FTM_MFLO_IC_DISABLED = 0,
      FTM_MFLO_IC_BYPASSED = 1,
      FTM_MFLO_IC_ENABLED = 2,
      FTM_MFLO_IC_DYNAMIC = 3
      \endcode
   \param iDmttMode = DMTT Mode
      Defined by ftm_mflo_dmtt_mode
      \code
      FTM_MFLO_DMTT_DISABLED = 0,
      FTM_MFLO_DMTT_LEGACY = 1,
      FTM_MFLO_DMTT_W_TDM2_RDSP_NOT_USED = 2,
      FTM_MFLO_DMTT_W_TDM2_RDSP_USED = 3,
      FTM_MFLO_DMTT_TPC_BASED = 4
      \endcode

   \return true if successful, false if fail.

   \warning Supported in MBP2600/MBP2700 only.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_UBM_MF_SET_DEMOD_CONFIG( HANDLE hResourceContext,
         unsigned char eFftSize,
         unsigned char eCpLength,
         unsigned char iPpc,
         unsigned char iPilotPattern,
         unsigned char iAdaptiveThresh,
         unsigned char eIcMode,
         unsigned char eDmttMode );

   /******************************************************************************
                     MediaFLO NS FTM
   *******************************************************************************/

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.1 Subcommand 0 - Enable FLO Demod

         This function enables FLO demodulation during FTM mode.
         The MediaFLO hardware must have been previously	initialized by the
         FTM MFLO Set Mode RF.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_SET_NS_MODE( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.2 Subcommand 1 - Acquire FLO System

         This function forces FLO to perform a FLO system acquisition. An attempt
         will be made to acquire both the WIDE and LOCAL systems.   Acquisition may
         require several seconds. The request message contains an additional field
         that instructs FTM to wait (in msec) before returning the system status in
         the response message.  If the wait time is 0 the response will be sent
         immediately.  In this case the FLO system may not be acquired when the
         response is sent.  The FTM_MF_GET_OIS_STATE command can be used to determine
         if a system has been acquired.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iacqWaitTime = time to wait for acquisition.
   \param bLocalAcquired = 1: Local System is valid (acquired)
                     0: Local System is invalid (not acquired)
   \param bWideAcquired = 1: Wide System is valid (acquired)
                     0: Wide System is invalid (not acquired)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_ACQUIRE_FLO_SYSTEM( HANDLE hResourceContext,
         unsigned short iAcqWaitTime,
         unsigned char *pbLocalAcquired,
         unsigned char *pbWideAcquired );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.3 Subcommand 2 - Activate Flow

         This function activates the Flow specified by the caller.
         Activation may require several seconds. The request message contains
         an additional field that instructs FTM to wait (in msec) before returning
         the activation response message.  If the wait time is 0 the response will
         be sent immediately and the ACTIVATION_STATUS in the response will be set
         to ACTIVATING_FLOW.  In addition the stream information returned in the
         response will be invalid (set to 0).  The FTM_MFLO_NS_GET_STREAM_INFO command
         can be used to determine if the Flow was successfully activated. If a wait
         time is specified the ACTIVATION_STATUS in the response will be set to either
         ACTIVATION_FAILED or ACTIVATION_SUCCESS depending upon the stream state at the
         wait time expiration.
         Non-signaling mode in FTM supports a maximum of 20 simultaneously active Flows.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAcqWaitTime = time to wait for acquisition.
   \param iFlowId = specfies the flow to activate.
   \param eBinding = specifies the binding type. This is both an input and output param
      Defined by mftypes_binding_type:
      \code
      MFLO_OVERHEAD   = 0x00,
      RT_VIDEO_CODEC  = 0x01,
      RT_AUDIO_CODEC  = 0x02,
      NRT_MEDIA       = 0x03,
      DATACAST        = 0x04,
      TAP             = 0x05,
      FIXED_TAP       = 0x06,
      \endcode
   \param eStatus = specifies activate flow status.
   Defined by mftypes_activate_status_type:
      \code
      FAILED				= 0x00,
      ACTIVATING			= 0x01,
      SUCCESS				= 0x02
      \endcode
   \param iMlcID = specifies the MLC ID.
   \param iStreamID = specifies the Stream ID.
   \param bStreamUsesBothLayers = true(1) if enhanced layer fata is present.
   \param bByteInterleaveEnabled = true(1) if byte interleave is enabled.
   \param eSystem = specifies the system.
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_ACTIVATE_FLOW( HANDLE hResourceContext,
         unsigned short iAcqWaitTime,
         unsigned long iFlowId,
         unsigned char *piBindingType,
         unsigned char *piStatus,
         unsigned char *piMlcId,
         unsigned char *piSteamId,
         unsigned char *pbStreamUsesBothLayers,
         unsigned char *pbByteInterleaveEnabled,
         unsigned char *piSystem );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.4 Subcommand 3 - Deactivate Flow

         This function deactivates the Flow specified by the caller. If the Flow
         Id in the request message is not active FLOW_NOT_ACTIVATED is returned
         in the response, otherwise FLOW_DEACTIVATING is returned.
         The FTM_MFLO_NS_GET_STREAM_INFO command can be used to determine if the Flow
         was successfully deactivated.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFlowId = specifies the Flow ID.
   \param eStatus = specifies deactivate flow status.
   Defined by mftypes_flow_status_type:
      \code
      FLOW_NOT_ACTIVATED  	= 0x00,
      FLOW_DEACTIVATING		= 0x02
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_DEACTIVATE_FLOW( HANDLE hResourceContext,
         unsigned long iFlowId,
         unsigned char *piStatus );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 1.3.5 Subcommand 4 - Get Stream Info

         This function command retrieves the stream information for the specified
         Flow ID.  If the Flow Id specified in the request message is not active
         FLOW_NOT_ACTIVATED is returned in the response message, otherwise
         FLOW_ACTIVATED is returned.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFlowId = specifies the Flow ID.
   \param eStatus = specifies deactivate flow status.
   Defined by mftypes_flow_status_type:
      \code
      FLOW_NOT_ACTIVATED		= 0x00,
      FLOW_ACTIVATED			= 0x01
      \endcode
   \param iMlcID = specifies the MLC ID.
   \param iStreamID = specifies the Stream ID.
   \param bStreamUsesBothLayers = true(1) if enhanced layer fata is present.
   \param bByteInterleaveEnabled = true(1) if byte interleave is enabled.
   \param peBindingType = specifies the binding type.
      Defined by mftypes_binding_type:
      \code
      MFLO_OVERHEAD   = 0x00,
      RT_VIDEO_CODEC  = 0x01,
      RT_AUDIO_CODEC  = 0x02,
      NRT_MEDIA       = 0x03,
      DATACAST        = 0x04,
      TAP             = 0x05,
      FIXED_TAP       = 0x06,
      \endcode
   \param eSystem = specifies the system.
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_STREAM_INFO( HANDLE hResourceContext,
         unsigned long iFlowId,
         unsigned char *piStatus,
         unsigned char *piMlcID,
         unsigned char *piStreamID,
         unsigned char *pbStreamUsesBothLayers,
         unsigned char *pbByteInterleaveEnabled,
         unsigned char *piBindingType,
         unsigned char *piSystem );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.6 Subcommand 5 - Get MLC Dynamic Parameters.

      This command retrieves the dynamic parameters for the specified MLC ID. The information
      returned can be used to calculate PER statistics. If the MLC is not active 0 is returned
      in the first byte of the response message, otherwise 1 is returned.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = specifies the MLC ID. This is both an input and output param.
   \param eSystem = specifies the system
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode
   \param piFFTAddress = MLC Base FFT Address
   \param peTransmitMode = Transmit Mode
      Defined by mftypes_trans_mode_type:
      \code
      REG_QPSK_1_3         = 0,
      MREG_QPSK_1_2        = 1,
      REG_16_QAM_1_3       = 2,
      REG_16_QAM_1_2       = 3,
      REG_16_QAM_2_3       = 4,
      REG_16_QPSK_1_5      = 5,
      LAY_QPSK_1_3_ER_4    = 6,
      LAY_QPSK_1_2_ER_4    = 7,
      LAY_QPSK_2_3_ER_4    = 8,
      LAY_QPSK_1_3_ER_6_25 = 9,
      LAY_QPSK_1_2_ER_6_25 = 10,
      LAY_QPSK_2_3_ER_6_25 = 11
      \endcode
   \param peRSOuterCode = RS Outer Code
      Defined by mftypes_reed_solomon_code_type:
      \code
      RS_16_16_0		= 0x0,
      RS_16_14_2		= 0x1,
      RS_16_12_4		= 0x2,
      RS_16_8_8		= 0x3
      \endcode
   \param piCodeBlocks = Code Blocks in current Superframe
   \param pbStream1Bound = true(1) if Stream 1 is bound
   \param pbStream2Bound = true(1) if Stream 2 is bound
   \param iStream0Length = Stream 0 length
   \param iStream1Length = Stream 1 length
   \param iStream2Length = Stream 2 length
   \param pBasePlpStats = structure of type mftypes_plp_stats_type to hold the Base PLP statistics
                           as defined in QLIB_Defines.h
   \param pEnhPlpStats = structure of type mftypes_plp_stats_type to hold the Enhanced PLP statistics
                           as defined in QLIB_Defines.h
   \param pBaseCbStats = structure of type mftypes_cb_stats_type to hold the Base CB statistics
                           as defined in QLIB_Defines.h
   \param pEnhCbStats = structure of type mftypes_cb_stats_type to hold the Enhanced CB statistics
                           as defined in QLIB_Defines.h

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pMlcPlpStats to store structure of type mftypes_mlc_plp_stats_type

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_MLC_DYN_PARAMS( HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char iSystem,
         unsigned char *piStatus,
         unsigned long *piFFTAddress,
         unsigned char *piTransmitMode,
         unsigned char *piRSOuterCode,
         unsigned char *piCodeBlocks,
         unsigned char *pbStream1Bound,
         unsigned char *pbStream2Bound,
         unsigned short *piStream0Length,
         unsigned short *piStream1Length,
         unsigned short *piStream2Length,
         void *pBasePlpStats,
         void *pEnhPlpStats,
         void *pBaseCbStats,
         void *pEnhCbStats );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.19 Subcommand 6 - Get OIS demodulation statistics.

      This command retrieves OIS demodulation statistics for the specified system.
      Both cumulative and statistics for both the most recent superframe are reported in the response.
      In order for valid data to be returned this command requires:
         The FTAP OIS Reception test is enabled for the specified system. See FTM_MFLO_NS_ENABLE_FTAP_OIS_RECEPTION_MODE.
         The MediaFLO Log packet 0x120E (MFLO FTAP WOIS PARAMS) or 0x1210 (MFLO FTAP LOIS PARAMS)

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iSystem = specifies the system
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode
   \param piLastNumGoodPlps = pointer to store good PLPs in last Superframe
   \param piLastNumErasuresPlps = pointer to store bad PLPs in last Superframe
   \param piTotalNumGoodPlps = pointer to store cumulative good PLPs
   \param piTotalNumErasures = pointer to store cumlative bad PLPs

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_OIS_STATS( HANDLE hResourceContext,
         unsigned char iSystem,
         unsigned long* piLastNumGoodPlps,
         unsigned long* piLastNumErasuresPlps,
         unsigned long* piTotalNumGoodPlps,
         unsigned long* piTotalNumErasures);

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.7 Subcommand 7 - Reset MLC PLP Stats.

      This command clears (resets to 0) the accumulated PLP/CB statistics for the specified MLC ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = specifies the MLC ID
   \param eSystem = specifies the system
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_RESET_MLC_PLP_STATS( HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char iSystem );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.8 Subcommand 8 - Reset ALL PLP Stats.

      This command clears (resets to 0) the accumulated PLP/CB statistics for all of the active MLCs.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_RESET_ALL_PLP_STATS( HANDLE hResourceContext );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.9 Subcommand 9 - Get the number of Active Flows.

      This command returns the number of currently active Flows. FTM Non-signaling mode
      supports a maximum of 20 simultaneously active Flows.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \param piFlowCount = the number of active flows

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_ACTIVE_FLOW_COUNT( HANDLE hResourceContext,
         unsigned char *piFlowCount );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.9 Subcommand 9 - Get all active flow Ids.

      This command returns a list of all of the currently active flows.
      The total number of active flows is also returned. The list of active flow IDs
      returned is formatted as a 20 entry by uint32 (4 byte) array (80 total bytes)
      Each array entry corresponds to an active Flow ID. If there are less than 20 active
      Flows (FTM Non-signaling mode supports a maximum of 20 simultaneously active flows)
      the array will be zero padded. Note that 0 is a valid Flow ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piFlowCount = This is both and input and output param.  As an input param
                       it indicates the number of elements in pActiveFlowInfoArray.
                       As an output param it indicates the actual number of active
                       flows in the system.
   \param pArrayFlowIds = array of all active flow ids. The list of active Flow IDs returned is
                  formatted as a 20 entry by uint32 (4 byte) array (64 total bytes). Each
                  array entry corresponds to an active Flow ID. If there are less than 16
                  active Flows (20 is maximum concurrent activated Flows in FTM non-signaling
                  mode) the array will be zero padded. Note that 0 is a valid Flow ID.

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pArrayFlowIds to store 20x4Byte array (80 total bytes).

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_ALL_ACTIVE_FLOW_IDS( HANDLE hResourceContext,
         unsigned char *piFlowCount,
         unsigned long *pArrayFlowIds );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.10 Subcommand 10 - Get control channel records.

      This command returns a list of control channel records for the specified system.
      A maximum of eight records are returned per response message. The record retrieval starts
      at the record number (greater than 0) specified in the request message. The total number
      of records for the specified system is returned.  The next record number in the record
      database is also returned. If the system contains more than eight records subsequent
      requests must be performed to retrieve all of the records. The total_records and next_record
      fields allow all of the records for a system to be retrieved via a series of
      FTM_MFLO_NS _GET_CONTROL_CHANNEL_RECORDS messages. For example, if the starting_record field
      in the request was set to 5 and the total_records field contains 15 in the response the
      next_record field in the response field will contain 13 (a maximum 8 records can be retrieved
      in one request). Since eight records are retrieved in each response the number of requests
      required to retrieve all of the records for a system is:
         ( TOTAL_RECORD )   mod 8
      The list of Control Channel records returned is formatted as an 8 entry (8 records) array of
      8 byte records (64 total bytes).  Each array entry corresponds to a Control Channel record.
      If there are less than 8 records the array will be zero padded.
      The Flow ID for a particular service is computed as follows using the Control Channel record:
         Flow ID = ( Service ID * 16 ) + Flow Descriptor

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSystem = the system
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode
   \param iStartRecord = the starting record for the next retrieval.
   \param eStatus = Record retrieval status
      0  = starting record count is invalid or no records are available for the specified system
      1 = records were retrieved
      Defined by mftypes_record_retrieval_status_type:
      \code
      NO_RECORDS_AVAILABLE	= 0x00,
      RECORDS_RETRIEVED	= 0x01
      \endcode
   \param iTotalRecords = The total number of records that are available for the specified system.
                  The iTotalRecords and iNextRecord allow all of the records for a system to
                  be retrieved via a series of calls to QLIB_MFLO_NS_GET_CONTROL_CHANNEL_RECORDS.
                  Since eight records are retrieved in each response the number of requests required
                  to retrieve all of the records for a system is: ( TOTAL_RECORD ) mod 8
   \param iNextRecord = the record number for the next available record for the system.
                  For example, if the iStartRecord param i was set to 5 and the iTotalRecord
                  param contains 15, then the iNextRecord will be 13 (maximum 8 records can
                  be retrieved in one request).
   \param pControlChannelRecordArray = Array[8] of mftypes_mf_control_channel_record structures
                           (mftypes_mf_control_channel_record is defined in QLib_Defines.h)

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pControlChannelRecordArray to store eight structures of type mftypes_mf_control_channel_record

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_CONTROL_CHANNEL_RECORDS( HANDLE hResourceContext,
         unsigned char eSystem,
         unsigned short iStartRecord,
         unsigned char *piStatus,
         unsigned short *piTotalRecords,
         unsigned short *piNextRecord,
         void *pControlChannelRecordArray );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.11 Subcommand 11 - Get AIS channel.

   This command returns the current AIS channel.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piChannel = Currently programmed AIS Channel.
   AIS channel to Khz conversion: Freq (Khz) = 470,000 + AIS_CHAN*50

   \return true if successful, false if fail.

   \warning Not supported on MBP2600/MBP2700

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_AIS_CHANNEL( HANDLE hResourceContext,
         unsigned short *piChannel );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.12 Subcommand 12 - Deactivate all flows.

   This command causes all flows to be deactivated.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_DEACTIVATE_ALL_FLOWS( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.13 Subcommand 13 - Tune Freq and BW.

   This command tunes the RF to the specified frequency and bandwidth.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFreqKhz = Frequency in kHz for MBP1600/MBP1610 or Hz for MBP2600/MBP2700.
   \param iBandwidthMHz = Bandwidth in MHz.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_TUNE_FREQ_AND_BW( HANDLE hResourceContext,
         unsigned long iFreq,
         unsigned char iBandwidthMHz );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 1.3.14 Subcommand 431 - Get RSSI.

   This command retrieves the RSSI. This value is not valid when the FLO is not actively decoding.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param bRssiValid = specifies validity of the RSSI measurement
   \param iRssi = Signed RSSI value in 1/100th dB resolution.

   \return true if successful, false if fail.

   \warning MBP1600/MBP1610 Only

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_RSSI( HANDLE hResourceContext,
         unsigned char *pbRssiValid,
         short *piRssi );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 1.3.15 Subcommand 15 - Set FLO Sleep.

   This command Enables/disables FLO sleep.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSleepSetting = indicates sleep setting.
      Defined by mftypes_sleep_settings_type:
      \code
      SLEEP_AND_SNOOZE_DISABLED		= 0x00,
      SLEEP_ENABLED_SNOOZE_DISABLED	= 0x01,
      SLEEP_DISABLED_SNOOZE_ENABLED	= 0x02,
      SLEEP_ENABLED_SNOOZE_ENABLED	= 0x03
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_SET_FLO_SLEEP( HANDLE hResourceContext,
         unsigned char iSleepSetting );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 1.3.16 Subcommand 433 - Get OIS State.

   This command returns the state of the WIde and Local OIS.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piWideOisStatus = Wide OIS Status.
      Defined by mftypes_state_type:
      \code
      INVALID_STATE		= 0x00,
      ACQUIRING_STATE		= 0x01,
      IDLE_STATE			= 0x02,
      ACTIVE_STATE		= 0x03,
      UNAVAILABLE_STATE	= 0x04
      \endcode
   \param piLocalOisStatus = Local OIS Status.
      Defined by mftypes_state_type:
      \code
      INVALID_STATE		= 0x00,
      ACQUIRING_STATE		= 0x01,
      IDLE_STATE			= 0x02,
      ACTIVE_STATE		= 0x03,
      UNAVAILABLE_STATE	= 0x04
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_OIS_STATE( HANDLE hResourceContext,
         unsigned char *piWideOisStatus,
         unsigned char *piLocalOisStatus );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.17 Subcommand 17 - Intialize RF.

   This command intializes the MediaFLO driver for FTM Mode. This command must be sent prior to
   sending the FTM_MFLO_NS_SET_MODE command.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eRfMode = MediaFLO RF Mode
      Defined by ftm_mflo_rf_mode:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iFreqKhz = Channel frequency in kHz
   \param eBandwidth = Channel bandwidth
      Defined by ftm_mflo_bandwidth:
      \code
      FTM_MFLO_5_MHZ_BW	= 5,
      FTM_MFLO_6_MHZ_BW	= 6,
      FTM_MFLO_7_MHZ_BW	= 7,
      FTM_MFLO_8_MHZ_BW	= 8
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_INITIALIZE_RF( HANDLE hResourceContext,
         unsigned char eRfMode,
         unsigned long iFreqKhz,
         unsigned char eBandwidth );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.18 Subcommand 18 - Set Config Options.

      This command sets the demodulation configuration. This command only applies to the MBPxxxxx.
      For Pilot Pattern (0, 3, 6) the DMTT mode must be set to TPC based.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eFftSize = FFT Size
      Defined by ftm_mflo_fft_size:
      \code
      FTM_MFLO_2K_FFT = 0,
      FTM_MFLO_4K_FFT = 1,
      FTM_MFLO_8K_FFT = 2
      \endcode
   \param eCpLength = CP Length
      Defined by ftm_mflo_cp_length:
      \code
      FTM_MFLO_1_16_CP_LENGTH = 0,
      FTM_MFLO_1_8_CP_LENGTH = 1,
      FTM_MFLO_3_16_CP_LENGTH = 2,
      FTM_MFLO_1_4_CP_LENGTH = 3
      \endcode
   \param iPpc = 0 - No PPC Symbols, 1 - PPC Symbols
   \param iPilotPattern = 0 - (2,6), 1 - (0,3,6)
   \param iAdaptiveThresh = Adaptive Thresholding (0 - Enabled, 1 - Disabled)
   \param eIcMode = IC Mode
      Defined by ftm_mflo_ic_mode:
      \code
      FTM_MFLO_IC_DISABLED = 0,
      FTM_MFLO_IC_BYPASSED = 1,
      FTM_MFLO_IC_ENABLED = 2,
      FTM_MFLO_IC_DYNAMIC = 3
      \endcode
   \param eDmttMode = DMTT Mode
      Defined by ftm_mflo_dmtt_mode
      \code
      FTM_MFLO_DMTT_DISABLED = 0,
      FTM_MFLO_DMTT_LEGACY = 1,
      FTM_MFLO_DMTT_W_TDM2_RDSP_NOT_USED = 2,
      FTM_MFLO_DMTT_W_TDM2_RDSP_USED = 3,
      FTM_MFLO_DMTT_TPC_BASED = 4
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_SET_CONFIG_OPTIONS( HANDLE hResourceContext,
         unsigned char eFftSize,
         unsigned char eCpLength,
         unsigned char iPpc,
         unsigned char iPilotPattern,
         unsigned char iAdaptiveThresh,
         unsigned char eIcMode,
         unsigned char eDmttMode );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.20 Subcommand 19 - Reset OIS Stats.

      This command resets the OIS statistics for the specified system.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSystem = System
      Defined by mftypes_system_type in QLib_Defines.h:
      \code
      WIDE = 0x00,	//! Wide System
      LOCAL = 0x01,	//! Local System
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_RESET_OIS_STATS( HANDLE hResourceContext,
         unsigned char eSystem );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.21 Subcommand 20 - Get MLC Dynamic Parameters.

      This command requests MLC Dynamic Parameters for the specified MLC.
      This command requires enabling the Log Code (0x1214) MFLO MLC LOCATION DYNAMIC PARAMS.
      If this log code is not enabled, the response to this command is undefined.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcId = The MLC ID
   \param eSystem = System
      Defined by mftypes_system_type in QLib_Defines.h:
      \code
      WIDE = 0x00,	//! Wide System
      LOCAL = 0x01,	//! Local System
      \endcode
   \param piMlcId = Pointer to store the MLC ID
   \param piMlcFftMode = Pointer to store the MLC FFT mode
   \param piSfNumber = Pointer to store the last Superframe number
   \param piMlcStartSymbol = Pointer to store the MLC Start Symbol
   \param piMlcSlotOffset = Pointer to store the MLC Slot Offset
   \param piMlcSlotMin = Pointer to store the MLC Slot Minimum
   \param piMlcSlotTotal = Pointer to store the MLC Total Slots
   \param piMlcSlotMax = Pointer to store the MLC Slot Maximum
   \param piNumCbs = Pointer to store the number of Code Blocks in the last Superframe
   \param peTransMode = Pointer to store the MLC Transmit Mode
      Defined by mftypes_trans_mode_type in QLib_Defines.h
      \code
      REG_QPSK_1_3         = 0,
      MREG_QPSK_1_2        = 1,
      REG_16_QAM_1_3       = 2,
      REG_16_QAM_1_2       = 3,
      REG_16_QAM_2_3       = 4,
      REG_16_QPSK_1_5      = 5,
      LAY_QPSK_1_3_ER_4    = 6,
      LAY_QPSK_1_2_ER_4    = 7,
      LAY_QPSK_2_3_ER_4    = 8,
      LAY_QPSK_1_3_ER_6_25 = 9,
      LAY_QPSK_1_2_ER_6_25 = 10,
      LAY_QPSK_2_3_ER_6_25 = 11
      \endcode
   \param peOuterCode = Pointer to store the Outer Code used for the MLC
      Defined by ftm_mflo_mlc_outer_code in QLib_Defines.h
      \code
         FTM_MFLO_NS_OUTER_CODE_NONE = 0,
         FTM_MFLO_NS_OUTER_CODE_RS_7_8 = 1,
         FTM_MFLO_NS_OUTER_CODE_RS_3_4 = 2,
         FTM_MFLO_NS_OUTER_CODE_RS_1_2 = 3,
         FTM_MFLO_NS_OUTER_CODE_REP_4 = 4
      \endcode
   \param piStream1Bound = Pointer to store the "is Stream 1 Bound" flag
      If it is not being decoded, this field is set to FALSE; otherwise, this will be set to TRUE.
   \param piStream2Bound = Pointer to store the "is Stream 2 Bound" flag
      If it is not being decoded, this field is set to FALSE; otherwise, this will be set to TRUE.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_MLC_DYN_PARAMS_LOG( HANDLE hResourceContext,
         unsigned char iMlcId,
         unsigned char eSystem,
         unsigned char* piMlcId,
         unsigned char* piMlcFftMode,
         unsigned long* piSfNumber,
         unsigned short* piMlcStartSymbol,
         unsigned char* piMlcSlotOffset,
         unsigned char* piMlcSlotMin,
         unsigned long* piMlcSlotTotal,
         unsigned char* piMlcSlotMax,
         unsigned char* piNumCbs,
         unsigned char* peTransMode,
         unsigned char* peOuterCode,
         unsigned char* piStream1Bound,
         unsigned char* piStream2Bound );

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 3.3.22 Subcommand 21 - Get Flow PLP Dynamic Parameters.

      This command requests the PLP Statistics based on a Flow ID.
      The response contains both the total (cumulative) PLP statistics and the PLP statistics
      from the most recent superframe.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFlowId = The Flow ID
   \param piTotalNumGoodPlps = Pointer to store the cumulative number of good PLPs
   \param piTotalNumErasuresPlps = Pointer to store the cumulative number of bad PLPs
   \param piPerSfGoodPlps = Pointer to store the number of good PLPs in the most recent superframe
   \param piPerSfErasurePlps = Pointer to store the number of bad PLPs in the most recent superframe

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_FLOW_PLP_DYN_PARAMS( HANDLE hResourceContext,
         unsigned long iFlowId,
         unsigned long* piTotalNumGoodPlps,
         unsigned long* piTotalNumErasuresPlps,
         unsigned long* piPerSfGoodPlps,
         unsigned long* piPerSfErasurePlps );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.23 Subcommand 22 - Enable FTAP PLP Data Logging

      This command starts/stops FTAP PLP Data Packet Records Logging.
      The FTAP PLP data records for all the active FTAP Flows will be logged each superframe.
      This command requires that the following log packets are enabled:
      1)	MFLO FTAP PACKETRECORD HEADER PARAMS (0x120C)
      2)	MFLO FTAP PACKETRECORD PARAMS (0x120D)

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFtapEnable = 0 - Disable FTAP Logging, 1 - Enable FTAP Logging.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_ENABLE_FTAP_PLP_DATA( HANDLE hResourceContext,
         unsigned char iFtapEnable );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.24 Subcommand 23 - Enable FTAP OIS Reception Mode

      This command starts/stops FTAP OIS RECEPTION Test mode.
      When the device is set in this mode, OIS will be acquired every superframe (either WOIS/LOIS or BOTH)
      until it is turned off by the same command.
      Once in this mode, the FDS starts logging WOIS/LOIS information
      (log codes 0x120E (MFLO FTAP WOIS PARAMS) or 0x1210 (MFLO FTAP LOIS PARAMS)) once every superframe.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param eOisTestMode = OIS Test Mode.
      Defined by ftm_mflo_ois_test_mode_type in QLib_Defines.h
      \code
         FTM_MFLO_NS_ENABLE_WOIS_ONLY = 0x0,
         FTM_MFLO_NS_ENABLE_LOIS_ONLY = 0x1,
         FTM_MFLO_NS_ENABLE_WOIS_AND_LOIS = 0x2,
         FTM_MFLO_NS_DISABLE_WOIS_AND_LOIS = 0x3
      \endcode

   \return true if successful, false if fail.

   \warning Enabling OIS reception while demodulating MLCs will result in undefined behavior on the device.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_ENABLE_FTAP_OIS_RECEPTION_MODE( HANDLE hResourceContext,
         unsigned char eOisTestMode );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.25 Subcommand 24 - Enable WICLIC Reception Mode

      This command enables/disables FTAP WICLIC RECEPTION Test mode.
      When FTAP WICLIC RECEPTION mode is enabled the WIC/LIC symbols will be decoded every superframe until
      FTAP WICLIC RECEPTION Test mode is disabled.
      When enabled, the WID/LID parameters are logged once every superframe (log code 0x1210 (MFLO FTAP WID LID Params) ).
      Note: FTAP OIS RECEPTION Test mode (see FTM_MFLO_NS_ENABLE_FTAP_OIS_RECEPTION_MODE (23) )
            must first be enabled before enabling FTAP WIC/LIC Reception mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iWicLicMode = 0 - Disable WIC/LIC Reception Mode, 1 - Enable WIC/LIC Reception Mode.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_ENABLE_FTAP_WICLIC_RECEPTION_MODE( HANDLE hResourceContext,
         unsigned char iWicLicMode );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.26 Subcommand 25 - Set Jammer Detection Mode

      This command configures the Jammer Detection Mode.
      Jammer Detect functionality is only supported by MBP2700.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iJDMode = Jammer Detect Mode
      Defined by mftypes_jd_mode_type:
      \code
      MFLO_JD_MODE_AUTO = 0,
      MFLO_JD_MODE_1    = 1,
      MFLO_JD_MODE_2    = 2
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_SET_JAMMER_DETECT_MODE( HANDLE hResourceContext,
         unsigned char iJDMode );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.26 Subcommand 26 - Freeze the AGC gain state

      This command freezes the AGC gain state at the current gain state.
      There may be a delay of up to 500ms after the command is sent before the gain state is frozen.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFreezeState = 0 - Unfreeze, 1 - Freeze

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_FREEZE_GAIN_STATE( HANDLE hResourceContext,
         unsigned char iFreezeState );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.27 Subcommand 27 - Enable the AFC outer loop

      This command freezes the AFC outer loop.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iEnable = 0 - Disable, 1 - Enable

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_ENABLE_AFC_OUTER_LOOP( HANDLE hResourceContext,
         unsigned char iEnable );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.28 Subcommand 28 - Disable the AGC Gain Tracking

      This command disables the AGC Gain Tracking.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iDisable = 0 - Enable, 1 - Disable

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_DISABLE_AGC_GAIN_TRACKING( HANDLE hResourceContext,
         unsigned char iDisable );

   /******************************************************************************/
   /**

   Factory Test Mode MediaFLO Non-Signalling, 3.3.29 Subcommand 29 - Get the Jammer Detection State

      This command gets the current Jammer Detection state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piJDMode = Jammer Detect Mode
      Defined by mftypes_jd_mode_type:
      \code
      MFLO_JD_MODE_AUTO = 0,
      MFLO_JD_MODE_1    = 1,
      MFLO_JD_MODE_2    = 2
      \endcode
   \param piJDState = Jammer Detect State
      Defined by mftypes_jd_state_type:
      \code
      MFLO_JD_STATE_INVALID      = 0,
      MFLO_JD_STATE_MODE_1       = 1,
      MFLO_JD_STATE_MODE_2       = 2,
      MFLO_JD_STATE_INITIALIZING = 3
      \endcode
   \param piJDSubstate = Jammer Detect Substate
      Defined by mftypes_jd_substate_type:
      \code
      MFLO_JD_SUBSTATE_NONE    = 0,
      MFLO_JD_SUBSTATE_POLLING = 1
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_JAMMER_DETECT_STATE( HANDLE hResourceContext,
         unsigned char* piJDMode,
         unsigned char* piJDState,
         unsigned char* piJDSubstate);

   /******************************************************************************/
   /**
   Factory Test Mode MediaFLO Non-Signalling, 1.3.14 Subcommand 30 - Get RSSI and Gain State.

   This command retrieves the RSSI and Gain State. This value is not valid when the FLO is not actively decoding.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iGainState = the current gain state
   \param iRssi = Signed RSSI value in 1/100th dB resolution.
   \param bRssiValid = specifies validity of the RSSI measurement

   \return true if successful, false if fail.

   \warning MBP2700 Only

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_RSSI_AND_GAIN_STATE( HANDLE hResourceContext,
         unsigned char *piGainState,
         short *piRssi,
         unsigned char *pbRssiValid );

   /******************************************************************************/
   /**
   MFLO get active flow Info

   This command gets information regarding all active flows in the system.

   Calls QLIB_FTM_MFLO_NS_GET_ALL_ACTIVE_FLOW_IDS then calls QLIB_FTM_MFLO_NS_GET_STREAM_INFO
   for each active flow.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piActiveFlowCount = This is both and input and output param.  As an input param
                       it indicates the number of elements in pActiveFlowInfoArray.
                       As an output param it indicates the actual number of active
                       flows in the system.
   \param pActiveFlowInfoArray = Array of mftypes_active_flow_info structures
                           (mftypes_active_flow_info is defined in QLib_Defines.h)

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pActiveFlowInfoArray to	store information about all active flows in
          the system.  The required amount of space can be determined by first calling
          QLIB_MFLO_GetBoundFlowCount.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_ACTIVE_FLOW_INFO_LIST( HANDLE hResourceContext,
         unsigned char* piActiveFlowCount,
         void* pActiveFlowInfoArray );

   /******************************************************************************/
   /**
   This function measures the Physical layer packet error rate.  This type of packet
   error rate measurement does not require a known nor finite sequence of FTAP packets
   from the FLO Tx Waveform.  The measurement is made by querying the accumulated PLP
   statistics for all requested Flow IDs.  Accumulated statistics continue to be
   queried until the minimum number of PLPs have been recorded.  Accumulated PLP
   statistics are queried through the FTM_MFLO_NS_GET_MLC_DYN_PARAMS function.

   After calling FTM_MFLO_NS_RESET_ALL_PLP_STATS to reset the accumulated statistics for all
   MLCs, this function may be called repeatedly until the desired level of confidence
   is achieved. To measure the packet error rate again, call FTM_MFLO_NS_RESET_ALL_PLP_STATS again
   before calling this function.

   Usage examples:

      FTM_MFLO_NS_RESET_ALL_PLP_STATS
      FTM_MFLO_NS_GetPER_Phy		--------------	Measure PER for first test


      FTM_MFLO_NS_RESET_ALL_PLP_STATS	---------
      FTM_MFLO_NS_GetPER_Phy					 |
      FTM_MFLO_NS_GetPER_Phy					 |-----  Measure PER for second test
      FTM_MFLO_NS_GetPER_Phy					 |		Note: Example shows measurement ran longer
      FTM_MFLO_NS_GetPER_Phy			---------		      to achieve desired level of confidence

   This function reports the packet error rate statistics for all requested Flow IDs,
   the current system state, and an error code.

   \param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs
   \param aiFlowIDs = array of Flow IDs for which to get accumulated PLP statistics
   \param aiMlcIDs = array of MLC IDs that correspond to the Flow IDs in aiFlowIDs,
                     this is required because MFLO_GET_MLC_PLP_STAT_DYN_PARAMS requires
                 the MLC ID not Flow ID
   \param aeSystem = array of mftypes_system_type with elements that correspond to the
                     Flow IDs in aiFlowIDs, the System for each Flow ID is required by
                 QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS
   \param iNoStatsTimeout = time in seconds to wait for statistics to update, max of 5
                      seconds recommended
   \param ePerToMeas = which packet error rate to measure, statistics for both Base and
                       Enhancement layers will be reported but the determination of
                  whether or not iMinPLPs have been measured depends on what this
                  variable has been set to (mftypes_per_to_measure)
   \param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
   \param eRS = which statistics to report, Pre or Post Reed Solomon (mftypes_reed_solomon)
   \param paOverallStats = an array of type mftypes_per_statistics with length
                     iNumberOfFlowIDs to store the packet error rate statistics for
                     all requested Flow IDs
   \param peWideAreaSystemState = the current wide area system state if one of the
                           elements in aeSystem is WIDE (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state if one of the
                                   elements in aeSystem is LOCAL (mftypes_state_type)
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed, see the warning below for one
                   exception (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array.  Sufficient memory for pOverallStats must be allocated by the
          calling program.

          The results reported in paOverallStats are only valid if...
            FTM_MFLO_NS_GetPER_Phy return value is true		AND
            Requested system state(s) == ACTIVE_STATE	AND
             peErrorCode == NOT_AVAILABLE.

          If at some point during the measurement, the requested system state is not
          ACTIVE_STATE,  the function will return but not fail.  The current system
          state will be reported through peWideAreaSystemState and peLocalAreaSystemState.

          If the statistics for one or more of the MLC IDs in aiMlcIDs have not updated
          after the number of seconds specified by iNoStatsTimeout have elapsed, the
          function will return but not fail. peErrorCode will be set to PLP_STATS_NOT_UPDATING.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GetPER_Phy( HANDLE hResourceContext,
         unsigned char iNumberOfFlowIDs,
         unsigned long* aiFlowIDs,
         unsigned char* aiMlcIDs,
         unsigned long* aeSystem,
         unsigned short iNoStatsTimeout,
         unsigned long ePerToMeas,
         unsigned long iMinPLPs,
         unsigned long eRS,
         void* paOverallStats,
         unsigned long* peWideAreaSystemState,
         unsigned long* peLocalAreaSystemState,
         unsigned long* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls several of the MediaFLO FTM
   API's to perform a specialized task.

   This function performs initialization steps required before making a new
   Physical layer + MAC layer packet error rate measurement.  This type of packet
   error rate measurement requires a known and finite sequence of FTAP packets from
   the FLO Tx Waveform.  This function resets the overall packet error rate statistics,
   updates data structures used in the packet error rate measurement, clears the log
   queue, registers the following log codes:

         LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
         LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

   and enables FTAP PLP Data Logging Mode by through QLIB_FTM_MFLO_NS_ENABLE_FTAP_PLP_DATA.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs, must match the actual
                       number of active FlowIDs or this funtion will fail
   \param aiFlowIDs = array of Flow IDs for which to measure the packet error rate
   \param paOverallStats = an array of type mftypes_per_statistics with length
                     iNumberOfFlowIDs to store the packet error rate
                     statistics for all requested Flow IDs
   \param peErrorCode = an error code used whenever possible to provide more
                   information about why the function failed (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array. Sufficient memory for pOverallStats must be allocated by the
          calling program.

          QLIB_MFLO_InitTAPMsgContent_PhyMAC must be called once before the first
          time this function is called.

          This function must be called once before calling QLIB_FTM_MFLO_NS_GetPER_PhyMAC.
          The function QLIB_FTM_MFLO_NS_Stop_PhyMAC should be called at the end of the packet
          error rate measurement. To make a new packet error rate measurement,
          this function must be called again before calling QLIB_FTM_MFLO_NS_GetPER_PhyMAC.
          See QLIB_FTM_MFLO_NS_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_Start_PhyMAC( HANDLE hResourceContext,
         unsigned char iNumberOfFlowIDs,
         unsigned long* aiFlowIDs,
         void* paOverallStats,
         unsigned long* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function measures the Physical layer + MAC layer packet error rate. This
   type of packet error rate measurement requires a known and finite sequence of
   FTAP packets from the FLO Tx Waveform.

   The measurement is made by getting erasure and FTAP Sequence Number /
   FTAP Test Signature information from log packets with the following log codes:

      LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
      LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

   FTAP Sequence Number / FTAP Test Signature errors are determined by comparing
   the values reported in the log packets to the expected values that were set
   by QLIB_MFLO_InitTAPMsgContent_PhyMAC. Missing or mismatching pairs in each
   superframe are counted as errors.

   Total errors include erasures and FTAP Sequence Number / FTAP Test Signature errors.

   This function continues parsing log packets and keeping track of overall packet
   error rate statistics until the minimum number of PLPs have been measured for all
   requested Flow IDs.

   After calling QLIB_MFLO_Start_PhyMAC, this function may be called repeatedly until
   desired level of confidence is achieved. Once the current measurement is complete,
   call QLIB_MFLO_Stop_PhyMAC.  To measure the packet error rate again, call
   QLIB_MFLO_Start_PhyMAC again before calling this function.

   Usage example:

      QLIB_MFLO_InitTAPMsgContent_PhyMAC	-------- Called once at start of session

      QLIB_FTM_MFLO_NS_Start_PhyMAC		---------
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|-----	Measure PER for first test
      QLIB_FTM_MFLO_NS_Stop_PhyMAC		---------

      QLIB_FTM_MFLO_NS_Start_PhyMAC		---------
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|-----  Measure PER for second test
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|		Note: Example shows measurement
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|		      ran longer to achieve
      QLIB_FTM_MFLO_NS_Stop_PhyMAC		---------             desired confidence

      QLIB_MFLO_FinalCleanup_PhyMAC		-------- Called once at end of session

   This function reports the packet error rate statistics for all requested Flow IDs,
   the current system state, and an error code.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNoLogPktsTimeout = time in seconds to wait for log packets
   \param ePerToMeas = which packet error rate to measure, statistics for both Base
                  and Enhancement layers will be reported but the determination of
                  whether or not iMinPLPs have been measured depends on what this
                  variable has been set to (mftypes_per_to_measure)
   \param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
   \param eSystem = the system, required for getting the system state (mftypes_system_type)
   \param eErasureIndicator = which erasure indicator to use when counting erasures
                              (mftypes_reed_solomon)
   \param paOverallStats = an array of type mftypes_per_statistics to store the packet
                     error rate statistics for all requested Flow IDs
   \param peWideAreaSystemState = the current wide area system state if eSystem was set to
                                  WIDE or WIDE_AND_LOCAL (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state if eSystem was set to
                                   LOCAL or WIDE_AND_LOCAL (mftypes_state_type)
   \param peErrorCode = an error code used whenever possible to provide more information
                   about why the function failed, see the warning below for two
                   exceptions (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array. Sufficient memory for pOverallStats must be allocated by the
          calling program.

          The results reported in paOverallStats are only valid if
            QLIB_FTM_MFLO_NS_GetPER_PhyMAC return value is true		AND
            Requested system state(s) == ACTIVE_STATE				AND
            peErrorCode == NOT_AVAILABLE.

          If at some point during the measurement, the system state is not ACTIVE_STATE,
          the function will return but not fail. The current system state will be reported
          through peWideAreaSystemState and peLocalAreaSystemState. At this point
          QLIB_FTM_MFLO_NS_Stop_PhyMAC should be called.

          If no log packets are received after the number of seconds specified by
          iNoLogPktsTimeout have elapsed, the function will return but not fail and
          peErrorCode will be set to NO_LOG_PACKETS. At this point QLIB_FTM_MFLO_NS_Stop_PhyMAC
          should be called.

         The time period that is used when checking for status is controlled by the
         timing index QMSL_Timeout_Delay_MediaFLO_StatusCheck, to be used with the functions
         QLIB_ConfigureTimeOut() and QLIB_GetTimeOut().

          This function depends on receiving all packets for all requested Flow IDs in
          an expected order. If any expected log packets are not received, the function
          will return but not fail and peErrorCode will be set to MISSING_LOG_PACKETS.
          At this point QLIB_FTM_MFLO_NS_Stop_PhyMAC should be called.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GetPER_PhyMAC( HANDLE hResourceContext,
         unsigned short iNoLogPktsTimeout,
         unsigned long ePerToMeas,
         unsigned long iMinPLPs,
         unsigned long eSystem,
         unsigned long eErasureIndicator,
         void* paOverallStats,
         unsigned long* peWideAreaSystemState,
         unsigned long* peLocalAreaSystemState,
         unsigned long* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function stops the current Physical layer + MAC layer packet error rate
   measurement. It disables FTAP PLP Data Logging Mode, clears all log codes for
   LOG_EQUIP_ID_1X, and clears the log queue.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning This function should be called at the end of a packet error rate
          measurement after all calls to QLIB_FTM_MFLO_NS_GetPER_PhyMAC for the current
          measurement are complete. See QLIB_FTM_MFLO_NS_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_Stop_PhyMAC( HANDLE hResourceContext );

   /******************************************************************************
                     DVB-H Diag Commands
   *******************************************************************************/

   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.1 Subcommand 0 -Layer 1 Acquire Command

         This command tells DVB-H L1 to acquire on the given frequency, mode and guard.
         DVB-H L1 must be powered up via QLIB_DIAG_DVBH_UBM_L1_POWERUP_CMD()	before this command is sent.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param	iMode = Mode to acquire on
      Defined by ubm_dvbh_mode in QLib_Defines.h:
      \code
      UBM_L1_MODE_1	= 1,
      UBM_L1_MODE_2	= 2,
      UBM_L1_MODE_3	= 3,
      UBM_L1_MODE_ALL	= 255,
      \endcode
   \param iGuard = Signal Guard interval to use.
      Defined by ubm_dvbh_guard in QLib_Defines.h:
      \code
      UBM_L1_GUARD_4		= 0,
      UBM_L1_GUARD_8		= 1,
      UBM_L1_GUARD_16		= 2,
      UBM_L1_GUARD_32		= 3,
      UBM_L1_GUARD_ALL	= 255
      \endcode
   \param iFrequency = Frequency to acquire. Value in Hz/10
   \param iBandwidth = The bandwidth to acquire to.
      Defined by ubm_dvbh_bandwidth in QLib_Defines.h:
      \code
      UBM_L1_BW_5		=5,
      UBM_L1_BW_6		=6,
      UBM_L1_BW_7		=7,
      UBM_L1_BW_8		=8,
      \endcode
   \param iPriority = Sets the acquire priority, 0 for low, 1 for high.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning  Must call QLIB_DIAG_DVBH_UBM_L1_POWERUP_CMD() first.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_ACQ_CMD(HANDLE  hResourceContext,
         unsigned char iMode,
         unsigned char iGuard,
         unsigned long iFrequency,
         unsigned char iBandwidth,
         unsigned char iPriority);
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.2 Subcommand 1 - Layer 1 Configure demodulation of a control PID.

         This command configures the demodulation of a control pid.
         L1 must be powered up and acquisition must have occurred before this command can be issued.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param	iPid = The control ID to configure.
   \param iAdd_rem_ind = Select add (0) or remove (1) PID.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning  L1 must be powered up and acquisition must have occurred before this command can be issued.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_CFG_CTRL_PID_CMD(HANDLE  hResourceContext,
         unsigned short iPid,
         unsigned char  iAdd_rem_ind);
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.3 Subcommand 2 - Layer 1 Configure demodulation of a data PID.

         This command configures the demodulation of a data pid.
         L1 must be powered up and acquisition must have occurred before this command can be issued.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param	iPid = The program ID to configure.
   \param iAdd_rem_ind = Select add (0) or remove (1) PID.
   \param iFrame_size = A frame size of between 0 and 3 (frames) for a given pid.
   \param iMpe_fec = enable mpe_fed, 0 if mpe_fec is disabled, 1 for mpe_fec enabled.
   \param iMax_burst_dur = Max burst duration in ms.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning  L1 must be powered up and acquisition must have occurred before this command can be issued.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_CFG_DATA_PID_CMD(HANDLE  hResourceContext,
         unsigned short iPid,
         unsigned char  iAdd_rem_ind,
         unsigned char iFrame_size,
         unsigned char iMpe_fec,
         unsigned short iMax_burst_dur);
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.6 Subcommand 5 - Layer 1 Set or read L1 config item.

         This command can set or read a given L1 config item


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  iGet_set_item = Get/Set the item. 0 sets item to a value. 1 to gets the item value.
   \param	iItem = configuration item to set/get.
      Defined by ubm_dvbh_configuration_items in QLib_Defines.h:
      \code
      UBM_L1_CFG_EBI2_WS		=0,
      UBM_L1_CFG_USE_PLLS		=1,
      UBM_L1_CFG_CTRL_PID_EXT_MODE	=2,
      UBM_L1_CFG_CTRL_PID_EBI2_MODE	=3,
      UBM_L1_CFG_PAGE_MODE		=4,
      UBM_L1_CFG_USE_MODE2_CPCE	=5,
      UBM_L1_CFG_DEBUG		=6,
      UBM_L1_HANDLE_L3_ACK		=7,
      UBM_L1_CFG_USE_ACQ_TIMEOUT	=8,
      UBM_L1_CFG_SNOOZE		=9,
      UBM_L1_CFG_BW			=11,
      UBM_L1_RX_FRONT_WARMUP		=12,
      UBM_L1_CHANGE_TCXO_PDM		=13,
      UBM_L1_MAP_ADDR_12_18		=14,
      UBM_L1_FAP_LAP_CORR_MASK	=15,
      UBM_L1_SIGNAL_MONITOR		=16,
      UBM_L1_CFG_INPUT_MODE		=17,
      UBM_L1_CFG_RDSP_CLK_SRC		=18,
      \endcode
   \param iValue = value for configuration item.
   \param	piItem = pointer where the configuration item that was set is stored.
      Defined by ubm_dvbh_configuration_items in QLib_Defines.h:
   \param piValue = pointer to location to store the value of configuration item.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_CFG_ITEM (HANDLE  hResourceContext,
         unsigned char iGet_set_item,
         unsigned char  iItem,
         unsigned long iValue,
         unsigned char  *piItem,
         unsigned long *piValue);
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.8 Subcommand 7 - Layer 1 Get acquisition status

         This function returns the acquisition state of DVB-H L1


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param	piAcquisition_sucess = pointer to the location to store the acquestion status
                           0 indicates  acquisition failed (mode and guard invalid)
                           1 indicates acquisition succeeded
   \param  piMode = Pointer to location to store the mode of the acquired signal.
      Defined by ubm_dvbh_mode in QLib_Defines.h:
      \code
      UBM_L1_MODE_1	= 1,
      UBM_L1_MODE_2	= 2,
      UBM_L1_MODE_3	= 3,
      \endcode
   \param	piGuard = Pointer to location to store the guard of the acquired signal.
      Defined by ubm_dvbh_guard in QLib_Defines.h:
      \code
      UBM_L1_GUARD_4		=0,
      UBM_L1_GUARD_8		=1,
      UBM_L1_GUARD_16		=2,
      UBM_L1_GUARD_32		=3,
      \endcode

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_ACQ_STATUS  (HANDLE  hResourceContext,
         unsigned char *piAcquisition_sucess,
         unsigned char  *piMode,
         unsigned char  *piGuard );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.9 Subcommand 8 - Layer 1 Sets up the PRBS one bit error test.

         The test box is set up to transmit a packet containing all 0 bits.
         This command sets up a hardware block within the MPE-FEC decoder block to count
         the number of bits set to 1 between the two packet counts.
         This count can be read back using the UBM_L1_PRBS_ONE_BIT_STATUS() (3.3.10) command.



   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param	iLength = Number of packets to run the test over
   \param  iThreshold = Threshold to determine good vs bad sample. If set to 0 then the default is used.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_PRBS_ONE_BIT_CMD  (HANDLE  hResourceContext,
         unsigned long  iLength,
         unsigned long  iThreshold );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.10 Subcommand 9 - Layer 1 Gets the results from the 1 bit prbs test.

         This command returns the number of 1 bits counted in the PRBS one bit test (3.3.9).


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer().
   \param piTest_done		= pointer to store test Status, 0 indicates the test is not finished 1 = test finished .
   \param piOnes_cnt		= pointer to store the number of ones counted in the test.
   \param piGood_pkt_cnt	= pointer to store the number of good packets.
                        (should be equal to length given in UBM_L1_PRBS_ONE_BIT_CMD if the test is finished).
   \param piBad_pkt_cnt	= pointer to store the number of packets thrown away due to sampling issues.
   \param piMax_sample_pkt_cnt	= Max number of packets used for one sample.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_PRBS_ONE_BIT_STATUS  (HANDLE  hResourceContext,
         unsigned char *piTest_done,
         unsigned long *piOnes_cnt,
         unsigned long *piGood_pkt_cnt,
         unsigned long *piBad_pkt_cnt,
         unsigned long *piMax_sample_pkt_cnt );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.11 Subcommand 10 -This command powers up the MBP receiver and initializes DVB-H L1.

         This command should be called before any other DVB-H L1 non-signaling commands.


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_POWERUP_CMD  (HANDLE  hResourceContext );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.12 Subcommand 11 -This command powers down the MBP receiver.

         This command should be called when exiting DVBH non-signalling mode.


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_POWERDOWN_CMD  (HANDLE  hResourceContext );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.13 Subcommand 13 -  Wake up UBM.

         This command will trigger DVB-H L1 to wakeup immediately when in snooze/slotted mode.


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_WAKEUP_NOW_CMD   (HANDLE  hResourceContext );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.16 Subcommand 15 - Enable LNA update.

         This command will enable the LNA update loop.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_ENABLE_LNA_UPDATE    (HANDLE  hResourceContext );
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.17 Subcommand 16 - Disable LNA update.

         This command will disable the LNA update loop.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_DISABLE_LNA_UPDATE    (HANDLE  hResourceContext );

   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.18 Subcommand 17 - Collect packet count stats for MPE-FEC.

         This command is used to collect packet counter statistics for MPE-FEC.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iPid				= Program ID for data pid
   \param piPid			= Pointer to store	program ID for data pid
   \param piPid_valid		= Pointer to store whether the PID was valid, if 0 ignore Pid_burst_cnt
   \param piPid_burst_cnt	= Pointer to store the number of bursts for pid above
   \param piRs1_pkt_cnt	= Pointer to store RS1 packet counter
   \param piRs1_trash_cnt	= Pointer to store RS1 trashed packet counter
   \param piPrbs_cnt		= Pointer to store PRBS counter

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_PACKET_CNT_CMD    (HANDLE  hResourceContext,
         unsigned short iPid,
         unsigned short *piPid,
         unsigned char *piPid_valid,
         unsigned long *piPid_burst_cnt,
         unsigned long *piRs1_pkt_cnt,
         unsigned long *piRs1_trash_cnt,
         unsigned long *piPrbs_cnt	);
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.19 Subcommand 18 - Set default jammer thresholds.

         This command changes the default jammer thresholds.


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iNumber_of_Jammers			= Number of jammers, from 0 to 2
   \param  iJammer_1_Gain				= Gain for Jammer 1
   \param  iNotch_1_I					= Notch I, 1st Jammer if Number of Jammers > 0
   \param  iNotch_1_Q					= Notch Q, 1st Jammer if Number of Jammers > 0
   \param  iJammer_2_Gain				= Gain for Jammer 2
   \param  iNotch_2_I					= Notch I, 2nd Jammer if Number of Jammers > 1
   \param  iNotch_2_Q					= Notch Q, 2nd Jammer if Number of Jammers > 1

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning
      This command has to be sent prior to the acquisition command.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_CFG_JAMMER_CMD    (HANDLE  hResourceContext,
         unsigned char iNumber_of_Jammers,
         unsigned char iJammer_1_Gain,
         unsigned short iNotch_1_I,
         unsigned short iNotch_1_Q,
         unsigned char iJammer_2_Gain,
         unsigned short iNotch_2_I,
         unsigned short iNotch_2_Q);
   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.20 Subcommand 19 - Gets the RSSI.

         This command will trigger DVB-H L1 to display and return the current RSSI information

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pRssi_qb			= Pointer to location to store required Signal Strength Information in Q6 format
   \param pLna_state		= Pointer to location to store Low Noise Amplifier state
   \param pDb_vref_q6		= Pointer to location to store dBvref in Q6 format
   \param pLog2ee_q6       = Pointer to location to store Log2 of the normalized energy estimate in Q6 format.
   \param pAgc_filt_state_q12 =Pointer to location to store DVGA gain in Q12 log2 format

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning
      The system must be acquired.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_RSSI_STATUS    (HANDLE  hResourceContext,
         short *pRssi_qb,
         unsigned char *pLna_state,
         short *pDb_vref_q6,
         short *pLog2ee_q6,
         short *pAgc_filt_state_q12);

   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.24 Subcommand 23 - Configure the post FFT jammer Cancellation.

         This command configures post-FFT jammer parameters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iMode = Mode of the acquired signal.
      Defined by ubm_dvbh_mode in QLib_Defines.h:
      \code
      UBM_L1_MODE_1	= 1,
      UBM_L1_MODE_2	= 2,
      UBM_L1_MODE_3	= 3,
      \endcode
   \param Threshold  =  Post FFT Threshold for mode above
   \param Gain = Post FFT gain for mode above
   \param Region0_Odd_Value = Odd Value for Region 0
   \param Region0_Even_Value =Even Value for Region 0
   \param  Region1_Odd_Value = Odd Value for Region 1
   \param  Region1_Even_Value =Even Value for Region 1
   \param  Region2_Odd_Value = Odd Value for Region 2
   \param  Region2_Even_Value =Even Value for Region 2
   \param  Region3_Odd_Value = Odd Value for Region 3
   \param  Region3_Even_Value =Even Value for Region 3
   \param  Region4_Odd_Value = Odd Value for Region 4
   \param  Region4_Even_Value =Even Value for Region 4
   \param  Region5_Odd_Value = Odd Value for Region 5
   \param  Region5_Even_Value =Even Value for Region 5
   \param  Region6_Odd_Value = Odd Value for Region 6
   \param  Region6_Even_Value =Even Value for Region 6
   \param  Region7_Odd_Value = Odd Value for Region 7
   \param  Region7_Even_Value =Even Value for Region 7
   \param  Region_Enable = Region enable for mode above


   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning
      The system must be acquired.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_CFG_POST_FFT_JAMMER_CMD    (HANDLE  hResourceContext,
         unsigned char iMode,
         unsigned char Threshold,
         unsigned char Gain,
         unsigned short Region0_Odd_Value,
         unsigned short Region0_Even_Value,
         unsigned short Region1_Odd_Value,
         unsigned short Region1_Even_Value,
         unsigned short Region2_Odd_Value,
         unsigned short Region2_Even_Value,
         unsigned short Region3_Odd_Value,
         unsigned short Region3_Even_Value,
         unsigned short Region4_Odd_Value,
         unsigned short Region4_Even_Value,
         unsigned short Region5_Odd_Value,
         unsigned short Region5_Even_Value,
         unsigned short Region6_Odd_Value,
         unsigned short Region6_Even_Value,
         unsigned short Region7_Odd_Value,
         unsigned short Region7_Even_Value,
         unsigned short Region_Enable);


   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.28 Subcommand 27 - Force the LNA State.

         This command allows the LNA State to be forced.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLNAState	= LNA value from 0 to 5. 0 is the highest gain state.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning
      The system must be acquired.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_SET_LNA_CMD(HANDLE  hResourceContext,unsigned char iLNAState);

   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.34 Subcommand 33 - Prepare for RSSI Calibration information.

         This command sets up the UUT for CW calibration. It allows the UUT to tune
         when no signal is present.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning
      The system must be acquired.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_PREPARE_FOR_CW_CAL (HANDLE  hResourceContext);

   /******************************************************************************/
   /**
   DVB-H L1 DIAG, 4.3.36 Subcommand 35 - Disable DSP Loops.

         This command disables or enables specific DSP loops.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTimeTrackDisable = enable (0) or disable (1) the RDSP Time Tracking Loop.
   \param iFreqTrackDisable = enable (0) or disable (1) the RDSP Frequency Tracking Loop.
   \param iRxFrontCoarseDcDisable = enable (0) or disable (1) the RDSP Coarse DC Correction Loop.
   \param iRxFrontAgcDisable = enable (0) or disable (1) the RDSP AGC Loop.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_DVBH_UBM_L1_DISABLE_DSP_LOOPS_CMD (HANDLE hResourceContext,
         unsigned char iTimeTrackDisable,
         unsigned char iFreqTrackDisable,
         unsigned char iRxFrontCoarseDcDisable,
         unsigned char iRxFrontAgcDisable);

	/******************************************************************************
                     MediaFLO DIAG
   *******************************************************************************/
   /******************************************************************************/
   /**
   This function returns the wid and lid statistics evaluated over the specified
   number of wid/lid packetes. This function calls the function that parses the wid/lid
   packets.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNoLogPktsTimeout= time in seconds to wait for statistics to update, max of 5
                       seconds recommended
   \param iMinNumWIDLIDs   = Number of wid/lids
   \param _iWIC            = Expected Wide area Identification Channel
   \param _iLIC            = Expected Local area Identification Channel
   \param wid_lid_overall_stats = pointer of type wid_lid_overall_stats. Stores the overall
                           stats for wid/lid
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GetWICLICPER(
      HANDLE hResourceContext,
      unsigned short _iNoLogPktsTimeout,
      unsigned long _iMinNumWIDLIDs,
      int _iWIC,
      int _iLIC,
      void* wid_lid_overall_stats,
      unsigned char* peErrorCode );

   /******************************************************************************/
   /**
   This function performs tasks necessary for starting the wid/lid 0x1210 log packets.
   This function resets the overall wid/lid statistics, clears the log queue, sets the
   0x1210 log mask, enables the OIS FTAP mode(recommended by system engineering), enables the
   wic/lic reception mode.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param wid_lid_overall_stats = pointer of type wid_lid_overall_stats. Stores the overall
                           stats for wid/lid
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_StartWICLIC(
      HANDLE hResourceContext,
      void* wid_lid_overall_stats,
      unsigned char* peErrorCode);

   /******************************************************************************/
   /**
   This function performs tasks necessary for stopping the wid/lid 0x1210 log packets.
   This function resets the overall wid/lid statistics, clears the log queue, disables the
   0x1210 log mask, disables the OIS FTAP mode(recommended by system engineering),
   disables the wic/lic reception mode.


   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param wid_lid_overall_stats = pointer of type wid_lid_overall_stats. Stores the overall
                           stats for wid/lid
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_StopWICLIC(
      HANDLE hResourceContext,
      void* wid_lid_overall_stats,
      unsigned char* peErrorCode);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.1 MFLO reset PLP statistics
   This command resets the PLP statistics for either one MLC or all MLCs.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param bResetAll = indicates which PLP statistics to reset
                  true resets PLP statistics for all MLCs,
                  false resets PLP statistics for the requested MLC
   \param iMlcId = the MLC ID for which to reset the PLP statistics,
               this parameter only applies if bResetAll is set to false
   \param eSystem = the system (mftypes_system_type),
                this parameter only applies if bResetAll is set to false

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_RST_PLP_STATS(	HANDLE hResourceContext,
         unsigned char bResetAll,
         unsigned char iMlcId,
         unsigned char eSystem );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.2 MFLO get FLO state

   This command gets the current system state for the specified system.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSystem = the system (mftypes_system_type),
   \param peSystemState = the current system state (mftypes_state_type)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_FLO_STATE(	HANDLE hResourceContext,
         unsigned char eSystem,
         unsigned char* peSystemState );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.3 MFLO start FLO

   This command sends the request to acquire the system.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_START_FLO(	HANDLE hResourceContext );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.4 MFLO get FLO version

   This command gets the FLO HW and SW version information.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piHWmajor = the HW Major Number
   \param piHWminor = the HW Minor Number
   \param piHWpatch = the HW Patch Number
   \param piSWmajor = the SW Major Number
   \param piSWminor = the SW Minor Number
   \param piSWpatch = the SW Patch Number
   \param piSWbuild = the SW Build Number

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_FLO_VERSION_INFO(	HANDLE hResourceContext,
         unsigned char* piHWmajor,
         unsigned char* piHWminor,
         unsigned char* piHWpatch,
         unsigned char* piSWmajor,
         unsigned char* piSWminor,
         unsigned char* piSWpatch,
         unsigned char* piSWbuild );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.5 MFLO activate flow

   This command activates the specified flow.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFlowID = the Flow ID
   \param eBinding = the binding value (mftypes_binding_type)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_ACTIVATE_FLOW( HANDLE hResourceContext,
         unsigned long iFlowID,
         unsigned char eBinding );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.6 MFLO deactivate

   This command deactivates the specified flow.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFlowID = the Flow ID

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_DEACTIVATE_FLOW( HANDLE hResourceContext, unsigned long iFlowID );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.7 MFLO get active flow

   This command gets the number of active flows in the system.
   Use QLIB_MFLO_GET_BOUND_FLOW_LIST to get more information about the active flows.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piBoundFlowCount = the number of active flows

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_BOUND_FLOW_COUNT( HANDLE hResourceContext, unsigned char* piBoundFlowCount );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.7 MFLO get active flow

   This command gets information regarding all active flows in the system.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piBoundFlowCount = This is both and input and output param.  As an input param
                       it indicates the number of elements in pActiveFlowInfoArray.
                       As an output param it indicates the actual number of active
                       flows in the system.
   \param pActiveFlowInfoArray = Array of mftypes_active_flow_info structures
                           (mftypes_active_flow_info is defined in QLib_Defines.h)

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pActiveFlowInfoArray to	store information about all active flows in
          the system.  The required amount of space can be determined by first calling
          QLIB_MFLO_GetBoundFlowCount.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_BOUND_FLOW_LIST( HANDLE hResourceContext,
         unsigned char* piBoundFlowCount,
         void* pActiveFlowInfoArray);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.8 MFLO get MLC info

   This command gets information about the specified MLC ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = the MLC ID
   \param eSystem = the system (mftypes_system_type)
   \param pActiveMlcInfo = information regarding the specified MLC (mftypes_active_mlc_info)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_MLC_INFO(	HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char eSystem,
         void* pActiveMlcInfo );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.9 MFLO get RSSI value

   This command gets the current calculated RSSI (Received Signal Strength Indicator) value.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param pdRSSI = the RSSI value in dBm

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_RSSI_VALUE( HANDLE hResourceContext, double* pdRSSI );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.10 MFLO get MLC PLP

   This command gets accumulated PLP stats and accumulated CB stats before and after RS.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = the MLC ID
   \param eSystem = the system (mftypes_system_type)
   \param pMlcPlpStats = structure of type mftypes_mlc_plp_stats_type to hold the PLP and CB statistics
                           as defined in QLIB_Defines.h

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS(	HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char eSystem,
         void* pMlcPlpStats );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.11 MFLO set channel

   This command sets the MediaFLO RF channel.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iChannel = the channel

   \return true if successful, false if fail.

   \warning Once the channel is changed, the MFLO_START_FLO command needs to be sent in order
          to acquire the system on the new channel.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_SET_RF_CHNL( HANDLE hResourceContext, unsigned char iChannel );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.12 MFLO get channel

   This command gets the MediaFLO RF channel.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piChannel = the channel

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_RF_CHNL( HANDLE hResourceContext, unsigned char *piChannel );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.13 MFLO reset MLC PLP

   This command resets the PLP and CB statistics associated with the specified MLC ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcId = the MLC ID for which to reset the PLP and CB statistics
   \param eSystem = the system (mftypes_system_type),

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_RST_MLC_PLP_STATS(	HANDLE hResourceContext,
         unsigned char iMlcId,
         unsigned char eSystem );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.14 MFLO MLC dynamic

   This command gets the MLC Dynamic information associated with the specified MLC ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = the MLC ID
   \param eSystem = the system (mftypes_system_type)
   \param pMLC_Dyn_Params = structure to store the info associated with the MLC ID
                      (mftypes_mlc_dyn_params_type)

   \return true if successful, false if fail.

   \warning The response to this command is only valid if the specified MLC ID is
          currently active.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_MLC_DYN_PARAMS(	HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char eSystem,
         void* pMLC_Dyn_Params);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.15 MFLO get OIS PLPs

   This command gets the OIS PLP statistics associated with the specified OIS type.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eOIStype = the OIS type, wide area or local area (mftypes_system_type)
   \param piLastNumGoodPLPs = number of good PLPs from last decoding
   \param piLastNumErasures = number of erasures from last decoding
   \param piTotalNumGoodPLPs = cummulative number of good PLPs including from last decoding
   \param piTotalNumErasures = cummulative number of erasures including from last decoding

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_OIS_PLP_STAT(	HANDLE hResourceContext,
         unsigned char eOIStype,
         unsigned long* piLastNumGoodPLPs,
         unsigned long* piLastNumErasures,
         unsigned long* piTotalNumGoodPLPs,
         unsigned long* piTotalNumErasures );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.16 MFLO reset OIS PLPs statistics

   This command resets the OIS PLP statistics associated with the specified OIS type.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eOIStype = the OIS type, wide area or local area (mftypes_system_type)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_RST_OIS_PLP_STAT( HANDLE hResourceContext, unsigned char eOIStype );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.17 MFLO GET FLO SUBSTATE

   This command gets the protocol task's substate.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSystem = the system (mftypes_system_type)
   \param peSubstate = the protocol task's substate (mftypes_substate)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_FLO_SUBSTATE(	HANDLE hResourceContext,
         unsigned char eSystem,
         unsigned char* peSubstate );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.18 MFLO GET ACTIVE MLC LIST

   This command gets the number of active MLCs on both of the systems, WOIS and LOIS.
   Use QLIB_MFLO_GET_ACTIVE_MLC_LIST to get more information about the active MLCs.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piActiveMlcCount = the number of active MLCs

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_ACTIVE_MLC_COUNT( HANDLE hResourceContext, unsigned char* piActiveMlcCount );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.18 MFLO GET ACTIVE MLC LIST

   This command gets information regarding all active MLCs on both of the systems.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piActiveMlcCount = This is both and input and output param.  As an input param
                       it indicates the number of elements in pActiveMlcInfoArray.
                       As an output param it indicates the actual number of active
                       MLCs on both systems.
   \param pActiveMlcInfoArray = Array of mftypes_active_mlc_info structures
                         (mftypes_active_mlc_info is defined in QLib_Defines.h)

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pActiveMlcInfoArray to	store information about all active MLCs on both
          systems.  The required amount of space can be determined by first calling
          QLIB_MFLO_GetActiveMlcCount.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_ACTIVE_MLC_LIST( HANDLE hResourceContext,
         unsigned char* piActiveMlcCount,
         void* pActiveMlcInfoArray);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.19 MFLOG GET FLO PLP DYN

   This command gets the PLP statistics for a specific Flow ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFlowID = the Flow ID
   \param piTotalNumGoodPlps = the total number of good PLPs
   \param piTotalNumErasures = the total number of erasures
   \param piSFNumGoodPlps = the number of good PLPs in the last superframe
   \param piSFNumErasures = the number of erasures in the last superframe

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_FLO_PLP_DYN_PARAMS( HANDLE hResourceContext,
         unsigned long iFlowID,
         unsigned short* piTotalNumGoodPlps,
         unsigned short* piTotalNumErasures,
         unsigned short* piSFNumGoodPlps,
         unsigned short* piSFErasures);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.20 MFLO ENABLE FTAP PLP DATA

   This command enables logging of FTAP PLP parameters.  This is the first diag
   command that needs to be issued when logging FTAP PLP packet parameters.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param bEnable = true enables logging, false disables logging

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_ENABLE_FTAP_PLP_DATA( HANDLE hResourceContext, unsigned char bEnable );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.21 MFLO ENABLE FTAP OIS RECEPTION MODE

   This command places the FDS in a test mode called "FTAP OIS Reception Mode".
   In this mode, the device decodes WOIS Only, LOIS Only or both WOIS and LOIS
   every superframe.  Once in this mode, the FDS starts logging WOIS/LOIS log
   codes (0x120E and 0x120F) once every superframe.  This command can also be used
   to disable "FTAP OIS Reception Mode".

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eOIS_ReceptionMode = the FTAP OIS reception mode (mftypes_ois_reception_mode_type)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_ENABLE_FTAP_OIS( HANDLE hResourceContext, unsigned char eOIS_ReceptionMode );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic ICD, 2.3.22 MFLO ENABLE FTAP WICLIC RECEPTION MODE

   This command places the FDS in a test mode called "FTAP WIC/LIC Reception Mode".
   In this mode, the device decodes WIC/LIC every superframe.  Once in this mode,
   the FDS starts logging the WIC/LIC log code (0x1210) once every superframe.
   This command can also be used to disable "FTAP WIC/LIC Reception Mode".

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param bEnable = true places FDS in FTAP WIC/LIC Reception Mode,
                false disables WIC/LIC decode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_ENABLE_WIC_LIC( HANDLE hResourceContext, unsigned char bEnable );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, MFLO GET PLATFORM CONFIG

   This command retrieves the platform configuration information.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param pihwPlatform = The FLO HW type configured in AMSS
   \param pihwVersion = The FLO HW version detected at power up
   \param piMBPBusMode = MBP Bus Mode detected by AMSS
      Defined by mftypes_mbp_bus_mode:
      \code
      MFLO_MBP_BUS_MODE_EBI2     = 0,
      MFLO_MBP_BUS_MODE_EBI2_MUX = 1,
      MFLO_MBP_BUS_MODE_SDIO     = 2
      \endcode
   \param pbMBPDetected = 0 - MBP not detected at power up, 1 - MBP detected at power up
   \param piReserved = reserved

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_PLATFORM_CONFIG( HANDLE hResourceContext,
         unsigned char* pihwPlatform,
         unsigned char* pihwVersion,
         unsigned char* piMBPBusMode,
         unsigned char* pbMBPDetected,
         unsigned long* piReserved);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, MFLO GET C/I Average

   This command retrieves the C/I Average.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piCIAverage = The C/I Average

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_CI_AVG( HANDLE hResourceContext,
         unsigned short* piCIAverage);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, MFLO FTM SET DEMOD CONFIG

   This command sets the demodulation configuration.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFftSize = FFT Size
      Defined by ftm_mflo_fft_size:
      \code
      FTM_MFLO_2K_FFT = 0,
      FTM_MFLO_4K_FFT = 1,
      FTM_MFLO_8K_FFT = 2
      \endcode
   \param iCpLength = CP Length
      Defined by ftm_mflo_cp_length:
      \code
      FTM_MFLO_1_16_CP_LENGTH = 0,
      FTM_MFLO_1_8_CP_LENGTH = 1,
      FTM_MFLO_3_16_CP_LENGTH = 2,
      FTM_MFLO_1_4_CP_LENGTH = 3
      \endcode
   \param iPpc = 0 - No PPC Symbols
   \param iPilotPattern = 0 - (2,6), 1 - (0,3,6)
   \param iReserved = Reserved - must be set to 1
   \param iIcMode = IC Mode
      Defined by ftm_mflo_ic_mode:
      \code
      FTM_MFLO_IC_DISABLED = 0,
      FTM_MFLO_IC_BYPASSED = 1,
      FTM_MFLO_IC_DYNAMIC = 3
      \endcode
   \param iDmttMode = DMTT Mode
      Defined by ftm_mflo_dmtt_mode
      \code
      FTM_MFLO_DMTT_W_TDM2_RDSP_NOT_USED = 2,
      FTM_MFLO_DMTT_W_TDM2_RDSP_USED = 3,
      FTM_MFLO_DMTT_TPC_BASED = 4
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_SET_DEMOD_CONFIG( HANDLE hResourceContext,
         unsigned char eFftSize,
         unsigned char eCpLength,
         unsigned char iPpc,
         unsigned char iPilotPattern,
         unsigned char iReserved,
         unsigned char eIcMode,
         unsigned char eDmttMode );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, RF Initialise

         Initialises the RF Mode, Bandwidth and Frequency

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRfMode = The RF mode to initialise.
   Defined by ftm_mflo_rf_mode in QLib_Defines.h:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iFreqKhz = The RF frequency to initialise in KHz
   \param iBandwidth = The RF bandwidth to use.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_RF_INITIALIZE(HANDLE hResourceContext,
         unsigned char iRfMode,
         unsigned long iFreqKhz,
         unsigned char iBandwidth);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Get PLL Lock Status

         This function returns the lock state of the RF synthesizer PLL.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pbLockState = the lock state of the RF synthesizer PLL (0=unlocked, 1=locked)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_GET_PLL_LOCK_STATUS(HANDLE hResourceContext,
         unsigned short* pbLockState);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Tune RF In Hz

      This function tunes the PLL to the requested frequency.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param ifreqHz = Required frequency in Hz.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_TUNE_RF_IN_HZ(HANDLE hResourceContext,
         unsigned long iFreq );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Get Rx RSSI

         This function returns a 16-bit signed receiver signal strength indicator (RSSI) value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piIsValid = Returns whether the RSSI value is valid (1) or not (0).
   \param piRssiValue = RSSI value in 1/100 dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_GET_RSSI(HANDLE hResourceContext,
         unsigned char *piIsValid,
         short *piRssiValue );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Get AGC State

         This function returns the current gain state and 15-bit signed DVGA accumulator value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piDvga = current DVGA accumulator value, 15-bit signed value
   \param piGainState = current gain state (0 = highest gain state, 6 = lowest gain state)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_GET_AGC_STATE(HANDLE hResourceContext,
         short*piDvga,
         unsigned char*piGainState);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Get RSSI Cal Point

         This function returns a 16-bit signed RSSI cal point for the requested gain state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iGainState = the gain state (0 = highest gain, 6 = lowest gain)
   \param iInputPower = the receiver input power, 1 dB resolution
   \param iNumberOfSamples = the number of times to sample the cal point
   \param piCalPoint = the RSSI cal point for the requested gain state, 1/256 dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_GET_RSSI_CAL_POINT(HANDLE hResourceContext,
         unsigned char iGainState,
         short iInputPower,
         unsigned short iNumberOfSamples,
         short* piCalPoint);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Load RSSI Cal Point

         Loads a RSSI Calibration point in to phone memory

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFreq = The desired frequency in MHz
   \param iGainState = the phone gain state
   \param iCalPoint = the calibration point (dB/256)
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_LOAD_RSSI_CAL_POINT(HANDLE hResourceContext,
         unsigned short iFreq,
         unsigned char iGainState,
         short iCalPoint);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Load AGC Cal Channel List

         Loads the AGC Calibration channel list to the phone memory.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iChannelsFreqs = List of up to 32 frequencies in MHz to load to the phone
   \param iNumberOfChannelsFreqs = The number of channels/frequencies to load.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_LOAD_AGC_CAL_CHAN_LIST( HANDLE hResourceContext,
         unsigned short *iChannelsFreqs,
         unsigned short iNumberOfChannelsFreqs);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Write Cal Data to NV

         This function stores the current state of the RF Calibration to NV.

   \param hResourceContext =	Resource context that was returned from the call to QLIB_ConnectServer()
   \param iitemCode=			either 0, for the channel list, or 1 for the RSSI calibration point
   \param iGainState=			when writing the calibration point, this specifes the coresponding gains state.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_WRITE_CAL_DATA_TO_NV(HANDLE hResourceContext,
         unsigned long iItemCode,
         unsigned long iGainState);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Set Gain State

         This function sets the gain state of the currently selected LNA.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iGainState = the desired gain state (0=highest gain state, 6=lowest gain state)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_SET_GAIN_STATE(HANDLE hResourceContext,
         unsigned char iGainState);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Set AGC Default

         Restores the AGC to the default state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_SET_AGC_DEFAULT(HANDLE hResourceContext);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Set AGC Gain Tracking

      This command enables/disables the AGC Gain Tracking.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iState = 0 - Disable, 1 - Enable

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_SET_AGC_GAIN_TRACKING( HANDLE hResourceContext,
         unsigned char iState);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Select LNA.

         This command allows the LNA selection to be modified
         independent of the MFLO_FTM_MF_RF_INITIALIZE command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRfMode = The RF mode to initialise.
   Defined by ftm_mflo_rf_mode in QLib_Defines.h:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iLNASelect = LNA value to select.
   Defined by ftm_mflo_rf_lna_type in QLib_Defines.h:
      \code
      MFLO_F_LBAND	= 1,	( MBP2700 LBAND )
      MFLO_F_LNA1		= 2,	( MBP2700 LNA1 )
      MFLO_F_LNA2		= 3,	( MBP2700 LNA2 )
      \endcode
   \param iReserved1 = Set to zero.
   \param iReserved2 = Set to zero.

   \return true if successful, false if fail.

   \warning MBP2700 only

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_LNA_SELECT(HANDLE hResourceContext,
         unsigned char iRfMode,
         unsigned char iLNASelect,
         unsigned long iReserved1,
         unsigned long iReserved2);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Set Antenna tune switch.

         This command allows the antenna tune selection to be modified independent
         of the MFLO_FTM_RF_INITIALIZE command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRfMode = The RF mode to initialise.
   Defined by ftm_mflo_rf_mode in QLib_Defines.h:
      \code
      FTM_MFLO_US		= 0,
      FTM_MFLO_VHF	= 1,
      FTM_MFLO_UHF	= 2,
      FTM_MFLO_L_BAND = 3
      \endcode
   \param iAntTuneMask = The mask to apply to the Tune Settings.
   \param iAntTuneSettings = The Antenna Tune Settings (only bits with a corresponding 1 in the Tune Mask will actually be set)
   \param iReserved1 = Set to zero.
   \param iReserved2 = Set to zero.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_SET_ANT_TUNE(HANDLE hResourceContext,
         unsigned char iRfMode,
         unsigned char iAntTuneMask,
         unsigned char iAntTuneSettings,
         unsigned long iReserved1,
         unsigned long iReserved2);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Enable FLO Demod

         This function enables FLO demodulation during FTM mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_SET_NS_MODE(HANDLE hResourceContext);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Acquire FLO System

         This function forces FLO to perform a FLO system acquisition.
         An attempt will be made to acquire both the WIDE and LOCAL systems.
         Acquisition may require several seconds.
         The QLIB_MFLO_FTM_NS_GET_OIS_STATE command can be used to determine if a system has been acquired.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iReserved = Set to 0.
   \param bReserved1 = Not used
   \param bReserved2 = Not used

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_ACQUIRE_FLO_SYSTEM( HANDLE hResourceContext,
         unsigned short iReserved,
         unsigned char *pbReserved1,
         unsigned char *pbReserved2 );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Activate Flow

         This function activates the Flow specified by the caller.
         Activation may require several seconds. The request message contains
         an additional field that instructs FTM to wait (in msec) before returning
         the activation response message.  If the wait time is 0 the response will
         be sent immediately and the ACTIVATION_STATUS in the response will be set
         to ACTIVATING_FLOW.  In addition the stream information returned in the
         response will be invalid (set to 0). The FTM_MFLO_NS_GET_STREAM_INFO command
         can be used to determine if the Flow was successfully activated. If a wait
         time is specified the ACTIVATION_STATUS in the response will be set to either
         ACTIVATION_FAILED or ACTIVATION_SUCCESS depending upon the stream state at the
         wait time expiration.
         Non-signaling mode in FTM supports a maximum of 20 simultaneously active Flows.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iReserved = Set to zero.
   \param iFlowId = specfies the flow to activate.
   \param eBinding = specifies the binding type.
      Defined by mftypes_binding_type:
      \code
      MFLO_OVERHEAD   = 0x00,
      RT_VIDEO_CODEC  = 0x01,
      RT_AUDIO_CODEC  = 0x02,
      NRT_MEDIA       = 0x03,
      DATACAST        = 0x04,
      TAP             = 0x05,
      FIXED_TAP       = 0x06,
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_ACTIVATE_FLOW( HANDLE hResourceContext,
         unsigned short iReserved,
         unsigned long iFlowId,
         unsigned char iBindingType);

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Deactivate Flow

         This function deactivates the Flow specified by the caller.
         If the Flow Id in the request message is not active FLOW_NOT_ACTIVATED is returned
         in the response, otherwise FLOW_DEACTIVATING is returned.
         The MFLO_FTM_NS_GET_STREAM_INFO command can be used to determine if the Flow
         was successfully deactivated.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFlowId = specifies the Flow ID or use 0xFFFFFFFF for all active Flow IDs

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_DEACTIVATE_FLOW( HANDLE hResourceContext,
         unsigned long iFlowId );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Tune Freq and BW.

   This command tunes the RF to the specified frequency and bandwidth.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iFreqKhz = Frequency in Hz.
   \param iBandwidthMHz = Bandwidth in MHz.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_TUNE_FREQ_AND_BW( HANDLE hResourceContext,
         unsigned long iFreq,
         unsigned char iBandwidthMHz );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Get RSSI and Gain State.

   This command retrieves the RSSI and Gain State. This value is not valid when the FLO is not actively decoding.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iGainState = the current gain state
   \param iRssi = Signed RSSI value in 1/100th dB resolution.
   \param bRssiValid = specifies validity of the RSSI measurement

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GET_RSSI_AND_GAIN_STATE( HANDLE hResourceContext,
         unsigned char *piGainState,
         short *piRssi,
         unsigned char *pbRssiValid );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Get Stream Info

         This function command retrieves the stream information for the specified
         Flow ID.  If the Flow Id specified in the request message is not active
         FLOW_NOT_ACTIVATED is returned in the response message, otherwise
         FLOW_ACTIVATED is returned.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFlowId = specifies the Flow ID.
   \param eStatus = specifies deactivate flow status.
   Defined by mftypes_flow_status_type:
      \code
      FLOW_NOT_ACTIVATED		= 0x00,
      FLOW_ACTIVATED			= 0x01
      \endcode
   \param iMlcID = specifies the MLC ID.
   \param iStreamID = specifies the Stream ID.
   \param bStreamUsesBothLayers = true(1) if enhanced layer data is present.
   \param bReserved = Reserved.
   \param peBindingType = specifies the binding type.
      Defined by mftypes_binding_type:
      \code
      MFLO_OVERHEAD   = 0x00,
      RT_VIDEO_CODEC  = 0x01,
      RT_AUDIO_CODEC  = 0x02,
      NRT_MEDIA       = 0x03,
      DATACAST        = 0x04,
      TAP             = 0x05,
      FIXED_TAP       = 0x06,
      \endcode
   \param eSystem = specifies the system.
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GET_STREAM_INFO( HANDLE hResourceContext,
         unsigned long iFlowId,
         unsigned char *piStatus,
         unsigned char *piMlcID,
         unsigned char *piStreamID,
         unsigned char *pbStreamUsesBothLayers,
         unsigned char *pbReserved,
         unsigned char *piBindingType,
         unsigned char *piSystem );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling - Get MLC Dynamic Parameters.

      This command retrieves the dynamic parameters for the specified MLC ID. The information
      returned can be used to calculate PER statistics. If the MLC is not active 0 is returned
      in the first byte of the response message, otherwise 1 is returned.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = specifies the MLC ID. This is both an input and output param.
   \param eSystem = specifies the system
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode
   \param piFFTAddress = MLC Base FFT Address
   \param peTransmitMode = Transmit Mode
      Defined by mftypes_trans_mode_type:
      \code
      REG_QPSK_1_3         = 0,
      MREG_QPSK_1_2        = 1,
      REG_16_QAM_1_3       = 2,
      REG_16_QAM_1_2       = 3,
      REG_16_QAM_2_3       = 4,
      REG_16_QPSK_1_5      = 5,
      LAY_QPSK_1_3_ER_4    = 6,
      LAY_QPSK_1_2_ER_4    = 7,
      LAY_QPSK_2_3_ER_4    = 8,
      LAY_QPSK_1_3_ER_6_25 = 9,
      LAY_QPSK_1_2_ER_6_25 = 10,
      LAY_QPSK_2_3_ER_6_25 = 11
      \endcode
   \param peRSOuterCode = RS Outer Code
      Defined by mftypes_reed_solomon_code_type:
      \code
      RS_16_16_0		= 0x0,
      RS_16_14_2		= 0x1,
      RS_16_12_4		= 0x2,
      RS_16_8_8		= 0x3
      \endcode
   \param piCodeBlocks = Code Blocks in current Superframe
   \param pbStream1Bound = true(1) if Stream 1 is bound
   \param pbStream2Bound = true(1) if Stream 2 is bound
   \param iStream0Length = Stream 0 length
   \param iStream1Length = Stream 1 length
   \param iStream2Length = Stream 2 length
   \param pBasePlpStats = structure of type mftypes_plp_stats_type to hold the Base PLP statistics
                           as defined in QLIB_Defines.h
   \param pEnhPlpStats = structure of type mftypes_plp_stats_type to hold the Enhanced PLP statistics
                           as defined in QLIB_Defines.h
   \param pBaseCbStats = structure of type mftypes_cb_stats_type to hold the Base CB statistics
                           as defined in QLIB_Defines.h
   \param pEnhCbStats = structure of type mftypes_cb_stats_type to hold the Enhanced CB statistics
                           as defined in QLIB_Defines.h

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pMlcPlpStats to store structure of type mftypes_mlc_plp_stats_type

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_MLC_DYN_PARAMS( HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char iSystem,
         unsigned char *piStatus,
         unsigned long *piFFTAddress,
         unsigned char *piTransmitMode,
         unsigned char *piRSOuterCode,
         unsigned char *piCodeBlocks,
         unsigned char *pbStream1Bound,
         unsigned char *pbStream2Bound,
         unsigned short *piStream0Length,
         unsigned short *piStream1Length,
         unsigned short *piStream2Length,
         void *pBasePlpStats,
         void *pEnhPlpStats,
         void *pBaseCbStats,
         void *pEnhCbStats );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling, Reset MLC PLP Stats.

      This command clears (resets to 0) the accumulated PLP/CB statistics for the specified MLC ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iMlcID = specifies the MLC ID
   \param eSystem = specifies the system
      Defined by mftypes_system_type:
      \code
      WIDE			= 0x00,		//! Wide System
      LOCAL			= 0x01,		//! Local System
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_RESET_MLC_PLP_STATS( HANDLE hResourceContext,
         unsigned char iMlcID,
         unsigned char iSystem );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling, Reset ALL PLP Stats.

      This command clears (resets to 0) the accumulated PLP/CB statistics for all of the active MLCs.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_RESET_ALL_PLP_STATS( HANDLE hResourceContext );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling, Get the number of Active Flows.

      This command returns the number of currently active Flows. FTM Non-signaling mode
      supports a maximum of 20 simultaneously active Flows.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \param piFlowCount = the number of active flows

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GET_ACTIVE_FLOW_COUNT( HANDLE hResourceContext,
         unsigned char *piFlowCount );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Get all active flow Ids.

      This command returns a list of all of the currently active flows.
      The total number of active flows is also returned. The list of active flow IDs
      returned is formatted as a 20 entry by uint32 (4 byte) array (80 total bytes)
      Each array entry corresponds to an active Flow ID. If there are less than 20 active
      Flows (FTM Non-signaling mode supports a maximum of 20 simultaneously active flows)
      the array will be zero padded. Note that 0 is a valid Flow ID.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piFlowCount = This is both and input and output param.  As an input param
                       it indicates the number of elements in pActiveFlowInfoArray.
                       As an output param it indicates the actual number of active
                       flows in the system.
   \param pArrayFlowIds = array of all active flow ids. The list of active Flow IDs returned is
                  formatted as a 20 entry by uint32 (4 byte) array (64 total bytes). Each
                  array entry corresponds to an active Flow ID. If there are less than 16
                  active Flows (20 is maximum concurrent activated Flows in FTM non-signaling
                  mode) the array will be zero padded. Note that 0 is a valid Flow ID.

   \return true if successful, false if fail.

   \warning Prior to calling this function, sufficient space should be allocated in
          pArrayFlowIds to store 20x4Byte array (80 total bytes).

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GET_ALL_ACTIVE_FLOW_IDS( HANDLE hResourceContext,
         unsigned char *piFlowCount,
         unsigned long *pArrayFlowIds );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Get OIS State.

   This command returns the state of the WIde and Local OIS.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param piWideOisStatus = Wide OIS Status.
      Defined by mftypes_state_type:
      \code
      INVALID_STATE		= 0x00,
      ACQUIRING_STATE		= 0x01,
      IDLE_STATE			= 0x02,
      ACTIVE_STATE		= 0x03,
      UNAVAILABLE_STATE	= 0x04
      \endcode
   \param piLocalOisStatus = Local OIS Status.
      Defined by mftypes_state_type:
      \code
      INVALID_STATE		= 0x00,
      ACQUIRING_STATE		= 0x01,
      IDLE_STATE			= 0x02,
      ACTIVE_STATE		= 0x03,
      UNAVAILABLE_STATE	= 0x04
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GET_OIS_STATE( HANDLE hResourceContext,
         unsigned char *piWideOisStatus,
         unsigned char *piLocalOisStatus );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Enable FTAP PLP Data Logging

      This command starts/stops FTAP PLP Data Packet Records Logging.
      The FTAP PLP data records for all the active FTAP Flows will be logged each superframe.
      This command requires that the following log packets are enabled:
      1)	MFLO FTAP PACKETRECORD HEADER PARAMS (0x120C)
      2)	MFLO FTAP PACKETRECORD PARAMS (0x120D)

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFtapEnable = 0 - Disable FTAP Logging, 1 - Enable FTAP Logging.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_ENABLE_FTAP_PLP_DATA( HANDLE hResourceContext,
         unsigned char iFtapEnable );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Non-Signalling, Set FLO Sleep.

   This command Enables/disables FLO sleep.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSleepSetting = indicates sleep setting.
      Defined by mftypes_sleep_settings_type:
      \code
      SLEEP_AND_SNOOZE_DISABLED		= 0x00,
      SLEEP_ENABLED_SNOOZE_DISABLED	= 0x01,
      SLEEP_DISABLED_SNOOZE_ENABLED	= 0x02,
      SLEEP_ENABLED_SNOOZE_ENABLED	= 0x03
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_SET_FLO_SLEEP( HANDLE hResourceContext,
         unsigned char iSleepSetting );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Calibration, Set Jammer Detection Mode

      This command configures the Jammer Detection Mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iEnable = Flag to disable (0) or enable (1) jammer detection
   \param iJDMode = Jammer Detect Mode
      Defined by mftypes_jd_mode_type:
      \code
      MFLO_JD_MODE_AUTO = 0,
      MFLO_JD_MODE_1    = 1,
      MFLO_JD_MODE_2    = 2
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_SET_JAMMER_DETECT_MODE( HANDLE hResourceContext,
         unsigned char iEnable,
         unsigned char iJDMode );

   /******************************************************************************/
   /**
   MediaFLO Diagnostic Calibration, Set AGC Gain Tracking

      This command enables/disables the AGC Gain Tracking.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iState = 0 - Disable, 1 - Enable

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_SET_AGC_GAIN_TRACKING( HANDLE hResourceContext,
         unsigned char iState);

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling, Freeze the AGC gain state

      This command freezes the AGC gain state at the current gain state.
      There may be a delay of up to 500ms after the command is sent before the gain state is frozen.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFreezeState = 0 - Unfreeze, 1 - Freeze

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_FREEZE_GAIN_STATE( HANDLE hResourceContext,
         unsigned char iFreezeState );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling, Get FTM state

      This command returns the current FTM state of the FLO device driver.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFTMState = FTM state
      Defined by mftypes_ftm_state_type:
      \code
      MFLO_FTM_STATE_UNINITIALIZED	= 0,
      MFLO_FTM_STATE_INITIALIZING	= 1,
      MFLO_FTM_STATE_DEMOD			= 2,
      MFLO_FTM_STATE_CALIBRATION	= 3
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_GET_FLO_FTM_STATE( HANDLE hResourceContext,
         unsigned char* piFTMState );

   /******************************************************************************/
   /**

   MediaFLO Diagnostic Non-Signalling, Get the Jammer Detection State

      This command gets the current Jammer Detection state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piJDMode = Jammer Detect Mode
      Defined by mftypes_jd_mode_type:
      \code
      MFLO_JD_MODE_AUTO = 0,
      MFLO_JD_MODE_1    = 1,
      MFLO_JD_MODE_2    = 2
      \endcode
   \param piJDState = Jammer Detect State
      Defined by mftypes_jd_state_type:
      \code
      MFLO_JD_STATE_INVALID      = 0,
      MFLO_JD_STATE_MODE_1       = 1,
      MFLO_JD_STATE_MODE_2       = 2,
      MFLO_JD_STATE_INITIALIZING = 3
      \endcode
   \param piJDSubstate = Jammer Detect Substate
      Defined by mftypes_jd_substate_type:
      \code
      MFLO_JD_SUBSTATE_NONE    = 0,
      MFLO_JD_SUBSTATE_POLLING = 1
      \endcode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GET_JAMMER_DETECT_STATE( HANDLE hResourceContext,
         unsigned char* piJDMode,
         unsigned char* piJDState,
         unsigned char* piJDSubstate);

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This command is used to enable the logging of LOG_MFLO_FDM_RECORDS_C log packets.
   The LOG_MFLO_FDM_RECORDS_C log packets contain the FDM (Flow Description Message)
   information for a given WIDE/LOCAL system - a mapping table between Flow IDs and
   MLC IDs.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eMessageType = identifies which part of the control channel data is being
                     requested (mftypes_cc_msg_type)

   \return true if the LOG_MFLO_FDM_RECORDS_C log packets have been generated
           successfully, otherwise false.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GET_CONTROL_CHANNEL_DATA(HANDLE hResourceContext, unsigned char eMessageType );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function gets information required to deactivate all active Flow IDs through
   QLIB_MFLO_GET_BOUND_FLOW_LIST. Then it sends the QLIB_MFLO_DEACTIVATE_FLOW
   command for each active Flow ID. After deactivating all Flow IDs, this function
   will call QLIB_MFLO_GET_BOUND_FLOW_COUNT in one second intervals until the command
   reports	that the number of active Flow IDs is zero or the number of seconds specified
   by the iTimeOut input parameter have elapsed.

   This function will fail if any of the functions called fail or if all active
   Flow IDs have not been de-activated after the specified number of seconds to wait.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iTimeOut - the number of seconds to wait for QLIB_MFLO_GET_BOUND_FLOW_COUNT
                 to report that the number of active Flow IDs is zero.

   \return true if successful, false if fail.

   \warning none

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_DeactivateAllFlows( HANDLE hResourceContext, unsigned short iTimeOut );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This command gets the current system state for the specified system(s) by calling
   QLIB_MFLO_GET_FLO_STATE for each system requested.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param eSystem = the system (mftypes_system_type)
   \param peWideAreaSystemState = the current wide area system state (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state (mftypes_state_type)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GetSystemState( HANDLE hResourceContext, unsigned char eSystem,
         unsigned char* peWideAreaSystemState,
         unsigned char* peLocalAreaSystemState );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function measures the Physical layer packet error rate.  This type of packet
   error rate measurement does not require a known nor finite sequence of FTAP packets
   from the FLO Tx Waveform.  The measurement is made by querying the accumulated PLP
   statistics for all requested Flow IDs.  Accumulated statistics continue to be queried
   until the minimum number of PLPs have been recorded.	Accumulated PLP statistics
   are queried through the QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS function.

   After calling QLIB_MFLO_RST_PLP_STATS to reset the accumulated statistics for all
   MLCs, this function may be called repeatedly until the desired level of confidence
   is achieved.

   The time period that is used when checking for status is controlled by the
   timing index QMSL_Timeout_Delay_MediaFLO_StatusCheck, to be used with the functions
   QLIB_ConfigureTimeOut() and QLIB_GetTimeOut().

   Usage examples:

      QLIB_MFLO_RST_PLP_STATS
      QLIB_MFLO_GetPER_Phy		--------------	Measure PER for first test


      QLIB_MFLO_RST_PLP_STATS		---------
      QLIB_MFLO_GetPER_Phy				|
      QLIB_MFLO_GetPER_Phy				|-----  Measure PER for second test
      QLIB_MFLO_GetPER_Phy				|		Note: Example shows measurement ran longer
      QLIB_MFLO_GetPER_Phy		---------		      to achieve desired level of confidence

   This function reports the packet error rate statistics for all requested Flow IDs,
   the current system state, and an error code.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs
   \param aiFlowIDs = array of Flow IDs for which to get accumulated PLP statistics
   \param aiMlcIDs = array of MLC IDs that correspond to the Flow IDs in aiFlowIDs,
                     this is required because QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS
                 requires the MLC ID not Flow ID
   \param aeSystem = array of mftypes_system_type with elements that correspond to the
                     Flow IDs in aiFlowIDs, the System for each Flow ID is required by
                 QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS
   \param iNoStatsTimeout = time in seconds to wait for statistics to update, max of 5
                      seconds recommended
   \param ePerToMeas = which packet error rate to measure, statistics for both Base and
                       Enhancement layers will be reported but the determination of
                  whether or not iMinPLPs have been measured depends on what this
                  variable has been set to (mftypes_per_to_measure)
   \param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
   \param eRS = which statistics to report, Pre or Post Reed Solomon (mftypes_reed_solomon)
   \param paOverallStats = an array of type mftypes_per_statistics with length
                     iNumberOfFlowIDs to store the packet error rate statistics for
                     all requested Flow IDs
   \param peWideAreaSystemState = the current wide area system state if one of the
                           elements in aeSystem is WIDE (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state if one of the
                                   elements in aeSystem is LOCAL (mftypes_state_type)
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed, see the warning below for one
                   exception (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array.  Sufficient memory for pOverallStats must be allocated by the
          calling program.

          The results reported in paOverallStats are only valid if...
            QLIB_MFLO_GetPER_Phy return value is 1		AND
            Requested system state(s) == ACTIVE_STATE	AND
             peErrorCode == NOT_AVAILABLE.

          If at some point during the measurement, the requested system state is not
          ACTIVE_STATE,  the function will return but not fail.  The current system
          state will be reported through peWideAreaSystemState and peLocalAreaSystemState.

          If the statistics for one or more of the MLC IDs in aiMlcIDs have not updated
          after the number of seconds specified by iNoStatsTimeout have elapsed, the
          function will return but not fail. peErrorCode will be set to PLP_STATS_NOT_UPDATING.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GetPER_Phy( HANDLE hResourceContext, unsigned char iNumberOfFlowIDs,
         unsigned long* aiFlowIDs, unsigned char* aiMlcIDs,
         unsigned char* aeSystem, unsigned short iNoStatsTimeout,
         unsigned char ePerToMeas, unsigned long iMinPLPs,
         unsigned char eRS, void* paOverallStats,
         unsigned char* peWideAreaSystemState,
         unsigned char* peLocalAreaSystemState,
         unsigned char* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function performs initialization steps required before making ANY
   Physical layer + MAC layer packet error rate measurements.  This type of packet
   error rate measurement requires a known and finite sequence of FTAP packets from
   the FLO Tx Waveform.  This function initializes an internal data structure with
   information about the finite sequence of FTAP packets in the FLO Tx Waveform and
   dynamically allocates memory for internal data structures used to make the packet
   error rate measurement.

   This function will fail if pTAP_MsgContent is NULL or if there are any memory
   allocation errors.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param pTAP_MsgContent = pointer to mftypes_tap_msg_content structure filled with
                        information about the finite sequence of FTAP packets in
                      the FLO Tx Waveform.
   \param peErrorCode = an error code used whenever possible to provide more
                   information about why the function failed (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function must be called once at the start of testing, before calling
          any of the following functions:

            QLIB_MFLO_Start_PhyMAC
            QLIB_MFLO_GetPER_PhyMAC
            QLIB_MFLO_Stop_PhyMAC
            QLIB_FTM_MFLO_NS_Start_PhyMAC
            QLIB_FTM_MFLO_NS_GetPER_PhyMAC
            QLIB_FTM_MFLO_NS_Stop_PhyMAC

          There is no need to call this function more than once during one session
          if the contents of the mftypes_tap_msg_content structure (pTAP_MsgContent)
          do not need to change.

          To prevent memory leaks, memory dynamically allocated by this function
          should be freed by calling QLIB_MFLO_FinalCleanup_PhyMAC at the end of
          testing.

          See QLIB_MFLO_GetPER_PhyMAC or QLIB_FTM_MFLO_NS_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_InitTAPMsgContent_PhyMAC( HANDLE hResourceContext,
         void* pTAP_MsgContent, unsigned char* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function performs initialization steps required before making a new
   Physical layer + MAC layer packet error rate measurement.  This type of packet
   error rate measurement requires a known and finite sequence of FTAP packets from
   the FLO Tx Waveform.  This function resets the overall packet error rate statistics,
   updates data structures used in the packet error rate measurement, clears the log
   queue, registers the following log codes:

         LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
         LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

   and enables FTAP PLP Data Logging Mode by through QLIB_MFLO_ENABLE_FTAP_PLP_DATA.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs, must match the actual
                             number of active FlowIDs or this funtion will fail
   \param aiFlowIDs = array of Flow IDs for which to measure the packet error rate
   \param paOverallStats = an array of type mftypes_per_statistics with length
                     iNumberOfFlowIDs to store the packet error rate
                     statistics for all requested Flow IDs
   \param peErrorCode = an error code used whenever possible to provide more
                   information about why the function failed (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array.  Sufficient memory for pOverallStats must be allocated by the
          calling program.

          QLIB_MFLO_InitTAPMsgContent_PhyMAC must be called once before the first
          time this function is called.

          This function must be called once before calling QLIB_MFLO_GetPER_PhyMAC.
          The function QLIB_MFLO_Stop_PhyMAC should be called at the end of the packet
          error rate measurement.  To make a new packet error rate measurement,
          this function must be called again before calling QLIB_MFLO_GetPER_PhyMAC.
          See QLIB_MFLO_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_Start_PhyMAC( HANDLE hResourceContext,
         unsigned char iNumberOfFlowIDs, unsigned long* aiFlowIDs,
         void* paOverallStats, unsigned char* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function measures the Physical layer + MAC layer packet error rate.  This
   type of packet error rate measurement requires a known and finite sequence of
   FTAP packets from the FLO Tx Waveform.

   The measurement is made by getting erasure and FTAP Sequence Number /
   FTAP Test Signature information from log packets with the following log codes:

      LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
      LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

    FTAP Sequence Number / FTAP Test Signature errors are determined by comparing
   the values reported in the log packets to the expected values that were set
   by QLIB_MFLO_InitTAPMsgContent_PhyMAC.  Missing or mismatching pairs in each
   superframe are counted as errors.

   Total errors include erasures and FTAP Sequence Number / FTAP Test Signature errors.

   This function continues parsing log packets and keeping track of overall packet
   error rate statistics until the minimum number of PLPs have been measured for all
   requested Flow IDs.

   After calling QLIB_MFLO_Start_PhyMAC, this function may be called repeatedly until
   desired level of confidence is achieved. Once the current measurement is complete,
   call QLIB_MFLO_Stop_PhyMAC.  To measure the packet error rate again, call
   QLIB_MFLO_Start_PhyMAC again before calling this function.

   Usage example:

      QLIB_MFLO_InitTAPMsgContent_PhyMAC	-------- Called once at start of session

      QLIB_MFLO_Start_PhyMAC				---------
      QLIB_MFLO_GetPER_PhyMAC						|-----	Measure PER for first test
      QLIB_MFLO_Stop_PhyMAC				---------

      QLIB_MFLO_Start_PhyMAC				---------
      QLIB_MFLO_GetPER_PhyMAC						|
      QLIB_MFLO_GetPER_PhyMAC						|-----  Measure PER for second test
      QLIB_MFLO_GetPER_PhyMAC						|		Note: Example shows measurement
      QLIB_MFLO_GetPER_PhyMAC						|		      ran longer to achieve
      QLIB_MFLO_Stop_PhyMAC				---------             desired confidence
            .
            .
            .
      QLIB_MFLO_FinalCleanup_PhyMAC		-------- Called once at end of session

   This function reports the packet error rate statistics for all requested Flow IDs,
   the current system state, and an error code.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNoLogPktsTimeout = time in seconds to wait for log packets
   \param ePerToMeas = which packet error rate to measure, statistics for both Base
                  and Enhancement layers will be reported but the determination of
                  whether or not iMinPLPs have been measured depends on what this
                  variable has been set to (mftypes_per_to_measure)
   \param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
   \param eSystem = the system, required for getting the system state (mftypes_system_type)
   \param eErasureIndicator = which erasure indicator to use when counting erasures
                              (mftypes_reed_solomon)
   \param paOverallStats = an array of type mftypes_per_statistics to store the packet
                     error rate statistics for all requested Flow IDs
   \param peWideAreaSystemState = the current wide area system state if eSystem was set to
                                  WIDE or WIDE_AND_LOCAL (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state if eSystem was set to
                                   LOCAL or WIDE_AND_LOCAL (mftypes_state_type)
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed, see the warning below for two
                   exceptions (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array.  Sufficient memory for pOverallStats must be allocated by the
          calling program.

          The results reported in paOverallStats are only valid if...
            QLIB_MFLO_GetPER_PhyMAC return value is true		AND
            Requested system state(s) == ACTIVE_STATE			AND
             peErrorCode == NOT_AVAILABLE.

          If at some point during the measurement, the system state is not ACTIVE_STATE,
            the function will return but not fail.  The current system state will be reported
          through peWideAreaSystemState and peLocalAreaSystemState.  At this point
          QLIB_MFLO_Stop_PhyMAC should be called.

          If no log packets are received after the number of seconds specified by
          iNoLogPktsTimeout have elapsed, the function will return but not fail and
          peErrorCode will be set to NO_LOG_PACKETS. At this point QLIB_MFLO_Stop_PhyMAC
          should be called.

         The time period that is used when checking for status is controlled by the
         timing index QMSL_Timeout_Delay_MediaFLO_StatusCheck, to be used with the functions
         QLIB_ConfigureTimeOut() and QLIB_GetTimeOut().


          This function depends on receiving all packets for all requested Flow IDs in
          an expected order.  If any expected log packets are not received, the function
          will return but not fail and peErrorCode will be set to MISSING_LOG_PACKETS.
          At this point QLIB_MFLO_Stop_PhyMAC should be called.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_GetPER_PhyMAC( HANDLE hResourceContext, unsigned short iNoLogPktsTimeout,
         unsigned char ePerToMeas, unsigned long iMinPLPs,
         unsigned char eSystem, unsigned char eErasureIndicator,
         void* paOverallStats, unsigned char* peWideAreaSystemState,
         unsigned char* peLocalAreaSystemState, unsigned char* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function stops the current Physical layer + MAC layer packet error rate
   measurement.  It disables FTAP PLP Data Logging Mode, clears all log codes for
   LOG_EQUIP_ID_1X, and clears the log queue.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning This function should be called at the end of a packet error rate
          measurement after all calls to QLIB_MFLO_GetPER_PhyMAC for the current
          measurement are complete. See QLIB_MFLO_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_Stop_PhyMAC( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function performs final cleanup for Physical layer + MAC layer packet error
   rate measurements.  This function only needs to be called once at the end of a
   test session after all packet error rate measurements to be made with the current
   TAP Message content that was initialized by QLIB_MFLO_InitTAPMsgContent_PhyMAC
   are complete.  This function disables FTAP PLP Data Logging Mode clears all log
   codes for LOG_EQUIP_ID_1X, and clears the log queue.  It also frees up any memory
   that was dynamically allocated by QLIB_MFLO_InitTAPMsgContent_PhyMAC and
   QLIB_MFLO_Start_PhyMAC.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning To prevent memory leaks, this function must be called once at the end
          of testing when no more packet error rate measurements are to be made
          with the current TAP Message Content.  See QLIB_MFLO_GetPER_PhyMAC for
          a usage example.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FinalCleanup_PhyMAC( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   This function measures the Physical layer packet error rate.  This type of packet
   error rate measurement does not require a known nor finite sequence of FTAP packets
   from the FLO Tx Waveform.  The measurement is made by querying the accumulated PLP
   statistics for all requested Flow IDs.  Accumulated statistics continue to be
   queried until the minimum number of PLPs have been recorded.  Accumulated PLP
   statistics are queried through the MFLO_FTM_NS_GET_MLC_DYN_PARAMS function.

   After calling MFLO_FTM_NS_RESET_ALL_PLP_STATS to reset the accumulated statistics for all
   MLCs, this function may be called repeatedly until the desired level of confidence
   is achieved. To measure the packet error rate again, call MFLO_FTM_NS_RESET_ALL_PLP_STATS again
   before calling this function.

   Usage examples:

      MFLO_FTM_NS_RESET_ALL_PLP_STATS
      MFLO_FTM_NS_GetPER_Phy		--------------	Measure PER for first test


      MFLO_FTM_NS_RESET_ALL_PLP_STATS	---------
      MFLO_FTM_NS_GetPER_Phy					 |
      MFLO_FTM_NS_GetPER_Phy					 |-----  Measure PER for second test
      MFLO_FTM_NS_GetPER_Phy					 |		Note: Example shows measurement ran longer
      MFLO_FTM_NS_GetPER_Phy			---------		      to achieve desired level of confidence

   This function reports the packet error rate statistics for all requested Flow IDs,
   the current system state, and an error code.

   \param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs
   \param aiFlowIDs = array of Flow IDs for which to get accumulated PLP statistics
   \param aiMlcIDs = array of MLC IDs that correspond to the Flow IDs in aiFlowIDs,
                     this is required because MFLO_FTM_GET_MLC_PLP_STAT_DYN_PARAMS requires
                 the MLC ID not Flow ID
   \param aeSystem = array of mftypes_system_type with elements that correspond to the
                     Flow IDs in aiFlowIDs, the System for each Flow ID is required by
                 QLIB_MFLO_FTM_GET_MLC_PLP_STAT_DYN_PARAMS
   \param iNoStatsTimeout = time in seconds to wait for statistics to update, max of 5
                      seconds recommended
   \param ePerToMeas = which packet error rate to measure, statistics for both Base and
                       Enhancement layers will be reported but the determination of
                  whether or not iMinPLPs have been measured depends on what this
                  variable has been set to (mftypes_per_to_measure)
   \param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
   \param eRS = which statistics to report, Pre or Post Reed Solomon (mftypes_reed_solomon)
   \param paOverallStats = an array of type mftypes_per_statistics with length
                     iNumberOfFlowIDs to store the packet error rate statistics for
                     all requested Flow IDs
   \param peWideAreaSystemState = the current wide area system state if one of the
                           elements in aeSystem is WIDE (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state if one of the
                                   elements in aeSystem is LOCAL (mftypes_state_type)
   \param peErrorCode = an error code used whenever possible to provide more information
                        about why the function failed, see the warning below for one
                   exception (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array.  Sufficient memory for pOverallStats must be allocated by the
          calling program.

          The results reported in paOverallStats are only valid if...
            MFLO_FTM_NS_GetPER_Phy return value is true		AND
            Requested system state(s) == ACTIVE_STATE	AND
             peErrorCode == NOT_AVAILABLE.

          If at some point during the measurement, the requested system state is not
          ACTIVE_STATE,  the function will return but not fail.  The current system
          state will be reported through peWideAreaSystemState and peLocalAreaSystemState.

          If the statistics for one or more of the MLC IDs in aiMlcIDs have not updated
          after the number of seconds specified by iNoStatsTimeout have elapsed, the
          function will return but not fail. peErrorCode will be set to PLP_STATS_NOT_UPDATING.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GetPER_Phy( HANDLE hResourceContext,
         unsigned char iNumberOfFlowIDs,
         unsigned long* aiFlowIDs,
         unsigned char* aiMlcIDs,
         unsigned long* aeSystem,
         unsigned short iNoStatsTimeout,
         unsigned long ePerToMeas,
         unsigned long iMinPLPs,
         unsigned long eRS,
         void* paOverallStats,
         unsigned long* peWideAreaSystemState,
         unsigned long* peLocalAreaSystemState,
         unsigned long* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls several of the MediaFLO FTM
   API's to perform a specialized task.

   This function performs initialization steps required before making a new
   Physical layer + MAC layer packet error rate measurement.  This type of packet
   error rate measurement requires a known and finite sequence of FTAP packets from
   the FLO Tx Waveform.  This function resets the overall packet error rate statistics,
   updates data structures used in the packet error rate measurement, clears the log
   queue, registers the following log codes:

         LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
         LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

   and enables FTAP PLP Data Logging Mode by through QLIB_FTM_MFLO_NS_ENABLE_FTAP_PLP_DATA.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs, must match the actual
                       number of active FlowIDs or this funtion will fail
   \param aiFlowIDs = array of Flow IDs for which to measure the packet error rate
   \param paOverallStats = an array of type mftypes_per_statistics with length
                     iNumberOfFlowIDs to store the packet error rate
                     statistics for all requested Flow IDs
   \param peErrorCode = an error code used whenever possible to provide more
                   information about why the function failed (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array. Sufficient memory for pOverallStats must be allocated by the
          calling program.

          QLIB_MFLO_InitTAPMsgContent_PhyMAC must be called once before the first
          time this function is called.

          This function must be called once before calling QLIB_FTM_MFLO_NS_GetPER_PhyMAC.
          The function QLIB_FTM_MFLO_NS_Stop_PhyMAC should be called at the end of the packet
          error rate measurement. To make a new packet error rate measurement,
          this function must be called again before calling QLIB_FTM_MFLO_NS_GetPER_PhyMAC.
          See QLIB_FTM_MFLO_NS_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_Start_PhyMAC( HANDLE hResourceContext,
         unsigned char iNumberOfFlowIDs,
         unsigned long* aiFlowIDs,
         void* paOverallStats,
         unsigned long* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function measures the Physical layer + MAC layer packet error rate. This
   type of packet error rate measurement requires a known and finite sequence of
   FTAP packets from the FLO Tx Waveform.

   The measurement is made by getting erasure and FTAP Sequence Number /
   FTAP Test Signature information from log packets with the following log codes:

      LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
      LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

   FTAP Sequence Number / FTAP Test Signature errors are determined by comparing
   the values reported in the log packets to the expected values that were set
   by QLIB_MFLO_InitTAPMsgContent_PhyMAC. Missing or mismatching pairs in each
   superframe are counted as errors.

   Total errors include erasures and FTAP Sequence Number / FTAP Test Signature errors.

   This function continues parsing log packets and keeping track of overall packet
   error rate statistics until the minimum number of PLPs have been measured for all
   requested Flow IDs.

   After calling QLIB_MFLO_Start_PhyMAC, this function may be called repeatedly until
   desired level of confidence is achieved. Once the current measurement is complete,
   call QLIB_MFLO_Stop_PhyMAC.  To measure the packet error rate again, call
   QLIB_MFLO_Start_PhyMAC again before calling this function.

   Usage example:

      QLIB_MFLO_InitTAPMsgContent_PhyMAC	-------- Called once at start of session

      QLIB_FTM_MFLO_NS_Start_PhyMAC		---------
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|-----	Measure PER for first test
      QLIB_FTM_MFLO_NS_Stop_PhyMAC		---------

      QLIB_FTM_MFLO_NS_Start_PhyMAC		---------
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|-----  Measure PER for second test
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|		Note: Example shows measurement
      QLIB_FTM_MFLO_NS_GetPER_PhyMAC				|		      ran longer to achieve
      QLIB_FTM_MFLO_NS_Stop_PhyMAC		---------             desired confidence

      QLIB_MFLO_FinalCleanup_PhyMAC		-------- Called once at end of session

   This function reports the packet error rate statistics for all requested Flow IDs,
   the current system state, and an error code.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()
   \param iNoLogPktsTimeout = time in seconds to wait for log packets
   \param ePerToMeas = which packet error rate to measure, statistics for both Base
                  and Enhancement layers will be reported but the determination of
                  whether or not iMinPLPs have been measured depends on what this
                  variable has been set to (mftypes_per_to_measure)
   \param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
   \param eSystem = the system, required for getting the system state (mftypes_system_type)
   \param eErasureIndicator = which erasure indicator to use when counting erasures
                              (mftypes_reed_solomon)
   \param paOverallStats = an array of type mftypes_per_statistics to store the packet
                     error rate statistics for all requested Flow IDs
   \param peWideAreaSystemState = the current wide area system state if eSystem was set to
                                  WIDE or WIDE_AND_LOCAL (mftypes_state_type)
   \param peLocalAreaSystemState = the current local area system state if eSystem was set to
                                   LOCAL or WIDE_AND_LOCAL (mftypes_state_type)
   \param peErrorCode = an error code used whenever possible to provide more information
                   about why the function failed, see the warning below for two
                   exceptions (mftypes_error_code)

   \return true if successful, false if fail.

   \warning This function does not dynamically allocate memory for the pOverallStats
          array. Sufficient memory for pOverallStats must be allocated by the
          calling program.

          The results reported in paOverallStats are only valid if
            QLIB_FTM_MFLO_NS_GetPER_PhyMAC return value is true		AND
            Requested system state(s) == ACTIVE_STATE				AND
            peErrorCode == NOT_AVAILABLE.

          If at some point during the measurement, the system state is not ACTIVE_STATE,
          the function will return but not fail. The current system state will be reported
          through peWideAreaSystemState and peLocalAreaSystemState. At this point
          QLIB_FTM_MFLO_NS_Stop_PhyMAC should be called.

          If no log packets are received after the number of seconds specified by
          iNoLogPktsTimeout have elapsed, the function will return but not fail and
          peErrorCode will be set to NO_LOG_PACKETS. At this point QLIB_FTM_MFLO_NS_Stop_PhyMAC
          should be called.

         The time period that is used when checking for status is controlled by the
         timing index QMSL_Timeout_Delay_MediaFLO_StatusCheck, to be used with the functions
         QLIB_ConfigureTimeOut() and QLIB_GetTimeOut().

          This function depends on receiving all packets for all requested Flow IDs in
          an expected order. If any expected log packets are not received, the function
          will return but not fail and peErrorCode will be set to MISSING_LOG_PACKETS.
          At this point QLIB_FTM_MFLO_NS_Stop_PhyMAC should be called.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_GetPER_PhyMAC( HANDLE hResourceContext,
         unsigned short iNoLogPktsTimeout,
         unsigned long ePerToMeas,
         unsigned long iMinPLPs,
         unsigned long eSystem,
         unsigned long eErasureIndicator,
         void* paOverallStats,
         unsigned long* peWideAreaSystemState,
         unsigned long* peLocalAreaSystemState,
         unsigned long* peErrorCode );

   /******************************************************************************/
   /**
   This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
   API's to perform a specialized task.

   This function stops the current Physical layer + MAC layer packet error rate
   measurement. It disables FTAP PLP Data Logging Mode, clears all log codes for
   LOG_EQUIP_ID_1X, and clears the log queue.

   \param hResourceContext = Resource context that was returned from the call to ConnectServer()

   \return true if successful, false if fail.

   \warning This function should be called at the end of a packet error rate
          measurement after all calls to QLIB_FTM_MFLO_NS_GetPER_PhyMAC for the current
          measurement are complete. See QLIB_FTM_MFLO_NS_GetPER_PhyMAC for usage examples.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_MFLO_FTM_NS_Stop_PhyMAC( HANDLE hResourceContext );



	/******************************************************************************
                     ISDB-T DIAG
   *******************************************************************************/

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.2 Subcommand 100

      This command tells ISDB-T L1 to acquire on the given frequency, mode and guard.
      ISDB-T L1 must be powered up via QLIB_DIAG_ISDB_UBM_L1_POWERUP_CMD() before this command is sent.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFrequency = Frequency to acquire. Value in Hz/10

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning  Must call QLIB_DIAG_ISDB_UBM_L1_POWERUP_CMD() first.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_ACQ_CMD(HANDLE hResourceContext,
         unsigned long iFrequency);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.3 Subcommand 101

      This command stops the DSP and puts ISDB layer state machine in reset

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RESET_CMD(HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.4 Subcommand 102

      This function returns the acquisition state of ISDB-T L1

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piAcquisition_sucess = pointer to the location to store the acquestion status
                           0 indicates acquisition failed (mode and guard invalid)
                           1 indicates acquisition succeeded
   \param piMode = Pointer to location to store the mode of the acquired signal.
      Defined by ubm_isdb_mode in QLib_Defines.h:
      \code
      ISDB_UBM_L1_MODE_2 = 2
      ISDB_UBM_L1_MODE_3 = 3
      \endcode
   \param piGuard = Pointer to location to store the guard of the acquired signal.
      Defined by ubm_isdb_guard in QLib_Defines.h:
      \code
      ISDB_UBM_L1_GUARD_4		= 0
      ISDB_UBM_L1_GUARD_8		= 1
      ISDB_UBM_L1_GUARD_16	= 2
      \endcode

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_ACQ_STATUS (HANDLE hResourceContext,
         unsigned char *piAcquisition_success,
         unsigned char *piMode,
         unsigned char *piGuard );

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.5 Subcommand 103

      This command powers up the MBP receiver and initializes ISDB-T L1.
      This command should be called before any other ISDB-T L1 non-signaling commands.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_POWERUP_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.6 Subcommand 104

      This command powers down L1.
      This command should be called when exiting ISDB-T non-signalling mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_POWERDOWN_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.7 Subcommand 105

      This command routes the Y1Y2 samples to the debug bus.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_ROUTE_Y1Y2_DEBUG_BUS (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 106

      This command collects packet counter statistics regarding tsif packets arrived.
      This command is preserved for compatibility - it is recommended to use
      UBM_L1_ISDB_RS1_PACKET_CNT_V2_CMD (140) instead.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iRs1PacketCount = pointer to store the RS1 packet counter
   \param iRs1TrashedPacketCount = pointer to store the RS1 trashed packet counter

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RS1_PACKET_CNT_CMD (HANDLE hResourceContext,
         unsigned long* piRs1PacketCount,
         unsigned long* piRs1TrashedPacketCount);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.9 Subcommand 107

      This command records a given amount of tsif packets and saves them into a file in the efs
      (history of theses files is not save)

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iNumOfPackets = Number of tsif packets to record.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RECORD_TSIF_PACKETS_CMD (HANDLE hResourceContext,
         unsigned short iNumOfPackets);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.10 Subcommand 108

      When CIR logging is enabled the CIR information is logged once every nth frame.
      This command configures n.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning This command is currently not supported.

   *******************************************************************************/
   //QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_CONFIGURE_CIR_LOG_PARAMS (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T DIAG, 5.3.11 Subcommand 109

      This command sets up the VBER one-bit error test.
      The test box is set up to transmit packets containing all 0 bits.
      The SW then counts the number of bits set to 1 in the tsif output for each packet.
      This count can be read back using the UBM_L1_ISDB_VBER_ONE_BIT_STATUS (Section 5.3.12) command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLength = Number of packets to run the test over
   \param iThreshold = Threshold for determining if the packet is considered “good”
   \param iPid = Only packets with this PID will be tested (this is done to filter any “unwanted” PIDs)

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_PRBS_ONE_BIT_CMD (HANDLE hResourceContext,
         unsigned long iLength,
         unsigned long iThreshold,
         unsigned short iPid);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.12 Subcommand 110

      This command returns the number of 1 bits counted in the PRBS one-bit test (Section 5.3.11).

   \param hResourceContext		= Resource context that was returned from the call to QLIB_ConnectServer().
   \param piTestDone			= pointer to store test Status, 0 indicates the test is not finished 1 = test finished.
   \param piOnesCount			= pointer to store the total number of ones counted in the test.
   \param piGoodPacketCount	= pointer to store the number of packets used for this test (should be equal the length requested).
   \param piBadPacketCount		= pointer to store the number of invalid packets got
                          (that has more than the threshold number of ones in them).
   \param piError				= pointer to store the error flag.
                          0 - Success
                          1 - Too many unmatched Pids. Received more than a thousand consecutive unmatched PIDs found.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_PRBS_ONE_BIT_STATUS (HANDLE hResourceContext,
         unsigned char *piTestDone,
         unsigned long *piOnesCount,
         unsigned long *piGoodPacketCount,
         unsigned long *piBadPacketCount,
         unsigned char *piError);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.13 Subcommand 111

      This command will enable the LNA update loop.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_ENABLE_LNA_UPDATE (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.14 Subcommand 112

      This command disables the LNA update loop.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_DISABLE_LNA_UPDATE (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.15 Subcommand 113

      This command changes pre-FFT jammer parameters. This command has to be sent prior to the acquisition command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iNoOfJammers = Needs to be between 0 to 6
   \param piJammerGainBuffer = Pointer to buffer containing the jammer gains (one for each jammer)
   \param piNotchIBuffer = Pointer to buffer containing Notch I values (one for each jammer)
   \param piNotchQBuffer = Pointer to buffer containing Notch Q values (one for each jammer)

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_CFG_JAMMER_CMD (HANDLE hResourceContext,
         unsigned char iNoOfJammers,
         const unsigned char* piJammerGainBuffer,
         const unsigned short* piNotchIBuffer,
         const unsigned short* piNotchQBuffer);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.16 Subcommand 114

      This command triggers ISDB L1 to display and return the current RSSI information.

   \param hResourceContext	= Resource context that was returned from the call to QLIB_ConnectServer()
   \param pRssiQ6			= Pointer to location to store required Signal Strength Information in Q6 format
   \param pLnaState		= Pointer to location to store Low Noise Amplifier state
   \param pDbAdcFullScaleQ6= Pointer to location to store dbAdcFullScaleQ6 in Q6 format
   \param pLog2eeQ6		= Pointer to location to store Log2 of the normalized energy estimate in Q6 format.
   \param pAgcFiltStateQ12	= Pointer to location to store DVGA gain in Q12 log2 format

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning The system must be acquired.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RSSI_STATUS (HANDLE hResourceContext,
         short *pRssiQ6,
         unsigned char *pLnaState,
         short *pDbAdcFullScaleQ6,
         short *pLog2eeQ6,
         short *pAgcFiltStateQ12);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.17 Subcommand 115

      This command triggers ISDB L1 to enter the recovery state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RECOVERY_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.18 Subcommand 116

      This command configures RDSP data logging.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLogItem = The log item that is being configured:
                  0 = CPCE
                  1 = CPCE Utilized Pathlist
                  2 = CPCE Candidate Pathlist
                  3 = CIR (ISDB-T only)
                  4 = Acquisition Likelihood
                  5 = Jammer FFT Output
                  6 = RDSP periodic logging
                  7 = TMCC info
                  8 = Detailed Acquisition
                  9 = Subcarrier Monitoring
   \param iFlag = For CIR:
               Byte 1: Set to 0.
               Bytes 2-3: Logging Interval in OFDM symbols. A value of 0 disables the CIR logging. A non-zero value enables the CIR logging.
               Byte 4: 0-Disable, 1-Enable
               For RDSP Periodic:
               Byte 1: RDSP Log Item. (See Table 3-18).
               Bytes 2-3: Logging Interval in OFDM symbols.
               Byte 4: 0-Disable, 1-Enable
               For TMCC:
               Byte 1: Set to 0.
               Bytes 2-3: Logging Interval in OFDM frames. A value of 0 disables the TMCC logging. A non-zero value enables the TMCC logging.
               Byte 4: Set to 0
               For Sub-carrier Monitoring:
               Bytes 1-2: Sub-carrier index to log (Index according to ISDB-T standard).
               Byte 3: Mode of operation (2 or 3).
               Byte 4: 0-Disable, 1-Enable.
               For Acquisition Likelihood, Jammer FFT output, Detailed Acquisition:
               Byte 1: 0-Disable, 1-Enable

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_CFG_RDSP_LOG_CMD (HANDLE hResourceContext,
         unsigned char iLogItem,
         unsigned long iFlag);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.19 Subcommand 117

      This command is used to configure post-FFT jammer parameters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iMode = 1 - UBM_L1_MODE_1
               2 - UBM_L1_MODE_2
               3 - UBM_L1_MODE_3
   \param iThreshold = Post-FFT Threshold for iMode
   \param iGain = Post FFT Gain for iMode
   \param iRegion0Odd = Odd Value for Region 0
   \param iRegion0Even = Even Value for Region 0
   \param iRegion1Odd = Odd Value for Region 1
   \param iRegion1Even = Even Value for Region 1
   \param iRegion2Odd = Odd Value for Region 2
   \param iRegion2Even = Even Value for Region 2
   \param iRegion3Odd = Odd Value for Region 3
   \param iRegion3Even = Even Value for Region 3
   \param iRegion4Odd = Odd Value for Region 4
   \param iRegion4Even = Even Value for Region 4
   \param iRegion5Odd = Odd Value for Region 5
   \param iRegion5Even = Even Value for Region 5
   \param iRegion6Odd = Odd Value for Region 6
   \param iRegion6Even = Even Value for Region 6
   \param iRegion7Odd = Odd Value for Region 7
   \param iRegion7Even = Even Value for Region 7
   \param iRegionEnable = Region enable for iMode

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_CFG_POST_FFT_JAMMER_CMD (HANDLE hResourceContext,
         unsigned char iMode,
         unsigned char iThreshold,
         unsigned char iGain,
         unsigned short iRegion0Odd,
         unsigned short iRegion0Even,
         unsigned short iRegion1Odd,
         unsigned short iRegion1Even,
         unsigned short iRegion2Odd,
         unsigned short iRegion2Even,
         unsigned short iRegion3Odd,
         unsigned short iRegion3Even,
         unsigned short iRegion4Odd,
         unsigned short iRegion4Even,
         unsigned short iRegion5Odd,
         unsigned short iRegion5Even,
         unsigned short iRegion6Odd,
         unsigned short iRegion6Even,
         unsigned short iRegion7Odd,
         unsigned short iRegion7Even,
         unsigned short iRegionEnable);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.20 Subcommand 118

      This command enables/disables use of RSSI calibration information.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iUseRssiCal = TRUE if RSSI calibration data should be used, FALSE otherwise

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_USE_RSSI_CAL_CMD (HANDLE hResourceContext,
         unsigned char iUseRssiCal);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.21 Subcommand 119

      This command sets the LNA state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLnaState = LNA state

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_SET_LNA_CMD (HANDLE hResourceContext,
         unsigned char iLnaState);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.22 Subcommand 120

      This command sets the address for RDSP periodic logging dynamic items.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iDynamicItem = RSDP Periodic Logging Dynamic Item Number (0-7)
   \param iAddress = ARM address offset of the RDSP item to be logged

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_SET_PERIODIC_LOG_ADDR (HANDLE hResourceContext,
         unsigned char iDynamicItem,
         unsigned long iAddress);

   /******************************************************************************/
   /**
   ISDB-T DIAG, 5.3.24 Subcommand 122

      This command controls the antenna tune PDM pin.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iEnable = Enable/disable
   \param iPolarity = Polarity
   \param iInput = Density

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_ANT_TUNE_PDM_CONTROL_CMD (HANDLE hResourceContext,
         unsigned char iEnable,
         unsigned char iPolarity,
         unsigned short iInput);

   /******************************************************************************/
   /**
   ISDB-T DIAG, 5.3.24 Subcommand 123

      This command is used to write specific RDSP registers.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iItem = The RDSP item to write
   \param iOffset = Offset
   \param iValue = Value

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RDSP_WRITE_CMD (HANDLE hResourceContext,
         unsigned char iItem,
         unsigned char iOffset,
         unsigned long iValue);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 124

      This command gets the TMCC Frame information field of the TMCC.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piTmccRawData = pointer to store the TMCC Information field

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_GET_TMCC_FRAME_INFO_CMD (HANDLE hResourceContext,
         unsigned long* piTmccRawData);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 125

      This command can set or read a given L1 configuration item.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iGet_set_item = Get/Set the item. 0 sets item to a value. 1 to gets the item value.
   \param	iItem = configuration item to set/get.
      Defined by ubm_isdb_configuration_items in QLib_Defines.h
   \param iValue = value for configuration item.
   \param	piItem = pointer where the configuration item that was set is stored.
      Defined by ubm_isdb_configuration_items in QLib_Defines.h
   \param piValue = pointer to location to store the value of configuration item.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_CFG_ITEM (HANDLE hResourceContext,
         unsigned char iGet_set_item,
         unsigned char iItem,
         unsigned long iValue,
         unsigned char *piItem,
         unsigned long *piValue);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 126

      PEEK_POKE command allows the user to read/write registers values of the DSP/HW memory region.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iIfDsp = Registers Interface 0 - hw, 1 – dsp
   \param iIfRead = operation type 0 - reading operation 1 - writing operation
   \param iValue = value for configuration item.
   \param iAddrOffset = Addr offset from BASE ADDRESS addr_offset is added to the BASE ADDRESS
      which is defined differently for HW/DSP.
      BOFDM_BASE : hold the HW base address
      MICRO_RDSP_BASE : hold the DSP base address
      The HW/DSP Bases address are defined differently for SURF and FFA.
   \param iValue = Value – Valid only for write operation.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_PEEK_POKE_CMD (HANDLE hResourceContext,
         unsigned char iIfDsp,
         unsigned char iIfRead,
         unsigned long iAddrOffset,
         unsigned long iValue);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 127

      TBD.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RESET_PKT_TRASH_COUNTERS_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 128

      TBD.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_HARD_RECOVERY_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 129

      SAMPSRV_DUMP command handles a sample server dump command from QXDM.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iStart = Start
   \param iSize = Size

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_SAMPSRV_DUMP_CMD (HANDLE hResourceContext,
         unsigned long iStart,
         unsigned long iSize);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 130

      ESR_RESET command resets the ESR counters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_ESR_RESET_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 131

      PERF_CNTS_RESET command resets the counters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_PERF_CNTS_RESET_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 132

      PERF_CNTS_SET_PRINT_MASK command sets the counters to be printed by sending mask value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iMask = The print mask

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_PERF_CNTS_SET_PRINT_MASK_CMD (HANDLE hResourceContext,
         unsigned long iMask);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 133

      LOG_ACQ command handles request for acquisition status log.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_LOG_ACQ_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 134

      GET_TECHNOLOGY command returns and also print out the current working technology of the target and
      the supported technologies.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piCurrentTechnology = pointer to store the current technology field
   \param piSupportedTechnology = pointer to store the supported technologies field

   \warning This command is preserved for compatibility – use DIAG_ISDB_UBM_L1_GET_VERSION_INFO_CMD (139) instead.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_GET_TECHNOLOGY_CMD (HANDLE hResourceContext,
         unsigned long* piCurrentTechnology,
         unsigned char* piSupportedTechnology);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 135

      RDSP_HALT command halts the RDSP.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RDSP_HALT_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 136

      Retry the last acquisition.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning This command is currently not supported

   *******************************************************************************/
   //QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RETRY_ACQ_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 137

      Returns the current state of the L1 state machine.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piCurrentState = pointer to store the current state

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_GET_CURRENT_STATE_CMD (HANDLE hResourceContext,
         unsigned long* piCurrentState);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.1 Subcommand 138

      Restore all L1 SW configuration values to their defaults.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RESTORE_DEFAULT_CFG_CMD (HANDLE hResourceContext);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 139

      Returns version information and supported technologies information.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piCurrentTechnology = pointer to store the current technology field
   \param piSupportedTechnology = pointer to store the supported technologies field
   \param piSWVersion = pointer to store the software version
   \param piSWRelease = pointer to store the software release
   \param piSWBuild = pointer to store the software build
   \param piFWVersion = pointer to store the firmware release
   \param piFWSubVersion = pointer to store the firmware sub-version
   \param piData = pointer to store the 30 byte data array
   \param piTime = pointer to store the 30 byte time array

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_GET_VERSION_INFO_CMD (HANDLE hResourceContext,
         unsigned char* piCurrentTechnology,
         unsigned char* piSupportedTechnology,
         unsigned long* piSWVersion,
         unsigned long* piSWRelease,
         unsigned long* piSWBuild,
         unsigned short* piFWVersion,
         unsigned short* piFWSubVersion,
         char* piData,
         char* piTime);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 140

      Returns the RS1 (Reed Solomon) and ESR (Error Second Rate) cumulative counters.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piRS1PacketCount = pointer to store the RS1 packet count
   \param piRS1TrashCount = pointer to store the RS1 trash count
   \param piESRTotalSeconds = pointer to store the ESR total seconds
   \param piESRBadSeconds = pointer to store the ESR bad seconds

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_RS1_PACKET_CNT_RES_V2_CMD (HANDLE hResourceContext,
         unsigned long* piRS1PacketCount,
         unsigned long* piRS1TrashCount,
         unsigned long* piESRTotalSeconds,
         unsigned long* piESRBadSeconds);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, 5.3.8 Subcommand 141

      Returns various parameters used in the L1-L3 API. This command is used to debug the L1-L3 API.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iParameterID = the ID of the parameter to be queried
   \param piItemSize = pointer to store the size in bytes of the returned item data
   \param piStatus = pointer to store the status (0-success, 1-failure)
   \param piItemData = pointer to a buffer to store the item data
   \param iItemDataBufferSize = the amount of memory allocated by the caller for the item data buffer

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_GET_PARAM_VAL_CMD (HANDLE hResourceContext,
         unsigned char iParameterID,
         unsigned long* piItemSize,
         unsigned char* piStatus,
         unsigned char* piItemData,
         unsigned long iItemDataBufferSize);

   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, Subcommand 143

      Configures UBM tracking loops.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTrackingLoopId = The tracking loop ID: 0 - Time, 1 - Frequency, 2 - Coarse DC, 3 - Fine DC, 4 - AGC
   \param iOperation = 0 - Disable, 1 - Enable, 2 - Reset and disable (valid only for fine DC)

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_HANDLE_TRACK_LOOPS_CFG_CMD(HANDLE hResourceContext,
         unsigned char iTrackingLoopId,
         unsigned char iOperation);


   /******************************************************************************/
   /**
   ISDB-T L1 DIAG, Subcommand 144

      Gets UBM tracking loops status.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param piIsTrackingLoopEnabled = Bit 0 - Time, Bit 1 - Frequency, Bit 2 - Coarse DC, Bit 3 - Fine DC, Bit 4 - AGC.
                            1 means that the loop is enabled. 0 means that the loop is disabled.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_ISDB_UBM_L1_GET_TRACK_LOOPS_STATUS_CMD(HANDLE hResourceContext,
         unsigned char* piIsTrackingLoopEnabled);


   /******************************************************************************
                     MBP RF DIAG
   *******************************************************************************/

   /******************************************************************************/
   /**
   MBP RF DIAG, Subcommand 12

      This command sets a configuration item

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iItem = Defined by mbprftypes_cfg_item_type in QLib_Defines.h:
      \code
         MBP_RF_CFG_NB_JD_ENABLED = 31,	//"< ' Narrowband Jammer Detection Control (0=disable,1=enable)
         MBP_RF_CFG_WB_JD_ENABLED = 32	//"< ' Wideband Jammer Detection Control (0=disable,1=enable)
      \endcode
   \param iValue = Value to set.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_MBP_RF_SET_CFG_ITEM_CMD(HANDLE hResourceContext,
         unsigned char iItem,
         unsigned long iValue);

   /******************************************************************************/
   /**
   MBP RF DIAG, Subcommand 21

   This command sets the Jammer Detect Operation Mode

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iMode = Defined by mbprftypes_jd_mode_type in QLib_Defines.h:
      \code
         MBP_RF_JD_MODE_AUTO = 0,
         MBP_RF_JD_MODE_1    = 1,
         MBP_RF_JD_MODE_2    = 2
      \endcode

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_MBP_RF_SET_OPERATION_MODE_CMD(HANDLE hResourceContext,
         unsigned char iMode);

   /******************************************************************************/
   /**
   MBP RF DIAG, Subcommand 30
   This command can read values from RFIC / ADC registers.
   This command can write values to RFIC / ADC registers.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iHwBlock = 0 for RFIC, 1 for ADC
   \param iIsWrite = 0 for READ operation, 1 for WRITE operation
   \param iAddress = Register address
   \param piVal    = Value to write, in WRITE operation. The read value is returned in this
      field, for READ operations.

   \return 1 if response packet was successfully returned, 0 if it failed for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_MBP_RF_PEEK_POKE_CMD(HANDLE hResourceContext,
         unsigned char iHwBlock,
         unsigned char iIsWrite,
         unsigned char iAddress,
         unsigned char *piVal);

   #ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QMSL_MEDIAFLO_BROADCAST_H)
