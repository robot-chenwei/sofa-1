/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/*
 * DecimateMesh.h
 *
 *  Created on: 2nd of June 2010
 *      Author: Olivier
 */

#ifndef CGALPLUGIN_DECIMATEMESH_H
#define CGALPLUGIN_DECIMATEMESH_H

#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/DataEngine.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/helper/gl/template.h>


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>

// Adaptor for Polyhedron_3
#include <CGAL/Surface_mesh_simplification/HalfedgeGraph_Polyhedron_3.h>

// Simplification function
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>

// Stop-condition policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>

// Typedefs SOFA
typedef sofa::core::topology::BaseMeshTopology::Edge Edge;
typedef sofa::core::topology::BaseMeshTopology::Triangle Triangle;
typedef sofa::core::topology::BaseMeshTopology::SeqEdges SeqEdges;
typedef sofa::core::topology::BaseMeshTopology::SeqTriangles SeqTriangles;


// Typedefs CGAL
typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Polyhedron_3<Kernel> Surface;
typedef Surface::HalfedgeDS HalfedgeDS;
typedef Kernel::Point_3 Point;

namespace SMS = CGAL::Surface_mesh_simplification ;
using namespace sofa;
using namespace sofa::defaulttype;

namespace cgal
{

template <class DataTypes>
class DecimateMesh : public sofa::core::DataEngine
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(DecimateMesh,DataTypes),sofa::core::DataEngine);

//        typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename Coord::value_type Real;
    typedef Vec<3,Real> Vec3;

public:
    DecimateMesh();
    virtual ~DecimateMesh();

    void init();
    void reinit();

    void doUpdate();
    void draw();
    void writeObj();
    void computeNormals();

    virtual std::string getTemplateName() const
    {
        return templateName(this);
    }

    static std::string templateName(const DecimateMesh<DataTypes>* = NULL)
    {
        return DataTypes::Name();
    }

    virtual void handleEvent(sofa::core::objectmodel::Event *event);


    void geometry_to_surface(Surface &s);
    void surface_to_geometry(Surface &s);
    bool testVertexAndFindIndex(const Vec3 &vertex, int &index);

    //Inputs
    sofa::core::objectmodel::Data<VecCoord> m_inVertices; ///< List of vertices
    sofa::core::objectmodel::Data<SeqTriangles> m_inTriangles; ///< List of triangles
    sofa::core::objectmodel::Data<int> m_edgesTarget; ///< Desired number of edges after simplification
    sofa::core::objectmodel::Data<float> m_edgesRatio; ///< Ratio between the number of edges and number of initial edges

    // Outputs
    sofa::core::objectmodel::Data<VecCoord> m_outVertices; ///< New vertices after decimation
    sofa::core::objectmodel::Data<SeqTriangles> m_outTriangles; ///< New triangles after decimation
    sofa::core::objectmodel::Data< helper::vector<Vec3> > m_outNormals; ///< New normals after decimation

    // Parameters
    sofa::core::objectmodel::Data<bool> m_writeToFile; ///< Writes the decimated mesh into a file


};


template <class DataTypes, class HDS>
class geometry_to_surface_op :  public CGAL::Modifier_base<HDS>
{
public:

    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;

    typedef HDS Halfedge_data_structure;

private:

    VecCoord m_vertices;
    SeqTriangles m_triangles;


public:

    geometry_to_surface_op(const VecCoord &vertices, const SeqTriangles &triangles)
//        geometry_to_surface_op(helper::ReadAccessor vertices, helper::ReadAccessor triangles)
    {
        m_vertices = vertices;
        m_triangles = triangles;
    }

    void operator()( HDS& hds)
    {
        unsigned int numVertices = m_vertices.size();
        unsigned int numTriangles = m_triangles.size();

        CGAL::Polyhedron_incremental_builder_3<HalfedgeDS> builder(hds, true);
        builder.begin_surface(numVertices, numTriangles);

        for (unsigned int i = 0; i < numVertices; i++)
        {
            builder.add_vertex( Point( m_vertices[i][0], m_vertices[i][1], m_vertices[i][2] ));
        }

        for (unsigned int i = 0; i < numTriangles; i++ )
        {
            builder.begin_facet();
            for ( int j = 0; j < 3; j++ )
            {
                builder.add_vertex_to_facet( m_triangles[i][j] );
            }
            std::cout << std::endl;
            builder.end_facet();
        }

        if (builder.check_unconnected_vertices())
        {
            builder.remove_unconnected_vertices();
        }

        builder.end_surface();
    }

};


#if  !defined(CGALPLUGIN_SIMPLIFICATIONMESH_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_CGALPLUGIN_API DecimateMesh<defaulttype::Vec3dTypes>;
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
extern template class SOFA_CGALPLUGIN_API DecimateMesh<defaulttype::Vec3fTypes>;
#endif //SOFA_DOUBLE
#endif

} //cgal

#endif /* CGALPLUGIN_DECIMATEMESH_H */
