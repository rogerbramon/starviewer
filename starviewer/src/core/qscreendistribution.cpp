/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "qscreendistribution.h"

#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QSize>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidgetAction>
#include <QLinearGradient>
#include <QRadialGradient>

namespace udg {

const int QScreenDistribution::MaximumScreenNumberPixelSize = 50;

QScreenDistribution::QScreenDistribution(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    m_mouseInScreen = -1;
    m_marging = 50;
    this->setMinimumSize(QSize(300,200));
}

QScreenDistribution::~QScreenDistribution()
{    
}

QSize QScreenDistribution::sizeHint() const
{
    return QSize(400, 200);
}

void QScreenDistribution::mouseMoveEvent(QMouseEvent *event)
{
    bool outside = true;
    int i = 0;
    // Per cada icona de finestra, comprobar si el ratolí hi és dins
    while (outside && i < m_screens.size())
    {
        if (m_screens[i].contains(event->pos()))
        {
            outside = false;
            setCursor(Qt::PointingHandCursor);
            if (m_mouseInScreen != i)
            {
                m_mouseInScreen = i;
                repaint();
            }
        }
        i++;
    }
    // Si no se n'hi ha trobat cap, llavors està a fora
    if (outside)
    {
        setCursor(Qt::ArrowCursor);
        if (m_mouseInScreen != -1)
        {
            m_mouseInScreen = -1;
            repaint();
        }
    }
}

void QScreenDistribution::mousePressEvent(QMouseEvent *event)
{
    bool outside = true;
    int i = 0;
    // Per cada icona de finestra, comprobar si s'ha fet un clic dins
    while (outside && i < m_screens.size())
    {
        if (m_screens[i].contains(event->pos()))
        {
            outside = false;
            emit screenClicked(i);
            event->ignore();
        }
        i++;
    }
}

void QScreenDistribution::paintEvent(QPaintEvent *event)
{
    m_screens.clear();

    // Calcul de les posicions on s'haurà de pintar les diferents icones de finestres.
    computeSizesAndPositions();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(Qt::white, 2, Qt::SolidLine);
    painter.setPen(pen);

    QFont font;
    font.setPixelSize(m_screenNumberPixelSize);
    painter.setFont(font);

    for (int i = 0; i < m_screens.count(); i++)
    {
        if (m_mouseInScreen == i)
        {
            QLinearGradient linearGradient(0.1, 0.1, 1.2, 1.2);
            linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            linearGradient.setColorAt(0, QColor("#777777"));
            linearGradient.setColorAt(1, QColor("#111111"));
            painter.setBrush(QBrush(linearGradient));
            painter.drawRoundedRect(m_screens.at(i), 2, 2);

            QRadialGradient radialGradient(0.9, 2.1, 2.5);
            radialGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            radialGradient.setColorAt(0, QColor(255, 255, 255, 0));
            radialGradient.setColorAt(0.74, QColor(255, 255, 255, 0));
            radialGradient.setColorAt(0.75, QColor(255, 255, 255, 40));
            radialGradient.setColorAt(1, QColor(255, 255, 255, 0));
            painter.setBrush(QBrush(radialGradient));
            painter.drawRoundedRect(m_screens.at(i), 2, 2);
        }
        else
        {
            painter.setPen(Qt::NoPen);

            QLinearGradient linearGradient(0.1, 0.1, 1.2, 1.2);
            linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            linearGradient.setColorAt(0, QColor("#666666"));
            linearGradient.setColorAt(1, QColor("#000000"));
            painter.setBrush(QBrush(linearGradient));
            painter.drawRoundedRect(m_screens.at(i), 2, 2);

            QRadialGradient radialGradient(0.9, 2.1, 2.5);
            radialGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            radialGradient.setColorAt(0, QColor(255, 255, 255, 0));
            radialGradient.setColorAt(0.74, QColor(255, 255, 255, 0));
            radialGradient.setColorAt(0.75, QColor(255, 255, 255, 40));
            radialGradient.setColorAt(1, QColor(255, 255, 255, 0));
            painter.setBrush(QBrush(radialGradient));
            painter.drawRoundedRect(m_screens.at(i), 2, 2);

            painter.setPen(pen);
        }
        
        //Pintar el numero
        painter.drawText(m_screens.at(i), Qt::AlignCenter, QString::number(i + 1));
    }
    event->accept();
}

void QScreenDistribution::computeSizesAndPositions()
{
    QDesktopWidget *desktop = QApplication::desktop();   

    int MinimumX = 0;
    int MinimumY = 0;
    int MaximumX = 0;
    int MaximumY = 0;

    // Per cada pantalla
    for (int i = 0; i < desktop->screenCount(); i++)
    {
        // Agafem les seves coordenades
        QPoint topLeft = desktop->screenGeometry(i).topLeft();
        QPoint bottomRight = desktop->screenGeometry(i).bottomRight();

        m_screens.append(QRect(topLeft, bottomRight));

        // I calculem el tamany màxim que ocupa tot el conjunt de pantalles
        if (topLeft.x() < MinimumX)
        {
            MinimumX = topLeft.x();
        }
        if (bottomRight.x() > MaximumX)
        {
            MaximumX = bottomRight.x();
        }
        if (topLeft.y() < MinimumY)
        {
            MinimumY = topLeft.y();
        }
        if (bottomRight.y() > MaximumY)
        {
            MaximumY = bottomRight.y();
        }
    }

    int totalWidth = MaximumX - MinimumX;
    int totalHeight = MaximumY - MinimumY;    

    // Calculem la proporció en que s'ha de dividir per que no es deformi el dibuix
    float widthDivisor = (float)totalWidth / ((float)width() - m_marging * 2);
    float heightDivisor = (float)totalHeight / ((float)height() - m_marging * 2);

    float divisor = (widthDivisor < heightDivisor) ? heightDivisor : widthDivisor;
    int offsetX = 0;
    int offsetY = 0;

    if (widthDivisor > heightDivisor)
    {
        float centering = (height() - m_marging * 2 - (totalHeight / divisor)) / 2;
        offsetX = -1 * MinimumX / divisor + m_marging;
        offsetY = -1 * MinimumY / divisor + m_marging + centering;
    }
    else
    {
        float centering = (width() - m_marging * 2 - (totalWidth / divisor)) / 2;
        offsetX = -1 * MinimumX / divisor + m_marging + centering;
        offsetY = -1 * MinimumY / divisor + m_marging;
    }

    // Adaptem les posicións a les posicions de dibuix escalades i centrades
    int minimumScreenHeight = 0;
    for (int i = 0; i < m_screens.count(); i++)
    {
        // Requadre
        QRect screen;
        screen.setLeft(m_screens.at(i).left() / divisor + offsetX + 3);
        screen.setTop(m_screens.at(i).top() / divisor + offsetY + 3);
        screen.setRight(m_screens.at(i).right() / divisor + offsetX - 3);
        screen.setBottom(m_screens.at(i).bottom() / divisor + offsetY - 3);
        m_screens.replace(i, screen);

        if (i == 0 || screen.height() < minimumScreenHeight)
        {
           minimumScreenHeight = screen.height();
        }
    }

    m_screenNumberPixelSize = (minimumScreenHeight - 15 < MaximumScreenNumberPixelSize)? minimumScreenHeight - 15: MaximumScreenNumberPixelSize;
}

} // end namespace udg