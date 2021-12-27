/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef itkDeformationFieldRegulizer_h
#define itkDeformationFieldRegulizer_h

#include "itkDeformationVectorFieldTransform.h"
#include "itkImageRegionIterator.h"

namespace itk
{

/**
 * \class DeformationFieldRegulizer
 * \brief This class combines any itk transform with the
 * DeformationFieldTransform.
 *
 * This class is a base class for Transforms that also use
 * a diffusion / regularization of the deformation field.
 *
 * \ingroup Transforms
 * \ingroup Common
 */

template <class TAnyITKTransform>
class ITK_TEMPLATE_EXPORT DeformationFieldRegulizer : public TAnyITKTransform
{
public:
  /** Standard itk. */
  typedef DeformationFieldRegulizer Self;
  typedef TAnyITKTransform          Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DeformationFieldRegulizer, TAnyITKTransform);

  /** Input space dimension. */
  itkStaticConstMacro(InputSpaceDimension, unsigned int, Superclass::InputSpaceDimension);
  /** Output space dimension. */
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, Superclass::OutputSpaceDimension);

  /** Typedef's inherited from Superclass. */
  using typename Superclass::ScalarType;
  using typename Superclass::ParametersType;
  using typename Superclass::JacobianType;
  using typename Superclass::InputVectorType;
  using typename Superclass::OutputVectorType;
  using typename Superclass::InputCovariantVectorType;
  using typename Superclass::OutputCovariantVectorType;
  using typename Superclass::InputVnlVectorType;
  using typename Superclass::OutputVnlVectorType;
  using typename Superclass::InputPointType;
  using typename Superclass::OutputPointType;

  /** Typedef's needed in this class. */
  typedef DeformationVectorFieldTransform<ScalarType, Self::InputSpaceDimension> IntermediaryDFTransformType;
  typedef typename IntermediaryDFTransformType::Pointer                          IntermediaryDFTransformPointer;
  typedef typename IntermediaryDFTransformType ::CoefficientVectorImageType      VectorImageType;
  typedef typename VectorImageType::PixelType                                    VectorPixelType;
  typedef ImageRegionIterator<VectorImageType>                                   IteratorType;

  /** Typedef's for the vectorImage. */
  typedef typename VectorImageType::RegionType  RegionType;
  typedef typename VectorImageType::SpacingType SpacingType;
  typedef typename VectorImageType::PointType   OriginType;

  /** Function to create and initialze the deformation fields. */
  void
  InitializeDeformationFields();

  /** Function to update the intermediary deformation field by adding
   * a diffused deformation field to it.
   */
  virtual void
  UpdateIntermediaryDeformationFieldTransform(typename VectorImageType::Pointer vecImage);

  /** itk Set macro for the region of the deformation field. */
  itkSetMacro(DeformationFieldRegion, RegionType);

  /** itk Set macro for the spacing of the deformation field. */
  itkSetMacro(DeformationFieldSpacing, SpacingType);

  /** itk Set macro for the origin of the deformation field. */
  itkSetMacro(DeformationFieldOrigin, OriginType);

  /** itk Get macro for the deformation field transform. */
  itkGetConstObjectMacro(IntermediaryDeformationFieldTransform, IntermediaryDFTransformType);

  /** Method to transform a point. */
  OutputPointType
  TransformPoint(const InputPointType & inputPoint) const override;

protected:
  /** The constructor. */
  DeformationFieldRegulizer();
  /** The destructor. */
  ~DeformationFieldRegulizer() override = default;

private:
  /** The deleted copy constructor. */
  DeformationFieldRegulizer(const Self &) = delete;
  /** The deleted assignment operator. */
  void
  operator=(const Self &) = delete;

  /** Declaration of members. */
  IntermediaryDFTransformPointer m_IntermediaryDeformationFieldTransform;
  bool                           m_Initialized;

  /** Declarations of region things. */
  RegionType  m_DeformationFieldRegion;
  OriginType  m_DeformationFieldOrigin;
  SpacingType m_DeformationFieldSpacing;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkDeformationFieldRegulizer.hxx"
#endif

#endif // end #ifndef itkDeformationFieldRegulizer_h
