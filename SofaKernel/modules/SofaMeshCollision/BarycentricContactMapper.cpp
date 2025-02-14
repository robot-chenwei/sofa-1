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
#define SOFA_COMPONENT_COLLISION_BARYCENTRICCONTACTMAPPER_CPP
#include <SofaMeshCollision/BarycentricContactMapper.inl>
#include <sofa/helper/Factory.inl>

namespace sofa
{

namespace component
{

namespace collision
{

using namespace defaulttype;

ContactMapperCreator< ContactMapper<LineModel> > LineContactMapperClass("default",true);
ContactMapperCreator< ContactMapper<TriangleModel> > TriangleContactMapperClass("default",true);
ContactMapperCreator< ContactMapper<CapsuleModel> > CapsuleContactMapperClass("default",true);

template class SOFA_MESH_COLLISION_API ContactMapper<LineModel, sofa::defaulttype::Vec3Types>;
template class SOFA_MESH_COLLISION_API ContactMapper<TriangleModel, sofa::defaulttype::Vec3Types>;
template class SOFA_MESH_COLLISION_API ContactMapper<CapsuleModel, sofa::defaulttype::Vec3Types>;

} // namespace collision

} // namespace component

} // namespace sofa

