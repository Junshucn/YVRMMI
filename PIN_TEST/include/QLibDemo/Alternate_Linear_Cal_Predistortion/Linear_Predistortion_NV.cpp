/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/Alternate_Linear_Cal_Predistortion_NV_Method/Linear_Predistortion_NV.cpp#5 $
 * $DateTime: 2016/04/04 17:51:51 $
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
#include <list>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
using namespace std;

#define PI 3.1415926538
#define NUMELEMS( arr ) ((sizeof(arr))/(sizeof(arr[0])))


#include "Linear_Predistortion_NV.h"
#define DEBUG_PREDIST_PHASEDRIFT
#define DEBUG_PREDIST_XCORR
#define DEBUG_PREDIST_AMAM_GAIN
#define DEBUG_PREDIST_AMPM_GAIN
#define DEBUG_SMOOTHING
#define DEBUG_AMAM
#define DEBUG_AMPM
#define DEBUG_PREDIST_IQFFT


long numEnvSamples = ENV_IN_NUM_SAMPLES;
long numEnvSamplesSwapped = ENV_IN_NUM_SAMPLES_SWAPPED;


int ComputeFFT(const int isign, int numSamples_In,
               double * m_pDI, double * m_pDQ, 
               double * m_pFFTI, double * m_pFFTQ)        
{
   unsigned long n, mmax, m, j, istep, i;
   double wtemp, wr, wpr, wi, wpi, theta,
      tempr, tempi;

   unsigned long nn = 32768;
   static double data[65537];//size 2*nn+1
   memset(data,0,65537*sizeof(double));

   for (i = 1; i <= nn; i++)
   {
      data[2*i - 1] = 0.0;//zero out buffer
      data[2*i] = 0.0;//zero out buffer

      if(i<numSamples_In)
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
            tempr = wr * data[j] - wi * data[j+1];
            tempi = wr * data[j+1] + wi * data[j];
            data[j] = data[i] - tempr;
            data[j+1] = data[i+1] - tempi;
            data[i] += tempr;
            data[i+1] += tempi;
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
   return 0;
}


float estimateAndCorrectPhaseDrift(float * PhaseVec,long estStart,long estEnd, long corrStart,long corrEnd)
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
   fnamestr << "C:\\Phase_Unwrap_" << i << ".csv";

   ofstream phUnWrapFile;
   phUnWrapFile.open (fnamestr.str().c_str(),ios::out);
   phUnWrapFile << "Index,Ph_in,Ph_unWrap"<< endl;
#endif

   //Actual Unwrap (if diff > 170*2)
   for(loopVar = min_start+1;loopVar<=max_end;loopVar++)
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



   for(loopVar = estStart;loopVar<estEnd;loopVar++)
   {
      phaseDiffAccum = phaseDiffAccum + (PhaseVec[loopVar+1] - PhaseVec[loopVar]);
      slopeEstimationLength++;
   }

   float averageSlopeChange = phaseDiffAccum / slopeEstimationLength;

#ifdef DEBUG_PREDIST_PHASEDRIFT
   fnamestr.str("");
   fnamestr << "C:\\Phase_Corr_" << i << ".csv";

   ofstream phCorrFile;
   phCorrFile.open (fnamestr.str().c_str(),ios::out);
   phCorrFile << "Ph_in,Ph_out"<< endl;
#endif

   for(loopVar = corrStart;loopVar<corrEnd;loopVar++)
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

long crossCorrelate(float * CVec1,const float * CVec2, long CVec1Size, long CVec2Size)
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
   fnamestr1 << "C:\\Cross_Corr_Inps_v1_" << i << ".csv";
   fnamestr2 << "C:\\Cross_Corr_Inps_v2_" << i << ".csv";

   ofstream xCorrInFile1,xCorrInFile2;
   xCorrInFile1.open (fnamestr1.str().c_str(),ios::out);
   xCorrInFile2.open (fnamestr2.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < CVec1Size;loopVar++)
   {
      xCorrInFile1 << CVec1[loopVar] << endl;
   }
   for(loopVar = 0;loopVar < CVec2Size;loopVar++)
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

   for(loopVar = 0;loopVar < corrVecLen;loopVar++)
   {
      if(loopVar < sizeLargerVec)
      {
         corrVec[loopVar] = inner_product(Vec1BeginIter,Vec1EndIter,Vec2BeginIter,(float)0.0);
         Vec1EndIter++;
         Vec2BeginIter--;
      }
      if(loopVar >= sizeLargerVec)
      {
         if(loopVar == sizeLargerVec)
         {
            Vec2BeginIter = tempVec2.begin();
            Vec1EndIter   = tempVec1.end();
         }
         Vec1BeginIter++;
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
   fnamestr3 << "C:\\Cross_Corr_Op_" << i << ".csv";
   fnamestr4 << "C:\\Cross_Corr_Delay" << i << ".csv";

   ofstream xCorrOutFile1,xCorrOutFile2;
   xCorrOutFile1.open (fnamestr3.str().c_str(),ios::out);
   xCorrOutFile2.open (fnamestr4.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < corrVec.size();loopVar++)
   {
      xCorrOutFile1 << corrVec[loopVar] << endl;
   }
   xCorrOutFile2 << "Delay : " << (largestInd - sizeVec1) << endl;
   xCorrOutFile1.close();
   xCorrOutFile2.close();
#endif

   return (largestInd - sizeVec1);
}

long crossCorrelateIQFFT
(float * CVec1,const float * CVec2, long CVec1Size, long CVec2Size)
{
   if(CVec1Size > CVec2Size) CVec1Size = CVec2Size;

   unsigned int n=CVec1Size;
   unsigned int np = 32768;

   static double f0[32768];
   static double f1[32768];
   static double f2[32768];

   static double g1_i[32768];
   static double g1_q[32768];
   static double g2_i[32768];
   static double g2_q[32768];

   static double f_i[32768];
   static double f_q[32768];
   static double g_i[32768];
   static double g_q[32768];

   memset(f0,0,32768*sizeof(double));
   memset(f1,0,32768*sizeof(double));
   memset(f2,0,32768*sizeof(double));
   memset(g1_i,0,32768*sizeof(double));
   memset(g1_q,0,32768*sizeof(double));
   memset(g2_i,0,32768*sizeof(double));
   memset(g2_q,0,32768*sizeof(double));
   memset(f_i,0,32768*sizeof(double));
   memset(f_q,0,32768*sizeof(double));
   memset(g_i,0,32768*sizeof(double));
   memset(g_q,0,32768*sizeof(double));

   unsigned int i=0;

   for(i=0; i < n; i++) 
   {
      f0[i]=0.0;
      f1[i]=CVec2[i];
      f2[i]=CVec1[i];
   }

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
   static int k=0;
   k++;
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\IQFFT_Op_" << k << ".csv";

   ofstream xCorrOutFile1;
   xCorrOutFile1.open (fnamestr1.str().c_str(),ios::out);
   for(unsigned int loopVar = 0;loopVar < n;loopVar++)
   {
      xCorrOutFile1 << f_i[loopVar] << "," <<f_q[loopVar]<< "," 
         << sqrt(f_i[loopVar]*f_i[loopVar]+f_q[loopVar]*f_q[loopVar]) << endl;
   }
   xCorrOutFile1.close();
#endif

   return Index;
}

void computeGain(const float * InPwr,float * MeasPwr, unsigned long VecLength, 
                 unsigned long skipSamples, vector<float>& inPwr, vector<float>& gain)
{
   multimap<long, float> Pwr_Gain;
   set<long> PwrSet;
   unsigned int loopVar = 0;
   
   printf("Vec Length = %d\n",VecLength);

   float minInPwr,maxInPwr;
   minInPwr = 20*log10(*min_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));
   maxInPwr = 20*log10(*max_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));
   

#ifdef DEBUG_PREDIST_AMAM_GAIN
   static int i = 0;
   i++;
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\AMAM_GAIN_Inputs_" << i << ".csv";

   ofstream amamGainIpFile;
   amamGainIpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < VecLength;loopVar++)
   {
      amamGainIpFile << InPwr[loopVar] << "," << MeasPwr[loopVar]<< endl;
   }
   amamGainIpFile.close();
