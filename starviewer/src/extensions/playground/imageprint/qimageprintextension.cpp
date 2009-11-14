#include <iostream>
#include <QWidget>

#include "qimageprintextension.h"
#include "imageprintfactory.h"
#include "dicomprintfactory.h"
#include "qprintjobcreatorwidget.h"
#include "qdicomjobcreatorwidget.h"
#include "qprinterconfigurationwidget.h"
#include "qprintingconfigurationwidget.h"
#include "dicomprintermanager.h"
#include "dicomprinter.h"
#include "dicomprintpage.h"
#include "dicomprintjob.h"
#include "image.h"
#include "dicomprint.h"
#include "volume.h"
#include "q2dviewer.h"

//TODO: Ouch! SuperGuarrada (tm). Per poder fer sortir el menú i tenir accés al Patient principal. S'ha d'arreglar en quan es tregui les dependències de interface, pacs, etc.etc.!!
#include "../interface/qapplicationmainwindow.h"

namespace udg {

QImagePrintExtension::QImagePrintExtension( QWidget *parent )
{	
    setupUi( this );

	// Només per provar.
	m_factory=new DicomPrintFactory();
	//m_tbToolBox->addItem(m_factory->getPrintingConfigurationWidget(),"PrintingConfigurationWidget");
	//m_tbToolBox->addItem(m_factory->getPrintJobCreatorWidget(),"JobCreatorWidget");
    fillSelectedDicomPrinterComboBox();

    m_printerConfigurationWidgetProof = m_factory->getPrinterConfigurationWidget();

    createConnections();
    configureInputValidator();
}

QImagePrintExtension::~QImagePrintExtension()
{
}

void QImagePrintExtension::createConnections()
{ 
    connect(m_configurationPrinterToolButton, SIGNAL(clicked()), SLOT(configurationPrinter()));
    connect(m_selectedPrinterComboBox, SIGNAL(currentIndexChanged(int)), SLOT(selectedDicomPrinterChanged(int)));
    connect(m_printerConfigurationWidgetProof, SIGNAL(printerSettingsChanged()), SLOT(fillSelectedDicomPrinterComboBox()));
    connect(m_selectionImageRadioButton, SIGNAL(clicked()), SLOT(imageSelectionModeChanged()));
    connect(m_currentImageRadioButton, SIGNAL(clicked()), SLOT(imageSelectionModeChanged()));
    //Sliders quan canvia de valor
    connect(m_intervalImagesSlider, SIGNAL(valueChanged(int)), SLOT(m_intervalImagesSliderValueChanged(int)));
    connect(m_fromImageSlider, SIGNAL(valueChanged(int)), SLOT(m_fromImageSliderValueChanged(int)));
    connect(m_toImageSlider, SIGNAL(valueChanged(int)), SLOT(m_toImageSliderValueChanged(int)));
    //LineEdit canvien de valor
    connect(m_intervalImagesLineEdit, SIGNAL(textEdited (const QString &)), SLOT(m_intervalImagesLineEditTextEdited(const QString &)));
    connect(m_fromImageLineEdit, SIGNAL(textEdited (const QString &)), SLOT(m_fromImageLineEditTextEdited(const QString &)));
    connect(m_toImageLineEdit, SIGNAL(textEdited (const QString &)), SLOT(m_toImageLineEditTextEdited(const QString &)));

    connect(m_qdicomPrinterBasicSettingsWidget, SIGNAL(basicDicomPrinterSettingChanged()), SLOT(updateNumberOfDicomPrintPagesToPrint()));
    connect(m_printButton, SIGNAL(clicked()), SLOT(print()));

    //connect( m_2DView, SIGNAL( eventReceived( unsigned long ) ), SLOT( strokeEventHandler(unsigned long) ) );
    connect( m_sliceViewSlider, SIGNAL( valueChanged(int) ) , m_2DView , SLOT( setSlice(int) ) );

}

void QImagePrintExtension::configureInputValidator()
{
    m_intervalImagesLineEdit->setValidator(new QIntValidator(0, 99999, m_intervalImagesLineEdit));
    m_fromImageLineEdit->setValidator(new QIntValidator(0, 99999, m_fromImageLineEdit));
    m_toImageLineEdit->setValidator(new QIntValidator(0, 99999, m_toImageLineEdit));
}

void QImagePrintExtension::setInput(Volume *input)
{
    m_2DView->setInput(input);

    m_sliceViewSlider->setMinimum(0);
    m_sliceViewSlider->setMaximum(input->getImages().count());

    updateSelectionImagesValue();
}

void QImagePrintExtension::configurationPrinter()
{
    
    m_printerConfigurationWidgetProof->show();
}

void QImagePrintExtension::fillSelectedDicomPrinterComboBox()
{
    DicomPrinterManager dicomPrinterManager;

    m_selectedPrinterComboBox->clear();

    foreach(DicomPrinter dicomPrinter, dicomPrinterManager.getDicomPrinterList())
    {
        QString comboBoxText = dicomPrinter.getAETitle();

        if (dicomPrinter.getDescription().length() > 0)
        {
            comboBoxText += " - " + dicomPrinter.getDescription();
        }
       
         m_selectedPrinterComboBox->addItem(comboBoxText, dicomPrinter.getID());

        if (dicomPrinter.getIsDefault())
        {
            //Si és impressora per defecte la deixem com a impressora seleccionada
            m_selectedPrinterComboBox->setCurrentIndex(m_selectedPrinterComboBox->findData(dicomPrinter.getID()));
            selectedDicomPrinterChanged(m_selectedPrinterComboBox->findData(dicomPrinter.getID()));
        }
    }
}

void QImagePrintExtension::print()
{
    DicomPrint dicomPrint;

    dicomPrint.print(getSelectedDicomPrinter(), getDicomPrintJobToPrint());
}

DicomPrintJob QImagePrintExtension::getDicomPrintJobToPrint()
{
    DicomPrintJob dicomPrintJob;
    DicomPrinter selectedDicomPrinter = getSelectedDicomPrinter();

    dicomPrintJob.setDicomPrintPages(getDicomPrintPageListToPrint());
    dicomPrintJob.setFilmDestination(selectedDicomPrinter.getDefaultFilmDestination());
    dicomPrintJob.setLabel("");
    dicomPrintJob.setMediumType(selectedDicomPrinter.getDefaultMediumType());
    dicomPrintJob.setNumberOfCopies(m_numberOfCopiesSpinBox->value());
    dicomPrintJob.setPrintPriority(selectedDicomPrinter.getDefaultPrintPriority());

    return dicomPrintJob;
}

QList<DicomPrintPage> QImagePrintExtension::getDicomPrintPageListToPrint()
{
    QList<Image*> selectedImagesToPrint = getSelectedImagesToPrint();
    QList<DicomPrintPage> dicomPrintPageList;
    DicomPrinter dicomPrinter = getSelectedDicomPrinter();
    int numberOfImagesPerPage = dicomPrinter.getDefaultFilmLayoutColumns() * dicomPrinter.getDefaultFilmLayoutRows();
    int numberOfPage = 1;

    while (!selectedImagesToPrint.isEmpty())
    {
        int indexOfImagePerPage = 0;
        QList<Image*> imagesPageList;
        DicomPrintPage dicomPrintPage = fillDicomPrintPagePrintSettings(dicomPrinter);
        
        dicomPrintPage.setPageNumber(numberOfPage);

        //Emplenen una dicomPrintPage amb les imatges en funció del número d'imatges que hi caben
        while (indexOfImagePerPage < numberOfImagesPerPage && !selectedImagesToPrint.isEmpty())
        {
            imagesPageList.append(selectedImagesToPrint.takeFirst());
            indexOfImagePerPage++;
        }

        dicomPrintPage.setImagesToPrint(imagesPageList);
        dicomPrintPageList.append(dicomPrintPage);
        numberOfPage++;
    }

    return dicomPrintPageList;
}

QList<Image*> QImagePrintExtension::getSelectedImagesToPrint()
{
    QList<Image*> imagesToPrint, imagesVolum = m_2DView->getInput()->getImages();
    int indexOfImage = m_fromImageSlider->value() -1;

    while (indexOfImage < m_toImageSlider->value())
    {
        imagesToPrint.append(imagesVolum.at(indexOfImage));
        indexOfImage += m_intervalImagesSlider->value();
    }

    return imagesToPrint;
}

int QImagePrintExtension::getNumberOfPagesToPrint()
{
    int numberOfDicomPrintPagesToPrint = 0, numberOfImagesPerPage, numberOfImagesToPrint;
    DicomPrinter selectedDicomPrinter = getSelectedDicomPrinter();

    numberOfImagesToPrint = (m_toImageSlider->value() - m_fromImageSlider->value() +1) / m_intervalImagesSlider->value();
    numberOfImagesPerPage = selectedDicomPrinter.getDefaultFilmLayoutRows() * selectedDicomPrinter.getDefaultFilmLayoutColumns(); 

    if (numberOfImagesToPrint > 0 && numberOfImagesPerPage > 0)
    {
        numberOfDicomPrintPagesToPrint = numberOfImagesToPrint / numberOfImagesPerPage ;
        //Si tenim residu hem d'incrementar en una el número de pàgines
        numberOfDicomPrintPagesToPrint += numberOfImagesToPrint % numberOfImagesPerPage > 0 ? 1 : 0;
    }

    return numberOfDicomPrintPagesToPrint;
}

DicomPrintPage QImagePrintExtension::fillDicomPrintPagePrintSettings(DicomPrinter dicomPrinter)
{
    DicomPrintPage dicomPrintPage;

    dicomPrintPage.setBorderDensity(dicomPrinter.getDefaultBorderDensity());
    dicomPrintPage.setEmptyImageDensity(dicomPrinter.getDefaultEmptyImageDensity());
    dicomPrintPage.setFilmLayout(dicomPrinter.getDefaultFilmLayout());
    dicomPrintPage.setFilmOrientation(dicomPrinter.getDefaultFilmOrientation());
    dicomPrintPage.setFilmSize(dicomPrinter.getDefaultFilmSize());
    dicomPrintPage.setMagnificationType(dicomPrinter.getDefaultMagnificationType());
    dicomPrintPage.setMaxDensity(dicomPrinter.getDefaultMaxDensity());
    dicomPrintPage.setMinDensity(dicomPrinter.getDefaultMinDensity());
    dicomPrintPage.setPolarity(dicomPrinter.getDefaultPolarity());
    dicomPrintPage.setSmoothingType(dicomPrinter.getDefaultSmoothingType());
    dicomPrintPage.setTrim(dicomPrinter.getDefaultTrim());

    return dicomPrintPage;
}

void QImagePrintExtension::selectedDicomPrinterChanged(int indexOfSelectedDicomPrinter)
{
    m_qdicomPrinterBasicSettingsWidget->clear();

    if (indexOfSelectedDicomPrinter >= 0)
    {
        DicomPrinter selectedDicomPrinter = DicomPrinterManager().getPrinterByID(m_selectedPrinterComboBox->itemData(indexOfSelectedDicomPrinter).toInt());

        m_hostNameLabel->setText(selectedDicomPrinter.getHostname());
        m_portLabel->setText(QString().setNum(selectedDicomPrinter.getPort()));

        m_qdicomPrinterBasicSettingsWidget->setDicomPrinterBasicSettings(selectedDicomPrinter);
        m_qdicomPrinterBasicSettingsWidget->setEnabled(true);
    }
    else
    {
        m_qdicomPrinterBasicSettingsWidget->setEnabled(false);
    }
}

void QImagePrintExtension::imageSelectionModeChanged()
{
    m_selectionImagesFrame->setEnabled(m_selectionImageRadioButton->isChecked());
}
                           
void QImagePrintExtension::m_intervalImagesSliderValueChanged(int value)
{
    m_intervalImagesLineEdit->setText(QString().setNum(value));
    updateNumberOfDicomPrintPagesToPrint();
}

void QImagePrintExtension::m_fromImageSliderValueChanged(int value)
{
    m_fromImageLineEdit->setText(QString().setNum(value));
    updateNumberOfDicomPrintPagesToPrint();
}

void QImagePrintExtension::m_toImageSliderValueChanged(int value)
{
    m_toImageLineEdit->setText(QString().setNum(value));
    updateNumberOfDicomPrintPagesToPrint();
}

void QImagePrintExtension::m_intervalImagesLineEditTextEdited(const QString &text)
{
    int value = text.toInt();

    if (value > m_intervalImagesSlider->maximum())
    {
        m_intervalImagesSlider->setValue(m_intervalImagesSlider->maximum());
        m_intervalImagesLineEdit->setText(QString().setNum(m_intervalImagesSlider->maximum()));
    }
    else
    {
        m_intervalImagesSlider->setValue(value);
    }

    updateNumberOfDicomPrintPagesToPrint();
}

void QImagePrintExtension::m_fromImageLineEditTextEdited(const QString &text)
{
    int value = text.toInt();

    if (value > m_fromImageSlider->maximum())
    {
        m_fromImageSlider->setValue(m_fromImageSlider->maximum());
        m_fromImageLineEdit->setText(QString().setNum(m_fromImageSlider->maximum()));
    }
    else
    {
        m_fromImageSlider->setValue(value);
    }

    updateNumberOfDicomPrintPagesToPrint();
}

void QImagePrintExtension::m_toImageLineEditTextEdited(const QString &text)
{
    int value = text.toInt();

    if (value > m_toImageSlider->maximum())
    {
        m_toImageSlider->setValue(m_toImageSlider->maximum());
        m_toImageLineEdit->setText(QString().setNum(m_toImageSlider->maximum()));
    }
    else
    {
        m_toImageSlider->setValue(value);
    }

    updateNumberOfDicomPrintPagesToPrint();
}

void QImagePrintExtension::updateNumberOfDicomPrintPagesToPrint()
{
    int numberOfDicomPrintPagesToPrint = getNumberOfPagesToPrint();

    if (numberOfDicomPrintPagesToPrint > 0)
    {
        m_pagesToPrintLabel->setText(QString().setNum(numberOfDicomPrintPagesToPrint));
    }
    else
    {
        m_pagesToPrintLabel->setText("N/A");
    }
}

void QImagePrintExtension::updateSelectionImagesValue()
{
    int tickInterval;
    int numberOfImagesOfVolume = m_2DView->getInput()->getImages().count();

    m_fromImageSlider->setMaximum(numberOfImagesOfVolume);
    
    m_toImageSlider->setMaximum(numberOfImagesOfVolume);
    m_toImageSlider->setValue(numberOfImagesOfVolume);
    m_toImageLineEdit->setText(QString().setNum(numberOfImagesOfVolume));

    if (numberOfImagesOfVolume < 10)
    {
        tickInterval = 1;
    }
    else if (numberOfImagesOfVolume < 50)
    {
        tickInterval = 2;
    }
    else if (numberOfImagesOfVolume < 100)
    {
        tickInterval = 5;
    }
    else if (numberOfImagesOfVolume < 400)
    {
        tickInterval = 10;
    }
    else tickInterval = 20;

    m_toImageSlider->setTickInterval(tickInterval);
    m_fromImageSlider->setTickInterval(tickInterval);

    
}

DicomPrinter QImagePrintExtension::getSelectedDicomPrinter()
{
    DicomPrinter dicomPrinter;
    int indexOfSelectedDicomPrinter = m_selectedPrinterComboBox->currentIndex();

    dicomPrinter = DicomPrinterManager().getPrinterByID(m_selectedPrinterComboBox->itemData(indexOfSelectedDicomPrinter).toInt());

    //sobreescrivim els settings que formen part configuració bàsica de la impressora amb els de control QDicomPrinterBasicSettings
    m_qdicomPrinterBasicSettingsWidget->getDicomPrinterBasicSettings(dicomPrinter);

    return dicomPrinter;
}

}                                      