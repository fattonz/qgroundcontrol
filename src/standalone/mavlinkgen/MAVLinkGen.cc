/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009, 2010 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Implementation of class MAVLinkGen
 *
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */


#include <QFile>
#include <QFlags>
#include <QThread>
#include <QSplashScreen>
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyleFactory>
#include <QAction>
#include <QSettings>
#include <QFontDatabase>
#include <QMainWindow>

#include "MAVLinkGen.h"
#include "XMLCommProtocolWidget.h"


/**
 * @brief Constructor for the main application.
 *
 * This constructor initializes and starts the whole application. It takes standard
 * command-line parameters
 *
 * @param argc The number of command-line parameters
 * @param argv The string array of parameters
 **/

MAVLinkGen::MAVLinkGen(int &argc, char* argv[]) : QApplication(argc, argv)
{
    this->setApplicationName("MAVLink Generator");
    this->setApplicationVersion("v. 1.0.0 (Beta)");
    this->setOrganizationName(QLatin1String("QGroundControl"));
    this->setOrganizationDomain("http://qgroundcontrol.org");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    // Exit main application when last window is closed
    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));

    // Set application font
    QFontDatabase fontDatabase = QFontDatabase();
    const QString fontFileName = ":/general/vera.ttf"; ///< Font file is part of the QRC file and compiled into the app
    const QString fontFamilyName = "Bitstream Vera Sans";
    if(!QFile::exists(fontFileName)) printf("ERROR! font file: %s DOES NOT EXIST!\n", fontFileName.toStdString().c_str());
    fontDatabase.addApplicationFont(fontFileName);
    setFont(fontDatabase.font(fontFamilyName, "Roman", 12));

    // Create main window
    QMainWindow* window = new QMainWindow();
    window->setCentralWidget(new XMLCommProtocolWidget(window));
    window->setWindowTitle(applicationName() + " " + applicationVersion());
//    window->setBaseSize(qMin(1024, static_cast<int>(QApplication::desktop()->width()*0.8f)), qMin(900, static_cast<int>(QApplication::desktop()->height()*0.8f)));
    window->show();
}

/**
 * @brief Destructor for the groundstation. It destroys all loaded instances.
 *
 **/
MAVLinkGen::~MAVLinkGen()
{
}