#endif


   float InPwrdBm;
   float measPwrdBm;
   float in_pwr_dBmx10;
   long  in_pwr_rounded;
   printf("ComputeGain First Loop Start");
   for(loopVar = skipSamples;loopVar<(VecLength-skipSamples);loopVar++)
   {
      InPwrdBm = 20*log10(InPwr[loopVar]); 
      measPwrdBm = MeasPwr[loopVar];
      in_pwr_dBmx10 = (float)(10.0*(InPwrdBm+0.05)); //Add 0.05 to help round
      in_pwr_rounded = (long)in_pwr_dBmx10;

      if( (in_pwr_rounded/10.0 > minInPwr) && (in_pwr_rounded/10.0 < maxInPwr))
      {
         Pwr_Gain.insert(pair<long,float>(in_pwr_rounded, measPwrdBm-InPwrdBm));
         PwrSet.insert(in_pwr_rounded);
      }
   }
   printf("ComputeGain First Loop Done");

   set<long>::iterator pwrIter;
   pair<multimap<long, float>::iterator, multimap<long, float>::iterator> startStopGains;
   multimap<long, float>::iterator pwrGainIter;

   float sumGain;
   float meanGain;
   for(pwrIter=PwrSet.begin();pwrIter!=PwrSet.end();pwrIter++)
   {
      startStopGains = Pwr_Gain.equal_range(*pwrIter);

      sumGain = 0.0;

      for(pwrGainIter = startStopGains.first; pwrGainIter != startStopGains.second;pwrGainIter++)
      {
         sumGain = sumGain + (*pwrGainIter).second;
      }
      meanGain = sumGain/Pwr_Gain.count(*pwrIter);
      inPwr.push_back((float)((*pwrIter)/10.0));
      gain.push_back(meanGain);
   }
   printf("ComputeGain Second Loop Done");


