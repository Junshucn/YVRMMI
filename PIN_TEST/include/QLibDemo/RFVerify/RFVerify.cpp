/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_External_Demo_MSVC6_and_7/RFVerify/RFVerify.cpp#3 $
 * $DateTime: 2017/08/28 03:26:13 $
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
#include "RFVerify.h"


extern HANDLE g_hResourceContext;

// "0=CDMA;" << "1=WCDMA;" <<"2=GSM;" <<"10=LTE;" <<"11=TDSCDMA"; technology
// "Band Selection;
// "0/GSM850;1/GSM900;2/DCS1800;3/PCS1900;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23;24;"
// "25;26;27;28;29;30;31;32;33;34;35;36;37;38;39;40;41;42;43;44;66;125;126;127;252;255";

void Test_ARA()
{	
   unsigned short _iLogFlags = LOG_IO | LOG_FN | LOG_RET | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP;	

   int band = 0;

	/*enum TECHNOLOGY_FAMILY_TYPE
	{
		TECHNOLOGY_CDMA = 0,
		TECHNOLOGY_WCDMA = 1,
		TECHNOLOGY_GSM = 2,
		TECHNOLOGY_LTE = 10,
		TECHNOLOGY_TDSCDMA = 11
	};*/

   TECHNOLOGY_FAMILY_TYPE tech = TECHNOLOGY_CDMA; 
   
   QLIB_SetLogFlags( g_hResourceContext, _iLogFlags );
   QLIB_StartLogging( g_hResourceContext, "ARA_QMSL_debug.txt" ); 
   QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
   Sleep(200);

   // *****************************************************//
   // QLIB_FTM_GET_RF_PATH_INFORMATION ,get RF path version 1 from DUT, e.g. below LTE B1
   // *****************************************************//

   RFPathInformationRequest req; /*!< Request Packet */
   RFPathInformationResponse response; /*!< Response Packet */

   req.technology = tech;
   req.band = band;
   req.subBand = 0;// default no subband
   req.version = 1;//default version 1
   unsigned char status = QLIB_FTM_GET_RF_PATH_INFORMATION( g_hResourceContext, & req, &response );
   if ( !status || 0 == response.responsePayload.numberofPackets )
   {
	   printf (" QLIB_FTM_GET_RF_PATH_INFORMATION Failed ... \n");
      return; // failure
   }

   // ****************************************************//
	// Parse QLIB_FTM_GET_RF_PATH_INFORMATION and paths loop
	// ****************************************************//

	for( uint16 packetIndex = 0; packetIndex < response.responsePayload.numberofPackets; ++packetIndex )// path loop
	{
		RFPathInformationPacketVersion1 & rfPath = response.responsePayload.packet[packetIndex].version1;

		if (1 == rfPath.txrxcalaction )// only test Cal path   /* 1 = Calibration 0 = Characterization 2 = offset calibration */ // 5/5
		{
			if (rfPath.rfmPathType == PATH_TYPE_PRX || rfPath.rfmPathType == PATH_TYPE_DRX)// Prx or Drx path ARA
			{
				if( !AssignRadioAllocations(g_hResourceContext, rfPath, tech, band) )
				{
					printf (" ARA to path index = %d failed \n", packetIndex);
					return;
				}
				else
				{
					printf (" ARA to path index = %d pass \n", packetIndex);
				}

				//start call as before 

				if ( rfPath.rfmPathType == PATH_TYPE_PRX )//Prx test only /*!< Defines Type of RFM Path; 0 = PRx; 1 = DRx 2 = Tx */
				{

				    printf (" do Prx test ... \n");
					// do Prx test
				}
				else if ( rfPath.rfmPathType == PATH_TYPE_DRX)// Drx test only
				{
				    printf (" do Drx test ... \n");
					// do Drx test
				}

				//end call as before

			}// end Prx and Drx path ARA
			else // Tx path ARA ,here will do Tx ARA need swap data
			{					
				if ( rfPath.rfmPathType == PATH_TYPE_TX )// Tx test only
				{
					RFPathInformationPacketVersion1  Rfpathv1_Tx;
					
					//swap associatedRxSig/dev/ant to Prx
					Rfpathv1_Tx.signalPath = rfPath.associatedRxSignalPath;
					Rfpathv1_Tx.rfmDevice = rfPath.associatedRxRFMDevice;
					Rfpathv1_Tx.antennaPath = rfPath.associatedRxAntennaPath;

					
					//swap sig/dev/ant to Tx 
					Rfpathv1_Tx.associatedTxSignalPath = rfPath.signalPath;
					Rfpathv1_Tx.associatedTxRFMDevice = rfPath.rfmDevice;
					Rfpathv1_Tx.associatedTxAntennaPath = rfPath.antennaPath;

					//find the associated DRx dev/sig/ant 
					for ( uint16 i = 0; i < response.responsePayload.numberofPackets; ++i )
					{
						RFPathInformationPacketVersion1 & rfPathtemp = response.responsePayload.packet[i].version1;

						if ( Rfpathv1_Tx.rfmDevice == rfPathtemp.rfmDevice &&
							  Rfpathv1_Tx.signalPath == rfPathtemp.signalPath&&
							  Rfpathv1_Tx.antennaPath == rfPathtemp.antennaPath )
						{  
							//associated Drx info
							Rfpathv1_Tx.associatedRxSignalPath =  rfPathtemp.associatedRxSignalPath;
							Rfpathv1_Tx.associatedRxRFMDevice = rfPathtemp.associatedRxRFMDevice;
							Rfpathv1_Tx.associatedRxAntennaPath = rfPathtemp.associatedRxAntennaPath;
							break;
						}
					}
					
					rfPath.signalPath = static_cast<uint16> (Rfpathv1_Tx.signalPath);
					rfPath.rfmDevice = Rfpathv1_Tx.rfmDevice;
					rfPath.antennaPath = Rfpathv1_Tx.antennaPath;

					rfPath.associatedRxSignalPath =  static_cast<uint16> (Rfpathv1_Tx.associatedRxSignalPath);
					rfPath.associatedRxRFMDevice = Rfpathv1_Tx.associatedRxRFMDevice;
					rfPath.associatedRxAntennaPath = Rfpathv1_Tx.associatedRxAntennaPath;

					rfPath.associatedTxSignalPath = static_cast<uint16> (Rfpathv1_Tx.associatedTxSignalPath);
					rfPath.associatedTxRFMDevice = Rfpathv1_Tx.associatedTxRFMDevice;
					rfPath.associatedTxAntennaPath = Rfpathv1_Tx.associatedTxAntennaPath;

					
               if( !AssignRadioAllocations(g_hResourceContext, rfPath, tech, band) )                    
					{
						printf (" ARA to path index = %d failed \n", packetIndex);
						return;
					}
					else
					{
						printf (" ARA to path index = %d pass \n", packetIndex);
					}

					//start call as before 

					// test Tx
					printf (" do Tx test ...\n");

					//end call

				}// End Tx test 
			}// end tx path ara	

			//Print current path all Prx/Drx/Tx dev/sig/ant
			printf (" Index = %d    Band = %d   PrxSigPath = %d  PrxRfmDev = %d   DrxSigPath = %d   DrxRfmDev =  %d  TxSigPath = %d   TxRfmDev =  %d  RfmType =  %d  ActType =  %d  \n",
			packetIndex, band, rfPath.signalPath, rfPath.rfmDevice, rfPath.associatedRxSignalPath, rfPath.associatedRxRFMDevice, rfPath.associatedTxSignalPath, rfPath.associatedTxRFMDevice, 
			rfPath.rfmPathType, rfPath.rxrsbcalaction); 
		}// end cal type
		
   }// end loop

	QLIB_StopLogging (g_hResourceContext);
}

