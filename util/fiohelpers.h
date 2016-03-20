//Program Header Information /////////////////////////////
/**
  * @file fiohelpers.h
  *
  * @brief Supplemental methods for file io
  *
  * @details Defines a few methods to make file io more robust
  *          
  *         
  * 
  * @version 1.00
  *          Masters Student (26 Febuary, 2016) 
  *          Initial develolpment of File IO helpers
  *    
  * @note Rquires "stringlib.h", stdio
  */

//External Libraries///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "stringlib.h"


//Methods//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

//read in characters from file stream until we get to user
// specified deliminator
char fGetToDelim(FILE* scanFile, char** loadStr, char* delim)
   {
    //variables
    char holder;
    int loadNdx;    

    //primeloop
    holder = fgetc(scanFile); 
    while((holder != EOF) && !isMember(holder, delim))
       {
        appendChar(loadStr, holder);
        holder = fgetc(scanFile);
       }

    return holder; 
   }


//return the file size (in chars) of
//the currently manipulated file
int getFileSize(FILE* scanFile)
   {
    int fileSize;

    if(scanFile != NULL)
        {
         fseek(scanFile, 0, SEEK_END);
         fileSize = ftell(scanFile);
         rewind(scanFile); 
         return fileSize;
        }

    return -1; //error condition
   }
