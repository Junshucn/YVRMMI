/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_WWAN/QMSL_NV/NV_Manager/QMSL_Linear_Predistortion_NV.cpp#7 $
 * $DateTime: 2017/05/22 22:31:28 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#include "QMSL_Linear_Predistortion_NV.h"
#if !defined( QMSL_POSIX_PORTABLE )
#else
void ZeroMemory(
   void* Destination,
   unsigned long Length
)
{
   memset((Destination),0, Length);
}
typedef unsigned long DWORD;     //!<' Definition of unsigned 32-bit type
#endif
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>  
#include <cmath>
#include <algorithm>
#include <numeric>
#include <set>
#include "QMSL_Vector.h"
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
using namespace std;

#define PI 3.1415926538
#define NUMELEMS( arr ) ((sizeof(arr))/(sizeof(arr[0])))

//#define DEBUG_PREDIST_PHASEDRIFT
//#define DEBUG_PREDIST_XCORR
//#define DEBUG_PREDIST_AMAM_GAIN
//#define DEBUG_PREDIST_AMPM_GAIN
//#define DEBUG_SMOOTHING
//#define DEBUG_AMAM
//#define DEBUG_AMPM
//#define DEBUG_PREDIST_IQFFT
//#define DEBUG_SLOPE_COMP
//#define DEBUG_LOG_PREDIST_CAL_PARAMS
//#define DEBUG_UPDATE_DELAY_FOR_AMAM_GAIN_SPREAD

int ComputeFFT(const int isign, int numSamples_In,
               double * m_pDI, double * m_pDQ,
               double * m_pFFTI, double * m_pFFTQ)
{
   DWORD n, mmax, m, j, istep, i;
   double wtemp, wr, wpr, wi, wpi, theta,
          tempr, tempi;

   const DWORD nn = 32768;
   double* data = new double[2 * nn + 1];

   for (i = 1; i <= nn; i++)
   {
      data[2*i - 1] = 0.0;//zero out buffer
      data[2*i] = 0.0;//zero out buffer

      if(i < static_cast<unsigned int>(numSamples_In))
      {
         data[2*i - 1] = m_pDI[i-1];
         data[2*i] = m_pDQ[i-1];
      }

   }

   n = nn << 1;
   j = 1;

   //Perform bit-reversing
   for(i = 1; i < n; i += 2)
   {
      if(j > i)
      {
         SWAP(data[j], data[i]);
         SWAP(data[j+1], data[i+1]);
      }
      m = n >> 1;
      while( m >= 2 && j > m )
      {
         j -= m;
         m >>= 1;
      }
      j += m;
   }

   //Danielson-Lanczos routine
   mmax = 2;
   while( n > mmax)//Executed log2(nn) times.
   {
      istep = mmax << 1;
      theta = isign * 2* acos(-1.0) / mmax;//Initialize trigonometric recurrence
      wtemp = sin(0.5 * theta);
      wpr = -2.0 * wtemp * wtemp;
      wpi = sin(theta);
      wr = 1.0;
      wi = 0.0;
      for(m = 1; m < mmax; m += 2)//Danielson-Lanczos formula application
      {
         for(i = m; i <= n; i += istep)
         {
            j = i + mmax;
            if(j < 2 * nn)
            {
               tempr = wr * data[j] - wi * data[j+1];
               tempi = wr * data[j+1] + wi * data[j];
               data[j] = data[i] - tempr;
               data[j+1] = data[i+1] - tempi;
               data[i] += tempr;
               data[i+1] += tempi;
            }
         }
         wr = (wtemp = wr) * wpr - wi * wpi + wr;//Trigonometric recurrence
         wi = wi * wpr + wtemp * wpi + wi;
      }
      mmax = istep;
   }

   // scale the data if we go in the forward direction
   double divisor;
   if (isign == 1)
      divisor = (double)nn;
   else
      divisor = 1.0;
   for (i = 1; i <= nn ; i++)
   {
      m_pFFTI[i - 1] = 0.0;//zero out buffer
      m_pFFTQ[i - 1] = 0.0;//zero out buffer
      m_pFFTI[i - 1] = data[2 * i - 1] / divisor;
      m_pFFTQ[i - 1] = data[2 * i ] / divisor;
   }

   delete [] data;
   return 0;
}


float QMSL_Linear_Predistortion_NV::estimateAndCorrectPhaseDrift(float * PhaseVec,long estStart,long estEnd, long corrStart,long corrEnd)
{
   float phaseDiffAccum = 0.0;
   long slopeEstimationLength = 0;
   long loopVar = 0;

   //Before Correcting Phase Drift (Unwrap Phase)
   //Input Phase is between +/- 180

   //Fix the whole of the correction Region
   long min_start = 0;
   long max_end   = corrEnd;

#ifdef DEBUG_PREDIST_PHASEDRIFT
   static int i = 0;
   i++;
   ostringstream fnamestr;
   fnamestr.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr << "C:\\Qualcomm\\QDART\\Temp\\Phase_Unwrap_" << i << ".csv";
#else
   fnamestr <<P_tmpdir <<"Phase_Unwrap_" << i << ".csv";
#endif



   ofstream phUnWrapFile;
   phUnWrapFile.open (fnamestr.str().c_str(),ios::out);
   phUnWrapFile << "Index,Ph_in,Ph_unWrap"<< endl;
#endif

   //Actual Unwrap (if diff > 170*2)
   for(loopVar = min_start+1; loopVar<=max_end; loopVar++)
   {
#ifdef DEBUG_PREDIST_PHASEDRIFT
      phUnWrapFile << loopVar << "," << PhaseVec[loopVar] << ",";
#endif
      while(fabs(PhaseVec[loopVar] - PhaseVec[loopVar-1]) > 340.0) //>2*pi
      {
         if(PhaseVec[loopVar] > PhaseVec[loopVar-1])
            PhaseVec[loopVar] = PhaseVec[loopVar] - 360;
         else
            PhaseVec[loopVar] = PhaseVec[loopVar] + 360;
      }
#ifdef DEBUG_PREDIST_PHASEDRIFT
      phUnWrapFile << PhaseVec[loopVar] << endl;
#endif
   }


#ifdef DEBUG_PREDIST_PHASEDRIFT
   phUnWrapFile.close();
#endif


   //??Normalize to first value (Set Phase[0]=0);



   for(loopVar = estStart; loopVar<estEnd; loopVar++)
   {
      phaseDiffAccum = phaseDiffAccum + (PhaseVec[loopVar+1] - PhaseVec[loopVar]);
      slopeEstimationLength++;
   }

   float averageSlopeChange = phaseDiffAccum / slopeEstimationLength;

#ifdef DEBUG_PREDIST_PHASEDRIFT
   fnamestr.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr << "C:\\Qualcomm\\QDART\\Temp\\Phase_Corr_" << i << ".csv";
#else
   fnamestr <<P_tmpdir<<"Phase_Corr_" << i << ".csv";
#endif

   ofstream phCorrFile;
   phCorrFile.open (fnamestr.str().c_str(),ios::out);
   phCorrFile << "Ph_in,Ph_out"<< endl;
#endif

   for(loopVar = corrStart; loopVar<corrEnd; loopVar++)
   {
#ifdef DEBUG_PREDIST_PHASEDRIFT
      phCorrFile << PhaseVec[loopVar] << ",";
#endif
      PhaseVec[loopVar] = PhaseVec[loopVar] - (loopVar-corrStart)*averageSlopeChange;
#ifdef DEBUG_PREDIST_PHASEDRIFT
      phCorrFile << PhaseVec[loopVar] << endl;
#endif
   }
#ifdef DEBUG_PREDIST_PHASEDRIFT
   phCorrFile.close();
#endif

   return averageSlopeChange;
}

long QMSL_Linear_Predistortion_NV::crossCorrelate(float * CVec1,const float * CVec2, long CVec1Size, long CVec2Size)
{
   //Optimization 1
   //If we have more measured data than Input - truncate measured data
   if(CVec1Size > CVec2Size) CVec1Size = CVec2Size;

   vector <float> tempVec1(CVec1, CVec1+CVec1Size);
   vector <float> tempVec2(CVec2, CVec2+CVec2Size);
   int sizeVec1 = (int)tempVec1.size();
   int sizeVec2 = (int)tempVec2.size();
   unsigned int loopVar = 0;

   unsigned long sizeLargerVec = sizeVec1;

#ifdef DEBUG_PREDIST_XCORR
   static int i = 0;
   i++;
   ostringstream fnamestr1,fnamestr2;
   fnamestr1.str("");
   fnamestr2.str("");


#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\Cross_Corr_Inps_v1_" << i << ".csv";
   fnamestr2 << "C:\\Qualcomm\\QDART\\Temp\\Cross_Corr_Inps_v2_" << i << ".csv";
#else
   fnamestr1 <<P_tmpdir<<"Cross_Corr_Inps_v1_" << i << ".csv";
   fnamestr2 <<P_tmpdir<< "Cross_Corr_Inps_v2_" << i << ".csv";
#endif

   ofstream xCorrInFile1,xCorrInFile2;
   xCorrInFile1.open (fnamestr1.str().c_str(),ios::out);
   xCorrInFile2.open (fnamestr2.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < (unsigned long)CVec1Size; loopVar++)
   {
      xCorrInFile1 << CVec1[loopVar] << endl;
   }
   for(loopVar = 0; loopVar < (unsigned long)CVec2Size; loopVar++)
   {
      xCorrInFile2 << CVec2[loopVar] << endl;
   }
   xCorrInFile1.close();
   xCorrInFile2.close();
#endif

   if(sizeVec1 > sizeVec2)
   {
      tempVec2.resize(sizeVec1,0.0);
   }
   if(sizeVec1 < sizeVec2)
   {
      tempVec1.resize(sizeVec2,0.0);
      sizeLargerVec = sizeVec2;
   }

   unsigned long corrVecLen = 2*sizeLargerVec - 1;

   vector <float>::iterator Vec1BeginIter, Vec1EndIter;
   vector <float>::iterator Vec2BeginIter;

   vector <float> corrVec(corrVecLen);
   Vec1BeginIter = tempVec1.begin();
   Vec1EndIter = tempVec1.begin()+1;
   Vec2BeginIter = tempVec2.end()-1;

   cout << "Corr Vec Len = " << corrVecLen << endl;
   cout << "Correlation Started " << endl;

   for(loopVar = 0; loopVar < corrVecLen && Vec1EndIter != tempVec1.end(); loopVar++)
   {
      if(loopVar < sizeLargerVec)
      {
         corrVec[loopVar] = inner_product(Vec1BeginIter,Vec1EndIter,Vec2BeginIter,(float)0.0);
         ++Vec1EndIter;
         --Vec2BeginIter;
      }
      if(loopVar >= sizeLargerVec)
      {
         if(loopVar == sizeLargerVec)
         {
            Vec2BeginIter = tempVec2.begin();
            Vec1EndIter   = tempVec1.end();
         }
         ++Vec1BeginIter;
         corrVec[loopVar] = inner_product(Vec1BeginIter,Vec1EndIter,Vec2BeginIter,(float)0.0);
      }
   }
   cout << "Correlation Ended " << endl;

   float largestVal = (float)1e-100;
   unsigned long largestInd = 0;

   for(loopVar = 0; loopVar < corrVec.size(); loopVar++)
   {
      if(corrVec[loopVar] > largestVal)
      {
         largestVal = corrVec[loopVar];
         largestInd = loopVar;
      }
   }

#ifdef DEBUG_PREDIST_XCORR
   ostringstream fnamestr3,fnamestr4;
   fnamestr3.str("");
   fnamestr4.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr3 << "C:\\Qualcomm\\QDART\\Temp\\Cross_Corr_Op_" << i << ".csv";
   fnamestr4 << "C:\\Qualcomm\\QDART\\Temp\\Cross_Corr_Delay" << i << ".csv";
#else
   fnamestr3 <<P_tmpdir<<"Cross_Corr_Op_" << i << ".csv";
   fnamestr4 <<P_tmpdir<< "Cross_Corr_Delay" << i << ".csv";
#endif

   ofstream xCorrOutFile1,xCorrOutFile2;
   xCorrOutFile1.open (fnamestr3.str().c_str(),ios::out);
   xCorrOutFile2.open (fnamestr4.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < corrVec.size(); loopVar++)
   {
      xCorrOutFile1 << corrVec[loopVar] << endl;
   }
   xCorrOutFile2 << "Delay : " << (largestInd - sizeVec1) << endl;
   xCorrOutFile1.close();
   xCorrOutFile2.close();
#endif

   return (largestInd - sizeVec1);
}

long QMSL_Linear_Predistortion_NV::crossCorrelateIQFFT
(float * CVec1,const float * CVec2, long CVec1Size, long CVec2Size)
{
   if(CVec1Size > CVec2Size) CVec1Size = CVec2Size;

   unsigned int n=CVec1Size;
   const unsigned int np = 32768;

   double* f0 = new double[np];
   double* f1 = new double[np];
   double* f2 = new double[np];

   double* g1_i = new double[np];
   double* g1_q = new double[np];
   double* g2_i = new double[np];
   double* g2_q = new double[np];

   double* f_i = new double[np];
   double* f_q = new double[np];
   double* g_i = new double[np];
   double* g_q = new double[np];

   unsigned int i=0;

   for(i=0; i < n; i++)
   {
      f0[i]=0.0;
      f1[i]=CVec2[i];
      f2[i]=CVec1[i];
   }

#ifdef DEBUG_PREDIST_IQFFT
   static int k=0;
   k++;
   ostringstream fnamestr_in;
   fnamestr_in.str("");
   fnamestr_in << "C:\\Qualcomm\\QDART\\Temp\\IQFFT_In_" << k << ".csv";

   ofstream xCorrInFile1;
   xCorrInFile1.open (fnamestr_in.str().c_str(),ios::out);
   for(unsigned int loopVar = 0; loopVar < n; loopVar++)
   {
      xCorrInFile1 << f1[loopVar] << "," << f2[loopVar] << endl;
   }
   xCorrInFile1.close();
#endif

   ComputeFFT(1, n, f1,f0, g1_i, g1_q);
   ComputeFFT(1, n, f2,f0, g2_i, g2_q);

   for(i=0; i < np; i++)
   {
      g_i[i] = g1_i[i]*g2_i[i] + g1_q[i]*g2_q[i];
      g_q[i] = g1_q[i]*g2_i[i] - g1_i[i]*g2_q[i];
   }

   ComputeFFT(1, n, g_i,g_q, f_i, f_q);

   double maxVal = 0;
   int Index = 0;
   double currentVal;
   for(i=0; i < n/2; i++)
   {
      currentVal = sqrt(f_i[i]*f_i[i]+f_q[i]*f_q[i]);
      if(currentVal > maxVal)
      {
         maxVal = currentVal;
         Index = i;
      }
   }

#ifdef DEBUG_PREDIST_IQFFT
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\IQFFT_Op_" << k << ".csv";

   ofstream xCorrOutFile1;
   xCorrOutFile1.open (fnamestr1.str().c_str(),ios::out);
   for(unsigned int loopVar = 0; loopVar < n; loopVar++)
   {
      xCorrOutFile1 << f_i[loopVar] << "," <<f_q[loopVar]<< ","
                    << sqrt(f_i[loopVar]*f_i[loopVar]+f_q[loopVar]*f_q[loopVar]) << endl;
   }
   xCorrOutFile1 << Index << endl;
   xCorrOutFile1.close();
#endif

   delete [] f0;
   delete [] f1;
   delete [] f2;
   delete [] g1_i;
   delete [] g1_q;
   delete [] g2_i;
   delete [] g2_q;
   delete [] g_i;
   delete [] g_q;
   delete [] f_i;
   delete [] f_q;

   return Index;
}

void QMSL_Linear_Predistortion_NV::computeGain(const float * InPwr,float * MeasPwr, unsigned long VecLength,
                                               unsigned long skipSamples, vector<float>& inPwr, vector<float>& gain)
{
   multimap<float, float> Pwr_Gain;
   unsigned int loopVar = 0;

   float minInPwr,maxInPwr;
   minInPwr = 20*log10(*min_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));
   maxInPwr = 20*log10(*max_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));

#ifdef DEBUG_PREDIST_AMAM_GAIN
   static int i = 0;
   i++;
   ostringstream fnamestr1;
   fnamestr1.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\AMAM_GAIN_Inputs_" << i << ".csv";
#else
   fnamestr1 <<P_tmpdir<<"AMAM_GAIN_Inputs_" << i << ".csv";
#endif

   ofstream amamGainIpFile;
   amamGainIpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < VecLength; loopVar++)
   {
      amamGainIpFile << InPwr[loopVar] << "," << MeasPwr[loopVar]<< endl;
   }
   amamGainIpFile.close();
#endif


   float InPwrdBm;
   float estGain;
   int numSteps = 50;
   float inPwrRange = maxInPwr - minInPwr;
   float inPwrSteps = inPwrRange/numSteps;
   set<float> PwrSet;
   set<float>::iterator pwrIter;
   for(loopVar = 0; loopVar < static_cast<unsigned int>(numSteps); loopVar++)
   {
      PwrSet.insert((minInPwr+inPwrSteps*loopVar));
   }

   pair<multimap<float, float>::iterator, multimap<float, float>::iterator> startStopGains;
   multimap<float, float>::iterator pwrGainIter;

   float currBinInPwr;
   float PrevInPwrdBm;
   for(pwrIter=PwrSet.begin(); pwrIter!=PwrSet.end(); pwrIter++)
   {
      for(loopVar = skipSamples+1; loopVar<(VecLength-skipSamples); loopVar++)
      {
         currBinInPwr = (float)(*pwrIter);
         InPwrdBm = 20*log10(InPwr[loopVar]);
         PrevInPwrdBm = 20*log10(InPwr[loopVar-1]);
         if( (currBinInPwr >= PrevInPwrdBm) && (currBinInPwr <= InPwrdBm) || (currBinInPwr <= PrevInPwrdBm) && (currBinInPwr >= InPwrdBm))
         {
            if(PrevInPwrdBm != InPwrdBm)
            {
               estGain = MeasPwr[loopVar] - currBinInPwr +
                         ((MeasPwr[loopVar-1]-MeasPwr[loopVar])*(currBinInPwr - InPwrdBm))/(PrevInPwrdBm - InPwrdBm);
            }
            else // This will only happen in the currBinInPwr == PrevInPwrdBm == InPwrdBm
            {
               estGain = MeasPwr[loopVar] - currBinInPwr;
            }
            Pwr_Gain.insert(pair<float,float>(*pwrIter, estGain));
         }
      }
   }

   float sumGain;
   float meanGain;
   for(pwrIter=PwrSet.begin(); pwrIter!=PwrSet.end(); pwrIter++)
   {
      startStopGains = Pwr_Gain.equal_range(*pwrIter);

      sumGain = 0.0;

      for(pwrGainIter = startStopGains.first; pwrGainIter != startStopGains.second; pwrGainIter++)
      {
         sumGain = sumGain + (*pwrGainIter).second;
      }
      meanGain = sumGain/Pwr_Gain.count(*pwrIter);
      inPwr.push_back((float)((*pwrIter)));
      gain.push_back(meanGain);
   }
#ifdef DEBUG_PREDIST_AMAM_GAIN
   fnamestr1.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\AMAM_GAIN_Outputs_" << i << ".csv";
#else
   fnamestr1 <<P_tmpdir<<"AMAM_GAIN_Outputs_" << i << ".csv";
#endif


   ofstream amamGainOpFile;
   amamGainOpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < inPwr.size(); loopVar++)
   {
      amamGainOpFile << inPwr[loopVar] << "," << gain[loopVar]<< endl;
   }
   amamGainOpFile.close();
#endif

}

void QMSL_Linear_Predistortion_NV::computePhase(const float * InPwr, float * MeasPhase, unsigned long VecLength
                                                , unsigned long skipSamples, vector<float>& inPwr, vector<float>& phase)
{
   multimap<float, float> Pwr_Phase;
   unsigned int loopVar =0;

   float minInPwr,maxInPwr;
   minInPwr = 20*log10(*min_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));
   maxInPwr = 20*log10(*max_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));

#ifdef DEBUG_PREDIST_AMPM_GAIN
   static int i = 0;
   i++;
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\AMPM_GAIN_Inputs_" << i << ".csv";

   ofstream ampmGainIpFile;
   ampmGainIpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < VecLength; loopVar++)
   {
      ampmGainIpFile << InPwr[loopVar] << "," << MeasPhase[loopVar]<< endl;
   }
   ampmGainIpFile.close();
#endif


   float InPwrdBm;
   float PrevInPwrdBm;
   float estPhase;

   int numSteps = 50;
   float inPwrRange = maxInPwr - minInPwr;
   float inPwrSteps = inPwrRange/numSteps;
   set<float> PwrSet;
   set<float>::iterator pwrIter;
   for(loopVar = 0; loopVar < static_cast<unsigned int>(numSteps); loopVar++)
   {
      PwrSet.insert((minInPwr+inPwrSteps*loopVar));
   }

   float currBinInPwr;
   for(pwrIter=PwrSet.begin(); pwrIter!=PwrSet.end(); pwrIter++)
   {
      for(loopVar = skipSamples+1; loopVar<(VecLength-skipSamples); loopVar++)
      {
         currBinInPwr = *pwrIter;
         InPwrdBm = 20*log10(InPwr[loopVar]);
         PrevInPwrdBm = 20*log10(InPwr[loopVar-1]);
         if( ((currBinInPwr >= PrevInPwrdBm) && (currBinInPwr <= InPwrdBm)) || ((currBinInPwr <= PrevInPwrdBm) && (currBinInPwr >= InPwrdBm)))
         {
            if(PrevInPwrdBm != InPwrdBm)
            {
               estPhase = MeasPhase[loopVar] +
                          ((MeasPhase[loopVar-1]-MeasPhase[loopVar])*(currBinInPwr - InPwrdBm))/(PrevInPwrdBm - InPwrdBm);
            }
            else  // This will only happen in the currBinInPwr == PrevInPwrdBm == InPwrdBm
            {
               estPhase = MeasPhase[loopVar];
            }
            Pwr_Phase.insert(pair<float,float>(*pwrIter, estPhase));
         }
      }
   }

   //With new method set first and last phase distortion to be equal to first_but_one and last_but_one


   pair<multimap<float, float>::iterator, multimap<float, float>::iterator> startStopPhases;
   multimap<float, float>::iterator pwrPhaseIter;

   float sumPhase;
   float meanPhase;
   for(pwrIter=PwrSet.begin(); pwrIter!=PwrSet.end(); pwrIter++)
   {
      startStopPhases = Pwr_Phase.equal_range(*pwrIter);

      sumPhase = 0.0;

      for(pwrPhaseIter = startStopPhases.first; pwrPhaseIter != startStopPhases.second; pwrPhaseIter++)
      {
         sumPhase = sumPhase + (*pwrPhaseIter).second;
      }
      meanPhase = sumPhase/Pwr_Phase.count(*pwrIter);

      inPwr.push_back(*pwrIter);
      phase.push_back(meanPhase);
   }

#ifdef DEBUG_PREDIST_AMPM_GAIN
   fnamestr1.str("");
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\AMPM_GAIN_Outputs_" << i << ".csv";

   ofstream ampmGainOpFile;
   ampmGainOpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < inPwr.size(); loopVar++)
   {
      ampmGainOpFile << inPwr[loopVar] << "," << phase[loopVar]<< endl;
   }
   ampmGainOpFile.close();
#endif
}


