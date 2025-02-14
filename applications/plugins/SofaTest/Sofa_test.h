/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
// Francois Faure, 2013

#ifndef SOFA_SOFATEST_BASETEST_H
#define SOFA_SOFATEST_BASETEST_H

#include "InitPlugin_test.h"
#include <sofa/helper/testing/BaseTest.h>
#include <sofa/helper/testing/NumericTest.h>
#include <SofaSimulationGraph/testing/BaseSimulationTest.h>

#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>
using sofa::defaulttype::Rigid3Types;
using sofa::defaulttype::Rigid3Mass;
using sofa::defaulttype::Vec3Types;
using sofa::defaulttype::Vec1Types;

namespace sofa
{
    using namespace sofa::helper::testing ;
    class SOFA_SOFATEST_API BaseSofa_test : public BaseSimulationTest
    {
    public:
        BaseSofa_test();
        virtual ~BaseSofa_test() ;
        void clearSceneGraph() ;
    } ;

    template<class R=SReal>
    class SOFA_SOFATEST_API Sofa_test : public NumericTest<R>, public BaseSofa_test
    {
    };

} /// namespace sofa

#endif /// SOFA_SOFATEST_BASETEST_H
