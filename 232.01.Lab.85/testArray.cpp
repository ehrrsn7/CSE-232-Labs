/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test array.h
 * Author
 *    Br. Helfrich
 ************************************************************************/

#ifndef DEBUG
#define DEBUG   
#endif
#undef DEBUG  // Remove this comment to disable unit tests

#include <iostream>
using namespace std;

#include "testArray.h"     // for the array unit tests

/**********************************************************************
 * MAIN
 * This is just a simple menu to launch a collection of tests
 ***********************************************************************/
int main()
{
   
#ifdef DEBUG
   // unit tests
   TestArray().run();
#endif // DEBUG
   
   // my code
   array;
   
   return 0;
}

