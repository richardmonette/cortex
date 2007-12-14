//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef IE_CORE_MARCHINGCUBES_H
#define IE_CORE_MARCHINGCUBES_H

#include <cassert>

#include "OpenEXR/ImathVec.h"

#include "IECore/VectorTypedData.h"
#include "IECore/MeshPrimitive.h"
#include "IECore/MeshPrimitiveBuilder.h"
#include "IECore/ImplicitSurfaceFunction.h"

namespace IECore
{

/// Templated implementation of "Efficient implementation of Marching Cubes cases with topological guarantees", Thomas Lewiner et al 2003, http://cuca.mat.puc-rio.br/~tomlew
template< typename ImplicitFn = ImplicitSurfaceFunction<Imath::V3f, float>, typename MeshBuilder = MeshPrimitiveBuilder<float> >
class MarchingCubes : public RefCounted
{
	public :
		typedef ImplicitFn ImplicitFnType;
		typedef MeshBuilder MeshBuilderType;
		
		typedef typename ImplicitFn::Point Point;
		typedef typename ImplicitFn::PointBaseType PointBaseType;
		typedef typename ImplicitFn::ValueBaseType ValueBaseType;
		
		typedef typename Imath::Vec3<PointBaseType> Vector;
		typedef Imath::Box<Vector> BoxType;
		
		MarchingCubes( typename ImplicitFn::Ptr fn, typename MeshBuilder::Ptr builder );

		virtual ~MarchingCubes();
				
		void march( const BoxType &bound, const Imath::V3i &res, ValueBaseType iso = (ValueBaseType)0.0 );

	protected :
	
		inline Point gridToWorld( const PointBaseType i, const PointBaseType j, const PointBaseType k ) const;
				
		inline ValueBaseType getIsoValue( const int i, const int j, const int k );

		void processCube();
		
		bool testFace( signed char face );

		bool testInterior( signed char s );

		void computeIntersectionPoints( ValueBaseType iso );

		void addTriangle( const char* trig, char n, int v12 = -1 );

		int addVertexX();
		int addVertexY();
		int addVertexZ();
		int addVertexC();

		Vector getGradient( const int i, const int j, const int k );

		ValueBaseType getGradientX( const int i, const int j, const int k );
		ValueBaseType getGradientY( const int i, const int j, const int k );		
		ValueBaseType getGradientZ( const int i, const int j, const int k );

		inline int getVertX( const int i, const int j, const int k ) const;		
		inline int getVertY( const int i, const int j, const int k ) const;		
		inline int getVertZ( const int i, const int j, const int k ) const;

		inline void setVertX( const int val, const int i, const int j, const int k );		
		inline void setVertY( const int val, const int i, const int j, const int k );		
		inline void setVertZ( const int val, const int i, const int j, const int k );

		BoxType m_bound;

		IECore::V3iVectorDataPtr m_verts;

		Imath::V3i m_currentGridPos;

		ValueBaseType m_currentCubeValues[8];         
		unsigned char m_lutEntry; 
		unsigned char m_currentCase; 
		unsigned char m_currentConfig; 
		unsigned char m_currentSubConfig; 
		
		typename ImplicitFn::Ptr m_fn;

		typename MeshBuilder::Ptr m_builder;

		Imath::V3i m_resolution;
		
		unsigned m_numVerts;
		
		typedef TypedData< std::vector< Imath::Vec3<PointBaseType > > > V3xVectorData;
		typedef boost::intrusive_ptr< TypedData< std::vector< Imath::Vec3<PointBaseType > > > > V3xVectorDataPtr;
		V3xVectorDataPtr m_P;
		V3xVectorDataPtr m_N;		
		
	private:
	
		/// Lookup tables.
		/// \todo We'd ideally like to make these consts, but a bug in gcc 4.0.2 seems to be preventing us.
		static char g_cases[256][2] ;
		static char g_tiling1[16][3] ;
		static char g_tiling2[24][6] ;
		static char g_test3[24] ;
		static char g_tiling3_1[24][6] ;
		static char g_tiling3_2[24][12] ;
		static char g_test4[8] ;
		static char g_tiling4_1[8][6] ;
		static char g_tiling4_2[8][18] ;
		static char g_tiling5[48][9] ;
		static char g_test6[48][3] ;
		static char g_tiling6_1_1[48][9] ;
		static char g_tiling6_1_2[48][21] ;
		static char g_tiling6_2[48][15] ;
		static char g_test7[16][5] ;
		static char g_tiling7_1[16][9] ;
		static char g_tiling7_2[16][3][15] ;
		static char g_tiling7_3[16][3][27] ;
		static char g_tiling7_4_1[16][15] ;
		static char g_tiling7_4_2[16][27] ;
		static char g_tiling8[6][6] ;
		static char g_tiling9[8][12] ;
		static char g_test10[6][3] ;
		static char g_tiling10_1_1[6][12] ;
		static char g_tiling10_1_1_[6][12] ;
		static char g_tiling10_1_2[6][24] ;
		static char g_tiling10_2[6][24] ;
		static char g_tiling10_2_[6][24] ;
		static char g_tiling11[12][12] ;
		static char g_test12[24][4] ;
		static char g_tiling12_1_1[24][12] ;
		static char g_tiling12_1_1_[24][12] ;
		static char g_tiling12_1_2[24][24] ;
		static char g_tiling12_2[24][24] ;
		static char g_tiling12_2_[24][24] ;
		static char g_test13[2][7] ;
		static char g_subconfig13[64] ;
		static char g_tiling13_1[2][12] ;
		static char g_tiling13_1_[2][12] ;
		static char g_tiling13_2[2][6][18] ;
		static char g_tiling13_2_[2][6][18] ;
		static char g_tiling13_3[2][12][30] ;
		static char g_tiling13_3_[2][12][30] ;
		static char g_tiling13_4[2][4][36] ;
		static char g_tiling13_5_1[2][4][18] ;
		static char g_tiling13_5_2[2][4][30] ;
		static char g_tiling14[12][12] ;		
};

}

#include "IECore/MarchingCubes.inl"

#endif // IE_CORE_MARCHINGCUBES_H
