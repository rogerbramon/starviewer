/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gràfics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "dicomimagereaderdcmtk.h"

#include "logging.h"
#include "image.h"
#include <QApplication> // pel "processEvents()"
#include <cmath> // pel floor

#include <dcmtk/dcmimgle/dcmimage.h>

namespace udg {

DICOMImageReaderDCMTK::DICOMImageReaderDCMTK(QObject *parent)
 : DICOMImageReader(parent)
{
}

DICOMImageReaderDCMTK::~DICOMImageReaderDCMTK()
{
}

bool DICOMImageReaderDCMTK::load()
{
    bool ok = readyToLoad();

    if( ok )
    {
        // nombre de bytes que copiem->debug
        unsigned long bytes = 0;
        // buffer on colocarem la llesca que hem llegit
        unsigned char *dicomBuffer = NULL;
        // imatges totals i comptador per calcular el progrés
        int total = m_inputImageList.size();
        int slice = 0;
        emit started();
        // Per cada imatge
        foreach( Image *image, m_inputImageList )
        {
            DicomImage *dicomImage = new DicomImage( qPrintable( image->getPath() ) );
            if( dicomImage != NULL )
            {
                if( dicomImage->getStatus() == EIS_Normal )
                {
                    dicomImage->setMinMaxWindow();
                    dicomBuffer = (unsigned char *)dicomImage->getOutputData();
                    bytes = dicomImage->getOutputDataSize();
                    // bytes que ens dóna
                    DEBUG_LOG( QString("Bytes que ens dóna DicomImage: %1, bytes que ens donen per paràmetre: %2").arg( bytes ).arg( m_sliceByteIncrement ) );
                }
                else
                    DEBUG_LOG( QString( "Error en carregar la DicomImage. Error: %1 ").arg( DicomImage::getString( dicomImage->getStatus() ) ) );

                // copiem les dades del buffer d'imatge cap a vtk
                memcpy( m_imageBuffer, dicomBuffer, m_sliceByteIncrement );
                m_imageBuffer += m_sliceByteIncrement;
                slice++;
                qApp->processEvents();
                emit progress( (int)floor( (double)slice/(double)total * 100 ) );
            }
        }
        emit finished();
    }

    return ok;
}

}