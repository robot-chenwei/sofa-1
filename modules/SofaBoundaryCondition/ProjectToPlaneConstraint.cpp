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
#define SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_ProjectToPlaneConstraint_CPP
#include <SofaBoundaryCondition/ProjectToPlaneConstraint.inl>
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/Node.h>

namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{

using namespace sofa::defaulttype;
using namespace sofa::helper;


int ProjectToPlaneConstraintClass = core::RegisterObject("Attach given particles to their initial positions")
#ifndef SOFA_FLOAT
        .add< ProjectToPlaneConstraint<Vec3dTypes> >()
        .add< ProjectToPlaneConstraint<Vec2dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< ProjectToPlaneConstraint<Vec3fTypes> >()
        .add< ProjectToPlaneConstraint<Vec2fTypes> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class SOFA_BOUNDARY_CONDITION_API ProjectToPlaneConstraint<Vec3dTypes>;
template class SOFA_BOUNDARY_CONDITION_API ProjectToPlaneConstraint<Vec2dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_BOUNDARY_CONDITION_API ProjectToPlaneConstraint<Vec3fTypes>;
template class SOFA_BOUNDARY_CONDITION_API ProjectToPlaneConstraint<Vec2fTypes>;
#endif


} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa

