/* file: hppmASub.c
 * Collection of aSub subroutines...
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dbDefs.h>
#include <alarm.h>
#include <registryFunction.h>
#include "aSubRecord.h"
#include <epicsExport.h>
#include <math.h>
#include <stdlib.h>

#define SIZE(x)               (sizeof(x)/sizeof(x[0]))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;



static long asPre(aSubRecord *pr){
/*------------------------------------------------------------------------------
 * Breaks up a comma delimited preamble waveform into its fields. 
 * Reads a data waveform and scales it. 
 * The preamble waveform is of the form:
 *  "a,b,c,d,e,f,g,h,i,j", where
 *  a   	data format: 0=ascii,1=byte,2=word,3=compressed
 *  b   	type: 1=normal,2=average,3=envelope
 *  c		num points
 *  d		count (for averaging)
 *  e		x increment
 *  f       x origin 
 *  g       x ref
 *  h       y inc
 *  i       y origin
 *  j		y ref 
 *
 * DATA CONVERSION: amplitude=yinc*(val-yref) + yor
 *                  time=[(index-1)-xref]*xinc + xor
 *----------------------------------------------------------------------------*/
    #define DATA_LEN 500
    #define PRE_LEN 10
    #define PRE_INT 4    
    char  *p = (char*)pr->a;
    short *q = (short*)pr->b;
    double *cal = (double*)pr->c;
    short *up = (short*)pr->d; 
    short *ut = (short*)pr->e;
    double *ppw=(double*)pr->valk;
    double *ppt=(double*)pr->vall;
    double *pMax=(double*)pr->valm;
    double *pMin=(double*)pr->valn;
    double *pAve=(double*)pr->valo;
    const int n=16, preDouble=PRE_LEN - PRE_INT;
    int *ppi[PRE_INT], i, pMaxPos=0, pMinPos=0; 
    char *pch, tstr[PRE_LEN][n+1];
    double *ppf[preDouble];
    double unitsP, unitsT, exponent=*cal/10.0, base=10.0, factor=pow(base,exponent), sum=0.0;
    
    ppi[0]=(int*)pr->vala; ppi[1]=(int*)pr->valb; ppi[2]=(int*)pr->valc; ppi[3]=(int*)pr->vald;
    ppf[0]=(double*)pr->vale; ppf[1]=(double*)pr->valf; ppf[2]=(double*)pr->valg; ppf[3]=(double*)pr->valh;
    ppf[4]=(double*)pr->vali; ppf[5]=(double*)pr->valj;

    // Split preamble string
    pch = strtok(p,",");
    for (i=0; i<PRE_LEN; i++){
        if (pch != NULL){
            strncpy( tstr[i],pch,n); tstr[i][n]=0;
            pch = strtok(NULL, ",");            
            //printf("pch=%s,tstr=%s\n",pch,tstr[i]);
        }
    }
    for(i=0; i<PRE_INT; i++){
        *ppi[i]=atoi(tstr[i]);
        //printf("ppi[%i]=%i\n",i,*ppi[i]);
    }
    for(i=0; i<preDouble; i++){
        *ppf[i]=atof(tstr[i+PRE_INT]);
        //printf("ppf[%i]=%E\n",i,*ppf[i]);
    }
    
    // Unit conversion
    if(*up==1) unitsP=1e-6;
    else if(*up==2) unitsP=1e3;
    else unitsP=1;
    if(*ut==1) unitsT=1e6;
    else if(*ut==2) unitsT=1e9;
    else unitsT=1;
    
    //printf("cal=%f, exponent=%f, base=%f, factor=%f\n",*cal,exponent,base,factor);
    
    // Scale waveforms
    for (i=0; i<DATA_LEN; i++){
        ppw[i]=(*ppf[3]*(q[i]-*ppf[5]) + *ppf[4])*(1.0/factor)*unitsP;
        //ppw[i]=(*ppf[3]*(q[i]-*ppf[5]) + *ppf[4])*unitsP;
        ppt[i]=(*ppf[0]*(i-*ppf[2]) + *ppf[1])*unitsT;
        //ppt[i]=*ppf[0]*(i-*ppf[2]);
    }
 
    // Find max
    *pMax=ppw[0];
    for (i=0; i<DATA_LEN; i++){
        if(ppw[i]>*pMax){
            *pMax=ppw[i];
            pMaxPos=i;
        }
    }
    
    // Find min
    *pMin=ppw[0];
    for (i=0; i<DATA_LEN; i++){
        if(ppw[i]<*pMin){
            *pMin=ppw[i];
            pMinPos=i;
        }
    }

    // Find average
    for (i=0; i<DATA_LEN; i++){
        sum+=ppw[i];
    }    
    *pAve=sum/DATA_LEN;

    //printf("pMax=%f, pos=%d, pMin=%f, pos=%d, pAve=%f\n",*pMax,pMaxPos,*pMin,pMinPos,*pAve);    

    return(0);
}

/**************************************************************************/

epicsRegisterFunction(asPre);