ftm_rf_technology_type GetFTMRFTechnology( TECHNOLOGY_FAMILY_TYPE technology )
{
   ftm_rf_technology_type tech = FTM_RF_TECH_UNKNOWN;
   switch(technology)
   {
      case TECHNOLOGY_CDMA:
         tech = FTM_RF_TECH_CDMA;
         break;
      case TECHNOLOGY_WCDMA:
         tech = FTM_RF_TECH_WCDMA;
         break;
      case TECHNOLOGY_LTE:
         tech = FTM_RF_TECH_LTE;
         break;
      case TECHNOLOGY_GSM:
         tech = FTM_RF_TECH_GSM;
         break;
      case TECHNOLOGY_TDSCDMA:
         tech = FTM_RF_TECH_TDSCDMA;
         break;
      default:
			printf("No FTM RF tech for this technology family\n");			
			break;
   }
   return tech;
}

void PopulateARAProperties( rf_path_assignment &rfpaths, ftm_rf_test_property_data* requestData )
{
   vector <ftm_rf_test_property_data_type> propertyDataVector;
   ftm_rf_test_property_data_type propertyEntry;

   //Prx

	propertyEntry.type = FTM_TRM_ARA_PROP_CARRIER_IDX ;
	propertyEntry.value = 0;// hardcore for signal carrier
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_BAND ;
	propertyEntry.value = rfpaths.band;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_RXTX ;
	propertyEntry.value = 1;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_CHAIN ;
	propertyEntry.value = 0;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_DEVICE;
	propertyEntry.value = rfpaths.rx_dev[0];
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_SIGNAL_PATH;
	propertyEntry.value =  rfpaths.rx_sigpath[0];
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_ANTENNA_PATH;
	propertyEntry.value =  rfpaths.rx_antpath[0];
	propertyDataVector.push_back(propertyEntry);

	//Drx
	propertyEntry.type = FTM_TRM_ARA_PROP_CARRIER_IDX ;
	propertyEntry.value = 0;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_BAND ;
	propertyEntry.value = rfpaths.band;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_RXTX ;
	propertyEntry.value = 1;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_CHAIN ;   
	propertyEntry.value = 1;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_DEVICE;
	propertyEntry.value = rfpaths.rx_dev[1];
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_SIGNAL_PATH;
	propertyEntry.value = rfpaths.rx_sigpath[1];
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_ANTENNA_PATH;
	propertyEntry.value = rfpaths.rx_antpath[1];
	propertyDataVector.push_back(propertyEntry);

	// TX 

	propertyEntry.type = FTM_TRM_ARA_PROP_CARRIER_IDX ;
	propertyEntry.value = 0;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_BAND ;
	propertyEntry.value = rfpaths.band;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_RXTX ;
	propertyEntry.value = 2;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_CHAIN ;
	propertyEntry.value = 0;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_DEVICE;
	propertyEntry.value = rfpaths.tx_dev;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_SIGNAL_PATH;
	propertyEntry.value = rfpaths.tx_sigpath;
	propertyDataVector.push_back(propertyEntry);

	propertyEntry.type = FTM_TRM_ARA_PROP_ANTENNA_PATH;
	propertyEntry.value = rfpaths.tx_antpath;;
	propertyDataVector.push_back(propertyEntry);

   requestData->num_properties = propertyDataVector.size();
   copy(propertyDataVector.begin(), propertyDataVector.end(), requestData->properties);
}

bool AssignRadioAllocations( HANDLE hResourceContext, RFPathInformationPacketVersion1 Rfpathv1,TECHNOLOGY_FAMILY_TYPE tech, int band )
 {
   rf_path_assignment rfPath;

	//Get the ftm rf technology 
	ftm_rf_technology_type technology = GetFTMRFTechnology( (TECHNOLOGY_FAMILY_TYPE)tech ); 

	if(tech == TECHNOLOGY_GSM)
	{
		switch(band)
		{
			case 0:
				band = 850;
				break;
			case 1:
				band = 900;
				break;
			case 2:
				band = 1800;
				break;
			case 3:
				band = 1900;
				break;		
			default:
				printf("No FTM RF tech for this technology family\n");				
				break;
		}
	}


   rfPath.band = band;
	rfPath.subBand = 0;// not consider sub band

	rfPath.tx_antpath = Rfpathv1.associatedTxAntennaPath;
	rfPath.tx_dev = Rfpathv1.associatedTxRFMDevice;
	rfPath.tx_sigpath = static_cast<uint16> ( Rfpathv1.associatedTxSignalPath );

	//To support Aux port DRx 
	if( Rfpathv1.rfmPathType == 0 ||  Rfpathv1.rfmPathType == 2) // Prx or Tx
	{
		rfPath.rx_sigpath[PATH_TYPE_PRX] = static_cast<uint16> ( Rfpathv1.signalPath );
		rfPath.rx_dev[PATH_TYPE_PRX] = Rfpathv1.rfmDevice;
		rfPath.rx_antpath[PATH_TYPE_PRX] = Rfpathv1.antennaPath;
	
		rfPath.rx_sigpath[PATH_TYPE_DRX] = static_cast<uint16> ( Rfpathv1.associatedRxSignalPath );
		rfPath.rx_dev[PATH_TYPE_DRX] = Rfpathv1.associatedRxRFMDevice;
		rfPath.rx_antpath[PATH_TYPE_DRX] = Rfpathv1.associatedRxAntennaPath; 

		
	}
	else if( Rfpathv1.rfmPathType == 1 ) // Drx 
	{
		rfPath.rx_sigpath[PATH_TYPE_PRX] =  static_cast<uint16> ( Rfpathv1.associatedRxSignalPath );
		rfPath.rx_dev[PATH_TYPE_PRX] = Rfpathv1.associatedRxRFMDevice;
		rfPath.rx_antpath[PATH_TYPE_PRX] = Rfpathv1.associatedRxAntennaPath;
		
		rfPath.rx_sigpath[PATH_TYPE_DRX] = static_cast<uint16> ( Rfpathv1.signalPath );
		rfPath.rx_dev[PATH_TYPE_DRX] = Rfpathv1.rfmDevice;
		rfPath.rx_antpath[PATH_TYPE_DRX] = Rfpathv1.antennaPath;
	}
		
   //Send ARA disable - this is a requirement from the TRM team. Disable needs to be sent before a change in assignments.
   rf_path_assignment dummyRFPaths;
   ftm_rf_test_property_data dummyRequestData;
   ftm_rf_test_property_data dummyResponseData;
   unsigned long error_code = 999;
   unsigned long radioConfigSpecificErrCode = 999;

	memset(&dummyRequestData,0,sizeof(dummyRequestData));
	memset(&dummyResponseData,0,sizeof(dummyResponseData));

	dummyRequestData.num_properties = 0;

	unsigned char status = QLIB_FTM_ASSIGN_RADIO_ALLOCATION( hResourceContext,
																			   technology,
																				static_cast<void*>(&dummyRequestData),
																				static_cast<void*>(&dummyResponseData),
																				&error_code,
																				&radioConfigSpecificErrCode );
	if( error_code != 0 )
	{
		//printf(" ARA disable failed error code = %d\n", error_code );
		//return  false; if no ARA enable, this will be failed, so remove return false.
	}

	//Send the actual ARA command with the path that corresponds to the current path index
   ftm_rf_test_property_data requestData;
	ftm_rf_test_property_data responseData;
	error_code = 999;
   radioConfigSpecificErrCode = 999;

	memset(&requestData,0,sizeof(requestData)); 
	memset(&responseData,0,sizeof(responseData));

	PopulateARAProperties(rfPath, &requestData);
  
   status = QLIB_FTM_ASSIGN_RADIO_ALLOCATION( hResourceContext,
                                              technology,
                                              static_cast<void*>(&requestData),
                                              static_cast<void*>(&responseData),
                                              &error_code,
                                              &radioConfigSpecificErrCode );
	if( error_code != 0 )
   {
		printf(" ARA enable failed error code = %d\n", error_code );
		return  false; 
	}
   return true;
 }
  
