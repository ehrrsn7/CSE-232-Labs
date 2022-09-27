/***********************************************************************
 * Header:
 *    TEST ARRAY
 * Summary:
 *    Unit tests for array
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include <array>
#include "array.h"      // class under test
#include "unitTest.h"   // unit test baseclass
#include "spy.h"        // spy is a mock class to monitor the class under test

/***********************************************
 * TEST ARRAY
 * Unit tests for the Array class
 ***********************************************/
class TestArray : public UnitTest
{
public:
   void run()
   {
      reset();   
      
      // Size, Empty
      test_size_empty();
      test_size_standard();
      test_empty_empty();
      test_empty_standard();
      
      // Subscript operator and at
      test_subscript_read();
      test_subscript_write();
      test_at_readValid();
      test_at_writeValid();
      test_at_readError();
      test_at_writeError();

      // Front and back
      test_front_read();
      test_front_write();
      test_back_read();
      test_back_write();
      
      // Iterator
      test_begin();
      test_end();
      test_iterator_default();
      test_iterator_nondefault();
      test_iterator_copy();
      test_iterator_assign();
      test_iterator_equals_equals();
      test_iterator_equals_differentValues();
      test_iterator_equals_differentArrays();
      test_iterator_notequals_equals();
      test_iterator_notequals_differentValues();
      test_iterator_notequals_differentArrays();
      test_iterator_dereferenceRead();
      test_iterator_dereferenceWrite();
      test_iterator_incrementPrefix();
      test_iterator_incrementPostfix();

      report("Array");
   }
   

   /***************************************
    * SIZE EMPTY 
    ***************************************/

   // size of an empty array
   void test_size_empty()
   {  // setup
      const custom::array<Spy, 0> a{};
      // exercise
      size_t size = a.size();
      // verify
      assertUnit(size == 0);
   }  // teardown
   
   // size of a standard fixture
   void test_size_standard()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      // exercise
      size_t size = a.size();
      // verify
      assertUnit(size == 4);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // empty array empty?
   void test_empty_empty()
   {  // setup
      const custom::array<Spy, 0> a{};
      // exercise
      bool empty = a.empty();
      // verify
      assertUnit(empty == true);
   }  // teardown

   // standard array empty?
   void test_empty_standard()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      // exercise
      bool empty = a.empty();
      // verify
      assertUnit(empty == false);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown


   /***************************************
    * SUBSCRIPT []
    ***************************************/

