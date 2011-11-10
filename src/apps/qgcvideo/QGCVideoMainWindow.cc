/*=====================================================================
 ======================================================================*/

/**
 * @file
 *   @brief Implementation of main window
 *
 *   @author Dominik Honegger
 *
 */

#include "QGCVideoMainWindow.h"
#include "ui_QGCVideoMainWindow.h"

#include "UDPLink.h"
#include <QDebug>

QByteArray imageRecBuffer1 = QByteArray(376*240,255);
QByteArray imageRecBuffer2 = QByteArray(376*240,255);
QByteArray imageRecBuffer3 = QByteArray(376*240,255);
QByteArray imageRecBuffer4 = QByteArray(376*240,255);
static int part = 0;
unsigned char last_id = 0;

QGCVideoMainWindow::QGCVideoMainWindow(QWidget *parent) :
        QMainWindow(parent),
        link(QHostAddress::Any, 5555),
        ui(new Ui::QGCVideoMainWindow)
{
    ui->setupUi(this);

    // Set widgets in video mode
    ui->video1Widget->enableVideo(true);
    ui->video2Widget->enableVideo(true);
    ui->video3Widget->enableVideo(true);
    ui->video4Widget->enableVideo(true);

    // Connect link to this widget, receive all bytes
    connect(&link, SIGNAL(bytesReceived(LinkInterface*,QByteArray)), this, SLOT(receiveBytes(LinkInterface*,QByteArray)));

    // Open port
    link.connect();
}

QGCVideoMainWindow::~QGCVideoMainWindow()
{
    delete ui;
}

