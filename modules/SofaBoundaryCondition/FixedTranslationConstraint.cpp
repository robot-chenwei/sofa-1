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
#define SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_FIXEDTRANSLATIONCONSTRAINT_CPP
#include <SofaBoundaryCondition/FixedTranslationConstraint.inl>
#include <sofa/core/ObjectFactory.h>

#include <sofa/defaulttype/RigidTypes.h>

namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{

using namespace sofa::defaulttype;
using namespace sofa::helper;

int FixedTranslationConstraintClass = core::RegisterObject("Attach given rigids to their initial positions but they still can have rotations")
#ifndef SOFA_FLOAT
        .add< FixedTranslationConstraint<Rigid3dTypes> >()
        .add< FixedTranslationConstraint<Rigid2dTypes> >()
        .add< FixedTranslationConstraint<Vec6dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< FixedTranslationConstraint<Rigid3fTypes> >()
        .add< FixedTranslationConstraint<Rigid2fTypes> >()
        .add< FixedTranslationConstraint<Vec6fTypes> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class SOFA_BOUNDARY_CONDITION_API FixedTranslationConstraint<Rigid3dTypes>;
template class SOFA_BOUNDARY_CONDITION_API FixedTranslationConstraint<Rigid2dTypes>;
template class SOFA_BOUNDARY_CONDITION_API FixedTranslationConstraint<Vec6dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_BOUNDARY_CONDITION_API FixedTranslationConstraint<Rigid3fTypes>;
template class SOFA_BOUNDARY_CONDITION_API FixedTranslationConstraint<Rigid2fTypes>;
template class SOFA_BOUNDARY_CONDITION_API FixedTranslationConstraint<Vec6fTypes>;
#endif

} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa

