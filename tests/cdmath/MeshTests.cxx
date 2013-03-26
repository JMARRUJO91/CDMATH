/*
 * meshtests.cxx
 *
 *  Created on: 24 janv. 2012
 *      Authors: CDMAT
 */

#include "MeshTests.hxx"
#include "MEDLoader.hxx"
#include "Face.hxx"
#include "Cell.hxx"
#include "Node.hxx"

#include <string>
#include <cmath>

using namespace ParaMEDMEM;
using namespace std;

//----------------------------------------------------------------------
void
MeshTests::testClassMesh( void )
//----------------------------------------------------------------------
{
    Mesh M1(0.0,1.0,4);
	CPPUNIT_ASSERT_EQUAL( 1, M1.getDim() );
	CPPUNIT_ASSERT_EQUAL( 5, M1.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 4, M1.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 5, M1.getNumberOfFaces() );
	double xinf=0.0;
	double xsup=4.0;
	double yinf=0.0;
	double ysup=4.0;
    Mesh M2(xinf,xsup,4,yinf,ysup,4);
	CPPUNIT_ASSERT_EQUAL( 2, M2.getDim() );
	CPPUNIT_ASSERT_EQUAL( 25, M2.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 16, M2.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 40, M2.getNumberOfFaces() );
	double x1=M2.getCells()[4].x();
	double y1=M2.getCells()[4].y();
	CPPUNIT_ASSERT_EQUAL( x1, 0.5 );
	CPPUNIT_ASSERT_EQUAL( y1, 1.5 );

	double x2=M2.getNodes()[24].x();
	double y2=M2.getNodes()[24].y();
	CPPUNIT_ASSERT_EQUAL( x2, 4. );
	CPPUNIT_ASSERT_EQUAL( y2, 4. );

	double eps=1.E-10;
    M2.setGroupAtPlan(xsup,0,eps,"RightEdge");
    M2.setGroupAtPlan(xinf,0,eps,"LeftEdge");
    M2.setGroupAtPlan(yinf,1,eps,"BottomEdge");
    M2.setGroupAtPlan(ysup,1,eps,"TopEdge");
	CPPUNIT_ASSERT_EQUAL( 4, int(M2.getNamesOfGroups().size()) );
	CPPUNIT_ASSERT(M2.getNamesOfGroups()[2].compare("BottomEdge")==0);
	int nbFaces=M2.getNumberOfFaces();
	IntTab indexFaces=M2.getIndexFacePeriodic();
	for (int i=0;i<nbFaces;i++)
	{
		double x=M2.getFaces()[i].x();
		double y=M2.getFaces()[i].y();
		if (abs(y)<1.E-10 && abs(x-0.5)<1.E-10)
		{
			int indexFace=M2.getIndexFacePeriodic(i);
			double xi=M2.getFace(indexFace).x();
			double yi=M2.getFace(indexFace).y();
			CPPUNIT_ASSERT_EQUAL( xi, x );
			CPPUNIT_ASSERT_EQUAL( yi, ysup );
			CPPUNIT_ASSERT_EQUAL( true, M2.getFace(indexFace).isBorder() );
			CPPUNIT_ASSERT_EQUAL( indexFace, indexFaces[i] );
		}

		if (abs(y-0.5)<1.E-10 && abs(x-1.)<1.E-10)
			CPPUNIT_ASSERT_EQUAL( -1, M2.getIndexFacePeriodic(i) );

	}

	M2.writeMED("TestMesh.med");
	Mesh M22("TestMesh.med");
	CPPUNIT_ASSERT_EQUAL( 2, M22.getDim() );
	CPPUNIT_ASSERT_EQUAL( 25, M22.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 16, M22.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 40, M22.getNumberOfFaces() );

	Mesh M23("maillage.med");
	CPPUNIT_ASSERT(M23.getNamesOfGroups()[0].compare("BORD1")==0);
	CPPUNIT_ASSERT(M23.getNamesOfGroups()[1].compare("BORD2")==0);
	CPPUNIT_ASSERT(M23.getNamesOfGroups()[2].compare("BORD3")==0);
	CPPUNIT_ASSERT(M23.getNamesOfGroups()[3].compare("BORD4")==0);

	Mesh M3(M1);
	CPPUNIT_ASSERT_EQUAL( 1, M3.getDim() );
	CPPUNIT_ASSERT_EQUAL( 5, M3.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 4, M3.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 5, M3.getNumberOfFaces() );

    M3=M2;
	CPPUNIT_ASSERT_EQUAL( 2, M3.getDim() );
	CPPUNIT_ASSERT_EQUAL( 25, M3.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 16, M3.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 40, M3.getNumberOfFaces() );

    Mesh M4;
    M4=M3;
	CPPUNIT_ASSERT_EQUAL( 2, M4.getDim() );
	CPPUNIT_ASSERT_EQUAL( 25, M4.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 16, M4.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 40, M4.getNumberOfFaces() );

	Mesh M5(0.0,1.0,4,0.0,1.0,4,0.0,1.0,4);
    CPPUNIT_ASSERT_EQUAL( 3, M5.getDim() );

    string fileNameVTK="TestMesh.vtu";
    M4.writeVTK(fileNameVTK) ;
    string fileNameMED="TestMesh.med";
    M4.writeMED(fileNameMED) ;
    Mesh M6(fileNameMED);
	CPPUNIT_ASSERT_EQUAL( 2, M6.getDim() );
	CPPUNIT_ASSERT_EQUAL( 25, M6.getNumberOfNodes() );
	CPPUNIT_ASSERT_EQUAL( 16, M6.getNumberOfCells() );
	CPPUNIT_ASSERT_EQUAL( 40, M6.getNumberOfFaces() );
}