   // attempt to read an element
   void test_subscript_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      Spy s(0);
      Spy::reset();
      // exercise
      s = a[1];
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 1);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(s == Spy(49));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // write an element
   void test_subscript_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy s(99);
      Spy::reset();
      // exercise
      a[1] = s;
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 1);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 99 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(a[1] == Spy(99));
      a[1] = Spy(49);
      assertStandardFixture(a);
   }  // teardown

   // attempt to read an element
   void test_at_readValid()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      Spy s(0);
      Spy::reset();
      // exercise
      s = a.at(1);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 1);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(s == Spy(49));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown
   
   // write an element
   void test_at_writeValid()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy s(99);
      Spy::reset();
      // exercise
      a.at(1) = s;
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 1);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 99 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(a[1] == Spy(99));
      a[1] = Spy(49);
      assertStandardFixture(a);
   }  // teardown

   // attempt to read an element
   void test_at_readError()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      Spy s(0);
      Spy::reset();
      // exercise
      try
      {
         s = a.at(10);
         assertUnit(false);
         // verify
      }
      catch (std::out_of_range e)
      {
         // intentially left blank
      }
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(s == Spy(0));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown
   
   // write an element
   void test_at_writeError()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy s(99);
      Spy::reset();
      // exercise
      try
      {
         a.at(10) = s;
         // verify
         assertUnit(false);
      }
      catch (std::out_of_range e)
      {
         // intentially left blank
      }
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown
   
   /***************************************
    * FRONT and BACK
    ***************************************/
   
   // read from the front of the standard fixture
   void test_front_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      Spy s(99);
      Spy::reset();
      // exerciase
      s = a.front();
      // verify
      assertUnit(Spy::numAssign() == 1); // s = a[0]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(s == Spy(26));
      assertStandardFixture(a);
   }  // teardown

   // write to the front of the standard fixture
   void test_front_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy s(99);
      Spy::reset();
      // exerciase
      a.front() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);  // a[0] = 99
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(a.__elems_[0] == Spy(99));
      a.__elems_[0] = Spy(26);
      assertStandardFixture(a);
   }  // teardown
   
   // read from the back of the standard fixture
   void test_back_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> aRW;
      setupStandardFixture(aRW);
      const custom::array<Spy, 4> a(aRW);
      Spy s(99);
      Spy::reset();
      // exerciase
      s = a.back();
      // verify
      assertUnit(Spy::numAssign() == 1);  // s = a[3]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(s == Spy(89));
      assertStandardFixture(a);
   }  // teardown
   
   // write to the back of the standard fixture
   void test_back_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy s(99);
      Spy::reset();
      // exerciase
      a.back() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);  // a[3] = 99
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertUnit(a.__elems_[3] == Spy(99));
      a.__elems_[3] = Spy(89);
      assertStandardFixture(a);
   }  // teardown

   
   /***************************************
    * ITERATOR
    ***************************************/
   
   // beginning of the array
   void test_begin()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator it;
      // exercise
      it = a.begin();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //      it
      assertUnit(it.p == &(a.__elems_[0]));
   }  // teardown
  
   // one past the end of the array
   void test_end()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator it;
      // exercise
      it = a.end();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                           it
      assertUnit(it.p == &(a.__elems_[4]));
   }  // teardown

   // Default iterator
   void test_iterator_default()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy::reset();
      // exercise
      custom::array<Spy, 4>::iterator it;
      // verify
      assertUnit(it.p == nullptr);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // Non-default iterator
   void test_iterator_nondefault()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      Spy::reset();
      // exercise
      custom::array<Spy, 4>::iterator it(&(a.__elems_[2]));
      // verify
      assertUnit(it.p == &(a.__elems_[2]));
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      assertStandardFixture(a);
   }  // teardown

   // Assign an iterator
   void test_iterator_copy()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);    
      Spy::reset();
      // exercise
      custom::array<Spy, 4>::iterator itLHS(itRHS);
      // verify
      assertUnit(itLHS.p == itRHS.p);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

  // Assign an iterator
   void test_iterator_assign()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      custom::array<Spy, 4>::iterator itLHS;
      Spy::reset();
      // exercise
      itLHS = itRHS;
      // verify
      assertUnit(itLHS.p == itRHS.p);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // Two equals iterators should be equals
   void test_iterator_equals_equals()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               itLHS
      //               itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itLHS;
      itLHS.p = &(a.__elems_[2]);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      bool b;
      Spy::reset();
      // exercise
      b = (itLHS == itRHS);
      // verify
      assertUnit(b == true);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // Same array, different values
   void test_iterator_equals_differentValues()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //    itLHS      itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itLHS;
      itLHS.p = &(a.__elems_[0]);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      bool b;
      Spy::reset();
      // exercise
      b = (itLHS == itRHS);
      // verify
      assertUnit(b == false);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // Same values, different arrays
   void test_iterator_equals_differentArrays()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //    itLHS      
      custom::array<Spy, 4> aLHS;
      setupStandardFixture(aLHS);
      custom::array<Spy, 4>::iterator itLHS;
      itLHS.p = &(aLHS.__elems_[0]);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //    itRHS      
      custom::array<Spy, 4> aRHS;
      setupStandardFixture(aRHS);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(aRHS.__elems_[0]);
      bool b;
      Spy::reset();
      // exercise
      b = (itLHS == itRHS);
      // verify
      assertUnit(b == false);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(aLHS);
      assertStandardFixture(aRHS);
   }  // teardown   
   
   // Two equals iterators should be equals
   void test_iterator_notequals_equals()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               itLHS
      //               itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itLHS;
      itLHS.p = &(a.__elems_[2]);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      bool b;
      Spy::reset();
      // exercise
      b = (itLHS != itRHS);
      // verify
      assertUnit(b == false);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // Same array, different values
   void test_iterator_notequals_differentValues()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //    itLHS      itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itLHS;
      itLHS.p = &(a.__elems_[0]);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      bool b;
      Spy::reset();
      // exercise
      b = (itLHS == itRHS);
      // verify
      assertUnit(b != true);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(a);
   }  // teardown

   // Same values, different arrays
   void test_iterator_notequals_differentArrays()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //    itLHS      
      custom::array<Spy, 4> aLHS;
      setupStandardFixture(aLHS);
      custom::array<Spy, 4>::iterator itLHS;
      itLHS.p = &(aLHS.__elems_[0]);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //    itRHS      
      custom::array<Spy, 4> aRHS;
      setupStandardFixture(aRHS);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(aRHS.__elems_[0]);
      bool b;
      Spy::reset();
      // exercise
      b = (itLHS != itRHS);
      // verify
      assertUnit(b == true);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(aLHS);
      assertStandardFixture(aRHS);
   }  // teardown      
   
   // Dereference an iterator
   void test_iterator_dereferenceRead()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               it
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator it;
      it.p = &(a.__elems_[2]);
      Spy s;
      Spy::reset();
      // exercise
      s = *it;
      // verify
      assertUnit(Spy::numAlloc() == 1);     // allocate for 67
      assertUnit(Spy::numAssign() == 1);    // s = a[2]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(s == Spy(67));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      assertStandardFixture(a);
   }  // teardown

   // Dereference an iterator
   void test_iterator_dereferenceWrite()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               it
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator it;
      it.p = &(a.__elems_[2]);
      Spy s(99);
      Spy::reset();
      // exercise
      *it = s;
      // verify
      assertUnit(Spy::numAssign() == 1);    // a[2] = 99
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 99 | 89 |
      //    +----+----+----+----+
      //                it
      assertUnit(a.__elems_[2] == Spy(99));
      a.__elems_[2] = Spy(67);
      assertStandardFixture(a);
   }  // teardown

   // Increment an iterator
   void test_iterator_incrementPrefix()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      custom::array<Spy, 4>::iterator itLHS;
      Spy::reset();
      // exercise
      itLHS = ++itRHS;
      // verify
      assertUnit(Spy::numAssign() == 0);   
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                     itRHS
      //                     itLHS
      assertUnit(&(a.__elems_[3]) == itLHS.p);
      assertUnit(&(a.__elems_[3]) == itRHS.p);
      assertStandardFixture(a);
   }  // teardown

   // Increment an iterator
   void test_iterator_incrementPostfix()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //               itRHS
      custom::array<Spy, 4> a;
      setupStandardFixture(a);
      custom::array<Spy, 4>::iterator itRHS;
      itRHS.p = &(a.__elems_[2]);
      custom::array<Spy, 4>::iterator itLHS;
      Spy::reset();
      // exercise
      itLHS = itRHS++;
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                     itRHS
      //              itLHS
      assertUnit(&(a.__elems_[2]) == itLHS.p);
      assertUnit(&(a.__elems_[3]) == itRHS.p);
      assertStandardFixture(a);
   }  // teardown

   /*************************************************************
    * SETUP STANDARD FIXTURE
    *      0    1    2    3
    *    +----+----+----+----+
    *    | 26 | 49 | 67 | 89 |
    *    +----+----+----+----+
    *************************************************************/
   void setupStandardFixture(custom::array<Spy, 4> & a)
   {
      a.__elems_[0] = Spy(26);
      a.__elems_[1] = Spy(49);
      a.__elems_[2] = Spy(67);
      a.__elems_[3] = Spy(89);
   }

   void setupStandardFixture(std::array<Spy, 4> & a)
   {
      a[0] = Spy(26);
      a[1] = Spy(49);
      a[2] = Spy(67);
      a[3] = Spy(89);
   }

   /*************************************************************
    * VERIFY STANDARD FIXTURE PARAMETERS
    *      0    1    2    3
    *    +----+----+----+----+
    *    | 26 | 49 | 67 | 89 |
    *    +----+----+----+----+
    *************************************************************/
   void assertStandardFixtureParameters(const custom::array<Spy, 4> & a, int line, const char * function)
   {
      assertIndirect(a.size() == 4);
      assertIndirect(a.__elems_[0] == Spy(26));
      assertIndirect(a.__elems_[1] == Spy(49));
      assertIndirect(a.__elems_[2] == Spy(67));
      assertIndirect(a.__elems_[3] == Spy(89));
      assertIndirect(sizeof(a) == sizeof(Spy) * 4);

   }

   
   void assertStandardFixtureParameters(const std::array<Spy, 4> & a, int line, const char * function)
   {
      assertIndirect(a.size() == 4);
      assertIndirect(a[0] == Spy(26));
      assertIndirect(a[1] == Spy(49));
      assertIndirect(a[2] == Spy(67));
      assertIndirect(a[3] == Spy(89));
   }


};

#endif // DEBUG
