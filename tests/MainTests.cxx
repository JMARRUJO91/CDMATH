/*
 * tests_mesh.cxx
 *
 *  Created on: 24 janv. 2012
 *      Authors: CDMAT
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/CompilerOutputter.h>

#include "MatrixTests.hxx"
#include "VectorTests.hxx"
#include "PointTests.hxx"
#include "NodeTests.hxx"
#include "CellTests.hxx"
#include "FaceTests.hxx"
#include "FieldTests.hxx"
#include "MeshTests.hxx"
#include "DoubleTabTests.hxx"
#include "IntTabTests.hxx"
#include "LinearSolverTests.hxx"

#include <iostream>

using namespace std;


int main( int argc, char* argv[] )
{
   // Create the event manager and test controller
   CppUnit::TestResult controller;
   // Add a listener that colllects test result
   CppUnit::TestResultCollector result;
   controller.addListener( &result );
   // Add a listener that print dots as test run.
   CppUnit::BriefTestProgressListener progress;
   controller.addListener( &progress );
   // Add the top suite to the test runner
   CppUnit::TextUi::TestRunner runner;
   runner.addTest( MatrixTests::suite() );
   runner.addTest( VectorTests::suite() );
   runner.addTest( IntTabTests::suite() );
   runner.addTest( DoubleTabTests::suite() );
   runner.addTest( PointTests::suite() );
   runner.addTest( NodeTests::suite() );
   runner.addTest( CellTests::suite() );
   runner.addTest( FaceTests::suite() );
   runner.addTest( FieldTests::suite() );
   runner.addTest( MeshTests::suite() );
   runner.addTest( IntTabTests::suite() );
   runner.addTest( DoubleTabTests::suite() );
   runner.addTest( LinearSolverTests::suite() );
   runner.run( controller );
   CppUnit::CompilerOutputter outputter( &result, CppUnit::stdCOut() );
   outputter.write();
   ofstream xmlFileOut("cppUnitResults.xml");
   CppUnit::XmlOutputter xmlOut(&result, xmlFileOut);
   xmlOut.write();
   return result.wasSuccessful() ? 0 : 1;
}