void QMSL_Linear_Predistortion_NV::splsmooth(const vector<float>& x, vector<float>& y, int meassize,  const vector<float>& xknots, int NknotIntervals)
{
   int ns,ind2=0,ind1=0,kr,j,li=0,i,lg,currRow;
   int xxknot_size = 0;
   float* R;
   float* A;
   float* coeffs;
   float * M;
   float* xxknots;
   float u = 0 ;
   float v = 0;
   float hj = 0;
   float sum;
   float *G;
   int A_size = 0;

   // Check that NknotIntervals is a valid size
   if ( NknotIntervals == 0 )
      return;

   G = (float*)malloc(25*sizeof(float));
   ns = NknotIntervals+ 3;
   R = (float*)malloc(ns*5*sizeof(float));
   M = (float*)malloc(meassize*4*sizeof(float));
   kr = 0;
   lg = 0;
   currRow = 0;
   xxknot_size = (NknotIntervals+5);
   xxknots = new float[xxknot_size];

   if(!G || !R || !M || !xxknots)
   {
      free(G);
      free(R);
      free(M);
      delete[] xxknots;
      return;
   }

   memset( G, 0, 25*sizeof(float) );
   memset( M, 0, meassize*4*sizeof(float));

   // Initialize xknots memory
   for(i = 0 ; i < xxknot_size ; i++)
   {
      xxknots[i] = 0;
   }

   xxknots[0] = xknots[0];
   xxknots[1] = xknots[0] ;

   for(i = 0 ; i <= NknotIntervals ; i++)
   {
      xxknots[i+2] = xknots[i];
   }
   xxknots[NknotIntervals+3] = xknots[NknotIntervals];
   xxknots[NknotIntervals+4] = xknots[NknotIntervals];

   //Orthogonal transformation
   if(NknotIntervals == 1)
   {
      third_order( x, y, meassize);
      // free allocated memory before returning
      delete[] xxknots;
      free(M);
      free(G);
      free(R);
      return;
   }
   else
   {
      for(j = 0; j <= NknotIntervals-1; j++)
      {
         ind1 = ind2;
         // Find the no of contributions due to interval j.
         while(ind2< meassize && x[ind2] <= xknots[j+1] )
         {
            ind2++;
         }
         li = ind2- ind1;
         if(li)
         {
            //NOTE : ind2 = one past last value in interval
            //Create A
            A_size = (ind2-ind1+lg)*5*sizeof(float);
            A = (float*)malloc(A_size);
            memset((void *) A,0, A_size );

            if(!A)
            {
               delete[] xxknots;
               free(M);
               free(G);
               free(R);
               return;
            }

            hj = xxknots[j+3] -xxknots[j+2];
            for(i = 0 ; i < li ; i++)
            {
               //fill spline matrix.
               u = x[ind1+i] - xxknots[j+2];
               v = xxknots[j+3] - x[ind1+i];
               A[5*i+0] = v/ (hj *(xxknots[j+3] - xxknots[j+1]));

               A[5*i+1] = u/ (hj *(xxknots[j+4] - xxknots[j+2]));
               A[5*i+2] = u * A[5*i+1]/(xxknots[j+5] - xxknots[j+2]);
               A[5*i+1] = ((x[ind1+i] -xxknots[j+1]) * A[5*i+0]  + (xxknots[j+4] - x[ind1+i]) * A[5*i+1])/(xxknots[j+4] - xxknots[j+1]);
               A[5*i+0] = v * A[5*i+0]/(xxknots[j+3] - xxknots[j+0]);
               A[5*i+3] = u* A[5*i+2];
               A[5*i+2] = ((x[ind1+i] - xxknots[j+1]) * A[5*i+1] + (xxknots[j+5] - x[ind1+i])*A[5*i+2]);
               A[5*i+1] = ((x[ind1+i] - xxknots[j])*A[5*i] + (xxknots[j+4] - x[ind1+i])*A[5*i+1]);
               A[5*i] = v*A[5*i];
               //Copy A to M
               M[(currRow)*4+0] = A[5*i];
               M[(currRow)*4+1] = A[5*i+1];
               M[(currRow)*4+2] = A[5*i+2];
               M[(currRow)*4+3] = A[5*i+3];
               currRow++;
               //augment Y to A.
               A[5*i+4] = y[ind1+i];
            }
            if(lg)
            {
               memcpy((void*)(A+5*li),(const void *)G,4*5*sizeof(float));
            }

            qrfact(A,li+lg,5);
            if(li+lg >=5)
            {
               memcpy(G,A,5*5*sizeof(float));
            }
            else
            {
               memcpy(G,A,(li+lg)*5*sizeof(float));
            }

            lg = 4;
            memcpy(R+5*kr,G,5*sizeof(float));
            for(i = 0 ; i < 4; i++)
            {
               memcpy((void *)(G+i*5),(const void*) (G+(i+1)*5+1),3*sizeof(float));
               G[i*5+3] = 0;
               G[i*5+4] = G[(i+1)*5+4];
            }
            //set last row of G to zero
            memset((void*)(G+4*5),0,5*sizeof(float));
            kr = kr+1;
            free(A);
         }
      }//for (j = 0:NKnotIntervals)

      //Transfer last rows to R
      memcpy((void *)(R+kr*5),(const void *)G,5*sizeof(float));
      for(i = 1 ; i < ns-kr; i++)
      {
         memcpy((void*)(R+(kr+i)*5),(const void *)(G+(i)*5+i),(4-i)*sizeof(float));
         memset((void *)(R+(kr+i)*5+4-i) ,0,i*sizeof(float));
         R[(kr+i)*5+4] = G[i*5+4];
      }
   }//if NknotIntervals > 1
   delete[] xxknots;
   free(G);

   //Get coefficients from R
   coeffs = (float *)malloc((ns+3)*sizeof(float));
   if(!coeffs)
   {
      free(R);
      free(M);
      return;
   }
   memset((void*) coeffs,0,(ns+3)*sizeof(float));
   for(i = ns-1 ; i >= 0; i--)
   {
      //d(i) = (R(i,5) - R(i,2:4)*d(i+[1:3]))/R(i,1);
      sum = R[i*5+1]*coeffs[i+1] + R[i*5+2]*coeffs[i+2] + R[i*5+3]*coeffs[i+3];
      coeffs[i] = R[i*5+4] - sum;
      if(R[i*5])
      {
         coeffs[i] /= R[i*5];
      }
   }

   free(R);
   //Get smoothed solution.
   //y = M*coeffs
   ind1 = 0 ;
   ind2 = 0;
   for(j = 0 ; j < NknotIntervals; j++)
   {
      ind1 = ind2;
      // Find the no of contributions due to interval j.
      while (ind2< meassize && x[ind2] <= xknots[j+1] )
      {
         ind2++;
      }
      li = ind2- ind1;
      if(li)
      {
         for(i = ind1; i < ind2; i++)
            y[i] = M[i*4]*coeffs[j] + M[i*4+1]*coeffs[j+1] + M[i*4+2]*coeffs[j+2] +M[i*4+3]*coeffs[j+3];
      }
   }

   free(coeffs);
   free(M);
}


void QMSL_Linear_Predistortion_NV::qrfact(float* A, int rows, int cols)
{
   int i,j,k,p,z;
   float housec=0;
   float normv = 0;
   float normw=0.0;
   float beta=0;
   float temp=0;
   float signvi = 0;

   float* H;
   float* tempA;

   for (i = 0; i <= cols-1; i++)
   {
      normv = 0 ;
      if (i >= rows-1)
         z = rows -1;
      else
         z = i;
      /*
      v = A(:,i);
      c = norm(v(i:end))* -sign(v(i));
      */

      if (A[z*cols+i] >= 0 )
         signvi = 1;
      else
         signvi = -1;

      for (j = z; j <= rows-1; j++ )
         normv = normv+ A[j*cols+i]*A[j*cols+i];

      housec = sqrt(normv)*(-signvi);

      /*
      w = [ zeros(1:i-1,1);v(i)-c;v(i+1:end,1)]
      beta = 2/(w'*w);
      H1 = eye(size(R,1)) - beta*w*w';
      A = H1*A;
      */
      if (housec)
      {
         normw = normv;
         normw = normw + housec*housec - 2*housec*A[z*cols+i];
         beta = 2/normw;

         /*
         Calculate H = H(i:end,i:end) since the part above is
         just identity matrix and has no affect on A
         */

         //Allocate space for  H
         H = (float*)malloc((rows-z)*(rows-z)*sizeof(float));
         if(!H)
         {
            return;
         }
         //Fill H
         for (k = 0; k <= rows-z-1; k++)
         {
            for (j = 0; j <= rows-z-1; j++)
            {
               if (k == j)
               {
                  // 1- v^2
                  H[k*(rows-z)+j] = 1 - beta*A[(z+k)*cols+i]*A[(z+k)*cols+i];
                  if (k == 0)
                  {
                     H[k*(rows-z)+j] = H[k*(rows-i)+j]+beta*(-housec*housec + 2*housec*A[(z+k)*cols+i]);
                  }
               }
               else if (k < j)
               {
                  H[k*(rows-z)+j] =  -beta*A[(z+k)*cols+i]*A[(z+j)*cols+i];
                  if (k == 0)
                  {
                     H[k*(rows-z)+j] = H[k*(rows-z)+j] + beta*housec* A[(z+j)*cols+i];
                  }
                  H[j*(rows-z)+k] = H[k*(rows-z)+j];
               }
            }
         }

         // matrix multiply
         //   tempA = H*A(i:end,:); A(i:end) = tempA;

         //Create tempA
         tempA = (float*)malloc((rows-z)*(cols-z)*sizeof(float));
         if(!tempA)
         {
            free(H);
            return;
         }
         for (k = z; k <= rows-1; k++)
         {
            for (j = z; j <=cols-1; j++)
            {
               temp = 0;
               for (p = z; p <=rows-1; p++)
                  temp = temp + H[(k-z)*(rows-z)+(p-z)]*A[p*cols+j];
               tempA[(k-z)*(cols-z)+(j-z)] = temp;
            }
         }
         // copy A(i:end) = tempA;
         for( k = z; k <=rows-1; k++)
         {
            for (j = z; j<=cols-1; j++)
            {
               temp = tempA[(k-z)*(cols-z)+(j-z)];
               A[k*cols+j] =  temp;
            }
         }

         //Free H and tempA
         free(H);
         free(tempA);
      }
   } //end for i

} //end of qrfact


void QMSL_Linear_Predistortion_NV::third_order(const vector<float>& x, vector<float>& y, int meassize)
{
   unsigned int row =0;
   int loopVar=0;
   // y = a + bx + cx^2 + dx^3
   float s_xny[4]; //0 - s_x0y,1 - s_xy, 2 - s_x2y
   float s_xn[7];  //0 - s_1, 1 - s_x  ,2 - s_x2, 6 - s_x6
   ZeroMemory(s_xny,4*sizeof(float));
   ZeroMemory(s_xn,7*sizeof(float));

   float a,b,c,d;

   float x_prods;
   unsigned int xpower;


   //Compute the elements of the matrix
   for( loopVar=0; loopVar<meassize; loopVar++)
   {
      x_prods = 1.0;
      for(xpower = 0; xpower < 4; xpower++)
      {
         s_xny[xpower] += pow(x[loopVar],(float)xpower) * y[loopVar];
      }
      for(xpower = 0; xpower < 7; xpower++)
      {
         s_xn[xpower] += pow(x[loopVar],(float)xpower);
      }
   }

   //Prepare the matrix
   float A[4][5];

   for( row = 0; row < 4; row ++)
   {
      for(unsigned int col = 0; col < 4; col ++)
      {
         A[row][col] = s_xn[row+col];
      }
   }

   for( row = 0; row < 4; row ++)
   {
      A[row][4] = s_xny[row];
   }

   qrfact(&(A[0][0]),4,5);

   d = (A[3][4])/A[3][3];
   c = (A[2][4] - d*A[2][3])/A[2][2];
   b = (A[1][4] - d*A[1][3] - c*A[1][2])/A[1][1];
   a = (A[0][4] - d*A[0][3] - c*A[0][2] -b*A[0][1])/A[0][0];

   //Now smooth
   for( loopVar=0; loopVar<meassize; loopVar++)
   {
      float x_ = x[loopVar];
      y[loopVar] = a + b*x_ + c*x_*x_ + d*x_*x_*x_;
   }
}



int QMSL_Linear_Predistortion_NV::choose_knots_x(const vector<float>& x, vector<float>& y, vector<float>& xknots,int meassize,float delta_x)
{
   int NKnots = 0;

   float min_x = *min_element(x.begin(),x.end());
   float max_x = *max_element(x.begin(),x.end());
   float current_x = min_x;

   while((current_x += delta_x) < max_x)
   {
      xknots.push_back( current_x );
      NKnots++;
   }

   xknots.push_back( max_x );
   NKnots++;

   return (NKnots-1);
}

int QMSL_Linear_Predistortion_NV::choose_knots_y(const vector<float>& x, vector<float>& y, vector<float>& xknots,int meassize,float delta_y)
{
   int NKnots = 0,i;
   float yknot=0;

   yknot = y[0];
   float xknot = x[0];
   xknots.push_back( xknot );
   NKnots++;
   for(i = 0; i < meassize-1; i++)
   {
      double yVal = y[i];
      double yValNext = y[i+1];
      if((yVal-yknot) > delta_y)
      {
         float xVal = x[i];
         float xValNext = x[i+1];
         NKnots++;
         xknots.push_back( (xValNext+xVal)/2 );
         yknot = (float)((yValNext+yVal)/2.0);
      }
   }

   xknot = x[meassize-1];
   xknots.push_back( xknot );
   return NKnots;
}

void QMSL_Linear_Predistortion_NV::smoothData( vector<float> InPwr, vector<float> InDistortion, vector<float>& SmoothedDistortion, float winSize, bool xknot)
{
   vector<float> xknots;
   int numIntervals;

   if(xknot)
   {
      numIntervals = choose_knots_x( InPwr, InDistortion, xknots, InPwr.size() ,winSize );
   }
   else
   {
      numIntervals = choose_knots_y( InPwr, InDistortion, xknots, InPwr.size() ,winSize );
   }

   SmoothedDistortion = InDistortion;
   splsmooth( InPwr, SmoothedDistortion,   InPwr.size(), xknots, numIntervals );

#ifdef DEBUG_SMOOTHING
   static int i = 0;
   i++;
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\Smoothing_IO_" << i << ".csv";

   ofstream smFile;
   smFile.open (fnamestr1.str().c_str(),ios::out);
   for(unsigned int loopVar = 0; loopVar < InPwr.size(); loopVar++)
   {
      smFile << InPwr[loopVar] << "," << InDistortion[loopVar]<< "," << SmoothedDistortion[loopVar]<< endl;
   }
   smFile.close();
#endif

}

void QMSL_Linear_Predistortion_NV::truncateData( float truncLevel, vector<float>& InPwr, vector<float>& Gain, vector<float>& Phase)
{
   vector<float>::iterator inpPwrIter,lastInpPwrIter;
   vector<float>::iterator lastGainIter,lastPhseIter;

   lastInpPwrIter = InPwr.begin();
   lastGainIter = Gain.begin();
   lastPhseIter = Phase.begin();
   for(inpPwrIter = InPwr.begin(); inpPwrIter!=InPwr.end(); inpPwrIter++)
   {
      if((*inpPwrIter) > truncLevel)
      {
         InPwr.erase(InPwr.begin(),lastInpPwrIter);
         Gain.erase(Gain.begin(),lastGainIter);
         Phase.erase(Phase.begin(),lastPhseIter);
         break;
      }
      lastInpPwrIter = inpPwrIter;
      if(inpPwrIter != InPwr.begin())
      {
         lastGainIter++;
         lastPhseIter++;
      }
   }
}

void QMSL_Linear_Predistortion_NV::generateAMAMNV( QMSL_Vector<float> targetOutputPowers, vector<float> inPwr,
                                                   vector<float> smoothedGain,   QMSL_Vector<long>& amamLinear)
{
   //Step 7.1.2 : Generate Measured Output Powers vector
   QMSL_2DVector<float, float> InterpAMAM;
   QMSL_Vector<float> outPower;
   QMSL_Vector<float> gainVec;
   QMSL_Vector<float> inPwrVec;
   unsigned long loopVar = 0;
   for(loopVar = 0; loopVar < inPwr.size(); loopVar++)
   {
      gainVec.Append(smoothedGain[loopVar]);
      outPower.Append(inPwr[loopVar]+smoothedGain[loopVar]);
      inPwrVec.Append(inPwr[loopVar]);
   }
   InterpAMAM.SetXYvectors(&outPower,&inPwrVec);

   //Step 7.1.3 : Generate AMAM Log Vector
   const int LINEAR_SECTION_OF_DATA = 2;
   QMSL_Vector<float> amAmLog;
   int numPointsToAverageSlope = inPwrVec.Size()/LINEAR_SECTION_OF_DATA;
   InterpAMAM.InterpolateY2vector_SlopeAveragingExtrapolation(targetOutputPowers,amAmLog,numPointsToAverageSlope);

   //Step 7.1.4 : Generate AMAM Linear Vector (Also from float to long)
   for(loopVar = 0; loopVar < amAmLog.Size(); loopVar++)
   {
      amamLinear.Append((long)pow(10.0,amAmLog[loopVar]/20.0));
   }

#ifdef DEBUG_AMAM
   static int amamIndex = 0;
   amamIndex++;
   ostringstream fnamestr1;
   fnamestr1.str("");

   ostringstream fnamestr2;
   fnamestr2.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr1 << "C:\\Qualcomm\\QDART\\Temp\\AMAM_NVGEN_Inputs_" << amamIndex << ".csv";
   fnamestr2 << "C:\\Qualcomm\\QDART\\Temp\\AMAM_NVGEN_Outputs_" << amamIndex << ".csv";
#else
   fnamestr1 <<P_tmpdir<<"AMAM_NVGEN_Inputs_"<<amamIndex<<".csv";
   fnamestr2 <<P_tmpdir<<"AMAM_NVGEN_Ouputs_" << amamIndex << ".csv";
#endif


   ofstream amamFile1,amamFile2;
   amamFile1.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < outPower.Size(); loopVar++)
   {
      amamFile1 << outPower[loopVar] << "," << gainVec[loopVar]<< "," << inPwrVec[loopVar]<<endl;
   }
   amamFile1.close();

   amamFile2.open (fnamestr2.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < targetOutputPowers.Size(); loopVar++)
   {
      amamFile2 << targetOutputPowers[loopVar] << "," << amAmLog[loopVar]<< "," << amamLinear[loopVar]<< endl;
   }
   amamFile2.close();

#endif
}


void QMSL_Linear_Predistortion_NV::generateAMPMNV( vector<float> inPwr, float dacScale, vector<float> smoothedPhaseDistortion, QMSL_Vector<long>& amPmProcessed)
{

   //Step 7.2.1 : Generate target input power (for phase).
   float maxPowerInput = *max_element(inPwr.begin(),inPwr.end()); //In Log Terms

   bool exceededMaxInputLevel = false;
   QMSL_Vector<float> targetInputPowers;
   unsigned int loopVar;
   for(loopVar = 1; loopVar < 128; loopVar++) //128*128 = 2^14
   {
      float inPwrLog = 20*log10(loopVar*(float)128.0*dacScale);
      if(!exceededMaxInputLevel && (inPwrLog < maxPowerInput ))
      {
         targetInputPowers.Append(inPwrLog);
      }
      else
      {
         targetInputPowers.Append(maxPowerInput);
         exceededMaxInputLevel = true;
      }
   }
   if(!exceededMaxInputLevel)
   {
      targetInputPowers.Append(20*log10((float)(pow(2.0,14)-1)*dacScale));
   }
   else
   {
      targetInputPowers.Append(maxPowerInput);
   }

   //Step 7.2.2 : Generate Array of Input Levels & Phases. Invert Phase
   QMSL_2DVector<float, float> InterpAMPM;
   QMSL_Vector<float> inPowers; //is the same as inPwrVec above (but done seperately to help refactorization)
   QMSL_Vector<float> phaseVec;
   for(loopVar = 0; loopVar < inPwr.size(); loopVar++)
   {
      inPowers.Append(inPwr[loopVar]);
      phaseVec.Append((float)-1.0*smoothedPhaseDistortion[loopVar]);
   }
   InterpAMPM.SetXYvectors(&inPowers,&phaseVec);

   //Step 7.2.3 : Generate AMPM Vector
   //Interpolate and figure out AMPM
   const int LINEAR_SECTION_OF_DATA = 8;
   QMSL_Vector<float> amPm;
   int numPointsToAverageSlope = inPowers.Size()/LINEAR_SECTION_OF_DATA;
   InterpAMPM.InterpolateY2vector_SlopeAveragingExtrapolation(targetInputPowers,amPm,numPointsToAverageSlope);

   //Step 7.2.4 : Normalize Phase.
   //Take min and set it to 0.
   QMSL_Vector<float> amPmNormalized;
   float minAmPm = *min_element(amPm._data.begin(),amPm._data.end());
   for(loopVar = 0; loopVar < amPm.Size(); loopVar++)
   {
      amPmNormalized.Append( amPm[loopVar] - minAmPm );
   }

   //Step 7.2.5 : Wrap into +/- 180
   float amPmVal;
   //Todo: Check if remainder is required
   float remainder;
   QMSL_Vector<float> amPmWrapped;
   for(loopVar = 0; loopVar < amPmNormalized.Size(); loopVar++)
   {
      amPmVal = amPmNormalized[loopVar];
      remainder = amPmVal-(long)amPmVal;
      if(amPmVal >= 0)
         amPmWrapped.Append(((((long)amPmVal+180)%360)-180)+remainder);
      else
         amPmWrapped.Append(((((long)amPmVal-180)%360)+180)+remainder);
   }

   //Step 7.2.6 : Scale to 16 bit number
   for(loopVar = 0; loopVar < amPmNormalized.Size(); loopVar++)
   {
      amPmProcessed.Append( (long)(amPmWrapped[loopVar]/180*pow(2.0,15)));
   }


#ifdef DEBUG_AMPM
   static int ampmIndex = 0;
   ampmIndex++;
   ostringstream fnamestr3;
   fnamestr3.str("");

   ostringstream fnamestr4;
   fnamestr4.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr3 << "C:\\Qualcomm\\QDART\\Temp\\AMPM_NVGEN_Inputs_" << ampmIndex << ".csv";
   fnamestr4 << "C:\\Qualcomm\\QDART\\Temp\\AMPM_NVGEN_Outputs_" << ampmIndex << ".csv";
#else
   fnamestr3 <<P_tmpdir<<"AMPM_NVGEN_Inputs_"<<ampmIndex<<".csv";
   fnamestr4 <<P_tmpdir<< "AMPM_NVGEN_Outputs" << ampmIndex << ".csv";
#endif

   ofstream ampmFile1,ampmFile2;
   ampmFile1.open (fnamestr3.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < inPowers.Size(); loopVar++)
   {
      ampmFile1 << inPowers[loopVar] << "," << phaseVec[loopVar]<<endl;
   }
   ampmFile1.close();

   ampmFile2.open (fnamestr4.str().c_str(),ios::out);
   for(loopVar = 0; loopVar < targetInputPowers.Size(); loopVar++)
   {
      ampmFile2 << targetInputPowers[loopVar] << "," << amPm[loopVar]<< "," << amPmNormalized[loopVar] << "," << amPmWrapped[loopVar]<< "," << amPmProcessed[loopVar] << endl ;
   }
   ampmFile2.close();

#endif
}

void  QMSL_Linear_Predistortion_NV::slopeCompensate( vector<float> inPwr, vector<float>& outVector, float slopeFactor)
{
#ifdef DEBUG_SLOPE_COMP
   static int slopeCompLogIndex = 0;
   slopeCompLogIndex++;
   ostringstream fnamestr;
   fnamestr.str("");
   fnamestr << "C:\\Qualcomm\\QDART\\Temp\\SlopeComp_" << slopeCompLogIndex << ".csv";
   ofstream slopeCompLog;
   slopeCompLog.open (fnamestr.str().c_str(),ios::out);
#endif

   float maxPwr = *max_element(inPwr.begin(),inPwr.end());
   for(unsigned int loopVar = 0; loopVar<inPwr.size(); loopVar++)
   {
#ifdef DEBUG_SLOPE_COMP
      slopeCompLog<<inPwr[loopVar]<<","<<outVector[loopVar];
#endif

      outVector[loopVar] = static_cast<float>(outVector[loopVar]+(inPwr[loopVar]-maxPwr)/10.0*slopeFactor);

#ifdef DEBUG_SLOPE_COMP
      slopeCompLog<<","<<outVector[loopVar]<<endl;;
#endif
   }
#ifdef DEBUG_SLOPE_COMP
   slopeCompLog.close();
#endif
}

void QMSL_Linear_Predistortion_NV::logParams(const QMSL_GSM_PreDist_Cal_Result *params)
{
#ifdef DEBUG_LOG_PREDIST_CAL_PARAMS
   static int i = 0;
   i++;
   std::ostringstream fnamestr;
   fnamestr.str("");

#if !defined(QMSL_POSIX_PORTABLE)
   fnamestr << "C:\\Qualcomm\\QDART\\Temp\\Predist_Input_Params_" << i << ".txt";
#else
   fnamestr <<P_tmpdir<<"/Predist_Input_Params_" << i << ".txt";
#endif

   ofstream predistParamLogFile;
   predistParamLogFile.open (fnamestr.str().c_str(),ios::out);

   predistParamLogFile << "iFreqMapping[0] :" << params->iFreqMapping[0] << endl;
   predistParamLogFile << "iFreqMapping[1] :" << params->iFreqMapping[1] << endl;
   predistParamLogFile << "iFreqMapping[2] :" << params->iFreqMapping[2] << endl;
   predistParamLogFile << "iNumChannels :" << params->iNumChannels << endl;
   predistParamLogFile << "iNumPredistortionWaveformSamples :" << params->iNumPredistortionWaveformSamples << endl;
   predistParamLogFile << "iCalRgi :" << params->iCalRgi << endl;
   predistParamLogFile << "iSamplingRateHz :" << params->iSamplingRateHz << endl;
   predistParamLogFile << "iDcSamples :" << params->iDcSamples << endl;
   predistParamLogFile << "iEdgeSamples :" << params->iEdgeSamples << endl;
   predistParamLogFile << "iNoiseSamples :" << params->iNoiseSamples << endl;
   predistParamLogFile << "iEDGETxGainParam :" << params->iEDGETxGainParam << endl;
   predistParamLogFile << "iEDGETxCalScale :" << params->iEDGETxCalScale << endl;
   predistParamLogFile << "iDCTransientPercent :" << params->iDCTransientPercent << endl;
   predistParamLogFile << "iEDGETransientSymbols :" << params->iEDGETransientSymbols << endl;
   predistParamLogFile << "iOverRideModemConstants :" << params->iOverRideModemConstants << endl;
   predistParamLogFile << "dDigGainUnity :" << params->dDigGainUnity << endl;
   predistParamLogFile << "dRampUnity :" << params->dRampUnity << endl;
   predistParamLogFile << "dExtensionFloor :" << params->dExtensionFloor << endl;
   predistParamLogFile << "dDacScale :" << params->dDacScale << endl;

   predistParamLogFile.close();
#endif

}

#define MAX_ARR_SIZE 8192
#define MAX_DELAYS 7
long QMSL_Linear_Predistortion_NV::updateDelay
(float * CVec1,const float * CVec2, long CVecSize)
{

   static float gainArrDelay[MAX_ARR_SIZE][MAX_DELAYS]= {0.0};
   float gainMaxDelay[MAX_DELAYS] = {-10000.0,-10000.0,-10000.0,-10000.0,-10000.0,-10000.0,-10000.0};
   float gainMinDelay[MAX_DELAYS] = {10000.0,10000.0,10000.0,10000.0,10000.0,10000.0,10000.0};
   float rangeDelays[MAX_DELAYS] = {0};

   int delaySet[MAX_DELAYS] = {-3,-2,-1,0,1,2,3};
   int numDelays = 7;

   const int SKIP_SAMPLES = 300;
   const int HARD_CODED_AMAM_IN_BIN = 75;
   int lpVar = 0;

   for(lpVar = SKIP_SAMPLES; lpVar<CVecSize-SKIP_SAMPLES; lpVar++)
   {
      for(int delayVar = 0; delayVar<numDelays; delayVar++)
      {
         gainArrDelay[lpVar][delayVar] = CVec2[lpVar + delaySet[delayVar] ]-CVec1[lpVar];
      }
   }
#ifdef DEBUG_UPDATE_DELAY_FOR_AMAM_GAIN_SPREAD
   static int i = 0;
   i++;
   ostringstream fnamestr;
   fnamestr.str("");
   fnamestr << "C:\\Qualcomm\\QDART\\Temp\\Delay_Update_" << i << ".csv";
   FILE* fp = fopen(fnamestr.str().c_str(),"w");
#endif
   for(lpVar = SKIP_SAMPLES+2; lpVar<CVecSize-SKIP_SAMPLES; lpVar++)
   {
      if( (CVec1[lpVar] >= HARD_CODED_AMAM_IN_BIN && CVec1[lpVar-1] <= HARD_CODED_AMAM_IN_BIN) ||
            (CVec1[lpVar] <= HARD_CODED_AMAM_IN_BIN && CVec1[lpVar-1] >= HARD_CODED_AMAM_IN_BIN))
      {
#ifdef DEBUG_UPDATE_DELAY_FOR_AMAM_GAIN_SPREAD
         fprintf(fp,"%d,",lpVar);
#endif
         for(int delayVar = 0; delayVar<numDelays; delayVar++)
         {
            if(gainArrDelay[lpVar][delayVar] > gainMaxDelay[delayVar])
               gainMaxDelay[delayVar] = gainArrDelay[lpVar][delayVar];
            if(gainArrDelay[lpVar][delayVar] < gainMinDelay[delayVar])
               gainMinDelay[delayVar] = gainArrDelay[lpVar][delayVar];
            rangeDelays[delayVar] = gainMaxDelay[delayVar] - gainMinDelay[delayVar];
#ifdef DEBUG_UPDATE_DELAY_FOR_AMAM_GAIN_SPREAD
            fprintf(fp,"%f,%f,",gainMaxDelay[delayVar],gainMinDelay[delayVar]);
#endif
         }
#ifdef DEBUG_UPDATE_DELAY_FOR_AMAM_GAIN_SPREAD
         fprintf(fp,"\n");
#endif
      }
   }

   long delayUpdate;
   float smallestGainVariation = 10000.0;
   for(int delayVar = 0; delayVar<numDelays; delayVar++)
   {
      if(smallestGainVariation >= rangeDelays[delayVar])
      {
         smallestGainVariation = rangeDelays[delayVar];
         delayUpdate = delaySet[delayVar];
      }
   }

#ifdef DEBUG_UPDATE_DELAY_FOR_AMAM_GAIN_SPREAD
   fprintf(fp,"%d\n",delayUpdate);
   fclose(fp);
#endif

   return delayUpdate;
}


