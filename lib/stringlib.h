
//Program Header Information /////////////////////////////
/**
  * @file stringlib.h
  *
  * @brief Library for standard and supplimental processes involving strings
  *
  * @details Library intended to replace all standard string methods and provide supplemental
  *          string processes for dynamic allocation of strings and arrays
  * 
  * @version 1.00
  *          Masters Student (26 Febuary, 2016) 
  *          Initial develolpment of string library
  *    
  * @note Requires stdlib.h
  */



#include <stdlib.h> //for malloc and NULL
#include <stdio.h>

#ifndef STRINGLIB_H
#define STRINGLIB_H


//Global Constants & Type Definitions////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
typedef char* string;

const char NULLCHAR = '\0';

void alloStr(char** newStr, int size)
   {

    //null string condidtion
    *newStr = calloc(size, sizeof(char));
     clearStr(newStr);

    //previously allocated string condition
   }

void alloEmptyStrArr(char*** strArr, int size)
   {
    int strCnt;    

    *strArr = calloc(size, sizeof(char*));
    for(strCnt = 0; strCnt < size; strCnt++)
        {
            alloStr(&((*strArr)[strCnt]), size);
        } 
   }

int isEmptyStr(const char* str)
   {
    if(str[0] == NULLCHAR)
       {
        return 1;
       }

    return 0;
   };


//return the length of a string not including terminating null character
int strLen(const char* str)
   {
    //variables
    int ltrCnt = 0;
    
    if(str != NULL)
       {
        //loop over a string until we reach a null char
        for(ltrCnt = 0; str[ltrCnt] != '\0'; ltrCnt++){};
       }  
    
    return ltrCnt;
   }





//copy one string into a new string
//requires that newStr be NULL if memory allocation
// is intended
void strCpy(char **newStr, const char * originStr)
   {
    //variables
    int originStrLen;
    int charNdx;
    
    //we are copying a null string
    if(originStr == NULL)
       {
        *newStr = NULL;
         return;        
       }

    //get the length of the origin string for testing
    originStrLen = strLen(originStr);
    
    if(*newStr == NULL)
       {
        *newStr = (char *) calloc(originStrLen + 1, sizeof(char));
       }    

    else if(strLen(*newStr) <= originStrLen)
       { 
        //reallocate data to new string
        *newStr = realloc(*newStr, (originStrLen + 1) * sizeof(char));
       }
    
    //copy
    for(charNdx = 0; originStr[charNdx] != '\0'; charNdx++)
       {
        (*newStr)[charNdx] = originStr[charNdx];
       }
    
    //add terminating char
    (*newStr)[charNdx] = '\0';
   }


//string compare
int strCmp(const char* firstStr, const char* secondStr)
   {
    int letter;
    
    //check for unallocated strings
    if(firstStr == NULL && secondStr == NULL)
       {
        return 1; //true
       }
    
    //one string is null but not both
    else if(firstStr == NULL || secondStr == NULL)       
       {
        return 0; //false
       }
    
    //neither string is null
    else
       {
        for(letter = 0; (firstStr[letter] != '\0') && (secondStr[letter] != '\0'); letter++)
           {
            if(firstStr[letter] != secondStr[letter])
               {
                return 0; //false        
               }
           }
    
        return 1; //true
        }

   }




//tells us if the given test char is a member of
// the supplied string
int isMember(const char testChar, const char* memberString)
   {
    //variables
    int cmpNdx;

    if(memberString == NULL)
       {
        return 0;
       }

    for(cmpNdx = 0; memberString[cmpNdx] != NULLCHAR; cmpNdx++)
        {
            //return true if char is member
            if(memberString[cmpNdx] == testChar)
                {
                 return 1;
                }
        }
    
    return 0;   
   }





//tokenize string
// returns first token with string argument
// if further tokens are desired from parseString
// NULL should be passed
// delimiters are removed from token
char* tokenizeStr(char* parseString, char* delimiters)
   {
    //variables
    static int ltrNdx;
    static char* tempParse;
    
    char* buffString = NULL;
    int bufferNdx = 0;      

    //reset static variables for fresh call of function
    if(parseString != NULL)
       {
        ltrNdx = 0;
        strCpy(&tempParse, parseString);
       }
    
    //incement past delimiters at beginning of word
    while(isMember(tempParse[ltrNdx], delimiters))
       {
        ltrNdx++;
       }
    

    //we have not reached the end of string to parse
    if( ltrNdx < strLen(tempParse))
       {
            //dynamically allocate bufferString size 
            // to not exceed size of our un-parsed string
            buffString = (char *) calloc(strLen(tempParse)+1, sizeof(char));
            
            //incement past delimiters at beginning of word
            while(isMember(tempParse[ltrNdx], delimiters))
               {
                ltrNdx++;
               }
    
            //if a given char in string is a member of delimiters string
            for(ltrNdx; tempParse[ltrNdx] != NULLCHAR; ltrNdx++)
               {

                //test if we have reached a delimiter
                if(isMember(tempParse[ltrNdx], delimiters))
                    {
                    //break out of loop
                    break;       
                    }
                
                //store a char
                buffString[bufferNdx] = tempParse[ltrNdx];
                bufferNdx++;
               }

            return buffString;
       }

    else
       {
        //nothing left to parse
        return NULL;
       }

   }






//append a char to a string
int appendChar(char** dest, char adjunct)
   {
    //variables
    int ltrNdx;


    if(*dest == NULL)
       {
        return -1;
       } 

    //get to end of our string
    for(ltrNdx = 0; (*dest)[ltrNdx] != NULLCHAR; ltrNdx++){}
    

    //replace null char with appeneded char
    (*dest)[ltrNdx] = adjunct;
    (*dest)[ltrNdx+1] = NULLCHAR;

    return 1;
   }





//clear a dynamically allocated 
// string of garbage
int clearStr(char** str)
   {
    //variables
    int sizeNdx;

    if(*str == NULL)
       {
        return -1;
       }

    for( sizeNdx = 0; sizeNdx < sizeof(*str); sizeNdx++)
        {
         (*str)[sizeNdx] = NULLCHAR;
        }

    return 1;
   }






//remove a leading space from a string
void removeLeadingChar(char** str)
    {
     int cNdx;

     for(cNdx = 0; (*str)[cNdx] != NULLCHAR; cNdx++)
       {
        (*str)[cNdx] = (*str)[cNdx+1];
       }
    }



char* ftoa( double num )
   {
    char* temp = NULL;

    temp = calloc(100, sizeof(char));

    snprintf(temp, 100, "%f", num);

    return temp;
   }


#endif /* stringlib.h */
