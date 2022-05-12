/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFCal/RFCal_GSM.cpp#6 $
 * $DateTime: 2017/09/26 18:18:33 $
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

#include "RFCal_GSM.h"
#include "RFCal_Helper.h"
#include <sstream>
#include <list>

using namespace std;

unsigned char RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(HANDLE hPhone, RFMPathDB & rfmPathDB, FTM_RF_Mode_Enum rfMode, 
                  CGSM_Cal_Device_SigPath& bandDevSigPaths, 
                  unsigned short antennaPath, unsigned short nvContainer,unsigned short rx_chainID_selection,
                  GSM_RFC_QUERY_TYPE rfcQueryType)
{
   unsigned short rxNvContainer = nvContainer;
   unsigned short txNvContainer  = 0; // Hard-code txNvContainer to 0

   /* Query path info repository */
  
   bool prxRecordFound = false, txRecordFound = false;

   unsigned short band = 0;
   switch(rfMode)
   {
      case PHONE_MODE_GSM_850:
         band = 0;
         break;
      case PHONE_MODE_GSM_900:
         band = 1;
         break;
      case PHONE_MODE_GSM_1800:
         band = 2;
         break;
      case PHONE_MODE_GSM_1900:
         band = 3;
         break;
      default:
         return false;
   }


   //Get PRx Indices based on Path and Action Criteria
   std::vector < CRFPathInformationPacket > prxIndices;
   prxIndices.clear();
   std::vector < CRFPathInformationPacket > txIndices;
   txIndices.clear();

   { /* Get RFM Path by query */
      //PRx Criteria
      vector<PATH_TYPE> prxCriteria,drxCriteria,txCriteria;
      prxCriteria.push_back(PATH_TYPE_PRX);

      // Tx Criteria
      vector<ACTION_TYPE> actionCriteria;
      txCriteria.push_back(PATH_TYPE_TX);

      //Action Criteria
      actionCriteria.push_back(ACTION_TYPE_CAL);
      actionCriteria.push_back(ACTION_TYPE_OFFSET_CAL);
       
      rfmPathDB.QueryRFMPathInfoByPathType(hPhone, TECHNOLOGY_GSM, band, 0, prxCriteria,actionCriteria,prxIndices);
      if(prxIndices.empty())
      {
         return false;
      }
      //Get Tx Indices based on Path and Action Criteria
      rfmPathDB.QueryRFMPathInfoByPathType(hPhone, TECHNOLOGY_GSM, band, 0, txCriteria,actionCriteria,txIndices);
      if(txIndices.empty())
      {
         return false;
      }
   }
   if(rfcQueryType == GSM_TX_BASED_QUERY)
   {
      std::vector < CRFPathInformationPacket >::iterator tx_it = txIndices.begin();
      for (; tx_it != txIndices.end(); tx_it++)
      {
         if (tx_it->antennaPath == antennaPath && tx_it->nvcontainer == txNvContainer)
         {
            txRecordFound = true;
            break;
         }
      }
      if(!txRecordFound)
      {
         // If not specified nv container/antenna path, then use first one
         tx_it = txIndices.begin();
      }
      if (tx_it != txIndices.end())
      {
         bandDevSigPaths.tx.signal_path     = static_cast <int> (tx_it->signalPath);
         bandDevSigPaths.tx.rfm_dev         = static_cast <int> (tx_it->rfmDevice);
         bandDevSigPaths.tx.antenna_port = static_cast <int> (tx_it->antennaPath);
      }
      else
      {
         return false;
      }
      std::vector < CRFPathInformationPacket >::iterator rx_it = prxIndices.begin();
      for (; rx_it != prxIndices.end(); rx_it++)
      {
         if ( (rx_it->signalPath == tx_it->associatedRxSignalPath)
            && (rx_it->rfmDevice == tx_it->associatedRxRFMDevice)
            && (rx_it->antennaPath == tx_it->associatedRxAntennaPath))
         {
            prxRecordFound = true;
            break;
         }
      }

      if(!prxRecordFound)
      {
      //   ostr.str("");
      //   ostr.str("No PRx path is associated with specified Tx Path in RF Config Repository! Assigning first entry in RFC Table for Path Type PRx");
      //   m_pUIM->PrintToDebug(ostr.str());

      //   prxInfo = rfcBandPathInfo.GetRecord(*prxIndices.begin());
      }
      if ( rx_it != prxIndices.end() )
      {
         bandDevSigPaths.prx.signal_path     = static_cast <int> (rx_it->signalPath);
         bandDevSigPaths.prx.rfm_dev         = static_cast <int> (rx_it->rfmDevice);
         bandDevSigPaths.prx.antenna_port = static_cast <int> (rx_it->antennaPath);

         bandDevSigPaths.drx.signal_path     = static_cast <int> (rx_it->associatedRxSignalPath);
         bandDevSigPaths.drx.rfm_dev         = static_cast <int> (rx_it->associatedRxRFMDevice);
         bandDevSigPaths.drx.antenna_port = static_cast <int> (rx_it->associatedRxAntennaPath);
      }

   }
   else // GSM_RX_BASED_QUERY
   {
      vector <CRFPathInformationPacket>::iterator rx_it = prxIndices.begin();
      for (; rx_it != prxIndices.end(); rx_it++)
      {
         if (rx_it->antennaPath == antennaPath && static_cast <unsigned short> ( rx_it->signalPath) == rx_chainID_selection)
         {
            prxRecordFound = true;
            break;
         }
      }

      if(!prxRecordFound)
      {
         return false;
      }

      bandDevSigPaths.prx.signal_path     = static_cast <int> (rx_it->signalPath);
      bandDevSigPaths.prx.rfm_dev         = static_cast <int> (rx_it->rfmDevice);
      bandDevSigPaths.prx.antenna_port = static_cast <int> (rx_it->antennaPath);

      bandDevSigPaths.drx.signal_path     = static_cast <int> (rx_it->associatedRxSignalPath);
      bandDevSigPaths.drx.rfm_dev         = static_cast <int> (rx_it->associatedRxRFMDevice);
      bandDevSigPaths.drx.antenna_port = static_cast <int> (rx_it->associatedRxAntennaPath);

      vector <CRFPathInformationPacket>::iterator tx_it = txIndices.begin();
      for (; tx_it != txIndices.end(); tx_it++)
      {
         if (  (rx_it->signalPath == tx_it->associatedRxSignalPath)
            && (rx_it->rfmDevice == tx_it->associatedRxRFMDevice)
            && (rx_it->antennaPath == tx_it->associatedRxAntennaPath))
         {
            txRecordFound = true;
            break;
         }
      }

      if(!txRecordFound)
      {
         stringstream ss;
         ss << "No Tx path is associated with specified PRx Path in RF Config Repository! Assigning first entry in RFC Table for Path Type Tx";
         RFCal::Helper::Log::AddMessageToQMSLLog(hPhone,ss.str());
         tx_it = txIndices.begin();
      }

      bandDevSigPaths.tx.signal_path     = static_cast <int> (tx_it->signalPath);
      bandDevSigPaths.tx.rfm_dev         = static_cast <int> (tx_it->rfmDevice);
      bandDevSigPaths.tx.antenna_port = static_cast <int> (tx_it->antennaPath);
   }
   return true;
}
unsigned char RFCal::GSM_Cal::DO_GSM_V2_RX_Cal(HANDLE hPhone)
{
   GSM_CAL_Results results;
   GSM_CAL_Params params;
   deviceMutex = CreateMutex( NULL, FALSE, NULL); // Create mutex. This must be created outside this funciton. For demo purpose, created here.

   // ***************************** //
   // Assign parameters
   // ***************************** //
   AssignRxParams(hPhone, params);

   RFCal::Helper::Log::StartQMSLLogging(hPhone, params.FilePath.stQMSLLog);

   // ************************************************ //
   // Get RFM Path info and save it to the data base ( this will be done only per tech, NOT per band )
   // ************************************************ //
   if (!(params.repository.rfmPathDB.CreateRFMPathDB(hPhone, TECHNOLOGY_GSM, -1 /* all band */, 0)))
   {
      cout << "Failed in getting RFM Path info " << endl;
      return false;
   }

   GSM_Rx_CAL_Params & rxParams = params.rxParams;
   // ***************************** //
   // Set equipment
   // ***************************** //
   RFCal::DeviceDB::GetInstance().RegisterDevice (hPhone, params.DeviceConfig.type, params.DeviceConfig.boardId, params.DeviceConfig.primaryAddr, params.DeviceConfig.secondaryAddr );
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   
   if ( NULL == pMgr ) return false;
   
   pMgr->Set_RFMode ( params.rf_mode );
   pMgr->Set_ULChannel(static_cast<int>(params.channel));
   pMgr->Set_SimulationMode(false);
   pMgr->Get_Version();
   RFCal::SetPathLoss(hPhone, *pMgr, params.FilePath.stCalDBXML,"GSM");

   // ***************************** //
   // Set RFM path and Set mode
   // ***************************** //
   RFCal::GSM_Cal::INIT_RX(hPhone, params);

   // ***************************** //
   // Calibrate Rx FreqComp
   // ***************************** //
   RFCal::GSM_Cal::RX_Cal::DO_GSM_Rx_FreqComp(hPhone, params, results);

   // ***************************** //
   // Log Result
   // ***************************** //
   RFCal::GSM_Cal::LOG::Rx_FreqComp( hPhone, params, results);

   // ***************************** //
   // Process NV
   // ***************************** //
   if ( params.writeConsolidatedRxNvVersion != 0 )
   {

      std::vector<QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated> gsm_rx_chain_data;
      QMSL_RFNV_RX_CAL_GSM_NV gsm_rx_cal_nv;

      if( RFCal::GSM_Cal::Process_RxNV::Rx_Cal_NV_Consolidated(hPhone, params, results,  gsm_rx_chain_data, gsm_rx_cal_nv))
      {
          RFCal::Helper::NV::WriteCalRFNVToQCN(hPhone,params.FilePath.stCalibratedXQCN, false);
      }
   }
   RFCal::Helper::Log::StopQMSLLogging( hPhone );
   CloseHandle( deviceMutex ); // CloseHendle for the deviceMutex must be called outside this function. For demo purpose, called here.
 
   return true;
}
unsigned char RFCal::GSM_Cal::DO_GSM_V2_TX_Cal(HANDLE hPhone)
{
   GSM_CAL_Results results;
   GSM_CAL_Params params;
   AssignTxParams(hPhone, params);
   deviceMutex = CreateMutex( NULL, FALSE, NULL); // Create mutex. This must be created outside this funciton. For demo purpose, created here.
   RFCal::Helper::Log::StartQMSLLogging(hPhone, params.FilePath.stQMSLLog);
 
   // Set equipment
   RFCal::DeviceDB::GetInstance().RegisterDevice (hPhone, params.DeviceConfig.type, params.DeviceConfig.boardId, params.DeviceConfig.primaryAddr, params.DeviceConfig.secondaryAddr );
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   
   if ( NULL == pMgr ) return false;

   pMgr->Set_RFMode ( params.rf_mode );
   pMgr->Set_ULChannel(static_cast<int>(params.channel));
   pMgr->Set_SimulationMode(false);
   pMgr->Get_Version();
   RFCal::SetPathLoss(hPhone, (*pMgr), params.FilePath.stCalDBXML,"GSM");
   pMgr->Set_OutputPort( false );
   // Set RF Mode (Query RFC if necessary) 
   RFCal::GSM_Cal::INIT_TX(hPhone, params);

   // DO DA CAL
   if (params.performDACal)
   {
      RFCal::GSM_Cal::DA_Cal::DO_GSM_DA_Cal(hPhone, params, results);
   }

   if (params.performPreDistCal)
   {
      RFCal::GSM_Cal::PreDist_Cal::DO_GSM_PreDist_Cal(hPhone, params, results);
   }

  //----------------- NV Processing for Chain Enumerated NV ------------//
   if( params.txWriteChainEnumeratedNv || params.writeConsolidatedTxNvVersion )
   {
     // QMSL_GSM_Tx_Calibration_NV gsmTxCalNv;
      QMSL_GSM_Tx_Calibration_Consolidated_NV gsmTxCalConsolidatedNV;
      gsmTxCalConsolidatedNV.iDatatypeVer = params.writeConsolidatedTxNvVersion;

      bool logDaCalNV = true; //((params.daCalParams.nv_action & RFCAL_NV_ACTION_ENABLE_LOG) != 0);

      //----------------------- Version 2.0 predistortion calibration ---------------------//
      if( params.performPreDistCal )
      {
         if(params.writeConsolidatedTxNvVersion)
         {

            if ( RFCal::GSM_Cal::Process_TxNV::Consolidated_NV_From_DA_and_IQ_Predistortion_Cal(hPhone, params, results, gsmTxCalConsolidatedNV) )
            {
               RFCal::Helper::NV::WriteCalRFNVToQCN(hPhone,params.FilePath.stCalibratedXQCN, false);
            }
            else
            {
               RFCal::Helper::Log::StopQMSLLogging( hPhone );
               if ( results.calibratedPreDistData )
               {
                  delete  results.calibratedPreDistData ;
                  results.calibratedPreDistData = NULL;
               }
               return false;   
            }
         }
      }
   }
   if ( results.calibratedPreDistData )
   {
      delete  results.calibratedPreDistData ;
      results.calibratedPreDistData = NULL;
   }
   // Success
   RFCal::Helper::Log::StopQMSLLogging( hPhone );

   ReleaseMutex(deviceMutex);
   return true;
}

unsigned char RFCal::GSM_Cal::AssignGSMV5Params_DPD (HANDLE hPhone, GSM_CAL_Params &params)
{
	
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	p.m_DPD_Params.performDpdCal = true;
	p.m_DPD_Params.doRGIRunOnly = false;				// Param - Do predist RGI selection only
	p.m_DPD_Params.minDpdPowDelta = 1.5;				// Param - Sanity Check: Min DigGain Pow Delta
	p.m_DPD_Params.doSlopeCorrection = 0;				// Param - Do Slope Correction
	p.m_DPD_Params.doPowerAdaptation = 1;				// Param - Do Power Adaptation
	p.m_DPD_Params.doPiecewiseAdapt = 0;
	p.m_DPD_Params.doAdaptiveMaxPowEst = 0;

	// make sure both slope and piecewise adaptation aren't enabled at the same time
	if (p.m_DPD_Params.doSlopeCorrection && p.m_DPD_Params.doPiecewiseAdapt)
	{
		p.m_DPD_Params.doPiecewiseAdapt = false;
		//CUIManagerFactory::GetUIManager()->PrintToError( "Piecewise-linear and Slope adaptation can't both be enabled: defaulting to Slope." );
	}

	// make sure both piecewise and power aren't enabled at the same time
	if (p.m_DPD_Params.doPowerAdaptation && p.m_DPD_Params.doPiecewiseAdapt)
	{
		p.m_DPD_Params.doPowerAdaptation = false;
		//CUIManagerFactory::GetUIManager()->PrintToError( "Piecewise-linear and Power adaptation can't both be enabled: disabling Power Adapt." );
	}

	p.m_DPD_Params.dpdModType = static_cast<unsigned int>(gsmV4_Desired_DPD_Mod_Type);
	p.m_DPD_Params.useDaAdaptation = false;

	// If we are doing DA-only cal, without dpd-based RGI select, then set DA adaptation based predist RGI selection to true.
	if ( p.m_DA_Params.performDaCal && !p.m_DPD_Params.performDpdCal && !p.m_DPD_Params.doRGIRunOnly )
	{
		p.m_DPD_Params.useDaAdaptation = true;
	}

	// check the Dpd Mod Type
	if (p.m_DPD_Params.dpdModType > 1)
	{
		// make it 1 if it is greater than 1
		p.m_DPD_Params.dpdModType = 1;
	}

	// ValidateAndFillDPD

	if ( !p.m_DPD_Params.performDpdCal && !p.m_DPD_Params.doRGIRunOnly )
	{
		// The params must be resize to numBands in order for MultiSweep to work
		p.m_DPD_Params.dpd_RgiList.resize( p.m_bcc.m_numBands );
		p.m_DPD_Params.dpd_DigGainList.resize( p.m_bcc.m_numBands );
		p.m_DPD_Params.slopeCorrectionDigGainList.resize( p.m_bcc.m_numBands );
		p.m_DPD_Params.numDpd_DigGain = 0;
		p.m_DPD_Params.numDpd_Rgi     = 0;
		return true;
	}


	p.m_DPD_Params.calRGI_PowerHighBand = 26;			//Power of Calibration RGI (High Band)
	p.m_DPD_Params.calRGI_PowerLowBand = 27;			//Power of Calibration RGI (Low Band)

	unsigned int startRGI[4] = {25,19,24,25};			//DPD RGI List
	p.m_DPD_Params.numDpd_DigGain = 5;	
	vector<unsigned int> rgiList;
	for( unsigned int bandIdx = 0; bandIdx < p.m_bcc.m_numBands; bandIdx++)
	{
		rgiList.clear();
		for(unsigned int rgiIdx = 0; rgiIdx < p.m_DPD_Params.numDpd_DigGain; rgiIdx++)
		{
			rgiList.push_back(startRGI[bandIdx] + rgiIdx);
		}
		p.m_DPD_Params.dpd_RgiList.push_back(rgiList);
	}

	// currently, enforce the same # or RGIs per band
	p.m_DPD_Params.numDpd_Rgi = p.m_DPD_Params.dpd_RgiList[0].size();
	if( (p.m_DPD_Params.numDpd_Rgi > gsmV4_maxNumDpdRgi) || (p.m_DPD_Params.numDpd_Rgi < gsmV4_minNumDpdRgi) )
	{
		return false;
		//ostr<<"Error: \"DPD RGI List\" should have between " << gsmV4_minNumDpdRgi << " and " << gsmV4_maxNumDpdRgi << " RGIs per band. " << __FUNCTION__;

	}
	// checking RGIs
	for (unsigned int bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
	{
		if( p.m_DPD_Params.dpd_RgiList[bandIdx].size() != p.m_DPD_Params.numDpd_Rgi )
		{
			return false;

			//ostr<<"Error: \"DPD RGI List\" should have the same # of RGIs per band. " << __FUNCTION__;

		}
		for (unsigned short i=0; i<p.m_DPD_Params.numDpd_Rgi; ++i)
		{
			if (p.m_DPD_Params.dpd_RgiList[bandIdx][i] > gsmV4_maxRGI) /*m_gsmV4Params.m_DPD_Params.dpd_RgiList[bandIdx][i] < gsmV4_minRGI ||*/
			{
				return false;
				// ostr<<"Error: Invalid DPD Cal RGI of " << m_gsmV4Params.m_DPD_Params.dpd_RgiList[bandIdx][i] << ". " << __FUNCTION__;

			}
		}
	}
		
	p.m_DPD_Params.dpd_DigGainList.resize( p.m_bcc.m_numBands );

	for ( int bandIdx = 0; bandIdx < static_cast<int> (p.m_bcc.m_numBands) ; bandIdx ++)	//DPD Digital Gain List
	{
		p.m_DPD_Params.dpd_DigGainList[bandIdx].push_back(47);
	}

	p.m_DPD_Params.digGainForPredistRGI = 47;												//DigGain for PreDist RGI Select

	// enforce the same number of digital gains for each band.............1
	p.m_DPD_Params.numDpd_DigGain = p.m_DPD_Params.dpd_DigGainList[0].size();
	
	// checking digital gains
   for (unsigned int bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
   {
      if (p.m_DPD_Params.doRGIRunOnly)
      {
         if (p.m_DPD_Params.dpd_DigGainList[bandIdx].size() != gsmV4_reqNumDpdGainRGI_Only )
         {
            return false;
            /*ostr<<"Error: The DPD Digital Gain list needs to have " << gsmV4_reqNumDpdGainRGI_Only << " items. "
                << " Please correct the DPD Digital Gain list. " << __FUNCTION__;*/
            
         }
      }
      else
      {
         if( p.m_DPD_Params.dpd_DigGainList[bandIdx].size() > gsmV4_maxNumDpdGain || p.m_DPD_Params.dpd_DigGainList[bandIdx].size() < gsmV4_minNumDpdGain )
         {
            return false;
            /*ostr<<"Error: The DPD Digital Gain list shoud have between " << gsmV4_minNumDpdGain << " and "
                << gsmV4_maxNumDpdGain << " elements; band: " << m_gsmV4Params.m_bcc.m_bandList[bandIdx] << " " << __FUNCTION__;*/
            
         }
      }

      for (unsigned short i=0; i<p.m_DPD_Params.numDpd_DigGain; ++i)
      {
         if (p.m_DPD_Params.dpd_DigGainList[bandIdx][i] < gsmV4_minDpdGain || p.m_DPD_Params.dpd_DigGainList[bandIdx][i] > gsmV4_maxDpdGain)
         {
            return false;
            //ostr<<"Error: Invalid DPD Digital Gain of " << p.m_DPD_Params.dpd_DigGainList[bandIdx][i] << ". " << __FUNCTION__;
            
         }
      }
   }

      //----- Check the digital gain for predistortion RGI selection
   if (!p.m_DPD_Params.useDaAdaptation)
   {
      if (static_cast<unsigned short>(p.m_DPD_Params.digGainForPredistRGI) < gsmV4_minDpdGain ||
            static_cast<unsigned short>(p.m_DPD_Params.digGainForPredistRGI) > gsmV4_maxDpdGain)
      {
         return false;
         //ostr<<"Error: Invalid DPD Digital Gain for predist RGI selection of " << m_gsmV4Params.m_DPD_Params.digGainForPredistRGI << ". " << __FUNCTION__;
         
      }

      // verify the digGainForPredistRGI is in the digital gain list
      vector<unsigned int>::iterator it;

      for (unsigned int bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
      {
         for (unsigned int digGainIdx=0; digGainIdx<p.m_DPD_Params.dpd_DigGainList[bandIdx].size(); ++digGainIdx)
         {
            it = find(p.m_DPD_Params.dpd_DigGainList[bandIdx].begin(), p.m_DPD_Params.dpd_DigGainList[bandIdx].end(), p.m_DPD_Params.digGainForPredistRGI );

            if ( it == p.m_DPD_Params.dpd_DigGainList[bandIdx].end() )
            {
               return false;
               /*ostr<<"Error: DPD Digital Gain for predist RGI is not included in the digital gain list for band: "
                   << m_gsmV4Params.m_bcc.m_bandList[bandIdx] << "  " << __FUNCTION__;*/
              
            }
         }
      }
   }

	p.m_DPD_Params.doSlopeCorrectionAlignLow = false;
	vector <unsigned int> slopeCorrectionDigGain;	//Slope Correction Points
	slopeCorrectionDigGain.push_back(5);
	slopeCorrectionDigGain.push_back(15);
	slopeCorrectionDigGain.push_back(40);
	p.m_DPD_Params.slopeCorrectionDigGainList.push_back( slopeCorrectionDigGain);
	p.m_DPD_Params.slopeCorrectionDigGainList.push_back( slopeCorrectionDigGain);
	p.m_DPD_Params.slopeCorrectionDigGainList.push_back( slopeCorrectionDigGain);
	p.m_DPD_Params.slopeCorrectionDigGainList.push_back( slopeCorrectionDigGain);

	if ( p.m_DPD_Params.slopeCorrectionDigGainList.size() < p.m_bcc.m_numBands )
	{
		return false;
		//ostr << "Error: \"Slope Correction Digital Gain List\": Digital gain list is missing for some band(s). " <<__FUNCTION__;

	}

	p.m_DPD_Params.dacScale = 4;						//DAC Scaling
	p.m_DPD_Params.calPaRange = 0;						//Calibration PA Range


	if (p.m_DPD_Params.doAdaptiveMaxPowEst && (p.m_DPD_Params.numDpd_DigGain < 4) && p.m_DPD_Params.performDpdCal )
	{
		p.m_DPD_Params.doAdaptiveMaxPowEst = false;
		//CUIManagerFactory::GetUIManager()->PrintToError( "Adaptive Max Power Estimation can't both be enabled with < 4 digital gains." );
	}


	p.m_DPD_Params.nvChanMap.push_back(1);			//NV Mappings
	p.m_DPD_Params.nvChanMap.push_back(2);
	p.m_DPD_Params.nvChanMap.push_back(3);

	return true;
}
unsigned char RFCal::GSM_Cal::AssignGSMV5Params_RX(HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	p.m_Rx_Params.performRxCal = true;			// Perform Rx Cal
	p.m_rxNumSlots = 0;
	p.m_Rx_Params.rx_alt_path = 0;				//Alternate Path
	p.m_Rx_Params.do_alt_path_cal = false;   
	if (p.m_Rx_Params.rx_alt_path != 0)
	{
		p.m_Rx_Params.do_alt_path_cal = true;
	}
	p.m_FBRx_Params.performRxCalwithChar = false;

	RFCal::Helper::Convert::Str_To_Vector( p.m_FBRx_Params.m_chanList_rxcal,   "975,37,124;128,190,251;512,700,885;512,661,810");	// Param - Channels List
	p.m_Rx_Params.performHiLinRxCal = false;		//Perform High Linearity Cal

	// these band-enumerated params have to be resized in order for MultiSweep to work, even if Rx is disabled
	p.m_Rx_Params.gainOffset_MinLim_List.resize( p.m_bcc.m_numBands );
	p.m_Rx_Params.gainOffset_MaxLim_List.resize( p.m_bcc.m_numBands );

	if (p.m_Rx_Params.performRxCal)
	{
		p.m_Rx_Params.performHiLinRxCal = false;	//Perform High Linearity Cal

		//populate parameter for write to consolidated
		p.m_Rx_Params.write_to_consolidated = true;	//Write to Consolidated NV 

		// The # of slots must divide evenly into 16
		p.m_rxNumSlots = gsmV4_maxNumRxSlots; // # of Rx slots per frame

		//------ LNA state list 
		p.m_Rx_Params.lnaStateList.push_back(0);
		p.m_Rx_Params.lnaStateList.push_back(1);
		p.m_Rx_Params.lnaStateList.push_back(2);
		p.m_Rx_Params.lnaStateList.push_back(3);
		p.m_Rx_Params.numLNAState = p.m_Rx_Params.lnaStateList.size();

		//------ Rx Power List
		p.m_Rx_Params.lnaPowerList.push_back(-90);
		p.m_Rx_Params.lnaPowerList.push_back(-90);
		p.m_Rx_Params.lnaPowerList.push_back(-54);
		p.m_Rx_Params.lnaPowerList.push_back(-54);
		p.m_Rx_Params.numLNA_Power = p.m_Rx_Params.lnaPowerList.size();

		//------ Get the DRx cal vector
		// Perform Diversity Cal per Band
		p.m_Rx_Params.performDRxCalList.resize( p.m_bcc.m_numBands );
		for (unsigned char bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
		{
			p.m_Rx_Params.performDRxCalList[bandIdx] = true;
			if ( p.m_Rx_Params.performDRxCalList[bandIdx] )
			{
				p.m_Rx_Params.performDRxCal = true;
			}
		}



		p.m_Rx_Params.lnaStateListAllMode = p.m_Rx_Params.lnaStateList;
		p.m_Rx_Params.lnaPowerListAllMode = p.m_Rx_Params.lnaPowerList;

		//------ LNA state list for hi/lo linearity CAL
		p.m_Rx_Params.numLnaStateHiLin  = 0;
		p.m_Rx_Params.numLNA_PowerHiLin = 0;

		if( p.m_Rx_Params.performHiLinRxCal )
		{
			p.m_Rx_Params.lnaStateListHiLin.push_back (0);
			p.m_Rx_Params.lnaStateListHiLin.push_back (1);
			p.m_Rx_Params.lnaStateListHiLin.push_back (2);
			p.m_Rx_Params.lnaStateListHiLin.push_back (3);
			p.m_Rx_Params.numLnaStateHiLin = p.m_Rx_Params.lnaStateListHiLin.size();

			//cell power levels
			for( unsigned int index = 0; index < p.m_Rx_Params.numLNAState; ++index)
			{
				unsigned int lna = p.m_Rx_Params.lnaStateList[ index ];
				if( find(p.m_Rx_Params.lnaStateListHiLin.begin(), p.m_Rx_Params.lnaStateListHiLin.end(), lna) != p.m_Rx_Params.lnaStateListHiLin.end() )
				{
					p.m_Rx_Params.lnaPowerListHiLin.push_back( p.m_Rx_Params.lnaPowerList[ index ] );
				}
			}
			p.m_Rx_Params.numLNA_PowerHiLin = p.m_Rx_Params.lnaPowerListHiLin.size();

			// combine states of hi and lo linearity modes
			for(unsigned int index = 0; index < p.m_Rx_Params.numLnaStateHiLin; index++)
			{
				//bump up high linearity state by 128 to differentiate from low linearity.
				p.m_Rx_Params.lnaStateListAllMode.push_back(p.m_Rx_Params.lnaStateListHiLin[index] + 128);
				p.m_Rx_Params.lnaPowerListAllMode.push_back(p.m_Rx_Params.lnaPowerListHiLin[index]);
			}
		}

		//------- Rx Gain Offset Limits
		p.m_Rx_Params.gainOffset_MaxLim_List.resize(p.m_bcc.m_numBands);
		// PRx Max Lim
		p.m_Rx_Params.gainOffset_MaxLim_List[0].push_back(2800);
		p.m_Rx_Params.gainOffset_MaxLim_List[0].push_back(2550);
		p.m_Rx_Params.gainOffset_MaxLim_List[0].push_back(2200);
		p.m_Rx_Params.gainOffset_MaxLim_List[0].push_back(2000);
		p.m_Rx_Params.gainOffset_MaxLim_List[1].push_back(2850);
		p.m_Rx_Params.gainOffset_MaxLim_List[1].push_back(2700);
		p.m_Rx_Params.gainOffset_MaxLim_List[1].push_back(2200);
		p.m_Rx_Params.gainOffset_MaxLim_List[1].push_back(2000);
		p.m_Rx_Params.gainOffset_MaxLim_List[2].push_back(2700);
		p.m_Rx_Params.gainOffset_MaxLim_List[2].push_back(2540);
		p.m_Rx_Params.gainOffset_MaxLim_List[2].push_back(2320);
		p.m_Rx_Params.gainOffset_MaxLim_List[2].push_back(2000);
		p.m_Rx_Params.gainOffset_MaxLim_List[3].push_back(2750);
		p.m_Rx_Params.gainOffset_MaxLim_List[3].push_back(2530);
		p.m_Rx_Params.gainOffset_MaxLim_List[3].push_back(2200);
		p.m_Rx_Params.gainOffset_MaxLim_List[3].push_back(2000);
		// PRx Min Lim
		p.m_Rx_Params.gainOffset_MinLim_List[0].push_back(1801);
		p.m_Rx_Params.gainOffset_MinLim_List[0].push_back(1600);
		p.m_Rx_Params.gainOffset_MinLim_List[0].push_back(1650);
		p.m_Rx_Params.gainOffset_MinLim_List[0].push_back(1200);
		p.m_Rx_Params.gainOffset_MinLim_List[1].push_back(1850);
		p.m_Rx_Params.gainOffset_MinLim_List[1].push_back(1600);
		p.m_Rx_Params.gainOffset_MinLim_List[1].push_back(1650);
		p.m_Rx_Params.gainOffset_MinLim_List[1].push_back(1200);
		p.m_Rx_Params.gainOffset_MinLim_List[2].push_back(1801);
		p.m_Rx_Params.gainOffset_MinLim_List[2].push_back(1600);
		p.m_Rx_Params.gainOffset_MinLim_List[2].push_back(1600);
		p.m_Rx_Params.gainOffset_MinLim_List[2].push_back(1200);
		p.m_Rx_Params.gainOffset_MinLim_List[3].push_back(1800);
		p.m_Rx_Params.gainOffset_MinLim_List[3].push_back(1650);
		p.m_Rx_Params.gainOffset_MinLim_List[3].push_back(1550);
		p.m_Rx_Params.gainOffset_MinLim_List[3].push_back(1150);
		// DRx Max Lim
		p.m_Rx_Params.gainOffset_MaxLim_List_DRx = p.m_Rx_Params.gainOffset_MaxLim_List;
		// DRx Min Lim
		p.m_Rx_Params.gainOffset_MinLim_List_DRx = p.m_Rx_Params.gainOffset_MinLim_List;


	}

	return true;

}
unsigned char RFCal::GSM_Cal::AssignGSMV5Params_DA(HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;

	p.m_DA_Params.performDaCal = true;																				// Param - Perform DA Cal
	RFCal::Helper::Convert::Str_To_Vector( p.m_DA_Params.modList,      "4,9");										// Param - MCS List
	p.m_DA_Params.maxPclPowDelta = 200;																				// Param - Sanity Check: Max PCL Power Delta
	p.m_DA_Params.pclNvPowOffset = 0;																				// Param - PCL Target Power Offset
	p.m_DA_Params.numMods = p.m_DA_Params.modList.size();
	
	p.m_DA_Params.pclList.resize(p.m_bcc.m_numBands);
	RFCal::Helper::Convert::Str_To_Vector( p.m_DA_Params.pclList[0],      "4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19");
	RFCal::Helper::Convert::Str_To_Vector( p.m_DA_Params.pclList[1],      "4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19");
	RFCal::Helper::Convert::Str_To_Vector( p.m_DA_Params.pclList[2],      "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15");
	RFCal::Helper::Convert::Str_To_Vector( p.m_DA_Params.pclList[3],      "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15");

	p.m_DA_Params.numPCLs = p.m_DA_Params.pclList[0].size();
	p.m_DA_Params.pclIdxList.resize(p.m_bcc.m_numBands);

	p.m_DA_Params.usePclErrorNV = true;

	// this implies Rx cal is being performed without DA cal
	if (!p.m_DA_Params.performDaCal)
	{
		p.m_DA_Params.numMods = 2;
	}

	p.m_DA_Params.modIdxList.push_back(3);
	p.m_DA_Params.modIdxList.push_back(8);

	p.m_txNumSlots = gsmV4_maxNumTxSlots;

	/*
	p.m_DA_Params.pclIdxList.resize(p.m_bcc.m_numBands);
	for ( unsigned int bandIdx = 0 ; bandIdx < p.m_bcc.m_numBands; bandIdx ++ )
	{
		for ( unsigned int pclIdx = 0; pclIdx < p.m_DA_Params.numPCLs; pclIdx ++)
		{
			p.m_DA_Params.pclIdxList[bandIdx].push_back(p.m_DA_Params.numPCLs - pclIdx -1 );
		}
	}

	p.m_DA_Params.pclList.resize(p.m_bcc.m_numBands);
	vector <unsigned int> HBandPcl, LBandPcl;
	for (unsigned int i = 0; i < p.m_DA_Params.numPCLs; i++)
	{
		LBandPcl.push_back(i+4);
		HBandPcl.push_back(i);
	}

	p.m_DA_Params.pclList[0] = LBandPcl; 
	p.m_DA_Params.pclList[1] = LBandPcl;
	p.m_DA_Params.pclList[2] = HBandPcl;
	p.m_DA_Params.pclList[3] = HBandPcl;

	*/
	return true;
}
/*Init_GSM_V4_Cal_Params initializes the common calibration parameters
and calls functions to initialize parameters for DA, DPD and Rx cal*/
unsigned char RFCal::GSM_Cal::AssignGSMV5Params(HANDLE hPhone, GSM_CAL_Params & params)
{

	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	
	params.channel = 128;
	RFCal::Helper::Convert::Str_To_Vector( params.band_list,      "10,18,11,12");										// Param - Band List
	RFCal::Helper::Convert::Str_To_Vector( params.channel_list,   "975,37,124;128,190,251;512,700,885;512,661,810");	// Param - Channels List
	RFCal::Helper::Convert::Str_To_Vector( params.nv_mapping,      "1,2,3");											// Param - NV Mappings
	params.chainID = 0;																									// Param - Tx/Rx Chain ID List
	params.m_txAntPath = 0;																								// Param - Tx Antenna Path
	params.writeConsolidatedTxNvVersion = 1;
	params.setRFMPathVersion = 2;																						// Param - Set Device and SigPath
	p.m_bcc.m_get_nv_container_from_uut_rx = false;	// Param - Tx: Obtain NV Container from UUT
	p.m_bcc.m_get_nv_container_from_uut_tx = false;	// Param - Rx: Obtain NV Container from UUT

	p.m_nv_action = 1;								// Param - NV Write Action (might not be used)
	p.m_computeNV = (p.m_nv_action!=0);
	p.m_logCalMeasData = 1;							// Param - Log Cal Results to XML (might not be used)
	p.m_logDebugWaveform = 0;						// Param - Log Debug Waveform to CSV (might not be used)
	params.m_enGsmFtmRfModeExit = true;				// Param - Enable FTM RF Mode Exit
	
	// Assign band parameters
	
	p.m_bcc.m_bandList = params.band_list;
	p.m_bcc.m_chanList = params.channel_list;
	p.m_bcc.m_numBands = params.band_list.size();
	p.m_bcc.m_numChan  = params.channel_list[0].size();

	p.m_bcc.m_numChanPerBand.resize( p.m_bcc.m_numBands );
	for ( unsigned int bandIdx = 0 ; bandIdx < p.m_bcc.m_numBands; bandIdx ++ )
	{
		p.m_bcc.m_numChanPerBand[bandIdx] = p.m_bcc.m_chanList[bandIdx].size();
	}
	
	p.m_bcc.m_numTxChains = 1;
	p.m_bcc.m_numRxChains = 1;
	p.m_bcc.m_txChainList.push_back(params.chainID );
	p.m_bcc.m_rxChainList.push_back(params.chainID );

	// DA, DPD, Rx params
	if ( !AssignGSMV5Params_DA( hPhone, params) 
		|| !AssignGSMV5Params_DPD( hPhone, params) 
		|| !AssignGSMV5Params_RX (hPhone, params) )
	{
		return false;
	}

	p.m_sweepParamsTestEquip.m_ul_max_lvl = 30;			//Call Box Input Level (dBm)
	p.m_sweepParamsTestEquip.m_step_length_us = 577;	//Step Length (us)
	p.m_sweepParamsTestEquip.m_meas_length_us = 450;	//Measurement Length (us)
	p.m_sweepParamsTestEquip.m_trigger_offset_us = 20;	//Trigger Offset (us)
	p.m_measStartDelay_ms = 100;						//Measurement Start Delay (ms)

	// Slots
	p.m_TxSlotConfig.resize(GSM_V4_MAX_NUM_OF_SLOTS);
	p.m_TxSlotConfig[0] = 1;
	p.m_TxSlotConfig[1] = 1;
	p.m_TxSlotConfig[2] = 1;
	p.m_TxSlotConfig[3] = 1;
	p.m_TxSlotConfig[4] = 0;
	p.m_TxSlotConfig[5] = 0;
	p.m_TxSlotConfig[6] = 0;
	p.m_TxSlotConfig[7] = 0;
   
	p.m_txNumSlots = gsmV4_maxNumTxSlots;

	// Add the Call Box Slot Configuration
	// Default to copying over the slot configuration --> case for 1 Tx chain
	p.m_TxSlotConfig_CB = p.m_TxSlotConfig;
	// if 2 chains are used, add slots for second chain (shifted by 4 relative to first chain)
	if (p.m_bcc.m_numTxChains > 1)
	{
		// (GSM_V4_MAX_NUM_OF_SLOTS - GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE + 1) insures vector bounds
		for (unsigned char slotIdx=0; slotIdx < (GSM_V4_MAX_NUM_OF_SLOTS - GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE + 1); ++slotIdx)
		{
			if (p.m_TxSlotConfig[slotIdx] > 0)
			{
				p.m_TxSlotConfig_CB[slotIdx + GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE] = 1;
			}
		}
	}
	p.m_RxSlotConfig.resize(GSM_V4_MAX_NUM_OF_SLOTS);
	// Note: Only 1 Rx slot can be used
	// Note: Rx slots can only go in 0 or 1 with Tx configration of [1,1,1,1,0,0,0,0]
	// Note: Rx slots can only go in 1 or 2 with Tx configration of [0,1,1,1,1,0,0,0]
	p.m_rxSlotOffset = 0;
	if ( p.m_Rx_Params.performRxCal )
	{
		p.m_RxSlotConfig[0] = 1;
		// rxSlotOffset indicates the location of the rx slot
		p.m_rxSlotOffset    = 0;
	}

	// compute the number of frames required
	RFCal::GSM_Cal::GSM_V5::Compute_NumFrames(hPhone, params);


	// compute Tx measurement information
	RFCal::GSM_Cal::GSM_V5::Compute_MeasInfo (hPhone, params);

	// only need to do this if DA cal is run
	if (p.m_DA_Params.performDaCal)
	{
		// create the PCL map - This must be run prior to Create_PCL_Indexes
		RFCal::GSM_Cal::GSM_V5::Tx::Create_PCL_Map(hPhone, params);

		// create PCL indexes from the PCL values
		RFCal::GSM_Cal::GSM_V5::Tx::Create_PCL_Indexes( hPhone, params);

		// free memory associated with PCL maps
		RFCal::GSM_Cal::GSM_V5::Tx::Clear_PCL_Maps(hPhone, params);

		// create MCS indexes from the MCS values
		RFCal::GSM_Cal::GSM_V5::Tx::Create_MCS_Indexes(hPhone, params);
	}

	if ( p.m_DPD_Params.performDpdCal || p.m_DPD_Params.doRGIRunOnly )
	{
		// Build the DPD sweep Containers
		RFCal::GSM_Cal::GSM_V5::Tx::BuildDpdSweepContainers( hPhone, params, false );
	}

	// Assign predistortion params
	AssignGSMV5Params_Predistortion ( hPhone, params);

   // *********************************************** //
   // Set Output Filepaths (XQCN and QMSL log)
   // *********************************************** //
	RFCal::UserConfig::Set_OutputResultsFilePath ( hPhone, "C:\\Qualcomm\\QDART\\Temp","GSM_V5_Cal",  params.FilePath.stQMSLLog, params.FilePath.stCalibratedXQCN, params.FilePath.stDLFLog );


	return true;
}

unsigned char RFCal::GSM_Cal::AssignGSMV5Params_Predistortion (HANDLE hPhone, GSM_CAL_Params & params)
{
	CEDGE_PreDist_Cal_Data &p = params.preDistCalParamsV5;

	if ( !params.gsm_v4_Params.m_DPD_Params.performDpdCal )
	{
		return true;
	}
   params.performPreDistCal = 1;
   params.txWriteChainEnumeratedNv = 0;
	p.m_nv_action = p.m_nv_action;
	p.m_channels.resize(1);           // there is only 1 channel used for predistortion in GSM V4
	p.m_calRgiWithNoDaCal.resize(1);  // there is only 1 RGI

	for (unsigned int chanIdx=0; chanIdx< params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx)
	{
		p.m_nvMappings.push_back( (QMSL_GSM_Polar_Cal_Frequency)params.gsm_v4_Params.m_DPD_Params.nvChanMap[chanIdx] );
	}

	p.m_performPreDistCal         = true;						

	p.m_logCaptureData            = false;			// Param - Log PreDistortion Calibration Data
	p.m_swappedOrderEdgeFirst     = true;			// Param - Swapped Order - EDGE First
	
	p.m_overRideModemConsts       = true;			// Param - PreDist Modem Constants OverRide
	p.m_extensionFloor = 5200;						// Param - Modem Const:Extension Floor for Dynamic Range Expansion
	p.m_rampUnity =8192;							// Param - Modem Const:Ramp Unity
	p.m_digGainUnity = 64;							// Param - Modem Const:Digital Gain Unity

	p.m_overRideSmoothingDefaults = true;			// Param - Override Smoothing Defaults?
	p.m_enableAmAmSmoothing		  = false;			// Param - Enable AMAM Smoothing?
	p.m_amamKnotSize = 2.0;							// Param - AMAM Smoothing Knot
	p.m_ampmKnotSize = 2.0;							// Param - AMPM Smoothing Knot 
	
	p.m_inputLevel = 33.0;							// Param - Waveform Max Level
	p.m_edgeDuration = 625;							// Param - EDGE Duration
	p.m_preDcDuration = 625;						// Param - DC Duration

	p.m_dacScale          = params.gsm_v4_Params.m_DPD_Params.dacScale;
	p.m_tcxoPdm           = 0;     // don't need
	p.m_paRangeCalibrated = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.calPaRange);



	
	EDGE_PreDist_Cal_Data & preDistCalParams = params.preDistCalParams;
   
   preDistCalParams.swappedOrderEdgeFirst = true;
  // preDistCalParams.channel_list = daCalParams.channel_list;
   preDistCalParams.edgeDuration = 625;
   preDistCalParams.preDcDuration = 625;
   preDistCalParams.measStartDelay = 50;
   preDistCalParams.inputLevel = 35; // Waveform Max Level
   preDistCalParams.calRGIPower = 27;
   preDistCalParams.paRangeCalibrated = 0;
   preDistCalParams.dcSkipPercentage = 60; // PreDist:DC Transient Duration//
   preDistCalParams.edgeSkipSymbols = 90;
   preDistCalParams.tcxoPdm = 0;

   preDistCalParams.overRideModemConsts = true;
   preDistCalParams.digGainUnity = 64;
   preDistCalParams.rampUnity = 8192;
   preDistCalParams.extensionFloor = 5200;
   preDistCalParams.dacScale = 4;

   preDistCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F1_CHANNEL);
   preDistCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F2_CHANNEL);
   preDistCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F3_CHANNEL);

   preDistCalParams.overRideSmoothingDefaults = true;
   preDistCalParams.enableAmAmSmoothing = 0;
   preDistCalParams.amamKnotSize = 2.0;
   preDistCalParams.ampmKnotSize = 2.0;
   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::BuildDpdSweepContainers(HANDLE hPhone, GSM_CAL_Params & params, bool reverseSweepOrder )
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	// # of elements that should be in the Dpd Sweep
	unsigned int numElemDpdSweep = p.m_DPD_Params.numDpd_Rgi * p.m_DPD_Params.numDpd_DigGain;

	unsigned int curBand  = 0;

	unsigned int digGainIdx  = 0;
	unsigned int rgiIdx      = 0;
	unsigned short vectorIdx = 0;

	if ( reverseSweepOrder )   // Doing V4 Characterization, and desiring to reverse swep order ---> sweep order - RGI, DigGain
	{
		for (unsigned int bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
		{
			curBand = p.m_bcc.m_bandList[bandIdx];

			// resize the vectors for curBand to be correct
			p.m_dpd_digGain_MapVec[curBand].resize( numElemDpdSweep );
			p.m_dpd_RGI_MapVec[curBand].resize( numElemDpdSweep );

			for (rgiIdx=0; rgiIdx<p.m_DPD_Params.numDpd_Rgi; ++rgiIdx)
			{
				for (digGainIdx=0; digGainIdx<p.m_DPD_Params.numDpd_DigGain; ++digGainIdx)
				{
					// Cast to unsigned char due to FTM command requirement being unsigned char
					p.m_dpd_digGain_MapVec[curBand][vectorIdx] = static_cast<unsigned char>( p.m_DPD_Params.dpd_DigGainList[bandIdx][digGainIdx] );
					p.m_dpd_RGI_MapVec[curBand][vectorIdx]     = static_cast<unsigned char>( p.m_DPD_Params.dpd_RgiList[bandIdx][rgiIdx] );
					++vectorIdx;
				}
			}

			// reset vectorIdx
			vectorIdx = 0;
		}
	}
	else     // Doing V4 Calibration  ---> sweep order - DigGain, RGI
	{
		// fill in order of constant Digital Gain with increasing RGI
		for (unsigned int bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
		{
			curBand = p.m_bcc.m_bandList[bandIdx];

			// resize the vectors for curBand to be correct
			p.m_dpd_digGain_MapVec[curBand].resize( numElemDpdSweep );
			p.m_dpd_RGI_MapVec[curBand].resize( numElemDpdSweep );

			for (digGainIdx=0; digGainIdx<p.m_DPD_Params.numDpd_DigGain; ++digGainIdx)
			{
				for (rgiIdx=0; rgiIdx<p.m_DPD_Params.numDpd_Rgi; ++rgiIdx)
				{
					// Cast to unsigned char due to FTM command requirement being unsigned char
					p.m_dpd_digGain_MapVec[curBand][vectorIdx] = static_cast<unsigned char>( p.m_DPD_Params.dpd_DigGainList[bandIdx][digGainIdx] );
					p.m_dpd_RGI_MapVec[curBand][vectorIdx]     = static_cast<unsigned char>( p.m_DPD_Params.dpd_RgiList[bandIdx][rgiIdx] );
					++vectorIdx;
				}
			}

			// reset vectorIdx
			vectorIdx = 0;
		}
	}
	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Create_MCS_Indexes( HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	p.m_DA_Params.modIdxList.resize(p.m_DA_Params.numMods);

	for (unsigned int i=0; i<p.m_DA_Params.numMods; ++i)
	{
		p.m_DA_Params.modIdxList[i] = static_cast<unsigned short>( p.m_DA_Params.modList[i] - gsmV4_minMCS );
	}
	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Clear_PCL_Maps( HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	p.m_DA_Params.lowBandPCl_map.clear();
	p.m_DA_Params.highBandPCl_map.clear();
	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Create_PCL_Map(HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	unsigned short curPCLIdx = gsmV4_maxNumPCL - 1;

	// fill the low band map
	for (unsigned short curPCLVal=gsmV4_minLowBand_PCL; curPCLVal<=gsmV4_maxLowBand_PCL; ++curPCLVal)
	{
		p.m_DA_Params.lowBandPCl_map.insert( pair<unsigned short, unsigned short>(curPCLVal, curPCLIdx) );
		--curPCLIdx;
	}

	curPCLIdx = gsmV4_maxNumPCL - 1;
	// fill the high band map
	for (unsigned short curPCLVal=gsmV4_minHighBand_PCL; curPCLVal<=gsmV4_maxHighBand_PCL; ++curPCLVal)
	{
		p.m_DA_Params.highBandPCl_map.insert( pair<unsigned short, unsigned short>(curPCLVal, curPCLIdx) );
		--curPCLIdx;
	}
	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Create_PCL_Indexes(HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
   //highBandPCl_to_pclIdx_map, lowBandPCl_to_pclIdx_map
   unsigned int nvMode = 0;

   unsigned short curIdx = 0;

   bool doingLowBand = true;

   p.m_DA_Params.pclIdxList.resize(p.m_bcc.m_numBands);                     // size the vector to the number of Bands

   for (unsigned int bandIdx=0; bandIdx<p.m_bcc.m_numBands; ++bandIdx)
   {
      p.m_DA_Params.pclIdxList[bandIdx].resize(p.m_DA_Params.numPCLs); // size the vector to the number of PCLs
      nvMode = p.m_bcc.m_bandList[bandIdx];

      switch(nvMode)
      {
            // checking the low band PCLs
         case NV_MODE_GSM_EGSM:
         case NV_MODE_GSM_850:
            doingLowBand = true;
            break;
         case NV_MODE_GSM_DCS:
         case NV_MODE_GSM_1900:
            doingLowBand = false;
            break;
         default:
			return false;
      }

      if (doingLowBand)
      {
         for (unsigned short i=0; i<p.m_DA_Params.numPCLs; ++i)
         {
            curIdx = static_cast<unsigned short>( p.m_DA_Params.pclList[bandIdx][i] );
            p.m_DA_Params.pclIdxList[bandIdx][i] = p.m_DA_Params.lowBandPCl_map[ curIdx ];
         }
      }
      else
      {
         for (unsigned short i=0; i<p.m_DA_Params.numPCLs; ++i)
         {
            curIdx = static_cast<unsigned short>( p.m_DA_Params.pclList[bandIdx][i] );
            p.m_DA_Params.pclIdxList[bandIdx][i] = p.m_DA_Params.highBandPCl_map[ curIdx ];
         }
      }
   }
   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Compute_MeasInfo( HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	// compute number of frames must be called prior to this

	// Number of call box measurement steps.
	// There are an additional m_intraBandTxSegDelay steps added for each band change.
	p.m_intraBandSegDelay = GSM_V4_INTRA_BAND_SEG_DELAY;
	p.m_sweepParamsTestEquip.m_num_steps = p.m_numFrames*GSM_V4_MAX_NUM_OF_SLOTS + p.m_intraBandSegDelay*(p.m_bcc.m_numBands - 1);

	// Note: numContiguousDPD_Meas and numContiguousDPD_Meas_CB can be different, and typically are different!!!

	p.m_MeasInfo.numContiguousDPD_Meas = static_cast<unsigned short>( p.m_DPD_Params.numDpd_Rgi * p.m_DPD_Params.numDpd_DigGain * p.m_bcc.m_numTxChains);
	p.m_MeasInfo.numContiguousDA_Meas  = static_cast<unsigned short>( p.m_DA_Params.numMods * p.m_DA_Params.numPCLs * p.m_bcc.m_numTxChains);

	p.m_MeasInfo.numContiguousDPD_Meas_CB = static_cast<unsigned short>( p.m_numDpdFramesPerChan * p.m_txNumSlots * p.m_bcc.m_numTxChains );

	if (!p.m_DA_Params.performDaCal)
	{
		p.m_MeasInfo.numContiguousDA_Meas = 0;
	}
	if (!p.m_DPD_Params.performDpdCal && !p.m_DPD_Params.doRGIRunOnly)
	{
		p.m_MeasInfo.numContiguousDPD_Meas    = 0;
		p.m_MeasInfo.numContiguousDPD_Meas_CB = 0;
	}

	p.m_MeasInfo.numTotalDPD_Meas = static_cast<unsigned short>( p.m_MeasInfo.numContiguousDPD_Meas * p.m_bcc.m_numChan *p. m_bcc.m_numBands );
	p.m_MeasInfo.numTotalDA_Meas  = static_cast<unsigned short>( p.m_MeasInfo.numContiguousDA_Meas * p.m_bcc.m_numChan * p.m_bcc.m_numBands);

	unsigned short numTotalDPD_Meas_CB = static_cast<unsigned short>( p.m_MeasInfo.numContiguousDPD_Meas_CB * p.m_bcc.m_numChan * p.m_bcc.m_numBands );

	// total number of expected Tx measurements for the Call Box  - we only need this for the call box perspective. It is irrelavant for the data processing perspective.
	p.m_MeasInfo.numExpTxMeas_CB = numTotalDPD_Meas_CB + p.m_MeasInfo.numTotalDA_Meas;

	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Compute_NumFrames( HANDLE hPhone, GSM_CAL_Params & params)
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
   // Note: m_txNumSlots and m_rxNumSlots have already been verified to not equal zero.

   // number of frames due to DA Cal and Rx Cal
   unsigned int numDaCalFrames = 0; // default to DA cal not being performed
   
   // this is for if only Rx cal is being performed
   if (!p.m_DA_Params.performDaCal && p.m_Rx_Params.performRxCal)
   {
      // m_DA_Params.numMods accounts for the number of averages for
      // nominally, this is the same as the value computed when DA cal is performed

      /***
      In RX only case, we need to construct enough number of DA CAL frames to drive RX.
      */
      numDaCalFrames = (p.m_Rx_Params.numLNAState + p.m_Rx_Params.numLnaStateHiLin) * p.m_DA_Params.numMods * p.m_bcc.m_numBands * p.m_bcc.m_numChan;
   }
   else if (p.m_DA_Params.performDaCal)  // If DA Cal is being performed
   {
      numDaCalFrames = (p.m_DA_Params.numPCLs / p.m_txNumSlots)*p.m_DA_Params.numMods*p.m_bcc.m_numBands*p.m_bcc.m_numChan;
   }

   // number of frames due to DPD Cal
   unsigned int numDpdFrames = 0;   // default occurs if Dpd Cal is not being performed
   p.m_numDpdFramesPerChan     = 0;
   if ( p.m_DPD_Params.performDpdCal || p.m_DPD_Params.doRGIRunOnly )
   {
      unsigned int numTotalDpdSlots = p.m_DPD_Params.numDpd_DigGain * p.m_DPD_Params.numDpd_Rgi;

      p.m_numDpdFramesPerChan = numTotalDpdSlots / p.m_txNumSlots;

      numDpdFrames = (numTotalDpdSlots / p.m_txNumSlots ) * p.m_bcc.m_numBands * p.m_bcc.m_numChan;
      // add an extra frame if the numTotalDpdSlots isn't evenly divisible by m_txNumSlots
      // For example: m_txNumSlots typically equals 4
      //              4 RGIs x 5 DigitalGains = 20 --> 5 frames
      //              5 RGIs x 5 DigitalGains = 25 --> 6 frames + 1 extra slots --> so, use 7 frames
      if ( (numTotalDpdSlots % p.m_txNumSlots) > 0 )
      {
         numDpdFrames          += p.m_bcc.m_numBands * p.m_bcc.m_numChan;
         p.m_numDpdFramesPerChan += 1;
      }
   }

   // The # of frames from the UE perspective
   p.m_numFrames = numDaCalFrames + numDpdFrames;

   // The # of frames from the Call Box (CB) perspective
   p.m_numFramesForCB = p.m_numFrames + GSM_V4_INTRA_BAND_FRAME_DELAY*(p.m_bcc.m_numBands - 1);

   // make sure this is set to false initially
   p.m_Rx_Params.m_skipFirstRxMeas = false;

   // compute the number of frames per channel
   p.m_numFramesPerChan    = p.m_numFrames / p.m_bcc.m_numBands / p.m_bcc.m_numChan;
   p.m_numDaFramesPerChan  = p.m_numFramesPerChan - p.m_numDpdFramesPerChan;

   /***
   Here we want to make sure there are enough frames to accommodate all LNA states at least twice.

   At each channel, RX measurement is accommodated in TX frames (DA and DPD), 1 RX measurement per frame.
   Before hand, if a LNA state is to be measured at both high and low linearity modes, it is treated as 2 different states, which are added
   in the vector "m_Rx_Params.lnaStateListAllMode" as 2 individual states, and the state for the alternative linearity mode is bumped up by 128 (MSB set to 1).
   At each channel, for each LNA state (hi or lo linearity), at least 2 measurements are needed in order to average out good RSSI data.

   For example, if LNA state 1 is to be calibrated at both high and low linearity modes, 2 states, 1 and 129 (128 + 1), are in the vector
   as 2 different states. Each states needs to be measured at least twice for averaging. This means 4 frames are required to accommodate the measurement.

   For each channel, the number of DA frames are fixed at 8, so user can increase number of DPD frames if needed.
   */
   if( p.m_Rx_Params.performRxCal )
   {
      unsigned int minRxMeasurementPerChannel = (p.m_Rx_Params.numLNAState + p.m_Rx_Params.numLnaStateHiLin) * 2;
      if( minRxMeasurementPerChannel > p.m_numFramesPerChan )
      {
		  return false;
      }
   }
   return true;
}
unsigned char RFCal::GSM_Cal::DO_GSM_V5_CAL( HANDLE hPhone)
{
	GSM_CAL_Results results;
	GSM_CAL_Params params;
	
	// Reboot after reading Char XML
	bool bReboot = false;

	deviceMutex = CreateMutex( NULL, FALSE, NULL); // Create mutex. This must be created outside this funciton. For demo purpose, created here.

	/*Initialize GSM V4 params (DA, DPD, Rx setup similar to QSPR tree)*/
	if ( !AssignGSMV5Params(hPhone, params))
	{
		return false;
	}
	
	// **************************************************************** //
	// Start QMSL Logging
	// **************************************************************** //	
	RFCal::Helper::Log::StartQMSLLogging(hPhone, params.FilePath.stQMSLLog);

	// **************************************************************** //
	// Set up CalDB and EquipConfig
	// **************************************************************** //
	AssignCommonParams ( hPhone, params );

	// **************************************************************** //
	// Set up Equipment and Path Loss
	// **************************************************************** //

	RFCal::DeviceDB::GetInstance().RegisterDevice (hPhone, params.DeviceConfig.type, params.DeviceConfig.boardId, params.DeviceConfig.primaryAddr, params.DeviceConfig.secondaryAddr );
	RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();

	if ( NULL == pMgr ) return false;

	RFCal::SetPathLoss(hPhone, (*pMgr), params.FilePath.stCalDBXML,"GSM");

	// **************************************************************** //
	// Read Char Path
	// **************************************************************** //
	GSM_V5::InputFile::CharXMLPath ( hPhone, params );

	// **************************************************************** //
	// Read Char XML
	// **************************************************************** //

	if ( ! GSM_V5::InputFile::ReadCharXML( hPhone, params) )
	{
		return false;
	}
	if ( !GSM_V5::InputFile::writeDACharToConsolidatedNV( hPhone, params))
	{
		return false;	
	}

	// **************************************************************** //
	// Reboot DUT 
	// **************************************************************** //
	if ( bReboot )
	{
		RFCal::Helper::DIAG::RebootPhone ( hPhone );
	}
	// **************************************************************** //
	// Check FTM mode
	// **************************************************************** //   
	RFCal::Helper::DIAG::Change_FTM_Mode(hPhone, 3, 200);
	
	// Set RF Mode (Query RFC if necessary) 
	RFCal::GSM_Cal::INIT_V5 (hPhone, params);
		
	GSM_V4_PCL_Results_Chain  gsmV4_PCL_Results;
	GSM_V4_DPD_Results_Chain  gsmV4_DPD_Results;
	GSM_V4_RSSI_Est_Chain     gsmV4_RSSI_Results;
	GSM_V4_RSSI_Est_Chain     gsmV4_RSSI_Results_HiLin;

	RFCal::GSM_Cal::GSM_V5::Do_V5_Cal( hPhone, params, gsmV4_PCL_Results, gsmV4_DPD_Results, gsmV4_RSSI_Results, gsmV4_RSSI_Results_HiLin );



	//---------------------------------------------- Process the Tx NV ------------------------------------------------//

	GSM_V4_Tx_Cx_NV_Results tx_NV_Results;
	GSM_V4_Tx_Consolidated_NV_Results consolidated_Tx_NV_Results;
	CGSM_V4_Cal_Params & p = params.gsm_v4_Params;
	// - currently enforce DA and DPD being run together
	if ( p.m_DA_Params.performDaCal && p.m_DPD_Params.performDpdCal && p.m_computeNV )
	{
      //------------ These are the new GSM V5 calibration methods: calibraiton with characterized DA/Rx, and measuring 1 channel of predistortion per band/chain
      // Note: This method only does usePclErrorNV

      // Run a predistortion sweep for 1 channel from each band, and then process the Tx NV for each band.
      RFCal::GSM_Cal::GSM_V5::Tx::RunPredistSweep_And_Process_Tx_NV( hPhone, params, gsmV4_PCL_Results, gsmV4_DPD_Results, tx_NV_Results, consolidated_Tx_NV_Results, results );
   }

	//---------------------------------------------- Process the Rx NV -----------------------------------------------//
	GSM_V4_Rx_Cx_Cal_Meas   rx_Cal_Meas;
	GSM_V4_Rx_Cx_Cal_Meas   rx_Cal_Meas_HiLin;
	GSM_V4_Rx_Cx_NV_Results rx_NV_Results;
	GSM_V4_Rx_Cx_NV_Results rx_NV_Results_HiLin;

	// cache the NV items for NV write later
	unsigned char cacheNVItems = (params.gsm_v4_Params.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

	if ( ( params.gsm_v4_Params.m_Rx_Params.performRxCal ) && params.gsm_v4_Params.m_computeNV )
	{
		//write to consolidated NV item
		if( params.gsm_v4_Params.m_Rx_Params.write_to_consolidated)
		{
			unsigned int chainIdx = 0;

			//low linearity = 0 and high linearity = 1
			enum linearity
			{
				LOW_LIN,
				HIGH_LIN
			};

			//map with Antenna path, chain id, RFM dev, cal type and data
			GSM_RX_MEAS_AntPath chan_rssis_list_consolidated;
			QMSL_RFNV_RX_CAL_GSM_NV gsm_rx_cal_nv;

			//Repeat process for each band
			for ( unsigned int m_curBandIdx = 0; m_curBandIdx < params.gsm_v4_Params.m_bcc.m_numBands; ++m_curBandIdx) //iterate over Bands (max 4)
			{
				std::vector<QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated> gsm_rx_V4_chain_data;
				int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];

				for (chainIdx = 0; chainIdx < params.gsm_v4_Params.m_bcc.m_numRxChains; ++chainIdx) //iterate over chain id
				{

					unsigned int m_pRxChainID = params.gsm_v4_Params.m_bcc.m_rxChainList[chainIdx];
					unsigned int m_dRxChainID = m_pRxChainID + 1;
					unsigned short m_pRxDev = static_cast<unsigned short> (m_pRxChainID);
					unsigned short m_dRxDev = m_pRxDev + 1;
					unsigned short m_pRxAntPath = 65535;
					unsigned short m_dRxAntPath = 65535;

					if(params.setRFMPathVersion != 0)
					{
						CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
						RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, 65535, 0, 0, GSM_TX_BASED_QUERY);
						m_pRxDev = sigPaths.prx.rfm_dev;
						m_dRxDev = sigPaths.drx.rfm_dev;

						m_pRxAntPath = sigPaths.prx.antenna_port;
						m_dRxAntPath = sigPaths.drx.antenna_port;
					}

					//no diversity and no high linearity
					chan_rssis_list_consolidated.data[m_pRxAntPath].data[m_pRxChainID].data[m_pRxDev].data[LOW_LIN].data = gsmV4_RSSI_Results.chainData[m_pRxChainID].estRSSI_Map[m_curBand];

					if ( params.gsm_v4_Params.m_Rx_Params.performDRxCalList[m_curBandIdx])//if doing diversity
					{
						chan_rssis_list_consolidated.data[m_dRxAntPath].data[m_dRxChainID].data[m_dRxDev].data[LOW_LIN].data = gsmV4_RSSI_Results.chainData[m_dRxChainID].estRSSI_Map[m_curBand];
					}
					if( params.gsm_v4_Params.m_Rx_Params.performHiLinRxCal ) //if high linearity
					{
						chan_rssis_list_consolidated.data[m_pRxAntPath].data[m_pRxChainID].data[m_pRxDev].data[HIGH_LIN].data = gsmV4_RSSI_Results_HiLin.chainData[m_pRxChainID].estRSSI_Map[m_curBand];

						if ( params.gsm_v4_Params.m_Rx_Params.performDRxCalList[m_curBandIdx])//if doing diversity
						{
							chan_rssis_list_consolidated.data[m_dRxAntPath].data[m_dRxChainID].data[m_dRxDev].data[HIGH_LIN].data = gsmV4_RSSI_Results_HiLin.chainData[m_dRxChainID].estRSSI_Map[m_curBand];
						}
					}
				}

				//each band will get processed and stored separately
				GSM_V5::Rx::Process_GSM_Rx_V4_Cal_NV_Consolidated( hPhone, params, m_curBand, cacheNVItems, params.gsm_v4_Params.m_Rx_Params, chan_rssis_list_consolidated, gsm_rx_V4_chain_data, gsm_rx_cal_nv);
			}
		}

		//legacy processing technique
		else
		{
			GSM_V5::Rx::Process_GSM_Rx_Cx_NV_From_V4_Cal( hPhone, params, rx_NV_Results, gsmV4_RSSI_Results, rx_Cal_Meas, false,
				params.gsm_v4_Params.m_Rx_Params, params.gsm_v4_Params.m_bcc, params.gsm_v4_Params.m_nv_action);

			if( params.gsm_v4_Params.m_Rx_Params.performHiLinRxCal )
			{
				GSM_V5::Rx::Process_GSM_Rx_Cx_NV_From_V4_Cal( hPhone, params, rx_NV_Results_HiLin, gsmV4_RSSI_Results_HiLin, rx_Cal_Meas_HiLin, true,
					params.gsm_v4_Params.m_Rx_Params, params.gsm_v4_Params.m_bcc, params.gsm_v4_Params.m_nv_action);
			}
		}
	}

	if (params.m_enGsmFtmRfModeExit)
	{
		unsigned short exitStatus;
		QLIB_FTM_RF_MODE_EXIT( hPhone, &exitStatus );

	}

	//--------------------------------------------------Create QCN-----------------------------------------------------------
	RFCal::Helper::NV::WriteCalRFNVToQCN(hPhone,params.FilePath.stCalibratedXQCN, false);
	RFCal::Helper::Log::StopQMSLLogging( hPhone );
	CloseHandle( deviceMutex ); // CloseHendle for the deviceMutex must be called outside this function. For demo purpose, called here.


	return true;
}


//! Process_GSM_Rx_Cx_NV_From_V4_Cal
unsigned char RFCal::GSM_Cal::GSM_V5::Rx::Process_GSM_Rx_Cx_NV_From_V4_Cal( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_Rx_Cx_NV_Results& rx_NV_Results, GSM_V4_RSSI_Est_Chain& gsmV4_RSSI_Results,
	GSM_V4_Rx_Cx_Cal_Meas& rx_Cal_Meas, bool isHiLinearity, GSM_V4_Rx_Params& rxParams, GSM_V4_BandChainChan_Params& bcc,
	unsigned int nvAction )
{
	unsigned int chainIdx = 0;
	unsigned int dRxChain = 0;

	// cache the NV items for NV write later
	unsigned char cacheNVItems = (nvAction & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

	for ( unsigned int m_curBandIdx = 0; m_curBandIdx < bcc.m_numBands; ++m_curBandIdx)
	{
		int m_curBand = bcc.m_bandList[m_curBandIdx];

		for (chainIdx = 0; chainIdx < bcc.m_numRxChains; ++chainIdx)
		{
			int m_curChainID = bcc.m_rxChainList[chainIdx];

			Process_Ind_Rx_Cx_NV( hPhone, params, m_curBand, m_curChainID, cacheNVItems, rx_NV_Results, gsmV4_RSSI_Results, rx_Cal_Meas, m_curChainID, isHiLinearity, rxParams );
			if ( rxParams.performDRxCalList[m_curBandIdx] )
			{
				dRxChain = m_curChainID + 1;
				Process_Ind_Rx_Cx_NV( hPhone, params, m_curBand, m_curChainID, cacheNVItems, rx_NV_Results, gsmV4_RSSI_Results, rx_Cal_Meas, dRxChain, isHiLinearity, rxParams );
			}
		}
	}

	
	return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Rx::Process_Ind_Rx_Cx_NV( HANDLE hPhone, GSM_CAL_Params &params, int m_curBand,int m_curChainID, unsigned char cacheNVItems, GSM_V4_Rx_Cx_NV_Results& rx_NV_Results, GSM_V4_RSSI_Est_Chain& gsmV4_RSSI_Results,
                                            GSM_V4_Rx_Cx_Cal_Meas& rx_Cal_Meas, unsigned int chainID, bool isHiLinearity, GSM_V4_Rx_Params& rxParams )
{

   map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator itr;
   vector<int> lnaPowerList = isHiLinearity? rxParams.lnaPowerListHiLin : rxParams.lnaPowerList;

   unsigned char gainIdx = 0;
   unsigned char chanIdx = 0;

   // fill band_rx_cal_meas
   rx_Cal_Meas.chainMeas[chainID][m_curBand].iChainId        = static_cast<unsigned char>(chainID);
   // by default the NV container is the same as the ChainID
   rx_Cal_Meas.chainMeas[chainID][m_curBand].iNvContainer    = rx_Cal_Meas.chainMeas[chainID][m_curBand].iChainId;
   rx_Cal_Meas.chainMeas[chainID][m_curBand].iNumChannel     = gsmV4_RSSI_Results.chainData[chainID].estRSSI_Map[m_curBand].size();
   if(gsmV4_RSSI_Results.chainData[chainID].estRSSI_Map[m_curBand].empty() == false)
   {
      rx_Cal_Meas.chainMeas[chainID][m_curBand].iNumRxGainRange = gsmV4_RSSI_Results.chainData[chainID].estRSSI_Map[m_curBand].begin()->second.size();
   }
   else
   {
      rx_Cal_Meas.chainMeas[chainID][m_curBand].iNumRxGainRange = 0;
   }

   for(itr =  gsmV4_RSSI_Results.chainData[chainID].estRSSI_Map[m_curBand].begin();
         itr != gsmV4_RSSI_Results.chainData[chainID].estRSSI_Map[m_curBand].end(); ++itr)
   {
      rx_Cal_Meas.chainMeas[chainID][m_curBand].aiChannelList[chanIdx] = static_cast<unsigned short>(itr->first);

      for(gainIdx=0; gainIdx < itr->second.size(); ++gainIdx)
      {
         rx_Cal_Meas.chainMeas[chainID][m_curBand].aiRSSIValue[gainIdx][chanIdx] = itr->second[gainIdx].m_rssi;
         rx_Cal_Meas.chainMeas[chainID][m_curBand].aiCalibrationPwr[gainIdx]     = static_cast<double>(lnaPowerList[gainIdx] );
      }
      ++chanIdx;
   }

   //------------  use the correct NV container if alternate path cal is being done ------------------//
   if ( params.gsm_v4_Params.m_bcc.m_get_nv_container_from_uut_rx )
   {
      // Pass the correct value, either "Primary Rx" or "Diversity Rx"
      unsigned char prx_or_drx = 0;    // 0 indicates Primary Rx
      if (m_curChainID != chainID)
      {
         prx_or_drx = 2;               // 2 indicates Diversity Rx
      }

      // updates "iNvContainer" which determines the NV container to which the GSM Rx cal data is written.

	  RFCal::GSM_Cal::GSM_V5::GSM_Get_NV_Container( hPhone, params, static_cast<FTM_RF_Mode_Enum>(m_curBand), static_cast<unsigned char>(m_curChainID), prx_or_drx, rx_Cal_Meas.chainMeas[chainID][m_curBand].iNvContainer );
 
   }

   if( isHiLinearity )
   {
      QLIB_GSM_RX_CALIBRATION_GENERATE_NV_HI_LIN(
         hPhone,
         static_cast<unsigned short>(m_curBand),
         (void*)&rx_Cal_Meas.chainMeas[chainID][m_curBand],
         (void*)&rx_NV_Results.chainNV[chainID][m_curBand],
         cacheNVItems
      );
   }
   else
   {
      QLIB_GSM_RX_CALIBRATION_GENERATE_NV(
         hPhone,
         static_cast<unsigned short>(m_curBand),
         (void*)&rx_Cal_Meas.chainMeas[chainID][m_curBand],
         (void*)&rx_NV_Results.chainNV[chainID][m_curBand],
         cacheNVItems
      );
   }

   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Rx::Process_GSM_Rx_V4_Cal_NV_Consolidated ( HANDLE hPhone, GSM_CAL_Params & params, int m_curBand, unsigned char cacheNVItems, GSM_V4_Rx_Params& rxParams,
	const GSM_RX_MEAS_AntPath& chan_rssis_list_consolidated, //array with Antenna Path, chain ID, RFM Dev, caltype, channel list, RSSI and size of array
	std::vector<QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated>& gsm_rx_chain_data, QMSL_RFNV_RX_CAL_GSM_NV& gsm_rx_cal_nv )
{

	unsigned int nvVersion = 2; // consolidated nv version = 2

	QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated rxchain_calType_data; //params for consolidated NV

	memset((void*)(&rxchain_calType_data), 0 , sizeof(rxchain_calType_data)); //set everything to zero

	//iterator for array with Antenna Path, chain ID, RFM Dev, caltype, channel list, RSSI and size of array
	map <unsigned short, GSM_RX_MEAS_ChainID>::const_iterator antPathIt = chan_rssis_list_consolidated.data.begin();
	
	//	map<unsigned short, map<unsigned int, map<unsigned short, map<unsigned short, map< int, vector<GSM_RX_MEAS_TYPE> > > > > >::const_iterator antPathIt = chan_rssis_list_consolidated.begin();

	//iterating over the entire map and creating an array
	//iterate over antenna Paths
	while(antPathIt != chan_rssis_list_consolidated.data.end())
	{
		unsigned short antPath = antPathIt->first;

		//iterate over chains
		map<unsigned int, GSM_RX_MEAS_Dev >::const_iterator chainIt = antPathIt->second.data.begin();

		while (chainIt != antPathIt->second.data.end())
		{
			unsigned int chainID = chainIt->first;

			map<unsigned short,  GSM_RX_MEAS_Lin >::const_iterator RFMDevIt = chainIt->second.data.begin();

			//iterate of RFM Device
			while(RFMDevIt != chainIt->second.data.end())
			{
				unsigned short RFMDev = RFMDevIt->first;

				map<unsigned int, GSM_RX_MEAS_Band >::const_iterator calTypeIt = RFMDevIt->second.data.begin();

				//iterate over calibration types
				while( calTypeIt != RFMDevIt->second.data.end() )
				{
					unsigned short calType = calTypeIt->first;

					map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator chanIt;
					unsigned short ch_index = 0;

					//iterate over channels
					for(chanIt = calTypeIt->second.data.begin(); chanIt != calTypeIt->second.data.end(); ++chanIt)
					{
						rxchain_calType_data.aiChannelList[ch_index ] = static_cast<unsigned short>(chanIt->first);

						for(unsigned short gain_index=0; gain_index < chanIt->second.size(); ++gain_index)
						{
							rxchain_calType_data.aiRSSIValue[ gain_index ][ ch_index ] = chanIt->second[ gain_index ].m_rssi;

							rxchain_calType_data.aiCalibrationPwr[ gain_index ] = rxParams.lnaPowerList[gain_index];
						}
						++ch_index;
						rxchain_calType_data.iNumRxGainRange = chanIt->second.size();
					}

					//update items in the params
					rxchain_calType_data.iCaltype = calType;
					rxchain_calType_data.iRFMDevice = RFMDev;
					rxchain_calType_data.iNvContainer = static_cast<unsigned short> (chainID);
					rxchain_calType_data.iChainId = static_cast <unsigned char>(chainID);
					rxchain_calType_data.iAntennaPath = antPath;
					rxchain_calType_data.iNumChannel = calTypeIt->second.data.size(); //ch_index

					//push data into array
					gsm_rx_chain_data.push_back(rxchain_calType_data);

					++calTypeIt;
				}
				++RFMDevIt;
			}
			++chainIt;
		}
		++antPathIt;
	}

	QLIB_GSM_RX_CALIBRATION_GENERATE_NV_CONSOLIDATED(
		hPhone,
		static_cast<unsigned short>(m_curBand),
		(void*)&gsm_rx_chain_data[0],
		gsm_rx_chain_data.size(),
		(void*)&gsm_rx_cal_nv,
		cacheNVItems,
		nvVersion
		);

	return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Inspect_GSM_Tx_Consolidated_NV_PredistData_Validity( HANDLE hPhone, GSM_CAL_Params &params, FTM_RF_Mode_Enum curBand, unsigned short txAntPath, unsigned int txSigPath, unsigned short txRfmDev, int isDataValid ) 
{
   return ( isDataValid == 0 )? 0 : 1; 
};
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::DetermineTxNvContainer( HANDLE hPhone, GSM_CAL_Params &params, bool getNvContainerFromUUT )
{
   params.m_txNvContainer = static_cast<unsigned char>(params.chainID);   // default the NV container to equal the chainID

   if (getNvContainerFromUUT)
   {
      FTM_RF_Mode_Enum rf_mode = RFCal::Helper::Convert::GSMBandClassToRFMode( static_cast < BAND_CLASS_ENUM > ( params.gsm_v4_Params.m_bcc.m_bandList[0] ));

      // updates nvContainer" which determines the NV container to which the GSM Tx cal data is written.
      // Must pass the primary chainID to this function, as well as "1" to indicate Tx.
      unsigned char nvContainer = 0;
      RFCal::GSM_Cal::GSM_V5::GSM_Get_NV_Container ( hPhone, params, rf_mode, static_cast<unsigned char>(params.chainID), 1,  nvContainer);
      params.m_txNvContainer = static_cast < int > ( nvContainer );
   }
   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Determine_PreDist_Cal_RGI_from_DPD( HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, map<unsigned int, GSM_V4_DPD_Sweep_Record>& chanDigGainPwr, double preDistCalPower,
	double preDistPowerBuffer )
{
	stringstream ostr;
	bool chanRgiReached       = false;
	double curPower           = -1000.0;
	unsigned short rgiMatch   = 0;
	unsigned short rgiAllChan = 0;
	GSM_V4_BandChainChan_Params & bcc =  params.gsm_v4_Params.m_bcc;

	GSM_V4_DPD_Sweep_Record* powMeas;

	// Get the V4 - V3 power delta for help in determining the predistortion RGI
	//OpPwrTableGeneratorV2* opPwrTable = OpPwrTableGeneratorV2::getObject();
	double V4PowerDescripancy = 0;
	//if ( params.gsm_v4_Params.m_DPD_Params.useV3CharData )
	//{
	//	V4PowerDescripancy= opPwrTable->getDigGainDelta_V4_minus_V3(m_curBand);
	//}

	// buffer to prevent very near preDistCalPower from causing this to fail.
	// For example, if preDistCalPower = 27 dBm, 26.98 dBm shouldn't cause it to fail.
	// Additionally, the modem software uses 0.5 dB backoff form target. So, a target of 27 dBm is really 26.5 dBm in PCL terms.
	const double powerThresh = preDistCalPower - preDistPowerBuffer + V4PowerDescripancy;

	const unsigned short digGainForRgiCalc = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.digGainForPredistRGI);

	int m_curChan;
	for(unsigned char chanIdx = 0; chanIdx < bcc.m_numChan; ++chanIdx)
	{
		m_curChan = bcc.m_chanList[m_curBandIdx][chanIdx];
		powMeas  = &chanDigGainPwr[m_curChan];

		chanRgiReached  = false;

		if ( powMeas->digGainSweep.find(digGainForRgiCalc) != powMeas->digGainSweep.end() )
		{
			// search through RGI list for this digital gain
			// ma<RGI, power>
			map<unsigned short, double>::iterator rgiPowIter;
			for ( rgiPowIter = powMeas->digGainSweep[digGainForRgiCalc].digGainPwr.begin();
				rgiPowIter != powMeas->digGainSweep[digGainForRgiCalc].digGainPwr.end();
				++rgiPowIter )
			{
				curPower = rgiPowIter->second;
				if (curPower >= powerThresh)
				{
					rgiMatch = rgiPowIter->first;
					chanRgiReached = true;
					break;
				}
			}
			if(!chanRgiReached)
			{
				ostringstream ostr;
				ostr << "Failed to acheive target predistortion power of " << preDistCalPower << " dBm.\n"
					<< " Acheived " << curPower << " dBm. \n"
					<< " Check if target predistortion power is too high and consider using bigger DPD RGIs. \n"
					<< " Chain = " << params.chainID << "\n"
					<< " Band  = " << params.band_list[m_curBandIdx] << "\n"
					<< " Chan  = " << m_curChan << "\n"
					<< "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
				RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
				return false;
			}

			if(rgiMatch > rgiAllChan)
			{
				rgiAllChan = rgiMatch;
			}

		}
		else
		{
			ostr.str("");
			ostr << "DPD digital gain list doesn't contain " << digGainForRgiCalc << ".\n"
				<< "Consider adding " << digGainForRgiCalc << " into the digital gain list.\n"
				<< "band = " << params.band_list[m_curBandIdx] << "\n"
				<< "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
			RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
			return false;
			
		}
	}

   params.gsm_v4_Params.m_globals.m_predistCalRgi = rgiAllChan;

	return true;

}
														  

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Determine_PreDist_Cal_RGI_from_DPD_AvgPow( HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, int m_curChainID, const vector< int>& chanList, map<unsigned int, GSM_V4_DPD_Sweep_Record>& chanDigGainPwr,
                                                   const vector<unsigned int>& rgiList, double preDistCalPower )
{
   // The predistortion RGI, which will be written to the Tx NV.
   unsigned short rgiPreDist = 0;

   const double MAX_POW_DELTA = 1.0;
   int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
   //---------------------------------- Step 1: Compute Average Power Deltas ---------------------------------//
   // Average Power Deltas = average( chanPower - targetPower) across all channels.

   // # of channels, required for averaging
   unsigned int numChannels = chanList.size();
   unsigned int chanIdx     = 0;

   unsigned int curChan  = 0;
   unsigned short curRgi = 0;

   // Stores the average power deltas
   vector<double> avgRgiPowDelta_Vec( rgiList.size() );

   double curPowerDelta  = 0.0;
   double power          = 0.0;

   stringstream ostr;
   for ( unsigned short rgiIdx=0; rgiIdx<rgiList.size(); ++rgiIdx )
   {
      curRgi = static_cast<unsigned short>( rgiList[rgiIdx] );

      for ( chanIdx=0; chanIdx<numChannels; ++chanIdx )
      {
         curChan = chanList[chanIdx];

         // make sure the digGainForRgiCal exists in the data
         if ( chanDigGainPwr[curChan].digGainSweep.find(static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.digGainForPredistRGI)) == chanDigGainPwr[curChan].digGainSweep.end() )
         {
            ostr.str("");
            ostr << "DPD digital gain list doesn't contain RGI " << params.gsm_v4_Params.m_DPD_Params.digGainForPredistRGI
                 << ". Consider adding the RGI into DPD digital gain list. "
                 << " Current channel: " << curChan << ".\n"
                 << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
            RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
            return false;
         }

         // The absolute power delta for the current channel
         curPowerDelta  = fabs( chanDigGainPwr[curChan].digGainSweep[static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.digGainForPredistRGI)].digGainPwr[curRgi] - preDistCalPower );

         // convert to intenities, in order to average properly
         power += pow(10.0, curPowerDelta/20.0 );
      }

      // average the power, and convert back to dB
      avgRgiPowDelta_Vec[rgiIdx] = 20*log10(power / numChannels);
      // reset power for each RGI
      power = 0.0;
   }

   //---------------------------------- Step 2: Find the Minimum Absolute Power Delta and Predist RGI  ---------------------------------//
   // Look at the power deltas for each RGI in avgRgiPowDelta_Vec[rgiIdx]
   vector<double>::iterator smallestElem = min_element( avgRgiPowDelta_Vec.begin(), avgRgiPowDelta_Vec.end() );

   unsigned short smallestIdx = static_cast<unsigned short>( distance( avgRgiPowDelta_Vec.begin(), smallestElem ) );

   curRgi = static_cast<unsigned short>( rgiList[smallestIdx] );

   // verify that the smallest power delta isn't too large
   if ( avgRgiPowDelta_Vec[smallestIdx] > MAX_POW_DELTA )
   {
      ostringstream ostr;
      ostr << "Measured target predistortion power is too far from target predistortion power: \n"
           << "Avg Target Power Delta of " << avgRgiPowDelta_Vec[smallestIdx]  << " dB exceeds " << MAX_POW_DELTA << " dB\n "
           << "       Chain = "  << m_curChainID << "\n"
           << "        Band = "  << m_curBand    << "\n"
           << " Closest RGI = "  << curRgi       << "\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__ << "\n"
           << "Please adjust the candiate RGI list to correct for this issue." ;

      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
      return false;
   }

   rgiPreDist = static_cast<unsigned short>( rgiList[smallestIdx] );


   params.gsm_v4_Params.m_globals.m_predistCalRgi = rgiPreDist;

   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Pack_GSM_DPD_For_QMSL( HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, GSM_V4_DPD_Results_Band_Chan& gsmV4_DPD_Results, QMSL_GSM_EnhLinTx_Cal_Control& calControl_ToQMSL,
                                             QMSL_GSM_EnhLinTx_Cal_Data& calDPD_ToQMSL, unsigned short calRgi , int m_curChainID)
{
   // the char DAC corresponding to the maximum digital gain output power in the char data set
   float dacAtMaxDigGain = 0;

   // This is the RGI of the selected char DPD data set.
   // If adaptive char DPD data selection is disabled, this equals m_predistCalRgi
   unsigned short curRgi  = 0;
   unsigned short digGain = 0;
   unsigned int   opsPos  = 0;
   unsigned int   daqPos  = 0;
   int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];

   // if true, pick char data set based upon power measurements.
   // if false, pick char data set based solely on calRgi
   bool m_useAdaptiveCharRgiSelection   = true;
   // if true, adaptively determine the max power delta for power adaptation
   // if false, use the power at the fixed highes digital gain from the tree params for determining the max power delta
   bool m_useAdaptiveMaxPowerEstimation = params.gsm_v4_Params.m_DPD_Params.doAdaptiveMaxPowEst != 0;

   memset( &calControl_ToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Control) );
   memset( &calDPD_ToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Data) );

   calControl_ToQMSL.bSlopeCorrAlignLow     = params.gsm_v4_Params.m_DPD_Params.doSlopeCorrectionAlignLow;
   calControl_ToQMSL.bDoPowerAdaptation     = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.doPowerAdaptation);
   calControl_ToQMSL.bDoSlopeCorrection     = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.doSlopeCorrection);
   calControl_ToQMSL.bDoPiecewiseAdaptation = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.doPiecewiseAdapt);
   calControl_ToQMSL.iAlgType               = 1;

   calDPD_ToQMSL.iCalRgi      = params.gsm_v4_Params.m_globals.m_predistCalRgi;
   calDPD_ToQMSL.dDacScale    = static_cast<float>( params.gsm_v4_Params.m_DPD_Params.dacScale );
   calDPD_ToQMSL.iNumChannels = params.gsm_v4_Params.m_bcc.m_numChan;

   BAND_CLASS_ENUM bandEnum = RFCal::Helper::Convert::ConverGSMBand_To_BandClassEnum( m_curBand );
   stringstream ostr;
#ifdef INCLUDE_V4_CHAR_DPD_DEBUG_INFO
   GLOBAL_DUT_RGI[m_curChainID][m_curBand] = m_predistCalRgi;
#endif
   // Pack characterized data
   for (unsigned int chanIdx=0; chanIdx < calDPD_ToQMSL.iNumChannels; ++chanIdx)
   {
      calDPD_ToQMSL.iFreqMapping[chanIdx] = static_cast<QMSL_GSM_Polar_Cal_Frequency>( params.gsm_v4_Params.m_DPD_Params.nvChanMap[chanIdx] );

      int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

      vector<struct opPwrRecord> ops;
      vector<struct digRecord>   daqs;

      curRgi = Select_CharDPD_DataSet_V4( hPhone, params, static_cast<unsigned short>(params.gsm_v4_Params.m_globals.m_predistCalRgi),
                                          static_cast<unsigned short>(m_curBand),
                                          static_cast<unsigned short>(m_curChan),
                                          static_cast<unsigned short>(m_curChainID),
                                          m_useAdaptiveCharRgiSelection, ops, daqs,
                                          gsmV4_DPD_Results.band_Chan_sweepResults[m_curBand][m_curChan].digGainSweep );

#ifdef INCLUDE_V4_CHAR_DPD_DEBUG_INFO
      GLOBAL_CHAR_OPS[m_curChainID][m_curBand][m_curChan] = ops;
      GLOBAL_CHAR_DIG[m_curChainID][m_curBand][m_curChan] = daqs;
      GLOBAL_CHAR_RGI[m_curChainID][m_curBand][m_curChan] = curRgi;
#endif

      ostr.str("");
      ostr << "Char RGI(" << m_curChan << ") = " << curRgi;
      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
      ostr.str("");
      if ( m_useAdaptiveMaxPowerEstimation )
      {
         dacAtMaxDigGain = DetermineDacAtDigGainPwr( ops, daqs[daqs.size()-1].pwr, QMSL_GSM_NUM_DAC_AMAM_TABLE );
         // Estimate the DUT power at the maximum char Digital Gain
         // Function returns the esimated power, but also adds it to the DUT digGainSweep
         EstimatePowerAtMaxDigGain( hPhone, ops[QMSL_GSM_NUM_DAC_AMAM_TABLE-1].amam, ops[QMSL_GSM_NUM_DAC_AMAM_TABLE-1].oppwr, daqs,
                                    gsmV4_DPD_Results.band_Chan_sweepResults[m_curBand][m_curChan].digGainSweep, params.gsm_v4_Params.m_globals.m_dutDataRgi, dacAtMaxDigGain,
                                    static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.digGainForPredistRGI), calDPD_ToQMSL.dDacScale );
      }

      // Sanity check number of records
      if (ops.size() != QMSL_GSM_NUM_DAC_AMAM_TABLE)
      {
         ostr.str("");
         ostr << "Input characterization curve has " << ops.size() << " points, it should have "
              << QMSL_GSM_NUM_DAC_AMAM_TABLE << " points.\n"
              << "Please validate the characterization input file.\n"
              << "(band=" << bandEnum << ", chan=" << m_curChan << ", rgi=" << params.gsm_v4_Params.m_globals.m_predistCalRgi << ")\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }

      // Pack characterized predistortion curve
      for( opsPos=0; opsPos<QMSL_GSM_NUM_DAC_AMAM_TABLE; ++opsPos )
      {
         calDPD_ToQMSL.aCharCurves[chanIdx].dCharDac[opsPos]   = ops[opsPos].amam;
         calDPD_ToQMSL.aCharCurves[chanIdx].dCharPower[opsPos] = ops[opsPos].oppwr;
         calDPD_ToQMSL.aCharCurves[chanIdx].dCharAmpm[opsPos]  = ops[opsPos].ampm;
      }

      //------------------------------- Pack measured and characterized digital gains from DUT ------------------------------//
      bool useAllPoints = params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].empty();

      calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries = 0;
      unsigned int digGainTablePos                   = 0;

      //channelSweepResults dimension: digital gain -> RGI -> power
      GSM_V4_DPD_Sweep_Record* channelSweepResults = &gsmV4_DPD_Results.band_Chan_sweepResults[m_curBand][m_curChan];
      map<unsigned short, GSM_DA_Cal_DigGain_Record>::iterator digGainIter;

      for (digGainIter= channelSweepResults->digGainSweep.begin(); digGainIter!=channelSweepResults->digGainSweep.end(); ++digGainIter )
      {
         if (digGainIter->second.digGainPwr.find(params.gsm_v4_Params.m_globals.m_dutDataRgi) != digGainIter->second.digGainPwr.end())
         {
            digGain = static_cast<unsigned short>( digGainIter->first );

            bool usePoint = find( params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].begin(),
                                  params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].end(),
                                  digGain )
                            != params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].end();

            if (digGainTablePos >= QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
            {
               ostr.str("");
               ostr << "DUT digital gains table has over " << digGainTablePos << " entries, QMSL can handle at most "
                    << (QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE) << " entries.\n"
                    << "Check CAL tree DPD setting to reduce excessive DPD RGIs and digital gains.\n"
                    << "(band=" << bandEnum << ", chan=" << m_curChan << ", rgi=" << params.gsm_v4_Params.m_globals.m_dutDataRgi << ")\n"
                    << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
               RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
               return false;
            }

            calDPD_ToQMSL.aDutDigGain[chanIdx].iDigGain[digGainTablePos] = digGain;
            calDPD_ToQMSL.aDutDigGain[chanIdx].dPower[digGainTablePos]   = static_cast<float>( digGainIter->second.digGainPwr[params.gsm_v4_Params.m_globals.m_dutDataRgi] );
            calControl_ToQMSL.bSlopeCorrUseDutPoint[digGainTablePos]     = usePoint || useAllPoints;
            ++calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries;
            ++digGainTablePos;
         }
      }

      //------------------------ Pack characterized digital gain table ------------------------//
      if (daqs.size() > QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE)
      {
         ostr.str("");
         ostr << "Input characterization digital gains table has " << daqs.size() << " entries, QMSL can handle at most "
              << (QMSL_NV_MANAGER_ENH_CAL_DIG_GAIN_TABLE_SIZE) << " entries. "
              << "Please validate the characterization input file.\n"
              << "(band=" << bandEnum << ", chan=" << m_curChan << ", rgi=" << params.gsm_v4_Params.m_globals.m_predistCalRgi << ")\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }

      // only use the char values that correspond to the cal values - ignore all other digGain/power resuls
      calDPD_ToQMSL.aCharDigGain[chanIdx].iNumEntries = calDPD_ToQMSL.aDutDigGain[0].iNumEntries;  // same # of entries per channel

      digGainTablePos = 0;

      for ( daqPos=0; daqPos<daqs.size(); ++daqPos )
      {
         digGain = static_cast<unsigned short>( daqs[daqPos].digGain );

         if ( find( params.gsm_v4_Params.m_DPD_Params.dpd_DigGainList[m_curBandIdx].begin(),
                    params.gsm_v4_Params.m_DPD_Params.dpd_DigGainList[m_curBandIdx].end(),
                    digGain )
               == params.gsm_v4_Params.m_DPD_Params.dpd_DigGainList[m_curBandIdx].end()
            )
         {
            continue;
         }

         // ---- Slope correction list checking
         calControl_ToQMSL.bSlopeCorrUseCharPoint[digGainTablePos] = calControl_ToQMSL.bSlopeCorrUseDutPoint[digGainTablePos];

         calDPD_ToQMSL.aCharDigGain[chanIdx].iDigGain[digGainTablePos] = digGain;
         calDPD_ToQMSL.aCharDigGain[chanIdx].dPower[digGainTablePos]   = daqs[daqPos].pwr;

         ++digGainTablePos;
      }

      if ( calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries < 1 && params.gsm_v4_Params.m_DPD_Params.doPowerAdaptation )
      {
         ostr.str("");
         ostr << "DUT digital gains table has " << calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries << " entries " <<
              "for band " << bandEnum << ", channel " << m_curChan << ", rgi " << params.gsm_v4_Params.m_globals.m_dutDataRgi << ", "
              "need at least 1 to do power adaptation.\n" << "Check CAL tree DPD RGI list and digital gain list.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }
      if ( calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries < 2 && (params.gsm_v4_Params.m_DPD_Params.doSlopeCorrection || params.gsm_v4_Params.m_DPD_Params.doPiecewiseAdapt) )
      {
         ostr.str("");
         ostr << "DUT digital gains table has " << calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries << " entries " <<
              "for band " << bandEnum << ", channel " << m_curChan << ", rgi " << params.gsm_v4_Params.m_globals.m_dutDataRgi << ", "
              "need at least 2 to do slope correction.\n" << "Check CAL tree DPD RGI list and digital gain list.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }
      if ( calDPD_ToQMSL.aCharDigGain[chanIdx].iNumEntries < 1 && params.gsm_v4_Params.m_DPD_Params.doPowerAdaptation )
      {
         ostr.str("");
         ostr << "Characterized digital gains table has " << calDPD_ToQMSL.aCharDigGain[chanIdx].iNumEntries << " entries " <<
              "for band " << bandEnum << ", channel " << m_curChan << ", rgi " << params.gsm_v4_Params.m_globals.m_predistCalRgi << ", "
              "need at least 1 to do power adaptation.\n" << "Check CAL tree DPD RGI list and digital gain list.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }
      if ( calDPD_ToQMSL.aCharDigGain[chanIdx].iNumEntries < 2 && (params.gsm_v4_Params.m_DPD_Params.doSlopeCorrection || params.gsm_v4_Params.m_DPD_Params.doPiecewiseAdapt) )
      {
         ostr.str("");
         ostr << "Characterized digital gains table has " << calDPD_ToQMSL.aCharDigGain[chanIdx].iNumEntries << " entries " <<
              "for band " << bandEnum << ", channel " << m_curChan << ", rgi " << params.gsm_v4_Params.m_globals.m_predistCalRgi << ", "
              "need at least 2 to do slope correction.\n" << "Check CAL tree DPD RGI list and digital gain list.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }
   }  // END CHANNEL LOOP

   return true;
}
float RFCal::GSM_Cal::GSM_V5::Tx::EstimatePowerAtMaxDigGain( HANDLE hPhone, float maxCharAMAM, float maxCharPwr, vector<struct digRecord>& charDaqs,
	map<unsigned short, GSM_DA_Cal_DigGain_Record>& dutDigGainSweep, unsigned short calRgi,
	float dacAtMaxDigGain, unsigned short edgeDigGain, float dacScale )
{

   unsigned int numDigGain = charDaqs.size();

   // digital gain at the last digital gain index
   unsigned short digGain = static_cast<unsigned short>( charDaqs[numDigGain-1].digGain );

   //--------------------- First, make sure that maxCharAMAM != dacAtMaxDigGain ----------------------//
   // If so, break out of this function, and default to the direct digital gain method.
   if ( maxCharAMAM == dacAtMaxDigGain )
   {
      // use (digGain + 1) only for the purpose of having a unique element.
      // we still need to have the same # of elements per channel. Other channels may have numDigGain + 1 elements.
      unsigned short maxDigGain = digGain + 1;

      //---------- Add the maxDutDigGain and maxDutPwr to the DUT data set.
      // copy the same power over
      dutDigGainSweep[maxDigGain].digGainPwr[calRgi] = dutDigGainSweep[digGain].digGainPwr[calRgi] ;

      //---------- Add a digitalGain vs. Pow record to char data
      // resuse the charPower @ the maximum digital gain
      struct digRecord maxChar(maxDigGain, charDaqs[numDigGain-1].pwr );
      charDaqs.push_back(maxChar);
   }

   //---------------------- The required minimum dimension for performing a polynomial fit ------------------------//
   const unsigned int numRow = 4;
   if (numDigGain < numRow)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "DUT requires at least " << numRow << " digital gains. It has " << numDigGain << ". " << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str());
	
   }

   // column matrix dimension
   unsigned int numCol = numRow;

   // determine the indexes to use for the charDaqs for polynomial estimation
   unsigned short idx0 = 0;
   unsigned short idx1 = 1;
   unsigned short idx2 = 2;
   unsigned short idx3 = 3;
   if (numDigGain > 4)
   {
      idx3 = static_cast<unsigned short>(numDigGain - 1);
   }

   // digital gains in dB
   float digGain_dB_Vec[4] = { 20*log10(charDaqs[idx0].digGain), 20*log10(charDaqs[idx1].digGain), 20*log10(charDaqs[idx2].digGain), 20*log10(charDaqs[idx3].digGain) };

   //------------------------------------- Estimate maxDigGaindB ----------------------------------//
   // Purpose: Estimate the digital gain corresponding to the maximum AMAM/DAC from the char data set
   // Method:  Interpolate between the estimated DAC corresponding to the digital gain of 60 in the char data
   //          and the max possible DAC and digital gain combination.

   const float maxDigGain   = static_cast<float>( ( edgeDigGain * 2 ) - 1 );      // Max possible digital gain for 8PSK modulation
   const float maxDac       = 16384 * dacScale - 1;                               // Max possible DAC/AMAM value (16384 = 128*128 --> modem constant)
   float maxDac_dB          = 20*log10(maxDac);
   float dacAtMaxDigGain_dB = 20*log10(dacAtMaxDigGain);
   float maxCharAMAM_dB     = 20*log10(maxCharAMAM);        // dB of the maximum AMAM/DAC from the char data

   // a check is done to make sure that ( maxDac_dB - dacAtMaxDigGain_dB ) != 0 earlier in this funciton
   float digGainEstSlope = ( 20*log10(maxDigGain) - digGain_dB_Vec[3] ) / ( maxDac_dB - dacAtMaxDigGain_dB );

   float maxDigGain_dB = digGainEstSlope*(maxCharAMAM_dB - dacAtMaxDigGain_dB) + digGain_dB_Vec[3];

   float maxDutDigGain = pow( 10, maxDigGain_dB / 20 );

   //---------------------------------------- Estimate maxDutPwr --------------------------------------//
   // Purpose: Estimate maxDutPwr, which is the DUT power corresponding to maxDutDigGain
   // Method:  Use a polynomial extrapalation of the measured digital gain vs power data, ot obtain the maxDutPwr

   float maxDutPwr = 0.0;

   // X_mat = digital gain matrix for polynomial estimation
   R2MATRIX* X_mat;
   X_mat = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixAllocate( hPhone, numRow, numCol, sizeof(float));
   float** X_ptr = (float**)X_mat->dataPtr;

   // Fill the digital gain matrix for polynomial coefficient determination
   float curDigGain = 0.0;

   for (unsigned int rowIdx=0; rowIdx<numRow; ++rowIdx)
   {
      X_ptr[rowIdx][0] = 1;
      curDigGain = digGain_dB_Vec[rowIdx];
      for (unsigned int colIdx=1; colIdx<numCol; ++colIdx)
      {
         X_ptr[rowIdx][colIdx] = X_ptr[rowIdx][colIdx-1]*curDigGain;
      }
   }

   // Y = matrix of power values for power estimation
   R2MATRIX* Y = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixAllocate(hPhone, numRow, 1, sizeof(float));
   float** y = (float**)Y->dataPtr;

   float pow0 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx0].digGain) ].digGainPwr[calRgi] );
   float pow1 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx1].digGain) ].digGainPwr[calRgi] );
   float pow2 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx2].digGain) ].digGainPwr[calRgi] );
   float pow3 = static_cast<float>( dutDigGainSweep[ static_cast<unsigned short>(charDaqs[idx3].digGain) ].digGainPwr[calRgi] );
   y[0][0] = pow0;
   y[1][0] = pow1;
   y[2][0] = pow2;
   y[3][0] = pow3;

   // X_mat_T = transpose of X_mat
   R2MATRIX* X_mat_T = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixTranspose( hPhone, X_mat);
   // C_mat = X_mat_T * X_mat
   R2MATRIX* C_mat = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixMult( hPhone, X_mat_T, X_mat);
   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree( hPhone,X_mat);
   // Cinv_mat = inverse(C_mat)
   R2MATRIX* Cinv_mat = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixInvert( hPhone, C_mat);
   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree( hPhone, C_mat);
   // E = Cinv_mat * X_mat_T
   R2MATRIX* E = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixMult( hPhone,Cinv_mat, X_mat_T);
   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree( hPhone, Cinv_mat);
   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree( hPhone, X_mat_T);

   // Find the estmated polynomial coefficients - Estimation for Power (dB) vs digital gain (dB)
   R2MATRIX* A = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixMult( hPhone, E, Y);
   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree( hPhone, E);
   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree(hPhone, Y);
   float** a = (float**)A->dataPtr;
   float c3 = a[3][0];
   float c2 = a[2][0];
   float c1 = a[1][0];
   float c0 = a[0][0];

   RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree(hPhone, A);

   // find the estimated DUT power at max digital gain
   maxDutPwr = c3*(maxDigGain_dB*maxDigGain_dB*maxDigGain_dB) + c2*(maxDigGain_dB*maxDigGain_dB) + c1*(maxDigGain_dB) + c0;

   // compute max power error
   //float estPwr = c3*(digGain_dB_Vec[3]*digGain_dB_Vec[3]*digGain_dB_Vec[3]) + c2*(digGain_dB_Vec[3]*digGain_dB_Vec[3]) + c1*(digGain_dB_Vec[3]) + c0;
   //float perr =  static_cast<float>( dutDigGainSweep[digGain].digGainPwr[calRgi] ) - estPwr;
   //maxDutPwr += perr;

   // make sure the maxDutPwr is not less than the max power already present
   if ( maxDutPwr < dutDigGainSweep[digGain].digGainPwr[calRgi] )
   {
      maxDutPwr = static_cast<float>( dutDigGainSweep[digGain].digGainPwr[calRgi] );
   }

   //--------------------------- Add the extra Digital Gain vs. Power data to the Char and DUT Data Sets -----------------------//

   //---------- Add the maxDutDigGain and maxDutPwr to the DUT data set.
   dutDigGainSweep[static_cast<unsigned short>(maxDutDigGain)].digGainPwr[calRgi] = maxDutPwr;

   //---------- Add a digitalGain vs. Pow record to char data
   struct digRecord maxChar(maxDutDigGain, maxCharPwr);
   charDaqs.push_back(maxChar);

   return maxDutPwr;
}

//-------------------------------------------- 2D Matrix Operations ------------------------------------------------//
//! R2MatrixAllocate
RFCal::R2MATRIX * RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixAllocate( HANDLE hPhone, unsigned int numRows, unsigned int numCols, int elementSize)
{
	unsigned int i;
	RFCal::R2MATRIX *A;
	float **floatMatrix;

	A = (RFCal::R2MATRIX *)calloc(1,sizeof(RFCal::R2MATRIX));
	if (!A)
	{
		ostringstream ostr;
		ostr.str("");
		ostr << "Error allocating matrix! "  << __FUNCTION__;
		RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
		//throw CApplicationException( g_exc_general_exception, ostr.str() );
	}
	A->numRows = numRows;
	A->numCols = numCols;
	A->elemSize = elementSize;

	floatMatrix = (float**)calloc(numRows,sizeof(float *));
	if (!floatMatrix)
	{
		free(A);

		ostringstream ostr;
		ostr.str("");
		ostr << "Error allocating matrix! "  << __FUNCTION__;
		RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
		//throw CApplicationException( g_exc_general_exception, ostr.str() );
	}
	for (i=0; i<numRows; i++)
	{
		floatMatrix[i] = (float*)calloc(numCols,sizeof(float));
		if (!floatMatrix[i])
		{
			free(floatMatrix);
			free(A);

			ostringstream ostr;
			ostr.str("");
			ostr << "Error allocating matrix! "  << __FUNCTION__;
			RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
			//throw CApplicationException( g_exc_general_exception, ostr.str() );
		}
	}
	A->dataPtr = (char**)floatMatrix;

	return (A);
}
//! R2MatrixInvert
RFCal::R2MATRIX *RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixInvert(HANDLE hPhone, RFCal::R2MATRIX *A)
{
   // make sure the matrix is square
   if (A->numRows != A->numCols)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: Non-Square Matrix! "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
    //  throw CApplicationException( g_exc_general_exception, ostr.str() );
   }
   if (!A->dataPtr)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: Non-Square Matrix! "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
   //   throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   float big, pivotInverse, temp, absElement;
   int *pivotFlag = NULL, *swapCol = NULL, *swapRow = NULL;
   int i, n, row, col, swap, irow = 0, icol = 0;

   n = A->numRows;

   RFCal::R2MATRIX *Ai = RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixAllocate( hPhone, n, n, sizeof(float));
   SCALE_R2MATRIX(A, Ai, 1, n, n, 0, 0, float, float)
   float **a = (float**)Ai->dataPtr;

   pivotFlag = (int*)calloc(n, sizeof(int));
   swapRow   = (int*)calloc(n, sizeof(int));
   swapCol   = (int*)calloc(n, sizeof(int));

   if (!pivotFlag || !swapRow || !swapCol)
   {
      if(pivotFlag != NULL)
         free(pivotFlag);
      if(swapRow != NULL)
         free(swapRow);
      if(swapCol != NULL)
         free(swapCol);

      free(Ai);

      ostringstream ostr;
      ostr.str("");
      ostr << "Error: Matrix Allocation! "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
     // throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   for (i=0; i<n; i++)
   {
      big = 0.0;
      for (row=0; row<n; row++)
      {
         if (!pivotFlag[row])
         {
            for (col=0; col<n; col++)
            {
               if (!pivotFlag[col])
               {
                  absElement = fabs(a[row][col]);
                  if (absElement >= big)
                  {
                     big = absElement;
                     irow = row;
                     icol = col;
                  }
               }
            }
         }
      }

      pivotFlag[icol]++;

      if (irow != icol)
      {
         for (col=0; col<n; col++)
         {
            temp = a[irow][col];
            a[irow][col] = a[icol][col];
            a[icol][col] = temp;
         }
      }

      swapRow[i] = irow;
      swapCol[i] = icol;

      if (a[icol][icol] == 0.0)
      {
         free(swapRow);
         free(swapCol);
         free(pivotFlag);
         free(Ai);

         ostringstream ostr;
         ostr.str("");
         ostr << "Error: Singular Matrix! "  << __FUNCTION__;
		 RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
     //    throw CApplicationException( g_exc_general_exception, ostr.str() );
      }

      pivotInverse = static_cast<float>(1.0/a[icol][icol]);
      a[icol][icol] = 1.0;
      for (col=0; col<n; col++)
      {
         a[icol][col] = a[icol][col]*pivotInverse;
      }
      for (row=0; row<n; row++)
      {
         if (row != icol)
         {
            temp = a[row][icol];
            a[row][icol] = 0.0;
            for (col=0; col<n; col++)
            {
               a[row][col] = a[row][col] - a[icol][col]*temp;
            }
         }
      }
   }

   for (swap=n-1; swap>=0; swap--)
   {
      if (swapRow[swap] != swapCol[swap])
      {
         for (row=0; row <n; row++)
         {
            temp = a[row][swapRow[swap]];
            a[row][swapRow[swap]] = a[row][swapCol[swap]];
            a[row][swapCol[swap]] = temp;
         }
      }
   }

   free((char*)pivotFlag);
   free((char*)swapRow);
   free((char*)swapCol);

   return (Ai);
}

//! R2MatrixTranspose
RFCal::R2MATRIX * RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixTranspose(HANDLE hPhone, RFCal::R2MATRIX *A)
{
   if(!A->dataPtr)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: matrix A is unallocated. "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
     // throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   RFCal::R2MATRIX *A_T = R2MatrixAllocate( hPhone, A->numCols, A->numRows, A->elemSize);

   float **A_T_ptr = (float**)A_T->dataPtr;

   float **A_ptr = (float**)A->dataPtr;

   for (unsigned int rowIdx=0; rowIdx<A->numRows; ++rowIdx)
   {
      for (unsigned int colIdx=0; colIdx<A->numCols; ++colIdx)
      {
         A_T_ptr[rowIdx][colIdx] = A_ptr[colIdx][rowIdx];
      }
   }

   return (A_T);
}

//! R2MatrixMult
RFCal::R2MATRIX *RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixMult( HANDLE hPhone, RFCal::R2MATRIX *A, RFCal::R2MATRIX *B)
{
   if(B->numRows != A->numCols)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: matrix B row and matrix A col sizes must be equal. "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   if(!A->dataPtr)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: matrix A is unallocated. "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }
   if(!B->dataPtr)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: matrix B is unallocated. "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   // C = A * B
   RFCal::R2MATRIX *C;

   // simplify the row and col variables
   int aR = A->numRows;
   int aC = A->numCols;
   int bC = B->numCols;

   // allocate C to be of the highest ranking type of A and B
   // - largest element size gives the highest rank
   if( (A->elemSize) > (B->elemSize) )
   {
      C = R2MatrixAllocate( hPhone, aR, bC, A->elemSize);
   }
   else
   {
	  C = R2MatrixAllocate( hPhone,aR, bC, B->elemSize);
   }

   MULT_R2MATRIX(A, B, C, aR, aC, bC, float, float, float);

   return(C);
}

//! R2MatrixFree
void RFCal::GSM_Cal::GSM_V5::MatrixOperation::R2MatrixFree(HANDLE hPhone, RFCal::R2MATRIX *A)
{
   if (!A || !A->dataPtr || !A->numRows || !A->numCols)
   {
      ostringstream ostr;
      ostr.str("");
      ostr << "Error: attempting to free() an unallocated matrix. "  << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   char **a = A->dataPtr;

   for (unsigned int i=0; i< A->numRows; i++)
   {
      free(a[i]);
   }

   free((char*)a);
   a = 0;
   free((char*)A);

   return;
}

float RFCal::GSM_Cal::GSM_V5::Tx::DetermineDacAtDigGainPwr( const vector<struct opPwrRecord>& ops, float digGainPwrIn, unsigned int numDAC )
{

   // The DAC value associated with digGainIn and digGainPwrIn
   float maxDac = 0;

   // upper position for ops values for interpolation
   unsigned int upperPos = 0;

   // slope used for interpolation
   float slope = 0;

   // 1) make sure diGainPwrIn isn't > power at maxDac
   if ( digGainPwrIn >= ops[numDAC - 1].oppwr )
   {
      // return the largest DAC value
      // this will make the Least-Square Extrapolation max power estimation algorithm actually become the Direct Digital Gain Method
      maxDac = ops[numDAC - 1].amam;
      return maxDac;
   }

   // 2) Otherwise, loop through the DAC/Power list, and interpolate where necessary to get the maxDAC
   for(unsigned int lowerPos=numDAC-2; lowerPos!=0; --lowerPos)
   {
      upperPos = lowerPos + 1;

      // return DAC at upperPos
      if ( digGainPwrIn == ops[upperPos].oppwr )
      {
         maxDac = ops[upperPos].amam;
         break;
      }

      // return DAC at lowerPos
      if ( digGainPwrIn == ops[lowerPos].oppwr )
      {
         maxDac = ops[lowerPos].amam;
         break;
      }

      // interpolate between DACs at lowerPos and upperPos
      if ( (digGainPwrIn < ops[upperPos].oppwr) && (digGainPwrIn > ops[lowerPos].oppwr) )
      {
         slope = ( ops[upperPos].amam - ops[lowerPos].amam ) / ( ops[upperPos].oppwr - ops[lowerPos].oppwr );

         maxDac = slope*(digGainPwrIn - ops[lowerPos].oppwr) + ops[lowerPos].amam;
         break;
      }
   }

   return maxDac;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Determine_PreDist_Cal_RGI( HANDLE hPhone, GSM_CAL_Params &params, GSM_DA_Cal_Results_Channel& daCalData, double preDistCalPower, int m_curBandIdx, 
   int m_curChainID,  unsigned short  & m_predistCalRgi)
{
   GSM_DA_Cal_Sweep_Record preDistRangeEdgeDaCalRecord;

   unsigned short rgiMatch   = 0;
   unsigned short rgiAllChan = 0;
   double power             = -1000.0;   // initialize to unlikely value
   bool chanRgiReached      = false;

   // buffer to prevent very near preDistCalPower from causing this to fail.
   // For example, if preDistCalPower = 27 dBm, 26.98 dBm shouldn't cause it to fail.
   const double preDistPowerBuffer = 0.5;

   const unsigned short preDistCalPaRange = static_cast<unsigned short>( params.gsm_v4_Params.m_DPD_Params.calPaRange );

   const double powerThresh = preDistCalPower - preDistPowerBuffer;
   int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
   map<unsigned short,double>::iterator iter;
   for(unsigned char chanIdx = 0; chanIdx < params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx)
   {
      int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

      preDistRangeEdgeDaCalRecord = daCalData.channelSweepData[static_cast<unsigned short>(m_curChan)].edgeDaSweep[preDistCalPaRange];

      chanRgiReached = false;
      for( iter=preDistRangeEdgeDaCalRecord.rgiPwr.begin(); iter!=preDistRangeEdgeDaCalRecord.rgiPwr.end(); ++iter )
      {
         power = iter->second;
         if(power >= powerThresh)
         {
            rgiMatch = iter->first;
            chanRgiReached = true;
            break;
         }
      }
      if(!chanRgiReached)
      {
         ostringstream ostr;
         ostr << "Failed to acheive target predistortion power of " << preDistCalPower << " dBm.\n"
              << " Acheived " << power << " dBm. \n"
              << " Check if target predistortion power is too high or consider using DPD sweep for predistortion measurement.\n"
              << " Chain = " << m_curChainID << "\n"
              << " Band  = " << m_curBand << "\n"
              << " Chan  = " << m_curChan << "\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
      }

      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }

   m_predistCalRgi = rgiAllChan;

   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Apply_PCL_CorrectionTo_EdgeCharDA(  HANDLE hPhone, GSM_CAL_Params &params, int m_curBandIdx, GSM_DA_Cal_Results_Channel& daCalData, GSM_V4_PCL_Results_Band_Chan& gsmV4_PCL_Results, unsigned short numRGI )
{
   unsigned short preDistPaRange = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.calPaRange);

   // number of PCL indexes for 8PSK - there are less valid PCL indexes for 8PSK than GMSK, so reduce numEdgePCL to eliminate unnecessary searching
   unsigned short numEdgePCL = gsmV4_max8PSK_pclIdx_LB;  // default to low band setting

   int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];

   bool isLowBand = DetermineIfLowBand( m_curBand );
   if (!isLowBand)
   {
      numEdgePCL = gsmV4_max8PSK_pclIdx_HB;
   }

   // used for testing applying the PCL correction using entire range, or only the subset of PCL indexes
   //numEdgePCL = 16;

   // perform the PCl correction for 8PSK modulated data for PA range matching peDistPaRange
   for ( unsigned short chanIdx=0; chanIdx<params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx )
   {
      int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

      unsigned short shortChain = static_cast<unsigned short>(m_curChan);

      for (unsigned short rgiIdx=0; rgiIdx<numRGI; ++rgiIdx)
      {
         //------ Only applies correction to 8PSK
         double curDaCharPower = daCalData.channelSweepData[shortChain].edgeDaSweep[preDistPaRange].rgiPwr[rgiIdx];
         double curPowerCorr   = Determine_PCL_Pow_Error_For_DaChar( hPhone, curDaCharPower, numEdgePCL, gsmV4_PCL_Results.bandSweepData[m_curBand].edgePCLSweep[m_curChan].pclMeasPwr,
                                                                     gsmV4_PCL_Results.bandSweepData[m_curBand].edgePCLSweep[m_curChan].pclPwrDelta );
         // apply the correction
         daCalData.channelSweepData[shortChain].edgeDaSweep[preDistPaRange].rgiPwr[rgiIdx] = curDaCharPower + curPowerCorr;
      }
   }

   return true;
}
double RFCal::GSM_Cal::GSM_V5::Tx::Determine_PCL_Pow_Error_For_DaChar( HANDLE hPhone, double curDaPow, unsigned short numPCL, map<unsigned short, double>& pclPowMap,
                                              map<unsigned short, double>& pclPowError)
{
   unsigned short pclPowIdx  = 0;    // pclPowMap index
   unsigned short pclPowIdx2 = 0;    // second range PCL power index

   double interpError = 0.0;
   double interpSlope = 0.0;

   // check if curDaPow exceeds the range of PCL indexes
   if ( curDaPow <= pclPowMap[0] )
   {
      return pclPowError[0];
   }
   if ( curDaPow >= pclPowMap[numPCL-1] )
   {
      return pclPowError[numPCL-1];
   }

   // next, loop through all indexes except for the last one to find the appropriate PCL index
   for (pclPowIdx = 0; pclPowIdx < numPCL-1; ++pclPowIdx)
   {
      pclPowIdx2 = pclPowIdx + 1;

      // find the appropriate power index range
      // - check if equal to lower range power
      if ( pclPowMap[pclPowIdx] == curDaPow )
      {
         return pclPowError[pclPowIdx];
      }
      // - check if equal to upper range power
      else if ( pclPowMap[pclPowIdx2] == curDaPow )
      {
         return pclPowError[pclPowIdx2];
      }
      // - interpolate between lower and upper range if range contains the curDaPow
      else if ( (pclPowMap[pclPowIdx] < curDaPow) && (pclPowMap[pclPowIdx2] > curDaPow) )
      {
         interpSlope = ( pclPowError[pclPowIdx2] - pclPowError[pclPowIdx] ) / ( pclPowMap[pclPowIdx2] - pclPowMap[pclPowIdx] );
         interpError = interpSlope*(curDaPow - pclPowMap[pclPowIdx]) + pclPowError[pclPowIdx];
         return interpError;
      }
   }

   // At this point, all PCL indexes were exhausted, indicating curDaPow is > pclPowMap[pclPowIdx]
   return pclPowError[pclPowIdx];

}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::GSM_TX_Cx_NV_Extract_EDGE_DA_Cal_Data( HANDLE hPhone, GSM_CAL_Params &params,QMSL_GSM_Tx_Calibration_NV& calNV, unsigned long& updatedPayloadSize,
                                               GSM_DA_Cal_Results_Channel& daCalData, unsigned short& numRGI )
{
   // pointer to the static NV payload
   unsigned char* payloadPtr = calNV.nvRev0.aPayload;

   // NOTE:  orgPayloadSize does not accurately reflect the size of the static NV in terms of valid data.
   // Therefore, orgPayloadSize can not be relied upon at this stage for traversing the static NV payload
   // actualPayloadSize will be used to more accurately reflect the valid data in the static NV.
   unsigned long actualPayloadSize = 0;

   // indicates the variant data type in the DA Char static NV
   unsigned short curDataType_DaChar = 0;

   // used for determining PA states listed in the static NV payload
   list<unsigned short> paStateList;

   //-------------------------------- fill the RGI struct -----------------------------------//
   // NOTE: This assumes the RGI list should be the first element in the static NV,
   // and the RGI list is the same for each PMEAS table. Also, it assumes the RGI lists are consecutive. This is how the NV is written in QMSL.
   // If PMEAS tables and RGI tables are interdispersed, problems will arise.
   QMSL_GSM_RGI_List_Variant_Type tempRGI_List;

   // the RGI list should be the first element in the static NV
   curDataType_DaChar = payloadPtr[0] + (payloadPtr[1] << 8);

   stringstream ostr;
   if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_RGI_LIST)        // RGI List Table
   {
      memcpy_s( &tempRGI_List, sizeof(QMSL_GSM_RGI_List_Variant_Type), payloadPtr, sizeof(QMSL_GSM_RGI_List_Variant_Type) );

      payloadPtr        += sizeof(QMSL_GSM_RGI_List_Variant_Type);
      actualPayloadSize += sizeof(QMSL_GSM_RGI_List_Variant_Type);
   }
   else
   {
      ostr.str("");
      ostr << "GSM V4 DA Cal static NV did not contain RGI list. Please check static NV.\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
      return false;
   }

   // determine the number of RGIs
   numRGI = tempRGI_List.iNumValidRGI;

   // add the first PA element to the paStateList
   paStateList.push_back(tempRGI_List.iPaState);

   //-------------------------------- Determine the # of PA states in the static NV -----------------------------------//

   unsigned char rgiTableCntr = 1;   // initialize to 1 based on the above finding of the RGI list. There is 1 RGI list for each PA state and modulation
   unsigned short numPaStates = 0;
   const unsigned char numMod = 2;   // There must be 2 modulations in the static NV.

   while(1)
   {
      curDataType_DaChar = payloadPtr[0] + (payloadPtr[1] << 8);
      if (curDataType_DaChar != QMSL_RFNV_DATA_TYPE_ID_RGI_LIST)
      {
         break;
      }
      ++rgiTableCntr;
      paStateList.push_back( payloadPtr[3] );
      payloadPtr        += sizeof(QMSL_GSM_RGI_List_Variant_Type);
      actualPayloadSize += sizeof(QMSL_GSM_RGI_List_Variant_Type);
   }

   // compute the number of PA states
   numPaStates = rgiTableCntr / numMod;
   // only keep the unique PA states
   paStateList.unique();

   /*
   ostr.str("");
   ostr << "Number of PA states in static NV determined to be: " << numPaStates << endl;
   m_pUIM->PrintToDebug( ostr.str() );
   */

   // the static NV shoud contain only RGI and PMEAS tables
   unsigned char expectedNumPmeasTables = static_cast<unsigned char>( calNV.nvRev0.aVariantMarker.iNumElements ) - rgiTableCntr;
   unsigned char actualNumPmeasTables   = calNV.nvRev0.aNvHeader.iChannelListSize * static_cast<unsigned char>(numPaStates) * numMod;

   if (actualNumPmeasTables != expectedNumPmeasTables)
   {
      ostr.str("");
      ostr << "Warning: Static NV - The expected number of DA PMEAS tables does not equal the actual number of PMEAS tables. " <<
           "This indicates the static NV contains tables other than DA PMEAS." << endl;
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
   }

   // update the number of elements to reflect only the PMEAS and RGI tables
   calNV.nvRev0.aVariantMarker.iNumElements = actualNumPmeasTables + rgiTableCntr;

   //------------------------ fill the GSM_DA_Cal_Results_Channel ----------------------//
   // NOTE: chanTriggerLevel can be neglected in the GSM_DA_Cal_Results_Channel struct
   // payloadPtr is pointing to the next data table in the payload at this point
   QMSL_GSM_PMEAS_List_Variant_Type tempPMEAS_List;

   unsigned char  curMod     = 0;
   unsigned short curChan    = 0;
   unsigned short curPaState = 0;
   unsigned short curRGI     = 0;

   bool nonDACalItemsEncountered = false;

   unsigned char pMeasCntr = 0;        // keeps track of the # of PMEAS tables accessed

   const double powConFact   = 100.0;  // converts dBm*100 (as stored in static NV) to dBm

   // NOTE: the payloadPtr is currently pointing at the next table element - this table element should be a PMEAS table.
   // All of the RGI tables shoud have been passed.
   // This loop is broken out of once pMeasCntr == actualNumPmeasTables
   while( 1 )
   {
      curDataType_DaChar = payloadPtr[0] + (payloadPtr[1] << 8);

      if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_PMEAS_LIST) // check if a PMEAS table
      {
         // increment the PMEAS table counter
         ++pMeasCntr;
         // copy data to the PMEAS table
         memcpy_s( &tempPMEAS_List, sizeof(QMSL_GSM_PMEAS_List_Variant_Type), payloadPtr, sizeof(QMSL_GSM_PMEAS_List_Variant_Type) );

         // now, copy the PMEAS table to the GSM_DA_Cal_Results_Channel struct
         curMod     = tempPMEAS_List.iModulationType;  // 0 = GMSK, 1 = 8PSK
         curChan    = calNV.nvRev0.aNvHeader.iChannelList[ tempPMEAS_List.iChannelIndex ];
         curPaState = static_cast<unsigned short>( tempPMEAS_List.iPaState );

         // only fill the DA Cal struct if the curPaState is the calPaRange and the curMod is 8pSK
         if ( (curPaState == params.gsm_v4_Params.m_DPD_Params.calPaRange) && (curMod == QMSL_GSM_PwrErr_Per_PCL_8PSK_Type) )
         {
            for (unsigned char rgiIdx=0; rgiIdx<tempRGI_List.iNumValidRGI; ++rgiIdx)
            {
               curRGI = tempRGI_List.aRgiList[rgiIdx];
               daCalData.channelSweepData[curChan].edgeDaSweep[curPaState].rgiPwr[curRGI] = tempPMEAS_List.aPmeasList[rgiIdx] / powConFact;;
            }
         }
         // Continue looping in spite of having found all desired PMEAS tables.
         // This is so as to determine an accurate updatedPayloadSize.
         // increment the payload pointer
         payloadPtr        += sizeof(QMSL_GSM_PMEAS_List_Variant_Type);
         actualPayloadSize += sizeof(QMSL_GSM_PMEAS_List_Variant_Type);
      }
      else if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_AMAM_LIST )
      {
         nonDACalItemsEncountered = true;
         payloadPtr += sizeof(QMSL_GSM_DPD_AMAM_List_Variant_Type);
      }
      else if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_AMPM_LIST )
      {
         nonDACalItemsEncountered = true;
         payloadPtr += sizeof(QMSL_GSM_DPD_AMPM_List_Variant_Type);
      }
      else if (curDataType_DaChar == QMSL_RFNV_DATA_TYPE_ID_TX_PWR_ERR_LIST )
      {
         nonDACalItemsEncountered = true;
         payloadPtr += sizeof(QMSL_GSM_PwrErr_Per_PCL_List_Variant_Type);
      }
      // break out of the loop as soon as all PMEAS tables have been copied to GSM_DA_Cal_Results_Channel struct
      // actualPayloadSize >= updatedPayloadSize is added for loop breaking out safety
      if ( (pMeasCntr >= actualNumPmeasTables) || (actualPayloadSize >= updatedPayloadSize) )
      {
         break;
      }
   }

   // Update the payloadSize to reflect all of the RGI tables and PMEAS tables already in the static NV
   updatedPayloadSize = actualPayloadSize;


   return true;
}
//! Read_GSM_TxConsolidated_NV_Payload_And_Extract_Header
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Read_GSM_TxConsolidated_NV_Payload_And_Extract_Header( HANDLE hPhone, GSM_CAL_Params &params, int m_curBand, QMSL_GSM_Tx_Calibration_Consolidated_NV& readGsmNV, unsigned long& payloadSize )
{
   const unsigned int maxNvArrSize = GSM_TX_CAL_NV_REV0_MAX_PAYLOAD_SIZE;

   unsigned short nvID       = 0;
   unsigned int   nvSize     = maxNvArrSize;   // returned by QLIB call
   unsigned char  bResult = false;

   stringstream ostr;
   switch(m_curBand)
   {
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_GSM850_TX_CAL_DATA_I;
         break;
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_GSM900_TX_CAL_DATA_I;
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_GSM1800_TX_CAL_DATA_I;
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_GSM1900_TX_CAL_DATA_I;
         break;
      default:
         
         ostr.str("");
         ostr << "Unknown NV Mode" << m_curBand << ". Please remove non-GSM band from band list.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
         return false;
   }


   unsigned short iStatus = 0;
   unsigned char iStatus_2 = 0;

   QLIB_RFCAL_NV_Manager_BackupNVItems( hPhone, &nvID, &iStatus,1);
   
   vector < unsigned char > nvArr;
   nvArr.resize(nvSize);
    
   QLIB_RFCAL_NV_Manager_GetBackupNVItem( hPhone, nvID, &iStatus_2, & nvSize, &nvArr[0]);
   if(iStatus != 0 || iStatus_2 != 0)
   {
      ostr.str("");
      ostr << "GSM Consolidated Tx NV read from DUT failed for NV " << nvID << " - status: " << ". Check DUT status.\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
      return false;
   }
   
   memcpy_s( readGsmNV.nvRev1.aPayload, sizeof(readGsmNV.nvRev1.aPayload), &nvArr[0], nvSize);
   unsigned short* tempPtr = (unsigned short*)readGsmNV.nvRev1.aPayload;

   //version
   readGsmNV.nvRev1.aVariantMarker.iVersion = tempPtr[0];
   ++tempPtr;

   //number of elements
   readGsmNV.nvRev1.aVariantMarker.iNumElements = tempPtr[0];
   ++tempPtr;

   unsigned char* payLoadPtr = readGsmNV.nvRev1.aPayload;

   payloadSize = static_cast<unsigned long>(nvSize - 4);
   memcpy_s( payLoadPtr, sizeof(readGsmNV.nvRev1.aPayload), tempPtr, payloadSize);

   return true;

}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::RunPredistSweep_And_Process_Tx_NV( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_PCL_Results_Chain& gsmV4_PCL_Results, GSM_V4_DPD_Results_Chain& gsmV4_DPD_Results,
                                           GSM_V4_Tx_Cx_NV_Results& tx_NV_Results, GSM_V4_Tx_Consolidated_NV_Results& consolidated_Tx_NV_Results, GSM_CAL_Results & results)
{
  
   // cache the NV items for NV write later
   unsigned char cacheNVItems = (params.gsm_v4_Params.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

   static PreDist_Capture_Record_Channel calibratedPreDistData;
   results.calibratedPreDistData = & calibratedPreDistData;
  // CGSM_Linear_Tx_Cal_Alg::m_useFTMCmd      = true;   // has to be true
   params.gsm_v4_Params.m_DA_Params.usePclErrorNV = true;   // make sure this is true

   const double origMinLimitOnMaxPower = params.preDistCalParams.m_minLimitOnMaxPower;

   QMSL_GSM_Tx_Calibration_NV              calNV;               // chain enumerated calNV struct for a single band/device combination
   QMSL_GSM_Tx_Calibration_Consolidated_NV consolidatedCalNV; // consolidatdcalNV struct for a single band/device combination
   QMSL_GSM_V4_PCL_Cal_Result              calPCL_ToQMSL;       // QMSL struct for result passing PCL cal data to QMSL
   QMSL_GSM_EnhLinTx_Cal_Data              calDPD_ToQMSL;
   QMSL_GSM_EnhLinTx_Cal_Control           calControl_ToQMSL;
   memset( &calControl_ToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Control) );
   memset( &calDPD_ToQMSL, 0, sizeof(QMSL_GSM_EnhLinTx_Cal_Data) );

   // Fill the QMSL structs with values that are indpendent of band/chain
   for (unsigned int chanIdx=0; chanIdx<params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx)
   {
      calDPD_ToQMSL.iFreqMapping[chanIdx] = static_cast<QMSL_GSM_Polar_Cal_Frequency>( params.preDistCalParamsV5.m_nvMappings[chanIdx] );
   }
   calDPD_ToQMSL.dDacScale    = static_cast<float>(params.gsm_v4_Params.m_DPD_Params.dacScale);
   calDPD_ToQMSL.iNumChannels = params.gsm_v4_Params.m_bcc.m_numChan;

   calControl_ToQMSL.bSlopeCorrAlignLow     = params.gsm_v4_Params.m_DPD_Params.doSlopeCorrectionAlignLow;
   calControl_ToQMSL.bDoPowerAdaptation     = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.doPowerAdaptation);
   calControl_ToQMSL.bDoSlopeCorrection     = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.doSlopeCorrection);
   calControl_ToQMSL.bDoPiecewiseAdaptation = static_cast<unsigned short>(params.gsm_v4_Params.m_DPD_Params.doPiecewiseAdapt);
   calControl_ToQMSL.iAlgType               = 1;   // This will always be 1

   bool isConsolidatedNV = (params.setRFMPathVersion != 0) ? true : false;

   // ----------------- CHAIN ID Loop ------------------- //
   for (unsigned char curChainIdx=0; curChainIdx<params.gsm_v4_Params.m_bcc.m_numTxChains; ++curChainIdx)
   {
      int m_curChainID = params.gsm_v4_Params.m_bcc.m_txChainList[curChainIdx];

      // ----------------- BAND Loop ------------------- //
      for (unsigned int m_curBandIdx=0; m_curBandIdx<params.gsm_v4_Params.m_bcc.m_numBands; ++m_curBandIdx)
      {
         //---------- 1) Preliminary band setup and callbox/UUT configuration
         int m_curBand                  = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
         params.preDistCalParamsV5.m_nv_mode = static_cast<nv_mode_id_type>(m_curBand);

         FTM_RF_Mode_Enum curRfMode     = static_cast<FTM_RF_Mode_Enum>(m_curBand);
		 BAND_CLASS_ENUM m_band   = RFCal::Helper::Convert::ConverGSMBand_To_BandClassEnum( m_curBand );

		 RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, static_cast<unsigned short> (m_curChainID), FTM_MODE_ID_GSM);
		 unsigned char status = QLIB_FTM_SET_MODE( hPhone, static_cast<short>(curRfMode) );

      
       //  CRFCalCleanCommonAlg::Set_Band( CRFCalCleanCommonAlg::m_band, RFCAL_ALG_TYPE_GSM_LINEAR_TX_CAL );

         // update the channel for running predistortion
         params.preDistCalParamsV5.m_channels[0] = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][gsmV4_REF_CHAN_IDX];

         //---------- 2) Predistortion RGI determination and Power Limits
         params.preDistCalParamsV5.m_minLimitOnMaxPower = origMinLimitOnMaxPower + params.preDistCalParamsV5.m_calRGIPower;

         // determine the predistortion RGI power needed for this chain/band combo
         params.preDistCalParamsV5.m_calRGIPower = params.gsm_v4_Params.m_DPD_Params.calRGI_PowerLowBand;  // default to low-band power
         if ( !DetermineIfLowBand( m_curBand ) )
         {
            params.preDistCalParamsV5.m_calRGIPower = params.gsm_v4_Params.m_DPD_Params.calRGI_PowerHighBand;
         }

         // update m_predistCalRgi
         Determine_PreDist_Cal_RGI_from_DPD( hPhone, params, m_curBandIdx, gsmV4_DPD_Results.chainData[m_curChainID].band_Chan_sweepResults[m_curBand],
                                             params.preDistCalParamsV5.m_calRGIPower, 0.0 );

         // update the RGI for running predistortion
         params.preDistCalParamsV5.m_calRgiWithNoDaCal[0] = params.gsm_v4_Params.m_globals.m_predistCalRgi;
         calDPD_ToQMSL.iCalRgi                   = params.gsm_v4_Params.m_globals.m_predistCalRgi;
         calibratedPreDistData.calRgi            = params.gsm_v4_Params.m_globals.m_predistCalRgi;

         //---------- 3) Run the Predistortion Sweep
         // run the predistortion sweep for the current RGI/chan/band/chain
         ZeroMemory( &calibratedPreDistData, sizeof( PreDist_Capture_Record_Channel ) );

		 if (!RFCal::GSM_Cal::GSM_V5::Tx::Do_EDGE_PreDist_Cal( hPhone, params, params.gsm_v4_Params.m_globals.m_predistCalRgi, m_curBandIdx,calibratedPreDistData ))
		 {
			 return false;
		 }

         //---------- 4) Process the calibratedPreDistData in order to get the NV data.
         QMSL_GSM_PreDist_Cal_NV_Chan_struct predistCalNV;   // contains the AMAM/AMPM/MaxPower data and more
         Process_EDGE_PreDist_Cal_NV( hPhone, params, calibratedPreDistData, predistCalNV ,m_curBand );

         //---------- 5) Prepare the predistortion data for QLIB_RFCal_GSM_V4_Cal_Tx_Generate_NV
         PackPredistCalDataForNVProcessing( hPhone, params ,predistCalNV, calDPD_ToQMSL, calControl_ToQMSL, gsmV4_DPD_Results.chainData[m_curChainID],m_curBandIdx  );

         //---------- 6) Preliminary Static NV Processing
         // reset the orgPayloadSize
         unsigned long orgPayloadSize = 0;

         // Read in the calNV from the phone. Will append PCL data and predisortion data to the NV and rewrite.
         if(!isConsolidatedNV)
         {
            //memset( &calNV, 0, sizeof(QMSL_GSM_Tx_Calibration_NV) );    // initialize to 0
            //Read_GSM_TxCx_NV_Payload( hPhone, params, calNV, orgPayloadSize, m_curBand,m_curChainID);          // read the chain enumerated Tx calNV payload from the phone
            //GSM_TX_Cx_NV_Extract_Header( hPhone, params, &calNV, orgPayloadSize );
         }
         else
         {
            memset( &consolidatedCalNV, 0, sizeof(QMSL_GSM_Tx_Calibration_Consolidated_NV) );           // initialize to 0
            Read_GSM_TxConsolidated_NV_Payload_And_Extract_Header( hPhone, params, m_curBand, consolidatedCalNV, orgPayloadSize);  // read the consolidated Tx calNV payload from the phone and extract header
         }

         //---------- 7) PCL Error Processing
         memset( &calPCL_ToQMSL, 0, sizeof(QMSL_GSM_V4_PCL_Cal_Result) );

         // copy data from gsmV4_CalParams and gsmV4_PCL_Results to calPCL_ToQMSL
         // ---- channels and PCLs
		 calPCL_ToQMSL.iNumChannels    = params.gsm_v4_Params.m_bcc.m_numChan;
		 calPCL_ToQMSL.iNumMeasuredPCL = params.gsm_v4_Params.m_DA_Params.numPCLs;

		 for ( unsigned short chanIdx=0; chanIdx<params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx )
         {
			 int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

            calPCL_ToQMSL.iChannel[chanIdx] = static_cast<unsigned short>(m_curChan);

            // ---- PCL indexes and measured power for each index
			for ( unsigned short pclIdx=0; pclIdx<params.gsm_v4_Params.m_DA_Params.numPCLs; ++pclIdx )
            {
               // Assuming same EDGE and GSM PCL list
               // PCL indexes
               calPCL_ToQMSL.aiGSM_pclIdx[chanIdx][pclIdx]  = pclIdx;
               calPCL_ToQMSL.aiEDGE_pclIdx[chanIdx][pclIdx] = pclIdx;
               // PCL Delta values
               calPCL_ToQMSL.aiGSMTxPowerMeasErr[chanIdx][pclIdx]  = gsmV4_PCL_Results.chainData[m_curChainID].bandSweepData[m_curBand].gsmPCLSweep[m_curChan].pclPwrDelta[pclIdx];
               calPCL_ToQMSL.aiEDGETxPowerMeasErr[chanIdx][pclIdx] = gsmV4_PCL_Results.chainData[m_curChainID].bandSweepData[m_curBand].edgePCLSweep[m_curChan].pclPwrDelta[pclIdx];
            }
         }

         //---------- 8) Tx NV GENERATION
		 DetermineTxNvContainer( hPhone, params, params.gsm_v4_Params.m_bcc.m_get_nv_container_from_uut_tx );
		 calControl_ToQMSL.iTxNvContainer = params.m_txNvContainer;
         calPCL_ToQMSL.iTxNvContainer     = params.m_txNvContainer;

         if(!isConsolidatedNV)
         {
            QLIB_RFCal_GSM_V4_Cal_Tx_Generate_NV(
               hPhone,
               static_cast<unsigned short>(m_curBand),
               static_cast<unsigned char>(m_curChainID),
               (void*)& calPCL_ToQMSL,
               (void*)& calControl_ToQMSL,
               (void*)& calDPD_ToQMSL,
               (void*)& calNV,
               orgPayloadSize,
               cacheNVItems );

            // push the calNV onto the multi-band NV map
            tx_NV_Results.chainNV[m_curChainID][m_curBand] = calNV;

            //Inspect_GSM_Tx_NV_PredistData_Validity( static_cast<unsigned short>(m_curChainID),
            //                                        static_cast<FTM_RF_Mode_Enum>(m_curBand), calNV.iHasValidData );

			if ( !calNV.iHasValidData)
			{
				stringstream ostr;
				ostr.str("");
				ostr << "GSM_AMAM_Monotonicity_Check() failure for -\n"
					<< "band: "          << static_cast<FTM_RF_Mode_Enum>(m_curBand)    << "\n"
					<< "Tx chain: "      << static_cast<unsigned short>(m_curChainID) << "\n"
					<< "  Typically, this indicates the predistortion waveform is lacking in dynamic range.\n"
					<< "  Enable \"Log PreDistortion Calibration Data\" to inspect the predistortion waveform.\n"
					<< __FUNCTION__ << " " << __LINE__;

			}
         }
         else
         {
            CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
            RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, 65535, 0, 0, GSM_TX_BASED_QUERY);

			QLIB_RFCal_GSM_V4_Cal_Tx_Generate_Consolidated_NV(
               hPhone,
               static_cast<unsigned short>(m_curBand),
			   static_cast<unsigned char>(params.m_txAntPath),
			   static_cast<unsigned char>(params.chainID),
               static_cast<unsigned char>(sigPaths.tx.rfm_dev),
               (void*)& calPCL_ToQMSL,
               (void*)& calControl_ToQMSL,
               (void*)& calDPD_ToQMSL,
               (void*)& consolidatedCalNV,
               orgPayloadSize,
               cacheNVItems );
			 
            // push the calNV onto the multi-band NV map
            consolidated_Tx_NV_Results.data[m_curBand].data[sigPaths.tx.antenna_port].data[sigPaths.tx.signal_path].data[sigPaths.tx.rfm_dev] = consolidatedCalNV;

            Inspect_GSM_Tx_Consolidated_NV_PredistData_Validity( hPhone, params, static_cast<FTM_RF_Mode_Enum>(m_curBand), sigPaths.tx.antenna_port, sigPaths.tx.signal_path,
                                                                 sigPaths.tx.rfm_dev, consolidatedCalNV.iHasValidData );
         }

      }  // END BAND LOOP
   }     // END CHAIN LOOP

   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::PackPredistCalDataForNVProcessing( HANDLE hPhone, GSM_CAL_Params &params, QMSL_GSM_PreDist_Cal_NV_Chan_struct& predistCalNV, QMSL_GSM_EnhLinTx_Cal_Data& calDPD_ToQMSL,
	QMSL_GSM_EnhLinTx_Cal_Control& calControl_ToQMSL, GSM_V4_DPD_Results_Band_Chan& gsm_v4_dpdResults, int m_curBandIdx )
{
	

   // convert the predistCalNV data to the format necessary for char-DPD adaptation
   // Pack_GSM_DPD_For_QMSL variant just to get the predist data ready for later.
   // fill
   // 1) Determine the target output powers
   // 2) The aCharCurves and aCharDigGain will all be from the measured predistortion channel
   //    - the same index from, i.e. predistCalNV.aAMAM_AMPM_NV[gsmV4_REF_CHAN_IDX] will be copied over to each calDPD_ToQMSL.aCharCurves[chanIdx]
   // 3) The aDutDigGain will be the values measured for each channel
   float AMAM_Max_Power       = predistCalNV.aDyanmicRange.iMaxPower / 100.0f;
   unsigned short dacIdx      = 0;
   int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
   static const float dynRangeFactor = 128.0f/16383.0f;

   vector<float> targetOutputPowers(QMSL_NV_MANAGER_EDGE_DPD_TABLE_SIZE);

   //---------------------- Determine the targetOutputPowers -----------------------//
   for (unsigned short loopIdx=0; loopIdx<QMSL_NV_MANAGER_EDGE_DPD_TABLE_SIZE; ++loopIdx)
   {
      dacIdx                      = loopIdx + 1;
      targetOutputPowers[loopIdx] = AMAM_Max_Power + 20.0f*log10( static_cast<float>(dacIdx)*dynRangeFactor );
   }

   //----------------------------------- Main Channel Loop -----------------------------------//
   for ( unsigned int chanIdx = 0; chanIdx < params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx )
   {
	   int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

      //---------- 1) Fill every aCharCurve with the measured predistortion data from gsmV4_REF_CHAN_IDX
      for (unsigned short loopIdx=0; loopIdx<QMSL_NV_MANAGER_EDGE_DPD_TABLE_SIZE; ++loopIdx)
      {
         if( loopIdx < QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE )
         {
            calDPD_ToQMSL.aCharCurves[chanIdx].dCharAmpm[loopIdx] = predistCalNV.aAMAM_AMPM_NV[gsmV4_REF_CHAN_IDX].aAMPM_NV[0].AMPM[loopIdx];
            calDPD_ToQMSL.aCharCurves[chanIdx].dCharDac[loopIdx]  = predistCalNV.aAMAM_AMPM_NV[gsmV4_REF_CHAN_IDX].aAMAM_NV[0].AMAM[loopIdx];
         }
         else
         {
            unsigned short amamTable2Idx = loopIdx - QMSL_NV_MANAGER_POLAR_CAL_AMAM_TABLE_SIZE ;
            calDPD_ToQMSL.aCharCurves[chanIdx].dCharAmpm[loopIdx] = predistCalNV.aAMAM_AMPM_NV[gsmV4_REF_CHAN_IDX].aAMPM_NV[1].AMPM[amamTable2Idx];
            calDPD_ToQMSL.aCharCurves[chanIdx].dCharDac[loopIdx]  = predistCalNV.aAMAM_AMPM_NV[gsmV4_REF_CHAN_IDX].aAMAM_NV[1].AMAM[amamTable2Idx];
         }

         calDPD_ToQMSL.aCharCurves[chanIdx].dCharPower[loopIdx] = targetOutputPowers[loopIdx];
      }

      //---------- 2) Copy over measured DUTDigGain data
      GSM_V4_DPD_Sweep_Record* channelSweepResults = &gsm_v4_dpdResults.band_Chan_sweepResults[m_curBand][m_curChan];

      map<unsigned short, GSM_DA_Cal_DigGain_Record>::iterator digGainIter = channelSweepResults->digGainSweep.begin();

      calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries = 0;
      unsigned int digGainTablePos                   = 0;
	  bool useAllPoints                              = params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].empty();

      for (digGainIter= channelSweepResults->digGainSweep.begin(); digGainIter!=channelSweepResults->digGainSweep.end(); ++digGainIter )
      {
         if (digGainIter->second.digGainPwr.find( params.gsm_v4_Params.m_globals.m_predistCalRgi) != digGainIter->second.digGainPwr.end())
         {
            unsigned short curDigGain = static_cast<unsigned short>( digGainIter->first );

            // if the current digGain point is in the slope correction list, then use it.
			bool usePoint = find( params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].begin(),
                                  params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].end(),
                                  curDigGain )
                            != params.gsm_v4_Params.m_DPD_Params.slopeCorrectionDigGainList[m_curBandIdx].end();

            calDPD_ToQMSL.aDutDigGain[chanIdx].iDigGain[digGainTablePos] = curDigGain;
            calDPD_ToQMSL.aDutDigGain[chanIdx].dPower[digGainTablePos]   = static_cast<float>( digGainIter->second.digGainPwr[params.gsm_v4_Params.m_globals.m_predistCalRgi] );
            calControl_ToQMSL.bSlopeCorrUseDutPoint[digGainTablePos]     = usePoint || useAllPoints;
            ++calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries;
            ++digGainTablePos;
         }
      }
   }  //--------------------------- END Main CHANNEL LOOP ---------------------------------//

   //---------- 3) Copy over characterized DigGain data
   // Note: The characterized digGain data is just the digGain data from the gsmV4_REF_CHAN_IDX
   for ( unsigned int chanIdx = 0; chanIdx < params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx )
   {
      // copy over the charDigGAin
      calDPD_ToQMSL.aCharDigGain[chanIdx].iNumEntries = calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries;

      for ( unsigned short digGainIdx=0; digGainIdx < calDPD_ToQMSL.aDutDigGain[chanIdx].iNumEntries; ++digGainIdx )
      {
         calControl_ToQMSL.bSlopeCorrUseCharPoint[digGainIdx]     = calControl_ToQMSL.bSlopeCorrUseDutPoint[digGainIdx];
         calDPD_ToQMSL.aCharDigGain[chanIdx].iDigGain[digGainIdx] = calDPD_ToQMSL.aDutDigGain[gsmV4_REF_CHAN_IDX].iDigGain[digGainIdx];
         calDPD_ToQMSL.aCharDigGain[chanIdx].dPower[digGainIdx]   = calDPD_ToQMSL.aDutDigGain[gsmV4_REF_CHAN_IDX].dPower[digGainIdx];
      }
   }

   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Process_EDGE_PreDist_Cal_NV( HANDLE hPhone, GSM_CAL_Params &params, const PreDist_Capture_Record_Channel& calibratedPreDistData, QMSL_GSM_PreDist_Cal_NV_Chan_struct& calNV , int m_curBand)
{

	//Populate QMSL struct to pass to QLIB
   static QMSL_GSM_PreDist_Cal_Result calResult;
   unsigned char queue_nv_indicator = 0x0;

   Process_EDGE_PreDist_Cal_NV_Intermediate( hPhone, params, calibratedPreDistData, calResult, queue_nv_indicator );

   if(params.preDistCalParamsV5.m_swappedOrderEdgeFirst)
   {
      QLIB_RFCAL_GSM_Tx_Linear_Process_Swapped_PreDist_Cal_Results(  hPhone,
                                                                     static_cast<unsigned short>(params.preDistCalParamsV5.m_nv_mode),(void*)& calResult, (void*)& calNV,
                                                                     queue_nv_indicator);
   }
   else
   {
      QLIB_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results( hPhone,
                                                            static_cast<unsigned short>(params.preDistCalParamsV5.m_nv_mode),(void*)& calResult, (void*)& calNV,
                                                            queue_nv_indicator);
   }

   if ( !calNV.iHasValidData )
   {
	   stringstream ostr;
	   ostr.str("");
	   ostr << "GSM_AMAM_Monotonicity_Check() failure for -\n"
		   << "band: "          << m_curBand    << "\n"
		   << "Tx chain: "      << params.chainID << "\n"
		   << "  Typically, this indicates the predistortion waveform is lacking in dynamic range.\n"
		   << "  Enable \"Log PreDistortion Calibration Data\" to inspect the predistortion waveform.\n"
		   << __FUNCTION__ << " " << __LINE__;
	   return false;
   }
   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Process_EDGE_PreDist_Cal_NV_Intermediate( HANDLE hPhone, GSM_CAL_Params &params, const PreDist_Capture_Record_Channel& calibratedPreDistData,
	QMSL_GSM_PreDist_Cal_Result& calResult, unsigned char& queue_nv_indicator )
{

	CEDGE_PreDist_Cal_Data preDistCalParamsV5 = params.preDistCalParamsV5;
	// Populate QMSL struct to pass to QLIB
	memcpy_s(calResult.adTxAmpl[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[0].ampldArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
	memcpy_s(calResult.adTxAmpl[1], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[1].ampldArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
	memcpy_s(calResult.adTxAmpl[2], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[2].ampldArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
	memcpy_s(calResult.adTxPhaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[0].phaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
	memcpy_s(calResult.adTxPhaseArr[1], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[1].phaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
	memcpy_s(calResult.adTxPhaseArr[2], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[2].phaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
	float symbolrate           = 270833.0;
	calResult.iCalRgi          = (unsigned short)calibratedPreDistData.calRgi;
	calResult.iDcSamples       = (long)((preDistCalParamsV5.m_preDcDuration/4)*(calibratedPreDistData.samplingRateHz/symbolrate));
	calResult.iEdgeSamples     = (long)((preDistCalParamsV5.m_edgeDuration/4)*(calibratedPreDistData.samplingRateHz/symbolrate));
	calResult.iFreqMapping[0]                  = (QMSL_GSM_Polar_Cal_Frequency)preDistCalParamsV5.m_nvMappings[0];
	calResult.iFreqMapping[1]                  = (QMSL_GSM_Polar_Cal_Frequency)preDistCalParamsV5.m_nvMappings[1];
	calResult.iFreqMapping[2]                  = (QMSL_GSM_Polar_Cal_Frequency)preDistCalParamsV5.m_nvMappings[2];
	calResult.iNoiseSamples                    = 500;       // Todo: Get this from configured on the call box.
	calResult.iNumChannels                     = preDistCalParamsV5.m_channels.size();
	calResult.iNumPredistortionWaveformSamples = (unsigned short)calibratedPreDistData.numSamples;
	calResult.iSamplingRateHz                  = calibratedPreDistData.samplingRateHz;
	calResult.iDCTransientPercent              = (float)preDistCalParamsV5.m_dcSkipPercentage;
	calResult.iEDGETransientSymbols            = (unsigned short)preDistCalParamsV5.m_edgeSkipSymbols;


	 //DIG_GAIN_UNITY       = aGSM_PreDist_Cal->dDigGainUnity;
  //    RAMP_UNITY           = aGSM_PreDist_Cal->dRampUnity;
  //    EXTENSION_FLOOR      = aGSM_PreDist_Cal->dExtensionFloor;
	//Modem Settings
	if(preDistCalParamsV5.m_overRideModemConsts)
	{
		calResult.iOverRideModemConstants = 1;
		calResult.dDigGainUnity    = (float)preDistCalParamsV5.m_digGainUnity;
		calResult.dRampUnity       = (float)preDistCalParamsV5.m_rampUnity;
		calResult.dDacScale        = (float)preDistCalParamsV5.m_dacScale;
		calResult.dExtensionFloor  = (float)preDistCalParamsV5.m_extensionFloor;
	}
	//This case only comes into play when two different targets are run without QSPR closing.
	//Need to handle it anyways.
	else
	{
		calResult.iOverRideModemConstants = 0;
	}

	if(preDistCalParamsV5.m_overRideSmoothingDefaults)
	{
		calResult.iOverrideSmooothingDefaults = 1;
		calResult.iEnableAmAmSmoothing = (preDistCalParamsV5.m_enableAmAmSmoothing)?1:0;
		calResult.dAmAmKnot = (float)preDistCalParamsV5.m_amamKnotSize;
		calResult.dAmPmKnot = (float)preDistCalParamsV5.m_ampmKnotSize;
		//m_pUIM->PrintToDebug("Smoothing Override Enabled");
		/*
		ostringstream temp;
		temp << "Overides. AMAM Smoothing :" << ((preDistCalParams.m_enableAmAmSmoothing)?"Enabled":"Disabled")
		<< ", AMAM Knot :" << calResult.dAmAmKnot
		<< ", AMPM Knot :" << calResult.dAmPmKnot;
		m_pUIM->PrintToDebug(temp.str());
		*/
	}

	calResult.iEDGETxGainParam = GetEDGELinearTxGain( hPhone, params, preDistCalParamsV5.m_nv_mode);
	calResult.iEDGETxCalScale = (unsigned short)calibratedPreDistData.calScaleSetting;

	queue_nv_indicator = (preDistCalParamsV5.m_nv_action & RFCAL_NV_ACTION_QUEUE_NV) ? 1 : 0;

	return true;
}	 


short RFCal::GSM_Cal::GSM_V5::Tx::GetEDGELinearTxGain( HANDLE hPhone, GSM_CAL_Params & params, unsigned int nvMode )
{
	stringstream ostr;
   short envGain             = 0;
   unsigned short nvID       = 0;
   unsigned char nvArr[128];
   unsigned short nvSize     = 128;
   unsigned short readStatus = 0;
   bool bResult              = true;

   switch(nvMode)
   {
      case NV_MODE_GSM_EGSM:
         nvID = NV_EDGE_LINEAR_TX_GAIN_PARAM_I;
         break;
      case NV_MODE_GSM_850:
         nvID = NV_EDGE_850_LINEAR_TX_GAIN_PARAM_I;
         break;
      case NV_MODE_GSM_DCS:
         nvID = NV_EDGE_1800_LINEAR_TX_GAIN_PARAM_I;
         break;
      case NV_MODE_GSM_1900:
         nvID = NV_EDGE_1900_LINEAR_TX_GAIN_PARAM_I;
         break;
   }
 /*  
   if(false == m_useFTMCmd)  // Read NVs directly
   {
      QLIB_DIAG_NV_READ_F( hPhone, nvID, nvArr, static_cast<int>(nvSize), &readStatus);
      envGain = *(short *)(&nvArr[0] );
   }
   else */ // use the FTM API Readback Method to read NVs
   {
	   // sets the mode ID - default to C0
	   // function handles properly setting FTM_MODE_ID_GSM_EXTENDED_CX

	   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone,params.chainID, FTM_MODE_ID_GSM_EXTENDED_C);

	   unsigned short retType = 2;   // nvArr[0] points to EDGE envGain

	   bResult = ( QLIB_FTM_GSM_API_ReadEnvGainNV( hPhone, nvID, nvArr, &nvSize, &readStatus, retType ) != 0 );

	   // combine the 2 bytes for envGain. They are in LSByte order
	   envGain = nvArr[0] + (nvArr[1] << 8);
   }

   // verify that the envGain is a reasonable value
   // A bad result for envGain can be returned despite bResult and readStatus indicating a successful read.
   const short maxEnvGain = 100; // 93 is the general maximum value

   if ( readStatus != 0 || !bResult || (envGain > maxEnvGain) )
   {
      ostr.str("");
      ostr << "NV Read of EDGE Digital Gain Failed. " << __FUNCTION__;
	  RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
	  return false;
     
   }

   return envGain;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Do_EDGE_PreDist_Cal( HANDLE hPhone, GSM_CAL_Params &params, unsigned long calRgi, unsigned int m_curBandIdx, PreDist_Capture_Record_Channel& calibratedData  )
{
	const CEDGE_PreDist_Cal_Data & preDistCalParamsV5 = params.preDistCalParamsV5;
	unsigned short m_chainID = params.chainID;
   vector<double> PreDistSweepPwrs;

   ostringstream ostr;

   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   if ( NULL == pMgr)
   {
	   return false;
   }

   // set the chain to correct value
//   m_pLM->UUT_Set_Rx_Path_Only( m_chainID );
   int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
   pMgr->Set_Tech(TECHNOLOGY_GSM);
   pMgr->Set_Band(m_curBand);  
   unsigned short chanLoopVar = 0;
   for (vector<int>::const_iterator it = preDistCalParamsV5.m_channels.begin(); it != preDistCalParamsV5.m_channels.end(); it++) 
   {
	   calibratedData.channelNumbers[chanLoopVar] = (*it);

	   pMgr->Set_DL_Channel(static_cast<int>(*it), 0);

	   //! UUT Set Channel
	   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone,m_chainID,FTM_MODE_ID_GSM);
	   QLIB_FTM_SET_CHAN(hPhone, *it);
      //! Set Tx Off
      QLIB_FTM_SET_TX_OFF(hPhone);
      //! Set PA Range
	  unsigned char status = QLIB_FTM_SET_GSM_LINEAR_PA_RANGE(hPhone, 0, static_cast<unsigned short>(preDistCalParamsV5.m_paRangeCalibrated));

      //! Setup TE
	  pMgr->Set_UL_Expected_Level(preDistCalParamsV5.m_inputLevel);

      ///UUT_Send_PreDistortion_Sweep;
      unsigned long totalDuration = preDistCalParamsV5.m_edgeDuration+preDistCalParamsV5.m_preDcDuration;
	  pMgr->Config_PreDist_Capture( totalDuration, calibratedData.numSamples, calibratedData.samplingRateHz );

      //Try three times
      int retryLoop = 3;

	  while(retryLoop > 0)
	  {
		  pMgr->Start_PreDist_Capture();

		  //! Sleep for measurement delay
		  Sleep((dword)preDistCalParamsV5.m_measStartDelay);

		  unsigned char status;

		  RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, params.chainID, FTM_MODE_ID_GSM_EXTENDED_C);


		  if(!preDistCalParamsV5.m_swappedOrderEdgeFirst)
			  status = QLIB_FTM_TX_CFG2_AMAM_SWEEP( hPhone, preDistCalParamsV5.m_preDcDuration, preDistCalParamsV5.m_edgeDuration, calRgi, &calibratedData.calScaleSetting );
		  else
			  status = QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2( hPhone, preDistCalParamsV5.m_preDcDuration, preDistCalParamsV5.m_edgeDuration, calRgi, 0,  &calibratedData.calScaleSetting );

		  RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, params.chainID, FTM_MODE_ID_GSM_EXTENDED_C);

		  ostr.str("");
		  ostr << "Sending PreDist IQ:" << " chan " << (*it);
		  RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str());

		  unsigned long returnedSamples = 0;
		  //         m_pMM->TE_Get_PreDist_Capture_Data(returnedSamples,calibratedData.captureRecords[chanLoopVar].phaseArr,calibratedData.captureRecords[chanLoopVar].ampldArr);
		  pMgr->Get_PreDist_Capture_Data(returnedSamples,calibratedData.captureRecords[chanLoopVar].phaseArr,calibratedData.captureRecords[chanLoopVar].ampldArr);

		  if(calibratedData.numSamples == returnedSamples)
		  {
			  //Sweep Succesful
			  break;
		  }
		  else
		  {
			  ostr.str("");
			  ostr << "Expected : " << calibratedData.numSamples << " but received " << returnedSamples << ". Retrying " << (--retryLoop) << "times ...";
			  RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str());
			  if(retryLoop == 0)
			  {
				  ostr.str("Quitting ... ");
				  RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str());

				  ostr.str("");
				  ostr << "Number of samples received from TE: " << returnedSamples;
				  RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str());
				  return false;
			  }
		  }
	  }
   }

   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Read_GSM_TxCx_NV_Payload( HANDLE hPhone, GSM_CAL_Params &params, QMSL_GSM_Tx_Calibration_NV& readGsmNV, unsigned long& payloadSize, int m_curBand, int m_curChainID )
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	const unsigned int maxNvArrSize = GSM_TX_CAL_NV_REV0_MAX_PAYLOAD_SIZE;

	unsigned short nvID       = 0;
	unsigned short nvSize     = maxNvArrSize;   // returned by QLIB call
	unsigned short readStatus = 0;              // returned by QLIB call
	unsigned char nvContainer = 0;
	unsigned short nvSelector = 0;

	unsigned char bResult = false;

	unsigned char nvArr[maxNvArrSize]  = {0x00};

   if (p.m_bcc.m_get_nv_container_from_uut_tx)
   {
      GSM_Get_NV_Container( hPhone, params, static_cast<FTM_RF_Mode_Enum>(m_curBand), static_cast<unsigned char>(m_curChainID), 1, nvContainer );
      nvSelector = static_cast<unsigned short>(nvContainer);
   }
   else
      nvSelector = params.chainID;

   if (nvSelector != 0 && nvSelector != 2 && nvSelector != 4)
   {
      stringstream ostr;
      ostr.str("");
      ostr << "Unsupported NV Container number: " << nvSelector << ". Currently supported NV Containers are 0,2,4. Please check tree params."
           << "If support for new NV Container is required, please contact the devs."
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
	  RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
	  return false;
   }

   switch(m_curBand)
   {
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_C0_GSM850_TX_CAL_DATA_I;
         if (nvSelector == 2)
         {
            nvID = RFNV_GSM_C2_GSM850_TX_CAL_DATA_I;
         }
         else if (nvSelector == 4)
         {
            nvID = RFNV_GSM_C4_GSM850_TX_CAL_DATA_I;
         }

         break;
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_C0_GSM900_TX_CAL_DATA_I;
         if (nvSelector == 2)
         {
            nvID = RFNV_GSM_C2_GSM900_TX_CAL_DATA_I;
         }
         else if (nvSelector == 4)
         {
            nvID = RFNV_GSM_C4_GSM900_TX_CAL_DATA_I;
         }

         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_C0_GSM1800_TX_CAL_DATA_I;
         if (nvSelector == 2)
         {
            nvID = RFNV_GSM_C2_GSM1800_TX_CAL_DATA_I;
         }
         else if (nvSelector == 4)
         {
            nvID = RFNV_GSM_C4_GSM1800_TX_CAL_DATA_I;
         }

         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_C0_GSM1900_TX_CAL_DATA_I;
         if (nvSelector == 2)
         {
            nvID = RFNV_GSM_C2_GSM1900_TX_CAL_DATA_I;
         }
         else if (nvSelector == 4)
         {
            nvID = RFNV_GSM_C4_GSM1900_TX_CAL_DATA_I;
         }

         break;
      default:
         stringstream ostr;
		 ostr.str("");
		 ostr << "Unknown NV Mode" << m_curBand << ". Please remove non-GSM band from band list.\n"
			 << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
		 RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
		 return false;
   }

   bResult = QLIB_FTM_RFNV_READ( hPhone, nvID, nvArr, &nvSize, &readStatus );

   if(readStatus != 0 || bResult == 0)
   {
	   stringstream ostr;
	   ostr.str("");
	   ostr << "GSM CxTx NV read from DUT failed for NV " << nvID << " - status: " << readStatus << ". Check DUT status.\n"
		   << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
	   RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
	   return false;
   }

   payloadSize = nvSize;

   // fill the readGsmNV payload byte by byte
   memcpy_s( readGsmNV.nvRev0.aPayload, sizeof(readGsmNV.nvRev0.aPayload), nvArr, payloadSize);

   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::GSM_Get_NV_Container( HANDLE hPhone, GSM_CAL_Params &params, FTM_RF_Mode_Enum rfMode, unsigned char primPath, unsigned char forTxOrRx,
	unsigned char & nvContainer )
{

	unsigned char cmdSuccessful = 1;
	unsigned short status;


	// updates "nvContainer" which determines the NV container to which the GSM Rx cal data is written.
	cmdSuccessful = QLIB_FTM_GSM_GET_NV_CONTAINER( hPhone, &nvContainer, static_cast<unsigned char>(rfMode),
		forTxOrRx, primPath, &status );

	if (cmdSuccessful == 0)
	{
		ostringstream ostr;
		ostr << "QLIB_FTM_GSM_GET_NV_CONTAINER() failed for band " <<  rfMode << ", primary path " << primPath
			<< "\n" << __FUNCTION__ << " " << __LINE__;
		RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
		return false;
	}

	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Do_V5_Cal( HANDLE hPhone, GSM_CAL_Params & params, GSM_V4_PCL_Results_Chain & gsmV4_PCL_Results, 
	GSM_V4_DPD_Results_Chain & gsmV4_DPD_Results, GSM_V4_RSSI_Est_Chain & gsmV4_RSSI_Results, GSM_V4_RSSI_Est_Chain & gsmV4_RSSI_Results_HiLin)
{

	RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
	if ( NULL == pMgr ) 
		return false;

	// Assume no need for alternate path Cal
	//------------------------------------ Create Call Box Command -------------------------------------------//
	const bool performTxCal = params.gsm_v4_Params.m_DA_Params.performDaCal || params.gsm_v4_Params.m_DPD_Params.performDpdCal;

	// the GSM V4 FTM Sweep command buffer and it's true eventual size.
	unsigned short gsmV4CmdSize = 0;
	unsigned char GSM_V4_FTM_CMD_BUFF[GSM_V4_TEMP_CMD_BUFF_SIZE];

	// identifies which frames had Rx burst set in them
	// map< band, vector of frame IDs >
	map<unsigned int, vector<unsigned short>> RX_BURST_FRAME_IDENTIFIER;

	// create the gsm_V4_params, describing the frame and slot structure
	if ( performTxCal )
	{
		// handles combinations of all cals (DA, Rx, DPD)

		Create_GSM_V4_Cal_Command( hPhone, params, GSM_V4_FTM_CMD_BUFF, gsmV4CmdSize, RX_BURST_FRAME_IDENTIFIER );

	}

	unsigned short numContiguousTxMeasPerBand_CB = (params.gsm_v4_Params.m_MeasInfo.numContiguousDA_Meas + params.gsm_v4_Params.m_MeasInfo.numContiguousDPD_Meas_CB) * static_cast<unsigned short>(params.gsm_v4_Params.m_bcc.m_numChan);

	//------------------------------------ Set up Test Equipment ---------------------------------------//


	RFCal::GSM_Cal::GSM_V5::FillCallBoxParams( hPhone, params );

	pMgr->Get_Version();
	bool bSkipFirstRxMeas = false;

	pMgr->Config_GSM_V4_TxRx_Sweep( params.gsm_v4_Params.m_sweepParamsTestEquip, params.gsm_v4_Params.swpParams, params.gsm_v4_Params.m_Rx_Params.m_skipFirstRxMeas );

	pMgr->Start_GSM_V4_Sweep( params.gsm_v4_Params.swpParams );

	//----------------------------- Execute command to run the sweep ----------------------------------//
	// disabled predistortion for each Tx/Rx chain, configures the sweep, and executes the sweep
	Do_GSM_V5_Sweep( hPhone, params, GSM_V4_FTM_CMD_BUFF, gsmV4CmdSize, params.gsm_v4_Params.m_bcc.m_txChainList );


	//--------------------------- Get the Tx (Uplink) measurements ------------------------------------//
	// list of recorded Tx powers - this filters out measurements for non-Tx slots
	vector<double> outputPowVec;
	if ( performTxCal )
	{
		outputPowVec.resize( params.gsm_v4_Params.m_MeasInfo.numExpTxMeas_CB );
		pMgr-> Get_GSM_V4_Tx_Sweep_Results( params.gsm_v4_Params.m_sweepParamsTestEquip.m_num_steps, params.gsm_v4_Params.m_TxSlotConfig_CB,
			outputPowVec, numContiguousTxMeasPerBand_CB, params.gsm_v4_Params.m_intraBandSegDelay );
	}	
	//--------------------------- Get the Rx (Downlink) measurements ----------------------------------//
	GSM_V4_FTM_CMD_Response_Type gsmV4_Rx_MeasResults;
	memset( &gsmV4_Rx_MeasResults, 0, sizeof(GSM_V4_FTM_CMD_Response_Type) );
	// don't need to run this unless Rx cal was run
	if ( params.gsm_v4_Params.m_Rx_Params.performRxCal )
	{
		if ( !RFCal::GSM_Cal::GSM_V4::Get_GSM_V4_Sweep_Results( hPhone, gsmV4_Rx_MeasResults ))
		{
			return false;
		}
	}
	else // Rx cal is not being run
	{
		// This is performed automatically if UUT_Get_GSM_V4_Sweep_Results() is called. So, if Rx cal is not run, this command needs to be run.
		if (!RFCal::GSM_Cal::GSM_V4::Do_GSM_V4_Cleanup(hPhone))
		{
			return false;
		}
	}

	// stop the call box sweep (only some call boxes actually require this, i.e. 8820C/8870A)
	pMgr->End_GSM_V4_Sweep( params.gsm_v4_Params.swpParams );

	//-------------------------------- Tx Measurement Processing ------------------------------------//
	stringstream ss;
	if (performTxCal)
	{
		// output debug information if requested
		/*		if ( m_curCalParams.m_logDebugWaveform )
		{
		Log_V4_Meas_Waveform_To_File( outputPowVec );
		}
		*/
		// maps of Tx Chain to associated power vector
		map< unsigned int, vector<double> > dpdPowMap_Chain;
		map< unsigned int, vector<double> > daPowMap_Chain;

		RFCal::GSM_Cal::GSM_V5::Tx::Segment_DPD_and_DA_Pwr( hPhone, params, outputPowVec, dpdPowMap_Chain, daPowMap_Chain );

		// Loop through Tx Chains to process Tx Results
		for (unsigned short curChainIdx=0; curChainIdx<params.gsm_v4_Params.m_bcc.m_numTxChains; ++curChainIdx)
		{
			unsigned int m_curChainID = params.gsm_v4_Params.m_bcc.m_txChainList[curChainIdx];

			//Get Tx Chain ID from Tx Chain List param for now
			unsigned int m_txChainID = m_curChainID;

			//-- Obtain the PCL Power Deltas
			if ( params.gsm_v4_Params.m_DA_Params.performDaCal )
			{
				Tx::Fill_PCL_Results( hPhone, params, gsmV4_PCL_Results.chainData[m_txChainID], daPowMap_Chain[m_txChainID] );
				ss.str("");
				ss << "DA done.";
				RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());

			}

			//-- Obtain the DPD Results
			if ( params.gsm_v4_Params.m_DPD_Params.performDpdCal || params.gsm_v4_Params.m_DPD_Params.doRGIRunOnly )
			{
				Tx::Fill_DPD_Results( hPhone, params, gsmV4_DPD_Results.chainData[m_txChainID], dpdPowMap_Chain[m_txChainID] );
				ss.str("");
				ss << "DPD done.";
				RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
			}
		}
	}

	//---------------------------------- Rx Measurement Processing --------------------------------//
	if ( params.gsm_v4_Params.m_Rx_Params.performRxCal )
	{
		GSM_V4_Rx_Results_Chain rssiMaps;
		GSM_V4_Rx_Results_Chain rssiMapsHiLin;

		params.gsm_v4_Params.m_Rx_Params.numExpectedPayloads = static_cast<unsigned short>( params.gsm_v4_Params.m_numFrames * params.gsm_v4_Params.m_bcc.m_numRxChains );

		//input - gsmV4_Rx_MeasResults; RX_BURST_FRAME_IDENTIFIER
		//output - rssiMaps
		Rx::Process_Rx_Meas_Results( hPhone, params, gsmV4_Rx_MeasResults, rssiMaps, rssiMapsHiLin, RX_BURST_FRAME_IDENTIFIER, params.gsm_v4_Params.m_bcc, params.gsm_v4_Params.m_Rx_Params );

		for (unsigned short chainIdx=0; chainIdx<params.gsm_v4_Params.m_bcc.m_numRxChains; ++chainIdx)
		{
			unsigned int m_curChainID = params.gsm_v4_Params.m_bcc.m_rxChainList[chainIdx];
			unsigned int m_pRxChainID = m_curChainID;
			unsigned int m_dRxChainID = m_pRxChainID + 1;


			for ( unsigned int m_curBandIdx=0; m_curBandIdx<params.gsm_v4_Params.m_bcc.m_numBands; ++m_curBandIdx)
			{
				int m_curBand  = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
				//input - rssiMaps.chainData[m_pRxChainID]
				//output - gsmV4_RSSI_Results.chainData[m_pRxChainID]
				Rx::Estimate_RSSI_From_CharData( hPhone, params, rssiMaps.chainData[m_pRxChainID], gsmV4_RSSI_Results.chainData[m_pRxChainID], m_pRxChainID, false, m_curBandIdx );

				if ( params.gsm_v4_Params.m_Rx_Params.performDRxCalList[m_curBandIdx]  )
				{
					Rx::Estimate_RSSI_From_CharData( hPhone, params, rssiMaps.chainData[m_dRxChainID], gsmV4_RSSI_Results.chainData[m_dRxChainID], m_dRxChainID, false, m_curBandIdx );

				}
			}

		}
		ss.str("");
		ss << "Rx done.";
		RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());
	}
	return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Rx::Estimate_RSSI_From_CharData( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_Rx_Results_Band_Chan& rssiMaps, GSM_V4_Rx_RSSI_Estimates_Band_Chan& bandToChan_EstRSSI_Map,
	unsigned int chainID, bool isHiLinearity, int m_curBandIdx )
{

	stringstream ostr;
	unsigned short numChan  = static_cast<unsigned short>( rssiMaps.rssi_Map[ params.gsm_v4_Params.m_bcc.m_bandList[0] ].chanToLNA_AvgRSSI.size() );
	unsigned int   numLNA   = isHiLinearity? params.gsm_v4_Params.m_Rx_Params.numLnaStateHiLin : params.gsm_v4_Params.m_Rx_Params.numLNAState;

	// indexes
	unsigned short chanIdx = 0;
	unsigned short lnaIdx  = 0;
	int m_curBand  = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];

	vector<unsigned int> lnaStateList = isHiLinearity? params.gsm_v4_Params.m_Rx_Params.lnaStateListHiLin : params.gsm_v4_Params.m_Rx_Params.lnaStateList;
	vector<int>          lnaPowerList = isHiLinearity? params.gsm_v4_Params.m_Rx_Params.lnaPowerListHiLin : params.gsm_v4_Params.m_Rx_Params.lnaPowerList;

	BAND_CLASS_ENUM curBandEnum = RFCal::Helper::Convert::ConverGSMBand_To_BandClassEnum( m_curBand );

	unsigned int curRSSI  = 0;            // the current RSSI value
	double       curRxLev = 0.0;          // current Rx downlink power level
	unsigned int curLNA   = 0;            // the current LNA state

	//---------------------------- Compute Meas Rx (downlink) Gains --------------------------------//
	// map of LNA state to Rx (downlink) level
	map<unsigned int, int> lnaToRxLev_Map;
	for (lnaIdx = 0; lnaIdx < numLNA; ++lnaIdx)
	{
		lnaToRxLev_Map[ lnaStateList[lnaIdx] ] = lnaPowerList[lnaIdx];
	}

	// Create a map<channel, downlink frequency>
	// First fill it with calibrated channels, then add the Char channels next.
	map<unsigned int, double> chanTo_DlFreq;
	GSMRxCharTable            chan_measLnaGain;  // GSMRxCharTable = map< channel, map<lna state, gain> >

	for (chanIdx = 0; chanIdx < numChan; ++chanIdx )
	{
		int m_curChan                = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];
		int band = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];
		double ULFrequency = 0;
		double DLFrequency = 0;

		RFCal::Helper::Convert::ULchannel_To_Frequency(TECHNOLOGY_GSM, band, m_curChan, ULFrequency, DLFrequency);
		chanTo_DlFreq[m_curChan] = DLFrequency;

		for (lnaIdx = 0; lnaIdx < numLNA; ++lnaIdx)
		{
			curLNA   = lnaStateList[lnaIdx];
			curRSSI  = rssiMaps.rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA];
			curRxLev = static_cast<double>( lnaToRxLev_Map[curLNA] );
			chan_measLnaGain[m_curChan][curLNA] = ( 10*log10( curRSSI / 1.0 ) - curRxLev );
		}
	}

	// ---------------------------- Read in the Char data --------------------------//
	GSMRxCharTable chan_charLnaGain;                                 // Map of map< channel, map<lna state, gain> >
	unsigned char v2RxCharExists = 0;

	FTM_RF_Mode_Enum mode = static_cast<FTM_RF_Mode_Enum>( m_curBand );

	//------------ Read in the Char Rx Data ------------//

	//----- 1) First check the multi-chain format Rx Char Table
//	v2RxCharExists = GSMRxCharTableGeneratorV2::getObject()->getGSMRxCharTable( mode, chainID, chan_charLnaGain );

	GSMRxCharData_ChainBand gData;
	v2RxCharExists =  (params.repository.GetGSMRxCharData (gData));
	chan_charLnaGain = gData[static_cast<int> (chainID)][ static_cast<int> (mode)];

		//----- 2) If there is no V2 Rx char data, see if there is any V1 Rx char data
	if (!v2RxCharExists)
	{
		//GSMRxCharTableGenerator::getObject()->getGSMRxCharTable( mode, chan_charLnaGain );
		ostr.str("");
		ostr << "Error: \"GSM V4 Cal - Rx\", characterized Rx table unavailable  " << ". \n"
			<< "Check characterization data input file.\n"
			<< "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
		RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
		return false;
	}

	// Check if any characterized data is available
	if( chan_charLnaGain.size() == 0 )
	{
		ostr.str("");
		ostr << "Error: \"GSM V4 Cal - Rx\", characterized Rx table unavailable for band: " << mode << ", chain: "
			<< chainID << ". \n"
			<< "Check characterization data input file.\n"
			<< "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
		RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
		return false;
	}

	// Determine the channel list.
	// Also, add channels to the map<channel, freq>
	map<int, map<int, double> >::iterator chanIter;
	vector<unsigned int> charChanVec;

	for(chanIter = chan_charLnaGain.begin(); chanIter!=chan_charLnaGain.end(); ++chanIter)
	{
		charChanVec.push_back(chanIter->first);

		double ULFrequency = 0;
		double DLFrequency = 0;

		RFCal::Helper::Convert::ULchannel_To_Frequency(TECHNOLOGY_GSM, m_curBand, chanIter->first, ULFrequency, DLFrequency);
	}

	// Determine the Reference Channels.
	// Note: only 2 or 1 reference channels can be used.
	const unsigned int minNumRefChan = 1;  // the minimum number of reference channels to have
	const unsigned int maxNumRefChan = 2;  // maximum number of reference channels to have
	vector<unsigned int> refChanVec;

	// Also, check if the characterized data contains all channels that need to calibrate.
	// Also, assume that all LNA ranges are always available.
	for ( chanIdx = 0; chanIdx < numChan; ++chanIdx )
	{
		int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

		if ( find( charChanVec.begin(), charChanVec.end(), m_curChan ) != charChanVec.end() )
		{
			refChanVec.push_back(m_curChan);
		}
	}
	// throw an exception if too few ref channels were found
	if ( refChanVec.size() < minNumRefChan )
	{
		ostr.str("");
		ostr << "Error: \"GSM V4 Cal - Rx\", band " << m_curBand << ", expected " << minNumRefChan << " reference channels in char data, only found " << refChanVec.size() << " ref channels.\n"
			<< "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
		RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
		return false;
	}
	// Sort the reference channels to ensure index 1 is always the largest channel.
	// This is useful for the RSSI estimation interpolation formula used later.
	sort( refChanVec.begin(), refChanVec.end() );
	// remove the middle element if more than 2 reference channels were found
	if ( refChanVec.size() > maxNumRefChan )
	{
		refChanVec.erase( refChanVec.begin() + 1 );
	}

	// ------------------------------------------ Process with the Char Data and Meas Data --------------------------------------//
	GSMRxCharTable chan_refGainDeltas;

	unsigned int numRefChan = refChanVec.size();

	//----- 1) Compute the Reference Channel Gain Deltas.
	for (chanIdx = 0; chanIdx < numRefChan; ++chanIdx )
	{
		int m_curChan = refChanVec[chanIdx];
		for (lnaIdx = 0; lnaIdx < numLNA; ++lnaIdx)
		{
			curLNA   = lnaStateList[lnaIdx];
			// compute the gain delta: gainDelta = (char Gain) - (Meas Gain)
			chan_refGainDeltas[m_curChan][curLNA] = chan_charLnaGain[m_curChan][curLNA] - chan_measLnaGain[m_curChan][curLNA];
		}
	}

	//----- 2) Compute the estimated RSSIs for all channels
	//map< unsigned int, map< int, vector<GSM_RX_MEAS_TYPE> > > bandToChan_EstRSSI_Map;
	GSM_RX_MEAS_TYPE tempRxMeas;

	unsigned int numCharChan = charChanVec.size();
	unsigned int refChan1    = 0;
	unsigned int refChan2    = 0;

	double refChan1_gainDelta = 0.0;
	double refChan2_gainDelta = 0.0;
	double curGain            = 0.0;
	double interp_gainDelta   = 0.0;

	int curEstRSSI = 0;

	if (numRefChan == 2)
	{
		double curFreq  = 0.0;
		double refFreq1 = 0.0;
		double refFreq2 = 0.0;

		refChan1 = refChanVec[0];
		refChan2 = refChanVec[1];
		refFreq1 = chanTo_DlFreq[refChan1];
		refFreq2 = chanTo_DlFreq[refChan2];
		// Make sure that refFreq2 is higher in frequency than refFreq1
		// This problem should only appear with EGSM, but who knows what the future holds --> just perform check for all bands
		if (refFreq2 < refFreq1)
		{
			// reverse the definition of refChan1 and refChan2
			refChan1 = refChanVec[1];
			refChan2 = refChanVec[0];
			refFreq1 = chanTo_DlFreq[refChan1];
			refFreq2 = chanTo_DlFreq[refChan2];
		}

		for (chanIdx = 0; chanIdx < numCharChan; ++chanIdx )
		{
			int m_curChan = charChanVec[chanIdx];
			curFreq   = chanTo_DlFreq[m_curChan];
			for (lnaIdx = 0; lnaIdx < numLNA; ++lnaIdx)
			{
				curLNA   = lnaStateList[lnaIdx];
				curRxLev = static_cast<double>( lnaToRxLev_Map[curLNA] );
				refChan1_gainDelta = chan_refGainDeltas[refChan1][curLNA];
				refChan2_gainDelta = chan_refGainDeltas[refChan2][curLNA];

				tempRxMeas.m_lna = curLNA;

				//------------ estimate the RSSI -----------//
				if ( (curFreq == refFreq2) || (curFreq == refFreq1) )
				{
					// already know the RSSI for reference channels, so don't recompute
					curEstRSSI = rssiMaps.rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA];
				}
				else  // all channels other than reference channels
				{
					if (curFreq > refFreq2)
					{
						curGain = chan_charLnaGain[m_curChan][curLNA] - refChan2_gainDelta;
					}
					else if (curFreq < refFreq1)
					{
						curGain = chan_charLnaGain[m_curChan][curLNA] - refChan1_gainDelta;
					}
					else // use linear interpolation for frequencies between refChan1 and refChan2 gain deltas
					{
						interp_gainDelta = ( (refChan2_gainDelta - refChan1_gainDelta) / (refFreq2 - refFreq1) ) * (curFreq - refFreq1) + refChan1_gainDelta;
						curGain = chan_charLnaGain[m_curChan][curLNA] - interp_gainDelta;
					}

					curEstRSSI = static_cast<int>( pow(10.0, (curGain + curRxLev)/10.0 ) );
				}
				tempRxMeas.m_rssi = curEstRSSI;
				bandToChan_EstRSSI_Map.estRSSI_Map[m_curBand][m_curChan].push_back( tempRxMeas );
			}
		}
	}
	else if (numRefChan == 1)
	{
		refChan1    = refChanVec[0];

		for (chanIdx = 0; chanIdx < numCharChan; ++chanIdx )
		{
			int m_curChan  = charChanVec[chanIdx];
			for (lnaIdx = 0; lnaIdx < numLNA; ++lnaIdx)
			{
				curLNA             = lnaStateList[lnaIdx];
				curRxLev           = static_cast<double>( lnaToRxLev_Map[curLNA] );
				refChan1_gainDelta = chan_refGainDeltas[refChan1][curLNA];
				tempRxMeas.m_lna   = curLNA;
				//------------- estimate the RSSI -----------//
				if (m_curChan == refChan1)
				{
					// already know the RSSI for reference channels, so don't recompute
					curEstRSSI = rssiMaps.rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA];
				}
				else
				{
					curGain    = chan_charLnaGain[m_curChan][curLNA] - refChan1_gainDelta;
					curEstRSSI = static_cast<int>( pow(10.0, (curGain + curRxLev)/10.0 )  );
				}
				tempRxMeas.m_rssi = curEstRSSI;
				bandToChan_EstRSSI_Map.estRSSI_Map[m_curBand][m_curChan].push_back( tempRxMeas );
			}
		}
	}

	return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Rx::Process_Rx_Meas_Results( HANDLE hPhone, GSM_CAL_Params &params,const GSM_V4_FTM_CMD_Response_Type& gsmV4_Rx_MeasResults, GSM_V4_Rx_Results_Chain& rssiMaps,
                                               GSM_V4_Rx_Results_Chain& rssiMapsHiLin, map<unsigned int, vector<unsigned short>>& RX_BURST_FRAME_IDENTIFIER,
                                               GSM_V4_BandChainChan_Params& bcc, GSM_V4_Rx_Params& rxParams )
{
   stringstream ostr;
   // See if the returned results has the correct # of payloads
   if ( gsmV4_Rx_MeasResults.numPayload < rxParams.numExpectedPayloads)
   {

      ostr.str("");
      ostr << "Error: \"GSM V4 Cal - Rx\"\n"
           << "Expected " << rxParams.numExpectedPayloads << " payloads.\n"
           << "Received " << gsmV4_Rx_MeasResults.numPayload << " payloads.\n"
           << "Check phone log and firmware trace for further investigation. \n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
      return false;
   }

   unsigned int curLNA           = 0;   // the current LNA state
   unsigned int curRSSI          = 0;   // the current RSSI value
   unsigned short initPayloadIdx = 0;   // initial frame index

   if ( rxParams.m_skipFirstRxMeas )
   {
      // ignore the measurement for first payloads for each device, if told to skip the first Rx measurement
      initPayloadIdx = 1 * static_cast<unsigned short>(bcc.m_numRxChains);
   }

   // map<chain, map< band, map< channel, multimap< LNA state, All RSSI values > > > >
   // A multimap is used because multiple RSSI measurements can occur for a single LNA state.
   
  ChainBandChanLNARSSI_type chanToLNA_AllRSSI;
  ChainBandChanLNARSSI_type chanToLNA_AllRSSI_Hi_Lin;

   //------------ Extract Desired RSSI Measurements from gsmV4_Rx_MeasResults -----------//
   // Note: gsmV4_Rx_MeasResults contains measurements for every frame, regardless of whether bursts where scheuled there.
   // The frameID of gsmV4_Rx_MeasResults can occassionally be out of order.
   // This Extraction assumes no order
   for ( unsigned int m_curBandIdx = 0; m_curBandIdx < bcc.m_numBands; ++m_curBandIdx )
   {
      int m_curBand = bcc.m_bandList[m_curBandIdx];

      for (unsigned short rxSchedIdx=0; rxSchedIdx < static_cast<unsigned short>(RX_BURST_FRAME_IDENTIFIER[m_curBand].size() ); ++ rxSchedIdx)
      {
         // counter indicating whether all desired frames were found
         unsigned short foundFrameCntr = 0;
         // the current desired frame ID
         unsigned short curDesFrame    = RX_BURST_FRAME_IDENTIFIER[m_curBand][rxSchedIdx];

         // search through payload for desired frames
         for (unsigned short payloadIdx = initPayloadIdx; payloadIdx < gsmV4_Rx_MeasResults.numPayload; ++payloadIdx)
         {
            if (curDesFrame == gsmV4_Rx_MeasResults.payload[payloadIdx].frameID)
            {
               unsigned char m_pRxChainID = gsmV4_Rx_MeasResults.payload[payloadIdx].devID;
               int m_curBand    = bcc.m_bandList[m_curBandIdx];

               if(gsmV4_Rx_MeasResults.payload[payloadIdx].result[0].opID == 2)
               {
                  unsigned short m_curChan    = gsmV4_Rx_MeasResults.payload[payloadIdx].result[0].opResult.rssiResult.channel;
                  curLNA       = gsmV4_Rx_MeasResults.payload[payloadIdx].result[0].opResult.rssiResult.lnaState;
                  curRSSI      = gsmV4_Rx_MeasResults.payload[payloadIdx].result[0].opResult.rssiResult.rssiMeasResult;

                  /***
                  Need to work with FW team to finalize how linearity info is carried in the response.
                  For now, assume LNA state in the Rx measuremetn result contains linearity info:
                  if the MSB is set to 1, this means high linearity.
                  */
                  //if( curLNA >= 128 )
                  //{
                  //   chanToLNA_AllRSSI_Hi_Lin[m_pRxChainID][m_curBand][m_curChan].insert( pair<unsigned int, unsigned int>(curLNA - 128, curRSSI) );
                  //}
                  //else
                  {
                    chanToLNA_AllRSSI[m_pRxChainID].data[m_curBand].data[m_curChan].data.insert( pair<unsigned int, unsigned int>(curLNA, curRSSI) );

                  }

                  // look at the 2nd result if doing DRx calibration
                  if ( rxParams.performDRxCalList[m_curBandIdx] )
                  {
                     unsigned char m_dRxChainID = m_pRxChainID + 1;
                     m_curChan  = gsmV4_Rx_MeasResults.payload[payloadIdx].result[1].opResult.rssiResult.channel;
                     curLNA     = gsmV4_Rx_MeasResults.payload[payloadIdx].result[1].opResult.rssiResult.lnaState;
                     curRSSI    = gsmV4_Rx_MeasResults.payload[payloadIdx].result[1].opResult.rssiResult.rssiMeasResult;

                     //if( curLNA >= 128 )
                     //{
                     //   chanToLNA_AllRSSI_Hi_Lin[m_dRxChainID][m_curBand][m_curChan].insert( pair<unsigned int, unsigned int>(curLNA - 128, curRSSI) );
                     //}
                     //else
                     {
						//LNARSSI_type rssi;
						//			rssi.
                        chanToLNA_AllRSSI[m_dRxChainID].data[m_curBand].data[m_curChan].data.insert( pair<unsigned int, unsigned int>(curLNA, curRSSI) );
                     }
                  }

                  ++foundFrameCntr;
               }
            }
            // increment the counter that specifies how many chains were found
            if (foundFrameCntr == bcc.m_numRxChains)
            {
               break;
            }
         }
      }
   }

   // Now, verify that each channel/lna combination exits.
   // Also, average the RSSI results for each channel/lna combination.
   if (( params.gsm_v4_Params.m_Rx_Params.performRxCal ) &&(!params.gsm_v4_Params.m_FBRx_Params.performRxCalwithChar))
   {
      for (unsigned char devIdx=0; devIdx < bcc.m_numRxChains; ++devIdx)
      {
         unsigned int m_pRxChainID = bcc.m_rxChainList[devIdx];
         unsigned int m_dRxChainID = m_pRxChainID + 1;

         for ( unsigned int m_curBandIdx = 0; m_curBandIdx < bcc.m_numBands; ++m_curBandIdx )
         {
            int m_curBand = bcc.m_bandList[m_curBandIdx];
            for (unsigned short chanIdx = 0; chanIdx < params.gsm_v4_Params.m_FBRx_Params.m_chanList_rxcal[0].size(); ++chanIdx)
            {
               int m_curChan = params.gsm_v4_Params.m_FBRx_Params.m_chanList_rxcal[m_curBandIdx][chanIdx];
               Filter_RSSIs( hPhone, params, chanToLNA_AllRSSI, m_pRxChainID, rssiMaps, false, rxParams, m_curBand, m_curChan );

               if ( rxParams.performDRxCalList[m_curBandIdx] )
               {
                  Filter_RSSIs( hPhone, params,chanToLNA_AllRSSI, m_dRxChainID, rssiMaps, false, rxParams, m_curBand, m_curChan );

               }

            }  // end chan loop
         }  // end band loop
      }  // End DEV Loop

   }
   else
   {
      for (unsigned char devIdx=0; devIdx < bcc.m_numRxChains; ++devIdx)
      {
         unsigned int m_pRxChainID = bcc.m_rxChainList[devIdx];
         unsigned int m_dRxChainID = m_pRxChainID + 1;

         for ( unsigned int  m_curBandIdx = 0; m_curBandIdx < bcc.m_numBands; ++m_curBandIdx )
         {
            int m_curBand = bcc.m_bandList[m_curBandIdx];
            for (unsigned short chanIdx = 0; chanIdx < bcc.m_numChan; ++chanIdx)
            {
               int m_curChan = bcc.m_chanList[m_curBandIdx][chanIdx];
               Filter_RSSIs( hPhone, params,chanToLNA_AllRSSI, m_pRxChainID, rssiMaps, false, rxParams, m_curBand, m_curChan );

               if ( rxParams.performDRxCalList[m_curBandIdx] )
               {
                  Filter_RSSIs(hPhone, params, chanToLNA_AllRSSI, m_dRxChainID, rssiMaps, false, rxParams, m_curBand, m_curChan );


               }
            }  // end chan loop
         }  // end band loop
      }  // End DEV Loop
   }

   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Rx::Filter_RSSIs( HANDLE hPhone, GSM_CAL_Params &params,  ChainBandChanLNARSSI_type & chanToLNA_AllRSSI,
	unsigned int chainID, GSM_V4_Rx_Results_Chain& rssiMaps, bool isHiLinearity, GSM_V4_Rx_Params& rxParams, int m_curBand, int m_curChan )
{
	stringstream ostr;

   // iterator pair that identifies the range of RSSI values with the same LNA state, for a given channel
   pair <multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> lnaRange;
   // iterates over RSSI values for the same LNA state for a given channel.
   multimap<unsigned int, unsigned int>::iterator rssiIter;

   unsigned short curNumMeas = 0;
   unsigned int curLNA       = 0;   // the current LNA state

   // statistics used for filtering out bad RSSI measurements.
   // with some call boxes, occasionally the 2nd of the 2 RSSI measurements for a given LNA/pow comgination is bad (essentially, no donwlink power)
   unsigned int curMeanRSSI   = 0;     // the mean RSSI measurement for a given band/channel/LNA
   float tempMeanDiff_RSSI    = 0.0;   // temporary variable for (xN - xMean)
   float curSumMeanDiff_RSSI  = 0.0;   // Sum[ (xN - xMean)^2 ]
   float curCoV_RSSI          = 0.0;   // current coefficient of variation ( StDev / Mean)

   static const double MAX_COV_RSSI   = 0.35;         // maximum allowed CoV
   static const unsigned int MAX_RSSI = 100000000;    // maximum allowed RSSI - used for filtering out bad RSSI measurements

   // stores all of the RSSI measurements for the current band/channel/LNA
   vector<unsigned int> curRSSI_Vec;

   char* linearityLabel = isHiLinearity? "Hi" : "Lo";

#ifdef DEBUG_RSSI_RESULTS
   {
      ostr.str("");
      ostr << "RSSI Measurements -> Chain(" << chainID << ") Band(" << m_curBand << ") Chan(" << m_curChan << ") Linearity(" << linearityLabel << ")";
      m_pUIM->PrintToDebug( ostr.str() );
   }
#endif

   vector<unsigned int> lnaList;
   if( isHiLinearity )
   {
      lnaList = rxParams.lnaStateListHiLin;
   }
   else
   {
      lnaList = rxParams.lnaStateList;
   }
   
   // verify that curChan exists
   if ( chanToLNA_AllRSSI[chainID].data[m_curBand].data.find(m_curChan) != chanToLNA_AllRSSI[chainID].data[m_curBand].data.end() )
   {
      for (unsigned short lnaIdx = 0; lnaIdx < lnaList.size(); ++lnaIdx)
      {
         curLNA   = lnaList[lnaIdx];
         lnaRange = chanToLNA_AllRSSI[chainID].data[m_curBand].data[m_curChan].data.equal_range(curLNA);

         // verify that the LNA state exists
         if ( lnaRange.first == lnaRange.second )
         {
            ostr.str("");
            ostr << "Error: \"GSM V4 Cal - Rx\", no measurment results for linearity " << linearityLabel << " channel " << m_curChan << " LNA " << curLNA << " exist.\n"
                 << "Check if the DUT supports the linearity and LNA gain state.\n"
                 << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
			RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
			return false;
         }

         // Add all RSSI values together for the same LNA state for a given channel
         for (rssiIter = lnaRange.first; rssiIter != lnaRange.second; ++rssiIter)
         {
            curRSSI_Vec.push_back(rssiIter->second);
            curMeanRSSI += rssiIter->second;
            ++curNumMeas;
         }
         // average the results
         curMeanRSSI /= curNumMeas;

         // compute the stdDev
         for (rssiIter = lnaRange.first; rssiIter != lnaRange.second; ++rssiIter)
         {
            tempMeanDiff_RSSI = static_cast<float>(rssiIter->second) - static_cast<float>(curMeanRSSI);
            curSumMeanDiff_RSSI += tempMeanDiff_RSSI * tempMeanDiff_RSSI;
         }
         // (coefficient of variation) = (standard deviation) / (mean)
         curCoV_RSSI  = sqrt(curSumMeanDiff_RSSI / curNumMeas) / curMeanRSSI;

         // Filter out bad RSSI results. Occasionally, especially with the 8820C, one of the 2 RSSI measurements is bad.
         // If the coefficient of variation is too large, take the good RSSI.
         // Otherwise, take the mean RSSI.
         if ( curCoV_RSSI > MAX_COV_RSSI )
         {
            unsigned int minRSSI = *min_element( curRSSI_Vec.begin(), curRSSI_Vec.end() );
            unsigned int maxRSSI = *max_element( curRSSI_Vec.begin(), curRSSI_Vec.end() );

            if (maxRSSI > MAX_RSSI)
            {
               rssiMaps.chainData[chainID].rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA] = minRSSI;
            }
            else
            {
               rssiMaps.chainData[chainID].rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA] = maxRSSI;
            }
            ostr.str("");
            ostr << "...Warning: encountered large CoV in RSSI for chain: " << chainID << " band: " << m_curBand << " chan: " << m_curChan
                 << " Linearity: " << linearityLabel << " LNA: " << curLNA << " with Cov of " << curCoV_RSSI << ".";
			RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
            ostr.str("");
            ostr << "...maxRSSI(" << maxRSSI << ")  minRSSI(" << minRSSI << ") ";
            RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
			return false;
         }
         else
         {
            rssiMaps.chainData[chainID].rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA] = curMeanRSSI;
         }
         // print the RSSI result to the screen
#ifdef DEBUG_RSSI_RESULTS
         {
            ostr.str("");
            ostr << " - LNA(" << curLNA << ") = "
                 <<  rssiMaps.chainData[chainID].rssi_Map[m_curBand].chanToLNA_AvgRSSI[m_curChan][curLNA];
            m_pUIM->PrintToDebug( ostr.str() );
         }
#endif
         // reset required variables
         curNumMeas          = 0;
         tempMeanDiff_RSSI   = 0.0;
         curSumMeanDiff_RSSI = 0.0;
         curMeanRSSI         = 0;
         curCoV_RSSI         = 0.0;
         curRSSI_Vec.clear();
      }
   }
   else
   {
      // throw exception, no channel measurements exists
      ostr.str("");
      ostr << "Error: \"GSM V4 Cal - Rx\", no measurment results for chain "
           << chainID << ", band " << m_curBand << ", channel " << m_curChan << ", linearity " << linearityLabel << " exist. "
           << "Check phone log and firmware log for further investigation.\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
	  RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
      return false;
   }

   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Fill_DPD_Results( HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_DPD_Results_Band_Chan& gsmV4_DPD_Results, vector<double>& dpdPowerVec )
{
   // current value indicators
   unsigned short curDigGain  = 0;
   unsigned short curRGI      = 0;

   unsigned short dpdPowIdx  = 0;

   unsigned int numPowPerChan = params.gsm_v4_Params.m_DPD_Params.numDpd_Rgi * params.gsm_v4_Params.m_DPD_Params.numDpd_DigGain;

   // log to the debug window
   stringstream ostr;
   ostr.str("");
   ostr << "\n----- RGI Powers -----\n";

   // loop through bands
   for (unsigned int m_curBandIdx=0; m_curBandIdx<params.gsm_v4_Params.m_bcc.m_numBands; ++m_curBandIdx)
   {
      int m_curBand = params.gsm_v4_Params.m_bcc.m_bandList[m_curBandIdx];

      // loop through channels
      for (unsigned short chanIdx=0; chanIdx<params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx)
      {
         int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

         // log to QSPR debug window
         ostr << "---EDGE - PA " << params.gsm_v4_Params.m_DPD_Params.calPaRange << ", Chan " << m_curChan << "---\n";

         for (unsigned short vecIdx=0; vecIdx<numPowPerChan; ++vecIdx)
         {
            curDigGain = params.gsm_v4_Params.m_dpd_digGain_MapVec[m_curBand][vecIdx];
            curRGI     = params.gsm_v4_Params.m_dpd_RGI_MapVec[m_curBand][vecIdx];

            gsmV4_DPD_Results.band_Chan_sweepResults[m_curBand][m_curChan].digGainSweep[curDigGain].digGainPwr[curRGI] = dpdPowerVec[dpdPowIdx];

            // log to QSPR debug window
            ostr << "RGI::DG - Pow  " << curRGI << "::" << curDigGain << " - " << setprecision(3) << dpdPowerVec[dpdPowIdx] << "\n";

            ++dpdPowIdx;

         }
      }  // End chan loop
   }  // End band loop

   // log to QSPR debug window
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );

   return true;
}


unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Get_PCL_FromConsolidatedNV( HANDLE hPhone, int m_curBand, vector<short>& pclValues )
{

   const unsigned char numBytesVersion     = 2;
   const unsigned char numBytesNumElements = 2;
   const unsigned char numBytesPCLHeader   = 2;
   const unsigned char numBytesPerPCL      = 2;

   const unsigned int nvArrSize = numBytesVersion + numBytesNumElements + sizeof(QMSL_RFNV_VARIANT_SUB_HEADER_V2) + numBytesPCLHeader + (RFNV_PCL_NUM_ITEMS*numBytesPerPCL);

   unsigned short nvID               = RFNV_GSM_GSM900_POWER_LEVELS_I;
   unsigned short nvSize             = nvArrSize;
   unsigned short readStatus         = 0;
   unsigned char nvArr[nvArrSize]    = {0x00};
   bool bResult = true;

   switch(m_curBand)
   {
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_GSM900_POWER_LEVELS_I;
         break;
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_GSM850_POWER_LEVELS_I;
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_GSM1800_POWER_LEVELS_I;
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_GSM1900_POWER_LEVELS_I;
         break;
      default:
         stringstream ostr;
         ostr.str("");
         ostr << "Unknown NV Mode, " << m_curBand << ". Please remove non-GSM band from the band list.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
         return false;
   }

   bResult = (QLIB_FTM_RFNV_READ( hPhone, nvID, nvArr, &nvSize, &readStatus ) != 0);

   if(readStatus != 0 || !bResult)
   {
      stringstream ostr;
      ostr.str("");
      ostr << "PCL index value reading from DUT failed. Check DUT status.\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
      RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str() );
      return false;
   }

   // Extract the PCL power levels for each PCL index
   unsigned char  nvIdx  = 11;
   for (unsigned char pclIdx=0; pclIdx<RFNV_PCL_NUM_ITEMS; ++pclIdx)
   {
      pclValues[pclIdx] = nvArr[nvIdx] + (nvArr[nvIdx+1] << 8); // each pcl value is 2 bytes, so combine them. Returned values are in LSByte order
      nvIdx += numBytesPerPCL;
   }
   return true;
}
unsigned char  RFCal::GSM_Cal::GSM_V5::Tx::Compute_PCL_Deltas( HANDLE hPhone, GSM_CAL_Params & params, unsigned short & m_pclDeltaPowIdx,const vector<double>& inPowVec, const vector<short>& pclVecNV, GSM_V4_PCL_Sweep_Record& io_powDeltaMap,
                                          const vector<unsigned short>& pclList, bool isEdge, unsigned char maxEdgePCL)
{
   unsigned int numPCL = pclList.size();   // # of actual PCLs used for calibration

   // EDGE (8PSK) PCL error computation can be treated differently than GMSK. This is because the static NV does not contain predistortion data.
   // Therefore, any PCL indexes corresponding to power above the predisotrtion switch point yield invalid power measurements.
   // if true, don't compute all Edge PCL errors
   const bool computePartialEdgePclErrors = true;

   unsigned char maxPCL = static_cast<unsigned char>(numPCL);   // default for GMSK

   // only modify maxPCL to less than numPCL if it is EDGE, and it is desired to ignore high-power PCLs
   if (isEdge && computePartialEdgePclErrors)
   {
      maxPCL = maxEdgePCL;
   }

   // Note: m_pclDeltaPowIdx is an index for inPowVec - it accesses elements for inPowVec across multiple channels and bands - retains states upon multiple
   // calls to Compute_PCL_Deltas()
   unsigned char  pclListIdx = 0;   // index for pclList -> the list of PCL indexes actually used for calibration
   unsigned short pclIdx     = 0;   // index for pclVecNV -> the actual PCL powers for each PCL index, read from the NV

   const double pclConvFac = 100.0; // NV values in pclVecNV are multiplied by 100

   stringstream ostr;
   while ( pclListIdx < numPCL )
   {
      pclIdx = pclList[pclListIdx];
      // add maxPCL condidtion in case of EDGE - may want to not compute PCL errors for PCL indexes above predistortion limit
      if ( pclIdx < maxPCL )
      {
         // fill the power delta
         io_powDeltaMap.pclPwrDelta[pclIdx] = inPowVec[m_pclDeltaPowIdx] - ( (static_cast<double>( pclVecNV[pclIdx] ) / pclConvFac) + params.gsm_v4_Params.m_DA_Params.pclNvPowOffset );

         // log the result to QSPR
         ostr << " (" << pclIdx  << "," << setprecision(2) << io_powDeltaMap.pclPwrDelta[pclIdx]  << ") ";

         // fill the actual measured power
         io_powDeltaMap.pclMeasPwr[pclIdx] = inPowVec[m_pclDeltaPowIdx];
      }
      else
      {
         io_powDeltaMap.pclPwrDelta[pclIdx] = 0;
         io_powDeltaMap.pclMeasPwr[pclIdx]  = 0;
      }
      ++m_pclDeltaPowIdx;
      ++pclListIdx;
   }

   RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Get_Predist_Swpt_From_ConsolidatedNV( HANDLE hPhone, int m_curBand, const vector<short>& pclValues )
{
   const unsigned char numBytesVersion     = 2;
   const unsigned char numBytesNumElements = 2;
   const unsigned char numBytesPCLHeader   = 2;
   const unsigned char numBytesPerPCL      = 2;

   const unsigned int nvArrSize = numBytesVersion + numBytesNumElements + sizeof(QMSL_RFNV_VARIANT_SUB_HEADER_V2) + numBytesPCLHeader + (RFNV_PCL_NUM_ITEMS*numBytesPerPCL);

   unsigned short nvID       = RFNV_GSM_GSM900_EXTENDED_PA_SWPT_I;
   unsigned short nvSize     = nvArrSize;
   unsigned short readStatus = 0;
   short predistSwpt         = 0;
   bool bResult              = true;

   unsigned char nvArr[nvArrSize] = {0x00};

   switch(m_curBand)
   {
      case NV_MODE_GSM_EGSM:
         nvID = RFNV_GSM_GSM900_EXTENDED_PA_SWPT_I;
         break;
      case NV_MODE_GSM_850:
         nvID = RFNV_GSM_GSM850_EXTENDED_PA_SWPT_I;
         break;
      case NV_MODE_GSM_DCS:
         nvID = RFNV_GSM_GSM1800_EXTENDED_PA_SWPT_I;
         break;
      case NV_MODE_GSM_1900:
         nvID = RFNV_GSM_GSM1900_EXTENDED_PA_SWPT_I;
         break;
      default:
         return false;
   }

   bResult = (QLIB_FTM_RFNV_READ( hPhone, nvID, nvArr, &nvSize, &readStatus ) != 0);

   if(readStatus != 0 || !bResult)
   {
      stringstream ostr;
      ostr.str("");
      ostr << "Predistortion switchpoint reading from DUT failed. Check DUT status.\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
      RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ostr.str());
      return false;
   }

   predistSwpt = nvArr[39] + (nvArr[40] << 8);  //predistortion swpt is at the 18-th position, starting from 0

   unsigned int i;
   for (i=0; i<pclValues.size(); ++i)
      if (pclValues[i] >= predistSwpt) break;

   if (predistSwpt == -1)
      return 255; //indicating some data missing in NV or something went wrong
   else
      return static_cast<unsigned char>(i);

}

unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Fill_PCL_Results (HANDLE hPhone, GSM_CAL_Params &params, GSM_V4_PCL_Results_Band_Chan& gsmV4_PCL_Results, vector<double>& measPowerVec )
{	
	stringstream ostr;
	CGSM_V4_Cal_Params & p = params.gsm_v4_Params;
	GSM_V4_PCL_Sweep_Record    pclPwrDeltaRecord;
	GSM_V4_PCL_Results_Channel chanRecord;

	// current value indicators
	unsigned short curMod  = 0;

	// indicators of the current modulation scheme
	const unsigned short gsmMod  = QMSL_GSM_PwrErr_Per_PCL_GMSK_Type;
	const unsigned short edgeMod = QMSL_GSM_PwrErr_Per_PCL_8PSK_Type;

	//----------------- PCL Index Values from NV vector ------------------//
	vector<short> pclIdxValVec_NV(RFNV_PCL_NUM_ITEMS);

	//----------------- PCL index vector ------------------//
	// note --> pclIdxVec[gsmMod]  = gsm PCL index list
	// note --> pclIdxVec[edgeMod] = edge PCL index list
	vector<vector<unsigned short>> pclIdxVec( 2, vector<unsigned short>(p.m_DA_Params.pclIdxList[0]) );

	bool isEdge    = false;
	bool isLowBand = false;

	//set read from consolidated NV parameter to true or false based on m_setDeviceAndSigPath variable
	bool readConsolidatedNV = ( params.setRFMPathVersion != 0) ? true : false;

	// initialize m_pclDeltaPowIdx to 0
	unsigned short m_pclDeltaPowIdx = 0;

	// log to the debug window
	ostr.str("");
	ostr << "----- PCL Errors -----";

	RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ostr.str());

	// loop through bands
	for (unsigned int m_curBandIdx=0; m_curBandIdx<p.m_bcc.m_numBands; ++m_curBandIdx)
	{
		int m_curBand = p.m_bcc.m_bandList[m_curBandIdx];

		isLowBand = DetermineIfLowBand( m_curBand );
		//----------------- Read PCL Index Values From NV ------------------//
		CRFM_Path m_calRFMPaths;
		if(readConsolidatedNV)
			Get_PCL_FromConsolidatedNV( hPhone, m_curBand, pclIdxValVec_NV );
		//		else
		//			Get_PCL_FromNV( pclIdxValVec_NV );

		//Read the EDGE predistortion switchpoint
		unsigned char predistSwptPCLIndex = 0;
		if(readConsolidatedNV)
			predistSwptPCLIndex =  RFCal::GSM_Cal::GSM_V5::Tx::Get_Predist_Swpt_From_ConsolidatedNV(hPhone, m_curBand, pclIdxValVec_NV);
		//		else
		//			predistSwptPCLIndex = Get_Predist_Swpt_From_NV(pclIdxValVec_NV);

		if (predistSwptPCLIndex == 255) //some error occurred, so set it to default swpt
			predistSwptPCLIndex = isLowBand ? gsmV4_max8PSK_pclIdx_LB : gsmV4_max8PSK_pclIdx_HB;

		// loop through channels
		for (unsigned short chanIdx=0; chanIdx< params.gsm_v4_Params.m_bcc.m_numChan; ++chanIdx)
		{
			int m_curChan = params.gsm_v4_Params.m_bcc.m_chanList[m_curBandIdx][chanIdx];

			// loop through modulations
			for (unsigned short modIdx=0; modIdx<params.gsm_v4_Params.m_DA_Params.numMods; ++modIdx)
			{
				// determine the current modulation type
				curMod = ( (params.gsm_v4_Params.m_DA_Params.modIdxList[modIdx] > gsmV4_maxGMSK_mcsIdx) ? edgeMod : gsmMod ) ;
				if (curMod == edgeMod)
				{
					isEdge = true;
				}
				else
				{
					isEdge = false;
				}

				// compute the PCL Power Deltas
				Compute_PCL_Deltas( hPhone, params, m_pclDeltaPowIdx, measPowerVec, pclIdxValVec_NV, pclPwrDeltaRecord, pclIdxVec[curMod], isEdge, predistSwptPCLIndex );

				// fill the GSM_V4_PCL_Results_Channel record with the GSM_V4_PCL_Sweep_Record
				if (curMod == gsmMod)
				{
					chanRecord.gsmPCLSweep[m_curChan] = pclPwrDeltaRecord;
				}
				else
				{
					chanRecord.edgePCLSweep[m_curChan] = pclPwrDeltaRecord;
				}
				pclPwrDeltaRecord.pclMeasPwr.clear();
				pclPwrDeltaRecord.pclPwrDelta.clear();
			}  // END mod loop
		}  // END chan looop

		// fill the GSM_V4_PCL_Results_Band_Chan record with the GSM_V4_PCL_Results_Channel record
		gsmV4_PCL_Results.bandSweepData[m_curBand] = chanRecord;
		chanRecord.edgePCLSweep.clear();
		chanRecord.gsmPCLSweep.clear();

	}  // END band loop


	return true;
}
unsigned char RFCal::GSM_Cal::GSM_V5::Tx::Segment_DPD_and_DA_Pwr( HANDLE hPhone, GSM_CAL_Params &params, vector<double>& inputPowVec,
	map< unsigned int, vector<double> >& dpdPowMap_Chain, map< unsigned int, vector<double> >& daPowMap_Chain )
{
	CGSM_V4_Cal_Params & p = params.gsm_v4_Params;

	vector<double> daPowVec(p.m_MeasInfo.numTotalDA_Meas);
	vector<double> dpdPowVec(p.m_MeasInfo.numTotalDPD_Meas);

	// The sweep consists of doing all DA measurements per band, followed by all Dpd measurements per band
	unsigned short numDaPerBand  = p.m_MeasInfo.numContiguousDA_Meas  * static_cast<unsigned short>( p.m_bcc.m_numChan );

	// The call box can return extra measurements.
	// For example, 5 Digital Gains x 5 RGIs = 25 measurements, but requires 7 frames (4 slots per frame)
	// 7 frames yields 28 measurements.
	// So, numDpdToRemovePerChan = 28 - 25 = 3
	unsigned short numDpdToRemovePerChan = p.m_MeasInfo.numContiguousDPD_Meas_CB - p.m_MeasInfo.numContiguousDPD_Meas;

	vector<double>::const_iterator inputStartIter = inputPowVec.begin();
	vector<double>::const_iterator inputEndIter   = inputPowVec.begin() + numDaPerBand;

	vector<double>::iterator daVecIter  = daPowVec.begin();
	vector<double>::iterator dpdVecIter = dpdPowVec.begin();

	bool exitWhileLoop = false;

	//----------------- 1) Extract DA and DPD measurements from the inputPowVec -----------------------//
	if ( p.m_MeasInfo.numContiguousDPD_Meas > 0 )   // indicates both DA and DPD cal were performed or DPD characterization
   {
      while (1)
      {
         //---------------- copy the DA powers Per Band ---------------//
         copy( inputStartIter, inputEndIter, daVecIter );
         inputStartIter += numDaPerBand;
         if ( inputStartIter == inputPowVec.end() )
         {
            break;
         }
         inputEndIter += p.m_MeasInfo.numContiguousDPD_Meas_CB;
         daVecIter    += numDaPerBand;

         //---------------- copy the DPD powers per channel ---------------//
         for (unsigned int chanIdx=0; chanIdx<p.m_bcc.m_numChan; ++chanIdx)
         {
            // copy only up to the desired # of elements
            copy( inputStartIter, ( inputEndIter - numDpdToRemovePerChan ), dpdVecIter );
            inputStartIter += p.m_MeasInfo.numContiguousDPD_Meas_CB;
            if ( inputStartIter == inputPowVec.end() )
            {
               // this indicates a requirement to break out of the entire loop, not just the channel loop
               exitWhileLoop = true;
               break;
            }
            dpdVecIter += p.m_MeasInfo.numContiguousDPD_Meas;
            // don't execute this for the last channel
            if (chanIdx == (p.m_bcc.m_numChan - 1) )
            {
               break;
            }
            inputEndIter += p.m_MeasInfo.numContiguousDPD_Meas_CB;
         }

         if (exitWhileLoop)
         {
            break;
         }

         inputEndIter += numDaPerBand;
      }
   }
   else  // indicates only DA cal was performed
   {
      while (1)
      {
         //-------------- copy the DA powers Per Band -----------------//
         copy( inputStartIter, inputEndIter, daVecIter );
         inputStartIter += numDaPerBand;
         if ( inputStartIter == inputPowVec.end() )
         {
            break;
         }
         inputEndIter   += numDaPerBand;
         daVecIter      += numDaPerBand;
      }
   }

   //----------------- 2) Segment the Results by Chain -----------------------//
   if ( p.m_bcc.m_numTxChains == 1 )
   {
      //--- Just copy the first chain vector
      daPowMap_Chain[ p.m_bcc.m_txChainList[0] ]  = daPowVec;
      dpdPowMap_Chain[ p.m_bcc.m_txChainList[0] ] = dpdPowVec;
   }
   else
   {
      //--- Segment the 2 Tx chains

   }

   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::Do_GSM_V5_Sweep(HANDLE hPhone, GSM_CAL_Params &params,unsigned char gsm_V4_cmd[], unsigned short gsmV4CmdLen, 
	const vector<unsigned int>& chainID_Vec, bool compressed )
{
	stringstream ostr;

   unsigned short dummyChainID = 0; // dummy chain ID for setting the proper mode for the FTM command
   unsigned char status        = 1;

   //----- Step 0: Set the Mode ID
   FTM_Mode_Id_Enum gsmModeID = FTM_MODE_ID_GSM_EXTENDED_C;
   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM( hPhone,dummyChainID, gsmModeID);

   //----- Step 1: Disable predistortion for the # of chains required
   for (unsigned int chainIdx=0; chainIdx<chainID_Vec.size(); ++chainIdx)
   {
      status = QLIB_FTM_GSM_V4_DISABLE_PREDIST( hPhone, static_cast<unsigned short>(chainID_Vec[chainIdx]) );
      if(status == 0)
      {
		  return false;
         ostr.str("");
         ostr << "QLIB_FTM_GSM_V4_DISABLE_PREDIST( chainID = " << chainID_Vec[chainIdx] << " ) API failed. - "
              << __FUNCTION__;
		 RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str());
      }
   }

   //----- Step 2: Configure the GSM Frame Sweep
   if(compressed == FALSE)
   {
	   status = QLIB_FTM_GSM_V4_CAL( hPhone, (void*)gsm_V4_cmd, gsmV4CmdLen );
	   if(status == 0)
	   {
		   return false;
		   ostr.str("");
		   ostr << "QLIB_FTM_GSM_V4_SWEEP() API failed. - "
			   << __FUNCTION__;
		   RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str());
	   }
   }
   else
   {
	   status = QLIB_FTM_GSM_V4_CAL_COMPRESSED( hPhone, (void*)gsm_V4_cmd, gsmV4CmdLen );
	   if(status == 0)
	   {
		   return false;
		   ostr.str("");
		   ostr << "QLIB_FTM_GSM_V4_SWEEP() API failed. - "
			   << __FUNCTION__;
		   RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str());
	   }

   }
   //---- Step 3: Execute the GSM Frame Sweep
   status = QLIB_FTM_GSM_V4_CAL_EXECUTE_SWEEP( hPhone );
   if(status == 0)
   {
	   return false;
	   ostr.str("");
	   ostr << "QLIB_FTM_GSM_V4_CAL_EXECUTE_SWEEP() API failed. - "
		   << __FUNCTION__;
	   RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str());
   }

   return true;
}
bool RFCal::GSM_Cal::GSM_V5::DetermineIfLowBand( unsigned int uiCurBand )
{
   bool boolDoingLowBand = true;

   if ( (uiCurBand == NV_MODE_GSM_DCS) || (uiCurBand == NV_MODE_GSM_1900) )
   {
      boolDoingLowBand = false;
   }

   return boolDoingLowBand;
}
unsigned char RFCal::GSM_Cal::GSM_V5::FillCallBoxParams(HANDLE hPhone, GSM_CAL_Params & params)
{

	CGSM_V4_Cal_Params & m_gsmV4Params = params.gsm_v4_Params;
	TE_GSMV4_Sweep_Params& swpParams = params.gsm_v4_Params.swpParams;

	swpParams.bandVec = m_gsmV4Params.m_bcc.m_bandList;
	swpParams.chanVec = m_gsmV4Params.m_bcc.m_chanList;

	swpParams.performDaCal            = m_gsmV4Params.m_DA_Params.performDaCal != 0;
	swpParams.performDpdCal           = m_gsmV4Params.m_DPD_Params.performDpdCal || m_gsmV4Params.m_DPD_Params.doRGIRunOnly;
	swpParams.performRxCal            = m_gsmV4Params.m_Rx_Params.performRxCal != 0;
	swpParams.performHiLinRxCal       = m_gsmV4Params.m_Rx_Params.performHiLinRxCal;
	swpParams.performNonCharRxOnlyCal = false;

	swpParams.dpdMod  = 0;  // 8PSK

	swpParams.intraBandFrameDelay = GSM_V4_INTRA_BAND_FRAME_DELAY;

	if ( swpParams.performDaCal )
	{
		for (unsigned char idx=0; idx<m_gsmV4Params.m_DA_Params.numMods; ++idx)
		{
			swpParams.modList.push_back( ( (m_gsmV4Params.m_DA_Params.modIdxList[idx] <= gsmV4_GMSK_MCS_Idx) ? 0 : 1 ) );
		}
	}

	swpParams.numTotalFrames = static_cast<unsigned short>( m_gsmV4Params.m_numFramesForCB );

	swpParams.numBand        = static_cast<unsigned short>( m_gsmV4Params.m_bcc.m_numBands );
	swpParams.numChanPerBand = static_cast<unsigned short>( m_gsmV4Params.m_bcc.m_numChan );

	swpParams.numDaFramesPerChan  = static_cast<unsigned short>( m_gsmV4Params.m_numDaFramesPerChan );
	swpParams.numDpdFramesPerChan = static_cast<unsigned short>( m_gsmV4Params.m_numDpdFramesPerChan );

	swpParams.rxPowerList   = m_gsmV4Params.m_Rx_Params.lnaPowerListAllMode;
	swpParams.rxSlotOffset  = m_gsmV4Params.m_rxSlotOffset;

	swpParams.segregateDaAndDpdPerChan = true;
	swpParams.txSlotShiftAmt           = m_gsmV4Params.m_txShiftAmt;
	swpParams.slotConfig               = m_gsmV4Params.m_TxSlotConfig_CB;

	return true;

}

unsigned char RFCal::GSM_Cal::GSM_V5::Create_GSM_V4_Cal_Command( HANDLE hPhone, GSM_CAL_Params & params, unsigned char gsmV4CmdBuff[], unsigned short& gsmV4CmdSize,
                                                    map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER )
{
	CGSM_V4_Cal_Params &p = params.gsm_v4_Params;
	stringstream ostr;

   // pointer to gsmV4CmdBuff
   unsigned char* curPtr_gsmV4CmdBuff = gsmV4CmdBuff;

   // ensure gsmV4CmdSize is 0 initially
   gsmV4CmdSize = 0;

   // filling the GSM_V4_Cal_Params_Type header
   GSM_V4_Cal_Config_Header cmdCfgHeader;
   cmdCfgHeader.numFrames = static_cast<unsigned short>( p.m_numFrames );
   cmdCfgHeader.numChains = static_cast<unsigned short>( p.m_bcc.m_numTxChains );
   cmdCfgHeader.reserved  = 0;

   memset(cmdCfgHeader.a_devId,0,sizeof(cmdCfgHeader.a_devId)); // Initialization

   for (unsigned char i=0; i<p.m_bcc.m_numTxChains; ++i)
   {
      cmdCfgHeader.a_devId[i] = static_cast<unsigned char>( p.m_bcc.m_txChainList[i] );  // add device (chain ID) list
   }

   // copy the GSM_V4_Cal_Config_Header to the gsmV4CmdBuff
   memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &cmdCfgHeader, sizeof(GSM_V4_Cal_Config_Header) );
   curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Cal_Config_Header);
   gsmV4CmdSize += sizeof(GSM_V4_Cal_Config_Header);

   // # of slots used per frame
   const unsigned char  numDaCalSlots  = static_cast<unsigned char>( p.m_rxNumSlots * p.m_bcc.m_numRxChains + p.m_txNumSlots * p.m_bcc.m_numTxChains );
   unsigned char  numDpdCalSlots = 0;
   if( p.m_Rx_Params.performRxCal && p.m_Rx_Params.performHiLinRxCal )
   {
      //Do RX on DPD frames only when high linearity is enabled
      numDpdCalSlots = static_cast<unsigned char>( p.m_rxNumSlots * p.m_bcc.m_numRxChains + p.m_txNumSlots * p.m_bcc.m_numTxChains );
   }
   else
   {
      numDpdCalSlots = static_cast<unsigned char>( p.m_txNumSlots * p.m_bcc.m_numTxChains );
   }

   // 8 slots per TDMA frame
   const unsigned char  numSlotsPerFrame = GSM_V4_MAX_NUM_OF_SLOTS;

   // DPD table settings - these are constant
   const unsigned char dpdPaState = static_cast<unsigned char>( p.m_DPD_Params.calPaRange );   // PA range - nominally is PA0, but is user input dependent
   const unsigned char dpdModType = static_cast<unsigned char>( p.m_DPD_Params.dpdModType );   // 0 - 8PSK, 1 - GMSK

   unsigned char frameSlotIdx  = 0;   // the frame slot index, 0 to 7
   unsigned char curPclIdx     = 0;   // the current PCL index
   unsigned char curLnaIdx     = 0;   // the current LNA index
   unsigned char curChanIdx    = 0;   // the current channel index
   unsigned char curBandIdx    = 0;   // the current band index
   unsigned char dpdIdx        = 0;   // the DPD container index
   unsigned short frameIdx     = 0;   // the current frame index

   // the size of the DPD sweep containers
   unsigned char dpdSweepSize = static_cast<unsigned char>( p.m_DPD_Params.numDpd_Rgi * p.m_DPD_Params.numDpd_DigGain );

   unsigned short pclSweepFrameMarker = static_cast<unsigned short>(p.m_numDaFramesPerChan);
   unsigned short dpdSweepFrameMarker = static_cast<unsigned short>(p.m_numDpdFramesPerChan);

   bool doneWithFirstMod = false;   // flag indicating whether modulation 1 or 2 is to be used

   unsigned char curTxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp
   unsigned char curRxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp

   GSM_V4_Frame_Header curFrameHeader; // current frame header; values updated for each frame
   GSM_V4_Slot_Payload curSlotPayload; // current slot payload; added to each frame for each slot
   memset( &curFrameHeader, 0, sizeof(GSM_V4_Frame_Header) );
   memset( &curSlotPayload, 0, sizeof(GSM_V4_Slot_Payload) );

   unsigned short rxFrameCntr = 0;  // identifies the frames in which an Rx burst was scheduled; used for result extraction in Process_Rx_Meas_Results

   //--------------------------------------------------- The BAND LOOP -----------------------------------------------------------//
   /***
   At each channel, RX measurement is accommodated in TX frames (DA and DPD), 1 RX measurement per frame.
   Before hand, if a LNA state is to be measured at both high and low linearity modes, it is treated as 2 different states, which are added
   in the vector "m_Rx_Params.lnaStateListAllMode" as 2 individual states, and the state for the alternative linearity mode is bumped up by 128 (MSB set to 1).
   At each channel, for each LNA state (hi or lo linearity), at least 2 measurements are needed in order to average out good RSSI data.

   For example, if LNA state 1 is to be calibrated at both high and low linearity modes, 2 states, 1 and 129 (128 + 1), are in the vector
   as 2 different states. Each states needs to be measured at least twice for averaging. This means 4 frames are required to accommodate the measurement.
   */
   for (curBandIdx=0; curBandIdx<p.m_bcc.m_numBands; ++curBandIdx)
   {
      //---------------------------------------- The PCL/Rx CHANNEL LOOP ------------------------------------------------//
      if (p.m_DA_Params.performDaCal)
      {
         ostr.str("");
         ostr <<  "PCL/Rx Channel Loop" << endl;
		 RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

         for (curChanIdx=0; curChanIdx<p.m_bcc.m_numChan; ++curChanIdx)
         {
            //reset LNA index to the first LNA index for the current channel
            curLnaIdx = 0;

            // Fill DA/Rx sweep
            while (frameIdx < pclSweepFrameMarker)
            {
               //------------------------------ fill the frame header ------------------------------//
               curFrameHeader.band    = static_cast<unsigned char>( p.m_bcc.m_bandList[curBandIdx] );
               curFrameHeader.channel = static_cast<unsigned short>( p.m_bcc.m_chanList[curBandIdx][curChanIdx] );
               curFrameHeader.numSlotsInPayload  = numDaCalSlots;
               // copy the frame header to the GSM V4 CMD payload
               memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
               curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Frame_Header);
               gsmV4CmdSize += sizeof(GSM_V4_Frame_Header);

               //Print frame header to debug window
               ostr.str("");
               ostr << "Frame Index: " << frameIdx << endl;
               ostr << "GSM Frame Header" << endl;
               ostr << "Band::Channel::NumSlotsInPayload " << static_cast<unsigned short>(curFrameHeader.band) << "::"
                    << curFrameHeader.channel << "::"
                    << static_cast<unsigned short>(curFrameHeader.numSlotsInPayload);
               RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
			  

               //------------- loop through slot configuration: m_frameSlotConfig --------------//
               for ( frameSlotIdx=0; frameSlotIdx<numSlotsPerFrame; ++frameSlotIdx )
               {
                  curTxSlotFunc = p.m_TxSlotConfig[frameSlotIdx];
                  curRxSlotFunc = p.m_RxSlotConfig[frameSlotIdx];

                  //--------- RX SLOTS -------//
                  if ( curRxSlotFunc > 0 && p.m_Rx_Params.performRxCal )
                  {
                     // ---------- Add chainID[0] slot
                     // fill the slot header
                     curSlotPayload.devID    = cmdCfgHeader.a_devId[0];
                     curSlotPayload.slotNum  = frameSlotIdx;
                     curSlotPayload.slotOpID = Rx_RSSI_Meas_Op;
                     //Note: for high linearity, the MSB of LNA state byte in m_Rx_Params.lnaStateListAllMode is already set to 1.
                     curSlotPayload.slotPayload.rssiPayload.lnaState = static_cast<unsigned char>( p.m_Rx_Params.lnaStateListAllMode[curLnaIdx] );

                     // copy the slot payload over
                     memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                     gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                     //Print chainID[0] slot to debug window
                     ostr.str("");
                     ostr << "Rx Slot " << endl;
                     ostr << "DevID::SlotNum::SlotOpID::LNAState "
                          << static_cast<unsigned short>(curSlotPayload.devID) << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotNum) << "::"
                          << "Rx_RSSI_Meas_Op" << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotPayload.rssiPayload.lnaState);
                     RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

                     // ---------- Add chainID[1] slot
                     if (p.m_bcc.m_numRxChains > 1)
                     {
                        // fill the slot header
                        curSlotPayload.devID = cmdCfgHeader.a_devId[1];
                        // copy the slot payload over
                        memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                        curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                        gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                        //Print chainID[1] slot to debug window
                        ostr.str("");
                        ostr << "chainID[1] Rx Slot " << endl;
                        ostr << "devID:: " << static_cast<unsigned short>(curSlotPayload.devID);
                        RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
                     }

                     RX_BURST_FRAME_IDENTIFIER[curFrameHeader.band].push_back( rxFrameCntr );

                     // increment to the next LNA state; reset the LNA index if limit is reached.
                     ++curLnaIdx;
                     if ( curLnaIdx >= p.m_Rx_Params.lnaStateListAllMode.size() )
                     {
                        curLnaIdx = 0;
                     }
                  }

                  //--------- TX SLOTS --------//
                  if ( curTxSlotFunc > 0 )
                  {
                     // ---------- Add chainID[0] slot
                     // fill the slot header
                     curSlotPayload.devID    = cmdCfgHeader.a_devId[0];
                     curSlotPayload.slotNum  = frameSlotIdx;
                     curSlotPayload.slotOpID = Tx_PCL_Meas_Op;
                     // add the modulation
                     curSlotPayload.slotPayload.pclPayload.modType = static_cast<unsigned char>( p.m_DA_Params.modIdxList[0] );    // modType[0] - from tree
                     if ( doneWithFirstMod )
                     {
                        curSlotPayload.slotPayload.pclPayload.modType = static_cast<unsigned char>( p.m_DA_Params.modIdxList[1] ); // modType[1] - from tree
                     }
                     curSlotPayload.slotPayload.pclPayload.PCL_Index = static_cast<unsigned char>( p.m_DA_Params.pclIdxList[curBandIdx][curPclIdx] );
                     // copy the slot payload over
                     memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                     gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                     //Print chainID[0] slot to debug window
                     ostr.str("");
                     ostr << "Tx Slot " << endl;
                     ostr << "DevID::SlotNum::SlotOpID::ModType::PCLIndex "
                          << static_cast<unsigned short>(curSlotPayload.devID) << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotNum) << "::"
                          << "Tx_PCL_Meas_Op" << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotPayload.pclPayload.modType) << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotPayload.pclPayload.PCL_Index);
                     RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

                     // ---------- Add chainID[1] slot
                     if (p.m_bcc.m_numTxChains > 1)
                     {
                        // fill the slot header - only device ID needs to be changed
                        curSlotPayload.devID = cmdCfgHeader.a_devId[1];
                        // copy the slot payload over
                        memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                        curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                        gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                        //Print chainID[1] slot to debug window
                        ostr.str("");
                        ostr << "chainID[1] Tx Slot " << endl;
                        ostr << "devID:: " << static_cast<unsigned short>(curSlotPayload.devID);
                        RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
                     }

                     // increment to the next PCL
                     ++curPclIdx;
                  }

                  // reset the PCL index and modulation scheme if limit is reached
                  if ( curPclIdx >= p.m_DA_Params.numPCLs )
                  {
                     curPclIdx = 0;
                     if ( p.m_DA_Params.numMods > 1 )
                     {
                        doneWithFirstMod = !doneWithFirstMod;  // toggle the modulation scheme
                     }
                  }

               }  // END FRAME-SLOT LOOP

               // Increment the frameIdx
               ++frameIdx;

               // Increment the rxFrameCntr
               ++rxFrameCntr;

            }  // END DA/RX SWEEP LOOP

            // reset frameIdx for the next channel
            frameIdx = 0;
         }  // END PCL/Rx CHANNEL LOOP

      }  // END DA Cal Sweep

      /***
      At each channel, RX measurement is accommodated in TX frames (DA and DPD), 1 RX per frame.
      Depending on the number of LNA states, the states may not be evenly accommodated in the DA frames.
      So in the following DPD section, accommodation starts at the states left over from DA section.
      */
      unsigned char lnaIdxToStartWith = curLnaIdx;

      if (p.m_DPD_Params.performDpdCal || p.m_DPD_Params.doRGIRunOnly)
      {
         // reset frameIdx
         frameIdx = 0;

         ostr.str("");
         ostr << "DPD Channel Loop" << endl;
         RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

         //---------------------------------------- The DPD CHANNEL LOOP ------------------------------------------------//
         for (curChanIdx=0; curChanIdx<p.m_bcc.m_numChan; ++curChanIdx)
         {
            //reset LNA index to the starting index left over from DA accommodation for each channel.
            curLnaIdx = lnaIdxToStartWith;

            // Fill DPD Sweep
            while (frameIdx < dpdSweepFrameMarker)
            {
               //------------------------------ fill the frame header ------------------------------//
               curFrameHeader.band    = static_cast<unsigned char>( p.m_bcc.m_bandList[curBandIdx] );
               curFrameHeader.channel = static_cast<unsigned short>( p.m_bcc.m_chanList[curBandIdx][curChanIdx] );
               curFrameHeader.numSlotsInPayload  = numDpdCalSlots;
               // copy the frame header to the GSM V4 CMD payload
               memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
               curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Frame_Header);
               gsmV4CmdSize += sizeof(GSM_V4_Frame_Header);

               //Print frame header to debug window
               ostr.str("");
               ostr << "Frame Index: " << frameIdx << endl;
               ostr << " GSM Frame Header " << endl;
               ostr << "Band::Channel::NumSlotsInPayload " << static_cast<unsigned short>(curFrameHeader.band) << "::"
                    << curFrameHeader.channel << "::"
                    << static_cast<unsigned short>(curFrameHeader.numSlotsInPayload);
               RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

               //------------- loop through slot configuration: m_frameSlotConfig --------------//
               for ( frameSlotIdx=0; frameSlotIdx<numSlotsPerFrame; ++frameSlotIdx )
               {
                  curTxSlotFunc = p.m_TxSlotConfig[frameSlotIdx];
                  curRxSlotFunc = p.m_RxSlotConfig[frameSlotIdx];

                  //--------- RX SLOTS -------//
                  if ( curRxSlotFunc > 0 && p.m_Rx_Params.performRxCal && p.m_Rx_Params.performHiLinRxCal )
                  {
                     // ---------- Add chainID[0] slot
                     // fill the slot header
                     curSlotPayload.devID    = cmdCfgHeader.a_devId[0];
                     curSlotPayload.slotNum  = frameSlotIdx;
                     curSlotPayload.slotOpID = Rx_RSSI_Meas_Op;
                     //Note: for high linearity, the MSB of LNA state byte in m_Rx_Params.lnaStateListAllMode is already set to 1.
                     curSlotPayload.slotPayload.rssiPayload.lnaState = static_cast<unsigned char>( p.m_Rx_Params.lnaStateListAllMode[curLnaIdx] );

                     // copy the slot payload over
                     memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                     gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                     //Print chainID[0] slot to debug window
                     ostr.str("");
                     ostr << "Rx Slot " << endl;
                     ostr << "DevID::SlotNum::SlotOpID::LNAState "
                          << static_cast<unsigned short>(curSlotPayload.devID) << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotNum) << "::"
                          << "Rx_RSSI_Meas_Op" << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotPayload.rssiPayload.lnaState);
                     RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

                     // ---------- Add chainID[1] slot
                     if (p.m_bcc.m_numRxChains > 1)
                     {
                        // fill the slot header
                        curSlotPayload.devID = cmdCfgHeader.a_devId[1];
                        // copy the slot payload over
                        memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                        curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                        gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                        //Print chainID[1] slot to debug window
                        ostr.str("");
                        ostr << "chainID[1] Rx Slot " << endl;
                        ostr << "devID:: " << static_cast<unsigned short>(curSlotPayload.devID);
                        RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
                     }

                     RX_BURST_FRAME_IDENTIFIER[curFrameHeader.band].push_back( rxFrameCntr );

                     // increment to the next LNA state; reset the LNA index if limit is reached.
                     ++curLnaIdx;
                     if ( curLnaIdx >= p.m_Rx_Params.lnaStateListAllMode.size() )
                     {
                        curLnaIdx = 0;
                     }
                  }

                  // ADD TX SLOTS
                  if ( curTxSlotFunc > 0 )
                  {
                     // ---------- Add chainID[0] slot
                     // fill the slot header
                     curSlotPayload.devID                           = cmdCfgHeader.a_devId[0];
                     curSlotPayload.slotNum                         = frameSlotIdx;
                     curSlotPayload.slotOpID                        = Tx_RGI_Meas_Op;
                     // fill the slot payload
                     curSlotPayload.slotPayload.rgiPayload.RGI      = p.m_dpd_RGI_MapVec[ p.m_bcc.m_bandList[curBandIdx] ][dpdIdx];
                     curSlotPayload.slotPayload.rgiPayload.envScale = p.m_dpd_digGain_MapVec[ p.m_bcc.m_bandList[curBandIdx] ][dpdIdx];
                     curSlotPayload.slotPayload.rgiPayload.paState  = dpdPaState;
                     curSlotPayload.slotPayload.rgiPayload.modType  = dpdModType;

                     // copy the slot payload over
                     memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                     gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                     //Print chainID[0] slot to debug window
                     ostr.str("");
                     ostr << "Tx Slot " << endl;
                     ostr << "DevID::SlotNum::SlotOpID::RGI::EnvScale::PAState::ModType "
                          << static_cast<unsigned short>(curSlotPayload.devID) << "::" << static_cast<unsigned short>(curSlotPayload.slotNum) << "::"
                          << "Tx_RGI_Meas_Op" << "::" << static_cast<unsigned short>(curSlotPayload.slotPayload.rgiPayload.RGI) << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotPayload.rgiPayload.envScale) << "::" << static_cast<unsigned short>(curSlotPayload.slotPayload.rgiPayload.paState) << "::"
                          << static_cast<unsigned short>(curSlotPayload.slotPayload.rgiPayload.modType);
                     RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());

                     // ---------- Add chainID[1] slot
                     if (p.m_bcc.m_numTxChains > 1)
                     {
                        // fill the slot header - only device ID requires changing
                        curSlotPayload.devID = cmdCfgHeader.a_devId[1];

                        // copy the slot payload over
                        memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                        curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                        gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                        //Print chainID[1] slot to debug window
                        ostr.str("");
                        ostr << "chainID[1] Tx Slot " << endl;
                        ostr << "devID:: " << static_cast<unsigned short>(curSlotPayload.devID);
                        RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
                     }

                     // increment the DPD sweep container index
                     ++dpdIdx;
                     if (dpdIdx >= dpdSweepSize)
                     {
                        dpdIdx = 0;
                     }

                  }  // END Active Slots

               }  // END FRAME-SLOT LOOP

               // Increment the frameIdx
               ++frameIdx;

               // Increment the rxFrameCntr
               ++rxFrameCntr;

            } // END DPD SWEEP LOOP

            // reset frameIdx
            frameIdx = 0;

            // reset the dpd sweep container index for each channel
            dpdIdx = 0;

         }  // END DPD CHANNEL LOOP
      }  // END DPD BOOLEAN

      // reset the dpd sweep container index for each band too
      dpdIdx = 0;

   }  // END BAND LOOP
 
   return true;
}
unsigned char RFCal::GSM_Cal::DO_GSM_FBRX_CAL(HANDLE hPhone)
{
   GSM_CAL_Results results;
   GSM_CAL_Params params;
   deviceMutex = CreateMutex( NULL, FALSE, NULL); // Create mutex. This must be created outside this funciton. For demo purpose, created here.
   AssignFBRxParams(hPhone, params);

   RFCal::Helper::Log::StartQMSLLogging(hPhone, params.FilePath.stQMSLLog);
 
   // Set equipment
   RFCal::DeviceDB::GetInstance().RegisterDevice (hPhone, params.DeviceConfig.type, params.DeviceConfig.boardId, params.DeviceConfig.primaryAddr, params.DeviceConfig.secondaryAddr );
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   
   if ( NULL == pMgr ) return false;

   pMgr->Set_RFMode ( params.rf_mode );
   pMgr->Set_ULChannel(static_cast<int>(params.channel));
   pMgr->Set_SimulationMode(false);
   pMgr->Get_Version();
   RFCal::SetPathLoss(hPhone, (*pMgr), params.FilePath.stCalDBXML,"GSM");
   pMgr->Set_OutputPort( false );
   // Set RF Mode (Query RFC if necessary) 
   RFCal::GSM_Cal::INIT_FBRX(hPhone, params);

   // DO FBRx CAL
   if (params.gsm_v4_Params.m_FBRx_Params.enableFbrxGainCal)
   {
      RFCal::GSM_Cal::FBRx_Cal::Do_FBRx_Cal(hPhone, params, results);
   }


  //----------------- NV Processing for Chain Enumerated NV ------------//
   if( params.txWriteChainEnumeratedNv || params.writeConsolidatedTxNvVersion )
   {
     // QMSL_GSM_Tx_Calibration_NV gsmTxCalNv;
      QMSL_GSM_Tx_Calibration_Consolidated_NV gsmTxCalConsolidatedNV;
      gsmTxCalConsolidatedNV.iDatatypeVer = params.writeConsolidatedTxNvVersion;

      bool logDaCalNV = true; //((params.daCalParams.nv_action & RFCAL_NV_ACTION_ENABLE_LOG) != 0);

      //----------------------- Version 2.0 predistortion calibration ---------------------//
      if( params.performPreDistCal )
      {
         if(params.writeConsolidatedTxNvVersion)
         {

            if ( RFCal::GSM_Cal::Process_TxNV::Consolidated_NV_From_DA_and_IQ_Predistortion_Cal(hPhone, params, results, gsmTxCalConsolidatedNV) )
            {
	            RFCal::Helper::NV::WriteCalRFNVToQCN(hPhone,params.FilePath.stCalibratedXQCN, false);
            }
            else
            {
               RFCal::Helper::Log::StopQMSLLogging( hPhone );
               return false;   
            }
         }
      }
   }
   // Success
   RFCal::Helper::Log::StopQMSLLogging( hPhone );
   CloseHandle( deviceMutex ); // CloseHendle for the deviceMutex must be called outside this function. For demo purpose, called here.
 
   return true;
}

unsigned char RFCal::GSM_Cal::LOG::Rx_FreqComp ( HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   
   map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator itr;

//   TECHNOLOGY_FAMILY_TYPE technology = CBandHelper::GetTechnologyFamily( band );

  

   for ( map< int, map< int, vector<GSM_RX_MEAS_TYPE> > >::const_iterator path_itr = results.rxPath_chan_rssis_list.begin();
      path_itr != results.rxPath_chan_rssis_list.end(); path_itr++)
   {
       stringstream stTitle;
       if ( path_itr->first == 0 || path_itr->first == 2)
          stTitle <<  "Logging GSM Rx FreqComp Measurements (PRx / ";
       else
           stTitle <<  "Logging GSM Rx FreqComp Measurements (DRx /";

       stTitle << " RxPath " <<  path_itr->first << " )";
       RFCal::Helper::Log::AddTitleToQMSLLog( hPhone, stTitle.str() );

       for(itr = path_itr->second.begin(); itr != path_itr->second.end(); ++itr)
       {
         for(UINT gain_index=0; gain_index<itr->second.size(); ++gain_index)
         {
            if(params.rxParams.rx_gain_ranges.size() != itr->second.size())
            {
               return false;
               //ostr.str("");
               //ostr<<"The number of expected to actual measured rx gain ranges differs."
               //    <<" There are "<<params.m_rx_gain_ranges.size()<<" and "
               //    <<itr->second.size()<<" respectively. "<<__FUNCTION__;
               //throw CApplicationException( g_exc_general_exception, ostr.str() );
            }

          //  string curBand = CBandHelper::BandTypeToBandName( band );


            double rx_level = params.rxParams.rx_levels[gain_index];
            int rssi = itr->second[ gain_index ].m_rssi;
          
            double val = 16 * ( 10*log10( rssi/1.0 ) - rx_level);
         
            int rx_gain = static_cast<int>( RFCal::Helper::Convert::ROUND( val, 0 ) );//(int)(floor( val + 0.5 ));

            {
               stringstream ss;
               ss.str("");
               ss  <<"Ch: "     << itr->first
                   <<", Lvl: "  << rx_level
                   <<", GR: " << params.rxParams.rx_gain_ranges[gain_index]

                   << ", Rssi: " << rssi 
                   << ", Gain: "<<  rx_gain
                   <<" (Min "    << params.rxParams.rx_gain_offsets_min_lims[gain_index]
                   <<", Max "    << params.rxParams.rx_gain_offsets_max_lims[gain_index]
                   << ")";
                
                 /*  <<", Band "   << curBand */
               RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ss.str() );

            }
         }
      }
   }
   return true;
}
unsigned char RFCal::GSM_Cal::Process_TxNV::GetEDGELinearTxGain(HANDLE hPhone, unsigned int nvMode, unsigned char chainID, unsigned char useFTMCmd, short & envGain)
{
   envGain             = 0;
   unsigned short nvID       = 0;
   unsigned char nvArr[128];
   unsigned short nvSize     = 128;
   unsigned short readStatus = 0;
   bool bResult              = true;

   switch(nvMode)
   {
      case NV_MODE_GSM_EGSM:
         nvID = 5478; // NV_EDGE_LINEAR_TX_GAIN_PARAM_I;
         break;
      case NV_MODE_GSM_850:
         nvID = 6475; // NV_EDGE_850_LINEAR_TX_GAIN_PARAM_I;
         break;
      case NV_MODE_GSM_DCS:
         nvID = 6476; // NV_EDGE_1800_LINEAR_TX_GAIN_PARAM_I;
         break;
      case NV_MODE_GSM_1900:
         nvID = NV_EDGE_1900_LINEAR_TX_GAIN_PARAM_I;
         break;
   }
   if(false == useFTMCmd)  // Read NVs directly
   {
      QLIB_DIAG_NV_READ_F( hPhone, nvID, nvArr, static_cast<int>(nvSize), &readStatus);
      envGain = *(short *)(&nvArr[0] );
   }
   else  // use the FTM API Readback Method to read NVs
   {
      // sets the mode ID - default to C0
      // function handles properly setting FTM_MODE_ID_GSM_EXTENDED_CX
      RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, chainID, FTM_MODE_ID_GSM_EXTENDED_C);

      unsigned short retType = 2;   // nvArr[0] points to EDGE envGain

      bResult = ( QLIB_FTM_GSM_API_ReadEnvGainNV( hPhone, nvID, nvArr, &nvSize, &readStatus, retType ) != 0 );

      // combine the 2 bytes for envGain. They are in LSByte order
      envGain = nvArr[0] + (nvArr[1] << 8);
   }

   // verify that the envGain is a reasonable value
   // A bad result for envGain can be returned despite bResult and readStatus indicating a successful read.
   const short maxEnvGain = 100; // 93 is the general maximum value

   if ( readStatus != 0 || !bResult || (envGain > maxEnvGain) )
   {
      //ostr.str("");
      //ostr << "NV Read of EDGE Digital Gain Failed. " << __FUNCTION__;
      bResult = false;
   }

   return bResult;
}
unsigned char RFCal::GSM_Cal::Process_TxNV::EDGE_PreDist_Cal_NV_Intermediate(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results,   
                                                         QMSL_GSM_PreDist_Cal_Result & calResult, unsigned char queue_nv_indicator_predist)
{
   PreDist_Capture_Record_Channel& calibratedPreDistData = (*results.calibratedPreDistData);
   const EDGE_PreDist_Cal_Data & preDistCalParams = params.preDistCalParams;

   // Populate QMSL struct to pass to QLIB
   memcpy_s(calResult.adTxAmpl[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[0].ampldArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
   memcpy_s(calResult.adTxAmpl[1], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[1].ampldArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
   memcpy_s(calResult.adTxAmpl[2], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[2].ampldArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
   memcpy_s(calResult.adTxPhaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[0].phaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
   memcpy_s(calResult.adTxPhaseArr[1], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[1].phaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
   memcpy_s(calResult.adTxPhaseArr[2], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float), &calibratedPreDistData.captureRecords[2].phaseArr[0], QMSL_GSM_MAX_PREDIST_SAMPLES*sizeof(float));
   float symbolrate           = 270833.0;
   calResult.iCalRgi          = (unsigned short)calibratedPreDistData.calRgi;
   calResult.iDcSamples       = (long)((preDistCalParams.preDcDuration/4)*(calibratedPreDistData.samplingRateHz/symbolrate));
   calResult.iEdgeSamples     = (long)((preDistCalParams.edgeDuration/4)*(calibratedPreDistData.samplingRateHz/symbolrate));
   calResult.iFreqMapping[0]                  = (QMSL_GSM_Polar_Cal_Frequency)preDistCalParams.nvMappings[0];
   calResult.iFreqMapping[1]                  = (QMSL_GSM_Polar_Cal_Frequency)preDistCalParams.nvMappings[1];
   calResult.iFreqMapping[2]                  = (QMSL_GSM_Polar_Cal_Frequency)preDistCalParams.nvMappings[2];
   calResult.iNoiseSamples                    = 500;       // Todo: Get this from configured on the call box.
   calResult.iNumChannels                     = preDistCalParams.channel_list.size();
   calResult.iNumPredistortionWaveformSamples = (unsigned short)calibratedPreDistData.numSamples;
   calResult.iSamplingRateHz                  = calibratedPreDistData.samplingRateHz;
   calResult.iDCTransientPercent              = (float)preDistCalParams.dcSkipPercentage;
   calResult.iEDGETransientSymbols            = (unsigned short)preDistCalParams.edgeSkipSymbols;

   //Modem Settings
   if(preDistCalParams.overRideModemConsts)
   {
      calResult.iOverRideModemConstants = 1;
      calResult.dDigGainUnity    = (float)preDistCalParams.digGainUnity;
      calResult.dRampUnity       = (float)preDistCalParams.rampUnity;
      calResult.dDacScale        = (float)preDistCalParams.dacScale;
      calResult.dExtensionFloor  = (float)preDistCalParams.extensionFloor;
   }
   //This case only comes into play when two different targets are run without QSPR closing.
   //Need to handle it anyways.
   else
   {
      calResult.iOverRideModemConstants = 0;
   }

   if(preDistCalParams.overRideSmoothingDefaults)
   {
      calResult.iOverrideSmooothingDefaults = 1;
      calResult.iEnableAmAmSmoothing = (preDistCalParams.enableAmAmSmoothing)?1:0;
      calResult.dAmAmKnot = (float)preDistCalParams.amamKnotSize;
      calResult.dAmPmKnot = (float)preDistCalParams.ampmKnotSize;
      //m_pUIM->PrintToDebug("Smoothing Override Enabled");
      /*
      ostringstream temp;
      temp << "Overides. AMAM Smoothing :" << ((preDistCalParams.m_enableAmAmSmoothing)?"Enabled":"Disabled")
           << ", AMAM Knot :" << calResult.dAmAmKnot
           << ", AMPM Knot :" << calResult.dAmPmKnot;
      m_pUIM->PrintToDebug(temp.str());
      */
   }

   short txGainParam = 0;
   if (  GetEDGELinearTxGain(hPhone, params.m_nv_mode, static_cast < unsigned char > (params.chainID),1, txGainParam) )
   {
      calResult.iEDGETxGainParam = txGainParam;
      calResult.iEDGETxCalScale = (unsigned short)calibratedPreDistData.calScaleSetting;

      queue_nv_indicator_predist = (preDistCalParams.m_nv_action & 0 /* RFCAL_NV_ACTION_QUEUE_NV */) ? 1 : 0;
      return true;
   }
   else
   {
      return false;
   }
}

unsigned char RFCal::GSM_Cal::Process_TxNV::DA_Cal_NV_Intermediate( HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results, QMSL_GSM_DA_Cal_Result * calResultsToQmsl,
                                                    unsigned char& queue_nv_indicator )
{
   calResultsToQmsl->iNumMeasuredTxGainStates = static_cast<unsigned long>(params.daCalParams.paRanges.size());
   calResultsToQmsl->iNumChannels = params.daCalParams.channel_list.size();
   calResultsToQmsl->iNumTxPowerMeasurementsPerRange = params.daCalParams.numStepsPerPARange;
   calResultsToQmsl->iSaturationDetectionWindow = params.daCalParams.saturationDetectionWindow;
   unsigned short chan;

   //Walk through Channels
   for(unsigned long chanLoopVar = 0; chanLoopVar < params.daCalParams.channel_list.size(); ++chanLoopVar)
   {
      chan = static_cast <unsigned short>(params.daCalParams.channel_list[chanLoopVar]);

      //Populate struct to send to qmsl
      calResultsToQmsl->iChannel[chanLoopVar] = chan;

      calResultsToQmsl->iFreqMapping[chanLoopVar] = (QMSL_GSM_Polar_Cal_Frequency)params.daCalParams.nvMappings[chanLoopVar];

      map<unsigned short,double> rgiPwrTemp;
      map<unsigned short,double>::iterator rgiIter;

      unsigned long paLoopIdx = 0;
      unsigned short paState;
      unsigned long rgiLoopIdx;

      GSM_DA_Cal_Results currentChanCalData = results.da_cal_res.channelSweepData[chan];

      //Walk through PA values
      for(; paLoopIdx< params.daCalParams.paRanges.size(); ++paLoopIdx)
      {
         paState = params.daCalParams.paRanges[paLoopIdx];
         calResultsToQmsl->aiTxMeasToNVMapping[paLoopIdx] = paState;

         GSM_DA_Cal_Sweep_Record gsmSweep = currentChanCalData.gsmDaSweep[paState];
         GSM_DA_Cal_Sweep_Record edgeSweep = currentChanCalData.edgeDaSweep[paState];

         rgiLoopIdx = 0;
         for(rgiIter = gsmSweep.rgiPwr.begin(); rgiIter != gsmSweep.rgiPwr.end(); ++rgiIter)
         {
            calResultsToQmsl->aiGSMTxRGI[chanLoopVar][paLoopIdx][rgiLoopIdx] = rgiIter->first;
            calResultsToQmsl->aiGSMTxPowerMeas[chanLoopVar][paLoopIdx][rgiLoopIdx] = rgiIter->second;
            ++rgiLoopIdx;
         }

         rgiLoopIdx = 0;
         for(rgiIter = edgeSweep.rgiPwr.begin(); rgiIter != edgeSweep.rgiPwr.end(); ++rgiIter)
         {
            calResultsToQmsl->aiEDGETxRGI[chanLoopVar][paLoopIdx][rgiLoopIdx] = rgiIter->first;
            calResultsToQmsl->aiEDGETxPowerMeas[chanLoopVar][paLoopIdx][rgiLoopIdx] = rgiIter->second;
            ++rgiLoopIdx;
         }
      }
   }

   queue_nv_indicator = (params.daCalParams.nv_action & 0x1 /* RFCAL_NV_ACTION_QUEUE_NV */) ? 1 : 0;

   return true;
}
unsigned char RFCal::GSM_Cal::Process_RxNV::Rx_Cal_NV_Consolidated(HANDLE hPhone,  const GSM_CAL_Params & params, GSM_CAL_Results & results, 
   std::vector<QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated> & gsm_rx_chain_data,
   QMSL_RFNV_RX_CAL_GSM_NV & calNV)
{

   QMSL_GSM_Rx_Calibration_Params_and_Meas_Consolidated rxchain_calType_data; //params for consolidated NV

   memset((void*)(&rxchain_calType_data), 0 , sizeof(rxchain_calType_data)); //set everything to zero
   rxchain_calType_data.bReadNVFromDUT = ( params.rxParams.rx_read_nv_data_from_uut == true)? 1 : 0;
   ////iterator for array with Antenna Path, chain ID, RFM Dev, caltype, channel list, RSSI and size of array
   //map<unsigned short, map<unsigned char, map<unsigned short, map<unsigned short, map< int, vector<GSM_RX_MEAS_TYPE> > > > > >::const_iterator antPathIt = chan_rssis_list_consolidated.begin();

   ////iterating over the entire map and creating an array
   ////iterate over antenna Paths
   //while(antPathIt != chan_rssis_list_consolidated.end())
   //{
   //   unsigned short antPath = antPathIt->first;

   //   map<unsigned char, map<unsigned short, map<unsigned short, map< int, vector<GSM_RX_MEAS_TYPE> > > > >::const_iterator chainIt = antPathIt->second.begin();

   //   //iterate over chains
   //   while (chainIt != antPathIt->second.end())
   //   {
   //      unsigned char chainID = chainIt->first;

   //      map<unsigned short,  map<unsigned short, map< int, vector<GSM_RX_MEAS_TYPE> > > >::const_iterator RFMDevIt = chainIt->second.begin();

   //      //iterate of RFM Device
   //      while(RFMDevIt != chainIt->second.end())
   //      {
   //         unsigned short RFMDev = RFMDevIt->first;

   //         map<unsigned short, map< int, vector<GSM_RX_MEAS_TYPE> > >::const_iterator calTypeIt = RFMDevIt->second.begin();

   //         //iterate over calibration types
   //         while( calTypeIt != RFMDevIt->second.end() )
   //         {
   //            unsigned short calType = calTypeIt->first;

   //            map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator chanIt;
   //            unsigned short ch_index = 0;

   //            //iterate over channels
   //            for(chanIt = calTypeIt->second.begin(); chanIt != calTypeIt->second.end(); ++chanIt)
   //            {
   //               rxchain_calType_data.aiChannelList[ch_index ] = static_cast<unsigned short>(chanIt->first);

   //               for(unsigned short gain_index=0; gain_index < chanIt->second.size(); ++gain_index)
   //               {
   //                  rxchain_calType_data.aiRSSIValue[ gain_index ][ ch_index ] = chanIt->second[ gain_index ].m_rssi;

   //                  rxchain_calType_data.aiCalibrationPwr[ gain_index ] = params.rxParams.rx_levels[ gain_index ];
   //               }
   //               ++ch_index;
   //               rxchain_calType_data.iNumRxGainRange = chanIt->second.size();
   //            }

   //            //update items in the params
   //            rxchain_calType_data.iCaltype = calType;
   //            rxchain_calType_data.iRFMDevice = RFMDev;
   //            rxchain_calType_data.iNvContainer = static_cast<unsigned short>(chainID);
   //            rxchain_calType_data.iChainId = chainID;
   //            rxchain_calType_data.iAntennaPath = antPath;
   //            rxchain_calType_data.iNumChannel = calTypeIt->second.size(); //ch_index

   //            //push data into array
   //            gsm_rx_chain_data.push_back(rxchain_calType_data);

   //            ++calTypeIt;
   //         }
   //         ++RFMDevIt;
   //      }
   //      ++chainIt;
   //   }
   //   ++antPathIt;
   //}
   enum linearity
   {
      LOW_LIN,
      HIGH_LIN
   };
   
   unsigned short calType = (params.rxParams.rx_do_hi_lin)?HIGH_LIN : LOW_LIN;
   for ( map < int, map< int, vector<GSM_RX_MEAS_TYPE> > >::iterator path_itr = results.rxPath_chan_rssis_list.begin();
      path_itr != results.rxPath_chan_rssis_list.end(); path_itr++)
   {
      map< int, vector<GSM_RX_MEAS_TYPE> >::const_iterator chanIt;
      unsigned short ch_index = 0;

      //iterate over channels
      for(chanIt = path_itr->second.begin(); chanIt != path_itr->second.end(); ++chanIt)
      {
         rxchain_calType_data.aiChannelList[ch_index ] = static_cast<unsigned short>(chanIt->first);

         for(unsigned short gain_index=0; gain_index < chanIt->second.size(); ++gain_index)
         {
            rxchain_calType_data.aiRSSIValue[ gain_index ][ ch_index ] = chanIt->second[ gain_index ].m_rssi;

            rxchain_calType_data.aiCalibrationPwr[ gain_index ] = params.rxParams.rx_levels[ gain_index ];
         }
         ++ch_index;
         rxchain_calType_data.iNumRxGainRange = chanIt->second.size();
      }

      //update items in the params
      rxchain_calType_data.iCaltype = calType;

      rxchain_calType_data.iNumChannel = path_itr->second.size(); //ch_index

      if (path_itr->first == 0 || path_itr->first == 2 )
      {
         rxchain_calType_data.iRFMDevice = params.sigPaths.prx.rfm_dev;
         rxchain_calType_data.iChainId =  static_cast <unsigned char > (params.sigPaths.prx.signal_path);
         rxchain_calType_data.iAntennaPath =  params.sigPaths.prx.antenna_port;
      }
      else
      {
         rxchain_calType_data.iRFMDevice = params.sigPaths.drx.rfm_dev;
         rxchain_calType_data.iChainId =  static_cast <unsigned char > (params.sigPaths.drx.signal_path);
         rxchain_calType_data.iAntennaPath =  params.sigPaths.drx.antenna_port;
      }
      rxchain_calType_data.iNvContainer = rxchain_calType_data.iChainId;


      //push data into array
      gsm_rx_chain_data.push_back(rxchain_calType_data);



   }
   bool queue_nv_indicator = true; //params.nv_action;

   QLIB_GSM_RX_CALIBRATION_GENERATE_NV_CONSOLIDATED(
      hPhone,
      static_cast<unsigned short>(params.m_nv_mode),
      (void*)&gsm_rx_chain_data[0],
      gsm_rx_chain_data.size(),
      (void*)&calNV,
      static_cast<unsigned char>(queue_nv_indicator),
      params.writeConsolidatedRxNvVersion
   );
   return true;
}
unsigned char RFCal::GSM_Cal::Process_TxNV::Consolidated_NV_From_DA_and_IQ_Predistortion_Cal(HANDLE hPhone,  const GSM_CAL_Params & params,
            GSM_CAL_Results & results, QMSL_GSM_Tx_Calibration_Consolidated_NV & calNV)
{
   unsigned char bStatus = true;
   //------------------ DA Cal Processing ----------------//
   QMSL_GSM_DA_Cal_Result * calResultsDaCalToQmsl = new QMSL_GSM_DA_Cal_Result();
   if ( !calResultsDaCalToQmsl)
   {
      return false;
   }
   unsigned char queue_nv_indicator_da = 0x0;

   DA_Cal_NV_Intermediate (hPhone, params, results, calResultsDaCalToQmsl, queue_nv_indicator_da);

 
   //------------------ PreDist Cal Processing ----------------//
   QMSL_GSM_PreDist_Cal_Result * calResultPreDistToQMSL = new QMSL_GSM_PreDist_Cal_Result();
   if ( ! calResultPreDistToQMSL )
   {
      delete calResultsDaCalToQmsl;
      return false;
   }
   unsigned char queue_nv_indicator_predist = 0x0;

   if( params.performPreDistCal )
   {
      EDGE_PreDist_Cal_NV_Intermediate( hPhone, params, results, *calResultPreDistToQMSL, queue_nv_indicator_predist );
      calResultPreDistToQMSL->iTxNvContainer = static_cast<unsigned char>(params.m_txNvContainer);
  
   }
   else
   {
      /*  set number of channels calibrated to 0 */
      calResultPreDistToQMSL->iNumChannels =static_cast<unsigned char>( 0 );
   }

   calResultsDaCalToQmsl->iTxNvContainer  = static_cast<unsigned char>(params.m_txNvContainer);
   calResultsDaCalToQmsl->iTxSigPath = static_cast<unsigned char>(params.sigPaths.tx.signal_path);
   calNV.iDatatypeVer = 2;
   try
   {
   QLIB_RFCal_GSM_Tx_DA_Cal_IQ_Predist_Cal_Generate_Consolidated_NV(
      hPhone,
      static_cast<unsigned short>(params.m_nv_mode),
      static_cast<unsigned char>(params.sigPaths.tx.antenna_port),
      static_cast<unsigned char>(params.sigPaths.tx.rfm_dev),
      (void*) calResultsDaCalToQmsl,
      (void*) calResultPreDistToQMSL,
      (void*)& calNV,
      true /* queue_nv_indicator_da */);
   } 
   catch (...)
   {
      // Check data and NV mode.
      bStatus = false;
   }

   if ( 0 == calNV.iHasValidData )
   {
      // NV generation failure
      bStatus = false;
   }

   if ( calResultsDaCalToQmsl )
   {
      delete calResultsDaCalToQmsl;
      calResultsDaCalToQmsl = NULL;
   }
   if ( calResultPreDistToQMSL )
   {
      delete calResultPreDistToQMSL;
      calResultPreDistToQMSL = NULL;
   }

   return bStatus;
}
unsigned char  RFCal::GSM_Cal::AssignCommonParams( HANDLE hPhone, GSM_CAL_Params & params)
{
   params.query_path_from_rfc = true;  /// For Atlas Cal, set true

   params.DeviceConfig.boardId = 0;
   params.DeviceConfig.primaryAddr = 20;
   params.DeviceConfig.secondaryAddr = 0;
   params.DeviceConfig.similation_mode = false;
   params.DeviceConfig.type = QSCPI;

   params.rf_mode = PHONE_MODE_GSM_850;
   params.m_nv_mode = NV_MODE_GSM_850;
   
   params.chainID = 0;

   // ****************************** //
   // Set CalDB XML
   // ****************************** //  
   RFCal::UserConfig::Set_CalDBFilePath ( hPhone, params.FilePath.stCalDBXML );

   // ****************************** //
   // Set equipconfig Paths
   // ****************************** //
   RFCal::UserConfig::Set_EquipConfigFilePath ( hPhone, params.FilePath.stEquipConfigXML);


   return true;
}
unsigned char RFCal::GSM_Cal::AssignRxParams(HANDLE hPhone, GSM_CAL_Params & params)
{
   AssignCommonParams(hPhone, params);
   params.channel = 128;

   // ****************************** //
   // Set Output Filepaths (XQCN and QMSL log)
   // ****************************** //
   RFCal::UserConfig::Set_OutputResultsFilePath ( hPhone, "C:\\Qualcomm\\QDART\\Temp","GSM_Rx_Cal",  params.FilePath.stQMSLLog, params.FilePath.stCalibratedXQCN, params.FilePath.stDLFLog );

   GSM_Rx_CAL_Params & rxParams = params.rxParams;

   rxParams.rx_read_nv_data_from_uut = true;// true - read NV from DUT. 

   rxParams.rx_do_hi_lin = false;
   rxParams.rx_path = 0;
   rxParams.rx_chainID_selection = 2;  // C0 = 0 or C2=2 
   params.sigPaths.prx.antenna_port = 2; // Not really needed. overwrite by ftm resposne

   params.writeConsolidatedRxNvVersion = 2;

   rxParams.rx_alt_path = 0;
   rxParams.rx_only_calibrate_drx = true; // Set true for DRx cal
   rxParams.rx_use_both_main_and_aux = false; // set true for Aux port cal

   rxParams.burst_avgs = 5;
   
   RFCal::Helper::Convert::Str_To_Vector( rxParams.rx_gain_ranges,                     "0,1,2,3");
   RFCal::Helper::Convert::Str_To_Vector( rxParams.rx_levels,                          "-90,-90,-54,-54");

   rxParams.m_rx_antenna_path_selection = 0; // Antenna
   RFCal::Helper::Convert::Str_To_Vector( rxParams.channel_list,                       "128,145,163,180,198,215,233,251");
   RFCal::Helper::Convert::Str_To_Vector( rxParams.rx_gain_offsets_max_lims,           "2850,2700,2200,2000");
   RFCal::Helper::Convert::Str_To_Vector( rxParams.rx_gain_offsets_min_lims,           "1750,1600,1300,1200");

   return true;
}

unsigned char RFCal::GSM_Cal::AssignTxParams(HANDLE hPhone, GSM_CAL_Params & params)
{
   AssignCommonParams(hPhone, params);  

   params.channel = 128;
   params.chainID = 0;

   params.performDACal = true;
   params.performPreDistCal = true;

   params.txWriteChainEnumeratedNv = false;
   params.m_txAntPath = 65535;
   params.writeConsolidatedTxNvVersion = 1;

   GSM_DA_CAL_Params & daCalParams = params.daCalParams;
   daCalParams.channel_list.reserve(3);
   daCalParams.channel_list.push_back(128);
   daCalParams.channel_list.push_back(190);
   daCalParams.channel_list.push_back(251);

   daCalParams.segLength = 50;
   daCalParams.paRanges.reserve(2);
   daCalParams.paRanges.push_back(0);
   daCalParams.paRanges.push_back(1);
   daCalParams.inputLevel = 33;

   daCalParams.extraRGI0_afterPA = true;
   daCalParams.extraRGI0 = false;

   daCalParams.triggerMaxLevel = 38;
   daCalParams.triggerMinLevel = 28;

   daCalParams.tcxoPdm = 0;

   daCalParams.triggerLevRGI = 31;
   daCalParams.startRGI = 0;
   daCalParams.stopRGI = 31;
   daCalParams.stepRGI = abs(-1);
   daCalParams.trigPA = 0;
   daCalParams.measStartDelay = 50; // DA:Measurement Start Delay
   daCalParams.segLength = 50; // DA:Step Duration
   daCalParams.measLength = 100;
   daCalParams.tx_swp_cfg.m_meas_length_us = 100;
   const double GSM_TX_DA_SEGMENT_DURATION_US                  = 1008.0/273.0;   //US = microseconds
   daCalParams.tx_swp_cfg.m_step_length_us = GSM_TX_DA_SEGMENT_DURATION_US * daCalParams.segLength;
   daCalParams.tx_swp_cfg.m_ul_max_lvl = 33; // DA:Waveform Max Level
   daCalParams.tx_swp_cfg.m_trigger_offset_us = 30; // Measurement offset
   daCalParams.tx_swp_cfg.m_meas_length_us = daCalParams.measLength;

   daCalParams.numStepsPerPARange = (unsigned short)(1 + (long)floor(((float)(abs((int)daCalParams.stopRGI-(int)daCalParams.startRGI))) / daCalParams.stepRGI));
   daCalParams.tx_swp_cfg.m_num_steps = 1 +                         // Trigger Step
                                                       2*daCalParams.paRanges.size()*daCalParams.numStepsPerPARange +    // Measurement Steps
                                                       (daCalParams.paRanges.size()-1)*(daCalParams.extraRGI0?1:0) +      // Extra RGI0 Steps, if any
                                                       (daCalParams.paRanges.size()-1)*(daCalParams.extraRGI0_afterPA?1:0); //Extra RGI0 steps after entering PA range, if any 
   
   daCalParams.nv_action = 0;
   daCalParams.saturationDetectionWindow = 0.0;
   daCalParams.nvMappings.clear();
   daCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F1_CHANNEL);
   daCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F2_CHANNEL);
   daCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F3_CHANNEL);


   EDGE_PreDist_Cal_Data & preDistCalParams = params.preDistCalParams;
   preDistCalParams.swappedOrderEdgeFirst = true;
   preDistCalParams.channel_list = daCalParams.channel_list;
   preDistCalParams.edgeDuration = 625;
   preDistCalParams.preDcDuration = 625;
   preDistCalParams.measStartDelay = 50;
   preDistCalParams.inputLevel = 35; // Waveform Max Level
   preDistCalParams.calRGIPower = 27;
   preDistCalParams.paRangeCalibrated = 0;
   preDistCalParams.dcSkipPercentage = 60; // PreDist:DC Transient Duration//
   preDistCalParams.edgeSkipSymbols = 90;
   preDistCalParams.tcxoPdm = 0;

   preDistCalParams.overRideModemConsts = true;
   preDistCalParams.digGainUnity = 64;
   preDistCalParams.rampUnity = 8192;
   preDistCalParams.extensionFloor = 5200;
   preDistCalParams.dacScale = 4;

   preDistCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F1_CHANNEL);
   preDistCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F2_CHANNEL);
   preDistCalParams.nvMappings.push_back(QMSL_POLAR_CAL_F3_CHANNEL);

   preDistCalParams.overRideSmoothingDefaults = true;
   preDistCalParams.enableAmAmSmoothing = 0;
   preDistCalParams.amamKnotSize = 2.0;
   preDistCalParams.ampmKnotSize = 2.0;

   // ****************************** //
   // Set Output Filepaths (XQCN and QMSL log)
   // ****************************** //
   RFCal::UserConfig::Set_OutputResultsFilePath ( hPhone, "C:\\Qualcomm\\QDART\\Temp","GSM_Tx_Cal",  params.FilePath.stQMSLLog, params.FilePath.stCalibratedXQCN, params.FilePath.stDLFLog );

   return true;
}

unsigned char RFCal::GSM_Cal::AssignFBRxParams(HANDLE hPhone, GSM_CAL_Params & params)
{
   AssignCommonParams(hPhone, params);  
   params.setRFMPathVersion = 2;
   params.channel = 128;
   RFCal::Helper::Convert::Str_To_Vector( params.band_list,      "10,18,11,12");
   RFCal::Helper::Convert::Str_To_Vector( params.channel_list,   "975,37,124;128,190,251;512,700,885;512,661,810");
   RFCal::Helper::Convert::Str_To_Vector( params.nv_mapping,      "1,2,3");
   params.chainID = 0;

   params.m_txAntPath = 65535;
   params.writeConsolidatedTxNvVersion = 1;

   GSM_V4_FBRx_Params & fbrxParams = params.gsm_v4_Params.m_FBRx_Params;
   CGSM_V4_Cal_Params & m_gsmV4Params = params.gsm_v4_Params;

   fbrxParams.enableFbrxGainCal = true;
   RFCal::Helper::Convert::Str_To_Vector( fbrxParams.fbrx_gain_state_list,   "5");
   RFCal::Helper::Convert::Str_To_Vector( fbrxParams.pa_state_list,          "0");
   RFCal::Helper::Convert::Str_To_Vector( fbrxParams.rgi_list,               "19");
   RFCal::Helper::Convert::Str_To_Vector( fbrxParams.exp_pwr_list,           "30");
   
   params.gsm_v4_Params.m_bcc.m_bandList = params.band_list;
   params.gsm_v4_Params.m_bcc.m_chanList = params.channel_list;
   params.gsm_v4_Params.m_bcc.m_numBands = params.band_list.size();
   params.gsm_v4_Params.m_bcc.m_numChan  = params.channel_list[0].size();
   
   for (unsigned char i=0; i<params.gsm_v4_Params.m_bcc.m_numBands; ++i)
   {
      switch( params.gsm_v4_Params.m_bcc.m_bandList[i] )
      {
         case NV_MODE_GSM_EGSM:
            params.nv_mode_list.push_back(NV_MODE_GSM_EGSM);
            break;
         case NV_MODE_GSM_850:
            params.nv_mode_list.push_back(NV_MODE_GSM_850);
            break;
         case NV_MODE_GSM_DCS:
            params.nv_mode_list.push_back(NV_MODE_GSM_DCS);
            break;
         case NV_MODE_GSM_1900:
            params.nv_mode_list.push_back(NV_MODE_GSM_1900);
            break;
         default:
            break;
      }
   }
   RFCal::Helper::Convert::Str_To_Vector( params.gsm_v4_Params.m_bcc.m_txChainList,               "0"); // Tx/Rx Chain ID List 
   params.gsm_v4_Params.m_bcc.m_rxChainList = params.gsm_v4_Params.m_bcc.m_txChainList;
   params.gsm_v4_Params.m_bcc.m_numTxChains = static_cast < unsigned int >( params.gsm_v4_Params.m_bcc.m_txChainList.size() );
   params.gsm_v4_Params.m_bcc.m_numRxChains = static_cast < unsigned int >( params.gsm_v4_Params.m_bcc.m_rxChainList.size() );

   params.gsm_v4_Params.m_numFrames = 1;
   params.gsm_v4_Params.m_numFramesPerChan = 1;

   // set the Tx and Rx frame configurations - hardcoded slot configurations - this is 1 of only 2 ways that the slot config can
   // work currently, due to FTM constraints
   m_gsmV4Params.m_TxSlotConfig.resize(GSM_V4_MAX_NUM_OF_SLOTS);
   // Note: only [1,1,1,1,0,0,0,0] or [0,1,1,1,1,0,0,0] work due to the 3 slot shift on UUT side
   // Note: If 2 chains are specified, than only [1,1,1,1,0,0,0,0] works.
   // Chain 2 is specified in the same slot as Chain 0 in the command config, but is shifted by 4 slots on the UE.
   m_gsmV4Params.m_TxSlotConfig[0] = 1;
   m_gsmV4Params.m_TxSlotConfig[1] = 1;
   m_gsmV4Params.m_TxSlotConfig[2] = 1;
   m_gsmV4Params.m_TxSlotConfig[3] = 1;
   m_gsmV4Params.m_TxSlotConfig[4] = 0;
   m_gsmV4Params.m_TxSlotConfig[5] = 0;
   m_gsmV4Params.m_TxSlotConfig[6] = 0;
   m_gsmV4Params.m_TxSlotConfig[7] = 0;

   m_gsmV4Params.m_txNumSlots = 4;//gsmV4_maxNumTxSlots;

   // Add the Call Box Slot Configuration
   // Default to copying over the slot configuration --> case for 1 Tx chain
   m_gsmV4Params.m_TxSlotConfig_CB = m_gsmV4Params.m_TxSlotConfig;
   // if 2 chains are used, add slots for second chain (shifted by 4 relative to first chain)
   if (m_gsmV4Params.m_bcc.m_numTxChains > 1)
   {
      // (GSM_V4_MAX_NUM_OF_SLOTS - GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE + 1) insures vector bounds
      for (unsigned char slotIdx=0; slotIdx < (GSM_V4_MAX_NUM_OF_SLOTS - GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE + 1); ++slotIdx)
      {
         if (m_gsmV4Params.m_TxSlotConfig[slotIdx] > 0)
         {
            m_gsmV4Params.m_TxSlotConfig_CB[slotIdx + GSM_V4_SEC_CHAIN_TX_SHIFT_ON_PHONE] = 1;
         }
      }
   }

   m_gsmV4Params.m_RxSlotConfig.resize(GSM_V4_MAX_NUM_OF_SLOTS);
   // Note: Only 1 Rx slot can be used
   // Note: Rx slots can only go in 0 or 1 with Tx configration of [1,1,1,1,0,0,0,0]
   // Note: Rx slots can only go in 1 or 2 with Tx configration of [0,1,1,1,1,0,0,0]
   m_gsmV4Params.m_rxSlotOffset = 0;
   if ( m_gsmV4Params.m_Rx_Params.performRxCal )
   {
      m_gsmV4Params.m_RxSlotConfig[0] = 1;
      // rxSlotOffset indicates the location of the rx slot
      m_gsmV4Params.m_rxSlotOffset    = 0;
   }

   RFCal::GSM_Cal::FBRx_Cal::Compute_NumFrames(hPhone, params);

   // *********************************************** //
   // Callbox related parameters ( from XTT )
   // *********************************************** //
   m_gsmV4Params.m_sweepParamsTestEquip.m_ul_max_lvl =30.0; // "Call Box Input Level (dBm)"

   m_gsmV4Params.m_sweepParamsTestEquip.m_step_length_us = 577; // "Step Length (us)"

   m_gsmV4Params.m_sweepParamsTestEquip.m_meas_length_us = 450; // "Measurement Length (us)"

   m_gsmV4Params.m_sweepParamsTestEquip.m_trigger_offset_us = 20; // "Trigger Length (us)"

   m_gsmV4Params.m_measStartDelay_ms = 100 ; // ("Measurement Start Delay (ms)"

   // *********************************************** //
   // Set Output Filepaths (XQCN and QMSL log)
   // *********************************************** //
   RFCal::UserConfig::Set_OutputResultsFilePath ( hPhone, "C:\\Qualcomm\\QDART\\Temp","GSM_FBR_Cal",  params.FilePath.stQMSLLog, params.FilePath.stCalibratedXQCN, params.FilePath.stDLFLog );

   return true;
}

unsigned char RFCal::GSM_Cal::INIT_TX(HANDLE hPhone, GSM_CAL_Params & params)
{
   // Set Calibration state ON
   QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);
 
   if (params.query_path_from_rfc)
   {
      // ************************************************ //
      // Get RFM Path info and save it to the data base
      // ************************************************ //
      if (!(params.repository.rfmPathDB.CreateRFMPathDB(hPhone, TECHNOLOGY_GSM, -1 /* all band */, 0)))
      {
         cout << "Failed in getting RFM Path info " << endl;
         return false;
      }
      
      CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
      RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, 65535, 0, 0, GSM_TX_BASED_QUERY);
      /* Sequence to be followed
      1. Set Device and Signal path
      2. Set the RF Mode
      3. Set Antenna path. Set to tx Path during Tx Cal and Rx path during Rx Cal.
      */

      // Set the device and signal path
      if (!QLIB_FTM_GSM_SET_DEVICE_AND_SIG_PATH( hPhone,
         static_cast<unsigned char>(sigPaths.prx.rfm_dev),static_cast<unsigned char>(sigPaths.drx.rfm_dev),static_cast<unsigned char>(sigPaths.tx.rfm_dev),
         static_cast<unsigned char>(sigPaths.prx.signal_path),static_cast<unsigned char>(sigPaths.drx.signal_path),static_cast<unsigned char>(sigPaths.tx.signal_path)))
      {
         return false;
      }
      
      // Set RF Mode
      QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
      QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

      if (0xFFFF != sigPaths.tx.antenna_port)
      {
        if (!QLIB_FTM_GSM_SET_ASDIV_PATH( hPhone, static_cast<unsigned char>(sigPaths.tx.antenna_port)))
        {
           return false;
        }
      }
   }
   else
   {
         // Set RF Mode
      QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
      QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

   }

   // FTM_SET_TRANSMIT_BURST 
   unsigned char status = QLIB_FTM_SET_TRANSMIT_BURST(hPhone, 0 /* slot */,  0 /* 0 = TX_DATA_SOURCE_PSDRND (data_source) */, 5 /* = GSM_TX_TSC (TSC) */, 0 /* Num Burst */, 1 /* Burst Duration */);

   return status;
}
unsigned char RFCal::GSM_Cal::INIT_FBRX(HANDLE hPhone, GSM_CAL_Params & params)
{
   // Set Calibration state ON
   QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);

   // ************************************************ //
   // Get RFM Path info and save it to the data base
   // ************************************************ //
   if (!(params.repository.rfmPathDB.CreateRFMPathDB(hPhone, TECHNOLOGY_GSM, -1 /* all band */, 0)))
   {
	   cout << "Failed in getting RFM Path info " << endl;
	   return false;
   } 
	if ( 2 == params.setRFMPathVersion )
	{
		FTM_RF_Mode_Enum temp_rf_mode;
		CGSM_Cal_Device_SigPath temp_sigPath;

		vector<FTM_RF_Mode_Enum> rfModeVec;
		vector<CGSM_Cal_Device_SigPath> rfmPathVec;

		//Repeat process for each band
		for ( vector<int>::const_iterator band_it = params.band_list.begin(); band_it < params.band_list.end(); ++ band_it )
		{
			temp_rf_mode = static_cast<FTM_RF_Mode_Enum> (*band_it);
			// Get devices to calibrate from the RFC Global repository
			RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, temp_rf_mode, temp_sigPath, params.m_txAntPath, 0, 0, GSM_TX_BASED_QUERY);
			rfModeVec.push_back(temp_rf_mode);
			rfmPathVec.push_back(temp_sigPath);

			/*  if(temp_rf_mode == rf_mode)
			m_calRFMPaths = temp_sigPath;*/
		}

		params.sigPaths = temp_sigPath;

		/* Sequence to be followed
		1. Set the complete RFM path for all bands
		2. Set the RF Mode
		*/
		if ( !RFCal::GSM_Cal::GSM_Set_RFM_Path( hPhone, rfModeVec, rfmPathVec))
		{
			return false;
		}

		// Set RF Mode
		QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
		QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

	}
   else if (1 == params.setRFMPathVersion)
   {
      
      CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
      RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, 65535, 0, 0, GSM_TX_BASED_QUERY);
      /* Sequence to be followed
      1. Set Device and Signal path
      2. Set the RF Mode
      3. Set Antenna path. Set to tx Path during Tx Cal and Rx path during Rx Cal.
      */

      // Set the device and signal path
      if (!QLIB_FTM_GSM_SET_DEVICE_AND_SIG_PATH( hPhone,
         static_cast<unsigned char>(sigPaths.prx.rfm_dev),static_cast<unsigned char>(sigPaths.drx.rfm_dev),static_cast<unsigned char>(sigPaths.tx.rfm_dev),
         static_cast<unsigned char>(sigPaths.prx.signal_path),static_cast<unsigned char>(sigPaths.drx.signal_path),static_cast<unsigned char>(sigPaths.tx.signal_path)))
      {
         return false;
      }
      
      // Set RF Mode
      QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
      QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

      if (0xFFFF != sigPaths.tx.antenna_port)
      {
        if (!QLIB_FTM_GSM_SET_ASDIV_PATH( hPhone, static_cast<unsigned char>(sigPaths.tx.antenna_port)))
        {
           return false;
        }
      }
   }
   else
   {
         // Set RF Mode
      QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
      QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

   }
   return true;
}

unsigned char RFCal::GSM_Cal::INIT_V5(HANDLE hPhone, GSM_CAL_Params & params)
{
	// Set Calibration state ON
	QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);
/*
	if ( !params.query_path_from_rfc )
	{
		return false;
	}
*/

	// ************************************************ //
	// Get RFM Path info and save it to the data base
	// ************************************************ //
	if (!(params.repository.rfmPathDB.CreateRFMPathDB(hPhone, TECHNOLOGY_GSM, -1 /* all band */, 0)))
	{
		cout << "Failed in getting RFM Path info " << endl;
		return false;
	}

	if ( 2 == params.setRFMPathVersion )
	{
		FTM_RF_Mode_Enum temp_rf_mode;
		// CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
		CGSM_Cal_Device_SigPath temp_sigPath;

		vector<FTM_RF_Mode_Enum> rfModeVec;
		vector<CGSM_Cal_Device_SigPath> rfmPathVec;

		//Repeat process for each band
		for ( vector<int>::const_iterator band_it = params.band_list.begin(); band_it < params.band_list.end(); ++ band_it )
		{
			temp_rf_mode = static_cast<FTM_RF_Mode_Enum> (*band_it);
			// Get devices to calibrate from the RFC Global repository
			RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, temp_rf_mode, temp_sigPath, params.m_txAntPath, 0, 0, GSM_TX_BASED_QUERY);

			rfModeVec.push_back(temp_rf_mode);
			rfmPathVec.push_back(temp_sigPath);

			/*  if(temp_rf_mode == rf_mode)
			m_calRFMPaths = temp_sigPath;*/
		}

		/* Sequence to be followed
		1. Set the complete RFM path for all bands
		2. Set the RF Mode
		*/
		if ( !RFCal::GSM_Cal::GSM_Set_RFM_Path( hPhone, rfModeVec, rfmPathVec))
		{
			return false;
		}

		// Set RF Mode
		QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
		QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

	}

	else if ( 1 == params.setRFMPathVersion )
	{
		CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
		RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, params.m_txAntPath, 0, 0, GSM_TX_BASED_QUERY);

		/* Sequence to be followed
		1. Set Device and Signal path
		2. Set the RF Mode
		3. Set Antenna path. Set to tx Path during Tx Cal and Rx path during Rx Cal.
		*/

		// Set the device and signal path
		if (!QLIB_FTM_GSM_SET_DEVICE_AND_SIG_PATH( hPhone,
			static_cast<unsigned char>(sigPaths.prx.rfm_dev),static_cast<unsigned char>(sigPaths.drx.rfm_dev),static_cast<unsigned char>(sigPaths.tx.rfm_dev),
			static_cast<unsigned char>(sigPaths.prx.signal_path),static_cast<unsigned char>(sigPaths.drx.signal_path),static_cast<unsigned char>(sigPaths.tx.signal_path)))
		{
			return false;
		}

		// Set RF Mode
		QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
		QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

		if (0xFFFF != sigPaths.tx.antenna_port)
		{
			if (!QLIB_FTM_GSM_SET_ASDIV_PATH( hPhone, static_cast<unsigned char>(sigPaths.tx.antenna_port)))
			{
				return false;
			}
		}
	}
	else
	{
		// Set RF Mode
		QLIB_FTM_SET_MODE_ID(hPhone, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
		QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));
	}


	return true;
}

unsigned char RFCal::GSM_Cal::GSM_Set_RFM_Path(HANDLE hPhone, vector<FTM_RF_Mode_Enum>& rfModeVec, vector<CGSM_Cal_Device_SigPath>& bandRFMPaths)
{

	unsigned char cmdSuccessful = 1;


	unsigned int numBands = rfModeVec.size();

	ftm_gsm_set_rfm_path_type* gsmBandRFMPath = new ftm_gsm_set_rfm_path_type[numBands];

	for(unsigned int lpVar = 0; lpVar < numBands; ++lpVar)
	{

		gsmBandRFMPath[lpVar].rfMode = static_cast<uint8>(rfModeVec[lpVar]);

		gsmBandRFMPath[lpVar].pRxDev = static_cast<uint16>(bandRFMPaths[lpVar].prx.rfm_dev);
		gsmBandRFMPath[lpVar].dRxDev = static_cast<uint16>(bandRFMPaths[lpVar].drx.rfm_dev);
		gsmBandRFMPath[lpVar].txDev  = static_cast<uint16>(bandRFMPaths[lpVar].tx.rfm_dev);

		gsmBandRFMPath[lpVar].pRxSigPath = static_cast<uint8>(bandRFMPaths[lpVar].prx.signal_path);
		gsmBandRFMPath[lpVar].dRxSigPath = static_cast<uint8>(bandRFMPaths[lpVar].drx.signal_path);
		gsmBandRFMPath[lpVar].txSigPath  = static_cast<uint8>(bandRFMPaths[lpVar].tx.signal_path);

		gsmBandRFMPath[lpVar].pRxAntennaPath = static_cast<uint16>(bandRFMPaths[lpVar].prx.antenna_port);
		gsmBandRFMPath[lpVar].dRxAntennaPath = static_cast<uint16>(bandRFMPaths[lpVar].drx.antenna_port);
		gsmBandRFMPath[lpVar].txAntennaPath  = static_cast<uint16>(bandRFMPaths[lpVar].tx.antenna_port);

	}

	// Set the rfm path
	cmdSuccessful = QLIB_FTM_GSM_SET_RFM_PATH(hPhone, (unsigned char)numBands, gsmBandRFMPath);

	delete[] gsmBandRFMPath;

	if (cmdSuccessful == 0)
	{

		// ostr << "QLIB_FTM_GSM_SET_RFM_PATH() failed. " << __FUNCTION__ << " line " << __LINE__;
		return false;
	}

	return true;
}

unsigned char RFCal::GSM_Cal::INIT_RX(HANDLE hPhone, GSM_CAL_Params & params)
{
   unsigned char status = 1;
   // Set Calibration state ON
   QLIB_FTM_SET_CALIBRATION_STATE(hPhone, true);
 
   if (params.query_path_from_rfc)
   {     
      CGSM_Cal_Device_SigPath & sigPaths = params.sigPaths;
	  if ( ! RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, static_cast <unsigned short >  (params.rxParams.m_rx_antenna_path_selection ), 0, static_cast <unsigned short > (params.rxParams.rx_chainID_selection) , GSM_RX_BASED_QUERY) )
      {
         // If not executed, then, execute
         // ************************************************ //
         // Get RFM Path info and save it to the data base
         // ************************************************ //
         if (!(params.repository.rfmPathDB.CreateRFMPathDB(hPhone, TECHNOLOGY_GSM, -1 /* all band */, 0)))
         {
            cout << "Failed in getting RFM Path info " << endl;
            return false;
         }
         RFCal::GSM_Cal::QueryGSM_Devices_to_Calibrate(hPhone, params.repository.rfmPathDB, params.rf_mode, sigPaths, static_cast <unsigned short >  (params.rxParams.m_rx_antenna_path_selection ), 0, static_cast <unsigned short > (params.rxParams.rx_chainID_selection), GSM_RX_BASED_QUERY);
      }
      /* Sequence to be followed
      1. Set Device and Signal path
      2. Set the RF Mode
      3. Set Antenna path. Set to tx Path during Tx Cal and Rx path during Rx Cal.
      */

      // Set the device and signal path
      if (!QLIB_FTM_GSM_SET_DEVICE_AND_SIG_PATH( hPhone,
         static_cast<unsigned char>(sigPaths.prx.rfm_dev),static_cast<unsigned char>(sigPaths.drx.rfm_dev),static_cast<unsigned char>(sigPaths.tx.rfm_dev),
         static_cast<unsigned char>(sigPaths.prx.signal_path),static_cast<unsigned char>(sigPaths.drx.signal_path),static_cast<unsigned char>(sigPaths.tx.signal_path)))
      {
         return false;
      }
     // Set RF Mode
      RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, 0, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
      QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));

      if (0xFFFF != sigPaths.prx.antenna_port)
      {
        if (!QLIB_FTM_GSM_SET_ASDIV_PATH( hPhone, static_cast<unsigned char>(sigPaths.prx.antenna_port)))
        {
           return false;
        }
      }
   }
   else
   {
      // Set RF Mode
      RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, 0, FTM_MODE_ID_GSM);// Support Tx Chain Id = 0;
      QLIB_FTM_SET_MODE(hPhone, static_cast<short>(params.rf_mode));
   }
   
   return status;
}
unsigned char RFCal::GSM_Cal::RX_Cal::GroupGainRanges( HANDLE hPhone, const GSM_CAL_Params & params, map< double, vector<int> >& batches,  map< double, vector<int> >& batchesHiLin, bool performMeasGainRangeCheck)
{
   const double INVALID_RX_LEV     = -9999.99;
   double rx_level_across_channels = INVALID_RX_LEV;

   double rx_level = 0.0;
   int range       = 0;

   vector<int> ranges;
   vector<int> rangesHiLin;

   const GSM_Rx_CAL_Params & rxParams = params.rxParams;

   // Group together the gain ranges containing the same level
   //
   for( UINT rx_lvl_index=0; rx_lvl_index< rxParams.rx_levels.size(); ++rx_lvl_index)
   {
      ranges.clear();
      rangesHiLin.clear();
      rx_level = rxParams.rx_levels[ rx_lvl_index ];
      range    = rxParams.rx_gain_ranges[ rx_lvl_index ];

      if (performMeasGainRangeCheck)
      {
         if( find(rxParams.measured_gain_ranges.begin(),rxParams.measured_gain_ranges.end(),range) != rxParams.measured_gain_ranges.end() )
         {
            if(INVALID_RX_LEV == rx_level_across_channels)
            {
               rx_level_across_channels = rx_level;
            }
            else if(rx_level != rx_level_across_channels)
            {
               return false;//throw CApplicationException(g_exc_general_exception,"Please use Extrapolation Calibration with measured ranges that correspond to only one power level",__FUNCTION__);
            }
         }
      }

      // If already exists, get it to just expand
      if( batches.count( rx_level ) > 0 )
      {
         ranges = (*batches.lower_bound( rx_level )).second;
      }
      ranges.push_back( range );
      batches[ rx_level ] = ranges;

      if(params.rxParams.rx_do_hi_lin)
      {
         //check if we want to do high linearity CAL at the same LNA gain range.
         //for the same LNA gain range, high and low linearity CAL are done at the same cell power.
         if( find(rxParams.rx_gain_ranges_hi_lin.begin(), rxParams.rx_gain_ranges_hi_lin.end(), range) != rxParams.rx_gain_ranges_hi_lin.end() )
         {
            if( batchesHiLin.count( rx_level ) > 0 )
            {
               rangesHiLin = (*batchesHiLin.lower_bound( rx_level )).second;
            }
            rangesHiLin.push_back( range );
            batchesHiLin[ rx_level ] = rangesHiLin;
         }
      }
   }

   return true;
}
unsigned char RFCal::GSM_Cal::RX_Cal::DO_GSM_Rx_Gain_Range_Cal( HANDLE hPhone, const vector<int>& range_list, const int& burst_avgs, const bool& toggle_rx, vector<int> * rssis_prx, vector<int> * rssis_drx, bool isHiLinearity )
{
   if(range_list.size() <= 0)
   {
      return false;
      //ostr<<"Do not call this API with zero ranges to calibrate. "<<__FUNCTION__;
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   const int buffer_size = 8; // make the buffer of 8
   if(range_list.size() > buffer_size)
   {
      return false;
      //ostr<< apiName
      //    <<" cannot calibrate more than "<<buffer_size<<" gain ranges."
      //    <<" The requested amount is "<<range_list.size()<<". "<<__FUNCTION__;
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }

   char rng_bit_mask = 0;
   for(UINT i=0; i<range_list.size(); ++i)
   {
      rng_bit_mask |= (char)pow(2.0, range_list[i]);
   }

   unsigned long prxResults[buffer_size];
   unsigned long drxResults[buffer_size];
   
   unsigned char status;

   if ( NULL == rssis_drx)
   {
      if( isHiLinearity )
      {
         //currently the same FTM package is used for Hi/Lo linearity CAL
         status = QLIB_FTM_RX_GAIN_RANGE_CAL
                  (
                     hPhone,
                     static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(burst_avgs),
                     static_cast<unsigned char>(toggle_rx), prxResults
                  );
      }
      else
      {
         status = QLIB_FTM_RX_GAIN_RANGE_CAL
                  (
                     hPhone,
                     static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(burst_avgs),
                     static_cast<unsigned char>(toggle_rx), prxResults
                  );
      }
   }
   else
   {
      //currently the same FTM package is used for Hi/Lo linearity CAL
      status = QLIB_FTM_RX_GAIN_RANGE_CAL_PRX_DRX
               (
                  hPhone,
                  static_cast<unsigned char>(rng_bit_mask), static_cast<unsigned char>(burst_avgs),
                  static_cast<unsigned char>(toggle_rx), prxResults, drxResults
               );


   }
   if( status == 0 )
   {
      return false;
      //ostr<< apiName << " failed."
      //    <<" MAKE SURE THIS UUT SUPPORTS SUCH API."
      //    <<" Parameters: BitMask="<<static_cast<int>(rng_bit_mask)<<" BurstAvg="
      //    <<burst_avgs<<" ToggleState="<<toggle_rx<<". "<<__FUNCTION__;
      //throw CApplicationException( g_exc_general_exception, ostr.str() );
   }
   if ( rssis_prx )
   {
      (*rssis_prx).clear();
   }
   if ( rssis_drx )
   {
      (*rssis_drx).clear();
   }
   int range = 0;

   for(UINT range_index = 0; range_index<range_list.size(); ++range_index)
   {
      range = range_list[ range_index ];
      //the range value corresponds to the bit position
      //which in turn is the corresponding measurement index
      if ( rssis_prx )
      {
         rssis_prx->push_back( prxResults[ range ] );
      }
      if ( rssis_drx )
      {
         rssis_drx->push_back( drxResults[ range ] );
      }
   }
   return true;
}
unsigned char RFCal::GSM_Cal::RX_Cal::DO_GSM_Rx_FreqComp(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{

   int channel     = 0;
   double rx_level = 0.0;

   map< double, vector<int> > batches;
   map< double, vector<int> > batchesHiLin;
   map< double, vector<int> >::iterator batches_iter;
   map< double, vector<int> >::iterator batches_iter_hi_lin;

   //GSM_RX_MEAS_TYPE rx_meas;
   const GSM_Rx_CAL_Params & rxParams = params.rxParams;
   GroupGainRanges( hPhone, params, batches, batchesHiLin, false );

   //! Setup equipment digital modulation
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();

   if ( NULL == pMgr)
   {
      return false;
   }


   pMgr->Set_DL_Mod_Type( MOD_TYPE_RFCAL_DIG_GMSK_CONTINUOUS );

   pMgr->Set_OutputPort( false );

   //! Disable UUT Tx
   QLIB_FTM_SET_TX_OFF( hPhone );

   //! Configure GSM Rx Burst off
   QLIB_FTM_SET_RX_BURST ( hPhone, 0, static_cast < short > ( RX_FUNCTION_STOP_CONTINOUS_RX ) );

   results.rxPath_chan_rssis_list.clear();
   results.rxPath_chan_rssis_list_hi_lin.clear();

   // set the chain to correct value
   QLIB_FTM_SET_SECONDARY_CHAIN ( hPhone, params.chainID);
   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM ( hPhone, params.chainID );

   vector<int> rssis_prx;
   vector<int> rssis_drx;

   //! For each channel...
   for(UINT chan_ind = 0; chan_ind < rxParams.channel_list.size(); ++chan_ind )
   {
      channel = rxParams.channel_list [ chan_ind ];
      //! Config TE/UUT channel
      pMgr->Set_DL_Channel ( channel, 0, 0 );
      ::QLIB_FTM_SET_CHAN ( hPhone, channel );

      batches_iter = batches.begin();
      for( UINT batch_index=0; batch_index<batches.size(); ++batch_index, ++batches_iter)
      {
//         m_pUIM->HaltOnUserStopMsg();

         //! Confige RF Generator level
         rx_level = (*batches_iter).first;
         pMgr->Set_DL_Level ( rx_level );
         pMgr->Set_DL_State ( true );

         //! Do GSM Gain Range Cal
         if ( rxParams.rx_only_calibrate_drx )
         {
           // PRx + DRx case
            RFCal::GSM_Cal::RX_Cal::DO_GSM_Rx_Gain_Range_Cal( hPhone, (*batches_iter).second, rxParams.burst_avgs, (chan_ind==0) && (batch_index==0), &rssis_prx, &rssis_drx );
         }
         else
         {
          // PRx only case
            RFCal::GSM_Cal::RX_Cal::DO_GSM_Rx_Gain_Range_Cal( hPhone, (*batches_iter).second, rxParams.burst_avgs, (chan_ind==0) && (batch_index==0), &rssis_prx, NULL );
  
         }
         for(UINT rssi_index=0; rssi_index < rssis_prx.size(); ++rssi_index)
         {
            GSM_RX_MEAS_TYPE rx_meas;
            rx_meas.m_lna = (*batches_iter).second[ rssi_index ];
            rx_meas.m_rssi = rssis_prx[ rssi_index ];
            results.rxPath_chan_rssis_list[0][channel].push_back( rx_meas );
         }
         for(UINT rssi_index=0; rssi_index < rssis_drx.size(); ++rssi_index)
         {
            GSM_RX_MEAS_TYPE rx_meas;
            rx_meas.m_lna = (*batches_iter).second[ rssi_index ];
            rx_meas.m_rssi = rssis_drx[ rssi_index ];
            results.rxPath_chan_rssis_list[1][channel].push_back( rx_meas );
         }

         //! Do Gain Range CAL in high linearity at only the designated channel
         if( true == rxParams.rx_do_hi_lin && rxParams.channel_hi_lin == channel )
         {
            batches_iter_hi_lin = batchesHiLin.find(rx_level);
            if( batches_iter_hi_lin != batchesHiLin.end() )
            {
               map< int, vector<GSM_RX_MEAS_TYPE> > dummy;
               //DoHiLinMeasurement( params, channel, (*batches_iter_hi_lin).second, false, chan_rssis_list_hi_lin, dummy );
            }
         }
      }
   }
   //! Configure GSM Rx Burst off
   pMgr->Set_DL_State( false );
   QLIB_FTM_SET_RX_BURST (hPhone,  0,  static_cast < short > ( RX_FUNCTION_STOP_CONTINOUS_RX ));//RX OFF

   return true;
}

unsigned char RFCal::GSM_Cal::PreDist_Cal::DO_GSM_PreDist_Cal(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   vector<long> calRgis;

   if (params.performDACal)
   {
      calRgis.push_back(RFCal::GSM_Cal::PreDist_Cal::Determine_PreDist_Cal_RGI(hPhone, params, results));
   }
   
   results.calibratedPreDistData = new PreDist_Capture_Record_Channel(); // Need to deallocate later.
   if (NULL == results.calibratedPreDistData)
   {
      RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, "Not enough memory");
      return false;
   }

   for ( vector<long>::iterator it = calRgis.begin(); it != calRgis.end(); it++)
   {
      stringstream ss;
      ss <<"Running Predist calibration for RGI=" << (*it);
      RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ss.str());

      ZeroMemory( results.calibratedPreDistData, sizeof( PreDist_Capture_Record_Channel ) );

      results.calibratedPreDistData->calRgi = (*it);

      DO_EDGE_PreDist_Cal(hPhone, params, results.calibratedPreDistData->calRgi, results.calibratedPreDistData);

   }
   return true;
}
unsigned char RFCal::GSM_Cal::PreDist_Cal::DO_EDGE_PreDist_Cal(HANDLE hPhone, const GSM_CAL_Params & params, unsigned long calRgi, PreDist_Capture_Record_Channel * calibratedData)
{
   const EDGE_PreDist_Cal_Data & preDistCalParams = params.preDistCalParams;
   vector<double> PreDistSweepPwrs;
   
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   if ( NULL == pMgr)
   {
      return false;
   }

   unsigned long chanLoopVar = 0;
   for(vector<unsigned long>::const_iterator it = preDistCalParams.channel_list.begin(); it != preDistCalParams.channel_list.end(); it++)
   {
      calibratedData->channelNumbers[chanLoopVar] = (*it);
   
      pMgr->Set_DL_Channel(static_cast<int>(*it), 0);
  
      //! UUT Set Channel
      RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone,0,FTM_MODE_ID_GSM);
      QLIB_FTM_SET_CHAN(hPhone, *it);

      //! Set Tx Off
      QLIB_FTM_SET_TX_OFF(hPhone);
      
      //! Set PA Range
      unsigned char status = QLIB_FTM_SET_GSM_LINEAR_PA_RANGE(hPhone, 0, static_cast<unsigned short>(preDistCalParams.paRangeCalibrated));

      //! Setup TE
      pMgr->Set_UL_Expected_Level(preDistCalParams.inputLevel);

      ///UUT_Send_PreDistortion_Sweep;
      unsigned long totalDuration = preDistCalParams.edgeDuration+preDistCalParams.preDcDuration;
      pMgr->Config_PreDist_Capture( totalDuration, calibratedData->numSamples, calibratedData->samplingRateHz );
      
      //Try three times
      int retryLoop = 3;

      while(retryLoop > 0)
      {
         pMgr->Start_PreDist_Capture();
         //! Sleep for measurement delay
         Sleep((dword)preDistCalParams.measStartDelay);

         unsigned char status;

         RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, params.chainID, FTM_MODE_ID_GSM_EXTENDED_C);
      
 
         if(!preDistCalParams.swappedOrderEdgeFirst)
            status = QLIB_FTM_TX_CFG2_AMAM_SWEEP( hPhone, preDistCalParams.preDcDuration, preDistCalParams.edgeDuration, calRgi, &calibratedData->calScaleSetting );
         else
            status = QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2( hPhone, preDistCalParams.preDcDuration, preDistCalParams.edgeDuration, calRgi, 0,  &calibratedData->calScaleSetting );

         // restore the mode ID
         RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone,0,FTM_MODE_ID_GSM);
         stringstream ss;
         ss << "Sending PreDist IQ: chan " << (*it);
         RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ss.str());

         unsigned long returnedSamples = 0;
         pMgr->Get_PreDist_Capture_Data(returnedSamples,calibratedData->captureRecords[chanLoopVar].phaseArr,calibratedData->captureRecords[chanLoopVar].ampldArr);

         if(calibratedData->numSamples == returnedSamples)
         {
            //Sweep Succesful
            break;
         }
         else
         {
            ss.str("");
            ss << "Expected : " << calibratedData->numSamples << " but received " << returnedSamples << ". Retrying " << (--retryLoop) << "times ...";
            RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ss.str());
            if(retryLoop == 0)
            {
               return false;
            }
         }
      }
      chanLoopVar++;
   }
   return true;
}

unsigned long RFCal::GSM_Cal::PreDist_Cal::Determine_PreDist_Cal_RGI(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   GSM_DA_Cal_Sweep_Record preDistRangeEdgeDaCalRecord;
   const double preDistCalPower = params.preDistCalParams.calRGIPower;
   const unsigned short preDistCalPaRange = params.preDistCalParams.paRangeCalibrated;
  
   unsigned long rgiMatch = 0;
   unsigned long rgiAllChan = 0;

   double power = -1000.0;
   bool chanRgiReached;

   map<unsigned short,double>::iterator iter;
   for(vector <unsigned long>::const_iterator it = params.daCalParams.channel_list.begin(); it != params.daCalParams.channel_list.end(); it++)
   {
      unsigned short chan = static_cast<unsigned short>(*it);
      preDistRangeEdgeDaCalRecord = results.da_cal_res.channelSweepData[chan].edgeDaSweep[preDistCalPaRange];

      chanRgiReached = false;
      for(iter=preDistRangeEdgeDaCalRecord.rgiPwr.begin(); iter!=preDistRangeEdgeDaCalRecord.rgiPwr.end(); iter++)
      {
         power = iter->second;
         if(power >= preDistCalPower)
         {
            rgiMatch = iter->first;
            chanRgiReached = true;
            break;
         }
      }
      if(!chanRgiReached)
      {
         stringstream ss;
         ss << "Failed to acheive target predistortion power ";
         ss << preDistCalPower << " dBm in Channel " << chan << ". Acheived " << power << " dBm.";
         RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str() );
         return 0;//throw CApplicationException( g_exc_general_exception, ostr.str() );
      }

      if(rgiMatch > rgiAllChan)
      {
         rgiAllChan = rgiMatch;
      }
   }

   stringstream ss;
   ss << "Calibration RGI :" << rgiAllChan;
  // m_pUIM->PrintToDebug(ostr.str());

   return rgiAllChan;
}
unsigned char RFCal::GSM_Cal::DA_Cal::DO_GSM_DA_Cal(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   const GSM_DA_CAL_Params & daCalParams = params.daCalParams;
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();

   pMgr->Get_Version();

   unsigned char status = 1;
    // default values
   int retryLoop = 3;
   
   unsigned long gsmSweepIndexOfFirstRGIinFirstPaRange = 2;
   unsigned long gsmSweepIndexOfLastRGIinFirstPaRange  = 2;

   vector<double> daSweepPwrs;
   daSweepPwrs.reserve(100);

   for (std::vector <unsigned long>::const_iterator ch_it = params.daCalParams.channel_list.begin(); 
      ch_it != params.daCalParams.channel_list.end(); ch_it++)
   {
      pMgr->Set_DL_Channel((*ch_it),0.,+0.0677*1000.0);
      //! UUT Set Channel
      status = QLIB_FTM_SET_CHAN(hPhone, (*ch_it));
      //! Set VCTCXO
      status = QLIB_FTM_SET_PDM( hPhone, 0 /* PDM_GSM_VCTCXO_ADJ */, static_cast<unsigned short>(daCalParams.tcxoPdm) );
      //! Set Tx Off
      status = QLIB_FTM_SET_TX_OFF(hPhone);

      //! Setup TE
      pMgr->Set_UL_Expected_Level( daCalParams.inputLevel);
     
      pMgr->Config_GSM_Tx_Sweep( daCalParams.tx_swp_cfg );

      // Try three times - reset
    //  retryLoop = 3;
      while(retryLoop > 0)
      {
         pMgr->Start_GSM_Tx_Sweep();

         //! Sleep for measurement delay
         Sleep((dword)daCalParams.measStartDelay);

         DO_GSM_Tx_DA_Sweep(hPhone, params);

         pMgr->Get_GSM_Tx_Sweep_Results(daCalParams.tx_swp_cfg.m_num_steps,daSweepPwrs);
         if (!daSweepPwrs.empty())
         {
            unsigned char status = RFCal::GSM_Cal::DA_Cal::Check_Tx_DA_Sweep_Result(hPhone, daCalParams, daSweepPwrs,gsmSweepIndexOfFirstRGIinFirstPaRange, gsmSweepIndexOfLastRGIinFirstPaRange);
            if (status)
            {
               // Succcess. Exit from while loop
               ::Sleep(20);
               Store_Tx_DA_Results(hPhone, (*ch_it), daCalParams, daSweepPwrs, results);
               break;
            }
         }
         retryLoop--;
         // Failure case remove results++++++++++++++++++++++++++++++++++++++++++++++++
         unsigned int numRes = daSweepPwrs.size();
         daSweepPwrs.clear();
         daSweepPwrs.reserve(numRes);
      }
   }

   // LOG DA Cal
   return status;
}
unsigned char RFCal::GSM_Cal::DA_Cal::Store_Tx_DA_Results(HANDLE hPhone, const unsigned long channel, const GSM_DA_CAL_Params & daCalParams, const vector<double> & daSweepPwrs,  GSM_CAL_Results & results)
{

   //Store each channel's trigger level
   unsigned long stepLoopVar = 0;
   results.da_cal_res.chanTriggerLevel[static_cast<unsigned short>(channel)] = daSweepPwrs[stepLoopVar++];

   GSM_DA_Cal_Results daCalResults;
   daCalResults.edgeDaSweep.clear();
   daCalResults.gsmDaSweep.clear();

   for(vector <unsigned short>::const_iterator it = daCalParams.paRanges.begin(); it != daCalParams.paRanges.end(); it++)
   {
      GSM_DA_Cal_Sweep_Record gsmRecord,edgeRecord;

      edgeRecord.rgiPwr.clear();
      gsmRecord.rgiPwr.clear();

      //extra RGI after entering PA state
      //skipping the first PA state
      if( it != daCalParams.paRanges.begin() )
      {
         if(daCalParams.extraRGI0_afterPA)
         {
            stepLoopVar++;
         }
      }

      if(daCalParams.startRGI < daCalParams.stopRGI)
      {
         for(unsigned short rgiLoopVar = (unsigned short)daCalParams.startRGI; rgiLoopVar<=daCalParams.stopRGI; rgiLoopVar+=(unsigned short)daCalParams.stepRGI)
         {
            edgeRecord.rgiPwr[rgiLoopVar] = daSweepPwrs[stepLoopVar++];
            gsmRecord.rgiPwr[rgiLoopVar] = daSweepPwrs[stepLoopVar++];
         }
      }
      else
      {
         for(unsigned short rgiLoopVar = (unsigned short)daCalParams.startRGI; rgiLoopVar>=daCalParams.stopRGI && (short)rgiLoopVar >= 0; rgiLoopVar-=(unsigned short)daCalParams.stepRGI)
         {
            edgeRecord.rgiPwr[rgiLoopVar] = daSweepPwrs[stepLoopVar++];
            gsmRecord.rgiPwr[rgiLoopVar] = daSweepPwrs[stepLoopVar++];
         }
      }

      if(daCalParams.extraRGI0)
      {
         // Throw away the extra RGI0 step. It's just there to prevent PA blow-up, not as a point to measure
         stepLoopVar++;
      }
    
      daCalResults.edgeDaSweep[(*it)] = edgeRecord;
      daCalResults.gsmDaSweep[(*it)]  = gsmRecord;
   }
   results.da_cal_res.channelSweepData[static_cast<unsigned short>(channel)] = daCalResults;
   return true;
}
      
unsigned char RFCal::GSM_Cal::DA_Cal::Check_Tx_DA_Sweep_Result(HANDLE hPhone,  const GSM_DA_CAL_Params & daCalParams, const vector<double> & daSweepPwrs, 
         unsigned long & gsmSweepIndexOfFirstRGIinFirstPaRange,
         unsigned long & gsmSweepIndexOfLastRGIinFirstPaRange )
{
   bool check1 = false;
   bool check2 = false;
   unsigned char status = 1;
   // Check1 : Trigger Level Check
   check1 = (daSweepPwrs[0] > daCalParams.triggerMinLevel) && (daSweepPwrs[0] < daCalParams.triggerMaxLevel);

   // Check2 : For sanity ensure that the first GSM sweep powers monotonic
   // This will ensure that cases where the trigger is at the wrong time will get flagged.
   long numRgi = 1 + abs((long)((daCalParams.stopRGI - daCalParams.startRGI)/daCalParams.stepRGI));

   gsmSweepIndexOfLastRGIinFirstPaRange = 2*numRgi;   // update based upon numRgi

   if (daSweepPwrs.size() >= gsmSweepIndexOfLastRGIinFirstPaRange)
   {      
      if(daCalParams.stopRGI > daCalParams.startRGI)
      {
         check2 = (daSweepPwrs[gsmSweepIndexOfLastRGIinFirstPaRange] > daSweepPwrs[gsmSweepIndexOfFirstRGIinFirstPaRange]);
      }
      else
      {
         check2 = (daSweepPwrs[gsmSweepIndexOfLastRGIinFirstPaRange] < daSweepPwrs[gsmSweepIndexOfFirstRGIinFirstPaRange]);
      }
   }
  

   if(check1 && check2)
   {
      //Sweep Succesful
      status = 1;
   }
   else
   {
      if(!check1)
      {
         stringstream ss;
         ss << "Trigger Error. Trigger Level : " << daSweepPwrs[0] << "dBm does not lie between " <<
               daCalParams.triggerMinLevel << "dBm & " << daCalParams.triggerMaxLevel << "dBm. Retrying...";
         RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ss.str());
         status = 0;    
      }
      if(!check2)
      {
         stringstream ss;
         ss << "Trigger Mistimed. The power at first PA Range at RGI:" << daCalParams.startRGI
               << " is " << daSweepPwrs[gsmSweepIndexOfFirstRGIinFirstPaRange] << " dBm. and at RGI:"
               << daCalParams.stopRGI << " is " << daSweepPwrs[gsmSweepIndexOfLastRGIinFirstPaRange] << " dBm. Retrying...";
         RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, ss.str());
         status = 0;     
      }
   }
   return status;
}
unsigned char RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(HANDLE hPhone,unsigned short chainID, FTM_Mode_Id_Enum gsmModeID)
{
   unsigned char status = 1;
   if (FTM_MODE_ID_GSM == gsmModeID)
   {
      switch (chainID)
      {
         case 0:
            break;
         case 2:
            gsmModeID = FTM_MODE_ID_GSM_C2;
            break;
         case 4:
            gsmModeID = FTM_MODE_ID_GSM_C4;
            break;
         default:
            status = 0;
            break;
      }
   }
   else if (FTM_MODE_ID_GSM_EXTENDED_C == gsmModeID)
   {
      switch (chainID)
      {
         case 0:
            break;
         case 2:
            gsmModeID = FTM_MODE_ID_GSM_EXTENDED_C2;
            break;
         case 4:
            gsmModeID = FTM_MODE_ID_GSM_EXTENDED_C4;
            break;
         default:
            status = false;
            break;
      }
   }
   if (status)
   {
      status = QLIB_FTM_SET_MODE_ID( hPhone, static_cast<short>(gsmModeID) );
   }
   return status;
}
unsigned char RFCal::GSM_Cal::DA_Cal::DO_GSM_Tx_DA_Sweep(HANDLE hPhone, const GSM_CAL_Params & params)
{
   unsigned short numSegments = 0;
   unsigned char status = 0;

   const GSM_DA_CAL_Params & daParams = params.daCalParams;
   // calculate number of segments required, to only use required memory footprint
   static unsigned int MAX_SEGMENTS_DA_CAL = QMSL_GSM_MAX_RGI_STEPS * daParams.paRanges.size() * 2 // max segments due to DA cal with no extra segments, 2 is for modulations
                                             + daParams.paRanges.size() * 2                                // accounts for potential extra segments (inserting RGI steps between PA changes in XTT)
                                             + 1;                                                 // trigger segment

   vector<unsigned short> segLenList(MAX_SEGMENTS_DA_CAL);
   vector<unsigned char>  rgiList(MAX_SEGMENTS_DA_CAL);
   vector<unsigned char>  gsmModeList(MAX_SEGMENTS_DA_CAL);
   vector<unsigned char>  paRangeList(MAX_SEGMENTS_DA_CAL);

   //Load Trigger Segment
   {
      segLenList[0] = daParams.segLength;
      rgiList[0]     =  static_cast<unsigned char>( daParams.triggerLevRGI );
      gsmModeList[0]=1;//Use GSM Mode for trigger segment
      paRangeList[0] =  static_cast<unsigned char>( daParams.trigPA );
      ++numSegments;
   }

   unsigned short rgiVar;
   for(vector <unsigned short>::const_iterator it = daParams.paRanges.begin(); it != daParams.paRanges.end(); it++)
   {
      if(it != daParams.paRanges.begin())//extraRGI0_afterPA: skipping the first pa range
      {
         if(daParams.extraRGI0_afterPA)
         {
            segLenList[numSegments] = daParams.segLength;
            rgiList[numSegments]=0; // Minimum RGI
            gsmModeList[numSegments]=0;//EDGE Mode
            paRangeList[numSegments] =  static_cast<unsigned char>( *it );
            ++numSegments;
         }
      }

      if(daParams.startRGI < daParams.stopRGI)
      {
         for(rgiVar=daParams.startRGI; rgiVar<= daParams.stopRGI ; rgiVar+= daParams.stepRGI)
         {
            segLenList[numSegments]  = daParams.segLength;
            rgiList[numSegments]     =  static_cast<unsigned char>( rgiVar );
            gsmModeList[numSegments] = 0;//EDGE Mode
            paRangeList[numSegments] =  static_cast<unsigned char>( *it );
            ++numSegments;

            segLenList[numSegments]  = daParams.segLength;
            rgiList[numSegments]     =  static_cast<unsigned char>( rgiVar );
            gsmModeList[numSegments] = 1;//GSM Mode
            paRangeList[numSegments] =  static_cast<unsigned char>( *it );
            ++numSegments;
         }
      }
      else
      {
         for(rgiVar = daParams.startRGI; rgiVar >= daParams.stopRGI && ((short)rgiVar >= 0); rgiVar-= daParams.stepRGI)
         {
            segLenList[numSegments]  = daParams.segLength;
            rgiList[numSegments]     =  static_cast<unsigned char>( rgiVar );
            gsmModeList[numSegments] = 0;//EDGE Mode
            paRangeList[numSegments] =  static_cast<unsigned char>( *it );
            ++numSegments;

            segLenList[numSegments]  = daParams.segLength;
            rgiList[numSegments]     =  static_cast<unsigned char>( rgiVar );
            gsmModeList[numSegments] =1;//GSM Mode
            paRangeList[numSegments] = static_cast<unsigned char>( *it );
            ++numSegments;
         }
      }
      if(daParams.extraRGI0)
      {
         segLenList[numSegments] = daParams.segLength;
         rgiList[numSegments]=0; // Minimum RGI
         gsmModeList[numSegments]=1;//GSM Mode
         paRangeList[numSegments]=(unsigned char)(*it);
         ++numSegments;
      }
   }

   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, params.chainID, FTM_MODE_ID_GSM_EXTENDED_C);
   status = QLIB_FTM_DA_CAL( hPhone, numSegments, &segLenList[0], &rgiList[0], &gsmModeList[0], &paRangeList[0] );

   // restore the MODE ID
   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, params.chainID, FTM_MODE_ID_GSM);

   if(status == 0)
   {
      return false;
   }
   return true;
}
unsigned char RFCal::GSM_Cal::FBRx_Cal::Do_FBRx_Cal(HANDLE hPhone, GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   RFCal::CMeasurementManager * pMgr = RFCal::DeviceDB::GetInstance().GetMeasurementMgr();
   if ( NULL == pMgr ) 
      return false;
  
   unsigned short gsmV4CmdSize = 0;
   vector < unsigned char > GSM_V4_FTM_CMD_BUFF; //[GSM_V4_TEMP_CMD_BUFF_SIZE];
   GSM_V4_FTM_CMD_BUFF.resize ( GSM_V4_TEMP_CMD_BUFF_SIZE );

   CGSM_V4_Cal_Params & gsm_v4_Params = params.gsm_v4_Params;
   map<unsigned int, vector<unsigned short>> RX_BURST_FRAME_IDENTIFIER;
   RFCal::GSM_Cal::FBRx_Cal::Create_GSM_FBRx_Cal_Command( params, GSM_V4_FTM_CMD_BUFF, gsmV4CmdSize, RX_BURST_FRAME_IDENTIFIER );

   int therm_degC, thermADC;
   int gsmBand = 1; // gsm_v4_Params.m_bcc.m_bandList[0]
   RFCal::Get_Therm_Val( hPhone, TECHNOLOGY_GSM, (int)gsmBand, 0, params.sigPaths.tx, thermADC, therm_degC );

   results.therm = static_cast<uint16>(thermADC);
   
   RFCal::GSM_Cal::FBRx_Cal::FillCallBoxParams ( hPhone, params );

   pMgr->Get_Version();
   bool bSkipFirstRxMeas = false;
   pMgr->Config_GSM_V4_FBRx_Sweep ( params.gsm_v4_Params.m_sweepParamsTestEquip,  params.gsm_v4_Params.swpParams, bSkipFirstRxMeas );

   if ( gsm_v4_Params.m_Rx_Params.performRxCal )
	   pMgr->Start_GSM_V4_FBRx_Sweep( gsm_v4_Params.swpParams, true );
   else
	   pMgr->Start_GSM_V4_FBRx_Sweep( gsm_v4_Params.swpParams, false );

   //----------------------------- Execute command to run the sweep ----------------------------------//
   RFCal::GSM_Cal::GSM_V4::Do_GSM_V4_Sweep( hPhone, GSM_V4_FTM_CMD_BUFF, gsmV4CmdSize, gsm_v4_Params.m_bcc.m_txChainList, false );//compressed



   //--------------------------- Get the FBRx power measurement result from call box ----------------//
   // list of recorded Tx powers - this filters out measurements for non-Tx slots
  
   
  
   results.outputPowVec.resize( gsm_v4_Params.m_FBRx_Params.fbrx_gain_state_list.size() * gsm_v4_Params.m_bcc.m_numBands * gsm_v4_Params.m_bcc.m_numChan );
   pMgr->Get_GSM_V4_FBRx_Sweep_Results( gsm_v4_Params.m_sweepParamsTestEquip.m_num_steps, gsm_v4_Params.m_bcc.m_numChan , gsm_v4_Params.m_numFramesPerChan,
                                            results.outputPowVec, (unsigned short)gsm_v4_Params.m_FBRx_Params.fbrx_gain_state_list.size(), gsm_v4_Params.m_intraBandSegDelay );

   //--------------------------- Get the FBRx gain measurement result from UE ----------------//
   memset( & results.gsmV4_measResults, 0, sizeof(GSM_V4_FTM_CMD_Response_Type) );
   RFCal::GSM_Cal::GSM_V4::Get_GSM_V4_Sweep_Results( hPhone,  results.gsmV4_measResults );

   pMgr->End_GSM_V4_Sweep( gsm_v4_Params.swpParams );

   RFCal::GSM_Cal::GSM_V4::Do_GSM_V4_Cleanup( hPhone );

   RFCal::GSM_Cal::FBRx_Cal::Process_FBRx_Meas_Results( hPhone, params, results );

   RFCal::GSM_Cal::FBRx_Cal::Store_FBRx_Results ( hPhone, params, results );

   RFCal::GSM_Cal::FBRx_Cal::Log_FBRx_Result ( hPhone, params, results );

   RFCal::GSM_Cal::FBRx_Cal::Do_FBRx_Data_Push ( hPhone, params, results );

   return true;
}

unsigned char RFCal::GSM_Cal::FBRx_Cal::Log_FBRx_Result(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{

   const int WIDTH = 9;
   const GSM_V4_FBRx_Params & fbrx_params = params.gsm_v4_Params.m_FBRx_Params;
   stringstream ss;
   ss << endl << "=============================================================================" << endl;
   ss << "GSM FBRx Cal Results" << endl;
   ss << "=============================================================================" << endl;

   for (map<int, ftm_fbrxgain_path_result >::iterator itr =  results.fbrx_result.begin(); itr !=  results.fbrx_result.end(); itr++ )
   {  
      ss << endl;
      ss << setw(WIDTH) << "Band";
      ss << setw(WIDTH) << "SigPat";
      ss << setw(WIDTH) << "RfmD";
      ss << setw(WIDTH) << "Ant";
      ss << setw(WIDTH) << "Chan";
      ss << setw(WIDTH) << "FBGS";
      ss << setw(WIDTH) << "RGI";
      ss << setw(WIDTH) << "DigiSc";
      ss << setw(WIDTH) << "SWGain";
      ss << setw(WIDTH) << "LSE";
      ss << setw(WIDTH) << "LSEMin";
      ss << setw(WIDTH) << "LSEMax";
      ss << setw(WIDTH) << "FBRxG";
      ss << setw(WIDTH) << "FBRGMax";
      ss << setw(WIDTH) << "FBRGMin";
      ss << setw(WIDTH) << "Pwr" << endl;

      for (uint8 fbrxModeIdx =0; fbrxModeIdx < itr->second.numberOfFbrxModes; fbrxModeIdx++ )
      {
         ftm_fbrxgain_mode_result & mode_res = itr->second.modeResults [ fbrxModeIdx ];
      
         for (uint8 chIdx = 0; chIdx < mode_res.numberofChannels; chIdx ++ )
         {
            ftm_fbrxgain_channel_result & chRes = mode_res.channelResults[chIdx];
            
            for (uint8 gsIdx = 0; gsIdx < chRes.numberOfGainStates; gsIdx++ )
            {
               rfcommon_core_fbrx_result_type & res = chRes.fbrxResult [ gsIdx ];

               
               ss << setw(WIDTH) << itr->first;
               ss << setw(WIDTH) << static_cast<int>(itr->second.sig_path);
               ss << setw(WIDTH) << static_cast<int>(itr->second.rfmDevice);
               ss << setw(WIDTH) << static_cast<int>(itr->second.antennaPath);
               ss << setw(WIDTH) << chRes.channel;
               if ( ! params.gsm_v4_Params.m_FBRx_Params.rgi_list.empty() )
               {
                  ss << setw(WIDTH) <<  static_cast<int>(fbrx_params.rgi_list.front());
               }
                ss << setw(WIDTH) << static_cast<int>(res.fbrx_gain_state);
                ss << setw(WIDTH) << static_cast<int>(res.trunc_bits);
                ss << setw(WIDTH) << static_cast<int>(res.fbrx_sw_gain);

                ss << setw(WIDTH) << static_cast<int>(res.ls_err);
                ss << setw(WIDTH) << static_cast<int>(fbrx_params.LSE_MIN);
                ss << setw(WIDTH) << static_cast<int>(fbrx_params.LSE_MAX);
                ss << setw(WIDTH) << static_cast<double>(res.gain_fbrx_nom/100.0);
                ss << setw(WIDTH) << static_cast<double>(fbrx_params.fbrxPathGainNominaldB + fbrx_params.FBRX_PATH_GAIN_MARGIN);
                ss << setw(WIDTH) << static_cast<double>(fbrx_params.fbrxPathGainNominaldB - fbrx_params.FBRX_PATH_GAIN_MARGIN);
                ss << setw(WIDTH) << static_cast<double>(res.txfe_power/10.0);
                ss << endl;

            }  
         }
      }

   }
   ss << endl;
   ss << "Therm: " << results.therm << endl << endl;
   RFCal::Helper::Log::AddMessageToQMSLLog ( hPhone, ss.str());

   
   return true;
}
unsigned char RFCal::GSM_Cal::FBRx_Cal::Do_FBRx_Data_Push(HANDLE hPhone, GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   for ( vector <int>::const_iterator bandItr = params.band_list.begin(); bandItr != params.band_list.end(); bandItr++ )
   {
     
      QSCNode qscNode(TECHNOLOGY_GSM, 1000,0,RFCal::Helper::Convert::GetGSMBand(*bandItr),0);
      QSCSwpParams  swpParams;
      swpParams.calType.Enable_Push_FBRxGain_Data = true;
      swpParams.tech = TECHNOLOGY_GSM;
      swpParams.sweep_type = SEQ_CAL_CONNECTED;
      CBandInfo bandInfo;
      bandInfo.calType.Enable_FBRx_Gain = true;
      bandInfo.calType.Enable_NV_Write = true;
      bandInfo.band = RFCal::Helper::Convert::GetGSMBand(*bandItr);
      bandInfo.subBand = 0;
      swpParams.BandInfo.push_back ( bandInfo );
	  swpParams.repoPtr = &params.repository;
      RFCal::QSC::Execute_PushFBRxGain_In_ConnectedCal (hPhone, qscNode, swpParams );
   }
   return true;
}
unsigned char RFCal::GSM_Cal::FBRx_Cal::Store_FBRx_Results(HANDLE hPhone, GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   int power_idx=0;

   map<unsigned char, GSM_V4_FBRx_GAIN_RESULT_TYPE>::iterator fbrxresults;
   //map<int, vector<vector<double>> > result;
   results.fbrx_result.clear();
   const CGSM_V4_Cal_Params & m_curCalParams = params.gsm_v4_Params;

   for (unsigned char curChainIdx=0; curChainIdx <m_curCalParams.m_bcc.m_numTxChains; ++curChainIdx)
   {
      int m_curChainID = m_curCalParams.m_bcc.m_txChainList[curChainIdx];

	  map<unsigned int, CGSM_bandVsResult_type>::iterator chainToFBRxResult_itr = results.chainToFBRxResult.data.find(m_curChainID);

      if(results.chainToFBRxResult.data.end() == chainToFBRxResult_itr)
      {
         RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, "Result is not available");
         return false;
      }

      for (unsigned int m_curBandIdx=0; m_curBandIdx<m_curCalParams.m_bcc.m_numBands; ++m_curBandIdx)
      {
         int m_curBand       = m_curCalParams.m_bcc.m_bandList[m_curBandIdx];
		 map<unsigned int, CGSM_channelVsResult_type>::iterator bandToFBRxResult_itr = chainToFBRxResult_itr->second.data.find(m_curBand);

         if(chainToFBRxResult_itr->second.data.end() == bandToFBRxResult_itr)
         {
            RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, "Result is not available");
            return false;
         }

         ftm_fbrxgain_path_result fbrx_res;
         memset(&fbrx_res,0,sizeof(ftm_fbrxgain_path_result));
         fbrx_res.tech = TECHNOLOGY_GSM;
         fbrx_res.band = static_cast <uint16>(m_curBand);
         fbrx_res.numberOfFbrxModes = 1;
         fbrx_res.modeResults[0].numberofChannels = static_cast <uint8> (m_curCalParams.m_bcc.m_numChan) ;
         fbrx_res.modeResults[0].fbrxMode = 3;

         for(unsigned int curChanIdx=0 ; curChanIdx< m_curCalParams.m_bcc.m_numChan ; curChanIdx++)
         {
            ftm_fbrxgain_channel_result * chRes = &(fbrx_res.modeResults[0].channelResults[curChanIdx]);

            unsigned int curChan = m_curCalParams.m_bcc.m_chanList[m_curBandIdx][curChanIdx];
            chRes->channel = curChan;

			map<unsigned int, CGSM_fbrxGainVsResult_type>::iterator chanToFBRxResult_itr = bandToFBRxResult_itr->second.data.find(curChan);

            if(bandToFBRxResult_itr->second.data.end() == chanToFBRxResult_itr)
            {
            RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, "Result is not available");
            return false;
            }
            
            chRes->numberOfGainStates = static_cast <uint8>(m_curCalParams.m_FBRx_Params.fbrx_gain_state_list.size());
            
            for(unsigned int j=0; j<m_curCalParams.m_FBRx_Params.fbrx_gain_state_list.size() ; j++)
            {
               rfcommon_core_fbrx_result_type * fbrxRes = &(chRes->fbrxResult[j]);

               fbrxresults = chanToFBRxResult_itr->second.data.find((unsigned char)m_curCalParams.m_FBRx_Params.fbrx_gain_state_list[j]);

               if(chanToFBRxResult_itr->second.data.end() == fbrxresults)
               {
                  RFCal::Helper::Log::AddMessageToQMSLLog(hPhone, "Result is not available");
                  return false;
               }
               fbrxRes->fbrx_sw_gain = static_cast <uint16> ( fbrxresults->second.swfbrxGainResult);
               fbrxRes->ls_err = static_cast <uint32> ( fbrxresults->second.ls_err);
               fbrxRes->trunc_bits = static_cast <uint8> ( fbrxresults->second.trunc_bits);
               double swGaindB = 20* log10 ((double)fbrxRes->fbrx_sw_gain / (double)(32768));
               /* FBRx Path Analog Gain. */

               double txPower = results.outputPowVec[power_idx
                   +m_curCalParams.m_FBRx_Params.fbrx_gain_state_list.size()
                   *m_curCalParams.m_bcc.m_numChan*m_curBandIdx];

               double fbrxPathAnalogGaindB = /* Overall Gain */ swGaindB
                                                                - /* Tx Path Gain */(txPower - fbrxRes->txfe_power/10.0) 
                                                                - /* FBRx Path Digital Gain */(6.02 * (10 - fbrxRes->trunc_bits));

               fbrxRes->gain_fbrx_nom = static_cast <int>(fbrxPathAnalogGaindB *100.0); // temporary storage of FBRx gain 
               if ( txPower> 0 )
               {
                  fbrxRes->txfe_power = static_cast < int > ( txPower * 10.0 + 0.5);
               }
               else
               {
                  fbrxRes->txfe_power = static_cast < int > ( txPower * 10.0 - 0.5);
               }
               ++power_idx;
            }

         }
         power_idx=0;
       

         //******************************************************//
         // Register FBRx path data into FBRx Result DB 
         // Make sure gain_result->txfe_power is measuremed Tx data now
         //******************************************************//
         CRFM_Path rfm_path (params.sigPaths.tx);
         fbrx_res.sig_path = static_cast <uint32> ( rfm_path.signal_path);
         fbrx_res.rfmDevice = static_cast <uint16> ( rfm_path.rfm_dev);
         fbrx_res.antennaPath = static_cast <uint16> ( rfm_path.antenna_port);

         QSCNode qscNode(2, 1000,0,RFCal::Helper::Convert::GetGSMBand(m_curBand),0);
         fbrx_res.thermAdc = results.therm;

         params.repository.SetFBRxResult (hPhone, qscNode, rfm_path, fbrx_res);
         results.fbrx_result[  RFCal::Helper::Convert::GetGSMBand(m_curBand) ] = fbrx_res;
      }
   }

   return true;
}
unsigned char RFCal::GSM_Cal::FBRx_Cal::Process_FBRx_Meas_Results(HANDLE hPhone, const GSM_CAL_Params & params, GSM_CAL_Results & results)
{
   unsigned short initPayloadIdx = 0;   // initial frame index
   int start_offset = 5;
   CGSM_ChainToFBRxResult_type & chainToFBRxResult = results.chainToFBRxResult;
   const GSM_V4_BandChainChan_Params & bcc = params.gsm_v4_Params.m_bcc;
   int m_curChainID = bcc.m_txChainList[0];
   for ( unsigned int m_curBandIdx = 0; m_curBandIdx < bcc.m_numBands; ++m_curBandIdx )
   {
      int m_curBand = bcc.m_bandList[m_curBandIdx];

      int offset = start_offset;
      // search through payload for desired frames
      for (unsigned short payloadIdx = initPayloadIdx; payloadIdx < results.gsmV4_measResults.numPayload; ++payloadIdx)
      {
         char b;
         char *addr;
         addr = (char*)&(results.gsmV4_measResults.payload[0]);
         memcpy_s(&b, sizeof(char), (char*) (addr+offset), sizeof(char));

         if( (int)b == 2)
         {
            offset += (GSM_V4_Rx_RSSI_Result_Size+1)*2+5;
         }
         else if( (int)b == 6)
         {
            GSM_V4_FBRx_GAIN_RESULT_TYPE get_result_from_payload;
            memset ( &get_result_from_payload, 0, sizeof ( GSM_V4_FBRx_GAIN_RESULT_TYPE ) );
            unsigned short frameID;
            memcpy_s(&frameID, sizeof(unsigned short), (char*) (addr+offset-5), sizeof(unsigned short));
            memcpy_s(&get_result_from_payload.channel, sizeof(unsigned short), (char*) (addr+offset+1), sizeof(unsigned short));
            memcpy_s(&get_result_from_payload.fbrxState, sizeof(unsigned char), (char*) (addr+offset+3), sizeof(unsigned char));
            memcpy_s(&get_result_from_payload.hwfbrxGainResult, sizeof(unsigned int), (char*) (addr+offset+4), sizeof(unsigned int));
            memcpy_s(&get_result_from_payload.swfbrxGainResult, sizeof(unsigned int), (char*) (addr+offset+8), sizeof(unsigned int));
            memcpy_s(&get_result_from_payload.ls_err, sizeof(unsigned int), (char*) (addr+offset+17), sizeof(unsigned int));
            memcpy_s(&get_result_from_payload.trunc_bits, sizeof(unsigned int), (char*) (addr+offset+16), sizeof(unsigned char));
            //memcpy_s(&thermADC, sizeof(uint16), (char*) (addr+offset+21), sizeof(uint16));
            
			m_curBand = bcc.m_bandList[m_curBandIdx];

			for(unsigned int i = 0; i < bcc.m_chanList[m_curBandIdx].size(); ++i)
			{
				if(bcc.m_chanList[m_curBandIdx][i] == get_result_from_payload.channel)
				{
					map<unsigned int, CGSM_bandVsResult_type>::iterator chainToFBRxResult_itr = chainToFBRxResult.data.find(m_curChainID);

					if(chainToFBRxResult.data.end() == chainToFBRxResult_itr)
					{
						chainToFBRxResult.data[0].data[m_curBand].data[get_result_from_payload.channel].data.insert(pair<unsigned char, GSM_V4_FBRx_GAIN_RESULT_TYPE>(get_result_from_payload.fbrxState, get_result_from_payload));
						start_offset = offset + (GSM_V4_FBRx_GAIN_Result_Size+1)+5;
					}
					else
					{
						map<unsigned int, CGSM_channelVsResult_type>::iterator bandToFBRxResult_itr = chainToFBRxResult_itr->second.data.find(m_curBand);

						if(chainToFBRxResult_itr->second.data.end() == bandToFBRxResult_itr)
						{
						   chainToFBRxResult.data[0].data[m_curBand].data[get_result_from_payload.channel].data.insert(pair<unsigned char, GSM_V4_FBRx_GAIN_RESULT_TYPE>(get_result_from_payload.fbrxState, get_result_from_payload));
						   start_offset = offset + (GSM_V4_FBRx_GAIN_Result_Size+1)+5;
						}
						else
						{
							map<unsigned int, CGSM_fbrxGainVsResult_type>::iterator chanToFBRxResult_itr = bandToFBRxResult_itr->second.data.find(get_result_from_payload.channel);

   					        if(bandToFBRxResult_itr->second.data.end() == chanToFBRxResult_itr)
							{
								chainToFBRxResult.data[0].data[m_curBand].data[get_result_from_payload.channel].data.insert(pair<unsigned char, GSM_V4_FBRx_GAIN_RESULT_TYPE>(get_result_from_payload.fbrxState, get_result_from_payload));
								start_offset = offset + (GSM_V4_FBRx_GAIN_Result_Size+1)+5;
							}
						}
					}
				}
			}
            offset += (GSM_V4_FBRx_GAIN_Result_Size+1)+5;
         }
      }
   }



   return true;
}


unsigned char RFCal::GSM_Cal::FBRx_Cal::Compute_NumFrames(HANDLE hPhone, GSM_CAL_Params & params)
{
   // Note: m_txNumSlots and m_rxNumSlots have already been verified to not equal zero.
    CGSM_V4_Cal_Params & gsm_v4_Params = params.gsm_v4_Params;
   if( gsm_v4_Params.m_Rx_Params.performRxCal )
   {
      gsm_v4_Params.m_numFramesPerChan = gsm_v4_Params.m_Rx_Params.numLNAState * 2;
   }
   else
   {
      if(gsm_v4_Params.m_FBRx_Params.fbrx_gain_state_list.size() > 4)
         gsm_v4_Params.m_numFramesPerChan = 2;
      else
         gsm_v4_Params.m_numFramesPerChan = 1;
   }
   // The # of frames from the UE perspective
   gsm_v4_Params.m_numFrames = gsm_v4_Params.m_numFramesPerChan * gsm_v4_Params.m_bcc.m_numBands * gsm_v4_Params.m_bcc.m_numChan;

   // The # of frames from the Call Box (CB) perspective
   gsm_v4_Params.m_numFramesForCB = gsm_v4_Params.m_numFrames + GSM_V4_INTRA_BAND_FRAME_DELAY*(gsm_v4_Params.m_bcc.m_numBands - 1);

   return true;
}

unsigned char RFCal::DACharV2::SetData( int rfMode, unsigned short antPath, int sigPath, int rfmDev, int temp, int channel, int mod, int pa, map <int, double> rgiVsPwr)
{
	daCharV2data.data[rfMode].data[antPath].data[sigPath].data[rfmDev].data[temp].data[channel].data[mod].data[pa].rgiVsPwr = rgiVsPwr;
	return true;
	
}

unsigned char RFCal::DACharV2::GetData ( GSMDACharV2_RfMode & gData )
{

	gData = this -> daCharV2data;
	
	return (!gData.data.empty());

}
unsigned char RFCal::DACharV2::Init () 
{
   this-> daCharV2data.data.clear();
   return true;
}

unsigned char RFCal::GSMRxChar::SetData( int rfMode, int chain, int channel, map<int, double> lnaVsGain )
{
	gsmRxChardata[chain][rfMode][channel] = lnaVsGain;
	return true;
}

unsigned char RFCal::GSMRxChar::GetData( GSMRxCharData_ChainBand & g_data )
{
	g_data = this -> gsmRxChardata;

	return (!g_data.empty());
}

unsigned char RFCal::GSMRxChar::Init () 
{
   this-> gsmRxChardata.clear();
   return true;
}

unsigned char RFCal::GSM_Cal::GSM_V5::InputFile::CharXMLPath(HANDLE hPhone, GSM_CAL_Params & params)
{

	// ****************************** //
    // Set Parameter XML Paths
    // ****************************** //

    string param_folder = "c:\\temp\\"; // Set working folder correctly, otherwise relative does not work.

	params.FilePath.stCharXML    = param_folder + "SDR845_HPUE_SAWLESS_B21_ESC_Char.xml";

	return true;

}

unsigned char RFCal::GSM_Cal::GSM_V5::InputFile::ReadCharXML(HANDLE hPhone, GSM_CAL_Params & params)
{

	//**************************************************//
	//         Load XML 
	//**************************************************//
	MSXML2::IXMLDOMDocumentPtr spXMLDoc;   
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	spXMLDoc->load(params.FilePath.stCharXML.c_str());
	if (!spXMLDoc)
	{
		stringstream ss;
		ss << "The param XML not loaded. " << params.FilePath.stCharXML.c_str();
		return false;
	}

	// ***************************************************************//
	//      GSM DA Char V2 XML
	//************************************++**************************//

	MSXML2::IXMLDOMNodeListPtr pGSMDaCharV2List = spXMLDoc->selectNodes((_bstr_t)"/RFCal_Data/GSMDaCharTableV2"); 

	if (!pGSMDaCharV2List)
	{
		spXMLDoc.Release();
		return false;
	}

	// ***************************************************************//
	//      Read each of nodes and assign GSM DA Char V2 table map
	// ***************************************************************//
	int rfMode = 0;
	unsigned short antPath = 0;
	int sigPath = 0;
	int rfmDev = 0;
	int temp = 25;
	int channel = 0;
	int mod = 0;
	int pa = 0;
	int rgi = 0;
	double pwr = 0;

	for (int i= 0; i < pGSMDaCharV2List->length; i++)
	{
		MSXML2::IXMLDOMNodePtr pGSMDaCharV2Node = pGSMDaCharV2List->nextNode();
		if ( NULL == pGSMDaCharV2Node)
			continue;

		
		RFCal::Helper::XML::Read_Attribute(pGSMDaCharV2Node, "rf_mode", rfMode);	
		RFCal::Helper::XML::Read_Attribute(pGSMDaCharV2Node, "ant_path", antPath);	
		RFCal::Helper::XML::Read_Attribute(pGSMDaCharV2Node, "sig_path", sigPath);
		RFCal::Helper::XML::Read_Attribute(pGSMDaCharV2Node, "rfm_dev", rfmDev);
        if ( ! RFCal::Helper::XML::Read_Attribute(pGSMDaCharV2Node, "temperature", temp))
		{
			temp = 25;
		}


		MSXML2::IXMLDOMNodeListPtr pGSMChanModPaList = pGSMDaCharV2Node->selectNodes((_bstr_t)"GSMChanModPa"); 

		for (int table_idx = 0 ; table_idx < pGSMChanModPaList->length; table_idx++)
		{
			MSXML2::IXMLDOMNodePtr pTableNode = pGSMChanModPaList->nextNode();
			if ( NULL == pTableNode)
				continue;

			RFCal::Helper::XML::Read_Attribute(pTableNode, "channel", channel);
			RFCal::Helper::XML::Read_Attribute(pTableNode, "mod", mod);
			RFCal::Helper::XML::Read_Attribute(pTableNode, "pa", pa);

			MSXML2::IXMLDOMNodeListPtr pGSMRgiPwrList = pTableNode->selectNodes((_bstr_t)"GSMRgiPwr");

			std::map <int, double> rgiVsPwr;

			if ( NULL == pGSMRgiPwrList )
				continue;
			for (int GSMRgiPwrIndex = 0; GSMRgiPwrIndex < pGSMRgiPwrList->length; GSMRgiPwrIndex++)
			{
				MSXML2::IXMLDOMNodePtr pGSMRgiPwrNode = pGSMRgiPwrList->nextNode();
				if ( NULL == pGSMRgiPwrNode)
					continue;

				RFCal::Helper::XML::Read_Attribute(pGSMRgiPwrNode, "rgi", rgi);
				RFCal::Helper::XML::Read_Attribute(pGSMRgiPwrNode, "pwr", pwr);

				rgiVsPwr[rgi] = pwr;
				
				pGSMRgiPwrNode.Release();
			}   
			if ( ! rgiVsPwr.empty() )
			{
				// ***************************************************************//
				//      Store the set of char data into the repository
				// ***************************************************************//
				params.repository.SetDACharData( rfMode, antPath, sigPath, rfmDev, temp, channel, mod, pa, rgiVsPwr );			
			}
			pGSMRgiPwrList.Release();
		}
		pGSMChanModPaList.Release();
	}

	pGSMDaCharV2List.Release();

	// ***************************************************************//
	//      GSM Rx Char V2 XML
	//************************************++**************************//

	MSXML2::IXMLDOMNodeListPtr pGSMRxCharList = spXMLDoc->selectNodes((_bstr_t)"/RFCal_Data/RxGainTable"); 

	if (!pGSMRxCharList)
	{
		spXMLDoc.Release();
		return false;
	}

	// ***************************************************************//
	//      Read each of nodes and assign GSM Rx Char table map
	// ***************************************************************//
	rfMode = 0;
	channel = 0;
	int chain = 0;
	int lna = 0;
	double gain = 0;
	int instance = 0;
	for (int i= 0; i < pGSMRxCharList->length; i++)
	{
		MSXML2::IXMLDOMNodePtr pGSMRxCharNode = pGSMRxCharList->nextNode();
		if ( NULL == pGSMRxCharNode)
			continue;
		
		RFCal::Helper::XML::Read_Attribute(pGSMRxCharNode, "rf_mode", rfMode);	
		RFCal::Helper::XML::Read_Attribute(pGSMRxCharNode, "chain", chain);	
		RFCal::Helper::XML::Read_Attribute(pGSMRxCharNode, "instance", instance);

		MSXML2::IXMLDOMNodeListPtr pGSMRxGainCharList = pGSMRxCharNode->selectNodes((_bstr_t)"RxGainChar"); 

		for ( int table_idx = 0; table_idx< pGSMRxGainCharList->length ;table_idx++)
		{
			MSXML2::IXMLDOMNodePtr pTableNode = pGSMRxGainCharList->nextNode();
			if ( NULL == pTableNode)
				continue;

			RFCal::Helper::XML::Read_Attribute(pTableNode, "channel", channel);
			
			MSXML2::IXMLDOMNodeListPtr pGSMRxGainList = pTableNode->selectNodes((_bstr_t)"RxGain");

			std::map <int, double> lnaVsGain;

			if ( NULL == pGSMRxGainList )
				continue;
			for (int GSMRxGainIndex = 0; GSMRxGainIndex < pGSMRxGainList->length; GSMRxGainIndex++)
			{
				MSXML2::IXMLDOMNodePtr pGSMRxGainNode = pGSMRxGainList->nextNode();
				if ( NULL == pGSMRxGainNode)
					continue;

				RFCal::Helper::XML::Read_Attribute(pGSMRxGainNode, "lna", lna);
				RFCal::Helper::XML::Read_Attribute(pGSMRxGainNode, "gain", gain);

				lnaVsGain[lna] = gain;

				pGSMRxGainNode.Release();
			}   
			if ( ! lnaVsGain.empty() )
			{
				// ***************************************************************//
				//      Store the set of char data into the repository
				// ***************************************************************//
				params.repository.SetGSMRxCharData( rfMode, chain, channel, lnaVsGain );			
			}
			pGSMRxGainList.Release();
		}
		pGSMRxGainCharList.Release();
	}

	pGSMRxCharList.Release();

	spXMLDoc.Release();

	return true;
}
unsigned int RFCal::GSM_Cal::GSM_V5::InputFile::GetGsmConsolidatedNvID( int rfMode )
{

	unsigned int nvID = 0;

	switch(rfMode)
	{
	case NV_MODE_GSM_850:
		// 29230
		nvID = RFNV_GSM_GSM850_TX_CAL_DATA_I;
		break;
	case NV_MODE_GSM_EGSM:
		// 29231
		nvID = RFNV_GSM_GSM900_TX_CAL_DATA_I;
		break;
	case NV_MODE_GSM_DCS:
		// 29232
		nvID = RFNV_GSM_GSM1800_TX_CAL_DATA_I;
		break;
	case NV_MODE_GSM_1900:
		// 29233 
		nvID = RFNV_GSM_GSM1900_TX_CAL_DATA_I;
		break;
	}

	return nvID;
}
void RFCal::DACharV2::EnsureSortedEGSMChannels( QMSL_GSM_DA_Cal_Result& daCalIn )
{
	// Any channels at 975 to 1023 are lower in frequency than those in the 0 to 124 range.
	// Therefore, any channels >= ARFCN_LOW_FREQ should be placed before those in the 0 to 124 range
	// The map usedfor creating daCalIn is autosorted so any channels of ARFCN_LOW_FREQ or higher will be placed at the end
	const unsigned short ARFCN_LOW_FREQ = 975;

	unsigned int numChan = daCalIn.iNumChannels;

	bool isSorted = false;

	unsigned int chanIdx = 0;

	// return if the channels are already sorted
	for (chanIdx=0; chanIdx<numChan; ++chanIdx)
	{
		if ( daCalIn.iChannel[chanIdx] < ARFCN_LOW_FREQ )
		{
			isSorted = true;
		}
		else
		{
			// exit as soon as isSorted = false
			isSorted = false;
			break;
		}
	}
	if (isSorted)
	{
		return;
	}

	vector<unsigned short> sortedChanOrder;
	sortedChanOrder.assign( daCalIn.iChannel, daCalIn.iChannel + daCalIn.iNumChannels );

	QMSL_GSM_DA_Cal_Result tempDaCal;
	memcpy_s( &tempDaCal, sizeof(QMSL_GSM_DA_Cal_Result), &daCalIn, sizeof(QMSL_GSM_DA_Cal_Result) );

	// look for channels >= ARFCN_LOW_FREQ
	// rotate the sorted channels list to correct order
	while (!isSorted)
	{
		if ( sortedChanOrder[numChan-1] >= ARFCN_LOW_FREQ  )
		{
			rotate( sortedChanOrder.begin(), sortedChanOrder.end()-1, sortedChanOrder.end() );
		}
		else
		{
			isSorted = true;
		}
	}

	unsigned short curChan     = 0;   // current channel
	unsigned int   curChanIdx  = 0;   // index of the current channel in the daCal struct

	// sort the data by sortedChannel in the daCal struct
	for (chanIdx=0; chanIdx<numChan; ++chanIdx)
	{
		curChan = sortedChanOrder[chanIdx];

		tempDaCal.iChannel[chanIdx] = curChan;

		// find the array index referring to curChan
		for (curChanIdx=0; curChanIdx<numChan; ++curChanIdx)
		{
			if (daCalIn.iChannel[curChanIdx] == curChan)
			{
				break;
			}
		}

		// copy all of the data over for each channel
		for (unsigned int paIdx=0; paIdx<daCalIn.iNumMeasuredTxGainStates; ++paIdx)
		{
			for (unsigned short rgiIdx=0; rgiIdx<daCalIn.iNumTxPowerMeasurementsPerRange; ++rgiIdx)
			{
				tempDaCal.aiEDGETxPowerMeas[chanIdx][paIdx][rgiIdx] = daCalIn.aiEDGETxPowerMeas[curChanIdx][paIdx][rgiIdx];
				tempDaCal.aiGSMTxPowerMeas[chanIdx][paIdx][rgiIdx]  = daCalIn.aiGSMTxPowerMeas[curChanIdx][paIdx][rgiIdx];
				tempDaCal.aiGSMTxRGI[chanIdx][paIdx][rgiIdx]        = daCalIn.aiGSMTxRGI[curChanIdx][paIdx][rgiIdx];
				tempDaCal.aiEDGETxRGI[chanIdx][paIdx][rgiIdx]       = daCalIn.aiEDGETxRGI[curChanIdx][paIdx][rgiIdx];
			}
		}
	}

	// copy tempDaCal with data in the correct channel order to daCalIn
	memcpy_s( &daCalIn, sizeof(QMSL_GSM_DA_Cal_Result), &tempDaCal, sizeof(QMSL_GSM_DA_Cal_Result) );

}


unsigned char RFCal::GSM_Cal::GSM_V5::InputFile::writeDACharToConsolidatedNV(HANDLE hPhone, GSM_CAL_Params & params)
{
	DACharV2 gsmDaCharObj;
	GSMDACharV2_RfMode gData;
	const unsigned char cacheNVItems = 0;  // set cache NV items to false
	const unsigned int reqNumMod     = 2;  // required number of modulations per band/antPath/sigPath/rfmDev/channel
	stringstream ostr;
	if ( ! params.repository.GetDACharData ( gData ) )
	{
		return false;
	}

	unsigned short writeStatus = 0;


	// ---------------- Procedure ------------------//
	// 1) put DA char data into QMSL_GSM_DA_Cal_Result
	// 2) call QMSL_GSM_Tx_Calibration_Consolidated_NV for each item to get the NV struct
	// 3) Write each NV struct to the phone NV

	QMSL_GSM_DA_Cal_Result *curDaCalResult = new QMSL_GSM_DA_Cal_Result;      // struct for DA Cal results to be passed to QMSL
	QMSL_GSM_PreDist_Cal_Result *curPreDistCalResult = new QMSL_GSM_PreDist_Cal_Result; // struct for Pre Dist Cal results to be passed to QMSL
	QMSL_GSM_Tx_Calibration_Consolidated_NV *writeGsmConsolidatedNV = new QMSL_GSM_Tx_Calibration_Consolidated_NV; // GSM Tx calibration NV struct for writing to the phone NV
	// iterators for DA Char data
	map< int, GSMDACharV2_AntPath >::iterator            daByRFMode_MapIter;
	map< unsigned short, GSMDACharV2_SigPath >::iterator daByAntPath_MapIter;
	map< int, GSMDACharV2_RFMDev >::iterator             daBySigPath_MapIter;
	map< int, GSMDACharV2_Temp >::iterator                  daByRFMDev_MapIter;
	map< double, GSMDACharV2_Chan >::iterator               daByTemp_MapIter;
	map< int, GSMDACharV2_Mod >::iterator                daByChan_MapIter;
	map< int, GSMDACharV2_PA >::iterator                 daByMod_MapIter;
	map< int, GSMDACharV2_RGI >::iterator                daByPA_MapIter;
	map< int, double >::iterator                         daByRGI_MapIter;

	// current parameters during iteration
	unsigned short curRfMode  = 0;       // current RFMode (band)
	unsigned short curAntPath = 0;       // current Tx Antenna Path
	unsigned short curSigPath = 0;       // current Signal Path
	unsigned short curRfmDev  = 0;       // current RFM Device
	double         curTemp    = 0;       // current Temperature
	unsigned short curNvId    = 0;       // current NV ID
	unsigned short curMod     = 0;       // current modulation (GMSK or 8PSK)

	// indexes for QMSL_GSM_DA_Cal_Result
	unsigned int chanIdx = 0;
	unsigned int paIdx   = 0;
	unsigned int rgiIdx  = 0;

	// reset the curPreDistCalResult struct
	memset( curPreDistCalResult, 0, sizeof(QMSL_GSM_PreDist_Cal_Result) );

	//------------------------ ITERATE over RfMode (band) --------------------------//
	for( daByRFMode_MapIter = gData.data.begin(); daByRFMode_MapIter != gData.data.end(); ++daByRFMode_MapIter )
	{
		curRfMode = static_cast<unsigned short>(daByRFMode_MapIter->first);

		curNvId  = GetGsmConsolidatedNvID( curRfMode );   // the NV ID that will be written to the UE

		if ( ! curNvId )
		{
			return false;
		}
		// reset the curDaCalResult and writeGsmConsolidatedNV struct for each RfMode
		memset( curDaCalResult, 0, sizeof(QMSL_GSM_DA_Cal_Result) );
		memset( writeGsmConsolidatedNV, 0, sizeof(QMSL_GSM_Tx_Calibration_Consolidated_NV) );

		writeGsmConsolidatedNV->iDatatypeVer = 2;

		// initialize the packetSize (length of binary Tx_Cx NV)
		unsigned int packetSize = sizeof(QMSL_GSM_Tx_Cal_NV_Header_Type) + sizeof(QMSL_Variant_Marker_Type);

		//------------------------ ITERATE over Tx Antenna Path --------------------------//
		for( daByAntPath_MapIter = daByRFMode_MapIter->second.data.begin(); daByAntPath_MapIter != daByRFMode_MapIter->second.data.end(); ++daByAntPath_MapIter )
		{
			curAntPath = static_cast<unsigned short>(daByAntPath_MapIter->first);

			//------------------------ ITERATE over Signal Path --------------------------//
			for( daBySigPath_MapIter = daByAntPath_MapIter->second.data.begin(); daBySigPath_MapIter != daByAntPath_MapIter->second.data.end(); ++daBySigPath_MapIter )
			{
				curSigPath = static_cast<unsigned short>(daBySigPath_MapIter->first);
				curDaCalResult->iTxNvContainer = curDaCalResult->iTxSigPath =  (unsigned char)curSigPath;

				//------------------------ ITERATE over RFM Device --------------------------//
				for( daByRFMDev_MapIter = daBySigPath_MapIter->second.data.begin(); daByRFMDev_MapIter != daBySigPath_MapIter->second.data.end(); ++daByRFMDev_MapIter )
				{
					curRfmDev = static_cast<unsigned short>(daByRFMDev_MapIter->first);

					//------------------------ ITERATE over Temperature --------------------------//
					for( daByTemp_MapIter = daByRFMDev_MapIter->second.data.begin(); daByTemp_MapIter != daByRFMDev_MapIter->second.data.end(); ++daByTemp_MapIter )
					{
						curTemp = static_cast<unsigned short>(daByTemp_MapIter->first);

						if(curTemp == 25)//Room Temperature
						{
							// Fill the freq mappings list
							for (unsigned char freqIdx=0; freqIdx<QMSL_GSM_MAX_TX_CAL_CHANNELS; ++freqIdx)
							{
								curDaCalResult->iFreqMapping[freqIdx] = static_cast<QMSL_GSM_Polar_Cal_Frequency>( freqIdx+1 );
							}

							curDaCalResult->iNumChannels = daByTemp_MapIter->second.data.size();

							// Verify the required number of channels exist
							if ( curDaCalResult->iNumChannels != QMSL_GSM_MAX_TX_CAL_CHANNELS )
							{
								
								ostr.str("");
								ostr << "Expected " << QMSL_GSM_MAX_TX_CAL_CHANNELS << " channels of char DA data, received " << curDaCalResult->iNumChannels
									<< ".\n band: " << curRfMode << "\n antenna path: " << curAntPath << "\n signal path: " << curSigPath << "\n RFM device: " << curRfmDev
									<< __FUNCTION__ << " " << __LINE__
									<< "\n Please check the char DA data to ensure that all required channels are present.";

								// de-allocate memory before throwing exception
								delete curDaCalResult;
								delete curPreDistCalResult;
								delete writeGsmConsolidatedNV;

								RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
								return false;
							}

							//------------------------ ITERATE over channels ------------------------//
							for( chanIdx = 0, daByChan_MapIter = daByTemp_MapIter->second.data.begin(); daByChan_MapIter != daByTemp_MapIter->second.data.end(); ++daByChan_MapIter, ++chanIdx )
							{

								curDaCalResult->iChannel[chanIdx] = static_cast<unsigned short>(daByChan_MapIter->first);

								// verify the required number of modulations exist
								if ( daByChan_MapIter->second.data.size() != reqNumMod  )
								{
									
									ostr.str("");
									ostr << "Expected " << reqNumMod << " modulations of char DA data for "
										<< ".\n band: " << curRfMode << "\n antenna path: " << curAntPath << "\n signal path: " << curSigPath << "\n RFM device: " << curRfmDev
										<< "\n chan: " << curDaCalResult->iChannel[chanIdx] << __FUNCTION__ << " " << __LINE__
										<< "\n Please check the char DA data to ensure both modulations are present.";

									// de-allocate memory before throwing exception
									delete curDaCalResult;
									delete curPreDistCalResult;
									delete writeGsmConsolidatedNV;

									RFCal::Helper::Log::AddMessageToQMSLLog (hPhone, ostr.str());
									return false;
								}

								//----------------------- ITERATE over modulation --------------------//
								for( daByMod_MapIter=daByChan_MapIter->second.data.begin(); daByMod_MapIter!=daByChan_MapIter->second.data.end(); ++daByMod_MapIter )
								{
									curMod = static_cast<unsigned short>(daByMod_MapIter->first);

									curDaCalResult->iNumMeasuredTxGainStates = daByMod_MapIter->second.data.size();

									//----------------------- ITERATE over PA states -------------------//
									for( paIdx=0, daByPA_MapIter=daByMod_MapIter->second.data.begin(); daByPA_MapIter!=daByMod_MapIter->second.data.end(); ++daByPA_MapIter, ++paIdx )
									{
										curDaCalResult->aiTxMeasToNVMapping[paIdx] = static_cast<unsigned short>(daByPA_MapIter->first);

										curDaCalResult->iNumTxPowerMeasurementsPerRange = static_cast<unsigned short>(daByPA_MapIter->second.rgiVsPwr.size());

										//----------------------- ITERATE over RGI --------------------------//
										for( rgiIdx=0, daByRGI_MapIter=daByPA_MapIter->second.rgiVsPwr.begin(); daByRGI_MapIter!=daByPA_MapIter->second.rgiVsPwr.end(); ++daByRGI_MapIter, ++rgiIdx )
										{
											switch (curMod)
											{
											case QMSL_GSM_PwrErr_Per_PCL_GMSK_Type :
												{
													curDaCalResult->aiGSMTxRGI[chanIdx][paIdx][rgiIdx]        = static_cast<unsigned short>(daByRGI_MapIter->first);
													curDaCalResult->aiGSMTxPowerMeas[chanIdx][paIdx][rgiIdx]  = daByRGI_MapIter->second;
													break;
												}
											case QMSL_GSM_PwrErr_Per_PCL_8PSK_Type :
												{
													curDaCalResult->aiEDGETxRGI[chanIdx][paIdx][rgiIdx]        = static_cast<unsigned short>(daByRGI_MapIter->first);
													curDaCalResult->aiEDGETxPowerMeas[chanIdx][paIdx][rgiIdx]  = daByRGI_MapIter->second;
													break;
												}
											}
										}
									}  //----- END PA LOOP

									// add payload size for PMEAS
									packetSize += ( sizeof(QMSL_GSM_PMEAS_List_Variant_Type) * paIdx );

								}  //----- END MOD LOOP

							}  //----- END CHAN LOOP

							// add payload size for RGI
							packetSize += ( sizeof(QMSL_GSM_RGI_List_Variant_Type) * paIdx * reqNumMod );

							//------------------------- Make Sure EGMS data is written in the correct order, i.e. 975, 37, 124 -----------------------//
							if ( curRfMode == PHONE_MODE_GSM_900)
							{
								gsmDaCharObj.EnsureSortedEGSMChannels( *curDaCalResult );
							}

							//------------------------- WRITE TO NV ------------------------//
							// Get the NV Item Struct
							QLIB_RFCal_GSM_Tx_DA_Cal_IQ_Predist_Cal_Generate_Consolidated_NV (
								hPhone,
								static_cast<unsigned short>(curRfMode),
								static_cast<unsigned char>(curAntPath),
								static_cast<unsigned char>(curRfmDev),
								curDaCalResult,
								curPreDistCalResult,
								writeGsmConsolidatedNV,
								cacheNVItems);

							// Write the NV to the phone
							// packetSize reflect the actual size of the NV payload
							// GSM_V4_MAX_PACKET_SIZE indicates the max diag write size
							QLIB_FTM_CALL_RFNV_WRITE_V2( hPhone, curNvId, writeGsmConsolidatedNV->oNVitem.aRFNVItemData, static_cast<unsigned short>(packetSize), GSM_V4_MAX_PACKET_SIZE, &writeStatus );

							if( writeStatus != RFCOMMON_RFNV_WRITE_SUCCESS )
							{

								ostr.str("");
								ostr << "Failed to write DA Char data to NV: " << curNvId << __FUNCTION__ << " " << __LINE__ << "\n"
									<< "This indicates a potential phone communication problem.\n";
								
								// de-allocate memory before throwing exception
								delete curDaCalResult;
								delete curPreDistCalResult;
								delete writeGsmConsolidatedNV;

								RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str());
								return false;
							}

						}

					}  //----- END TEMPERATURE LOOP

				}  //----- END RFM DEVICE LOOP

			}  //----- END SIGNAL PATH LOOP

		}  //----- END ANTENNA PATH LOOP

	}  //----- END RF MODE LOOP
		//de-allocate memory
	delete curDaCalResult;
	delete curPreDistCalResult;
	delete writeGsmConsolidatedNV;


	return true;
}

unsigned char RFCal::GSM_Cal::GSM_V4::Do_GSM_V4_Cleanup( HANDLE hPhone )
{
   unsigned char status        = 1;
   unsigned short dummyChainID = 0; // dummy chain ID for setting the proper mode for the FTM command

   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, dummyChainID, FTM_MODE_ID_GSM_EXTENDED_C);

   status = QLIB_FTM_GSM_V4_CAL_CLEANUP( hPhone );

   if(status == 0)
   {
      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, "QLIB_FTM_GSM_V4_CAL_CLEANUP() API failed.");
      return false;
   }
   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V4::Get_GSM_V4_Sweep_Results( HANDLE hPhone, GSM_V4_FTM_CMD_Response_Type& gsmV4_measResults )
{
   unsigned char status        = 1;
   unsigned short dummyChainID = 0; // dummy chain ID for setting the proper mode for the FTM command

   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, dummyChainID, FTM_MODE_ID_GSM_EXTENDED_C);

   status = QLIB_FTM_GSM_V4_CAL_GET_RESULTS( hPhone, (void*)&gsmV4_measResults );

   if(status == 0)
   {
      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, "QLIB_FTM_GSM_V4_CAL_GET_RESULTS() API failed.");
      return false;
   }
   return true;
}
unsigned char RFCal::GSM_Cal::GSM_V4::Do_GSM_V4_Sweep( HANDLE hPhone, vector < unsigned char > & gsm_V4_cmd, unsigned short gsmV4CmdLen, const vector<unsigned int>& chainID_Vec, bool compressed )
{

   unsigned short dummyChainID = 0; // dummy chain ID for setting the proper mode for the FTM command
   unsigned char status        = 1;

   //----- Step 0: Set the Mode ID
   RFCal::GSM_Cal::FTM_SET_MODE_ID_GSM(hPhone, dummyChainID, FTM_MODE_ID_GSM_EXTENDED_C);

   //----- Step 1: Disable predistortion for the # of chains required
   for (unsigned int chainIdx=0; chainIdx<chainID_Vec.size(); ++chainIdx)
   {
      status = QLIB_FTM_GSM_V4_DISABLE_PREDIST( hPhone, static_cast<unsigned short>(chainID_Vec[chainIdx]) );
      if(status == 0)
      {
         // failure
         return false;
      }
   }

   //----- Step 2: Configure the GSM Frame Sweep
   if(compressed == FALSE)
   {
      status = QLIB_FTM_GSM_V4_CAL( hPhone, (void*)&gsm_V4_cmd[0], gsmV4CmdLen );
      if(status == 0)
      {
         return false;
      }
   }
   else
   {
      status = QLIB_FTM_GSM_V4_CAL_COMPRESSED( hPhone, (void*)&gsm_V4_cmd[0], gsmV4CmdLen );
      if(status == 0)
      {
         return false;
      }
   }

   //---- Step 3: Execute the GSM Frame Sweep
   status = QLIB_FTM_GSM_V4_CAL_EXECUTE_SWEEP( hPhone );
   if(status == 0)
   {
      return false;
   }
   return true;
}
unsigned char RFCal::GSM_Cal::FBRx_Cal::FillCallBoxParams(HANDLE hPhone, GSM_CAL_Params & params)
{
   CGSM_V4_Cal_Params & m_gsmV4Params = params.gsm_v4_Params;
   TE_GSMV4_Sweep_Params& swpParams = params.gsm_v4_Params.swpParams;
   
   swpParams.bandVec = m_gsmV4Params.m_bcc.m_bandList;
   swpParams.chanVec = m_gsmV4Params.m_bcc.m_chanList;

   swpParams.performDaCal            = false;// m_curCalParams.m_DA_Params.performDaCal != 0;
   swpParams.performDpdCal           = false;//m_curCalParams.m_DPD_Params.performDpdCal || m_curCalParams.m_DPD_Params.doRGIRunOnly;
   swpParams.performRxCal            = false;//m_curCalParams.m_Rx_Params.performRxCal != 0;
   swpParams.performHiLinRxCal       = false;//m_curCalParams.m_Rx_Params.performHiLinRxCal;
   swpParams.performNonCharRxOnlyCal = false;

   swpParams.dpdMod  = 0;  // 8PSK

   swpParams.intraBandFrameDelay = GSM_V4_INTRA_BAND_FRAME_DELAY;


   swpParams.numTotalFrames = static_cast<unsigned short>( m_gsmV4Params.m_numFramesForCB );

   swpParams.numBand        = static_cast<unsigned short>( m_gsmV4Params.m_bcc.m_numBands );
   swpParams.numChanPerBand = static_cast<unsigned short>( m_gsmV4Params.m_bcc.m_numChan );

   swpParams.numDaFramesPerChan  = 0;//s0;tatic_cast<unsigned short>( m_curCalParams.Get_NumDaFramesPerChan() );
   swpParams.numDpdFramesPerChan = 0;//;static_cast<unsigned short>( m_curCalParams.Get_NumDpdFramesPerChan() );

   swpParams.rxPowerList   = m_gsmV4Params.m_Rx_Params.lnaPowerListAllMode;
   swpParams.rxSlotOffset  = m_gsmV4Params.m_rxSlotOffset;

   swpParams.segregateDaAndDpdPerChan = true;
   swpParams.txSlotShiftAmt           = m_gsmV4Params.m_txShiftAmt;
   swpParams.slotConfig               = m_gsmV4Params.m_TxSlotConfig_CB;

   swpParams.performDaCal = true;
   swpParams.numDpdFramesPerChan = 0;
   swpParams.numDaFramesPerChan  = static_cast<unsigned short>( m_gsmV4Params.m_numFramesPerChan );
   swpParams.rxSlotOffset  = 0;

   m_gsmV4Params.m_intraBandSegDelay = GSM_V4_INTRA_BAND_SEG_DELAY;
   params.gsm_v4_Params.m_sweepParamsTestEquip.m_num_steps = m_gsmV4Params.m_numFrames*GSM_V4_MAX_NUM_OF_SLOTS 
      + m_gsmV4Params.m_intraBandSegDelay*(m_gsmV4Params.m_bcc.m_numBands - 1);

   return true;
}
unsigned char RFCal::GSM_Cal::FBRx_Cal::Create_GSM_FBRx_Cal_Command( const GSM_CAL_Params & params, vector < unsigned char > & gsmV4CmdBuff, unsigned short& gsmV4CmdSize,
                                                                map< unsigned int, vector<unsigned short> >& RX_BURST_FRAME_IDENTIFIER )
{
   unsigned char* curPtr_gsmV4CmdBuff = &gsmV4CmdBuff[0];
   const CGSM_V4_Cal_Params & v4_params = params.gsm_v4_Params;
   const GSM_V4_FBRx_Params & m_FBRx_Params = params.gsm_v4_Params.m_FBRx_Params;
   const GSM_V4_Rx_Params & m_Rx_Params = params.gsm_v4_Params.m_Rx_Params;

   // ensure gsmV4CmdSize is 0 initially
   gsmV4CmdSize = 0;

   // filling the GSM_V4_Cal_Params_Type header
   GSM_V4_Cal_Config_Header cmdCfgHeader;
   if(params.performRxCal)
      cmdCfgHeader.numFrames = static_cast<unsigned short>( v4_params.m_numFrames );
   else
      cmdCfgHeader.numFrames = static_cast<unsigned short>( v4_params.m_numFrames + 1);
   cmdCfgHeader.numChains = static_cast<unsigned short>( v4_params.m_bcc.m_numTxChains );
   cmdCfgHeader.reserved  = 0;
   for (unsigned char i=0; i< 2 /* v4_params.m_bcc.m_numTxChains */; ++i)
   {
      cmdCfgHeader.a_devId[i] = 0;  // no use
   }

   // copy the GSM_V4_Cal_Config_Header to the gsmV4CmdBuff
   memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &cmdCfgHeader, sizeof(GSM_V4_Cal_Config_Header) );
   curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Cal_Config_Header);
   gsmV4CmdSize += sizeof(GSM_V4_Cal_Config_Header);

   // 8 slots per TDMA frame
   const unsigned char  numSlotsPerFrame = GSM_V4_MAX_NUM_OF_SLOTS;

   unsigned char frameSlotIdx  = 0;   // the frame slot index, 0 to 7
   unsigned char curFBRxIdx     = 0;   // the current FBRx index
   unsigned char curLnaIdx     = 0;   // the current LNA index
   unsigned char curChanIdx    = 0;   // the current channel index
   unsigned char curBandIdx    = 0;   // the current band index
   unsigned short frameIdx     = 0;   // the current frame index

   unsigned char curTxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp
   unsigned char curRxSlotFunc = 0;   // the current slot function: Rx/Tx/NoOp

   GSM_V4_Frame_Header curFrameHeader; // current frame header; values updated for each frame
   GSM_V4_Slot_Payload curSlotPayload; // current slot payload; added to each frame for each slot
   memset( &curFrameHeader, 0, sizeof(GSM_V4_Frame_Header) );
   memset( &curSlotPayload, 0, sizeof(GSM_V4_Slot_Payload) );

   unsigned short rxFrameCntr = 0;  // identifies the frames in which an Rx burst was scheduled; used for result extraction in Process_Rx_Meas_Results

   //--------------------------------------------------- The BAND LOOP -----------------------------------------------------------//

   for (curBandIdx=0; curBandIdx< v4_params.m_bcc.m_numBands; ++curBandIdx)
   {
      //---------------------------------------- The FBRx/Rx CHANNEL LOOP ------------------------------------------------//

      for (curChanIdx=0; curChanIdx< v4_params.m_bcc.m_numChan; ++curChanIdx)
      {
         //reset LNA index to the first LNA index for the current channel
         curLnaIdx = 0;

         // Fill FBRx/Rx sweep
         for(frameIdx=0 ; frameIdx< v4_params.m_numFramesPerChan ; ++frameIdx)
         {
            //------------------------------ fill the frame header ------------------------------//
            curFrameHeader.band    = static_cast<unsigned char>( v4_params.m_bcc.m_bandList[curBandIdx] );
            curFrameHeader.channel = static_cast<unsigned short>( v4_params.m_bcc.m_chanList[curBandIdx][curChanIdx] );
            switch(frameIdx)
            {
               case 0:
                  if( v4_params.m_FBRx_Params.fbrx_gain_state_list.size()<4)
                  {
					      curFrameHeader.numSlotsInPayload =0;  
                     curFrameHeader.numSlotsInPayload = m_FBRx_Params.fbrx_gain_state_list.size()%4;
                     curFrameHeader.numSlotsInPayload = curFrameHeader.numSlotsInPayload*2;
					  
                     if( v4_params.m_Rx_Params.performRxCal)
                        curFrameHeader.numSlotsInPayload++;
                  }
                  else if(m_FBRx_Params.fbrx_gain_state_list.size()==4)
                  {
                     curFrameHeader.numSlotsInPayload = 4;
                     curFrameHeader.numSlotsInPayload = curFrameHeader.numSlotsInPayload*2;
                     if(m_Rx_Params.performRxCal)
                        curFrameHeader.numSlotsInPayload++;
                  }
                  else
                  {
                     curFrameHeader.numSlotsInPayload = 4;
                     curFrameHeader.numSlotsInPayload = curFrameHeader.numSlotsInPayload*2;
                     if( m_Rx_Params.performRxCal)
                        curFrameHeader.numSlotsInPayload++;
                  }
                  break;
               case 1:
                  if(m_FBRx_Params.fbrx_gain_state_list.size()==5)
                  {
                     curFrameHeader.numSlotsInPayload = 1;
                     curFrameHeader.numSlotsInPayload = curFrameHeader.numSlotsInPayload*2;
                     if(m_Rx_Params.performRxCal)
                        curFrameHeader.numSlotsInPayload++;
                  }
                  else
                  {
                     curFrameHeader.numSlotsInPayload = 1;
                  }
                  break;
               case 2:
               case 3:
               case 4:
               case 5:
               case 6:
               case 7:
               case 8:
                  curFrameHeader.numSlotsInPayload = 1;
                  break;
               default :
                  curFrameHeader.numSlotsInPayload = 0;
                  break;
            }
            // copy the frame header to the GSM V4 CMD payload
            memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
            curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Frame_Header);
            gsmV4CmdSize += sizeof(GSM_V4_Frame_Header);

            //------------- loop through slot configuration: m_frameSlotConfig --------------//
            for ( frameSlotIdx=0; frameSlotIdx<numSlotsPerFrame; ++frameSlotIdx )
            {
               curTxSlotFunc = v4_params.m_TxSlotConfig[frameSlotIdx];
               curRxSlotFunc = v4_params.m_RxSlotConfig[frameSlotIdx];

               //--------- RX SLOTS -------//
               if ( curRxSlotFunc > 0 && m_Rx_Params.performRxCal )
               {
                  // ---------- Add chainID[0] slot
                  // fill the slot header
                  curSlotPayload.devID    = cmdCfgHeader.a_devId[0];
                  curSlotPayload.slotNum  = frameSlotIdx;
                  curSlotPayload.slotOpID = Rx_RSSI_Meas_Op;
                  //Note: for high linearity, the MSB of LNA state byte in m_Rx_Params.lnaStateListAllMode is already set to 1.
                  curSlotPayload.slotPayload.rssiPayload.lnaState = static_cast<unsigned char>( m_Rx_Params.lnaStateListAllMode[curLnaIdx] );

                  // copy the slot payload over
                  memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                  curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                  gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                  // ---------- Add chainID[1] slot
                  if (v4_params.m_bcc.m_numRxChains > 1)
                  {
                     // fill the slot header
                     curSlotPayload.devID = cmdCfgHeader.a_devId[1];
                     // copy the slot payload over
                     memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                     gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);
                  }
                  rxFrameCntr = (unsigned short)(v4_params.m_numFramesPerChan*curBandIdx*v4_params.m_bcc.m_numChan + curChanIdx*v4_params.m_numFramesPerChan + frameIdx);
                  RX_BURST_FRAME_IDENTIFIER[curFrameHeader.band].push_back( rxFrameCntr );

                  // increment to the next LNA state; reset the LNA index if limit is reached.
                  ++curLnaIdx;
                  if ( curLnaIdx >= m_Rx_Params.lnaStateListAllMode.size() )
                  {
                     curLnaIdx = 0;
                  }
               }

               //--------- FBRx SLOTS --------//
               if (( curTxSlotFunc > 0 )&&(curFBRxIdx < m_FBRx_Params.fbrx_gain_state_list.size()))
               {
                  // ---------- Add chainID[0] slot

                  curSlotPayload.devID    = cmdCfgHeader.a_devId[0];
                  curSlotPayload.slotNum  = frameSlotIdx;
                  curSlotPayload.slotOpID = FBRx_Hw_Meas_Op;

                  curSlotPayload.slotPayload.fbrxHwPayload.expPwr = (unsigned char)static_cast<int>(m_FBRx_Params.exp_pwr_list[curFBRxIdx]*100);

                  // copy the slot payload over
                  memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                  curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                  gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                  // fill the slot header
                  curSlotPayload.devID    = cmdCfgHeader.a_devId[0];
                  curSlotPayload.slotNum  = frameSlotIdx;
                  curSlotPayload.slotOpID = FBRx_Meas_Op;

                  curSlotPayload.slotPayload.fbrxPayload.fbrx_gain_state = (unsigned char)m_FBRx_Params.fbrx_gain_state_list[curFBRxIdx];
                  curSlotPayload.slotPayload.fbrxPayload.pa_state = (unsigned char)m_FBRx_Params.pa_state_list[curFBRxIdx];
                  curSlotPayload.slotPayload.fbrxPayload.rgi = (unsigned char)m_FBRx_Params.rgi_list[curFBRxIdx];
                  curSlotPayload.slotPayload.fbrxPayload.mod_type = (unsigned char)0; //Mode Types 0 = GMSK,1 = 8PSK

                  // copy the slot payload over
                  memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                  curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                  gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);

                  // ---------- Add chainID[1] slot
                  if (v4_params.m_bcc.m_numTxChains > 1)
                  {
                     // fill the slot header - only device ID needs to be changed
                     curSlotPayload.devID = cmdCfgHeader.a_devId[1];
                     // copy the slot payload over
                     memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
                     curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
                     gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);
                  }

                  // increment to the next PCL
                  ++curFBRxIdx;
               }

            }  // END FRAME-SLOT LOOP

         }  // END FBRX/RX SWEEP LOOP
         // reset frameIdx for the next channel
         curFBRxIdx = 0;
         frameIdx = 0;
      }  // END DA Cal Sweep
   }  // END BAND LOOP

   if(!m_Rx_Params.performRxCal)
   {
	  curFrameHeader.numSlotsInPayload = 1;

      memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curFrameHeader, sizeof(GSM_V4_Frame_Header) );
      curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Frame_Header);
      gsmV4CmdSize += sizeof(GSM_V4_Frame_Header);

      memcpy_s( curPtr_gsmV4CmdBuff, sizeof(unsigned char) * GSM_V4_TEMP_CMD_BUFF_SIZE, &curSlotPayload, sizeof(GSM_V4_Slot_Payload) );
      curPtr_gsmV4CmdBuff += sizeof(GSM_V4_Slot_Payload);
      gsmV4CmdSize        += sizeof(GSM_V4_Slot_Payload);
   }


   return true;
}
//! Select_CharDPD_DataSet
/***
By comparing against the DPD data (Power-DigGain table) from measurement, this function selects an RGI and related DPD data set out of characterization database.
At this RGI, the second last entry of the DPD data set from characterization has the power closest to the power at the same DigGain from actual measurement.
The DPD data ( Power-DigGain table, Power-AM-PM table) from characterization at the selected RGI are copied into "daqs" and "ops" as out bound parameters.
The selected RGI is the return value of this function.
*/
unsigned short RFCal::GSM_Cal::GSM_V5::Tx::Select_CharDPD_DataSet_V4( HANDLE hPhone, GSM_CAL_Params &params,unsigned short calRgi, unsigned short band, unsigned short chan, unsigned short chainID, bool useAdaptiveSelection,
                                                           vector<struct opPwrRecord>& ops, vector<struct digRecord>& daqs,  map<unsigned short, GSM_DA_Cal_DigGain_Record>& dutDigGainSweep )
{

   OpPwrTableGeneratorV2* opPwrTablesV2 = params.gsm_v4_Params.m_pwrCharTablePtr;
   stringstream ostr;
   if ( !opPwrTablesV2 || !opPwrTablesV2->isDataPresent() )
   {
      ostr.str("");
      ostr << "No characterized Tx DPD data was found: BAND " << band << ", CHAIN " << chainID << ", CHAN " << chan
           << "\nMake sure the char data input node was run, and that char DPD data exists.\n"
           << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
      return false;
   }

   bool v4CharDigGainExists = opPwrTablesV2->isV4DataPresent();
   static bool displayedMessageAlready = false; // make sure this warning is only printed once
   if ( !v4CharDigGainExists && !displayedMessageAlready )
   {
      displayedMessageAlready = true;

      ostr.str("");
      ostr << "WARNING: No V4 characterized Tx data exists, using V3 data.\nThis may degrade performance. ";
      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
      return false;
   }

   bool foundCharData = false; // indicates if the desired char data table (band,chan,chainID,RGI) exists.

   vector<struct digRecord> v3Daqs;

   // Just get the data for calRgi, if not using adaptive char DPD selection
   if (!useAdaptiveSelection)
   {
      // Check for V4 data preferably, otherwise use V3 --- a nasty warning is displayed if V3 char data is being used
      if (v4CharDigGainExists)
      {
         foundCharData = opPwrTablesV2->getTablesV4( band, chan, chainID, calRgi, ops, v3Daqs, daqs );
      }
      else
      {
         foundCharData = opPwrTablesV2->getTables( band, chan, chainID, calRgi, ops, daqs );
      }

      // make sur the desired data was found
      if (!foundCharData)
      {
         ostr.str("");
         ostr << "Characterized Tx DPD data unavailable for BAND " << band << ", CHAIN " << chainID << ", CHAN " << chan << ", RGI " << calRgi
               << "\nMake sure the char data includes the desired DPD data.";
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }

      return calRgi;
   }

   // Parameters for adaptive char dataset selection with INITIAL values. They are not hardcoded!
   double tarPreDistPow      = 27.0;            // the target predistortion (digital gain) power
   double curPredistPow      = 0.0;             // the char data predistortion (digital gain) power
   unsigned short curRgi     = calRgi;          // current RGI used for char DPD selection
   double curDelta           = 0.0;             // power delta from current iteration
   unsigned int daqIdx       = 2;               // index of digital gain used for computing power deltas
   unsigned short daqDigGain = 40;              // digital gain used for computing power deltas

   // Obtain a map of all RGIs for the current band/chain/channel to digGain/RGI data
   map< int, vector<struct digRecord> > digGainPowerMap;

   if (v4CharDigGainExists)
   {
      opPwrTablesV2->getDigGainRGI_PowerMap( band, chan, chainID, digGainPowerMap );
   }
   else
   {
      opPwrTablesV2->getDigGainRGI_PowerMap( band, chan, chainID, digGainPowerMap, false );
   }

   map< int, vector<struct digRecord> >::iterator digPow_Iter = digGainPowerMap.begin();

   unsigned short smallestRgi   = 0;
   double         smallestDelta = 20.0;

   // compute the deltas for each set of the RGIs, and find the smallest delta --> this is the desired char DPD data set
   for (; digPow_Iter != digGainPowerMap.end(); ++digPow_Iter)
   {
      curRgi        = static_cast<unsigned short>( digPow_Iter->first );
      daqs          = digPow_Iter->second;
      daqIdx        = daqs.size() - 2;                    // set daqIdx to the 2nd to last element
      curPredistPow = daqs[daqIdx].pwr;
      daqDigGain    = static_cast<unsigned short>(daqs[daqIdx].digGain);

      // verify daqDigGain is included in the DUT dig gain list
      if ( dutDigGainSweep.find(daqDigGain) == dutDigGainSweep.end() )
      {
         ostr.str("");
         ostr << "DUT dig gain list does not include " << daqDigGain << ". " << __FUNCTION__
              << "Make sure digital gain list matches digital gains used in the input characterization data.\n"
              << "Function: " << __FUNCTION__ << "; Line: " << __LINE__;
         RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
         return false;
      }

      tarPreDistPow = dutDigGainSweep[daqDigGain].digGainPwr[calRgi];
      curDelta      = curPredistPow - tarPreDistPow;

      if (curDelta == 0)
      {
         smallestDelta = curDelta;
         smallestRgi   = curRgi;
         break;
      }

      if ( fabs(curDelta) < fabs(smallestDelta) )
      {
         smallestDelta = curDelta;
         smallestRgi   = curRgi;
      }
   }

   curRgi = smallestRgi;

   // get the desired data
   if (v4CharDigGainExists)
   {
      foundCharData = opPwrTablesV2->getTablesV4( band, chan, chainID, curRgi, ops, v3Daqs, daqs );
   }
   else
   {
      foundCharData = opPwrTablesV2->getTables( band, chan, chainID, curRgi, ops, daqs );
   }

   // make sur the desired data was found
   if ( !foundCharData )
   {
      ostr.str("");
      ostr << "Characterized Tx DPD data unavailable for BAND " << band << ", CHAIN " << chainID << ", CHAN " << chan << ", RGI " << calRgi
            << "\nMake sure the char data includes the desired DPD data.";
      RFCal::Helper::Log::AddMessageToQMSLLog( hPhone, ostr.str() );
      return false;

   }


   return curRgi;
}
void RFCal::OpPwrTableGeneratorV2::getDigGainRGI_PowerMap( int rfMode, int channel, int chain, map< int, vector<struct digRecord> >& powerMap, bool getV4PowerMap )
{
   int curRGI = 0;
   GSMCharDPD_TableKey curKey(rfMode, channel, chain, curRGI); // construct curKey with dummy RGI

   map< GSMCharDPD_TableKey, vector<struct digRecord> >::iterator digRecord_Iter = m_dData_V4.begin();

   // processing V4 data
   if (getV4PowerMap)
   {
      // get the V4 digital gain records
      for (; digRecord_Iter != m_dData_V4.end(); ++digRecord_Iter )
      {
         curKey = digRecord_Iter->first;
         // check if this is the key that we want
         if ( curKey.m_rfMode == rfMode && curKey.m_channel == channel && curKey.m_chain == chain )
         {
            curRGI           = curKey.m_rgi;
            powerMap[curRGI] = m_dData_V4[curKey];
         }
      }
   }
   else     // !getV4PowerMap
   {
      digRecord_Iter = m_dData_V3.begin();
      // get the V3 digital gain records
      for (; digRecord_Iter != m_dData_V3.end(); ++digRecord_Iter )
      {
         curKey = digRecord_Iter->first;
         // check if this is the key that we want
         if ( curKey.m_rfMode == rfMode && curKey.m_channel == channel && curKey.m_chain == chain )
         {
            curRGI           = curKey.m_rgi;
            powerMap[curRGI] = m_dData_V3[curKey];
         }
      }
   }

}
// returns true if desired data is found; otherwise returns false
bool RFCal::OpPwrTableGeneratorV2::getTables( int rfMode, int channel, int chain, int rgi, vector<struct opPwrRecord>& retOpPow, vector<struct digRecord>& retDigRec )
{
   GSMCharDPD_TableKey myKey(rfMode, channel, chain, rgi);

   // make sure the key exists, before returning any data
   if ( m_pData.find(myKey) == m_pData.end() || m_dData_V3.find(myKey) == m_dData_V3.end() )
   {
      return false;
   }

   retOpPow  = m_pData[myKey];
   retDigRec = m_dData_V3[myKey];

   return true;
}

bool RFCal::OpPwrTableGeneratorV2::getTablesV4( int rfMode, int channel, int chain, int rgi, vector<struct opPwrRecord>& retOpPow,
                                         vector<struct digRecord>& v3RetDigRec, vector<struct digRecord>& v4RetDigRec )
{
   GSMCharDPD_TableKey myKey(rfMode, channel, chain, rgi);

   // make sure the key exists, before returning any data
   if ( m_pData.find(myKey) == m_pData.end() || m_dData_V3.find(myKey) == m_dData_V3.end() || m_dData_V4.find(myKey) == m_dData_V4.end() )
   {
      return false;
   }

   retOpPow    = m_pData[myKey];
   v3RetDigRec = m_dData_V3[myKey];
   v4RetDigRec = m_dData_V4[myKey];

   return true;
}

