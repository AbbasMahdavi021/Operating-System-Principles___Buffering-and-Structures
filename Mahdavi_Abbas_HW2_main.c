/***********************************************************
* Class:  CSC-415-03 Fall 2022
* Name: Abbas Mahdavi
* Student ID: 918345420
* GitHub Name: AbbasMahdavi021
* Project: Assignment 2 – Structure in Memory and Buffering
*
* File: Mahdavi_Abbas_HW2_main.c
*
* Description: 
This assignment is to write a C program that 
fills the personalInfo object, from the data we give it such
as student ID, as well as data from the command line 
arguments, such as first and last name. Then it passes some 
values to the object fields and buffering data into block. 
Receiving strings and writing them character by character 
until it hits null. We ensured there was no data leaks. 
The purpose of this assignment is to give experience using 
structures, pointers, character strings, enumerated types, 
bitmap fields, and dealing with buffering.
*
**************************************************************/

#include "assignment2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

  /* I allocate the struct using malloc and populate the array list
  using -> based on the personalInfo requirements,
  so that we can assign values, and get the necessary info
  from the command line argument, keeping in mind their position in
  the command line */

  personalInfo *persInfo = malloc(sizeof(personalInfo));
  persInfo->firstName = argv[1];
  persInfo->lastName = argv[2];
  strcpy(persInfo->message, argv[3]);

  persInfo->studentID = 918345420;
  persInfo->level = SENIOR;

  persInfo->languages = KNOWLEDGE_OF_C | KNOWLEDGE_OF_JAVA | KNOWLEDGE_OF_PYTHON | KNOWLEDGE_OF_MIPS_ASSEMBLER;

  if (persInfo == NULL)
  {
    printf("Could not instantiate personal information!");
  }

  // Using the given writePersonalInfo and persInfo
  int r = writePersonalInfo(persInfo);
  // if r is not 0, it did not write correctly.
  if (r != 0)
  {
    printf("Could not write personal info!");
    return -1;
  }

  // Store the needed variables by calling the proper functions
  char *buffer = malloc(BLOCK_SIZE);
  int blockSize = BLOCK_SIZE;
  const char *nextString = getNext();

  int location = 0;

  // use while loop until we get null, if NULL is recieved, we are finished
  while (nextString != NULL)
  {
    /*check if string can fit in the blockSize
    We do this so we can proparly fill the block buffer
    We use the bytes recieved, to fill the buffer
    one single character at a time, until buffer is full.*/
    if (strlen(nextString) > blockSize)
    {
      memcpy(buffer + location, nextString, blockSize);
      commitBlock(buffer);
      // relocate the characters address, as we add to the buffer
      nextString += blockSize;
      // to redo the process, we empty the buffer, and the the index to 0.
      blockSize = BLOCK_SIZE;
      location = 0;
    }
    // if we reach this, it means there is enough space to fit the string in the buffer
    else
    {
      memcpy(buffer + location, nextString, strlen(nextString));
      // change variables after the process
      if (nextString != NULL)
      {
        blockSize -= strlen(nextString);
        location += strlen(nextString);
      }

      if (strlen(nextString) == blockSize)
      {
        // if fullfilled, commit and reset
        commitBlock(buffer);
        blockSize = BLOCK_SIZE;
        location = 0;
      }
      // continue to next line
      nextString = getNext();
    }
  }

  commitBlock(buffer);

  // free mem of persInfo
  free(persInfo);
  free(buffer);

  // call checkIt funtion to print the hex characters.
  int exits = checkIt();

  return exits;
}