void QGCVideoMainWindow::receiveBytes(LinkInterface* link, QByteArray data)
{
    // There is no need to differentiate between links
    // for this use case here
    Q_UNUSED(link);

    // Image data is stored in QByteArray
    // Output bytes and load Lenna!

    QString bytes;
    QString index;
    QString imageid;
    QString ascii;



    // TODO FIXME Fabian
    // RAW hardcoded to 22x22
    int imgWidth = 376;
    int imgHeight = 240;
    int imgColors = 255;


    //const int headerSize = 15;

    // Construct PGM header
    QString header("P5\n%1 %2\n%3\n");
    header = header.arg(imgWidth).arg(imgHeight).arg(imgColors);

    unsigned char i0 = data[0];
    unsigned char id = data[1];


    int xCount = 36;
    int yCount = 23;
    int flowdatacounterx = 0;
    int flowdatacounterx2 = 0;
    int flowdatacountery2 = 0;
    int flowdatacounterd = 0;

    char flowX[yCount*xCount];
    char flowY[yCount*xCount];
    unsigned char depth[yCount*xCount];
    char flowXavg = 0;
    char flowYavg = 0;

    index.append(QString().sprintf("%02x", i0));
    imageid.append(QString().sprintf("%03d", id));
    qDebug() << "Received" << data.size() << "bytes"<< " part: " <<index<< " imageid: " <<imageid;

    switch (i0)
    {
    case 0x01:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i] = data[i*4+4];
                imageRecBuffer2[i] = data[i*4+5];
                imageRecBuffer3[i] = data[i*4+6];
                imageRecBuffer4[i+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 1;
            break;
        }
    case 0x02:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4] = data[i*4+4];
                imageRecBuffer2[i+45120/4] = data[i*4+5];
                imageRecBuffer3[i+45120/4] = data[i*4+6];
                imageRecBuffer4[i+45120/4+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 2;
            break;
        }
    case 0x03:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4*2] = data[i*4+4];
                imageRecBuffer2[i+45120/4*2] = data[i*4+5];
                imageRecBuffer3[i+45120/4*2] = data[i*4+6];
                imageRecBuffer4[i+45120/4*2+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 4;
            break;
        }
    case 0x04:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4*3] = data[i*4+4];
                imageRecBuffer2[i+45120/4*3] = data[i*4+5];
                imageRecBuffer3[i+45120/4*3] = data[i*4+6];
                imageRecBuffer4[i+45120/4*3+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 8;
            break;
        }
    case 0x05:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4*4] = data[i*4+4];
                imageRecBuffer2[i+45120/4*4] = data[i*4+5];
                imageRecBuffer3[i+45120/4*4] = data[i*4+6];
                imageRecBuffer4[i+45120/4*4+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 16;
            break;
        }
    case 0x06:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4*5] = data[i*4+4];
                imageRecBuffer2[i+45120/4*5] = data[i*4+5];
                imageRecBuffer3[i+45120/4*5] = data[i*4+6];
                imageRecBuffer4[i+45120/4*5+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 32;
            break;
        }
    case 0x07:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4*6] = data[i*4+4];
                imageRecBuffer2[i+45120/4*6] = data[i*4+5];
                imageRecBuffer3[i+45120/4*6] = data[i*4+6];
                imageRecBuffer4[i+45120/4*6+310] = data[i*4+7];
            }
            if(id != last_id)
            {
                part = 0;
            }
            part = part | 64;
            break;
        }
    case 0x08:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i+45120/4*7] = data[i*4+4];
                imageRecBuffer2[i+45120/4*7] = data[i*4+5];
                imageRecBuffer3[i+45120/4*7] = data[i*4+6];
                imageRecBuffer4[i+45120/4*7+310] = data[i*4+7];
            }


            //            for (int i=0; i<828; i++){
            //                flowX[i] = 0;
            //                flowY[i] = 0;
            //                depth[i] = 0;
            //            }


            if(id != last_id)
            {
                part = 0;
            }
            part = part | 128;
            break;
        }



    case 0x0F:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                if (i>1) //0 lines
                {
                    if(flowdatacounterx<828)
                    {
                        if(-((data[i*4+6]&0xF0)-128)>-80&&-((data[i*4+6]&0xF0)-128)<80)
                        {
                            flowX[flowdatacounterx] = -(((data[i*4+6]&0xF0)-128)+((data[i*4+6]&0x0F)-8));
                        }
                        else
                        {
                            flowX[flowdatacounterx] = 0;
                        }
                        flowdatacounterx = flowdatacounterx+1;
                    }
                    //                    if(flowdatacounterx==828)
                    //                    {
                    //                        flowXavg = -(((data[(i+1)*4+6]&0xF0)-128)+((data[(i+1)*4+6]&0x0F)-8));
                    //                        flowdatacounterx = flowdatacounterx+1;
                    //                    }
                }
                if (i>1505) //5th line
                {
                    if(flowdatacounterx2<xCount)
                    {
                        if(-((data[i*4+6]&0xF0)-128)>-80&&-((data[i*4+6]&0xF0)-128)<80)
                        {
                            flowY[flowdatacountery2*xCount+flowdatacounterx2] = -(((data[i*4+6]&0xF0)-128)+((data[i*4+6]&0x0F)-8));
                        }
                        else
                        {
                            flowY[flowdatacountery2*xCount+flowdatacounterx2] = 0;
                        }
                        flowdatacountery2 = flowdatacountery2+1;
                        if(flowdatacountery2>yCount-4)
                        {
                            flowY[(flowdatacountery2+0)*xCount+flowdatacounterx2] = 0;
                            flowY[(flowdatacountery2+1)*xCount+flowdatacounterx2] = 0;
                            flowY[(flowdatacountery2+2)*xCount+flowdatacounterx2] = 0;
                            flowdatacountery2 = 0;
                            flowdatacounterx2 = flowdatacounterx2+1;

                        }
                    }
                    //                    if(i==9025+829)
                    //                    {
                    //                        flowYavg = -(((data[i*4+6]&0xF0)-128)+((data[i*4+6]&0x0F)-8));
                    //                        flowdatacounterx2 = flowdatacounterx2+1;
                    //                    }

                }
                if (i>3008) //9th line
                {
                    if(flowdatacounterd<828)
                    {
                        depth[flowdatacounterd] = data[i*4+6];
                    }
                    flowdatacounterd = flowdatacounterd+1;
                }
            }

            if(id != last_id)
            {
                part = 0;
            }
            part = part | 256;
            break;
        }
    }
    last_id = id;


    if(part==511)
    {

        QByteArray tmpImage1(header.toStdString().c_str(), header.toStdString().size());
        tmpImage1.append(imageRecBuffer1);
        QByteArray tmpImage2(header.toStdString().c_str(), header.toStdString().size());
        tmpImage2.append(imageRecBuffer2);
        QByteArray tmpImage3(header.toStdString().c_str(), header.toStdString().size());
        tmpImage3.append(imageRecBuffer3);
        QByteArray tmpImage4(header.toStdString().c_str(), header.toStdString().size());
        tmpImage4.append(imageRecBuffer4);

        // Load image into window
        //QImage test(":images/patterns/lenna.jpg");
        QImage image1;
        QImage image2;
        QImage image3;
        QImage image4;



        if (imageRecBuffer1.isNull())
        {
            qDebug()<< "could not convertToPGM()";

        }

        if (!image1.loadFromData(tmpImage1, "PGM"))
        {
            qDebug()<< "could not create extracted image1";

        }
        if (imageRecBuffer2.isNull())
        {
            qDebug()<< "could not convertToPGM()";

        }

        if (!image2.loadFromData(tmpImage2, "PGM"))
        {
            qDebug()<< "could not create extracted image2";

        }
        if (imageRecBuffer3.isNull())
        {
            qDebug()<< "could not convertToPGM()";

        }

        if (!image3.loadFromData(tmpImage3, "PGM"))
        {
            qDebug()<< "could not create extracted image3";

        }
        if (imageRecBuffer4.isNull())
        {
            qDebug()<< "could not convertToPGM()";

        }

        if (!image4.loadFromData(tmpImage4, "PGM"))
        {
            qDebug()<< "could not create extracted image3";

        }
        tmpImage1.clear();
        tmpImage2.clear();
        tmpImage3.clear();
        tmpImage4.clear();


        //save images
        QString imagename1 = ".bmp";
        QString imagename2 = ".bmp";
        imagename1.prepend(imageid);
        imagename2.prepend(imageid);
        imagename1.prepend("left");
        imagename2.prepend("rigth");
        image1.save(imagename1,0,-1);
        image2.save(imagename2,0,-1);

        //ui->video1Widget->copyImage(test);
        ui->video1Widget->copyFlow((const char*)flowX, (const char*)flowY,(const unsigned char*)depth,flowXavg,flowYavg, xCount, yCount);
        ui->video2Widget->copyFlow((const char*)flowX, (const char*)flowY,(const unsigned char*)depth,flowXavg,flowYavg, xCount, yCount);
        ui->video3Widget->copyFlow((const char*)flowX, (const char*)flowY,(const unsigned char*)depth,flowXavg,flowYavg, xCount, yCount);
        ui->video4Widget->copyFlow((const char*)flowX, (const char*)flowY,(const unsigned char*)depth,flowXavg,flowYavg, xCount, yCount);
        ui->video1Widget->copyImage(image1);
        ui->video2Widget->copyImage(image2);
        ui->video3Widget->copyImage(image3);
        ui->video4Widget->copyImage(image4);

        //ui->video4Widget->enableFlow(true);

        part = 0;
        imageRecBuffer1.clear();
        imageRecBuffer2.clear();
        imageRecBuffer3.clear();
        imageRecBuffer4.clear();

    }





    /* for (int j=0; j<data.size(); j++) {
        unsigned char v = data[j];
        bytes.append(QString().sprintf("%02x ", v));
        if (data.at(j) > 31 && data.at(j) < 127)
        {
            ascii.append(data.at(j));
        }
        else
        {
            ascii.append(219);
        }

    }*/

    //qDebug() << bytes;
    //qDebug() << "ASCII:" << ascii;




}