//Ignore double to float error, as this array has to be float
#pragma warning(disable:4305)

const float QMSL_Linear_Predistortion_NV::ampl_in_self_cal[SELF_CAL_ENV_IN_NUM_SAMPLES] =
{10769.67659 ,11065.41571 ,10931.34459 ,10986.69245 ,10923.13308 ,10929.84607 ,10885.26452 ,10868.6851 ,10832.34147 ,10807.17572 ,10777.05398 ,10748.06853 ,10722.42335 ,10699.86548 ,10682.45029 ,10668.14103 ,10660.10297 ,10656.20364 ,10658.60466 ,10666.84389 ,10680.39537 ,10701.51355 ,10724.80884 ,10755.47994 ,10787.8225 ,10823.3585 ,10862.07296 ,10899.82068 ,10938.85547 ,10976.21749 ,11010.65464 ,11042.95692 ,11071.49831 ,11094.28676 ,11112.27501 ,11121.90813 ,11123.35011 ,11117.9121 ,11106.47002 ,11082.68827 ,11050.42425 ,11010.3922 ,10959.8528 ,10900.90986 ,10833.24639 ,10756.31716 ,10669.43765 ,10575.06143 ,10473.03275 ,10361.70822 ,10243.56434 ,10118.5938 ,9989.817926 ,9852.611358 ,9712.972823 ,9568.515281 ,9422.052435 ,9273.622133 ,9123.651615 ,8974.581084 ,8826.70395 ,8682.715521 ,8543.359091 ,8407.787545 ,8280.008776 ,8159.374732 ,8047.555276 ,7944.195138 ,7850.036919 ,7765.858463 ,7690.844254 ,7625.55841 ,7567.092753 ,7515.862073 ,7470.76159 ,7429.093273 ,7390.180555 ,7350.805597 ,7312.008325 ,7268.577487 ,7220.391185 ,7164.153166 ,7099.869461 ,7025.619071 ,6938.235569 ,6838.407111 ,6725.548252 ,6596.720591 ,6452.538064 ,6293.970098 ,6118.529584 ,5926.43042 ,5720.173524 ,5500.551041 ,5264.832914 ,5017.226977 ,4760.295576 ,4493.895245 ,4223.536706 ,3950.983488 ,3684.222787 ,3426.187576 ,3185.653298 ,2975.070026 ,2806.052284 ,2683.509826 ,2626.058364 ,2644.199954 ,2726.535843 ,2877.10922 ,3084.887793 ,3336.366585 ,3621.122728 ,3934.98452 ,4265.0237 ,4606.585254 ,4954.808204 ,5306.032197 ,5657.717264 ,6007.433282 ,6352.20539 ,6690.86478 ,7023.081323 ,7344.47234 ,7656.472704 ,7958.470614 ,8247.342586 ,8524.47516 ,8789.620219 ,9038.75648 ,9274.377531 ,9496.834145 ,9701.831795 ,9892.023459 ,10068.97194 ,10225.52427 ,10367.45064 ,10495.74186 ,10606.51172 ,10702.7428 ,10783.32241 ,10848.41848 ,10898.55385 ,10934.52048 ,10958.46532 ,10964.56982 ,10959.75446 ,10943.1537 ,10913.24084 ,10869.97883 ,10817.23288 ,10751.79068 ,10675.95166 ,10590.82654 ,10494.90459 ,10388.32023 ,10274.58494 ,10153.60246 ,10024.38433 ,9888.219198 ,9746.879044 ,9600.681707 ,9450.283025 ,9297.807059 ,9142.30294 ,8987.499083 ,8833.016878 ,8680.855122 ,8530.848551 ,8385.942457 ,8246.723589 ,8114.484425 ,7991.083113 ,7877.876306 ,7775.966627 ,7686.007976 ,7608.373193 ,7546.161932 ,7499.798453 ,7466.828043 ,7450.818057 ,7451.767765 ,7466.361955 ,7498.168608 ,7546.587739 ,7608.042143 ,7684.716195 ,7774.605644 ,7876.33387 ,7988.95844 ,8113.116849 ,8245.518178 ,8385.887185 ,8532.219317 ,8683.439168 ,8838.496482 ,8996.342659 ,9155.017029 ,9313.534829 ,9471.411631 ,9626.705466 ,9777.797994 ,9925.03893 ,10064.64388 ,10198.97479 ,10326.86726 ,10447.1403 ,10558.24122 ,10663.13427 ,10758.31461 ,10845.32202 ,10926.61863 ,11000.26107 ,11067.27268 ,11129.86338 ,11186.24966 ,11237.30678 ,11288.12328 ,11336.13408 ,11382.88921 ,11429.39298 ,11477.80729 ,11528.14567 ,11581.10476 ,11636.58638 ,11698.7626 ,11762.71392 ,11830.68688 ,11903.29588 ,11981.82905 ,12063.37557 ,12147.90333 ,12235.5169 ,12325.03186 ,12417.96046 ,12511.07667 ,12603.59584 ,12694.90947 ,12784.62129 ,12869.01729 ,12951.32878 ,13027.04673 ,13095.87443 ,13158.46313 ,13212.38173 ,13258.5102 ,13294.16084 ,13322.09 ,13337.5363 ,13344.86515 ,13341.92912 ,13327.3275 ,13303.71678 ,13271.99424 ,13231.13797 ,13181.82916 ,13125.55749 ,13062.93577 ,12995.40372 ,12923.82672 ,12848.83572 ,12769.94708 ,12688.41653 ,12608.10407 ,12526.83217 ,12445.828 ,12365.74039 ,12286.24076 ,12209.81467 ,12134.83994 ,12062.96397 ,11990.7607 ,11922.47371 ,11856.92411 ,11791.48065 ,11726.92248 ,11661.59791 ,11598.16103 ,11532.87785 ,11464.41393 ,11394.65036 ,11322.16168 ,11244.72515 ,11163.65232 ,11077.88607 ,10986.73984 ,10887.45892 ,10783.2962 ,10673.20845 ,10555.27286 ,10430.3047 ,10299.51409 ,10161.71269 ,10019.5544 ,9872.114511 ,9719.63948 ,9562.82306 ,9404.475302 ,9243.565991 ,9080.940263 ,8917.680089 ,8757.310022 ,8599.17842 ,8443.078911 ,8292.760376 ,8147.814329 ,8008.753496 ,7875.920343 ,7751.43462 ,7635.651167 ,7528.014676 ,7430.110697 ,7340.355362 ,7260.097053 ,7188.628401 ,7126.486632 ,7070.992376 ,7024.459681 ,6986.180144 ,6954.591624 ,6928.001483 ,6906.850414 ,6892.272679 ,6879.319354 ,6871.185305 ,6865.749288 ,6863.209466 ,6861.206883 ,6860.351691 ,6860.588325 ,6861.606839 ,6863.706589 ,6866.401693 ,6866.933566 ,6867.99899 ,6869.347472 ,6870.646156 ,6870.347157 ,6869.129722 ,6868.97822 ,6868.613544 ,6866.339284 ,6863.95941 ,6862.470601 ,6859.88107 ,6857.043686 ,6854.59063 ,6850.78605 ,6846.951037 ,6844.443237 ,6842.022198 ,6839.012117 ,6836.6964 ,6835.801521 ,6834.694981 ,6835.025557 ,6835.979008 ,6836.33432 ,6838.301029 ,6841.013703 ,6844.092947 ,6847.600015 ,6852.099694 ,6854.634459 ,6858.906415 ,6862.225903 ,6866.82535 ,6870.214672 ,6873.781575 ,6877.971591 ,6880.41559 ,6882.393349 ,6885.261845 ,6888.725449 ,6890.653483 ,6893.745455 ,6897.980525 ,6900.615222 ,6906.026511 ,6912.900255 ,6920.568397 ,6929.505144 ,6940.840502 ,6953.493558 ,6967.825964 ,6985.051558 ,7002.997903 ,7024.04031 ,7044.779762 ,7065.856079 ,7087.529462 ,7109.992738 ,7128.225012 ,7144.686873 ,7157.252026 ,7164.162678 ,7165.519412 ,7160.209575 ,7145.028821 ,7119.335615 ,7084.387677 ,7035.191825 ,6973.825293 ,6898.43949 ,6807.310426 ,6700.595198 ,6580.079853 ,6442.761643 ,6286.312224 ,6116.090783 ,5929.993534 ,5726.136927 ,5508.638928 ,5278.498264 ,5035.052601 ,4781.693091 ,4520.255547 ,4252.865181 ,3984.006381 ,3716.346103 ,3459.549938 ,3216.189857 ,2994.085617 ,2805.586168 ,2660.748697 ,2561.711774 ,2522.74681 ,2545.886407 ,2623.500375 ,2751.364765 ,2922.344377 ,3122.023484 ,3344.55923 ,3579.767199 ,3822.672783 ,4068.2607 ,4311.629811 ,4549.090076 ,4779.645364 ,5000.72297 ,5209.641804 ,5409.552977 ,5595.200648 ,5766.858209 ,5926.139121 ,6073.441774 ,6206.177113 ,6326.29289 ,6434.347237 ,6531.206171 ,6617.755363 ,6696.423614 ,6764.479335 ,6825.419683 ,6879.05587 ,6927.327036 ,6970.019794 ,7009.039805 ,7043.320654 ,7074.297209 ,7103.148357 ,7128.744145 ,7151.153232 ,7169.482412 ,7184.440641 ,7193.814968 ,7197.461277 ,7194.562082 ,7183.639032 ,7164.820164 ,7136.269377 ,7096.297335 ,7043.835611 ,6978.787391 ,6900.673834 ,6807.417623 ,6699.860337 ,6577.042712 ,6437.766668 ,6283.703381 ,6115.675669 ,5929.97236 ,5731.319963 ,5520.161492 ,5296.430568 ,5063.984116 ,4824.525319 ,4581.631347 ,4338.520286 ,4098.451158 ,3868.604358 ,3657.453106 ,3466.450057 ,3309.487981 ,3194.158782 ,3122.892774 ,3102.424905 ,3136.278637 ,3221.187547 ,3346.782654 ,3515.724179 ,3712.547806 ,3928.659388 ,4162.311961 ,4403.619755 ,4648.151658 ,4890.888136 ,5129.465802 ,5360.700682 ,5583.081853 ,5790.979446 ,5987.675484 ,6170.529755 ,6338.494809 ,6492.075891 ,6630.846821 ,6753.08527 ,6862.146693 ,6959.613318 ,7042.0578 ,7112.485749 ,7174.742429 ,7229.808974 ,7275.705278 ,7318.174234 ,7357.005274 ,7396.682685 ,7436.564322 ,7477.193182 ,7525.878958 ,7577.049822 ,7636.511741 ,7704.087081 ,7781.385334 ,7867.352649 ,7961.810313 ,8066.460466 ,8181.674623 ,8302.769944 ,8432.339414 ,8567.814071 ,8707.944011 ,8853.436628 ,9000.750752 ,9148.975646 ,9298.349726 ,9447.906325 ,9594.879362 ,9739.20911 ,9880.292038 ,10017.08595 ,10148.44746 ,10274.34414 ,10393.96178 ,10505.87803 ,10611.83431 ,10709.23796 ,10798.39122 ,10880.86794 ,10953.44534 ,11019.24877 ,11075.1538 ,11122.09181 ,11160.31526 ,11189.84938 ,11209.0544 ,11220.51478 ,11222.2048 ,11213.94785 ,11197.47986 ,11173.30683 ,11138.2864 ,11096.12513 ,11046.10182 ,10985.32564 ,10915.75724 ,10839.84481 ,10755.08756 ,10661.26755 ,10561.51443 ,10452.53716 ,10338.26181 ,10216.0313 ,10086.82396 ,9953.631033 ,9816.222364 ,9673.607072 ,9526.99302 ,9378.780932 ,9231.212622 ,9083.580849 ,8937.587247 ,8795.480757 ,8658.667416 ,8528.599182 ,8407.924817 ,8299.265954 ,8201.800287 ,8118.902849 ,8051.93924 ,8002.273203 ,7971.341843 ,7961.846219 ,7971.8645 ,8002.233135 ,8054.028512 ,8128.817565 ,8221.848778 ,8334.240922 ,8466.127691 ,8612.845058 ,8774.618439 ,8951.958303 ,9140.114087 ,9337.239312 ,9542.297589 ,9753.426723 ,9970.089832 ,10190.50695 ,10409.82531 ,10630.04732 ,10849.78077 ,11065.5141 ,11275.92042 ,11481.01431 ,11678.54124 ,11867.65936 ,12049.06564 ,12219.38926 ,12378.05842 ,12526.16385 ,12661.36387 ,12781.95989 ,12891.76807 ,12986.66646 ,13064.09444 ,13131.43723 ,13182.6908 ,13219.99298 ,13243.76147 ,13255.68431 ,13253.9003 ,13240.50848 ,13216.48196 ,13182.57113 ,13140.78214 ,13089.98365 ,13032.8943 ,12969.30844 ,12901.63934 ,12829.55846 ,12754.84708 ,12677.53483 ,12599.36518 ,12521.38627 ,12443.99488 ,12368.57779 ,12293.5489 ,12221.83968 ,12150.1517 ,12084.22569 ,12022.23733 ,11961.74395 ,11905.84078 ,11853.93249 ,11806.10178 ,11762.65389 ,11724.10308 ,11686.85866 ,11655.56629 ,11626.31474 ,11601.8295 ,11580.19771 ,11562.64707 ,11549.7702 ,11539.85616 ,11533.8585 ,11531.807 ,11535.17902 ,11542.66402 ,11555.53027 ,11574.27229 ,11597.51926 ,11627.81894 ,11662.8935 ,11705.3243 ,11754.11498 ,11808.58638 ,11869.74307 ,11937.58623 ,12010.48184 ,12088.48181 ,12170.40621 ,12258.23453 ,12346.51446 ,12438.83834 ,12531.59305 ,12625.46677 ,12717.98115 ,12806.74728 ,12895.3416 ,12977.92399 ,13057.31157 ,13130.62973 ,13196.61318 ,13258.05311 ,13310.1135 ,13354.86447 ,13391.05004 ,13420.85021 ,13443.12278 ,13455.49315 ,13463.42226 ,13465.24692 ,13462.4588 ,13453.68796 ,13441.74711 ,13428.26739 ,13412.3719 ,13396.67754 ,13379.93079 ,13364.64718 ,13352.41413 ,13341.70465 ,13334.21131 ,13329.20171 ,13327.61684 ,13331.33378 ,13339.13519 ,13348.61791 ,13362.39815 ,13378.78408 ,13397.54826 ,13419.02123 ,13440.65999 ,13461.74034 ,13484.88824 ,13506.36827 ,13527.66001 ,13545.84016 ,13561.01587 ,13574.73958 ,13583.19135 ,13589.34949 ,13591.51337 ,13589.27846 ,13583.08059 ,13573.55179 ,13561.31485 ,13546.36699 ,13526.47721 ,13506.68109 ,13484.82426 ,13463.93991 ,13440.8451 ,13418.29185 ,13398.89981 ,13380.21164 ,13363.56808 ,13351.43057 ,13341.76708 ,13336.04549 ,13334.73235 ,13338.11591 ,13345.66774 ,13357.18469 ,13372.44787 ,13390.94066 ,13410.81355 ,13435.11331 ,13460.11876 ,13484.91275 ,13512.3552 ,13536.42026 ,13561.53021 ,13583.87187 ,13603.09265 ,13619.44574 ,13632.75184 ,13640.84374 ,13645.30907 ,13647.93943 ,13645.41741 ,13638.50972 ,13628.8458 ,13615.53836 ,13601.0252 ,13585.44162 ,13568.26718 ,13550.35564 ,13533.27055 ,13516.57021 ,13501.91932 ,13487.51249 ,13475.71697 ,13469.16461 ,13462.15052 ,13457.52349 ,13456.76187 ,13458.63156 ,13463.10744 ,13467.42951 ,13472.43507 ,13477.24831 ,13482.75233 ,13487.27521 ,13488.74872 ,13485.77406 ,13477.38322 ,13465.92426 ,13445.5484 ,13418.77529 ,13384.76512 ,13338.82813 ,13283.18924 ,13215.91979 ,13137.76134 ,13046.85784 ,12943.97833 ,12827.02331 ,12697.54081 ,12555.90574 ,12401.56796 ,12235.34989 ,12057.5878 ,11869.25588 ,11670.83779 ,11462.52825 ,11246.51581 ,11026.09835 ,10799.46796 ,10570.29887 ,10337.90378 ,10106.00765 ,9876.804428 ,9648.130069 ,9426.533716 ,9211.000087 ,9001.873854 ,8803.674036 ,8614.549842 ,8437.322031 ,8272.101823 ,8122.739812 ,7986.257224 ,7863.884074 ,7755.663932 ,7662.530561 ,7582.665105 ,7515.785441 ,7459.395839 ,7413.590246 ,7377.162568 ,7346.660776 ,7320.703014 ,7298.038053 ,7275.563445 ,7252.013181 ,7224.259487 ,7192.399973 ,7152.585156 ,7105.558698 ,7046.368317 ,6976.516494 ,6893.824837 ,6795.997261 ,6683.736633 ,6556.846225 ,6413.405292 ,6252.863309 ,6077.547596 ,5885.832909 ,5677.464553 ,5455.264332 ,5219.224967 ,4970.983687 ,4713.811816 ,4449.675082 ,4182.716125 ,3918.090068 ,3660.740298 ,3421.004629 ,3207.671119 ,3026.767622 ,2902.319347 ,2838.152947 ,2839.405091 ,2920.414909 ,3073.446084 ,3286.058782 ,3552.999475 ,3865.898467 ,4207.97053 ,4575.858033 ,4962.778653 ,5360.805043 ,5766.474089 ,6177.921859 ,6591.910513 ,7003.70937 ,7412.867033 ,7817.770074 ,8214.226793 ,8600.77172 ,8977.946612 ,9346.246456 ,9700.432584 ,10041.25636 ,10368.87099 ,10681.69721 ,10976.3689 ,11255.60923 ,11518.84697 ,11763.62732 ,11990.30534 ,12199.62263 ,12390.50498 ,12562.11254 ,12718.70775 ,12857.0735 ,12976.93726 ,13082.79744 ,13171.46326 ,13246.60581 ,13307.31296 ,13355.8052 ,13393.92916 ,13422.34193 ,13441.55786 ,13453.60951 ,13461.17323 ,13463.59569 ,13463.16455 ,13461.14368 ,13455.71965 ,13453.37734 ,13450.24512 ,13447.44067 ,13447.74449 ,13448.36292 ,13452.59601 ,13458.60974 ,13466.27378 ,13477.38246 ,13488.43452 ,13501.11826 ,13512.82154 ,13522.8075 ,13532.59489 ,13540.46229 ,13545.85896 ,13545.20272 ,13541.95041 ,13533.88519 ,13518.80583 ,13498.53693 ,13472.04769 ,13438.22089 ,13397.81123 ,13350.16713 ,13295.82539 ,13232.25285 ,13164.39291 ,13091.12804 ,13011.31258 ,12926.93368 ,12839.12672 ,12748.92464 ,12655.89679 ,12562.84524 ,12467.60378 ,12372.98499 ,12282.08012 ,12193.4884 ,12108.41436 ,12026.83975 ,11951.85473 ,11880.77997 ,11817.06174 ,11759.31012 ,11708.73447 ,11665.19538 ,11628.07088 ,11596.72764 ,11573.785 ,11554.45533 ,11541.81674 ,11534.28627 ,11531.66728 ,11533.91003 ,11539.96428 ,11549.81425 ,11564.12779 ,11582.64679 ,11603.04566 ,11626.7844 ,11654.38334 ,11686.71359 ,11719.61837 ,11757.54292 ,11799.73574 ,11844.6144 ,11895.12123 ,11948.19558 ,12006.42962 ,12066.40181 ,12133.1755 ,12201.11113 ,12271.70042 ,12346.22208 ,12423.35202 ,12499.92976 ,12577.38455 ,12654.00452 ,12730.39458 ,12802.92908 ,12873.08317 ,12938.5214 ,12996.41299 ,13046.51705 ,13088.90337 ,13122.39903 ,13143.5023 ,13152.45952 ,13148.22715 ,13129.96551 ,13095.99834 ,13048.46065 ,12982.39579 ,12897.47699 ,12796.46075 ,12675.30808 ,12536.51284 ,12377.65543 ,12201.1665 ,12004.26862 ,11787.9409 ,11553.69896 ,11300.28941 ,11030.20824 ,10743.15122 ,10437.96799 ,10115.97116 ,9779.541589 ,9428.269732 ,9063.520029 ,8688.160443 ,8300.36285 ,7903.867882 ,7499.357197 ,7087.563847 ,6669.760085 ,6250.243299 ,5827.470094 ,5408.030587 ,4993.164651 ,4586.485127 ,4195.598857 ,3824.58468 ,3483.234506 ,3188.602173 ,2952.830175 ,2788.097852 ,2714.385602 ,2742.471968 ,2856.008901 ,3055.717673 ,3327.220712 ,3645.020486 ,4001.604909 ,4383.418447 ,4783.571533 ,5194.821866 ,5612.297835 ,6033.745419 ,6456.551679 ,6874.067935 ,7288.554518 ,7696.780077 ,8098.548873 ,8488.980968 ,8868.693672 ,9236.603623 ,9591.526527 ,9931.394091 ,10256.81782 ,10562.72143 ,10853.15465 ,11124.4424 ,11372.66264 ,11599.77462 ,11806.40682 ,11988.24232 ,12145.439 ,12279.60361 ,12387.01058 ,12467.73251 ,12524.54081 ,12554.38081 ,12557.26449 ,12535.64703 ,12487.18266 ,12411.68762 ,12311.22348 ,12187.00717 ,12036.90261 ,11864.46634 ,11668.71549 ,11448.81594 ,11208.61301 ,10949.40941 ,10669.62197 ,10371.35473 ,10057.01436 ,9723.256584 ,9374.623922 ,9012.991514 ,8639.094581 ,8251.622487 ,7856.027991 ,7450.135334 ,7038.663713 ,6622.456229 ,6202.148242 ,5780.146528 ,5361.559319 ,4947.424315 ,4542.174524 ,4153.582697 ,3785.253334 ,3448.094607 ,3160.314576 ,2928.750465 ,2772.76257 ,2715.578128 ,2747.851679 ,2873.989215 ,3085.653591 ,3359.482215 ,3681.937245 ,4042.750839 ,4428.006051 ,4828.303436 ,5241.495256 ,5660.703481 ,6082.166978 ,6503.079087 ,6922.205843 ,7336.312242 ,7745.348187 ,8145.251067 ,8536.070653 ,8916.312472 ,9283.374829 ,9639.317372 ,9980.938092 ,10305.10045 ,10612.83278 ,10906.1292 ,11179.8719 ,11433.53968 ,11668.00164 ,11881.05904 ,12070.27202 ,12239.39098 ,12386.85604 ,12509.09213 ,12608.60924 ,12686.29259 ,12740.13518 ,12770.84417 ,12781.72024 ,12768.91532 ,12735.24083 ,12681.10927 ,12607.61298 ,12516.54979 ,12406.64878 ,12281.0744 ,12137.99176 ,11983.14014 ,11815.41707 ,11634.34529 ,11443.57878 ,11245.09802 ,11036.5668 ,10821.41958 ,10602.77499 ,10380.27304 ,10155.90384 ,9930.522228 ,9705.2479 ,9483.764552 ,9264.655652 ,9053.458206 ,8847.929461 ,8648.468528 ,8459.489497 ,8280.88524 ,8112.408521 ,7954.363515 ,7810.700986 ,7677.979973 ,7558.205213 ,7449.83204 ,7354.865654 ,7270.35099 ,7199.027115 ,7136.496959 ,7084.203293 ,7040.06744 ,7003.623761 ,6974.685946 ,6950.811488 ,6932.835577 ,6918.206027 ,6906.754262 ,6898.21361 ,6891.540267 ,6888.526065 ,6884.26012 ,6883.001703 ,6882.664964 ,6881.566482 ,6883.132985 ,6884.831406 ,6887.289421 ,6891.45952 ,6898.003802 ,6905.676901 ,6918.030448 ,6932.140095 ,6951.307425 ,6975.802992 ,7004.638873 ,7041.687667 ,7085.640093 ,7138.696038 ,7200.774599 ,7274.352361 ,7357.405944 ,7453.180468 ,7560.878588 ,7682.267624 ,7814.669228 ,7960.384203 ,8117.400909 ,8286.441345 ,8466.212438 ,8655.51109 ,8854.57659 ,9060.191935 ,9273.67248 ,9492.394725 ,9714.286702 ,9939.931107 ,10166.3246 ,10393.32784 ,10617.44318 ,10839.5011 ,11057.22505 ,11269.69942 ,11474.61792 ,11671.86627 ,11858.70775 ,12035.4565 ,12202.61642 ,12358.76853 ,12499.00189 ,12627.66459 ,12743.10618 ,12842.09463 ,12927.37327 ,12997.09983 ,13051.42974 ,13092.11022 ,13117.42445 ,13128.92614 ,13124.05397 ,13109.48143 ,13081.43558 ,13040.81233 ,12989.92416 ,12929.88102 ,12861.23841 ,12786.04711 ,12703.8317 ,12618.71725 ,12529.79271 ,12440.66295 ,12349.67 ,12260.2037 ,12171.242 ,12086.97952 ,12004.73451 ,11926.30942 ,11851.67979 ,11782.3658 ,11718.63647 ,11657.87085 ,11601.01124 ,11548.05929 ,11498.95319 ,11451.77193 ,11405.61813 ,11360.20848 ,11311.1423 ,11261.36355 ,11211.3039 ,11157.22283 ,11096.14506 ,11030.23999 ,10957.14966 ,10878.36262 ,10791.71059 ,10699.85476 ,10598.08268 ,10487.99391 ,10371.78511 ,10246.96115 ,10115.73788 ,9978.703413 ,9835.082103 ,9686.010584 ,9533.459677 ,9378.366503 ,9220.834879 ,9062.569711 ,8905.507178 ,8748.936113 ,8595.353537 ,8444.19823 ,8300.179806 ,8162.698719 ,8030.204342 ,7907.815241 ,7793.79951 ,7690.123246 ,7597.228799 ,7513.964681 ,7442.642738 ,7381.68588 ,7331.546466 ,7291.267991 ,7260.046312 ,7236.756825 ,7220.952223 ,7210.661343 ,7205.940322 ,7204.212995 ,7203.823406 ,7202.20916 ,7199.722764 ,7193.331575 ,7179.860334 ,7159.394274 ,7131.145508 ,7092.495837 ,7041.662163 ,6978.953483 ,6903.185427 ,6810.771933 ,6704.460372 ,6583.924105 ,6444.35262 ,6289.663004 ,6118.903531 ,5931.492381 ,5729.467759 ,5512.64573 ,5282.159581 ,5038.841342 ,4784.586921 ,4523.190898 ,4255.95977 ,3986.798352 ,3721.799849 ,3464.29185 ,3220.03934 ,2998.587452 ,2809.131158 ,2663.241279 ,2561.820824 ,2524.121851 ,2546.844476 ,2621.396694 ,2749.879054 ,2919.854608 ,3118.388677 ,3340.359466 ,3575.524351 ,3817.781769 ,4062.833571 ,4306.240206 ,4543.019209 ,4773.285047 ,4993.210479 ,5201.835572 ,5398.762822 ,5583.536902 ,5753.054148 ,5910.459055 ,6052.695967 ,6181.985576 ,6298.014923 ,6401.095824 ,6490.099148 ,6567.168576 ,6634.244514 ,6691.406865 ,6737.006223 ,6775.910766 ,6807.724024 ,6830.535522 ,6849.06869 ,6863.508713 ,6873.704855 ,6879.687835 ,6883.795426 ,6887.000503 ,6888.292312 ,6888.874077 ,6888.626313 ,6887.679256 ,6886.85611 ,6886.262704 ,6886.056375 ,6884.987969 ,6884.075484 ,6884.274798 ,6884.935981 ,6883.961405 ,6883.294605 ,6883.887155 ,6882.570768 ,6883.033255 ,6882.195204 ,6882.529898 ,6881.789795 ,6881.529043 ,6881.797533 ,6881.585503 ,6881.590551 ,6882.888663 ,6882.84542 ,6884.540905 ,6885.53638 ,6887.139512 ,6888.912365 ,6890.192318 ,6889.440511 ,6889.405499 ,6887.998607 ,6884.781348 ,6878.324223 ,6867.613759 ,6856.4061 ,6839.311141 ,6815.052286 ,6787.36283 ,6749.642707 ,6705.032256 ,6651.307709 ,6587.503849 ,6514.157827 ,6428.548289 ,6329.616862 ,6218.291204 ,6095.576559 ,5958.323724 ,5805.877235 ,5640.041907 ,5462.229956 ,5268.970167 ,5062.54793 ,4845.346957 ,4615.916441 ,4378.223414 ,4131.746221 ,3881.210654 ,3627.61787 ,3374.242543 ,3128.771003 ,2894.715113 ,2677.853454 ,2485.805118 ,2332.694553 ,2222.416344 ,2161.547736 ,2160.671752 ,2211.01825 ,2305.019035 ,2437.99708 ,2597.97104 ,2773.705605 ,2956.651282 ,3141.91922 ,3323.071447 ,3493.63857 ,3651.945511 ,3794.587033 ,3919.464659 ,4022.479632 ,4105.210763 ,4165.402198 ,4201.746316 ,4214.537485 ,4204.973419 ,4168.404711 ,4110.31714 ,4031.231833 ,3927.672717 ,3804.541018 ,3664.494135 ,3508.260262 ,3335.859315 ,3157.994752 ,2973.876198 ,2789.878416 ,2613.197831 ,2452.512545 ,2318.618639 ,2218.78121 ,2162.235847 ,2162.699899 ,2218.869896 ,2321.043301 ,2472.636926 ,2661.649741 ,2875.59107 ,3107.889914 ,3353.575277 ,3605.036527 ,3857.94862 ,4109.598857 ,4355.711733 ,4593.666581 ,4823.05181 ,5042.191374 ,5249.280215 ,5443.737209 ,5625.202126 ,5793.190111 ,5947.854834 ,6089.158362 ,6219.625272 ,6335.047455 ,6438.63187 ,6533.025966 ,6616.366021 ,6691.662861 ,6758.778134 ,6819.995415 ,6876.641203 ,6931.191632 ,6985.979958 ,7040.354771 ,7098.383781 ,7159.86104 ,7228.813203 ,7304.491154 ,7387.861973 ,7483.624846 ,7590.806433 ,7709.185078 ,7837.707518 ,7983.492327 ,8138.837341 ,8307.203223 ,8486.766247 ,8677.14563 ,8877.062762 ,9085.555563 ,9302.852056 ,9523.549814 ,9749.359592 ,9979.399593 ,10210.20002 ,10440.87354 ,10670.97538 ,10898.10667 ,11121.16353 ,11337.89652 ,11547.28421 ,11749.94534 ,11942.13373 ,12124.37713 ,12296.16708 ,12455.15237 ,12601.35376 ,12735.60188 ,12855.55261 ,12963.26745 ,13057.11657 ,13137.5533 ,13204.45968 ,13260.83889 ,13305.5549 ,13337.01415 ,13361.11966 ,13376.71983 ,13383.72517 ,13385.02954 ,13382.99739 ,13375.90397 ,13367.23356 ,13355.84562 ,13345.2378 ,13334.94369 ,13325.89278 ,13317.90821 ,13312.91921 ,13311.63886 ,13312.03189 ,13315.34572 ,13320.26989 ,13328.36289 ,13336.56529 ,13344.92573 ,13352.15999 ,13358.02404 ,13362.35236 ,13361.92301 ,13356.66698 ,13344.65585 ,13324.2469 ,13295.28824 ,13256.38872 ,13203.64341 ,13138.95651 ,13059.48078 ,12966.01221 ,12858.18869 ,12729.20128 ,12585.13637 ,12422.30515 ,12241.36917 ,12040.39086 ,11822.72419 ,11588.00662 ,11331.82824 ,11059.82223 ,10769.89713 ,10462.0104 ,10140.10121 ,9803.006751 ,9450.612765 ,9087.07819 ,8711.66263 ,8325.689904 ,7932.058598 ,7530.675695 ,7124.711423 ,6713.537591 ,6301.809669 ,5890.617836 ,5482.087338 ,5080.987098 ,4691.830632 ,4318.975096 ,3967.3124 ,3645.544252 ,3366.394798 ,3133.975464 ,2960.67359 ,2861.479041 ,2833.2026 ,2875.467821 ,2987.459979 ,3151.124506 ,3352.934972 ,3587.426728 ,3840.152691 ,4101.986883 ,4368.431708 ,4633.223979 ,4893.420945 ,5144.194876 ,5383.997561 ,5611.231522 ,5825.211719 ,6022.542953 ,6205.937241 ,6372.134104 ,6523.822528 ,6659.290383 ,6780.471278 ,6886.369873 ,6977.573824 ,7058.510486 ,7127.037703 ,7184.188096 ,7233.250376 ,7275.320793 ,7310.714749 ,7341.914808 ,7371.715245 ,7399.767697 ,7428.293264 ,7458.428346 ,7490.978863 ,7526.313329 ,7566.266516 ,7608.880713 ,7656.075894 ,7705.957429 ,7759.19481 ,7815.141619 ,7870.942404 ,7926.941912 ,7982.41706 ,8036.441585 ,8085.521296 ,8131.039173 ,8170.032683 ,8202.972552 ,8230.156351 ,8248.181615 ,8258.503817 ,8260.660978 ,8254.891058 ,8242.679013 ,8221.155616 ,8193.688784 ,8161.747224 ,8123.104138 ,8081.553227 ,8037.960991 ,7993.02182 ,7948.112372 ,7904.449546 ,7864.175543 ,7828.336119 ,7799.385613 ,7776.290288 ,7760.461682 ,7754.291344 ,7754.418534 ,7762.792262 ,7780.21655 ,7805.186607 ,7835.335683 ,7870.282469 ,7911.034334 ,7954.46822 ,7999.115208 ,8044.103109 ,8087.894241 ,8128.643894 ,8164.134299 ,8197.423434 ,8222.442622 ,8241.54863 ,8253.030178 ,8257.027949 ,8252.457723 ,8239.559791 ,8219.013377 ,8191.038527 ,8157.393363 ,8116.461405 ,8070.852094 ,8020.136442 ,7967.35881 ,7913.765505 ,7861.246201 ,7809.635307 ,7763.22194 ,7723.716692 ,7691.047419 ,7668.968363 ,7659.181788 ,7662.662258 ,7680.553026 ,7715.048423 ,7766.094166 ,7833.090454 ,7920.665423 ,8024.301625 ,8143.753648 ,8282.619605 ,8436.572647 ,8603.900752 ,8785.490837 ,8979.602773 ,9180.597417 ,9391.69251 ,9610.705919 ,9833.699687 ,10059.54126 ,10285.19016 ,10511.4964 ,10734.50919 ,10954.22202 ,11166.44243 ,11373.4193 ,11568.55773 ,11753.2398 ,11926.65457 ,12084.92474 ,12228.08368 ,12354.0606 ,12462.45528 ,12550.44078 ,12620.26102 ,12668.4608 ,12693.20833 ,12696.84166 ,12678.01756 ,12634.40053 ,12566.63855 ,12476.19431 ,12360.92131 ,12222.48577 ,12063.2746 ,11879.01487 ,11671.79541 ,11447.05489 ,11199.88195 ,10934.60559 ,10650.40784 ,10349.57374 ,10029.76273 ,9697.30908 ,9351.486273 ,8990.204195 ,8618.665781 ,8237.026697 ,7847.677123 ,7450.621682 ,7048.98301 ,6643.589778 ,6237.966604 ,5832.231037 ,5430.595232 ,5039.691463 ,4658.847046 ,4295.589343 ,3957.079064 ,3646.047079 ,3372.109864 ,3147.797335 ,2975.242929 ,2857.702681 ,2809.947401 ,2820.770064 ,2880.718886 ,2988.905994 ,3128.22685 ,3287.836195 ,3459.14377 ,3635.173277 ,3806.541001 ,3970.257954 ,4119.803334 ,4254.662187 ,4371.159098 ,4463.447932 ,4534.958194 ,4584.322276 ,4608.189243 ,4606.326953 ,4580.527604 ,4529.651358 ,4453.830195 ,4357.169129 ,4236.88931 ,4097.400488 ,3941.067889 ,3770.492371 ,3589.098837 ,3401.782053 ,3214.847192 ,3039.140968 ,2879.7441 ,2749.226955 ,2662.38811 ,2629.138231 ,2655.009317 ,2751.032741 ,2913.852649 ,3128.082837 ,3394.748159 ,3704.942253 ,4043.458309 ,4406.794456 ,4788.343656 ,5181.868292 ,5585.264884 ,5993.793435 ,6404.164653 ,6815.642065 ,7223.648853 ,7626.583633 ,8023.177021 ,8412.32354 ,8792.236895 ,9161.548194 ,9517.202986 ,9858.924631 ,10187.20082 ,10500.14292 ,10793.03875 ,11069.06205 ,11325.57986 ,11559.57853 ,11774.05766 ,11966.74182 ,12135.82394 ,12279.47965 ,12401.55947 ,12498.35717 ,12571.641 ,12620.54651 ,12647.17289 ,12648.90831 ,12627.80549 ,12585.81574 ,12519.81389 ,12435.52086 ,12332.61256 ,12208.48707 ,12068.49521 ,11913.37809 ,11743.2295 ,11559.72371 ,11365.89716 ,11160.9194 ,10948.32387 ,10730.78208 ,10506.45973 ,10279.32338 ,10053.93763 ,9827.32935 ,9604.723403 ,9386.262328 ,9175.503631 ,8974.212006 ,8780.940431 ,8599.758671 ,8433.126333 ,8279.945172 ,8141.839429 ,8022.540025 ,7919.932472 ,7833.340593 ,7765.645222 ,7715.654608 ,7681.703409 ,7663.329887 ,7661.088092 ,7671.158031 ,7693.85341 ,7725.805082 ,7765.245127 ,7810.248744 ,7861.076137 ,7914.009935 ,7965.010126 ,8018.32597 ,8067.430023 ,8113.087591 ,8152.300521 ,8186.256795 ,8213.409083 ,8231.80927 ,8241.863792 ,8242.939131 ,8236.158785 ,8220.010888 ,8194.470842 ,8161.299459 ,8120.486356 ,8072.354274 ,8017.706729 ,7957.055941 ,7892.71205 ,7824.545462 ,7752.321771 ,7680.612462 ,7607.188049 ,7535.955715 ,7465.683217 ,7396.850664 ,7333.030668 ,7272.654313 ,7217.641682 ,7166.16031 ,7121.892552 ,7080.86408 ,7046.240137 ,7015.056606 ,6990.052025 ,6968.62729 ,6950.471305 ,6936.358843 ,6923.419276 ,6913.782421 ,6906.685439 ,6901.255292 ,6896.622493 ,6891.256158 ,6888.244572 ,6884.832106 ,6881.366992 ,6877.815725 ,6875.026649 ,6870.260911 ,6866.724405 ,6862.868524 ,6858.374869 ,6854.958673 ,6851.503807 ,6847.726027 ,6844.440873 ,6841.224561 ,6839.549101 ,6836.993488 ,6835.942736 ,6835.129733 ,6834.844328 ,6835.527393 ,6836.586288 ,6838.570391 ,6841.350156 ,6844.069367 ,6847.089645 ,6850.086029 ,6853.513426 ,6857.51718 ,6860.296973 ,6862.564744 ,6864.392377 ,6867.627864 ,6870.188039 ,6872.6778 ,6874.0093 ,6875.7381 ,6877.508524 ,6879.845848 ,6881.769118 ,6884.799946 ,6890.122789 ,6896.395858 ,6904.562767 ,6914.68954 ,6928.86873 ,6946.363315 ,6969.209689 ,6997.428623 ,7032.967265 ,7074.941675 ,7125.785378 ,7186.454712 ,7255.543274 ,7338.083053 ,7430.559471 ,7534.950268 ,7653.136445 ,7783.335897 ,7924.825628 ,8079.558638 ,8246.768808 ,8422.912148 ,8609.564067 ,8807.894802 ,9011.688027 ,9222.515058 ,9440.99931 ,9662.767719 ,9887.719829 ,10115.22944 ,10343.49803 ,10569.97029 ,10792.80107 ,11014.25699 ,11230.12291 ,11439.61727 ,11640.71828 ,11834.80584 ,12019.57684 ,12193.97436 ,12358.00481 ,12512.20407 ,12653.0692 ,12782.56021 ,12900.53685 ,13003.612 ,13096.73781 ,13176.09313 ,13244.72838 ,13300.28506 ,13346.00848 ,13381.11251 ,13405.53031 ,13422.81094 ,13432.90952 ,13435.39808 ,13432.02443 ,13424.3183 ,13412.45947 ,13399.56377 ,13384.60679 ,13369.27109 ,13356.10269 ,13342.28044 ,13333.53799 ,13325.83951 ,13321.65206 ,13321.31756 ,13324.24892 ,13331.38702 ,13339.31614 ,13354.26018 ,13368.19239 ,13383.5088 ,13400.12004 ,13417.40349 ,13433.73559 ,13448.22418 ,13458.47868 ,13466.25176 ,13467.98785 ,13465.13039 ,13456.26951 ,13440.04069 ,13416.17918 ,13384.67334 ,13346.62919 ,13299.99804 ,13245.3939 ,13186.3756 ,13118.57245 ,13043.59078 ,12965.56306 ,12881.24348 ,12794.8389 ,12704.05133 ,12611.90816 ,12519.27718 ,12426.54755 ,12334.27658 ,12245.11775 ,12158.11672 ,12074.19662 ,11995.11492 ,11918.70647 ,11847.40471 ,11781.48386 ,11720.95233 ,11662.46656 ,11611.11902 ,11563.11162 ,11517.79367 ,11476.43079 ,11435.42766 ,11395.61928 ,11354.03056 ,11311.42163 ,11266.54708 ,11215.7373 ,11161.15435 ,11099.57385 ,11031.66732 ,10951.98429 ,10864.09009 ,10765.70857 ,10654.90726 ,10531.50678 ,10394.83428 ,10244.45917 ,10079.30871 ,9900.628577 ,9708.663787 ,9499.813485 ,9278.403811 ,9042.403164 ,8792.31965 ,8529.009205 ,8252.457488 ,7964.238051 ,7664.036864 ,7353.271784 ,7034.385509 ,6705.557273 ,6370.12162 ,6030.502608 ,5688.500227 ,5345.31715 ,5005.839861 ,4675.07939 ,4352.742304 ,4047.299448 ,3764.788376 ,3510.979025 ,3293.336978 ,3121.027907 ,2999.735731 ,2929.035808 ,2915.527641 ,2953.527711 ,3032.047429 ,3148.585311 ,3289.437407 ,3444.873964 ,3609.143616 ,3774.608891 ,3934.29329 ,4084.616158 ,4220.018824 ,4339.814065 ,4441.330526 ,4521.085348 ,4578.227375 ,4613.262066 ,4625.292896 ,4610.052008 ,4575.218857 ,4515.540193 ,4431.095229 ,4329.204268 ,4207.232544 ,4068.86388 ,3917.414948 ,3757.236952 ,3590.978549 ,3427.242181 ,3271.549035 ,3134.309707 ,3022.272926 ,2943.550649 ,2914.297823 ,2935.391549 ,3007.212457 ,3134.734201 ,3315.522561 ,3534.650079 ,3790.711475 ,4078.76617 ,4386.123731 ,4710.169947 ,5043.375863 ,5383.921466 ,5727.804923 ,6071.342038 ,6409.701165 ,6746.152343 ,7072.986619 ,7390.470601 ,7700.150408 ,7998.633043 ,8284.574641 ,8559.745378 ,8821.401022 ,9068.137699 ,9302.294717 ,9521.966085 ,9727.429048 ,9917.563972 ,10095.28701 ,10258.77298 ,10407.12974 ,10543.70187 ,10667.36381 ,10777.69949 ,10875.79463 ,10963.45367 ,11041.79851 ,11110.70405 ,11171.44999 ,11225.18593 ,11274.07607 ,11318.71708 ,11359.77545 ,11400.1546 ,11440.18165 ,11480.48636 ,11521.77072 ,11566.21276 ,11613.30995 ,11665.44378 ,11722.7313 ,11784.48716 ,11851.21535 ,11923.69402 ,11999.34399 ,12082.32148 ,12166.76912 ,12256.91592 ,12346.92923 ,12439.87821 ,12534.12012 ,12627.84701 ,12719.4009 ,12808.09725 ,12894.41546 ,12976.62954 ,13053.64594 ,13127.27485 ,13192.5198 ,13252.18357 ,13302.64673 ,13348.33583 ,13384.80236 ,13415.86186 ,13436.75904 ,13451.75413 ,13461.72814 ,13463.98194 ,13462.81425 ,13455.83501 ,13446.6911 ,13432.59284 ,13419.05879 ,13401.11983 ,13384.96672 ,13369.40597 ,13354.86685 ,13341.2537 ,13330.31245 ,13323.03996 ,13318.11755 ,13317.27049 ,13318.37634 ,13323.96896 ,13331.05757 ,13340.29077 ,13350.7776 ,13360.36066 ,13371.14526 ,13379.332 ,13384.8366 ,13386.30511 ,13383.06659 ,13372.86258 ,13355.47564 ,13329.23244 ,13291.00279 ,13242.47639 ,13183.91772 ,13111.92277 ,13025.75099 ,12928.03996 ,12816.29457 ,12691.1418 ,12553.72757 ,12401.77995 ,12239.25226 ,12063.53248 ,11879.37056 ,11682.13606 ,11477.84628 ,11266.25511 ,11047.34933 ,10824.16653 ,10595.85419 ,10366.10677 ,10136.66984 ,9908.298153 ,9682.653711 ,9460.757654 ,9243.003967 ,9033.202264 ,8832.022351 ,8642.284816 ,8463.166802 ,8296.026306 ,8142.592252 ,8003.353688 ,7878.037935 ,7767.661728 ,7672.031069 ,7589.446603 ,7520.557141 ,7462.486262 ,7416.693634 ,7378.920731 ,7348.61479 ,7322.416533 ,7299.73797 ,7278.62663 ,7255.547461 ,7229.780831 ,7200.237437 ,7162.926168 ,7116.461865 ,7060.333303 ,6992.753659 ,6911.778145 ,6817.564594 ,6709.174412 ,6583.879388 ,6444.704014 ,6290.625507 ,6121.33779 ,5936.561403 ,5737.42741 ,5526.273747 ,5303.258011 ,5070.807731 ,4831.628438 ,4587.597186 ,4344.597638 ,4104.699663 ,3875.625911 ,3662.556406 ,3471.653487 ,3313.889391 ,3198.569903 ,3123.510512 ,3102.710671 ,3137.314163 ,3217.957491 ,3345.531673 ,3512.191512 ,3706.81139 ,3923.581847 ,4155.781594 ,4396.584731 ,4640.559556 ,4883.536155 ,5121.69683 ,5353.584673 ,5573.842371 ,5783.438225 ,5978.981185 ,6160.875049 ,6327.687235 ,6480.697969 ,6618.051884 ,6738.102824 ,6846.204628 ,6938.653708 ,7018.128419 ,7084.237154 ,7140.127444 ,7183.543919 ,7217.504502 ,7243.953572 ,7264.303716 ,7278.453031 ,7288.295045 ,7294.446846 ,7298.528921 ,7300.635399 ,7301.622364 ,7301.778328 ,7300.187153 ,7297.697804 ,7292.266187 ,7283.873343 ,7273.717231 ,7256.80591 ,7235.278604 ,7206.303062 ,7168.742414 ,7121.205711 ,7062.509569 ,6990.877959 ,6905.626077 ,6807.858709 ,6694.070071 ,6564.420481 ,6420.222264 ,6258.857032 ,6081.506625 ,5887.787442 ,5680.337259 ,5456.391993 ,5219.118193 ,4970.917216 ,4711.026468 ,4444.420476 ,4173.296432 ,3902.375306 ,3636.206952 ,3380.369934 ,3144.865236 ,2942.01903 ,2777.870692 ,2669.607065 ,2626.643199 ,2653.069665 ,2747.856227 ,2911.721753 ,3129.192689 ,3385.992379 ,3677.86444 ,3994.455663 ,4326.538157 ,4670.240575 ,5018.82604 ,5369.888299 ,5722.821753 ,6071.709693 ,6414.823106 ,6754.222157 ,7082.924332 ,7404.302634 ,7715.213222 ,8015.350301 ,8304.261 ,8581.312123 ,8845.40483 ,9096.211451 ,9333.491438 ,9557.760557 ,9766.390767 ,9963.923925 ,10145.27051 ,10315.0822 ,10471.0907 ,10614.40675 ,10747.0859 ,10866.9085 ,10979.10832 ,11080.82332 ,11175.56917 ,11264.1112 ,11343.07983 ,11419.07386 ,11491.60357 ,11560.45747 ,11629.442 ,11694.66028 ,11760.85944 ,11827.75095 ,11895.41861 ,11965.07377 ,12033.19834 ,12105.62244 ,12179.11167 ,12254.85863 ,12331.10845 ,12408.7758 ,12484.90459 ,12562.96577 ,12640.07984 ,12715.70168 ,12788.85564 ,12858.59359 ,12923.48791 ,12982.7567 ,13033.9365 ,13079.28867 ,13112.868 ,13137.05977 ,13149.71942 ,13148.54886 ,13134.38045 ,13105.54582 ,13058.39337 ,12996.01854 ,12915.66683 ,12816.08335 ,12697.83334 ,12561.33577 ,12404.85592 ,12227.74081 ,12032.60427 ,11818.20547 ,11584.43037 ,11332.71909 ,11065.24308 ,10777.5833 ,10474.21674 ,10156.97678 ,9821.801512 ,9474.306718 ,9112.93174 ,8740.100752 ,8355.878509 ,7961.676046 ,7559.162119 ,7149.362489 ,6734.026543 ,6313.426584 ,5890.778876 ,5468.095683 ,5049.249786 ,4636.09082 ,4235.050801 ,3848.910414 ,3490.193467 ,3167.345999 ,2886.434429 ,2672.001779 ,2538.209941 ,2489.709124 ,2535.225107 ,2671.370317 ,2876.291755 ,3135.776497 ,3439.657206 ,3767.578289 ,4113.965395 ,4471.530369 ,4833.6111 ,5198.954637 ,5561.242564 ,5919.985013 ,6273.014992 ,6617.692916 ,6953.397073 ,7280.578483 ,7593.500148 ,7894.111497 ,8183.206743 ,8459.136604 ,8718.456943 ,8963.337409 ,9194.007512 ,9406.196845 ,9601.096131 ,9781.285543 ,9941.727992 ,10084.98006 ,10209.55885 ,10317.3634 ,10403.58854 ,10473.76767 ,10526.228 ,10557.15885 ,10571.35341 ,10568.39519 ,10547.79851 ,10510.14385 ,10457.17596 ,10389.13752 ,10305.06214 ,10209.4572 ,10101.68589 ,9982.804176 ,9854.56104 ,9719.341696 ,9575.383466 ,9428.301978 ,9276.453296 ,9122.126895 ,8966.653705 ,8811.521599 ,8659.404638 ,8510.426474 ,8366.022937 ,8227.612678 ,8096.949303 ,7975.330532 ,7864.216348 ,7764.674886 ,7675.165438 ,7600.05069 ,7540.473534 ,7493.50542 ,7462.418998 ,7448.67999 ,7450.411577 ,7468.210195 ,7501.864588 ,7551.709639 ,7614.989711 ,7695.045278 ,7786.838058 ,7890.621886 ,8007.166849 ,8131.485462 ,8265.307113 ,8405.719205 ,8552.922283 ,8703.636302 ,8858.048925 ,9013.930833 ,9170.205142 ,9325.849822 ,9479.421313 ,9630.912024 ,9778.924907 ,9922.873758 ,10060.31057 ,10192.80123 ,10319.9169 ,10439.88623 ,10554.163 ,10659.6517 ,10758.59275 ,10852.78767 ,10938.45457 ,11016.52284 ,11089.41977 ,11156.48894 ,11214.46544 ,11268.19359 ,11316.71129 ,11359.39797 ,11396.45038 ,11428.94556 ,11458.32106 ,11482.53486 ,11503.18091 ,11521.3116 ,11534.68284 ,11548.35049 ,11558.50636 ,11566.1048 ,11571.47524 ,11577.26616 ,11581.37132 ,11584.01851 ,11586.92386 ,11587.86051 ,11589.68121 ,11590.7646 ,11590.79312 ,11590.9645 ,11590.07384 ,11589.57764 ,11588.7535 ,11587.22852 ,11584.04269 ,11579.89312 ,11576.31329 ,11569.96416 ,11562.2525 ,11552.15461 ,11540.18618 ,11526.87198 ,11509.43373 ,11488.73658 ,11465.12965 ,11437.35212 ,11406.37917 ,11370.43129 ,11330.93326 ,11285.73822 ,11233.74673 ,11176.72199 ,11113.93543 ,11043.10395 ,10966.52379 ,10884.13122 ,10795.2692 ,10697.5896 ,10594.11351 ,10482.25786 ,10365.65984 ,10241.07136 ,10109.2136 ,9973.54918 ,9832.311303 ,9686.761475 ,9536.860157 ,9384.376039 ,9230.404731 ,9075.494707 ,8921.237651 ,8767.17105 ,8615.890162 ,8467.800992 ,8323.989582 ,8186.038853 ,8054.129823 ,7928.066481 ,7808.626111 ,7697.886764 ,7593.351773 ,7496.967125 ,7407.049613 ,7322.96792 ,7243.466407 ,7168.49258 ,7094.970157 ,7023.199136 ,6950.194206 ,6875.270625 ,6797.223474 ,6714.210186 ,6624.303169 ,6526.629867 ,6420.084769 ,6303.237253 ,6175.059355 ,6036.550749 ,5882.356919 ,5716.64889 ,5538.032221 ,5343.989596 ,5136.948518 ,4916.75469 ,4684.277478 ,4440.29116 ,4186.787752 ,3925.46448 ,3658.710584 ,3392.399638 ,3127.746616 ,2874.902906 ,2647.074447 ,2448.653742 ,2303.310934 ,2226.455151 ,2224.676059 ,2307.498027 ,2474.352147 ,2708.188489 ,2990.822369 ,3321.368879 ,3681.825128 ,4063.186363 ,4462.190043 ,4871.32361 ,5286.997991 ,5707.531776 ,6129.324552 ,6550.661903 ,6968.954337 ,7382.710966 ,7789.430619 ,8190.103567 ,8580.338775 ,8961.153623 ,9330.33708 ,9687.252573 ,10026.8242 ,10355.41431 ,10667.00591 ,10961.06147 ,11241.40554 ,11501.47044 ,11743.74871 ,11967.99788 ,12173.70089 ,12361.30076 ,12532.36164 ,12683.22033 ,12815.70207 ,12931.5735 ,13033.11334 ,13115.87998 ,13185.87279 ,13243.59188 ,13287.00354 ,13318.89978 ,13341.63485 ,13355.3913 ,13362.61668 ,13364.29938 ,13360.42723 ,13353.89844 ,13344.12803 ,13336.86249 ,13329.17644 ,13321.18305 ,13315.17021 ,13313.4022 ,13315.26513 ,13318.30211 ,13323.79823 ,13333.54572 ,13345.69897 ,13361.10735 ,13377.30423 ,13393.83467 ,13411.61055 ,13428.68774 ,13443.61773 ,13455.70252 ,13463.1707 ,13467.71712 ,13466.63682 ,13459.52442 ,13445.18847 ,13424.13557 ,13394.01118 ,13359.15014 ,13315.89717 ,13265.28524 ,13206.77507 ,13142.26451 ,13070.77549 ,12992.69396 ,12911.3953 ,12826.01225 ,12736.16673 ,12645.31765 ,12553.14678 ,12458.50968 ,12368.22484 ,12277.40729 ,12190.28394 ,12105.14133 ,12023.82787 ,11946.93781 ,11875.44173 ,11811.41901 ,11749.72146 ,11696.29028 ,11647.94529 ,11604.55584 ,11568.77457 ,11536.45385 ,11509.36701 ,11487.90012 ,11470.96441 ,11457.38597 ,11446.20505 ,11440.43368 ,11435.45664 ,11433.70306 ,11436.54674 ,11439.51301 ,11446.82899 ,11457.825 ,11471.07825 ,11488.01474 ,11508.08463 ,11533.81563 ,11563.8233 ,11598.44036 ,11638.44299 ,11683.58525 ,11734.41446 ,11791.37485 ,11854.54445 ,11921.88515 ,11995.79296 ,12073.41282 ,12156.25135 ,12242.69705 ,12330.30865 ,12421.28969 ,12510.78213 ,12599.9851 ,12686.9811 ,12770.93172 ,12852.36847 ,12926.52775 ,12996.31786 ,13055.18607 ,13106.9577 ,13149.20018 ,13181.79917 ,13205.09238 ,13213.90493 ,13213.8769 ,13203.63564 ,13182.75092 ,13150.09057 ,13108.13062 ,13057.91882 ,12998.94978 ,12932.51962 ,12859.62255 ,12781.42636 ,12697.98319 ,12611.69832 ,12524.40926 ,12435.02543 ,12346.70562 ,12260.1378 ,12175.67044 ,12095.52264 ,12016.36341 ,11941.97468 ,11874.94532 ,11812.95798 ,11757.63852 ,11708.09445 ,11663.38042 ,11626.80118 ,11597.15045 ,11571.44086 ,11553.15954 ,11540.78339 ,11533.9025 ,11529.61734 ,11532.50589 ,11538.04081 ,11547.907 ,11562.20508 ,11579.70755 ,11600.81314 ,11626.32448 ,11654.8571 ,11687.35512 ,11723.88144 ,11763.25767 ,11804.76571 ,11851.52846 ,11904.22426 ,11958.08979 ,12015.46343 ,12077.94869 ,12143.98048 ,12213.27618 ,12286.09336 ,12361.23248 ,12439.67847 ,12518.67489 ,12598.57519 ,12679.83324 ,12761.07668 ,12841.40861 ,12919.44682 ,12994.46845 ,13066.06653 ,13133.69462 ,13197.80559 ,13254.46895 ,13304.49731 ,13346.42708 ,13379.21797 ,13404.70733 ,13423.28341 ,13431.73206 ,13429.79328 ,13420.75058 ,13400.70037 ,13373.24982 ,13338.96504 ,13296.08724 ,13245.51785 ,13189.44951 ,13127.16178 ,13058.48891 ,12988.69599 ,12913.44613 ,12834.5482 ,12755.79803 ,12675.88499 ,12595.20227 ,12515.1597 ,12437.36367 ,12358.77463 ,12283.37674 ,12210.56941 ,12140.29425 ,12074.24243 ,12010.12398 ,11951.30414 ,11896.08989 ,11845.23544 ,11797.89098 ,11754.88438 ,11715.30471 ,11681.07862 ,11648.59159 ,11621.94151 ,11597.28569 ,11577.46331 ,11560.45097 ,11547.45094 ,11538.29922 ,11532.96563 ,11532.08188 ,11536.758 ,11544.26725 ,11559.21067 ,11577.57366 ,11601.37669 ,11632.51727 ,11668.64739 ,11711.36914 ,11759.66957 ,11813.67207 ,11875.58066 ,11942.83579 ,12014.15446 ,12090.30037 ,12168.74727 ,12252.42712 ,12335.93321 ,12420.00711 ,12503.73083 ,12585.50826 ,12660.58362 ,12733.09878 ,12797.86162 ,12856.09938 ,12901.96625 ,12936.12787 ,12958.38856 ,12965.7875 ,12957.16034 ,12930.90854 ,12887.79987 ,12826.31974 ,12744.27344 ,12641.60798 ,12518.96313 ,12377.69235 ,12213.01604 ,12029.81306 ,11827.27112 ,11602.73978 ,11358.88757 ,11096.54471 ,10816.00048 ,10518.12916 ,10203.12537 ,9873.075525 ,9527.233018 ,9168.803084 ,8798.76128 ,8414.575831 ,8022.358028 ,7621.055457 ,7210.318966 ,6794.295866 ,6374.983629 ,5953.908717 ,5531.460353 ,5112.455156 ,4698.037015 ,4292.986542 ,3900.298391 ,3525.916822 ,3178.871917 ,2866.615578 ,2599.842711 ,2396.400116 ,2262.613166 ,2209.519654 ,2248.186736 ,2356.790686 ,2524.980403 ,2737.489555 ,2978.103594 ,3234.775883 ,3501.887785 ,3769.887034 ,4034.594359 ,4295.01263 ,4545.27875 ,4784.638961 ,5010.375368 ,5224.566275 ,5425.538302 ,5612.359775 ,5784.497695 ,5943.540716 ,6089.938761 ,6222.282049 ,6342.621649 ,6452.359591 ,6552.36424 ,6642.123576 ,6725.452876 ,6802.987115 ,6875.3129 ,6946.296324 ,7015.740932 ,7085.672808 ,7158.369047 ,7234.254873 ,7316.760198 ,7405.754714 ,7505.168973 ,7614.24319 ,7731.797606 ,7860.001925 ,8001.015545 ,8154.683826 ,8317.303268 ,8492.767649 ,8677.737746 ,8871.340661 ,9072.472779 ,9281.574945 ,9497.125308 ,9715.115843 ,9938.75185 ,10163.87263 ,10388.64498 ,10613.55567 ,10835.64869 ,11054.53573 ,11266.96367 ,11473.61615 ,11674.27572 ,11864.4562 ,12045.91177 ,12217.9404 ,12376.07714 ,12522.573 ,12655.02395 ,12777.17002 ,12883.89409 ,12977.74649 ,13058.4549 ,13123.77488 ,13175.26263 ,13213.63134 ,13239.30751 ,13251.85748 ,13251.94406 ,13241.07192 ,13218.72501 ,13186.85382 ,13145.71754 ,13097.29133 ,13039.99433 ,12977.724 ,12909.43212 ,12835.25346 ,12759.44913 ,12680.55903 ,12599.37193 ,12518.98035 ,12438.33629 ,12357.98047 ,12279.27747 ,12201.43753 ,12126.80752 ,12054.70576 ,11982.79781 ,11915.1019 ,11847.60236 ,11782.57935 ,11718.15511 ,11654.59099 ,11590.97762 ,11526.39767 ,11460.10378 ,11391.53061 ,11321.80654 ,11243.40651 ,11163.30041 ,11077.08206 ,10984.20866 ,10884.43053 ,10780.29058 ,10668.79643 ,10549.41396 ,10422.19278 ,10291.29759 ,10152.91136 ,10009.20374 ,9860.205098 ,9707.776367 ,9551.360028 ,9393.180992 ,9232.067301 ,9072.717801 ,8913.438107 ,8756.596259 ,8602.725762 ,8452.943404 ,8310.525071 ,8174.252979 ,8045.31583 ,7927.4866 ,7819.758357 ,7723.457425 ,7640.823312 ,7572.192326 ,7516.396608 ,7476.367622 ,7453.771358 ,7446.155348 ,7455.190931 ,7480.347747 ,7521.260617 ,7576.134763 ,7648.990791 ,7733.910566 ,7830.814428 ,7940.921714 ,8060.125881 ,8189.122038 ,8326.872835 ,8469.754862 ,8618.557333 ,8771.181873 ,8925.506437 ,9080.417065 ,9235.135166 ,9388.474094 ,9539.444885 ,9685.900275 ,9828.078063 ,9966.143452 ,10098.34078 ,10222.57191 ,10338.89699 ,10446.90044 ,10547.22595 ,10638.07435 ,10721.32687 ,10792.54082 ,10852.66343 ,10904.39568 ,10942.49011 ,10969.67023 ,10985.53647 ,10989.49995 ,10980.40151 ,10960.19261 ,10928.04546 ,10882.46279 ,10823.60917 ,10754.0418 ,10672.77597 ,10581.05001 ,10478.62334 ,10364.2139 ,10242.1262 ,10112.24732 ,9975.169244 ,9830.290928 ,9681.316196 ,9527.002419 ,9369.549866 ,9210.042688 ,9049.687252 ,8889.672004 ,8732.850725 ,8578.378045 ,8428.120795 ,8284.04657 ,8146.138296 ,8018.005506 ,7896.498217 ,7785.318365 ,7687.44806 ,7602.666843 ,7529.323451 ,7471.433066 ,7430.50537 ,7403.863417 ,7394.91815 ,7404.545816 ,7430.613989 ,7474.956449 ,7536.874672 ,7616.978539 ,7713.999855 ,7827.244645 ,7957.40225 ,8102.337751 ,8260.956143 ,8432.622341 ,8615.931311 ,8808.85296 ,9012.343018 ,9223.341119 ,9439.511088 ,9660.025462 ,9883.969505 ,10110.45231 ,10336.80688 ,10560.77893 ,10782.24075 ,10999.15578 ,11213.24374 ,11419.63311 ,11619.10132 ,11808.67283 ,11989.00345 ,12160.12123 ,12317.18231 ,12462.59246 ,12595.85662 ,12714.06779 ,12818.47999 ,12908.67548 ,12983.60903 ,13042.27543 ,13086.77893 ,13116.20954 ,13129.35079 ,13129.73643 ,13116.68654 ,13090.31411 ,13053.22434 ,13003.38697 ,12943.97004 ,12877.00544 ,12802.29395 ,12723.58278 ,12639.41882 ,12552.79998 ,12464.24812 ,12375.53178 ,12287.1782 ,12201.90877 ,12118.13931 ,12040.04415 ,11964.75952 ,11895.25387 ,11832.34778 ,11774.99446 ,11725.83317 ,11680.25418 ,11640.9957 ,11609.62186 ,11583.62733 ,11563.09311 ,11548.87067 ,11538.57031 ,11532.49345 ,11533.50651 ,11537.79481 ,11544.72039 ,11557.10942 ,11573.23909 ,11591.58252 ,11614.26303 ,11642.34005 ,11671.76556 ,11705.77187 ,11743.97145 ,11785.7613 ,11831.83607 ,11881.22907 ,11934.2486 ,11993.53917 ,12055.2404 ,12120.51032 ,12187.97646 ,12258.40764 ,12332.3691 ,12405.95458 ,12482.39794 ,12558.28683 ,12634.40511 ,12707.09485 ,12778.86131 ,12846.71461 ,12912.40525 ,12969.32131 ,13020.26568 ,13064.07461 ,13096.95314 ,13118.1115 ,13128.7591 ,13127.81981 ,13109.27944 ,13077.98143 ,13028.83097 ,12961.37821 ,12878.83932 ,12774.4527 ,12650.75849 ,12507.94349 ,12346.93677 ,12163.89717 ,11960.36976 ,11740.54134 ,11498.95243 ,11239.14794 ,10963.41118 ,10667.90369 ,10356.95106 ,10030.99098 ,9689.784419 ,9332.614235 ,8964.585755 ,8586.145789 ,8195.021551 ,7794.057606 ,7386.975016 ,6973.4901 ,6553.14901 ,6132.561638 ,5710.342419 ,5289.820194 ,4874.290248 ,4471.530103 ,4085.224482 ,3720.488492 ,3391.840107 ,3112.975419 ,2895.155472 ,2754.307714 ,2713.012811 ,2764.733773 ,2903.316485 ,3129.035861 ,3414.11318 ,3744.02186 ,4108.46458 ,4497.098038 ,4900.597023 ,5314.630089 ,5734.915736 ,6156.646677 ,6577.688133 ,6995.892655 ,7411.129486 ,7817.986898 ,8215.791147 ,8606.599866 ,8986.453105 ,9353.123677 ,9706.825961 ,10047.38826 ,10372.97243 ,10682.97332 ,10978.02855 ,11256.33634 ,11515.0732 ,11760.28 ,11987.75054 ,12193.64985 ,12385.60183 ,12559.75759 ,12712.74205 ,12851.60726 ,12974.07011 ,13079.42532 ,13170.5727 ,13247.41733 ,13310.86501 ,13360.56823 ,13400.59937 ,13429.97778 ,13451.18495 ,13464.39352 ,13470.40558 ,13473.0395 ,13472.60454 ,13467.98232 ,13463.23839 ,13457.72616 ,13452.03475 ,13448.53168 ,13445.38921 ,13444.28638 ,13444.68868 ,13448.17092 ,13451.75383 ,13456.8255 ,13464.04184 ,13470.79942 ,13478.46801 ,13484.32604 ,13486.98688 ,13486.5351 ,13480.93131 ,13471.51444 ,13455.29742 ,13430.41203 ,13396.95018 ,13354.58306 ,13300.56562 ,13236.60196 ,13160.53878 ,13072.2511 ,12971.37887 ,12859.4114 ,12732.95862 ,12593.18318 ,12441.98206 ,12278.38229 ,12102.75339 ,11917.84752 ,11720.29983 ,11515.17265 ,11303.43514 ,11083.8287 ,10859.77906 ,10631.23365 ,10397.82003 ,10166.32353 ,9933.71613 ,9702.720851 ,9475.459568 ,9253.621907 ,9038.851657 ,8831.135983 ,8633.596309 ,8444.696189 ,8267.395592 ,8102.612516 ,7950.830435 ,7808.975494 ,7682.591669 ,7567.60901 ,7464.247101 ,7370.632355 ,7288.625366 ,7215.029843 ,7149.566856 ,7087.389964 ,7031.781906 ,6976.131695 ,6920.970762 ,6866.16223 ,6807.446838 ,6743.697259 ,6672.910575 ,6596.471206 ,6510.22388 ,6413.046553 ,6305.527848 ,6185.203892 ,6052.561213 ,5907.173015 ,5746.679024 ,5572.173695 ,5383.870749 ,5183.080051 ,4966.520059 ,4737.782153 ,4497.858067 ,4246.533508 ,3985.376299 ,3721.294367 ,3454.246987 ,3188.819388 ,2932.184943 ,2695.935791 ,2493.163254 ,2331.303353 ,2234.717313 ,2218.346757 ,2282.529875 ,2427.07918 ,2646.74168 ,2922.542417 ,3239.557825 ,3595.077566 ,3973.484139 ,4366.898675 ,4773.531642 ,5189.194323 ,5607.862334 ,6029.650141 ,6449.84654 ,6868.385219 ,7282.578748 ,7690.812648 ,8090.290829 ,8481.852089 ,8861.509466 ,9230.017212 ,9586.732032 ,9925.81218 ,10249.13258 ,10556.30327 ,10843.24724 ,11111.95645 ,11362.24226 ,11590.01145 ,11795.10442 ,11977.49246 ,12136.25192 ,12270.11849 ,12379.33199 ,12464.48217 ,12521.429 ,12553.12909 ,12559.20509 ,12537.47379 ,12491.71217 ,12419.87741 ,12321.45936 ,12195.8187 ,12048.49139 ,11876.15419 ,11677.80385 ,11459.9328 ,11219.61335 ,10959.00768 ,10678.9725 ,10381.34254 ,10064.84226 ,9732.037646 ,9386.710513 ,9025.352123 ,8652.354904 ,8268.080317 ,7873.546937 ,7469.905655 ,7059.024363 ,6644.368155 ,6224.378188 ,5802.865406 ,5381.885517 ,4965.197616 ,4554.356387 ,4156.02247 ,3777.442721 ,3423.242993 ,3105.705046 ,2839.71709 ,2641.641614 ,2515.977173 ,2491.667632 ,2560.235303 ,2705.484685 ,2925.945469 ,3197.340413 ,3504.393808 ,3836.980582 ,4186.98875 ,4545.884969 ,4909.321696 ,5273.844223 ,5635.937762 ,5995.129449 ,6346.779741 ,6691.959134 ,7027.332194 ,7353.407375 ,7668.783981 ,7972.591235 ,8265.405409 ,8544.277547 ,8811.386362 ,9062.838331 ,9300.683479 ,9525.414264 ,9737.73743 ,9935.056894 ,10118.79105 ,10289.72418 ,10447.4004 ,10593.23625 ,10727.40456 ,10850.48401 ,10963.60204 ,11068.3628 ,11163.55803 ,11252.76565 ,11334.45599 ,11413.05872 ,11486.39921 ,11557.17617 ,11623.72101 ,11690.28909 ,11755.07223 ,11821.9595 ,11889.05497 ,11956.01104 ,12026.21222 ,12098.27005 ,12171.13991 ,12247.07462 ,12325.57183 ,12406.22336 ,12489.24833 ,12572.50758 ,12656.818 ,12739.20707 ,12822.94031 ,12905.03027 ,12984.57316 ,13062.56165 ,13136.50401 ,13208.15556 ,13271.97638 ,13333.556 ,13387.44759 ,13434.25004 ,13477.82855 ,13513.5793 ,13540.78754 ,13563.05834 ,13579.37612 ,13588.03268 ,13593.36591 ,13593.82989 ,13589.65013 ,13582.41505 ,13570.97788 ,13559.32015 ,13545.92415 ,13532.41271 ,13518.15279 ,13503.94085 ,13491.58098 ,13482.14258 ,13474.1785 ,13469.96763 ,13468.74964 ,13469.53117 ,13471.50448 ,13476.51254 ,13484.53292 ,13495.70152 ,13507.22725 ,13520.60436 ,13534.32717 ,13547.48291 ,13559.41747 ,13571.4491 ,13581.9107 ,13589.20286 ,13593.46998 ,13592.47941 ,13586.30532 ,13576.80497 ,13560.76719 ,13537.39799 ,13510.40465 ,13476.49982 ,13434.34132 ,13389.17341 ,13336.17086 ,13277.31238 ,13213.69288 ,13143.00162 ,13069.63219 ,12993.61719 ,12913.46674 ,12831.48915 ,12747.15456 ,12663.16917 ,12579.18351 ,12495.27557 ,12413.32139 ,12332.80877 ,12255.32654 ,12179.42448 ,12107.57985 ,12038.28089 ,11970.98441 ,11907.73603 ,11847.82604 ,11788.7657 ,11734.11924 ,11681.15815 ,11629.13016 ,11578.99178 ,11528.15591 ,11477.09722 ,11427.43672 ,11377.88094 ,11328.59095 ,11276.42328 ,11224.06958 ,11172.56961 ,11120.89131 ,11069.42403 ,11018.89201 ,10968.75185 ,10919.48334 ,10874.57499 ,10830.96232 ,10791.93109 ,10758.16639 ,10727.89808 ,10703.36109 ,10684.31538 ,10672.44283 ,10667.74802 ,10668.3887 ,10676.53174 ,10689.9672 ,10709.44638 ,10735.88053 ,10768.70442 ,10803.55721 ,10841.51911 ,10884.67373 ,10930.28676 ,10978.44821 ,11026.8801 ,11077.25854 ,11128.10879 ,11179.90695 ,11230.93514 ,11280.97552 ,11332.11567 ,11382.2571 ,11432.64997 ,11483.03396 ,11533.66065 ,11584.37013 ,11636.86652 ,11689.72505 ,11745.70581 ,11802.79629 ,11861.31433 ,11923.24934 ,11987.92337 ,12053.7145 ,12123.22743 ,12195.77033 ,12271.41093 ,12347.90754 ,12426.81134 ,12508.5261 ,12592.38987 ,12675.65247 ,12758.90036 ,12842.10026 ,12922.53462 ,13002.58162 ,13077.42269 ,13150.55254 ,13219.84231 ,13283.92735 ,13344.00526 ,13397.76707 ,13445.97159 ,13485.91198 ,13519.98631 ,13547.43202 ,13569.45195 ,13583.59564 ,13592.80889 ,13596.69984 ,13594.07456 ,13590.20097 ,13580.93664 ,13570.19152 ,13556.71851 ,13541.8742 ,13528.24311 ,13513.79283 ,13501.50756 ,13489.40521 ,13479.92015 ,13473.58539 ,13468.26809 ,13467.54565 ,13469.83942 ,13475.97364 ,13484.02143 ,13492.97615 ,13506.62621 ,13522.90868 ,13538.94514 ,13555.98231 ,13574.37174 ,13592.91215 ,13609.80042 ,13626.27353 ,13642.20777 ,13655.33991 ,13666.95469 ,13673.30763 ,13679.33216 ,13680.58195 ,13679.30351 ,13674.37493 ,13667.93738 ,13656.28173 ,13643.3287 ,13628.9804 ,13612.79918 ,13595.89335 ,13577.42193 ,13557.72812 ,13540.86802 ,13523.98989 ,13509.32074 ,13495.37789 ,13483.49426 ,13475.69503 ,13469.96831 ,13468.37274 ,13470.47946 ,13475.84305 ,13484.66668 ,13496.80292 ,13510.41385 ,13527.0762 ,13544.08352 ,13562.15957 ,13581.69325 ,13601.79218 ,13619.85835 ,13636.97098 ,13651.90429 ,13666.32109 ,13676.73636 ,13682.81197 ,13685.82928 ,13685.98636 ,13682.86078 ,13674.98465 ,13664.80747 ,13651.24076 ,13634.44735 ,13616.93167 ,13599.05557 ,13579.8801 ,13560.88997 ,13540.98763 ,13523.11538 ,13506.65022 ,13491.20796 ,13478.57891 ,13469.80032 ,13461.27082 ,13456.73158 ,13454.92119 ,13455.73511 ,13457.89663 ,13460.40268 ,13463.65972 ,13465.80168 ,13468.5178 ,13468.56042 ,13463.76791 ,13456.5106 ,13442.69614 ,13423.27391 ,13393.61249 ,13355.91344 ,13308.23731 ,13249.70793 ,13175.68384 ,13088.26524 ,12987.12737 ,12869.5289 ,12734.93519 ,12583.95193 ,12414.13613 ,12224.0303 ,12019.27177 ,11796.67912 ,11552.26512 ,11293.33556 ,11015.38606 ,10719.06742 ,10407.99768 ,10080.9666 ,9739.340582 ,9383.397412 ,9016.597411 ,8636.818877 ,8248.189859 ,7850.935918 ,7443.99961 ,7031.426185 ,6614.299415 ,6193.042699 ,5772.501813 ,5351.905193 ,4934.836749 ,4524.616781 ,4124.813112 ,3739.040611 ,3375.502562 ,3042.790868 ,2747.87363 ,2505.615475 ,2332.316574 ,2230.476137 ,2217.539593 ,2290.541954 ,2423.792371 ,2612.56947 ,2838.968627 ,3087.730705 ,3348.752793 ,3617.050538 ,3884.500101 ,4147.117349 ,4401.759131 ,4646.417864 ,4881.485931 ,5104.249866 ,5312.463544 ,5508.138722 ,5689.665258 ,5855.382898 ,6009.821225 ,6149.909394 ,6276.709758 ,6392.11969 ,6494.062741 ,6585.516292 ,6667.254546 ,6739.537137 ,6804.167858 ,6860.233068 ,6909.82563 ,6954.640386 ,6995.140538 ,7031.714692 ,7063.234798 ,7092.47487 ,7118.540825 ,7140.651875 ,7161.418626 ,7177.188681 ,7188.426166 ,7194.649429 ,7194.640017 ,7187.037413 ,7171.6007 ,7147.428282 ,7112.922514 ,7066.198055 ,7005.234303 ,6933.704408 ,6845.663512 ,6741.93165 ,6623.250477 ,6488.685008 ,6336.59083 ,6169.437256 ,5987.949573 ,5788.906433 ,5576.448005 ,5352.960131 ,5114.122257 ,4867.776804 ,4614.756792 ,4357.762557 ,4103.514131 ,3856.561537 ,3623.584999 ,3417.957487 ,3242.16386 ,3113.516325 ,3047.908086 ,3041.209887 ,3103.52448 ,3238.419914 ,3434.453282 ,3682.64091 ,3976.350364 ,4304.731796 ,4657.710643 ,5031.971207 ,5420.798807 ,5818.393048 ,6222.452645 ,6628.409919 ,7034.401617 ,7436.912983 ,7834.248901 ,8225.725437 ,8609.194437 ,8981.411186 ,9343.267849 ,9692.228964 ,10029.03943 ,10350.21497 ,10655.98094 ,10942.67886 ,11210.97561 ,11464.1765 ,11695.21416 ,11903.97877 ,12092.98182 ,12260.16475 ,12404.22798 ,12527.87363 ,12631.34739 ,12708.03002 ,12763.89567 ,12800.1345 ,12810.47274 ,12802.19426 ,12774.46336 ,12724.15248 ,12654.88896 ,12569.36792 ,12463.67047 ,12340.27279 ,12205.28725 ,12053.6089 ,11887.85615 ,11711.05908 ,11522.69162 ,11324.86867 ,11118.70802 ,10907.28501 ,10689.02283 ,10468.10657 ,10244.11807 ,10020.44957 ,9798.475608 ,9578.106469 ,9362.245821 ,9150.663685 ,8945.22585 ,8749.072869 ,8559.95741 ,8381.791647 ,8214.03134 ,8057.683551 ,7911.729557 ,7778.018313 ,7656.640662 ,7544.912568 ,7442.142808 ,7350.358104 ,7265.147238 ,7187.056652 ,7113.483706 ,7042.728535 ,6973.31255 ,6904.080406 ,6831.932772 ,6756.934802 ,6677.431064 ,6588.231465 ,6491.776175 ,6388.297966 ,6272.679706 ,6145.257498 ,6006.018698 ,5853.53458 ,5689.614587 ,5512.067176 ,5321.979453 ,5118.376459 ,4904.433237 ,4680.289483 ,4446.964874 ,4206.41654 ,3961.580285 ,3716.794316 ,3475.450575 ,3244.792635 ,3030.93139 ,2843.536042 ,2688.437991 ,2581.689236 ,2529.519259 ,2531.517959 ,2597.873426 ,2718.102922 ,2883.772457 ,3086.234072 ,3319.614894 ,3569.417263 ,3831.629585 ,4100.24267 ,4369.148963 ,4635.225474 ,4893.246213 ,5143.96582 ,5382.634249 ,5609.112573 ,5820.310346 ,6018.723402 ,6199.605502 ,6363.963171 ,6512.794466 ,6645.50343 ,6762.111343 ,6863.458035 ,6951.752877 ,7024.812755 ,7086.80772 ,7138.247104 ,7179.119904 ,7213.675952 ,7241.287762 ,7266.435526 ,7288.684418 ,7310.826446 ,7335.590942 ,7364.072506 ,7399.193224 ,7442.997198 ,7494.971674 ,7557.757098 ,7634.376392 ,7724.307542 ,7827.601606 ,7944.770386 ,8076.269006 ,8221.208118 ,8380.6356 ,8553.795949 ,8738.248167 ,8934.1115 ,9138.282775 ,9351.35019 ,9571.781245 ,9796.407757 ,10024.82546 ,10255.13262 ,10485.44857 ,10714.67229 ,10941.42437 ,11162.3433 ,11378.63982 ,11588.06102 ,11788.08277 ,11977.91226 ,12153.55505 ,12321.01613 ,12474.16476 ,12611.19749 ,12734.92522 ,12845.09067 ,12938.75396 ,13017.2056 ,13081.34263 ,13129.98233 ,13164.62402 ,13186.19808 ,13193.96697 ,13187.72462 ,13171.78815 ,13143.95967 ,13107.04955 ,13062.17676 ,13008.1319 ,12948.04152 ,12883.10565 ,12813.44797 ,12739.56956 ,12664.83263 ,12587.13215 ,12508.70418 ,12431.05937 ,12354.98486 ,12279.46139 ,12206.496 ,12137.07871 ,12071.16158 ,12007.52747 ,11948.88199 ,11894.14568 ,11843.49975 ,11796.52194 ,11753.73527 ,11715.60503 ,11681.58936 ,11650.08995 ,11623.09256 ,11600.38888 ,11579.40645 ,11563.29747 ,11551.05709 ,11542.6334 ,11537.48496 ,11535.42726 ,11537.42103 ,11544.77312 ,11559.43779 ,11575.84986 ,11599.99856 ,11629.26686 ,11664.30679 ,11707.04535 ,11756.75695 ,11811.03295 ,11872.58481 ,11939.11251 ,12011.76818 ,12089.33144 ,12170.70377 ,12253.8579 ,12339.70032 ,12425.96186 ,12511.34923 ,12595.70215 ,12676.6589 ,12751.53832 ,12822.87606 ,12884.02369 ,12937.41412 ,12979.48312 ,13012.01692 ,13029.78555 ,13035.4856 ,13027.17886 ,13002.809 ,12962.93255 ,12908.31989 ,12838.39404 ,12750.92481 ,12647.47123 ,12530.65454 ,12396.08823 ,12249.45031 ,12088.8269 ,11914.20291 ,11728.58395 ,11532.69684 ,11327.01341 ,11113.65689 ,10894.03133 ,10668.84883 ,10438.7605 ,10206.99247 ,9974.651705 ,9743.89483 ,9514.83666 ,9289.589958 ,9070.926461 ,8859.481993 ,8656.282663 ,8463.734419 ,8283.910637 ,8112.532721 ,7959.125539 ,7818.12788 ,7690.987429 ,7581.313148 ,7488.329254 ,7409.736125 ,7348.960846 ,7306.569826 ,7278.380538 ,7269.07049 ,7275.377701 ,7297.902554 ,7334.059031 ,7385.039165 ,7451.828541 ,7529.843582 ,7619.928664 ,7723.658435 ,7836.11219 ,7959.288786 ,8090.874211 ,8230.153438 ,8377.124411 ,8529.252976 ,8685.010256 ,8844.447209 ,9005.959626 ,9168.784264 ,9330.981779 ,9491.50238 ,9650.173972 ,9805.695481 ,9957.289887 ,10101.92456 ,10243.97996 ,10377.4652 ,10503.539 ,10621.81942 ,10732.78208 ,10833.36977 ,10927.45382 ,11013.25538 ,11090.45336 ,11160.3856 ,11223.65866 ,11278.7484 ,11327.81121 ,11370.90299 ,11408.32403 ,11440.66073 ,11468.58569 ,11492.43691 ,11511.91894 ,11528.86775 ,11544.32499 ,11555.51405 ,11565.34295 ,11572.33204 ,11578.98324 ,11584.82054 ,11587.74657 ,11588.9204 ,11590.7703 ,11589.24525 ,11587.41907 ,11585.57426 ,11581.816 ,11576.56597 ,11570.72481 ,11563.55119 ,11554.8968 ,11545.50617 ,11533.14724 ,11519.38033 ,11501.01782 ,11483.06418 ,11458.97169 ,11431.65984 ,11400.34301 ,11365.4187 ,11325.13367 ,11279.18137 ,11228.66327 ,11169.6989 ,11104.1932 ,11034.22133 ,10956.32917 ,10870.05193 ,10777.5897 ,10677.2958 ,10571.00455 ,10456.01739 ,10336.13055 ,10210.40415 ,10077.22706 ,9940.147619 ,9797.15876 ,9649.824037 ,9499.930211 ,9346.968787 ,9192.231155 ,9036.843211 ,8881.851435 ,8729.034219 ,8578.562076 ,8431.288862 ,8290.409408 ,8155.314831 ,8028.819474 ,7911.542035 ,7806.209696 ,7710.902971 ,7628.846024 ,7562.534946 ,7509.183828 ,7472.463173 ,7451.757046 ,7446.55672 ,7458.456762 ,7485.986022 ,7529.031771 ,7586.599911 ,7659.97962 ,7746.134492 ,7845.215601 ,7956.306768 ,8078.071933 ,8209.194321 ,8348.589313 ,8494.365189 ,8646.898209 ,8802.051137 ,8959.524293 ,9119.79155 ,9278.564119 ,9436.4326 ,9591.881166 ,9743.735153 ,9889.896946 ,10031.40029 ,10164.09042 ,10290.38242 ,10409.04977 ,10517.8358 ,10614.08142 ,10702.42324 ,10778.97555 ,10844.00159 ,10897.63983 ,10938.01374 ,10968.24098 ,10984.28144 ,10989.31807 ,10983.0344 ,10963.40038 ,10932.81085 ,10889.81252 ,10834.47549 ,10770.36585 ,10695.06027 ,10608.08332 ,10512.81866 ,10408.63353 ,10296.16951 ,10175.64808 ,10047.95917 ,9914.803837 ,9775.366115 ,9633.199528 ,9484.595532 ,9336.195433 ,9185.999619 ,9034.951601 ,8885.756998 ,8738.91776 ,8596.518757 ,8460.967277 ,8331.771426 ,8211.53078 ,8102.681684 ,8007.971661 ,7923.857166 ,7858.70879 ,7811.458456 ,7780.285792 ,7769.597238 ,7778.819071 ,7809.539543 ,7859.841196 ,7932.085045 ,8024.611906 ,8135.26682 ,8266.299315 ,8413.458514 ,8575.824748 ,8754.455048 ,8943.247858 ,9143.56987 ,9353.651638 ,9570.875424 ,9793.161945 ,10020.77524 ,10249.39796 ,10475.09444 ,10699.83326 ,10921.08453 ,11137.85298 ,11343.77859 ,11540.32279 ,11727.8195 ,11902.61433 ,12062.98186 ,12208.00523 ,12336.54765 ,12445.56406 ,12537.2527 ,12608.70785 ,12660.58494 ,12688.75119 ,12694.5681 ,12679.49193 ,12639.35761 ,12575.94105 ,12489.61035 ,12379.29308 ,12245.76034 ,12090.80136 ,11910.71956 ,11708.33697 ,11486.79379 ,11243.735 ,10979.41521 ,10696.97842 ,10397.40099 ,10080.97124 ,9748.826833 ,9403.766336 ,9043.694222 ,8672.049588 ,8290.582168 ,7899.711377 ,7501.48978 ,7098.331708 ,6689.613317 ,6279.636014 ,5869.653538 ,5462.199118 ,5061.397849 ,4667.470298 ,4288.229967 ,3929.0813 ,3590.803599 ,3285.112178 ,3021.408508 ,2805.008092 ,2644.093789 ,2548.358697 ,2517.781391 ,2543.148705 ,2624.90407 ,2745.705376 ,2890.822604 ,3053.502025 ,3221.901991 ,3390.537695 ,3551.22486 ,3701.194711 ,3836.982608 ,3954.971228 ,4049.968776 ,4126.287425 ,4180.551515 ,4210.636382 ,4217.085555 ,4201.094743 ,4161.743206 ,4097.548186 ,4016.531899 ,3913.377163 ,3791.637309 ,3656.60925 ,3508.385981 ,3351.669912 ,3191.358915 ,3033.261883 ,2881.568172 ,2745.274906 ,2628.638168 ,2546.187049 ,2498.725495 ,2489.283956 ,2525.4045 ,2601.542778 ,2711.576179 ,2851.046105 ,3011.48038 ,3185.397793 ,3365.605493 ,3548.466145 ,3726.736212 ,3896.341674 ,4053.518114 ,4196.694343 ,4322.447534 ,4427.26699 ,4510.518062 ,4574.729849 ,4611.702878 ,4625.094165 ,4618.096759 ,4585.583825 ,4529.270631 ,4452.968381 ,4356.034778 ,4236.829314 ,4103.805351 ,3955.837443 ,3796.731989 ,3629.582213 ,3460.217293 ,3295.383745 ,3137.814795 ,2995.882736 ,2878.27998 ,2791.320951 ,2736.707602 ,2723.545123 ,2752.963269 ,2817.415077 ,2917.069627 ,3044.916183 ,3192.482347 ,3353.767354 ,3521.706568 ,3689.757349 ,3854.636796 ,4009.588836 ,4152.870848 ,4280.990422 ,4390.413329 ,4479.775204 ,4548.984364 ,4595.102172 ,4615.203116 ,4614.570599 ,4588.672426 ,4535.846521 ,4461.998819 ,4364.438236 ,4244.956921 ,4103.640257 ,3945.52295 ,3772.177296 ,3586.466854 ,3393.95016 ,3197.540834 ,3006.338382 ,2826.020565 ,2670.079607 ,2549.749466 ,2474.002093 ,2453.153965 ,2499.117319 ,2604.018022 ,2766.491509 ,2985.691177 ,3241.791784 ,3530.268194 ,3842.07504 ,4171.304325 ,4512.950465 ,4860.576214 ,5210.834578 ,5563.411386 ,5913.974352 ,6258.935194 ,6598.017535 ,6929.330758 ,7252.842761 ,7564.98028 ,7867.284401 ,8156.785434 ,8433.531418 ,8698.054241 ,8946.306096 ,9180.607976 ,9399.195109 ,9601.3864 ,9787.897318 ,9956.741073 ,10108.59653 ,10242.74268 ,10358.09483 ,10455.71316 ,10533.95381 ,10593.04304 ,10633.38719 ,10653.56951 ,10656.52234 ,10638.03974 ,10600.28917 ,10544.74653 ,10471.99452 ,10377.62095 ,10265.96721 ,10138.60606 ,9990.596305 ,9825.818737 ,9645.474427 ,9447.479734 ,9231.94486 ,9001.610345 ,8756.454039 ,8496.556699 ,8222.399519 ,7935.827758 ,7634.919743 ,7324.014107 ,7003.200708 ,6670.655043 ,6331.101182 ,5985.62862 ,5637.789032 ,5288.44868 ,4940.251071 ,4596.245121 ,4262.182824 ,3942.223428 ,3638.364638 ,3364.607615 ,3122.77035 ,2918.799507 ,2768.665651 ,2671.166226 ,2628.235916 ,2642.924186 ,2705.0515 ,2803.384856 ,2933.129301 ,3081.625456 ,3238.588214 ,3398.302666 ,3552.990341 ,3698.842157 ,3830.514072 ,3946.07059 ,4043.977505 ,4121.57098 ,4175.516125 ,4206.830028 ,4216.168418 ,4199.697685 ,4159.983797 ,4098.58349 ,4014.016695 ,3907.180211 ,3781.903251 ,3639.014746 ,3479.503827 ,3307.41909 ,3126.205906 ,2941.965898 ,2758.891728 ,2584.087985 ,2428.184427 ,2298.926716 ,2204.903989 ,2158.813714 ,2169.974001 ,2229.353362 ,2343.755573 ,2504.364009 ,2695.314724 ,2913.912513 ,3149.381361 ,3396.431049 ,3648.809815 ,3902.063008 ,4152.735039 ,4398.698853 ,4636.679606 ,4864.227721 ,5081.393243 ,5286.769683 ,5480.134201 ,5661.20612 ,5828.423392 ,5981.748873 ,6124.076066 ,6252.538831 ,6368.110449 ,6474.026769 ,6567.397401 ,6650.903074 ,6724.949703 ,6789.883423 ,6847.876085 ,6899.648438 ,6945.91953 ,6985.540551 ,7022.222549 ,7055.915006 ,7085.635608 ,7111.829684 ,7136.06526 ,7156.920427 ,7173.310246 ,7185.417806 ,7194.134283 ,7195.161515 ,7189.982249 ,7175.950854 ,7154.203293 ,7121.488769 ,7077.801154 ,7021.191574 ,6949.931748 ,6866.287192 ,6767.23487 ,6651.39005 ,6520.642901 ,6372.700704 ,6209.19777 ,6029.760997 ,5833.233038 ,5623.10621 ,5398.663758 ,5161.528979 ,4911.851467 ,4654.077645 ,4389.313003 ,4121.283195 ,3853.453268 ,3589.945276 ,3338.686328 ,3106.716933 ,2898.387702 ,2728.658317 ,2608.365527 ,2537.174014 ,2526.896261 ,2578.464685 ,2682.44001 ,2829.934966 ,3015.752434 ,3228.9475 ,3457.450593 ,3697.534103 ,3941.567644 ,4186.690453 ,4427.864147 ,4661.731521 ,4887.879689 ,5103.538376 ,5307.528173 ,5501.236266 ,5681.668881 ,5846.933984 ,6001.692045 ,6144.748145 ,6274.193537 ,6392.570586 ,6500.96661 ,6597.486982 ,6685.832285 ,6766.908494 ,6842.624782 ,6911.561665 ,6976.895632 ,7038.745617 ,7098.931043 ,7158.74036 ,7218.045138 ,7278.938701 ,7340.868699 ,7404.911952 ,7470.186331 ,7537.091943 ,7606.029745 ,7676.094005 ,7744.918589 ,7814.478262 ,7880.909357 ,7945.126834 ,8005.774387 ,8061.801452 ,8112.810643 ,8155.999663 ,8192.575711 ,8220.369442 ,8242.195049 ,8254.637574 ,8257.634419 ,8252.69174 ,8239.668938 ,8221.355261 ,8194.711405 ,8163.285602 ,8126.198451 ,8086.99929 ,8045.669071 ,8004.596513 ,7964.149508 ,7928.33165 ,7897.025062 ,7872.317657 ,7856.624679 ,7849.381351 ,7854.73255 ,7872.500559 ,7902.921048 ,7945.484715 ,8002.913076 ,8071.415192 ,8154.912399 ,8250.819181 ,8356.338277 ,8471.860484 ,8595.73979 ,8727.336533 ,8864.964288 ,9006.942309 ,9151.347283 ,9299.34306 ,9445.94685 ,9592.732452 ,9736.120265 ,9877.801927 ,10015.18558 ,10148.38558 ,10277.75643 ,10399.21263 ,10515.20158 ,10624.96199 ,10727.74573 ,10824.24668 ,10912.91132 ,10994.12824 ,11069.32844 ,11136.43369 ,11197.21044 ,11251.42656 ,11299.96167 ,11343.35697 ,11381.14068 ,11415.60651 ,11445.06029 ,11470.05929 ,11490.92245 ,11509.608 ,11525.16043 ,11538.90062 ,11551.24972 ,11559.56591 ,11568.84629 ,11575.37263 ,11579.39912 ,11584.34029 ,11588.0503 ,11588.99335 ,11590.29558 ,11589.64979 ,11588.38427 ,11585.36777 ,11580.55662 ,11574.13519 ,11567.22043 ,11558.16676 ,11547.2607 ,11534.95842 ,11519.37423 ,11499.80482 ,11476.86878 ,11449.87948 ,11418.46259 ,11380.62209 ,11336.39891 ,11285.75882 ,11228.39706 ,11160.45538 ,11084.31821 ,10998.66818 ,10900.29489 ,10789.90843 ,10669.24281 ,10535.65214 ,10387.58275 ,10226.06884 ,10051.45346 ,9862.588611 ,9660.064115 ,9444.921986 ,9214.039826 ,8971.13212 ,8714.509802 ,8444.035509 ,8162.331387 ,7869.110052 ,7563.776865 ,7249.04564 ,6925.790787 ,6591.887206 ,6252.170798 ,5906.378912 ,5556.306515 ,5204.653591 ,4854.148699 ,4506.699378 ,4165.712056 ,3835.801358 ,3523.816685 ,3237.058869 ,2979.625816 ,2764.342031 ,2603.226252 ,2497.080681 ,2452.958931 ,2475.481747 ,2552.403629 ,2673.009551 ,2830.339769 ,3009.822155 ,3200.788162 ,3396.99975 ,3590.40125 ,3776.301728 ,3949.579548 ,4106.517096 ,4246.260969 ,4365.74104 ,4463.319746 ,4538.618028 ,4589.634821 ,4614.878373 ,4617.078593 ,4595.326825 ,4547.981607 ,4478.697316 ,4390.392553 ,4278.324242 ,4148.769662 ,4005.883569 ,3849.560602 ,3685.622313 ,3517.431614 ,3349.377596 ,3188.366177 ,3041.131347 ,2912.87637 ,2815.714606 ,2749.674655 ,2721.910662 ,2737.881737 ,2792.518512 ,2879.981945 ,2999.316252 ,3141.861642 ,3299.000539 ,3465.858523 ,3635.065473 ,3801.291988 ,3961.325328 ,4108.565616 ,4241.411533 ,4359.140933 ,4456.667281 ,4533.786168 ,4590.467701 ,4621.061385 ,4629.955802 ,4616.323114 ,4578.554519 ,4517.883568 ,4437.511105 ,4334.974228 ,4214.084569 ,4078.270128 ,3929.968597 ,3773.099948 ,3610.424218 ,3448.035153 ,3294.124043 ,3154.705437 ,3036.739978 ,2951.702424 ,2902.933102 ,2893.848584 ,2936.248031 ,3024.404302 ,3151.759199 ,3317.837051 ,3513.287623 ,3730.711688 ,3962.308266 ,4206.120676 ,4453.665645 ,4701.917075 ,4946.452642 ,5183.910449 ,5414.304438 ,5631.75157 ,5837.703164 ,6030.488752 ,6206.809926 ,6367.564202 ,6514.559754 ,6643.851318 ,6756.123922 ,6856.411501 ,6939.860575 ,7008.522964 ,7065.107051 ,7108.568145 ,7142.587113 ,7166.870929 ,7183.415453 ,7193.648266 ,7199.099697 ,7201.701214 ,7203.218218 ,7204.73547 ,7207.744547 ,7215.755496 ,7228.554814 ,7246.864499 ,7273.850071 ,7310.192501 ,7353.81605 ,7409.097162 ,7475.069437 ,7550.513944 ,7637.30001 ,7734.050004 ,7841.426044 ,7957.173227 ,8084.110418 ,8218.212495 ,8358.670503 ,8505.918967 ,8658.167527 ,8814.067031 ,8972.370345 ,9131.592504 ,9290.919678 ,9449.797361 ,9604.751523 ,9756.648014 ,9904.038868 ,10045.20103 ,10178.82049 ,10306.01419 ,10424.10898 ,10530.82748 ,10628.32418 ,10715.93059 ,10788.52694 ,10852.32177 ,10904.81547 ,10942.81252 ,10969.58889 ,10983.99325 ,10986.10911 ,10977.54691 ,10957.22648 ,10924.05508 ,10880.12046 ,10826.41523 ,10759.70432 ,10684.0919 ,10598.9876 ,10504.10616 ,10401.02741 ,10289.16242 ,10170.37373 ,10043.34013 ,9910.696915 ,9771.830336};