#ifdef DEBUG_PREDIST_AMAM_GAIN
   fnamestr1.str("");
   fnamestr1 << "C:\\AMAM_GAIN_Outputs_" << i << ".csv";

   ofstream amamGainOpFile;
   amamGainOpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < inPwr.size();loopVar++)
   {
      amamGainOpFile << inPwr[loopVar] << "," << gain[loopVar]<< endl;
   }
   amamGainOpFile.close();
#endif

}

void computePhase(const float * InPwr, float * MeasPhase, unsigned long VecLength
                  , unsigned long skipSamples, vector<float>& inPwr, vector<float>& phase)
{
   multimap<long, float> Pwr_Phase;
   set<long> PwrSet;
   unsigned int loopVar =0;

   float minInPwr,maxInPwr;
   minInPwr = 20*log10(*min_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));
   maxInPwr = 20*log10(*max_element(&InPwr[skipSamples],&InPwr[VecLength-skipSamples]));

#ifdef DEBUG_PREDIST_AMPM_GAIN
   static int i = 0;
   i++;
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\AMPM_GAIN_Inputs_" << i << ".csv";

   ofstream ampmGainIpFile;
   ampmGainIpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < VecLength;loopVar++)
   {
      ampmGainIpFile << InPwr[loopVar] << "," << MeasPhase[loopVar]<< endl;
   }
   ampmGainIpFile.close();
