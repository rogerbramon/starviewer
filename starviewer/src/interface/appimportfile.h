/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGAPPIMPORTFILE_H
#define UDGAPPIMPORTFILE_H

#include <QObject>
#include <QString>

#include "identifier.h"

namespace udg {

// Fordward Declarations
class VolumeRepository;
class Input;
class Output;

/**
Mini-aplicaci� encarregada de permetre carregar un model des del sistema de fitxers al repositori de volums

@author Grup de Gr�fics de Girona  ( GGG )
*/
class AppImportFile : public QObject{
Q_OBJECT
public:
    
    AppImportFile(QObject *parent = 0, const char *name = 0);
    ~AppImportFile();
    
    /// Obre el di�leg per poder obrir arxius
    void open();

    /// Obre el di�leg per poder obrir arxius d'un directori DICOM
    void openDirectory();
    
    /// Retorna l'id de l'�ltim volum que ha depositat al repositori
    Identifier getVolumeIdentifier(){ return m_volumeID; }

signals:
    /// Aquesta senyal s'emetr� quan s'hagi afegit un volum al repositori
    void newVolume( Identifier );
    
public slots:
    /// M�tode espec�fic per quan es tanca l'aplicaci�, allibera recursos i "deixa les coses al seu lloc"
    void finish();

private:
    /// Directori de treball per fitxers ordinaris
    QString m_workingDirectory;

    /// Directori de treball per directoris dicom
    QString m_workingDicomDirectory;

    /// Filtres de fitxer
    QString m_openFileFilters;

    /// llegeix escriu configuracions
    void readSettings();
    void writeSettings();

    /// fa la feina 'bruta' d'obrir l'arxiu
    bool loadFile( QString fileName );

    /// fa la feina 'bruta' d'obrir un directori d'arxius dicom
    bool loadDirectory( QString directoryName );
    
    // :::::::::::::::::::::::::::::::::::::::::
    // Recursos
    // :::::::::::::::::::::::::::::::::::::::::

    /// L'id del volum amb el que estem treballant
    Identifier m_volumeID;    
    /// El repository de volums
    udg::VolumeRepository* m_volumeRepository;
    /// acc�s a l'entrada de dades -> aix� hauria de formar part d'una mini-app, per tant �s temporal
    Input *m_inputReader;
};

};  //  end  namespace udg

#endif
