/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test deque.h
 * Author
 *    Br. Helfrich
 ************************************************************************/

#ifndef DEBUG
#define DEBUG   
#endif
 //#undef DEBUG  // Remove this comment to disable unit tests

#include "testDeque.h"       // for the deque unit tests

/**********************************************************************
 * MAIN
 * This is just a simple menu to launch a collection of tests
 ***********************************************************************/
int main()
{
#ifdef DEBUG
   // unit tests
   TestDeque().run();
#endif // DEBUG
   
   return 0;
}