const float QMSL_Linear_Predistortion_NV::ampl_in[ENV_IN_NUM_SAMPLES_SWAPPED] =
{
   10987,10983,10974,10960,10942,10922,10897,10871,10842,10814,10786,
   10758,10732,10709,10689,10674,10663,10658,10656,10661,10670,10685,
   10706,10729,10759,10790,10824,10861,10897,10934,10970,11003,11034,
   11062,11085,11103,11116,11121,11119,11111,11096,11070,11038,10997,
   10947,10889,10823,10748,10664,10573,10475,10368,10255,10135,10012,
   9881,9746.8,9609.1,9467.8,9326,9181,9038,8895.6,8755.7,8619.4,8487.5,
   8360.4,8241.1,8127.8,8023.4,7926,7837.8,7758.2,7687.3,7625,7569,7519.4,
   7475.8,7435,7397.4,7359,7321.7,7280.8,7235.6,7184,7124.5,7057.8,6980.3,
   6890.4,6789.3,6675,6546.2,6403.6,6247.3,6074.8,5887.5,5687.4,5474.7,5247,
   5008.8,4762,4506.3,4247,3985.2,3728,3478.5,3242.6,3031.8,2856.4,2722.7,
   2640,2628.3,2679,2790.3,2961.6,3179.5,3434.2,3717.4,4025.2,4344.6,4675,
   5009.7,5347.6,5685.1,6021,6352,6677.4,6997,7306.6,7607.3,7899,8179,
   8447.2,8705.3,8949,9179,9397.8,9602.7,9791,9968.1,10130,10275,10408,
   10527,10630,10720,10795,10856,10903,10937,10960,10966,10962,10947,
   10920,10880,10831,10771,10700,10621,10532,10434,10328,10216,10096,
   9969.5,9837.6,9701.2,9560,9415.6,9269,9119.8,8971.6,8823.4,8677.6,8533.5,
   8394.2,8260,8132,8012,7901,7800.4,7710.4,7631.8,7565.8,7515.5,7478,
   7455.2,7448,7455,7475,7513.2,7564,7627.9,7705.2,7794.5,7894.2,8004.1,
   8124.4,8252,8387,8527.3,8672.2,8820.5,8971.6,9123.5,9275.4,9426.6,9576.2,
   9721.7,9864,10001,10132,10258,10377,10489,10594,10693,10782,10865,
   10942,11012,11076,11136,11190,11239,11288,11334,11379,11423,11469,
   11516,11566,11617,11674,11734,11797,11865,11937,12014,12093,12177,
   12262,12350,12440,12530,12619,12707,12793,12874,12953,13026,13092,
   13153,13205,13251,13286,13315,13332,13341,13341,13332,13313,13287,
   13252,13210,13161,13106,13045,12980,12911,12839,12763,12685,12608,
   12530,12452,12375,12298,12224,12151,12081,12011,11943,11879,11815,
   11753,11691,11629,11568,11504,11439,11372,11302,11227,11149,11066,
   10978,10882,10782,10676,10563,10443,10318,10186,10050,9909,9763.5,
   9613.2,9461.1,9307,9151.7,8995.6,8840,8687.6,8536.5,8389.4,8247.4,8110.4,
   7978.7,7853,7735.2,7625.2,7522.7,7429.2,7343,7265.6,7196.2,7135.4,7081,
   7034,6995.6,6962.8,6936,6912.4,6896.5,6883,6873.4,6866.8,6862.9,6861,
   6860,6860,6860.9,6862.4,6865,6867.6,6868.1,6869.4,6870.7,6872,6871.7,
   6870.4,6870,6869.6,6867.5,6864.6,6862.8,6860.2,6856.9,6854,6851.1,6847.2,
   6844.2,6841.8,6839.5,6837.2,6836,6835.6,6835,6836,6837,6837.6,6839.8,
   6842.4,6845.5,6848.6,6853.1,6855.2,6859.1,6862,6865.9,6869.2,6871.8,6875.4,
   6878,6879.8,6882.3,6885.4,6888.1,6890,6894.6,6898.4,6901.8,6908,6915,
   6922.8,6931.8,6943,6955.3,6969,6985.6,7002.4,7022.4,7042,7062,7082,
   7103.5,7121.8,7137.4,7151,7159.8,7164,7162.2,7153.4,7134.5,7107.6,7070.3,
   7019.8,6958.3,6883,6793.2,6689,6572.2,6439.4,6289,6125.8,5948.3,5754,
   5546.9,5328,5097.1,4855.6,4607.2,4351.4,4094,3835.8,3584.3,3344,3118.4,
   2917,2751.3,2626.4,2546.3,2524.6,2558,2641,2769.4,2936.6,3129.2,3343,
   3568.4,3801,4036.8,4270.6,4500,4722.2,4937.7,5140.6,5335.9,5519,5689.5,
   5847.2,5994.8,6129.4,6252,6363.4,6464.1,6554.4,6636,6710,6774.1,6832.2,
   6883.2,6929.6,6970.5,7008.2,7041.3,7071.2,7099,7124,7145.8,7164.2,7178.9,
   7189.8,7194.5,7195.2,7188.1,7174.4,7152,7121,7078.6,7025,6959.3,6881.4,
   6789,6683.8,6563.9,6429,6280.4,6119,5941.1,5751,5549.7,5336.2,5114.5,
   4885.6,4653,4419,4187.1,3961,3750.8,3557.8,3388.5,3255.6,3161.5,3110.6,
   3107.8,3155.4,3247.1,3376,3544.3,3735.4,3944.6,4169.4,4401,4635.8,4868.8,
   5098.6,5321.3,5537,5739.3,5930.8,6110.4,6276.2,6428.5,6568,6692.7,6803,
   6902.8,6991,7065.9,7131.2,7189.6,7240.8,7284,7324.8,7362,7400.6,7439,
   7478,7524.8,7573.4,7629.5,7692.8,7765,7845.2,7933.2,8029.6,8136.5,8250,
   8370.4,8498.4,8629.8,8766.6,8907.5,9050,9193.1,9337.4,9481,9622,9760.5,
   9895.8,10027,10153,10274,10389,10497,10599,10694,10780,10861,10932,
   10997,11053,11101,11141,11174,11197,11212,11220,11219,11209,11192,
   11167,11132,11091,11042,10983,10916,10843,10762,10672,10577,10473,
   10364,10248,10125,9997.4,9866.6,9731,9592.3,9450.6,9308.9,9167.2,9026.5,
   8888.2,8754,8624.8,8502.3,8389,8286.8,8194.8,8116.7,8053,8005.5,7974.8,
   7964.2,7971.2,7997.2,8042,8108.7,8192.6,8293.9,8413.2,8548.5,8696.6,8860.3,
   9035.2,9219.5,9412,9612.1,9816.8,10027,10239,10450,10662,10873,11080,
   11282,11479,11669,11851,12026,12191,12345,12489,12622,12741,12849,
   12946,13027,13096,13154,13196,13228,13247,13256,13251,13237,13212,
   13179,13138,13089,13034,12973,12908,12839,12767,12693,12617,12542,
   12466,12393,12319,12249,12180,12114,12053,11994,11938,11887,11839,
   11795,11755,11719,11684,11655,11627,11604,11583,11566,11553,11543,
   11536,11533,11534,11540,11549,11565,11584,11611,11642,11679,11722,
   11772,11827,11889,11956,12028,12104,12184,12269,12354,12443,12532,
   12622,12711,12796,12881,12961,13037,13109,13173,13233,13286,13332,
   13371,13402,13428,13446,13457,13464,13465,13462,13453,13442,13429,
   13414,13399,13383,13368,13356,13345,13337,13331,13328,13329,13335,
   13342,13353,13368,13385,13404,13425,13446,13468,13491,13512,13533,
   13550,13565,13578,13586,13592,13594,13592,13586,13577,13565,13551,
   13532,13512,13491,13469,13448,13425,13406,13387,13370,13356,13347,
   13339,13336,13336,13341,13349,13361,13376,13394,13413,13436,13460,
   13483,13509,13532,13555,13577,13595,13611,13624,13635,13640,13644,
   13644,13641,13634,13625,13612,13599,13584,13568,13551,13535,13519,
   13505,13491,13479,13472,13465,13459,13457,13457,13460,13463,13467,
   13472,13477,13482,13485,13486,13482,13474,13462,13441,13415,13381,
   13336,13282,13217,13142,13055,12957,12846,12723,12589,12443,12286,
   12118,11940,11753,11558,11354,11145,10931,10713,10493,10270,10049,
   9829.4,9611,9399.7,9193.4,8993.3,8803.4,8621.5,8450.8,8290.6,8145.2,8011.7,
   7891,7783.2,7689.2,7608,7538.6,7480.5,7432,7393.4,7361.2,7334,7311,
   7290.2,7268.8,7245.8,7218.4,7187,7147.6,7101.7,7043.8,6976.5,6897,6803.6,
   6696.8,6576.5,6441.4,6290,6125,5945.2,5750,5540.8,5320,5086.8,4844.6,
   4594.4,4339.6,4084,3832.4,3589.5,3366.4,3167.9,3003,2893,2837.8,2844.3,
   2924.6,3071,3272.6,3524.2,3820,4143.9,4492,4859.4,5238.6,5625.2,6017.8,
   6414,6810.2,7203.2,7593.8,7978.5,8356,8724,9084.4,9435.2,9772.6,10098,
   10411,10710,10992,11260,11513,11749,11968,12171,12357,12525,12678,
   12816,12935,13041,13132,13210,13275,13327,13369,13403,13428,13445,
   13456,13463,13465,13465,13463,13458,13456,13453,13450,13450,13450,
   13453,13458,13464,13473,13483,13493,13505,13515,13525,13533,13540,
   13544,13543,13540,13531,13516,13496,13470,13437,13398,13352,13300,
   13239,13174,13104,13028,12947,12863,12776,12687,12596,12506,12414,
   12326,12239,12156,12076,12001,11931,11865,11806,11752,11705,11664,
   11629,11599,11577,11558,11545,11537,11533,11534,11538,11546,11557,
   11573,11591,11613,11637,11666,11697,11731,11770,11812,11857,11907,
   11959,12016,12074,12139,12204,12272,12343,12417,12490,12564,12637,
   12710,12780,12847,12911,12969,13021,13065,13102,13129,13146,13151,
   13144,13123,13088,13040,12974,12891,12793,12676,12543,12391,12223,
   12036,11831,11609,11370,11114,10844,10556,10254,9937.4,9608.7,9264.8,
   8911.5,8547,8172.4,7789.8,7400.1,7003.4,6602,6199,5793,5390.8,4992.5,
   4602,4225.7,3867.6,3535,3243.4,3004,2827,2728,2723.2,2802.5,2959,
   3192.5,3476.4,3799.5,4152.8,4527,4915.6,5312.8,5714.8,6120.4,6526,6926.4,
   7324.4,7716,8102,8477,8842.4,9196.6,9539.2,9867.6,10183,10481,10763,
   11030,11275,11501,11708,11895,12057,12199,12319,12414,12485,12534,
   12557,12556,12532,12483,12409,12312,12193,12050,11886,11701,11493,
   11266,11021,10758,10476,10180,9867,9539.6,9198.8,8847.2,8482.8,8108.5,
   7726.2,7334.7,6939.2,6538.7,6135,5730,5328.8,4931.5,4542.8,4169.5,3814.6,
   3487.3,3203.8,2971.6,2803,2724,2729.8,2819.5,2994.2,3234,3523.4,3852.2,
   4210.6,4586.1,4976,5375.3,5779,6183.8,6588,6990,7387.2,7779.6,8163.2,
   8538.5,8904,9257.1,9600,9929.9,10244,10542,10827,11095,11344,11575,
   11788,11980,12151,12304,12435,12545,12634,12703,12749,12775,12782,
   12767,12733,12680,12609,12522,12417,12298,12162,12015,11856,11685,
   11503,11315,11117,10913,10705,10494,10279,10064,9848,9633.8,9422.4,
   9214,9013.4,8817,8627,8446.7,8275.8,8114.3,7962,7823.4,7694.4,7577.7,
   7471,7377,7292.6,7220,7157,7102,7057,7018.5,6987.8,6961.9,6941.6,
   6926.5,6913.8,6904.3,6896.8,6891.5,6889,6885,6884.4,6884,6883,6884.5,
   6886,6888.2,6891.6,6897.5,6904,6914.4,6927.2,6942.8,6964.4,6989.5,7021.8,
   7060.4,7106.2,7160.5,7225,7299.3,7382.8,7478.4,7584.8,7704,7832.8,7974.4,
   8125.8,8288.6,8461,8642.2,8832.4,9028.8,9232,9441,9652.4,9867.7,10084,
   10301,10517,10731,10942,11148,11349,11544,11731,11908,12076,12235,
   12383,12516,12639,12749,12844,12926,12994,13047,13088,13114,13128,
   13126,13114,13091,13055,13010,12957,12895,12826,12751,12672,12590,
   12505,12420,12333,12248,12163,12083,12004,11929,11857,11790,11728,
   11669,11613,11561,11512,11466,11420,11376,11330,11283,11235,11186,
   11131,11072,11007,10936,10859,10775,10686,10587,10481,10369,10249,
   10123,9991.6,9854.1,9711.2,9564.9,9416,9264.9,9112.2,8960.6,8809.2,8660.5,
   8515,8372.6,8238.2,8107.7,7985,7870.8,7764.6,7668.2,7581.4,7503.5,7436.8,
   7379.2,7331.8,7293.1,7263,7239.8,7224,7212.8,7207.4,7204.5,7204,7201.9,
   7199.8,7194.5,7185,7168.2,7145.6,7113.7,7072.4,7019.5,6956.2,6879.1,6787,
   6682.5,6564,6427.7,6278,6113,5932.8,5739,5531.6,5311.4,5079.4,4836.7,
   4587,4331.6,4073,3816.6,3566.2,3325.5,3102.4,2903.7,2740.4,2617.6,2541,
   2526.9,2563,2648.5,2781.6,2950.5,3144.2,3359.2,3585.2,3818,4053,4287.1,
   4514.8,4737,4949.8,5152.5,5344,5525.1,5692.2,5847.2,5990,6119.2,6237.2,
   6342.2,6436,6517,6588.4,6650.3,6703,6745,6782,6811.4,6833,6850.7,
   6864.6,6874.5,6880.4,6884.3,6887.4,6888.7,6889,6888.7,6887.4,6886.1,6884.8,
   6884.5,6884.2,6882.9,6882.4,6883,6884,6883,6883.6,6884,6883.2,6884,
   6883.2,6883.9,6883,6883,6883,6883,6882.4,6883.8,6883.2,6884.5,6885,
   6886.1,6887,6888.4,6888,6888,6887.4,6885.2,6881.2,6874,6863.6,6852.7,
   6834.2,6810.7,6783,6745.7,6702.4,6650.2,6588.8,6518.5,6437,6343.1,6237.6,
   6121.6,5993,5849.9,5694.2,5527.1,5347.6,5154,4951.6,4737,4513.6,4282,
   4044,3802.9,3559.2,3317.5,3084.2,2862,2656.6,2475.1,2330.4,2225,2165,
   2160.3,2203.8,2287.8,2409,2557.5,2722.8,2896.6,3073.8,3249.8,3418,3575.5,
   3720.2,3849.4,3961,4052.5,4125.6,4176.7,4206.2,4214,4201,4162.6,4105.4,
   4028.3,3928.6,3811,3677.4,3529.5,3365.4,3195.6,3020,2842.5,2670,2509.1,
   2369.4,2259,2183.8,2155.1,2181,2253.3,2369,2528.7,2717.8,2928.4,3154.4,
   3392,3633.6,3876.6,4118,4354.3,4583,4803.8,5015.4,5215.8,5404.8,5581.5,
   5746.2,5898,6037.8,6166.2,6284,6387.1,6483,6567.7,6645,6714.5,6777.6,
   6835.2,6889.4,6942,6995,7047.6,7104,7163.4,7229.8,7302.5,7381.6,7472.5,
   7573.2,7685.3,7805,7940.7,8086.6,8242.8,8410.8,8588.5,8776.6,8971.6,9175.6,
   9385.8,9600,9819,10041,10263,10485,10706,10924,11138,11346,11547,
   11742,11927,12103,12269,12424,12566,12698,12816,12923,13017,13100,
   13170,13229,13278,13316,13344,13366,13379,13385,13386,13384,13377,
   13369,13358,13348,13338,13329,13321,13315,13313,13312,13314,13317,
   13323,13330,13338,13346,13352,13357,13360,13359,13353,13340,13319,
   13290,13251,13199,13136,13059,12969,12866,12743,12606,12452,12281,
   12092,11886,11666,11426,11170,10900,10613,10311,9996.4,9667,9325.5,
   8973.6,8610.5,8238.4,7859.2,7473,7082.8,6687.6,6292.4,5897.4,5505,5119,
   4743.9,4383,4041.1,3724,3444.2,3206.2,3017.5,2893.4,2838,2845.8,2920.3,
   3052,3224.8,3431,3663.8,3909.2,4162.4,4418.2,4672,4921,5160.9,5390.8,
   5608.8,5815,6005.3,6183.2,6344.9,6493.2,6626.5,6746.2,6852.2,6943.8,7024.6,
   7096,7155.3,7206.2,7249.8,7288,7320.5,7350.6,7379.1,7406.4,7434.4,7464,
   7495.8,7530.2,7568.9,7609.8,7655,7702.6,7753.1,7806.4,7859.7,7913,7966,
   8018,8066.4,8110.6,8151.5,8185.2,8214.8,8236.6,8251.3,8259,8259.1,8252.2,
   8239.1,8217,8190.5,8159.2,8121.9,8082,8040.2,7997,7953.8,7911.4,7872,
   7836.2,7806.5,7782.4,7764.1,7754.4,7752,7756,7769.2,7789.2,7815.8,7846.4,
   7883,7923.6,7966.3,8009.6,8053.1,8095,8133.8,8167.6,8199.6,8223.4,8242,
   8253.4,8257.9,8254.6,8243.3,8225,8199.4,8168.4,8131,8088.2,8042,7991.8,
   7941,7889.4,7840.1,7792,7750.7,7714.6,7686.1,7667.4,7660.5,7665.6,7684.3,
   7718.2,7767.4,7831,7913.6,8011.4,8123.1,8252.6,8397,8553.4,8722.8,8904.6,
   9094.6,9292,9499.6,9710.8,9926.3,10143,10361,10578,10792,11002,11205,
   11403,11589,11766,11932,12084,12222,12344,12450,12537,12607,12658,
   12687,12696,12685,12652,12597,12521,12423,12302,12162,12002,11818,
   11615,11395,11154,10897,10622,10332,10024,9704.7,9373,9026.7,8670.8,
   8305.3,7932.6,7552.5,7167.6,6779,6389,5999.7,5612,5232.6,4862.4,4503.4,
   4164.4,3849,3560.8,3310.5,3107.6,2951.8,2849,2811.1,2825.2,2884.6,2988.4,
   3121,3272.8,3436,3604.6,3770.2,3929,4076.8,4209.8,4329.1,4426,4503.5,
   4561,4597.3,4608.6,4598.2,4565,4508.8,4430.2,4332.8,4213,4077,3925,
   3760.7,3586.2,3406.7,3227,3057.5,2902.2,2771.5,2678.8,2634,2642,2711.4,
   2846.4,3032.6,3266,3545.6,3856.8,4191.8,4548.6,4919.5,5300.8,5690.2,6083.2,
   6477.8,6873,7264.4,7651.2,8031.9,8405.8,8771,9126.6,9469.6,9799.6,10117,
   10421,10707,10976,11229,11461,11675,11870,12044,12195,12325,12435,
   12521,12586,12628,12650,12648,12626,12584,12520,12439,12341,12223,
   12090,11943,11782,11608,11424,11230,11027,10820,10608,10391,10174,
   9957.6,9741.4,9530.5,9322.8,9124.1,8932.8,8749.7,8578,8419.9,8273.8,8141.9,
   8027.2,7928,7843.2,7775.8,7724.8,7688.9,7667,7660.3,7665.8,7682.5,7709.8,
   7744.5,7785.6,7830.9,7881.6,7931.6,7982,8033.1,8079.4,8122.7,8159.2,8191.5,
   8216.6,8233.9,8243.2,8244.3,8238,8223.2,8199.4,8168.5,8130.4,8085.5,8034.2,
   7977.3,7915.8,7852,7784,7715.1,7645.6,7575.4,7508.6,7441.5,7377.6,7317.6,
   7261.2,7209.4,7161,7119.4,7080.2,7047.4,7017.2,6993,6972,6953.9,6939.6,
   6926.7,6916,6908.5,6902,6897.3,6891.6,6888,6885.2,6881.7,6878.2,6875.6,
   6873,6868.7,6866.2,6862.3,6858.6,6855.5,6852.4,6848.8,6845.8,6842.6,6841,
   6838.4,6837,6836.1,6835.2,6835.5,6835.8,6837.1,6838.8,6841.4,6844,6847.3,
   6849.8,6853.7,6857.4,6860.5,6862.8,6865.3,6868.8,6871.4,6874,6875.3,6877.2,
   6878.9,6881.2,6883,6885.6,6890.4,6896,6903.2,6912,6924,6939.4,6958.5,
   6983.4,7014,7051.8,7094.9,7148.4,7209.6,7281,7364.1,7456,7559.8,7676,
   7803,7940.4,8090.2,8251.2,8420.4,8599,8788.7,8983.6,9184.4,9392.6,9604.5,
   9818.8,10036,10254,10472,10687,10900,11110,11314,11513,11704,11889,
   12064,12230,12386,12533,12667,12791,12904,13003,13093,13170,13237,
   13292,13337,13373,13398,13416,13427,13432,13431,13426,13416,13404,
   13392,13378,13365,13353,13341,13334,13327,13324,13324,13327,13334,
   13341,13355,13368,13382,13397,13413,13428,13442,13452,13460,13465,
   13464,13459,13447,13430,13405,13374,13336,13290,13237,13180,13114,
   13042,12967,12886,12803,12716,12627,12538,12448,12359,12271,12187,
   12103,12026,11951,11881,11814,11754,11697,11644,11597,11552,11510,
   11471,11432,11394,11354,11313,11270,11221,11169,11110,11046,10971,
   10888,10796,10693,10578,10452,10314,10163,9998,9821.8,9632,9427.5,
   9211.8,8981.6,8739.2,8484,8217,7939,7650,7351.2,7045,6729.5,6407.6,
   6081.8,5753.4,5423.9,5096,4776.1,4463.2,4162.8,3882.2,3626,3399.4,3210,
   3065.6,2968.3,2919,2923.2,2970.6,3054.9,3172.4,3310,3460.4,3618.3,3777,
   3930.2,4075,4206.2,4323,4423.6,4504.4,4564.5,4603.8,4623,4617.4,4590.8,
   4546,4475.1,4386.6,4280.5,4157.4,4020,3872.2,3716.7,3557.2,3401.4,3254,
   3125,3019.4,2945.1,2916.6,2934.5,2998.8,3114,3279.6,3482.9,3719,3988.1,
   4276.8,4582.2,4898.4,5222,5550.4,5880.2,6207.4,6531.6,6853,7163.5,7467.2,
   7762.5,8047.4,8320.5,8584,8834.4,9071.2,9296.3,9508,9706.5,9890.8,10063,
   10223,10368,10502,10624,10734,10832,10921,11000,11071,11133,11190,
   11240,11287,11329,11369,11408,11447,11486,11526,11569,11614,11664,
   11718,11777,11839,11908,11978,12056,12135,12219,12305,12393,12483,
   12573,12662,12750,12835,12918,12996,13070,13140,13202,13259,13307,
   13351,13386,13416,13437,13451,13462,13464,13464,13457,13449,13435,
   13422,13407,13390,13375,13360,13348,13336,13328,13322,13319,13319,
   13321,13327,13334,13343,13353,13362,13372,13380,13385,13387,13384,
   13376,13360,13338,13305,13264,13212,13151,13075,12990,12892,12782,
   12659,12525,12377,12220,12050,11873,11683,11487,11284,11074,10860,
   10641,10420,10199,9978.8,9760.4,9545.4,9334.3,9130,8932.9,8744.8,8567.4,
   8400.4,8245,8102.4,7972.7,7855.8,7752.8,7663,7585.1,7519.8,7464.1,7420,
   7383,7353.2,7327.2,7304.6,7283.8,7262,7237.3,7210.2,7176.7,7135.8,7086.5,
   7027.6,6956.8,6873.8,6779.1,6670,6545.7,6409,6257.9,6093,5913.5,5721.2,
   5517.7,5303.4,5080.2,4851,4616.7,4383.2,4151.8,3929,3720,3529.4,3365.6,
   3239.2,3150.8,3105,3113.7,3167,3263.7,3402,3572.5,3766.2,3979.2,4204.4,
   4436.7,4671,4904.1,5132.4,5355,5566.6,5768.5,5957.6,6133.8,6296.6,6445.7,
   6582,6700.8,6808.4,6902.1,6983.2,7052.5,7110.6,7158.9,7195.8,7226.7,7250,
   7268.7,7281,7290.4,7295.8,7300,7301.8,7303,7303,7301.6,7299,7294.2,
   7285.8,7276.7,7261.2,7241.5,7215.4,7181.9,7139.2,7087.6,7025,6949.8,6862.4,
   6762.9,6648,6519.5,6376.6,6217.9,6044.4,5856,5655,5438.4,5210,4971.5,
   4722,4466.5,4206.2,3946.1,3689,3441.1,3208,3003.8,2831.6,2705.7,2637.2,
   2632.5,2690.2,2812.1,2993.2,3217.5,3476,3764.9,4073.2,4395,4726.4,5061.5,
   5399,5737.9,6073,6402.4,6729,7045.3,7355,7655.1,7945.4,8225,8494.2,
   8751.1,8996.2,9228.5,9450,9656.9,9851.6,10035,10205,10365,10511,10647,
   10772,10886,10993,11090,11181,11266,11342,11415,11485,11551,11617,
   11680,11742,11806,11869,11935,12000,12068,12138,12209,12282,12357,
   12432,12506,12582,12656,12729,12799,12866,12928,12985,13034,13078,
   13111,13135,13149,13150,13139,13115,13075,13019,12950,12863,12759,
   12637,12499,12342,12167,11975,11765,11537,11293,11034,10756,10464,
   10159,9837.2,9504,9158,8801,8433.8,8056.6,7672,7279.8,6882.8,6480.1,
   6075.6,5670.5,5267,4867.6,4475.6,4096.4,3733,3399.4,3098.6,2840.9,2648.4,
   2531,2492.8,2541.8,2673.8,2869.2,3115,3403.2,3715.2,4044.4,4386,4732,
   5082,5430.6,5775.6,6116.7,6451,6776.8,7094.8,7403.5,7699.4,7985,8259.4,
   8521.1,8767,9000.5,9220,9422.4,9608.8,9781.9,9936.2,10075,10196,10302,
   10388,10458,10513,10548,10566,10569,10556,10528,10484,10427,10355,
   10271,10176,10070,9953.8,9829.7,9698.8,9560,9418.8,9272.7,9124.6,8975.2,
   8826,8679.4,8535.6,8395.7,8261,8133,8013,7902.1,7801.8,7711.8,7633,
   7568.8,7517,7477.8,7455.2,7447.5,7454.2,7475.5,7511.6,7562.1,7625,7703.5,
   7792.2,7892.1,8003.8,8122.5,8250,8383.7,8523.6,8667.4,8814,8963.2,9112,
   9261.5,9409,9556,9699,9839.5,9974.8,10106,10232,10353,10467,10576,
   10676,10771,10861,10943,11018,11088,11153,11209,11261,11308,11350,
   11386,11418,11446,11471,11491,11511,11525,11538,11550,11560,11567,
   11573,11579,11583,11586,11589,11590,11592,11593,11593,11593,11592,
   11591,11590,11588,11585,11580,11576,11570,11563,11555,11544,11532,
   11518,11501,11481,11458,11431,11401,11366,11328,11284,11234,11179,
   11119,11051,10978,10899,10815,10722,10624,10518,10407,10290,10166,
   10038,9905.5,9767.8,9626.9,9482.6,9336.5,9188.8,9040.7,8893,8745.6,8601,
   8459.2,8321.4,8189,8062,7940.5,7824.6,7717,7615,7520.3,7432,7349.1,
   7270.8,7196.7,7126,7056.5,6987.8,6916.5,6844.2,6768,6687,6598.7,6503.4,
   6399.2,6285.4,6161,6027,5878,5718.8,5547.5,5362,5164.3,4954.6,4733.3,
   4501.4,4260,4011.4,3756.3,3500.2,3245.5,2996,2764.6,2557.6,2384.6,2269.6,
   2218.5,2240.2,2340.4,2516.4,2749.5,3027,3348,3695,4061.5,4444,4836.5,
   5234.6,5638.1,6042.4,6447.4,6849,7247.9,7639.4,8026.3,8404.4,8774,9133.2,
   9482.1,9817.8,10140,10452,10746,11026,11292,11539,11770,11984,12181,
   12361,12526,12672,12801,12914,13014,13097,13166,13225,13270,13304,
   13329,13347,13356,13361,13360,13357,13350,13342,13336,13329,13322,
   13317,13316,13318,13321,13326,13335,13346,13360,13375,13390,13406,
   13422,13436,13448,13457,13463,13465,13461,13452,13437,13415,13385,
   13351,13308,13259,13202,13140,13071,12996,12918,12836,12750,12662,
   12574,12482,12394,12306,12220,12137,12057,11982,11910,11845,11785,
   11729,11681,11636,11597,11564,11534,11509,11489,11473,11460,11449,
   11443,11438,11435,11437,11439,11444,11453,11464,11478,11496,11518,
   11545,11575,11611,11652,11698,11749,11806,11868,11934,12006,12081,
   12161,12244,12328,12415,12501,12586,12670,12750,12829,12901,12969,
   13029,13081,13127,13162,13190,13207,13213,13211,13199,13177,13144,
   13103,13054,12997,12933,12863,12788,12708,12625,12541,12455,12369,
   12285,12202,12123,12046,11972,11905,11843,11786,11736,11691,11651,
   11619,11592,11569,11553,11542,11536,11532,11535,11540,11549,11562,
   11578,11597,11620,11646,11675,11708,11744,11783,11825,11873,11924,
   11977,12035,12097,12162,12230,12301,12374,12450,12526,12603,12681,
   12759,12836,12911,12983,13052,13117,13179,13235,13284,13327,13362,
   13390,13411,13425,13429,13426,13415,13394,13367,13333,13291,13242,
   13188,13128,13062,12995,12923,12847,12771,12694,12616,12538,12462,
   12386,12311,12241,12172,12107,12044,11985,11931,11880,11833,11789,
   11749,11712,11680,11649,11624,11600,11581,11564,11551,11541,11535,
   11532,11535,11540,11551,11567,11587,11614,11646,11683,11727,11776,
   11831,11893,11959,12029,12103,12179,12260,12340,12421,12501,12580,
   12652,12722,12785,12842,12889,12924,12949,12961,12960,12943,12910,
   12861,12795,12710,12606,12484,12344,12182,12004,11807,11590,11355,
   11103,10834,10549,10248,9932.9,9603.4,9261.1,8909,8543.5,8168.8,7787.4,
   7397.4,7001,6600.6,6197.9,5793.6,5389.3,4989,4593.3,4207.6,3833.5,3478,
   3148.5,2851.8,2597.8,2402.4,2271.3,2213,2240.5,2334.8,2485.4,2680.6,2905,
   3147,3399.9,3657.8,3912.5,4165,4409.9,4645.4,4870,5082.8,5285,5474.4,
   5650.8,5813.6,5964.8,6104,6230.3,6345.6,6451.1,6547.6,6634.5,6715,6790.4,
   6860.2,6928.4,6995,7061.3,7129.4,7200.5,7276.4,7359,7448,7548.1,7655.8,
   7773.1,7900,8039.9,8189.8,8348.6,8519.2,8698,8884.8,9078.3,9279.2,9486,
   9695,9908.8,10125,10340,10556,10769,10980,11185,11385,11580,11768,
   11945,12116,12276,12425,12562,12687,12802,12902,12991,13067,13129,
   13178,13215,13240,13253,13254,13245,13225,13196,13158,13113,13060,
   13001,12939,12870,12798,12724,12648,12571,12495,12418,12342,12267,
   12193,12122,12053,11984,11919,11854,11791,11729,11667,11606,11543,
   11480,11413,11347,11274,11198,11119,11033,10941,10844,10743,10634,
   10518,10395,10269,10135,9997,9853.4,9707.2,9556.7,9405,9250,9096.6,
   8943.3,8791.6,8642.9,8497,8357.5,8224,8096.7,7979,7871.5,7772.8,7686.1,
   7612.6,7551.9,7503,7469.9,7452,7448,7459.4,7485.5,7526,7579.2,7649.2,
   7730.2,7822,7926.2,8039,8160.5,8290.6,8426,8566.2,8711.2,8858,9006.8,
   9155,9302.9,9448.6,9592.9,9732.2,9868.5,10000,10126,10244,10355,10458,
   10554,10641,10721,10790,10848,10899,10937,10965,10982,10989,10983,
   10967,10940,10903,10852,10791,10719,10638,10547,10446,10334,10216,
   10090,9957.8,9818,9675,9526.5,9375.4,9222.1,9068,8913.9,8762.4,8613.4,
   8467.2,8327,8191.6,8064.7,7945.4,7833.9,7734,7646.1,7569.6,7504.8,7456,
   7421.5,7400.6,7396.7,7409.4,7437.2,7482,7542.8,7620.4,7713.5,7821.4,7945,
   8082.2,8232.2,8394,8567.2,8749,8940.6,9140,9345.1,9554.8,9768,9983.6,
   10201,10418,10633,10845,11053,11258,11455,11646,11827,12000,12164,
   12315,12455,12584,12699,12801,12890,12965,13025,13071,13104,13122,
   13128,13122,13103,13073,13034,12983,12925,12859,12787,12711,12630,
   12547,12462,12377,12292,12210,12129,12053,11980,11911,11849,11791,
   11741,11695,11654,11621,11594,11571,11554,11543,11535,11532,11535,
   11540,11548,11561,11577,11595,11617,11644,11672,11704,11740,11779,
   11822,11868,11917,11971,12029,12089,12154,12219,12289,12359,12432,
   12506,12580,12653,12723,12792,12857,12920,12974,13023,13065,13097,
   13118,13129,13130,13115,13087,13045,12984,12910,12818,12708,12580,
   12435,12272,12089,11889,11673,11436,11184,10916,10630,10330,10016,
   9687.8,9344.8,8991.5,8629,8254.4,7870.6,7480.1,7085,6682,6278.2,5872.9,
   5468.6,5067.8,4675,4296.2,3933.4,3595.5,3295.8,3045.5,2854.4,2739.8,2718.6,
   2780.4,2923,3144.7,3419.6,3735.9,4084.2,4455.5,4841.2,5236.9,5639.4,6043.7,
   6448,6849.7,7249,7642.8,8027.6,8405.5,8775.4,9134,9481.8,9817.5,10141,
   10450,10745,11026,11291,11538,11773,11991,12189,12374,12543,12692,
   12827,12948,13052,13143,13220,13285,13337,13380,13413,13437,13454,
   13465,13470,13473,13472,13468,13464,13459,13454,13451,13448,13447,
   13447,13450,13453,13457,13463,13469,13475,13481,13483,13484,13481,
   13474,13462,13443,13417,13383,13340,13286,13223,13148,13062,12964,
   12856,12734,12600,12455,12299,12131,11955,11767,11571,11369,11160,
   10945,10728,10506,10283,10061,9837.8,9618.7,9402.4,9192.5,8988.2,8791.4,
   8603.6,8423.8,8255,8097.5,7952.2,7815.4,7693.4,7581.5,7480.8,7388.6,7307.4,
   7234.2,7169,7107.8,7051.2,6998.9,6945,6893.5,6838.8,6781.2,6717.6,6648.5,
   6573,6487.7,6392.4,6287.3,6170,6041.5,5901,5746.4,5579,5398.6,5207,
   5000.8,4782.8,4554.7,4316,4067.5,3814.2,3559.2,3302.6,3052.7,2814,2602,
   2422,2288.3,2221.6,2230,2312,2468.1,2690.6,2961.7,3270,3613.5,3977,
   4354.7,4744.2,5143,5544.2,5949,6352.4,6754.6,7153,7546.7,7932.4,8311,
   8680.6,9038.5,9386.8,9721.9,10042,10348,10638,10909,11164,11401,11617,
   11812,11986,12138,12267,12373,12457,12515,12549,12560,12545,12507,
   12446,12360,12250,12118,11966,11788,11590,11374,11137,10882,10609,
   10320,10013,9692,9359,9011.1,8652.6,8283.6,7905,7518,7123.8,6726.1,
   6323.6,5918.6,5514,5111.9,4715.2,4326.7,3955.6,3603,3278.4,2992.1,2761.6,
   2594,2500,2503.3,2585.4,2738.9,2959,3223.5,3520,3839.7,4175.4,4519.6,
   4868,5218.1,5565.8,5911.5,6250.6,6583.5,6908.4,7223.8,7530.8,7826.8,8113,
   8387.1,8649.6,8901.7,9138.4,9364,9576.8,9778.6,9965.6,10141,10304,10455,
   10595,10724,10843,10952,11054,11146,11233,11312,11388,11459,11527,
   11592,11656,11719,11781,11846,11911,11977,12046,12116,12187,12261,
   12337,12415,12495,12575,12656,12735,12815,12894,12970,13045,13116,
   13185,13248,13306,13362,13409,13453,13491,13522,13547,13568,13582,
   13590,13595,13595,13591,13584,13573,13562,13549,13536,13522,13508,
   13495,13485,13476,13470,13467,13466,13467,13471,13477,13486,13497,
   13510,13524,13538,13551,13563,13575,13585,13592,13596,13595,13589,
   13580,13565,13543,13517,13486,13446,13403,13354,13298,13239,13175,
   13105,13034,12959,12883,12804,12724,12644,12564,12484,12406,12329,
   12255,12182,12113,12046,11981,11919,11861,11803,11749,11697,11646,
   11596,11548,11499,11451,11403,11356,11309,11259,11210,11161,11112,
   11063,11015,10967,10920,10877,10835,10797,10764,10734,10709,10689,
   10675,10668,10666,10670,10681,10697,10719,10747,10780,10815,10854,
   10897,10942,10989,11036,11085,11134,11184,11233,11281,11330,11378,
   11426,11474,11522,11570,11619,11669,11720,11775,11830,11888,11948,
   12011,12076,12145,12216,12290,12364,12441,12520,12601,12681,12761,
   12841,12918,12995,13067,13137,13204,13266,13324,13377,13424,13466,
   13501,13531,13554,13573,13585,13594,13596,13594,13590,13581,13571,
   13558,13544,13531,13517,13505,13493,13483,13476,13470,13467,13468,
   13471,13478,13485,13497,13511,13527,13542,13561,13579,13598,13614,
   13631,13646,13659,13670,13676,13682,13683,13682,13677,13671,13660,
   13647,13633,13617,13600,13582,13563,13546,13530,13514,13501,13489,
   13480,13474,13470,13470,13473,13479,13488,13500,13513,13529,13545,
   13562,13580,13599,13616,13632,13646,13659,13670,13677,13682,13683,
   13682,13677,13670,13660,13647,13631,13615,13598,13580,13562,13543,
   13526,13510,13495,13482,13473,13464,13458,13455,13454,13455,13456,
   13459,13462,13464,13466,13464,13460,13452,13438,13418,13388,13351,
   13304,13247,13175,13091,12994,12882,12754,12611,12451,12272,12078,
   11870,11641,11397,11139,10863,10572,10267,9947.7,9615.6,9270.7,8916,
   8548.8,8174.8,7791.6,7400.2,7003.5,6602.8,6198,5794.2,5390,4989,4593.7,
   4207.8,3833.6,3478,3148.5,2851.8,2596.8,2401.8,2271.3,2213,2240.8,2336.4,
   2484.7,2680.6,2905,3147.8,3399.9,3658.2,3914.2,4166,4409.9,4644.8,4870.8,
   5085.6,5287,5476.4,5653.6,5815.4,5966.4,6105,6230.3,6345,6448.3,6540.2,
   6623.5,6697.2,6763.8,6823.4,6875.3,6922,6964.3,7002.8,7037.4,7067.4,7095.5,
   7120.4,7141.6,7161.6,7177,7188,7194.6,7195.4,7189.6,7176.2,7155.5,7125.2,
   7085.1,7032,6968.6,6893,6801.2,6696.4,6577.4,6443.4,6293,6129.8,5952.3,
   5758.8,5553.6,5338,5108,4871.6,4628.8,4382.2,4137.5,3899.2,3671.9,3467.8,
   3290.8,3150,3066,3036.8,3066.6,3164,3323.5,3534.2,3791.2,4087,4410.9,
   4756,5119.8,5495.2,5878.4,6267,6657,7047,7433.5,7815.4,8191.6,8561,
   8919.6,9269,9606.3,9932.6,10245,10543,10825,11089,11339,11572,11783,
   11976,12151,12305,12438,12553,12648,12718,12770,12803,12812,12804,
   12778,12731,12666,12586,12488,12372,12245,12104,11948,11782,11606,
   11421,11227,11027,10822,10612,10400,10185,9971.5,9758.8,9548.2,9341.6,
   9139,8942,8753.8,8571.6,8399.7,8237,8085.1,7942.4,7810.9,7690.6,7580.5,
   7477.6,7385.4,7301.2,7223.4,7151,7081.7,7014.8,6949,6882.4,6812.5,6740.2,
   6662.4,6575.4,6482.2,6382,6270.4,6147.8,6014.5,5868.6,5712.5,5543.6,5363.4,
   5170.4,4967.4,4755,4533.8,4305.2,4072,3837,3603.5,3375.6,3160.4,2963.6,
   2793.6,2656,2567.2,2527.2,2539.1,2610.2,2729.5,2890.4,3084.8,3308,3546.9,
   3797,4054.7,4312.2,4569.2,4818,5061.5,5293.8,5516.2,5724.4,5920.9,6103,
   6269,6420.8,6558.7,6681.4,6789.5,6884,6966.4,7034.6,7093.4,7142,7181.1,
   7214.4,7241.1,7265.4,7287,7307.8,7330.9,7356.8,7388.2,7427,7473.7,7528.6,
   7596.5,7676.6,7769,7873.4,7991.9,8123.4,8267.5,8425,8594.5,8774.2,8964.2,
   9161.4,9367,9579,9794.8,10014,10235,10456,10676,10894,11107,11315,
   11518,11712,11898,12072,12236,12391,12530,12657,12772,12874,12960,
   13033,13092,13137,13169,13189,13196,13190,13175,13149,13114,13072,
   13021,12964,12902,12836,12765,12693,12620,12545,12470,12396,12324,
   12253,12185,12120,12058,11998,11943,11891,11843,11798,11757,11720,
   11687,11656,11629,11606,11585,11567,11554,11543,11537,11533,11534,
   11537,11548,11563,11582,11608,11638,11674,11717,11766,11819,11879,
   11943,12013,12087,12165,12244,12326,12408,12490,12570,12649,12721,
   12791,12854,12909,12955,12991,13018,13031,13034,13022,12996,12955,
   12901,12832,12747,12647,12535,12406,12266,12113,11947,11770,11584,
   11388,11185,10975,10761,10542,10321,10098,9876,9655.6,9437.7,9223.8,
   9016.5,8815.6,8622.7,8440,8268.7,8105,7958.6,7823,7700.2,7593.4,7502,
   7424.2,7361.4,7317,7285,7270,7270.1,7285.2,7314.5,7356.2,7412.5,7481.6,
   7560.9,7653,7756.1,7867,7988.1,8116,8251.4,8393.4,8540,8689.8,8842.9,
   8998,9154,9310,9463.7,9616.6,9765.6,9912.6,10052,10189,10320,10444,
   10561,10671,10773,10867,10956,11036,11109,11175,11235,11287,11334,
   11375,11411,11442,11469,11492,11511,11527,11542,11553,11562,11569,
   11574,11580,11584,11586,11587,11588,11586,11586,11584,11581,11576,
   11571,11564,11556,11547,11535,11522,11504,11487,11464,11438,11408,
   11375,11337,11294,11246,11193,11132,11068,10997,10919,10834,10744,
   10646,10543,10431,10316,10194,10066,9934,9796.6,9655,9511.1,9364.2,
   9215.5,9066,8916.6,8768.8,8623.3,8480,8341.8,8209.6,8084.5,7968,7860.5,
   7764.2,7677.2,7605,7545.6,7499,7467.9,7451.2,7448.9,7462.4,7490,7532,
   7587.1,7657,7738.5,7832,7936.5,8051.2,8174.4,8305.8,8443,8586.6,8734.4,
   8884.2,9036.9,9190,9341.4,9493,9641.6,9786.6,9926,10061,10187,10308,
   10421,10525,10617,10702,10776,10839,10892,10932,10963,10981,10988,
   10985,10970,10944,10909,10861,10804,10738,10662,10576,10483,10381,
   10272,10155,10032,9903.6,9769.5,9633,9490.1,9347.6,9203.3,9058,8914,
   8772.2,8633.6,8501.2,8374.5,8255,8145.4,8049,7963.6,7890,7836.2,7796.6,
   7774.4,7770.4,7785.5,7819.8,7872.3,7945.2,8036.2,8144,8270.5,8411.8,8566.8,
   8737.2,8917,9107.2,9306.8,9513.4,9724.8,9941,10160,10377,10593,10807,
   11017,11221,11415,11602,11779,11944,12095,12232,12353,12456,12543,
   12611,12661,12689,12696,12684,12649,12592,12514,12414,12292,12150,
   11987,11802,11599,11377,11135,10875,10599,10307,9999.5,9678,9344.5,
   8996.8,8639.1,8272,7896.4,7513.8,7126.8,6734.4,6340.5,5946.6,5554.2,5167.6,
   4787,4417,4065.1,3731.6,3423.2,3150.2,2918,2732.2,2601.6,2532.8,2520.9,
   2560,2649.5,2770.2,2912.7,3069.8,3231.5,3393.2,3547.3,3691.8,3823.2,3939,
   4033.5,4110.2,4167.9,4203,4216.5,4208.4,4180.4,4128.8,4058.7,3972,3864,
   3742.8,3608.6,3463.8,3312,3158.6,3007.7,2864,2735.2,2625,2547.2,2501.2,
   2490.2,2520.8,2589.5,2689.4,2818.1,2967.2,3131.3,3302,3477.2,3650.2,3817.3,
   3973.8,4118.5,4248.8,4362.5,4455.6,4530.4,4586,4615.6,4625.2,4615.2,4581.4,
   4526,4452.2,4359.5,4245.8,4119.3,3979,3827.7,3668.6,3505.6,3345.8,3191.5,
   3048.4,2925,2828.2,2760.2,2725,2730.9,2772.8,2846.7,2952,3080.5,3225.8,
   3382.4,3544,3705.1,3863,4011.3,4149,4272.7,4379.4,4467.5,4537.2,4586.3,
   4611.4,4615.9,4600,4557.4,4495,4411.8,4307.6,4183,4040,3883.1,3712.2,
   3532,3346,3158.1,2976.4,2805.9,2659.8,2547,2475.8,2455.1,2496,2592,
   2740,2942.5,3181.4,3450.7,3744.2,4054.5,4378.4,4709.7,5044.6,5381.3,5719,
   6052.5,6382.2,6705.3,7021.8,7330,7627.8,7916.5,8192.8,8457.7,8711,8949.1,
   9174.4,9385.1,9580.6,9761.5,9926.4,10075,10208,10323,10422,10503,10567,
   10614,10643,10654,10649,10624,10583,10525,10451,10356,10247,10122,
   9978.2,9819,9645.4,9455.4,9249,9028.9,8795,8547.6,8286.6,8014.2,7728.8,
   7432.5,7128.2,6813.3,6491,6162.5,5830,5495.9,5161.2,4828.5,4500.8,4184,
   3880,3592.7,3334.8,3105.8,2913,2770.2,2675.8,2631.4,2640.4,2694.5,2783,
   2901.4,3040.6,3188.8,3342,3492.2,3636.2,3769,3887.6,3990.5,4076.6,4142.8,
   4187,4211.1,4214,4192.9,4151.2,4089.2,4005.8,3902,3781.4,3644.5,3492.2,
   3328,3155,2978.2,2802,2631,2475.2,2341.5,2237.4,2172,2158.4,2193,
   2273,2404.4,2570.4,2763.7,2979.6,3209,3448,3690.7,3933.8,4174.1,4410,
   4638.3,4857,5066.1,5264.4,5451.5,5627.4,5790.7,5940.8,6080.1,6208,6322.2,
   6427.8,6522.9,6607.2,6683,6750,6810.3,6864.2,6912.9,6956,6993.4,7028.6,
   7060.6,7089,7114,7137.4,7157.3,7173.4,7184.9,7194,7195.7,7192,7180.1,
   7161.2,7133,7094.4,7045.5,6983.8,6909.3,6823,6720.8,6605,6474.3,6328,
   6167.5,5991.8,5800.6,5597.2,5380.3,5152,4911.9,4664.6,4410.6,4153.4,3896,
   3641.8,3397.2,3169.4,2961.6,2784,2650.4,2561.8,2523.9,2545,2618.5,2736.8,
   2893.1,3082.4,3292.2,3515,3747,3981.6,4216.9,4447.8,4672,4889,5096.3,
   5292.8,5479.7,5655,5815.7,5966.2,6106.4,6234.2,6350.5,6458,6554.9,6643,
   6724,6799,6869,6934,6996.3,7055.2,7113.5,7171,7228.5,7287.4,7347.2,
   7409,7471.7,7536,7601.9,7669.2,7735,7801.6,7865.8,7927.4,7986.5,8041,
   8091.5,8135.8,8174.2,8205,8228.5,8246.4,8255.2,8256.2,8249.4,8236,8217.3,
   8190.8,8160.4,8124.4,8087,8047.2,8007.9,7968.8,7933.8,7903,7877.5,7860.4,
   7850.6,7851.4,7864,7887.8,7923.5,7972.2,8033,8104,8190.3,8286.2,8391.7,
   8505.4,8627,8755,8888.5,9025.4,9164.6,9307,9447.7,9588.8,9726.5,9862.6,
   9995,10123,10248,10366,10478,10585,10685,10780,10869,10950,11025,
   11094,11157,11214,11265,11311,11352,11388,11421,11449,11473,11493,
   11511,11526,11539,11551,11559,11567,11574,11577,11581,11584,11586,
   11587,11588,11586,11585,11582,11578,11572,11566,11557,11547,11535,
   11520,11501,11479,11453,11423,11387,11345,11297,11243,11180,11108,
   11029,10938,10837,10726,10605,10470,10323,10164,9992.8,9807.8,9611,
   9401.6,9177.8,8943.2,8695.4,8435,8164.2,7882.7,7590,7288.2,6979,6659.6,
   6334.2,6003.5,5668,5330.5,4993,4658.5,4328.6,4006.5,3697,3407.4,3142.6,
   2907.5,2716,2575,2485.2,2454.3,2484,2562.1,2680,2831.4,3003.2,3185.8,
   3373.8,3560,3739.8,3909.1,4063.6,4202.7,4324,4425.4,4506.6,4566.8,4603.2,
   4617,4610,4580.3,4527.4,4456,4366,4254.1,4127.8,3988.9,3838,3680.5,
   3519,3357.8,3202.4,3059.8,2933,2834.1,2763.6,2726.2,2728.6,2769,2840.6,
   2942.5,3070.4,3214.5,3370,3531.8,3693.8,3851.9,4002.8,4140.5,4265.4,4375.4,
   4466.4,4538.7,4592,4620.9,4630,4618.3,4584.4,4529,4455.2,4361.6,4249.2,
   4123.3,3984,3836.6,3682,3526,3372.8,3231.5,3104.6,3003,2933,2897.1,
   2900,2951.7,3042.8,3170.5,3333,3522,3731,3952.9,4186.4,4423.8,4662,
   4897.7,5126.8,5349.9,5562.4,5763,5953.2,6128.7,6289.8,6437,6571,6687.2,
   6791.6,6883.2,6959.2,7022.5,7074.6,7114.7,7146.6,7169.2,7185,7194.8,7200.2,
   7202.8,7204.2,7205.5,7207.6,7213.8,7224.6,7239.8,7262,7293.1,7331.8,7379.2,
   7437.8,7504.5,7581.6,7669.2,7766,7872.1,7986,8110.4,8240.2,8376.4,8518.4,
   8665,8814.8,8966.7,9119.6,9272.2,9425,9573.9,9720.2,9862.2,9999.4,10129,
   10253,10370,10478,10575,10665,10743,10810,10869,10916,10950,10974,
   10986,10987,10978,10958,10926,10884,10833,10770,10698,10618,10528,
   10431,10325,10213,10093,9968.2,9838.1,9703,9566,9426.6,9285.1,9144.2,
   9004.4,8867,8732.3,8605.4,8484.2,8372.8,8271,8181.2,8105,8044.4,8000
};


long QMSL_Linear_Predistortion_NV::numEnvSamples = ENV_IN_NUM_SAMPLES;
long QMSL_Linear_Predistortion_NV::numEnvSamplesSwapped = ENV_IN_NUM_SAMPLES_SWAPPED;


