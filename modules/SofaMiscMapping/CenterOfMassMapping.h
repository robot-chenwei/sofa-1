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
#ifndef SOFA_COMPONENT_MAPPING_CENTEROFMASSMAPPING_H
#define SOFA_COMPONENT_MAPPING_CENTEROFMASSMAPPING_H
#include "config.h"

#include <sofa/core/Mapping.h>
#include <sofa/core/behavior/BaseMass.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>

namespace sofa
{

namespace component
{

namespace mapping
{

/** mapping computing the center of mass of an object.
	the output of the mapping has to be a single dof.
	Its position is then set from the input DOFs, proportionally to their mass.
	This allow to control an object by setting forces on its center of mass.
 */
template <class TIn, class TOut>
class CenterOfMassMapping : public core::Mapping<TIn, TOut>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE2(CenterOfMassMapping,TIn,TOut), SOFA_TEMPLATE2(core::Mapping,TIn,TOut));

    typedef core::Mapping<TIn, TOut> Inherit;

    // Input types
    typedef TIn In;
    typedef typename In::Real Real;
    typedef typename In::Coord InCoord;
    typedef typename In::Deriv InDeriv;
    typedef typename In::VecCoord InVecCoord;
    typedef typename In::VecDeriv InVecDeriv;
    typedef typename In::MatrixDeriv InMatrixDeriv;
    typedef typename In::Real InReal;

    typedef Data<InVecCoord> InDataVecCoord;
    typedef Data<InVecDeriv> InDataVecDeriv;
    typedef Data<InMatrixDeriv> InDataMatrixDeriv;

    // Output types
    typedef TOut Out;
    typedef typename Out::VecCoord OutVecCoord;
    typedef typename Out::VecDeriv OutVecDeriv;
    typedef typename Out::Coord OutCoord;
    typedef typename Out::Deriv OutDeriv;
    typedef typename Out::MatrixDeriv OutMatrixDeriv;
    typedef typename Out::Real OutReal;

    typedef Data<OutVecCoord> OutDataVecCoord;
    typedef Data<OutVecDeriv> OutDataVecDeriv;
    typedef Data<OutMatrixDeriv> OutDataMatrixDeriv;

    void init() override;

    virtual void apply( const sofa::core::MechanicalParams* mparams, OutDataVecCoord& out, const InDataVecCoord& in) override;
    //void apply( typename Out::VecCoord& out, const typename In::VecCoord& in );

    virtual void applyJ( const sofa::core::MechanicalParams* mparams, OutDataVecDeriv& out, const InDataVecDeriv& in) override;
    //void applyJ( typename Out::VecDeriv& out, const typename In::VecDeriv& in );

    virtual void applyJT( const sofa::core::MechanicalParams* mparams, InDataVecDeriv& out, const OutDataVecDeriv& in) override;
    //void applyJT( typename In::VecDeriv& out, const typename Out::VecDeriv& in );

    virtual void applyJT( const sofa::core::ConstraintParams* /*cparams*/, InDataMatrixDeriv& /*out*/, const OutDataMatrixDeriv& /*in*/) override
    {
        serr << "applyJT(constraint ) not implemented" << sendl;
    }

    //void applyJT( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in );

    void draw(const core::visual::VisualParams* vparams) override;


protected :
    CenterOfMassMapping ( )
        : Inherit ()
    {}

    virtual ~CenterOfMassMapping()
    {}

    ///pointer on the input DOFs mass
    sofa::core::behavior::BaseMass * masses;

    /// the total mass of the input object
    double totalMass;
};


#if  !defined(SOFA_COMPONENT_MAPPING_CENTEROFMASSMAPPING_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3dTypes, sofa::defaulttype::Vec3dTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid2dTypes, sofa::defaulttype::Vec2dTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3dTypes, sofa::defaulttype::ExtVec3dTypes >;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3fTypes, sofa::defaulttype::Vec3fTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid2fTypes, sofa::defaulttype::Vec2fTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3fTypes, sofa::defaulttype::ExtVec3fTypes >;
#endif

#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3dTypes, sofa::defaulttype::Vec3fTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid2dTypes, sofa::defaulttype::Vec2fTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3dTypes, sofa::defaulttype::ExtVec3fTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3fTypes, sofa::defaulttype::Vec3dTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid2fTypes, sofa::defaulttype::Vec2dTypes >;
extern template class SOFA_MISC_MAPPING_API CenterOfMassMapping< sofa::defaulttype::Rigid3fTypes, sofa::defaulttype::ExtVec3dTypes >;
#endif
#endif
#endif

} // namespace mapping

} // namespace component

} // namespace sofa

#endif
