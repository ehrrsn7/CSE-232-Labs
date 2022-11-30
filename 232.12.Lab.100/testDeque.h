/***********************************************************************
 * Header:
 *    TEST DEQUE
 * Summary:
 *    Unit tests for deque
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "deque.h"
#include "unitTest.h"

#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

class TestDeque : public UnitTest
{
public:
   void run()
   {
      reset();

      // Utilities
      test_iaFromID_trivial();
      test_iaFromID_slided();
      test_iaFromID_wrapped();
      test_iaFromID_negSlide();

      // Construct
      test_construct_default();
      test_constructCopy_empty();
      test_constructCopy_standard();

      // Assign
      test_assign_emptyToEmpty();
      test_assign_standardToEmpty();
      test_assign_emptyToStandard();
      test_assign_smallToBig();
      test_assign_bigToSmall();
      test_assign_unwrap();
      test_assign_unwrapNegative();

      // Iterator
      test_begin_standard();
      test_end_standard();
      test_iterator_increment_standardMiddle();
      test_iterator_dereferenceRead_standard();
      test_iterator_dereferenceRead_wrap();
      test_iterator_dereferenceRead_wrapNegative();
      test_iterator_dereferenceWrite_standard();
      test_iterator_difference_standard();
      test_iterator_addonto_standard();

      // Access
      test_frontRead_standard();
      test_frontRead_wrap();
      test_frontRead_wrapNegative();
      test_frontWrite_standard();
      test_backRead_standard();
      test_backRead_wrap();
      test_backRead_wrapNegative();
      test_backWrite_standard();
      test_subscriptRead_standard();
      test_subscriptRead_wrap();
      test_subscriptRead_wrapNegative();
      test_subscriptWrite_standard();

      // Insert
      test_pushback_empty();
      test_pushback_room();
      test_pushback_grow();
      test_pushback_growWrap();
      test_pushfront_empty();
      test_pushfront_room();
      test_pushfront_grow();
      test_pushfront_growWrap();

      // Remove
      test_clear_empty();
      test_clear_standard();
      test_popback_standard();
      test_popback_wrap();
      test_popback_wrapNegative();
      test_popfront_standard();
      test_popfront_wrap();
      test_popfront_wrapNegative();

      // Status
      test_size_empty();
      test_size_standard();
      test_empty_empty();
      test_empty_standard();


      report("Deque");
   }

   /***************************************
    * UTILITIES
    ***************************************/

   // test iaFromID where iaFront = 0
   void test_iaFromID_trivial()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      int ia0 = d.iaFromID(/*id=*/0);
      int ia1 = d.iaFromID(/*id=*/1);
      int ia2 = d.iaFromID(/*id=*/2);
      // verify
      assertUnit(ia0 == 0);
      assertUnit(ia1 == 1);
      assertUnit(ia2 == 2);
   }  // teardown

   // test iaFromID where iaFront = 2
   void test_iaFromID_slided()
   {  // setup
      //             iaFront
      // ia = 0    1    2 
      //    +----+----+----+    +    +
      //    | 26 | 31 | 11 | 26   31  
      //    +----+----+----+    +    +
      // id = 1    2    0    1    2
      custom::deque<int> d;
      d.data = new int[3];
      d.data[2] = 11;
      d.data[0] = 26;
      d.data[1] = 31;
      d.numCapacity = 3;
      d.numElements = 3;
      d.iaFront = 2;
      // exercise
      int ia0 = d.iaFromID(/*id=*/0);
      int ia1 = d.iaFromID(/*id=*/1);
      int ia2 = d.iaFromID(/*id=*/2);
      // verify
      assertUnit(ia0 == 2);
      assertUnit(ia1 == 0);
      assertUnit(ia2 == 1);
   }  // teardown

   // test iaFromID where iaFront = 6
   void test_iaFromID_wrapped()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      d.data = new int[3];
      d.data[0] = 11;
      d.data[1] = 26;
      d.data[2] = 31;
      d.numCapacity = 3;
      d.numElements = 3;
      d.iaFront = 6;
      // exercise
      int ia0 = d.iaFromID(/*id=*/0);
      int ia1 = d.iaFromID(/*id=*/1);
      int ia2 = d.iaFromID(/*id=*/2);
      // verify
      assertUnit(ia0 == 0);
      assertUnit(ia1 == 1);
      assertUnit(ia2 == 2);
   }  // teardown

   // test iaFromID where iaFront = -1
   void test_iaFromID_negSlide()
   {  // setup
      //     iaFront
      // ia = -1    0    1    2    
      //     +    +----+----+----+ 
      //       11 | 26 | 31 | 11 | 
      //     +    +----+----+----+
      // id =  0    1    2    0
      custom::deque<int> d;
      d.data = new int[3];
      d.data[0] = 26;
      d.data[1] = 31;
      d.data[2] = 11;
      d.numCapacity = 3;
      d.numElements = 3;
      d.iaFront = -1;
      // exercise
      int ia0 = d.iaFromID(/*id=*/0);
      int ia1 = d.iaFromID(/*id=*/1);
      int ia2 = d.iaFromID(/*id=*/2);
      // verify
      assertUnit(ia0 == 2);
      assertUnit(ia1 == 0);
      assertUnit(ia2 == 1);
   }  // teardown

   // test iaFromID where iaFront = -6
   void test_iaFromID_negWrapped()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      d.data = new int[3];
      d.data[0] = 11;
      d.data[1] = 26;
      d.data[2] = 31;
      d.numCapacity = 3;
      d.numElements = 3;
      d.iaFront = -6;
      // exercise
      int ia0 = d.iaFromID(/*id=*/0);
      int ia1 = d.iaFromID(/*id=*/1);
      int ia2 = d.iaFromID(/*id=*/2);
      // verify
      assertUnit(ia0 == 0);
      assertUnit(ia1 == 1);
      assertUnit(ia2 == 2);
   }  // teardown

   /***************************************
    * CONSTRUCT
    ***************************************/

   // default constructor, no allocations
   void test_construct_default()
   {  // setup
      std::allocator<custom::deque<int>> alloc;
      custom::deque<int> d;
      d.iaFront = 66;
      d.numCapacity = 77;
      d.numElements = 88;
      d.data = (int*)0xBAADF00D;
      // exercise
      alloc.construct(&d);  // just call the constructor by itself
      // verify
      assertEmptyFixture(d);
   }  // teardown

    // copy constructor of an empty list
   void test_constructCopy_empty()
   {  // setup
      custom::deque<int> dSrc;
      // exercise
      custom::deque<int> dDes(dSrc);
      // verify
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   // copy constructor of a 3-element collection
   void test_constructCopy_standard()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      // exercise
      custom::deque<int> dDes(dSrc);
      // verify
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dSrc);
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dDes);
      // teardown
   }

   /***************************************
    * ASSIGN
    ***************************************/

    // use the assignment operator when both are empty
   void test_assign_emptyToEmpty()
   {  // setup
      custom::deque<int> dSrc;
      custom::deque<int> dDes;
      // exercise
      dDes = dSrc;
      // verify
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   // From the standard to fixture to an empty list
   void test_assign_standardToEmpty()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      custom::deque<int> dDes;
      // exercise
      dDes = dSrc;
      // verify
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dSrc);
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dDes);
      // teardown
   }

   // From the empty list to the standard to fixture
   void test_assign_emptyToStandard()
   {  // setup
      custom::deque<int> dSrc;
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> dDes;
      setupStandardFixture(dDes);
      // exercise
      dDes = dSrc;
      // verify
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   void test_assign_bigToSmall()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      //   iaFront
      // ia = 0    1  
      //    +----+----+
      //    | 85 | 99 |
      //    +----+----+
      // id = 0    1   
      custom::deque<int> dDes;
      dDes.data = new int[2];
      dDes.data[0] = 85;
      dDes.data[1] = 99;
      dDes.numCapacity = 2;
      dDes.numElements = 2;
      dDes.iaFront = 0;
      // exercise
      dDes = dSrc;
      // verify
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dSrc);
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dDes);
      // teardown
   }

   // assign a small list of 3 onto a larger one of 4
   void test_assign_smallToBig()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      //   iaFront
      // ia = 0    1    2    3
      //    +----+----+----+----+
      //    | 61 | 73 | 85 | 99 |
      //    +----+----+----+----+ 
      // id = 0    1    2    3
      custom::deque<int> dDes;
      dDes.data = new int[4];
      dDes.data[0] = 61;
      dDes.data[1] = 73;
      dDes.data[2] = 85;
      dDes.data[3] = 99;
      dDes.numCapacity = 4;
      dDes.numElements = 4;
      dDes.iaFront = 0;
      // exercise
      dDes = dSrc;
      // verify
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dSrc);
      //   iaFront
      // ia = 0    1    2    3
      //    +----+----+----+----+
      //    | 11 | 26 | 31 |    |
      //    +----+----+----+----+ 
      // id = 0    1     2
      assertUnit(dDes.numCapacity == 4);
      assertUnit(dDes.iaFront == 0);
      assertUnit(dDes.numElements == 3);
      assertUnit(dDes.data != nullptr);
      if (dDes.data != nullptr && dDes.numCapacity >= 3)
      {
         assertUnit(dDes.data[0] = 11);
         assertUnit(dDes.data[1] = 26);
         assertUnit(dDes.data[2] = 31);
      }
      // teardown
   }

   // From the standard to fixture to an empty list
   void test_assign_unwrap()
   {  // setup
      //             iaFront
      // ia = 0    1    2 
      //    +----+----+----+    +    +
      //    | 26 | 31 | 11 | 26   31  
      //    +----+----+----+    +    +
      // id = 1    2    0    1    2
      custom::deque<int> dSrc;
      dSrc.data = new int[3];
      dSrc.data[0] = 26;
      dSrc.data[1] = 31;
      dSrc.data[2] = 11;
      dSrc.numCapacity = 3;
      dSrc.numElements = 3;
      dSrc.iaFront = 2;
      custom::deque<int> dDes;
      // exercise
      dDes = dSrc;
      // verify
      //             iaFront
      // ia = 0    1    2 
      //    +----+----+----+    +    +
      //    | 26 | 31 | 11 | 26   31  
      //    +----+----+----+    +    +
      // id =  1   2    0    1    2
      assertUnit(dSrc.numCapacity == 3);
      assertUnit(dSrc.iaFront == 2);
      assertUnit(dSrc.numElements == 3);
      assertUnit(dSrc.data != nullptr);
      if (dSrc.data != nullptr && dSrc.numCapacity == 3)
      {
         assertUnit(dSrc.data[0] = 26);
         assertUnit(dSrc.data[1] = 31);
         assertUnit(dSrc.data[2] = 11);
      }
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dDes);
      // teardown
   }

   // From the standard to fixture to an empty list
   void test_assign_unwrapNegative()
   {  // setup
      //    iaFront
      // ia = -4   -3   -2   -1   0    1    2    
      //     +   +    +    +    +----+----+----+ 
      //      11   26   31   11 | 26 | 31 | 11 |
      //     +   +    +    +    +----+----+----+
      // id = 2    0    1    2    0    1    2
      custom::deque<int> dSrc;
      dSrc.data = new int[3];
      dSrc.data[0] = 26;
      dSrc.data[1] = 31;
      dSrc.data[2] = 11;
      dSrc.numCapacity = 3;
      custom::deque<int> dDes;
      dSrc.iaFront = -4;
      dSrc.numElements = 3;
      // exercise
      dDes = dSrc;
      // verify
      //    iaFront
      // ia = -4   -3   -2   -1   0    1    2    
      //     +   +    +    +    +----+----+----+ 
      //      11   26   31   11 | 26 | 31 | 11 |
      //     +   +    +    +    +----+----+----+
      // id = 2    0    1    2    0    1    2
      assertUnit(dSrc.numCapacity == 3);
      assertUnit(dSrc.numElements == 3);
      assertUnit(dSrc.iaFront == -4);
      assertUnit(dSrc.data != nullptr);
      if (dSrc.data != nullptr && dSrc.numCapacity == 3)
      {
         assertUnit(dSrc.data[0] = 26);
         assertUnit(dSrc.data[1] = 31);
         assertUnit(dSrc.data[2] = 11);
      }
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertStandardFixture(dDes);
      // teardown
   }

   /***************************************
    * ITERATOR
    ***************************************/

   // get the begin iterator from the standard fixture
   void test_begin_standard()
   {  // setup
      custom::deque<int>::iterator it;
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //           it
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      it = d.begin();
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //     it
      assertUnit(it.id == 0);
      assertUnit(it.pDeque == &d);
      assertStandardFixture(d);
      // teardown
   }

   // get the end iterator from the standard fixture
   void test_end_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1    2
      //           it
      custom::deque<int> d;
      setupStandardFixture(d);
      custom::deque<int>::iterator it;
      it.pDeque = &d;
      it.id = 1;
      // exercise
      it = d.end();
      //    +----+----+----+    +
      //    | 11 | 26 | 31 |
      //    +----+----+----+    + 
      // id = 0    1    2    3
      //                     it
      assertUnit(it.id == 3);
      assertUnit(it.pDeque == &d);
      assertStandardFixture(d);
      // teardown
   }

    // test the iterator to increment from the middle of the standard fixture
   void test_iterator_increment_standardMiddle()
   {  // setup
      custom::deque<int>::iterator it;
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1    2
      //           it
      custom::deque<int> d;
      setupStandardFixture(d);
      it.id = 1;
      it.pDeque = &d;
      // exercise
      ++it;
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1    2
      //                it
      assertUnit(it.id == 2);
      assertUnit(it.pDeque == &d);
      assertStandardFixture(d);
      // teardown
   }

   // the the iterator's dereference operator to access an item from the list
   void test_iterator_dereferenceRead_standard()
   {  // setup
      custom::deque<int>::iterator it;
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1    2
      //          it
      custom::deque<int> d;
      setupStandardFixture(d);
      it.id = 1;
      it.pDeque = &d;
      int iReturn = 99;
      // exercise
      iReturn = *it;
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1    2
      //           it
      assertUnit(iReturn == 26);
      assertUnit(it.id == 1);
      assertUnit(it.pDeque == &d);
      assertStandardFixture(d);
      // teardown
   }

   // the the iterator's dereference operator to access an item from the list
   void test_iterator_dereferenceRead_wrap()
   {  // setup
      custom::deque<int>::iterator it;
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      //                                         it
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      it.id = 1;
      it.pDeque = &d;
      int iReturn = 99;
      // exercise
      iReturn = *it;
      // verify
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      //                                         it
      assertUnit(iReturn == 26);
      assertUnit(it.id == 1);
      assertUnit(it.pDeque == &d);
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // the the iterator's dereference operator to access an item from the list
   void test_iterator_dereferenceRead_wrapNegative()
   {  // setup
      custom::deque<int>::iterator it;
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      //            it
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      it.id = 1;
      it.pDeque = &d;
      int iReturn = 99;
      // exercise
      iReturn = *it;
      // verify
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      //            it
      assertUnit(iReturn == 26);
      assertUnit(it.id == 1);
      assertUnit(it.pDeque == &d);
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_dereferenceWrite_standard()
   {  // setup
      custom::deque<int>::iterator it;
      //    +----+----+----+
      //    | 11 | 99 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //           it
      custom::deque<int> d;
      setupStandardFixture(d);
      d.data[1] = 99;
      it.id = 1;
      it.pDeque = &d;
      // exercise
      *it = 26;
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //           it
      assertUnit(it.id == 1);
      assertUnit(it.pDeque == &d);
      assertStandardFixture(d);
      // teardown
   }

   // the difference between two iterators
   void test_iterator_difference_standard()
   {  // setup
      custom::deque<int>::iterator it1;
      custom::deque<int>::iterator it2;
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //     it1        it2
      custom::deque<int> d;
      setupStandardFixture(d);
      it1.id = 0;
      it2.id = 2;
      it1.pDeque = it2.pDeque = &d;
      int difference;
      // exercise
      difference = it2 - it1;
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //     it1        it2
      assertUnit(it1.id == 0);
      assertUnit(it1.pDeque == &d);
      assertUnit(it2.id == 2);
      assertUnit(it2.pDeque == &d);
      assertUnit(difference = 2);
      assertStandardFixture(d);
      // teardown
   }

   // add onto an iterator
   void test_iterator_addonto_standard()
   {  // setup
      custom::deque<int>::iterator it;
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //      it
      custom::deque<int> d;
      setupStandardFixture(d);
      it.id = 0;
      it.pDeque = &d;
      // exercise
      it += 2;
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      //                it
      assertUnit(it.id == 2);
      assertUnit(it.pDeque == &d);
      assertStandardFixture(d);
      // teardown
   }

   /***************************************
    * FRONT and BACK
    ***************************************/

    // read the element off the front of the standard list
   void test_frontRead_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      s = d.front();
      // verify
      assertUnit(s == int(11));
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      assertStandardFixture(d);
      // teardown
   }

   // read front on the wrapped state
   void test_frontRead_wrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      int s(99);
      // exercise
      s = d.front();
      // verify
      assertUnit(s == int(11));
      // verify
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // read front in the negative wrapped state
   void test_frontRead_wrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      int s(99);
      // exercise
      s = d.front();
      // verify
      assertUnit(s == int(11));
      // verify
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // write the element to the front of the standard list
   void test_frontWrite_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.front() = s;
      // verify
      //    +----+----+----+
      //    | 99 | 26 | 31 |
      //    +----+----+----+      
      assertUnit(d.data[0] == 99);
      d.data[0] = 11;
      assertStandardFixture(d);
      // teardown
   }

   // read the element off the back of the standard list
   void test_backRead_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      s = d.back();
      // verify
      assertUnit(s == int(31));
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      assertStandardFixture(d);
      // teardown
   }

   // read back on the wrapped state
   void test_backRead_wrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      int s(99);
      // exercise
      s = d.back();
      // verify
      assertUnit(s == int(31));
      // verify
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // read back in the negative wrapped state
   void test_backRead_wrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      int s(99);
      // exercise
      s = d.back();
      // verify
      assertUnit(s == int(31));
      // verify
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // write the element to the back of the standard list
   void test_backWrite_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.back() = s;
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 99 |
      //    +----+----+----+      
      assertUnit(d.data[2] == int(99));
      d.data[2] = int(31);
      assertStandardFixture(d);
      // teardown
   }

   // read an element from the middle of the standard deque
   void test_subscriptRead_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      s = d[1];
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertUnit(s == 26);
      assertStandardFixture(d);
      // teardown
   }

   // read an element from the middle of the wrapped deque
   void test_subscriptRead_wrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      int s(99);
      // exercise
      s = d[1];
      // verify
      assertUnit(s == int(26));
      // verify
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // read an element from the middle of the negative wrapped deque
   void test_subscriptRead_wrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      int s(99);
      // exercise
      s = d[1];
      // verify
      assertUnit(s == int(26));
      // verify
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      d.iaFront = 0;
      assertStandardFixture(d);
      // teardown
   }

   // write an element to the middle of the standard deque
   void test_subscriptWrite_standard()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d[1] = s;
      // verify
      //    +----+----+----+
      //    | 11 | 99 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      assertUnit(d.data[1] == 99);
      d.data[1] = 26;
      assertStandardFixture(d);
      // teardown
   }

   /***************************************
    * PUSH BACK
    ***************************************/

   // push back to an empty deque
   void test_pushback_empty()
   {  // setup
      custom::deque<int> d;
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(d.numCapacity == 1);
      assertUnit(d.numElements == 1);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 1)
         assertUnit(d.data[0] == 99);
      // teardown
   }

   // push an element when there is excess capacity
   void test_pushback_room()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 |    |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      d.data[2] = 50;
      d.numElements = 2;
      int s(31);
      // exercise
      d.push_back(s);
      // verify
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      assertStandardFixture(d);
      // teardown     
   }

   // push back when the capacity must double
   void test_pushback_grow()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //    +----+----+----+----+----+----+
      //    | 11 | 26 | 31 | 99 |    |    |
      //    +----+----+----+----+----+----+   
      assertUnit(d.numCapacity == 6);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 4)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
         assertUnit(d.data[3] == 99);
      }
      // teardown      
   }

   // push back when the capacity must double
   void test_pushback_growWrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1   2     0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //    iaFront
      // ia = 0    1    2    3    4    5    
      //    +----+----+----+----+----+----+
      //    | 11 | 26 | 31 | 99 |    |    |
      //    +----+----+----+----+----+----+ 
      // id = 0    1    2    3
      assertUnit(d.numCapacity == 6);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 4)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
         assertUnit(d.data[3] == 99);
      }
      // teardown      
   }

   // push back when the capacity must double
   void test_pushback_growWrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //    iaFront
      // ia = 0    1    2    3    4    5    
      //    +----+----+----+----+----+----+
      //    | 11 | 26 | 31 | 99 |    |    |
      //    +----+----+----+----+----+----+ 
      // id = 0    1    2    3
      assertUnit(d.numCapacity == 6);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 4)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
         assertUnit(d.data[3] == 99);
      }
      // teardown      
   }



   /***************************************
    * PUSH FRONT
    ***************************************/

    // push front to an empty deque
   void test_pushfront_empty()
   {  // setup
      custom::deque<int> d;
      int s(99);
      // exercise
      d.push_front(s);
      // verify
      //     iaFront
      // ia = -1    0    
      //     +    +----+
      //       99 | 99 |
      //     +    +----+
      // id =  0       
      assertUnit(d.numCapacity == 1);
      assertUnit(d.numElements == 1);
      assertUnit(d.iaFront == -1);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 1)
         assertUnit(d.data[0] == 99);
      // teardown
   }

   // push an element when there is excess capacity
   void test_pushfront_room()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 |    |
      //    +----+----+----+      
      // id = 0    1    
      custom::deque<int> d;
      setupStandardFixture(d);
      d.data[2] = 50;
      d.numElements = 2;
      int s(31);
      // exercise
      d.push_front(s);
      // verify
      //     iaFront
      // ia = -1    0    1    2    
      //     +    +----+----+----+ 
      //       31 | 11 | 26 | 31 | 
      //     +    +----+----+----+
      // id =  0    1    2    0
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == -1);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 3)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
      }
      // teardown     
   }

   // push front when the capacity must double
   void test_pushfront_grow()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.push_front(s);
      // verify
      //     iaFront
      // ia = -1    0    1    2    3    4    5
      //     +    +----+----+----+----+----+----+
      //       99 | 26 | 31 | 11 |    |    | 99 |
      //     +    +----+----+----+----+----+----+
      // id =  0    1    2    3              0
      assertUnit(d.numCapacity == 6);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == -1);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 4)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
         assertUnit(d.data[5] == 99);
      }
      // teardown      
   }

   // push front when the capacity must double
   void test_pushfront_growWrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      int s(99);
      // exercise
      d.push_front(s);
      // verify
      //     iaFront
      // ia = -1    0    1    2    3    4    5
      //     +    +----+----+----+----+----+----+
      //       99 | 26 | 31 | 11 |    |    | 99 |
      //     +    +----+----+----+----+----+----+
      // id =  0    1    2    3              0
      assertUnit(d.numCapacity == 6);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == -1);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 4)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
         assertUnit(d.data[5] == 99);
      }
      // teardown      
   }

   // push front when the capacity must double
   void test_pushfront_growWrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //     iaFront
      // ia =  0    1    2    3    4    5
      //     +----+----+----+----+----+----+
      //     | 11 | 26 | 31 | 99 |    |    |
      //     +----+----+----+----+----+----+
      // id =  0    1    1    3         
      assertUnit(d.numCapacity == 6);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 4)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
         assertUnit(d.data[3] == 99);
      }
      // teardown      
   }

   /***************************************
    * CLEAR
    ***************************************/

    // clear an empty fixture
   void test_clear_empty()
   {  // setup
      custom::deque<int> d;
      // exercise
      d.clear();
      // verify
      assertEmptyFixture(d);
   }  // teardown

   void test_clear_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      d.clear();
      // verify
      assertEmptyFixture(d);
   }  // teardown

   /***************************************
    * POP FRONT and POP BACK
    ***************************************/

    // remove element off the back of the standard list
   void test_popback_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      d.pop_back();
      // verify
      //    +----+----+----+
      //    | 11 | 26 |    |
      //    +----+----+----+      
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 2)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
      }      
      // teardown
   }

   // remove element off the back of the wrapped state
   void test_popback_wrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2                   0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      // exercise
      d.pop_back();
      // verify
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7     
      //    +----+----+----+    +    +    +    +    +   
      //    | 11 | 26 |    | 11   26        11   26     
      //    +----+----+----+    +    +    +    +    +   
      // id = 0    1         0    1         0    1     
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 6);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 2)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
      }
      // teardown
   }

   //  remove element off the back of the negative wrapped state
   void test_popback_wrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      // exercise
      d.pop_back();
      // verify
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26        11   26      | 11 | 26 |    |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1         0    1         0    1     
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == -6);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 2)
      {
         assertUnit(d.data[0] == 11);
         assertUnit(d.data[1] == 26);
      }
      // teardown
   }

   // remove element off the front of the standard list
   void test_popfront_standard()
   {  // setup
      //   iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      // id = 0    1     2
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      d.pop_front();
      // verify
      //        iaFront
      // ia = 0    1    2 
      //    +----+----+----+
      //    |    | 26 | 31 |
      //    +----+----+----+      
      // id =       0    1     
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 1);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 2)
      {
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
      }
      // teardown
   }

   // remove element off the front of the wrapped state
   void test_popfront_wrap()
   {  // setup
      //                                 iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    | 11 | 26 | 31 | 11   26   31   11   26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id = 0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = 6;
      // exercise
      d.pop_front();
      // verify
      //                                      iaFront
      // ia = 0    1    2    3    4    5    6    7    8
      //    +----+----+----+    +    +    +    +    +    +
      //    |    | 26 | 31 |      26   31        26   31
      //    +----+----+----+    +    +    +    +    +    +
      // id =       0    1         0    1         0    1    
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 7);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 2)
      {
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
      }
      // teardown
   }

   //  remove element off the front of the negative wrapped state
   void test_popfront_wrapNegative()
   {  // setup
      //    iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //       11   26   31   11   26   31 | 11 | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =  0    1    2    0    1    2    0    1    2
      custom::deque<int> d;
      setupStandardFixture(d);
      d.iaFront = -6;
      // exercise
      d.pop_front();
      // verify
      //         iaFront
      // ia =  -6   -5   -4   -3   -2   -1   0    1    2    
      //     +    +    +    +    +    +    +----+----+----+ 
      //            26   31        26   31 |    | 26 | 31 |
      //     +    +    +    +    +    +    +----+----+----+
      // id =        0    1        0    1         0    1   
      assertUnit(d.numCapacity == 3);
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == -5);
      assertUnit(d.data != nullptr);
      if (d.numElements >= 2)
      {
         assertUnit(d.data[1] == 26);
         assertUnit(d.data[2] == 31);
      }
      // teardown
   }

   /***************************************
    * SIZE EMPTY
    ***************************************/

    // size of graph with one node
   void test_size_empty()
   {  // setup
      custom::deque<int> d;
      // exercise
      size_t size = d.size();
      // verify
      assertUnit(0 == size);
      assertEmptyFixture(d);
   }  // teardown

   // size of graph with four nodes
   void test_size_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      size_t size = d.size();
      // verify
      assertUnit(3 == size);
      assertStandardFixture(d);
      // teardown
   }

   // graph with one node
   void test_empty_empty()
   {  // setup
      custom::deque<int> d;
      // exercise
      bool empty = d.empty();
      // verify
      assertUnit(true == empty);
      assertEmptyFixture(d);
   }  // teardown

   // graph with four nodes
   void test_empty_standard()
   {  // setup
      //    +----+----+----+
      //    | 11 | 26 | 31 |
      //    +----+----+----+      
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      bool empty = d.empty();
      // verify
      assertUnit(false == empty);
      assertStandardFixture(d);
      // teardown
   }


   /****************************************************************
    * Setup Standard Fixture
    *      iaFront
    *    ia = 0    1    2
    *       +----+----+----+
    *       | 11 | 26 | 31 |
    *       +----+----+----+
    *    id = 0    1     2
    ****************************************************************/
   void setupStandardFixture(custom::deque<int>& d)
   {
      d.data = new int[3];
      d.data[0] = 11;
      d.data[1] = 26;
      d.data[2] = 31;

      d.numCapacity = 3;
      d.numElements = 3;
      d.iaFront = 0;
   }

   /****************************************************************
    * Verify Empty Fixture
    ****************************************************************/
   void assertEmptyFixtureParameters(const custom::deque<int>& d, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(d.iaFront == 0);
      assertIndirect(d.numElements == 0);
   }

   /****************************************************************
    * Verify Standard Fixture
    *      iaFront
    *    ia = 0    1    2
    *       +----+----+----+
    *       | 11 | 26 | 31 |
    *       +----+----+----+
    *    id = 0    1     2
    ****************************************************************/
   void assertStandardFixtureParameters(const custom::deque<int>& d, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(d.numCapacity == 3);
      assertIndirect(d.iaFront == 0);
      assertIndirect(d.numElements == 3);
      assertIndirect(d.data != nullptr);

      if (d.numCapacity == 3 && d.data != nullptr)
      {
         assertIndirect(d.data[0] == 11);
         assertIndirect(d.data[1] == 26);
         assertIndirect(d.data[2] == 31);
      }
   }
};

#endif // DEBUG
