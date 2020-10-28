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
#ifndef elxFixedSmoothingPyramid_h
#define elxFixedSmoothingPyramid_h

#include "elxIncludes.h" // include first to avoid MSVS warning
#include "itkMultiResolutionGaussianSmoothingPyramidImageFilter.h"

namespace elastix
{

/**
 * \class FixedSmoothingPyramid
 * \brief A pyramid based on the itk::MultiResolutionGaussianSmoothingPyramidImageFilter.
 *
 * The parameters used in this class are:
 * \parameter FixedImagePyramid: Select this pyramid as follows:\n
 *    <tt>(FixedImagePyramid "FixedSmoothingImagePyramid")</tt>
 *
 * \ingroup ImagePyramids
 */

template <class TElastix>
class FixedSmoothingPyramid
  : public itk::MultiResolutionGaussianSmoothingPyramidImageFilter<
      typename FixedImagePyramidBase<TElastix>::InputImageType,
      typename FixedImagePyramidBase<TElastix>::OutputImageType>
  , public FixedImagePyramidBase<TElastix>
{
public:
  /** Standard ITK-stuff. */
  typedef FixedSmoothingPyramid Self;
  typedef itk::MultiResolutionGaussianSmoothingPyramidImageFilter<
    typename FixedImagePyramidBase<TElastix>::InputImageType,
    typename FixedImagePyramidBase<TElastix>::OutputImageType>
                                          Superclass1;
  typedef FixedImagePyramidBase<TElastix> Superclass2;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FixedSmoothingPyramid, itk::MultiResolutionGaussianSmoothingPyramidImageFilter);

  /** Name of this class.
   * Use this name in the parameter file to select this specific pyramid. \n
   * example: <tt>(FixedImagePyramid "FixedSmoothingImagePyramid")</tt>\n
   */
  elxClassNameMacro("FixedSmoothingImagePyramid");

  /** Get the ImageDimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass1::ImageDimension);

  /** Typedefs inherited from the superclass. */
  typedef typename Superclass1::InputImageType         InputImageType;
  typedef typename Superclass1::OutputImageType        OutputImageType;
  typedef typename Superclass1::InputImagePointer      InputImagePointer;
  typedef typename Superclass1::OutputImagePointer     OutputImagePointer;
  typedef typename Superclass1::InputImageConstPointer InputImageConstPointer;
  typedef typename Superclass1::ScheduleType           ScheduleType;

  /** Typedefs inherited from Elastix. */
  typedef typename Superclass2::ElastixType          ElastixType;
  typedef typename Superclass2::ElastixPointer       ElastixPointer;
  typedef typename Superclass2::ConfigurationType    ConfigurationType;
  typedef typename Superclass2::ConfigurationPointer ConfigurationPointer;
  typedef typename Superclass2::RegistrationType     RegistrationType;
  typedef typename Superclass2::RegistrationPointer  RegistrationPointer;
  typedef typename Superclass2::ITKBaseType          ITKBaseType;

protected:
  /** The constructor. */
  FixedSmoothingPyramid() {}
  /** The destructor. */
  ~FixedSmoothingPyramid() override {}

private:
  /** The private constructor. */
  FixedSmoothingPyramid(const Self &); // purposely not implemented
  /** The private copy constructor. */
  void
  operator=(const Self &); // purposely not implemented
};

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#  include "elxFixedSmoothingPyramid.hxx"
#endif

#endif // end #ifndef elxFixedSmoothingPyramid_h
