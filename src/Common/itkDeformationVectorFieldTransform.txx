/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    $RCSfile$
 Language:  C++
 Date:      $Date$
 Version:   $Revision$
 
  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
	
	This software is distributed WITHOUT ANY WARRANTY; without even 
	the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the above copyright notices for more information.
	 
=========================================================================*/
#ifndef __itkDeformationVectorFieldTransform_TXX__
#define __itkDeformationVectorFieldTransform_TXX__

#include "itkDeformationVectorFieldTransform.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkScalarToArrayCastImageFilter.h"

namespace itk
{
	

	/**
	 * *********************** Constructor **************************
	 */

	template< class TScalarType, unsigned int NDimensions >
		DeformationVectorFieldTransform< TScalarType, NDimensions >
		::DeformationVectorFieldTransform()//:Superclass(SpaceDimension,0)
	{
		/** Initialize m_Images. */
		for ( unsigned int i = 0; i < SpaceDimension; i++ )
		{
			this->m_Images[ i ] = 0;
		}
		
	} // end Constructor
	
	
	/**
	 * *********************** Destructor ***************************
	 */

	template< class TScalarType, unsigned int NDimensions >
		DeformationVectorFieldTransform< TScalarType, NDimensions >
		::~DeformationVectorFieldTransform()
	{
	} // end Destructor
	
		
	/**
	 * ******************* SetCoefficientImage **********************
	 *
	 * Convert VectorImage (= deformation field) to series of images.
	 * Set the B-Spline coefficients using a deformation field
	 * image as input.
	 */

	template< class TScalarType, unsigned int NDimensions >
		void
		DeformationVectorFieldTransform< TScalarType, NDimensions >
		::SetCoefficientImage( VectorImagePointer vecImage )
	{
		/** Typedef's for iterators. */
		typedef ImageRegionConstIterator< VectorImageType >		VectorIteratorType;
		typedef ImageRegionIterator< ImageType >							IteratorType;		

		/** Create array of images representing the B-spline
		 * coefficients in each dimension.
		 */
		for ( unsigned int i = 0; i < SpaceDimension; i++ )
		{
			this->m_Images[ i ] = ImageType::New();
			this->m_Images[ i ]->SetRegions(	vecImage->GetLargestPossibleRegion() );
			this->m_Images[ i ]->SetOrigin(		vecImage->GetOrigin() );
			this->m_Images[ i ]->SetSpacing(	vecImage->GetSpacing() );
			this->m_Images[ i ]->Allocate();
		}

		/** Setup the iterators. */
		VectorIteratorType vecit( vecImage, vecImage->GetLargestPossibleRegion() );
		vecit.GoToBegin();
		IteratorType it[ SpaceDimension ];
		for ( unsigned int i = 0; i < SpaceDimension; i++ )
		{
			it[ i ] = IteratorType( this->m_Images[ i ], this->m_Images[ i ]->GetLargestPossibleRegion() );
			it[ i ].GoToBegin();
		}

		/** Copy one element of a vector to an image. */
		VectorPixelType vect;
		while ( !vecit.IsAtEnd() )
		{
			vect = vecit.Get();
			for ( unsigned int i = 0; i < SpaceDimension; i++ )
			{
				it[ i ].Set( vect[ i ] );
				++it[ i ];
			}
			++vecit;
		}

		/** Put it in the Superclass. */
		this->Superclass::SetCoefficientImage( this->m_Images );

	} // end SetCoefficientImage


	/**
	 * ******************* GetCoefficientImage **********************
	 *
	 * Convert series of coefficient images to VectorImage (= deformation field).
	 * 
	 */

	template< class TScalarType, unsigned int NDimensions >
		void
		DeformationVectorFieldTransform< TScalarType, NDimensions >
		::GetCoefficientVectorImage( VectorImagePointer & vecImage )
	{
		/** Typedef for the combiner. */
		typedef ScalarToArrayCastImageFilter<
			ImageType, VectorImageType >			ScalarImageCombineType;

		/** Get a handle to the series of coefficient images. */
		ImagePointer * coefImage;
		coefImage = this->GetCoefficientImage();

		/** Combine the coefficient images to a vector image. */
		typename ScalarImageCombineType::Pointer combiner = ScalarImageCombineType::New();
		for ( unsigned int i = 0; i < SpaceDimension; i++ )
		{
			combiner->SetInput( i, coefImage[ i ] );
		}
		vecImage = combiner->GetOutput();
		vecImage->Update();

	} // end SetCoefficientImage

  
} // end namespace itk

#endif // end #ifndef __itkDeformationVectorFieldTransform_TXX__
