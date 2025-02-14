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
#include <SofaGeneralTopology/CylinderGridTopology.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/rmath.h>

namespace sofa
{

namespace component
{

namespace topology
{

using namespace sofa::defaulttype;



int CylinderGridTopologyClass = core::RegisterObject("Cylinder grid in 3D")
        .addAlias("CylinderGrid")
        .add< CylinderGridTopology >()
        ;

CylinderGridTopology::CylinderGridTopology(int nx, int ny, int nz)
    : GridTopology(nx, ny, nz)
    , d_center(initData(&d_center,Vector3(0.0f,0.0f,0.0f),"center", "Center of the cylinder"))
    , d_axis(initData(&d_axis,Vector3(0.0f,0.0f,1.0f),"axis", "Main direction of the cylinder"))
    , d_radius(initData(&d_radius,(SReal)1.0,"radius", "Radius of the cylinder"))
    , d_length(initData(&d_length,(SReal)1.0,"length", "Length of the cylinder along its axis"))
{
}

CylinderGridTopology::CylinderGridTopology()
    : GridTopology()
    , d_center(initData(&d_center,Vector3(0.0f,0.0f,0.0f),"center", "Center of the cylinder"))
    , d_axis(initData(&d_axis,Vector3(0.0f,0.0f,1.0f),"axis", "Main direction of the cylinder"))
    , d_radius(initData(&d_radius,(SReal)1.0,"radius", "Radius of the cylinder"))
    , d_length(initData(&d_length,(SReal)1.0,"length", "Length of the cylinder along its axis"))
{
}

void CylinderGridTopology::setCenter(SReal x, SReal y, SReal z)
{
    d_center.setValue(Vector3(x,y,z));
}

void CylinderGridTopology::setAxis(SReal x, SReal y, SReal z)
{
    d_axis.setValue(Vector3(x,y,z));
}

void CylinderGridTopology::setRadius(SReal radius)
{
    d_radius.setValue(radius);
}

void CylinderGridTopology::setLength(SReal length)
{
    d_length.setValue(length);
}

Vector3 CylinderGridTopology::getPointInGrid(int i, int j, int k) const
{
    //return p0+dx*x+dy*y+dz*z;
    SReal r = d_radius.getValue();
    SReal l = d_length.getValue();
    Vector3 axisZ = d_axis.getValue();
    axisZ.normalize();
    Vector3 axisX = ((axisZ-Vector3(1,0,0)).norm() < 0.000001 ? Vector3(0,1,0) : Vector3(1,0,0));
    Vector3 axisY = cross(axisZ,axisX);
    axisX = cross(axisY,axisZ);
    axisX.normalize();
    axisY.normalize();
    axisZ.normalize();
    int nx = getNx();
    int ny = getNy();
    int nz = getNz();
    // coordonate on a square
    Vector3 p(i*2*r/(nx-1) - r, j*2*r/(ny-1) - r, 0);
    // scale it to be on a circle
    if (p.norm() > 0.0000001)
        p *= helper::rmax(helper::rabs(p[0]),helper::rabs(p[1]))/p.norm();
    if (nz>1)
        p[2] = k*l/(nz-1);
    return d_center.getValue()+axisX*p[0] + axisY*p[1] + axisZ * p[2];
}

} // namespace topology

} // namespace component

} // namespace sofa

