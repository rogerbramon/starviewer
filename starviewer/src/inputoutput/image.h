/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGIMAGE_H
#define UDGIMAGE_H

#define HAVE_CONFIG_H 1

#include <string>
#include <dimse.h> // provide the structure DcmDataSet
#include <dcdeftag.h> //provide the information for the tags

namespace udg {

/** Classe image encarregada de guardar la informació d'una imatge
@author marc
*/
class Image{
public:

    /// Constructor de la classe
    Image();

    ///Constructor a partir d'un dataset provinent d'un objecte DICOM, no emplene el camps ImageFileName, ImagePath i ImageSize
    Image(DcmDataset *imageDataset);

    /// destructor de la classe
    ~Image();

    /** Inserta el UID de la Imatge
     * @param  UID de la imatge
     */
    void setSOPInstanceUID (std::string );

    /** Inserta el UID de la sèrie al qual pertany la imatge
     * @param  UID de la sèrie a la que pertany la imatge
     */
    void setSeriesUID (std::string );

    /** Inserta el path de la imatge a l'ordinador local
     * @param path de la imatge
     */
    void setImagePath (std::string );

    /** Inserta el UID de la l'estudi al qual pertany la imatge
     * @param UID  UID de la l'estudi a la que pertany la imatge
     */
    void setStudyUID (std::string );

    /** Inserta el nom de la imatge
     * @param name [in] nom de la imatge
     */
    void setImageName (std::string );

    /** Inserta el número d'imatge
     * @param Número que ocupa la imatge dins la sèrie
     */
    void setImageNumber (int);

    /** Inserta el número de bytes que ocupa la imatge
     * @param  bytes de la imatge
     */
    void setImageSize (int);

    /** estableix el PACS al que pertany l'estudi
     * @param AETitle AETitle al que pertany la imatge
     */
    void setPacsAETitle( std::string AETitle );

    /** Retorna el UID de la Imatge
     * @return  UID de la imatge
     */
    std::string getSOPInstanceUID();

    /** Retorna el UID de la sèrie al qual pertany la imatge
     * @return UID de la sèrie a la que pertany la imatge
     */
    std::string getSeriesUID();

    /** Retorna el path de la imatge a l'ordinador local
     * @return path de la imatge
     */
    std::string getImagePath ();

    /** Retorna el UID de l'estudi al qual pertany la imatge
     * @return UID de la l'estudi a la que pertany la imatge
     */
    std::string getStudyUID();

    /** Retorna el nom de la imatge
     * @return el nom de la imatge
     */
    std::string getImageName();

    /** retorna el número d'imatge
     * @return Retorna el número que ocupa la imatge dins la sèrie
     */
    int getImageNumber();

    /** retorna el número de bytes que ocupa la imatge
     * @return retorna el número de bytes de la imatge
     */
    int getImageSize();

    /** retorna el AETitle de la imatge
     * @return AETitle de la imatge
     */
    std::string getPacsAETitle();

private:
    //variables que guarden la informació de la imatge
    std::string m_SoPUID;
    std::string m_seriesUID;
    std::string m_imagePath;
    std::string m_studyUID;
    std::string m_imageName;
    std::string m_pacsAETitle;
    int m_imageNumber;
    int m_imageSize;

};

};

#endif
