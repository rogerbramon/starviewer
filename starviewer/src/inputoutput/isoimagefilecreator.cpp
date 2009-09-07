/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *

 ***************************************************************************/

#include "isoimagefilecreator.h"

#include <QApplication>
#include <QProgressDialog>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "logging.h"

namespace udg {

IsoImageFileCreator::IsoImageFileCreator()
{
}

IsoImageFileCreator::IsoImageFileCreator(const QString &inputPath, const QString &outputIsoImageFilePath)
{
    m_inputPath = inputPath;
    m_outputIsoImageFilePath = outputIsoImageFilePath;
}

IsoImageFileCreator::~IsoImageFileCreator()
{
}

QString IsoImageFileCreator::getIsoImageLabel() const
{
    return m_isoImageLabel;
}

void IsoImageFileCreator::setIsoImageLabel( const QString &isoImageLabel )
{
    m_isoImageLabel = isoImageLabel;
}

QString IsoImageFileCreator::getInputPath() const
{
    return m_inputPath;
}

void IsoImageFileCreator::setInputPath( const QString &inputPath )
{
    m_inputPath = inputPath;
}

QString IsoImageFileCreator::getOutputIsoImageFilePath() const
{
    return m_outputIsoImageFilePath;
}

void IsoImageFileCreator::setOutputIsoImageFilePath( const QString &outputIsoImageFilePath )
{
    m_outputIsoImageFilePath = outputIsoImageFilePath;
}

IsoImageFileCreator::ImageFileCreationError IsoImageFileCreator::getLastError() const
{
    return m_lastError;
}

QString IsoImageFileCreator::getLastErrorDescription() const
{
    return m_lastErrorDescription;
}

void IsoImageFileCreator::startCreateIsoImageFile()
{
    // Es comprova que el directori o fitxer a partir del qual es vol generar el fitxer d'imatge ISO existeix
    if( !QFile::exists(m_inputPath) )
    {
        m_lastErrorDescription = QObject::tr("The input path that we want to turn into an ISO image doesn't exist.");
        m_lastError = InputPathNotFound;
        emit finishedCreateIsoImageFile(false);
    }
    else
    {
        QFileInfo outputIsoImageFilePathInfo(m_outputIsoImageFilePath);
        QFileInfo outputIsoImageDirPathInfo(outputIsoImageFilePathInfo.dir(),"");

        // Es comprova que el directori on es vol guardar el fitxer de imatge ISO existeixi
        if ( !outputIsoImageFilePathInfo.dir().exists() )
        {
            m_lastErrorDescription = QObject::tr("The directory where you want to save ISO image file doesn't exist.");
            m_lastError = OutputPathNotFound;
            emit finishedCreateIsoImageFile(false);
        }
        else
        {
            // Es comprova que es disposi de permisos d'escriptura en el directori on es vol guardar el fitxer d'imatge ISO
            // Es a dir, es comproven els permisos del directori.
            if ( !outputIsoImageDirPathInfo.isWritable() )
            {
                m_lastErrorDescription = QObject::tr("You don't have permissions to write in the output iso image directory path.");
                m_lastError = OutputDirPathAccessDenied;
                emit finishedCreateIsoImageFile(false);
            }
            else
            {
                // Es comprova que es diposi de permisos de escritura en el path on s'ha de crear el fitxer d'imatge ISO
                // Es a dir, es comproven els permisos del fitxer
                if ( QFile::exists(outputIsoImageFilePathInfo.absoluteFilePath()) && !outputIsoImageFilePathInfo.isWritable() )
                {
                    m_lastErrorDescription = QObject::tr("You don't have permissions to write in the output iso image file path.");
                    m_lastError = OutputFilePathAccessDenied;
                    emit finishedCreateIsoImageFile(false);
                }
                else
                {
                    QStringList processParameters;

                    // Afegim als par�metres l'etiqueta de la imatge ISO en cas de que tingui valor v�lid
                    if (!m_isoImageLabel.isEmpty())
                    {
                        processParameters << "-V"; 
                        processParameters << m_isoImageLabel;
                    }
                    processParameters << "-o"; 
                    processParameters << m_outputIsoImageFilePath; // Nom i directori on guardarem la imatge
                    processParameters << m_inputPath; // Path a convertir en iso

                    QString mkisofsFilePath = QCoreApplication::applicationDirPath() + "/mkisofs";

                    // Es comprova que existeixi el mkisofs al path on hauria d'estar per windows, mac i linux
                    // Windows
                    if ( !QFile::exists(mkisofsFilePath + ".exe") )
                    {
                        // Mac
                        if ( !QFile::exists(mkisofsFilePath) )
                        {
                            // Linux
                            mkisofsFilePath = "/usr/bin/mkisofs";
                        
                            // Si no existeix per cap sistema s'ha produit un error degut a que no s'ha pogut localitzar el mkisofs
                            if ( !QFile::exists(mkisofsFilePath) )
                            {
                                m_lastErrorDescription = QObject::tr("The mkisofs program cannot be found on the system.");
                                m_lastError = InternalError;
                            
                                emit finishedCreateIsoImageFile(false);
                            }
                        }
                    }
                    else
                    {
                        // Es crea el process i es connecta el seu signal finished amb l'slot finishCreationProcess
                        m_process = new QProcess();
                        connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishCreationProcess(int))); 
                        m_process->start(mkisofsFilePath, processParameters);
                    }
                }
            }
        }
    }
}

void IsoImageFileCreator::finishCreationProcess(int exitCode)
{
    // Despr�s de que s'emeti el signal finished del QProcess, s'executa aquest Slot on es comprova si el proc�s a finialitzat o no 
    // correctament i s'emet el signal finishedCreateIsoImageFile(bool)
    if( exitCode != 0 )
    {
        m_lastErrorDescription = QObject::tr("An error occurred with the iso image file create process.");
        m_lastError = InternalError;

        ERROR_LOG( "Error al crear ISO; Exit code qprocess: " + exitCode );
        emit finishedCreateIsoImageFile(false);
    }
    else
    {
        emit finishedCreateIsoImageFile(true);
    }
}

}