void Test_QLINK()
{ 	 
   unsigned short _iLogFlags = LOG_IO | LOG_FN | LOG_RET | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP;
	
	QLIB_SetLogFlags( g_hResourceContext, _iLogFlags ); 
	QLIB_StartLogging( g_hResourceContext,	"Qlink_QMSL_debug.txt" );		 
	QLIB_DIAG_CONTROL_F( g_hResourceContext, MODE_FTM_F );
	Sleep(100);

	ftm_qlink_property_data requestData;
	ftm_qlink_property_data responseData;
	unsigned long iErrorCode = 0;
	unsigned long iSpecificErrorCode = 0;
	unsigned char status = 0;
   
	memset(&requestData,0,sizeof(requestData)); 
	memset(&responseData,0,sizeof(requestData)); 
 
	//QLIB_FTM_QLINK_BLER_TEST
	requestData.num_properties = 1;
	requestData.properties[0].name = 1;
	requestData.properties[0].value = 0;
 
	status = QLIB_FTM_QLINK_BLER_TEST(g_hResourceContext, (void *)&requestData, (void *)&responseData,  &iErrorCode, &iSpecificErrorCode);
	 
	if(iErrorCode == 0&& status )
	{
		printf(" QLIB_FTM_QLINK_BLER_TEST pass\n");
		printf(" error count = %d\n", responseData.properties[0].value);
	}
	else 
	{
		printf(" QLIB_FTM_QLINK_BLER_TEST failed error code = %d\n", iErrorCode );		 
	}
 
	//QLIB_FTM_QLINK_PING_TEST
	memset(&requestData,0,sizeof(requestData)); 
	memset(&responseData,0,sizeof(requestData)); 
 
	requestData.num_properties = 1;
	requestData.properties[0].name = 1;
	requestData.properties[0].value = 0;
 
	status = QLIB_FTM_QLINK_PING_TEST(g_hResourceContext, (void *)&requestData, (void *)&responseData,  &iErrorCode, &iSpecificErrorCode);
	 
	if(iErrorCode == 0 && status )
	{
		printf(" QLIB_FTM_QLINK_PING_TEST pass\n");
		printf(" status = %d\n", responseData.properties[0].value);
	}
	else 
	{
		printf(" QLIB_FTM_QLINK_PING_TEST failed error code = %d\n", iErrorCode );		 
	}
 
	//QLIB_FTM_QLINK_READ_SLAVE_ID
	memset(&requestData,0,sizeof(requestData)); 
	memset(&responseData,0,sizeof(requestData)); 
 
	requestData.num_properties = 1;
	requestData.properties[0].name = 1;
	requestData.properties[0].value = 0;
 
	status = QLIB_FTM_QLINK_READ_SLAVE_ID(g_hResourceContext, (void *)&requestData, (void *)&responseData,  &iErrorCode, &iSpecificErrorCode);
 	 
	if(iErrorCode == 0&& status )
	{
		printf(" QLIB_FTM_QLINK_READ_SLAVE_ID pass\n");
		printf(" SLAVE_ID = %d\n", responseData.properties[0].value);
	}
	else 
	{
		printf(" QLIB_FTM_QLINK_READ_SLAVE_ID failed error code = %d\n", iErrorCode );			 
	} 
	 
	QLIB_StopLogging (g_hResourceContext);
}
