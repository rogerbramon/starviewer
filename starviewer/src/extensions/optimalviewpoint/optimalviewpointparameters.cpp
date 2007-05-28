/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "optimalviewpointparameters.h"

#include "identifier.h"

namespace udg {

OptimalViewpointParameters::OptimalViewpointParameters( QObject * parent )
 : Parameters( parent )
{
}

OptimalViewpointParameters::~OptimalViewpointParameters()
{
}

const QString & OptimalViewpointParameters::getSegmentationFileName() const
{
    return m_segmentationFileName;
}

void OptimalViewpointParameters::setSegmentationFileName( const QString & segmentationFileName )
{
    if ( m_segmentationFileName != segmentationFileName )
    {
        m_segmentationFileName = segmentationFileName; emit changed( SegmentationFileName );
    }
}

unsigned short OptimalViewpointParameters::getSegmentationIterations() const
{
    return m_segmentationIterations;
}

void OptimalViewpointParameters::setSegmentationIterations( unsigned short segmentationIterations )
{
    if ( m_segmentationIterations != segmentationIterations )
    {
        m_segmentationIterations = segmentationIterations; emit changed( SegmentationIterations );
    }
}

unsigned char OptimalViewpointParameters::getSegmentationBlockLength() const
{
    return m_segmentationBlockLength;
}

void OptimalViewpointParameters::setSegmentationBlockLength( unsigned char segmentationBlockLenth )
{
    if ( m_segmentationBlockLength != segmentationBlockLenth )
    {
        m_segmentationBlockLength = segmentationBlockLenth; emit changed( SegmentationBlockLength );
    }
}

unsigned char OptimalViewpointParameters::getSegmentationNumberOfClusters() const
{
    return m_segmentationNumberOfClusters;
}

void OptimalViewpointParameters::setSegmentationNumberOfClusters( unsigned char segmentationNumberOfClusters )
{
    if ( m_segmentationNumberOfClusters != segmentationNumberOfClusters )
    {
        m_segmentationNumberOfClusters = segmentationNumberOfClusters; emit changed( SegmentationNumberOfClusters );
    }
}

double OptimalViewpointParameters::getSegmentationNoise() const
{
    return m_segmentationNoise;
}

void OptimalViewpointParameters::setSegmentationNoise( double segmentationNoise )
{
    if ( m_segmentationNoise != segmentationNoise )
    {
        m_segmentationNoise = segmentationNoise; emit changed( SegmentationNoise );
    }
}

double OptimalViewpointParameters::getSegmentationImageSampleDistance() const
{
    return m_segmentationImageSampleDistance;
}

void OptimalViewpointParameters::setSegmentationImageSampleDistance( double segmentationImageSampleDistance )
{
    if ( m_segmentationImageSampleDistance != segmentationImageSampleDistance )
    {
        m_segmentationImageSampleDistance = segmentationImageSampleDistance; emit changed( SegmentationImageSampleDistance );
    }
}

double OptimalViewpointParameters::getSegmentationSampleDistance() const
{
    return m_segmentationSampleDistance;
}

void OptimalViewpointParameters::setSegmentationSampleDistance( double segmentationSampleDistance )
{
    if ( m_segmentationSampleDistance != segmentationSampleDistance )
    {
        m_segmentationSampleDistance = segmentationSampleDistance; emit changed( SegmentationSampleDistance );
    }
}

bool OptimalViewpointParameters::getShade() const
{
    return m_shade;
}

void OptimalViewpointParameters::setShade( bool shade )
{
    if ( m_shade != shade )
    {
        m_shade = shade; emit changed( Shade );
    }
}

double OptimalViewpointParameters::getVisualizationImageSampleDistance() const
{
    return m_visualizationImageSampleDistance;
}

void OptimalViewpointParameters::setVisualizationImageSampleDistance( double visualizationImageSampleDistance )
{
    if ( m_visualizationImageSampleDistance != visualizationImageSampleDistance )
    {
        m_visualizationImageSampleDistance = visualizationImageSampleDistance; emit changed( VisualizationImageSampleDistance );
    }
}

double OptimalViewpointParameters::getVisualizationSampleDistance() const
{
    return m_visualizationSampleDistance;
}

void OptimalViewpointParameters::setVisualizationSampleDistance( double visualizationSampleDistance )
{
    if ( m_visualizationSampleDistance != visualizationSampleDistance )
    {
        m_visualizationSampleDistance = visualizationSampleDistance; emit changed( VisualizationSampleDistance );
    }
}

void OptimalViewpointParameters::setVolumeObject( Volume * volumeObject )
{
    if ( m_volumeObject != volumeObject )
    {
        m_volumeObject = volumeObject; emit changed( VolumeObject );
    }
}

void OptimalViewpointParameters::setNumberOfPlanes( unsigned char numberOfPlanes )
{
    m_numberOfPlanes = numberOfPlanes; emit changed( NumberOfPlanes );
}

void OptimalViewpointParameters::setTransferFunction( const OptimalViewpoint::TransferFunction & transferFunction )
{
    m_transferFunction = transferFunction; emit changed( TransferFunction );
}

Volume * OptimalViewpointParameters::getVolumeObject() const
{
    return m_volumeObject;
}

unsigned char OptimalViewpointParameters::getNumberOfPlanes() const
{
    return m_numberOfPlanes;
}

const OptimalViewpoint::TransferFunction & OptimalViewpointParameters::getTransferFunction() const
{
    return m_transferFunction;
}

unsigned char OptimalViewpointParameters::getVisualizationBlockLength() const
{
    return m_visualizationBlockLength;
}

void OptimalViewpointParameters::setVisualizationBlockLength( unsigned char visualizationBlockLength )
{
    if ( m_visualizationBlockLength != visualizationBlockLength )
    {
        m_visualizationBlockLength = visualizationBlockLength; emit changed( VisualizationBlockLength );
    }
}

void OptimalViewpointParameters::setAdjustedTransferFunction( const OptimalViewpoint::TransferFunction & adjustedTransferFunction )
{
    std::cout << "OVP::satf" << std::endl;
    emit signalAdjustedTransferFunction( adjustedTransferFunction );
}



// nous paràmetres



bool OptimalViewpointParameters::getComputeWithOpacity() const
{
    return m_computeWithOpacity;
}

void OptimalViewpointParameters::setComputeWithOpacity( bool computeWithOpacity )
{
    if ( m_computeWithOpacity != computeWithOpacity )
    {
        m_computeWithOpacity = computeWithOpacity; emit changed( ComputeWithOpacity );
    }
}



int OptimalViewpointParameters::getInterpolation() const
{
    return m_interpolation;
}

void OptimalViewpointParameters::setInterpolation( int interpolation )
{
    if ( m_interpolation != interpolation )
    {
        m_interpolation = interpolation; emit changed( Interpolation );
    }
}



bool OptimalViewpointParameters::getSpecular() const
{
    return m_specular;
}

void OptimalViewpointParameters::setSpecular( bool specular )
{
    if ( m_specular != specular )
    {
        m_specular = specular; emit changed( Specular );
    }
}



double OptimalViewpointParameters::getSpecularPower() const
{
    return m_specularPower;
}

void OptimalViewpointParameters::setSpecularPower( double specularPower )
{
    if ( m_specularPower != specularPower )
    {
        m_specularPower = specularPower; emit changed( SpecularPower );
    }
}



signed char OptimalViewpointParameters::getUpdatePlane() const
{
    return m_updatePlane;
}

void OptimalViewpointParameters::setUpdatePlane( signed char updatePlane )
{
    if ( m_updatePlane != updatePlane )
    {
        m_updatePlane = updatePlane; emit changed( UpdatePlane );
    }
}



bool OptimalViewpointParameters::getCompute() const
{
    return m_compute;
}

void OptimalViewpointParameters::setCompute( bool compute )
{
    if ( m_compute != compute )
    {
        m_compute = compute; emit changed( Compute );
    }
}



}; // end namespace udg