#endif


   float InPwrdBm;
   float PrevInPwrdBm;
   float in_pwr_dBmx10;
   long  in_pwr_rounded;
   float estPhase;
   for(loopVar = skipSamples;loopVar<(VecLength-skipSamples);loopVar++)
   {
      InPwrdBm = 20*log10(InPwr[loopVar]); 
      in_pwr_dBmx10 = (float)(10.0*(InPwrdBm+0.05)); //Add 0.05 to help round
      in_pwr_rounded = (long)in_pwr_dBmx10;

      //Pwr_Phase.insert(pair<long,float>(in_pwr_rounded, MeasPhase[loopVar]));
      if( (in_pwr_rounded/10.0 > minInPwr) && (in_pwr_rounded/10.0 < maxInPwr))
         PwrSet.insert(in_pwr_rounded);
   }

   set<long>::iterator pwrIter;
   float currBinInPwr;
   for(pwrIter=PwrSet.begin();pwrIter!=PwrSet.end();pwrIter++)
   {
      for(loopVar = skipSamples+1;loopVar<(VecLength-skipSamples);loopVar++)
      {
         currBinInPwr = (float)(*pwrIter)/10.0;         
         InPwrdBm = 20*log10(InPwr[loopVar]); 
         PrevInPwrdBm = 20*log10(InPwr[loopVar-1]); 
         if( (currBinInPwr >= PrevInPwrdBm) && (currBinInPwr <= InPwrdBm) || (currBinInPwr <= PrevInPwrdBm) && (currBinInPwr >= InPwrdBm))
         {
            estPhase = MeasPhase[loopVar] + 
               ((MeasPhase[loopVar-1]-MeasPhase[loopVar])*(currBinInPwr - InPwrdBm))/(PrevInPwrdBm - InPwrdBm);
            Pwr_Phase.insert(pair<long,float>(*pwrIter, estPhase));
         }
      }
   }

   //With new method set first and last phase distortion to be equal to first_but_one and last_but_one


   pair<multimap<long, float>::iterator, multimap<long, float>::iterator> startStopPhases;
   multimap<long, float>::iterator pwrPhaseIter;

   float sumPhase;
   float meanPhase;
   for(pwrIter=PwrSet.begin();pwrIter!=PwrSet.end();pwrIter++)
   {
      startStopPhases = Pwr_Phase.equal_range(*pwrIter);

      sumPhase = 0.0;

      for(pwrPhaseIter = startStopPhases.first; pwrPhaseIter != startStopPhases.second;pwrPhaseIter++)
      {
         sumPhase = sumPhase + (*pwrPhaseIter).second;
      }
      meanPhase = sumPhase/Pwr_Phase.count(*pwrIter);

      inPwr.push_back((float)((*pwrIter)/10.0));
      phase.push_back(meanPhase);
   }

#ifdef DEBUG_PREDIST_AMPM_GAIN
   fnamestr1.str("");
   fnamestr1 << "C:\\AMPM_GAIN_Outputs_" << i << ".csv";

   ofstream ampmGainOpFile;
   ampmGainOpFile.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < inPwr.size();loopVar++)
   {
      ampmGainOpFile << inPwr[loopVar] << "," << phase[loopVar]<< endl;
   }
   ampmGainOpFile.close();
#endif
}


