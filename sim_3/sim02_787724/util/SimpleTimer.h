// Program Information ////////////////////////////////////////////////////////
/**
 * @file SimpleTimer.h
 *
 * @brief Definition file for simple timer class
 * 
 * @author Michael Leverington w/ Modifications by Connor Scully-Allison
 * 
 * @details Specifies all member methods of the SimpleTimer
 *
 * @version 1.00 (11 September 2015)
 * @version 1.01 (03 March 2016) Converted to C compatible code
 *
 * @Note None
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

// Header files ///////////////////////////////////////////////////////////////

#include <sys/time.h>
#include <string.h>
#include <stdbool.h>


       static const char NULL_CHAR = '\0';
       static const char RADIX_POINT = '.';

// Class definition  //////////////////////////////////////////////////////////
typedef struct SimpleTimer SimpleTimer;

struct SimpleTimer 
   {
       struct timeval startData;
       struct timeval endData;
       long int beginTime; 
       long int endTime;
       long int secTime;
       long int microSecTime;
       bool running;
       bool dataGood;
   };



/**
 * @brief Default constructor
 *
 * @details Constructs Timer class
 *          
 * @param None
 *
 * @note set running flag to false
 */
void makeSimpleTimer
       (
        SimpleTimer *self
       )
   {
    (*self).running = false;
   }




void start
       (
        SimpleTimer *self
       )
   {
    gettimeofday( &((*self).startData), NULL );

    (*self).running = true;

    (*self).dataGood = false;
   }

/**
 * @brief Stop control
 *
 * @details Takes final time data, calculates duration
 *          
 * @param None
 *
 * @note None
 */
void stop
       (
        SimpleTimer *self
       )
   {
    if( (*self).running )
       {
        gettimeofday( &((*self).endData), NULL );

        (*self).running = false;

        (*self).dataGood = true;
       }

    else
       {
        (*self).dataGood = false;
       }
   }

void getElapsedTime
       (
        char **timeStr, 
        SimpleTimer *self
       )
   {
    int low;
    int high; 
    int index = 0;
    char temp;

    if( (*self).dataGood )
       {
        (*self).secTime = (*self).endData.tv_sec - (*self).startData.tv_sec;
        (*self).microSecTime = (*self).endData.tv_usec - (*self).startData.tv_usec;

        if( (*self).microSecTime < 0 )
           {
            (*self).microSecTime += 1000000;

            (*self).secTime -= 1;
           }

        while( (*self).microSecTime > 0 )
           {
            (*timeStr)[ index ] = (char) ((*self).microSecTime % 10 + '0' );

            (*self).microSecTime /= 10;
    
            index++;
           }

        while( index < 6 )
           {
            (*timeStr)[ index ] = '0';

            index++;
           }

        (*timeStr)[ index ] = RADIX_POINT;

        index++;

        if( (*self).secTime == 0 )
           {
            (*timeStr)[ index ] = '0';
    
            index++;
           }

        while( (*self).secTime > 0 )
           {
            (*timeStr)[ index ] = (char) ( (*self).secTime % 10 + '0' );

            (*self).secTime /= 10;

            index++;
           }

        (*timeStr)[ index ] = NULL_CHAR;

        low = 0; high = index - 1;

        while( low < high )
           {
            temp = (*timeStr)[ low ];
            (*timeStr)[ low ] = (*timeStr)[ high ];
            (*timeStr)[ high ] = temp;

            low++; high--;
           }
       }

    else
       {
        strcpy( *timeStr, "No Data" );
       }
   
    }

#endif // ifndef SIMPLETIMER_H