void splsmooth(const vector<float>& x, vector<float>& y, int meassize,  const vector<float>& xknots, int NknotIntervals)
{
   int ns,ind2,ind1,kr,j,li,i,lg,currRow;
   float* R;float* A;float* coeffs;
   float * M;float* xxknots;
   float u = 0 ; float v = 0;float hj = 0;float sum;
   float *G;

   G = (float*)malloc(25*sizeof(float));
   ns = NknotIntervals+ 3;
   R = (float*)malloc(ns*5*sizeof(float));
   M = (float*)malloc(meassize*4*sizeof(float));
   ind1 = 0; ind2 = 0; kr = 0;li = 0;lg = 0;currRow = 0;

   memset( G, 0, 25*sizeof(float) );

   xxknots = (float*) malloc((NknotIntervals+5)*sizeof(float));


   xxknots[0] = xknots[0]; xxknots[1] = xknots[0] ;
   for(i = 0 ; i <= NknotIntervals ;i++)
   {
      xxknots[i+2] = xknots[i];
   }
   xxknots[NknotIntervals+3] = xknots[NknotIntervals];
   xxknots[NknotIntervals+4] = xknots[NknotIntervals];

   //Orthogonal transformation
   if(NknotIntervals == 1)
   {
      third_order( x, y, meassize);
      return;
   }
   else
   {
      for(j = 0; j <= NknotIntervals-1; j++)
      {
         ind1 = ind2;
         // Find the no of contributions due to interval j.
         while(ind2< meassize && x[ind2] <= xknots[j+1] ){ind2++;}
         li = ind2- ind1;
         if(li)
         {
            //NOTE : ind2 = one past last value in interval
            //Create A
            A = (float*)malloc((ind2-ind1+lg)*5*sizeof(float));
            memset((void *) A,0,(ind2-ind1+lg)*5*sizeof(float));

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
      for(i = 1 ; i < ns-kr;i++)
      {
         memcpy((void*)(R+(kr+i)*5),(const void *)(G+(i)*5+i),(4-i)*sizeof(float));
         memset((void *)(R+(kr+i)*5+4-i) ,0,i*sizeof(float));
         R[(kr+i)*5+4] = G[i*5+4];
      }
   }//if NknotIntervals > 1
   free(xxknots);
   free(G);

   //Get coefficients from R
   coeffs = (float *)malloc((ns+3)*sizeof(float));
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
   ind1 = 0 ; ind2 = 0;
   for(j = 0 ; j < NknotIntervals; j++)
   {
      ind1 = ind2;
      // Find the no of contributions due to interval j.
      while (ind2< meassize && x[ind2] <= xknots[j+1] ){ind2++;}
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


void qrfact(float* A, int rows, int cols)
{
   int i,j,k,p,z;
   float housec=0; float normv = 0; float normw=0.0; float beta=0; 
   float temp=0;float signvi = 0;

   float* H;
   float* tempA;

   for (i = 0; i <= cols-1;i++)
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
         //ASSERT(H != 0);
         //Fill H
         for (k = 0;k <= rows-z-1; k++)
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
         //ASSERT(tempA != 0 );

         for (k = z; k <= rows-1;k++)
         {
            for (j = z; j <=cols-1;j++)
            {
               temp = 0;
               for (p = z; p <=rows-1;p++)
                  temp = temp + H[(k-z)*(rows-z)+(p-z)]*A[p*cols+j];   
               tempA[(k-z)*(cols-z)+(j-z)] = temp;
            }
         }
         // copy A(i:end) = tempA;
         for( k = z; k <=rows-1; k++)
         {    for (j = z;j<=cols-1;j++)
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


void third_order(const vector<float>& x, vector<float>& y, int meassize)
{
   unsigned int row =0;
   unsigned int loopVar=0;
   // y = a + bx + cx^2 + dx^3
   float s_xny[4]; //0 - s_x0y,1 - s_xy, 2 - s_x2y
   float s_xn[7];  //0 - s_1, 1 - s_x  ,2 - s_x2, 6 - s_x6
   memset(s_xny,0,4*sizeof(float));
   memset(s_xn,0,7*sizeof(float));

   float a,b,c,d;

   float x_prods;
   unsigned int xpower;


   //Compute the elements of the matrix
   for( loopVar=0;loopVar<meassize;loopVar++)
   {
      x_prods = 1.0;
      for(xpower = 0;xpower < 4;xpower++)
      {
         s_xny[xpower] += pow(x[loopVar],(float)xpower) * y[loopVar];
      }
      for(xpower = 0;xpower < 7;xpower++)
      {
         s_xn[xpower] += pow(x[loopVar],(float)xpower);
      }
   }

   //Prepare the matrix
   float A[4][5];

   for( row = 0;row < 4;row ++)
   {
      for(unsigned int col = 0;col < 4;col ++)
      {
         A[row][col] = s_xn[row+col];
      }
   }

   for( row = 0;row < 4;row ++)
   {
      A[row][4] = s_xny[row];
   }

   qrfact(&(A[0][0]),4,5);

   d = (A[3][4])/A[3][3];
   c = (A[2][4] - d*A[2][3])/A[2][2];
   b = (A[1][4] - d*A[1][3] - c*A[1][2])/A[1][1];
   a = (A[0][4] - d*A[0][3] - c*A[0][2] -b*A[0][1])/A[0][0];

   //Now smooth
   for( loopVar=0;loopVar<meassize;loopVar++)
   {
      float x_ = x[loopVar];
      y[loopVar] = a + b*x_ + c*x_*x_ + d*x_*x_*x_;
   }
}



int choose_knots_x(const vector<float>& x, vector<float>& y, vector<float>& xknots,int meassize,float delta_x)
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

int choose_knots_y(const vector<float>& x, vector<float>& y, vector<float>& xknots,int meassize,float delta_y)
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

void smoothData( vector<float> InPwr, vector<float> InDistortion, vector<float>& SmoothedDistortion, float winSize, bool xknot)
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
   fnamestr1 << "C:\\Smoothing_IO_" << i << ".csv";

   ofstream smFile;
   smFile.open (fnamestr1.str().c_str(),ios::out);
   for(unsigned int loopVar = 0;loopVar < InPwr.size();loopVar++)
   {
      smFile << InPwr[loopVar] << "," << InDistortion[loopVar]<< "," << SmoothedDistortion[loopVar]<< endl;
   }
   smFile.close();
#endif

}

void truncateData( float truncLevel, vector<float>& InPwr, vector<float>& Gain, vector<float>& Phase)
{
   vector<float>::iterator inpPwrIter,lastInpPwrIter;
   vector<float>::iterator lastGainIter,lastPhseIter;

   lastInpPwrIter = InPwr.begin();
   lastGainIter = Gain.begin();
   lastPhseIter = Phase.begin();
   for(inpPwrIter = InPwr.begin();inpPwrIter!=InPwr.end();inpPwrIter++)
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

void generateAMAMNV( vector<float> targetOutputPowers, vector<float> inPwr, 
                    vector<float> smoothedGain,   vector<long>& amamLinear) 
{
   //Step 7.1.2 : Generate Measured Output Powers vector
   vector<float> outPower;
   vector<float> gainVec;
   vector<float> inPwrVec;
   unsigned long loopVar = 0;
   for(loopVar = 0;loopVar < inPwr.size();loopVar++)
   {
      gainVec.push_back(smoothedGain[loopVar]);
      outPower.push_back(inPwr[loopVar]+smoothedGain[loopVar]);
      inPwrVec.push_back(inPwr[loopVar]);
   }

   //Step 7.1.3 : Generate AMAM Log Vector
   const int LINEAR_SECTION_OF_DATA = 2;
   vector<float> amAmLog;
   int numPointsToAverageSlope = inPwrVec.size()/LINEAR_SECTION_OF_DATA;
   interpExterpWithAverageSlope(outPower, inPwrVec, targetOutputPowers,numPointsToAverageSlope,amAmLog);

   //Step 7.1.4 : Generate AMAM Linear Vector (Also from float to long)
   for(loopVar = 0;loopVar < amAmLog.size();loopVar++)
   {
      amamLinear.push_back((long)pow(10.0,amAmLog[loopVar]/20.0));
   }

#ifdef DEBUG_AMAM
   static int amamIndex = 0;
   amamIndex++;
   ostringstream fnamestr1;
   fnamestr1.str("");
   fnamestr1 << "C:\\AMAM_NVGEN_Inputs_" << amamIndex << ".csv";

   ostringstream fnamestr2;
   fnamestr2.str("");
   fnamestr2 << "C:\\AMAM_NVGEN_Outputs_" << amamIndex << ".csv";


   ofstream amamFile1,amamFile2;
   amamFile1.open (fnamestr1.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < outPower.size();loopVar++)
   {
      amamFile1 << outPower[loopVar] << "," << gainVec[loopVar]<< "," << inPwrVec[loopVar]<<endl;
   }
   amamFile1.close();

   amamFile2.open (fnamestr2.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < targetOutputPowers.size();loopVar++)
   {
      amamFile2 << targetOutputPowers[loopVar] << "," << amAmLog[loopVar]<< "," << amamLinear[loopVar]<< endl;
   }
   amamFile2.close();

#endif
}


void generateAMPMNV( vector<float> inPwr, vector<float> smoothedPhaseDistortion, vector<long>& amPmProcessed) 
{

   //Step 7.2.1 : Generate target input power (for phase).
   float maxPowerInput = *max_element(inPwr.begin(),inPwr.end()); //In Log Terms

   bool exceededMaxInputLevel = false;
   vector<float> targetInputPowers;
   unsigned int loopVar;
   for(loopVar = 1; loopVar < 128;loopVar++)//128*128 = 2^14
   {
      float inPwrLog = 20*log10(loopVar*(float)128.0);
      if(!exceededMaxInputLevel && (inPwrLog < maxPowerInput ))
      {
         targetInputPowers.push_back(inPwrLog);
      }
      else
      {
         targetInputPowers.push_back(maxPowerInput);
         exceededMaxInputLevel = true;
      }
   }
   if(!exceededMaxInputLevel)
   {
      targetInputPowers.push_back(20*log10((float)(pow(2.0,14)-1)));
   }
   else
   {
      targetInputPowers.push_back(maxPowerInput);
   }

   //Step 7.2.2 : Generate Array of Input Levels & Phases. Invert Phase
   vector<float> inPowers; //is the same as inPwrVec above (but done seperately to help refactorization)
   vector<float> phaseVec;
   for(loopVar = 0;loopVar < inPwr.size();loopVar++)
   {
      inPowers.push_back(inPwr[loopVar]);
      phaseVec.push_back((float)-1.0*smoothedPhaseDistortion[loopVar]);
   }

   //Step 7.2.3 : Generate AMPM Vector
   //Interpolate and figure out AMPM
   const int LINEAR_SECTION_OF_DATA = 8;
   vector<float> amPm;
   int numPointsToAverageSlope = inPowers.size()/LINEAR_SECTION_OF_DATA;
   interpExterpWithAverageSlope(inPowers, phaseVec, targetInputPowers,numPointsToAverageSlope,amPm);

   //Step 7.2.4 : Normalize Phase.
   //Take min and set it to 0.
   vector<float> amPmNormalized;
   float minAmPm = *min_element(amPm.begin(),amPm.end());
   for(loopVar = 0;loopVar < amPm.size();loopVar++)
   {
      amPmNormalized.push_back( amPm[loopVar] - minAmPm );
   }

   //Step 7.2.5 : Wrap into +/- 180
   float amPmVal;
   //Todo: Check if remainder is required
   float remainder;
   vector<float> amPmWrapped;
   for(loopVar = 0;loopVar < amPmNormalized.size();loopVar++)
   {
      amPmVal = amPmNormalized[loopVar];
      remainder = amPmVal-(long)amPmVal;
      if(amPmVal >= 0)
         amPmWrapped.push_back(((((long)amPmVal+180)%360)-180)+remainder);
      else
         amPmWrapped.push_back(((((long)amPmVal-180)%360)+180)+remainder);
   }

   //Step 7.2.6 : Scale to 16 bit number
   for(loopVar = 0;loopVar < amPmNormalized.size();loopVar++)
   {
      amPmProcessed.push_back( (long)(amPmWrapped[loopVar]/180*pow(2.0,15)));
   }


#ifdef DEBUG_AMPM
   static int ampmIndex = 0;
   ampmIndex++;
   ostringstream fnamestr3;
   fnamestr3.str("");
   fnamestr3 << "C:\\AMPM_NVGEN_Inputs_" << ampmIndex << ".csv";

   ostringstream fnamestr4;
   fnamestr4.str("");
   fnamestr4 << "C:\\AMPM_NVGEN_Outputs_" << ampmIndex << ".csv";


   ofstream ampmFile1,ampmFile2;
   ampmFile1.open (fnamestr3.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < inPowers.size();loopVar++)
   {
      ampmFile1 << inPowers[loopVar] << "," << phaseVec[loopVar]<<endl;
   }
   ampmFile1.close();

   ampmFile2.open (fnamestr4.str().c_str(),ios::out);
   for(loopVar = 0;loopVar < targetInputPowers.size();loopVar++)
   {
      ampmFile2 << targetInputPowers[loopVar] << "," << amPm[loopVar]<< "," << amPmNormalized[loopVar] << "," << amPmWrapped[loopVar]<< "," << amPmProcessed[loopVar] << endl ;
   }
   ampmFile2.close();

#endif
}

float interp(float x1,float x2,float y1,float y2,float x3)
{
  //cout << "interp " << x1 << ", " << x2 << ", " << y1 << ", " << y2 << ", " << x3 << endl;
  float slope = (y2-y1)/(x2-x1);
  return (y1 + (x3-x1)*slope);  
}

float extrap(float x1,float y1,float x2,float slope)
{
  //cout << "extrap " << x1 << ", " << y1 << ", " << slope << endl;
  return (y1 + slope*(x2-x1));
}

void interpExterpWithAverageSlope(vector<float> x, vector<float> y,
                                  vector<float> x1,int numPointsSlope, 
                                  vector<float>& y1)
{
   //Assumptions x, x1 are monotonic
   if(x.size() != y.size()) 
   {
      cout << "x and y must be same size" << endl;
      throw;
   }
   
   double beginSlope = 0.0;
   double endSlope = 0.0;
   double tempSlope;
   int lpVar,lpVar2;
   
   for(lpVar = 0;lpVar < numPointsSlope;lpVar++)
   {
     tempSlope = (y[lpVar]-y[lpVar+1])/(x[lpVar]-x[lpVar+1]);
     beginSlope =  (tempSlope + beginSlope * lpVar) / (lpVar + 1);
   }
   int N = x.size()-1;
   for(lpVar = 0;lpVar < numPointsSlope;lpVar++)
   {
     tempSlope = (y[N - lpVar]-y[N-lpVar-1])/(x[N-lpVar]-x[N-lpVar-1]);
     endSlope =  (tempSlope + endSlope * lpVar) / (lpVar + 1);
   }

    // cout << "Begin Slope : " << beginSlope << endl;
    // cout << "End Slope : " << endSlope << endl;

   float begin_x,end_x;
   end_x = *(x.end()-1);
   begin_x = *x.begin();
   float begin_y,end_y;
   end_y = *(y.end()-1);
   begin_y = *y.begin();
   bool increasing;
   if(end_x > begin_x) 
   {
     increasing = true;
   }
   else
   {
     increasing = false;
   }
     
   for(lpVar = 0;lpVar < x1.size();lpVar++)
   {
     if( ( increasing && (x1[lpVar] > end_x) ) || ( !increasing && (x1[lpVar] < end_x) ))
     {
       y1.push_back(extrap(end_x,end_y,x1[lpVar],endSlope));
       continue;
     }
     if( ( increasing && (x1[lpVar] < begin_x) ) || ( !increasing && (x1[lpVar] > begin_x) ))
     {
       y1.push_back(extrap(begin_x,begin_y,x1[lpVar],beginSlope));
       continue;
     }
     for(lpVar2 = 0;lpVar2 < x.size()-1;lpVar2++)
     {
       float x_1,x_2,y_1,y_2,x_3;
       x_1 = x[lpVar2];
       x_2 = x[lpVar2+1];
       y_1 = y[lpVar2];
       y_2 = y[lpVar2+1];
       x_3 = x1[lpVar];
       if( (increasing && (x_1 <= x_3) && (x_3 <= x_2)) || 
          (!increasing && (x_1 >= x_3) && (x_3 >= x_2)))
       {
          y1.push_back(interp(x_1,x_2,y_1,y_2,x_3));
          break;
       }
     }
   }
}
