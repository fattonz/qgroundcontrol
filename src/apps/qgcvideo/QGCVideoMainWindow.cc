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
    QString flowdepthonly;
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
    unsigned char fdo = data[2];


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
    flowdepthonly.append(QString().sprintf("%03d", fdo));

    qDebug() << "Received" << data.size() << "bytes"<< " part: " <<index<< " imageid: " <<imageid << " flowdepthonly: " <<flowdepthonly;


    //visualize gradient image
    for (int i=0; i<data.size()/4-1; i++)
    {
    if(data[i*4+6]!=0){
    data[i*4+6] = data[i*4+6]+128;
}
}

    switch (i0)
    {
    case 0x01:
        {
            for (int i=0; i<data.size()/4-1; i++)
            {
                imageRecBuffer1[i] = data[i*4+4];
                imageRecBuffer2[i] = data[i*4+5];
                imageRecBuffer3[i] = data[i*4+6];
                //imageRecBuffer4[i] = data[i*4+7];
              //imageRecBuffer3[i+310] = data[i*4+6];
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
                //imageRecBuffer4[i+45120/4] = data[i*4+7];
               // imageRecBuffer3[i+45120/4+310] = data[i*4+6];
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
               // imageRecBuffer4[i+45120/4*2] = data[i*4+7];
               // imageRecBuffer3[i+45120/4*2+310] = data[i*4+6];
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
                //imageRecBuffer4[i+45120/4*3] = data[i*4+7];
                //imageRecBuffer3[i+45120/4*3+310] = data[i*4+6];
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
                //imageRecBuffer4[i+45120/4*4] = data[i*4+7];
                //imageRecBuffer3[i+45120/4*4+310] = data[i*4+6];
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
               // imageRecBuffer4[i+45120/4*5] = data[i*4+7];
               // imageRecBuffer3[i+45120/4*5+310] = data[i*4+6];
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
               // imageRecBuffer4[i+45120/4*6] = data[i*4+7];
                //imageRecBuffer3[i+45120/4*6+310] = data[i*4+6];
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
               // imageRecBuffer4[i+45120/4*7] = data[i*4+7];
               // imageRecBuffer3[i+45120/4*7+310] = data[i*4+6];
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

            if(fdo==1)
            {
                for (int i=0; i<90240; i++)
                {
                    imageRecBuffer1[i] = 0;
                    imageRecBuffer2[i] = 0;
                    imageRecBuffer3[i] = 0;
                    imageRecBuffer4[i] = 0;
                }
                part = 511;
            }
            break;
        }
    }
    last_id = id;


    if(part==511)
    {


////////////////
float colormapJet[128][3] =
         {
           {0.0f,0.0f,0.53125f},
           {0.0f,0.0f,0.5625f},
           {0.0f,0.0f,0.59375f},
           {0.0f,0.0f,0.625f},
           {0.0f,0.0f,0.65625f},
           {0.0f,0.0f,0.6875f},
           {0.0f,0.0f,0.71875f},
           {0.0f,0.0f,0.75f},
           {0.0f,0.0f,0.78125f},
           {0.0f,0.0f,0.8125f},
           {0.0f,0.0f,0.84375f},
           {0.0f,0.0f,0.875f},
           {0.0f,0.0f,0.90625f},
           {0.0f,0.0f,0.9375f},
           {0.0f,0.0f,0.96875f},
           {0.0f,0.0f,1.0f},
           {0.0f,0.03125f,1.0f},
           {0.0f,0.0625f,1.0f},
           {0.0f,0.09375f,1.0f},
           {0.0f,0.125f,1.0f},
           {0.0f,0.15625f,1.0f},
           {0.0f,0.1875f,1.0f},
           {0.0f,0.21875f,1.0f},
           {0.0f,0.25f,1.0f},
           {0.0f,0.28125f,1.0f},
           {0.0f,0.3125f,1.0f},
           {0.0f,0.34375f,1.0f},
           {0.0f,0.375f,1.0f},
           {0.0f,0.40625f,1.0f},
           {0.0f,0.4375f,1.0f},
           {0.0f,0.46875f,1.0f},
           {0.0f,0.5f,1.0f},
           {0.0f,0.53125f,1.0f},
           {0.0f,0.5625f,1.0f},
           {0.0f,0.59375f,1.0f},
           {0.0f,0.625f,1.0f},
           {0.0f,0.65625f,1.0f},
           {0.0f,0.6875f,1.0f},
           {0.0f,0.71875f,1.0f},
           {0.0f,0.75f,1.0f},
           {0.0f,0.78125f,1.0f},
           {0.0f,0.8125f,1.0f},
           {0.0f,0.84375f,1.0f},
           {0.0f,0.875f,1.0f},
           {0.0f,0.90625f,1.0f},
           {0.0f,0.9375f,1.0f},
           {0.0f,0.96875f,1.0f},
           {0.0f,1.0f,1.0f},
           {0.03125f,1.0f,0.96875f},
           {0.0625f,1.0f,0.9375f},
           {0.09375f,1.0f,0.90625f},
           {0.125f,1.0f,0.875f},
           {0.15625f,1.0f,0.84375f},
           {0.1875f,1.0f,0.8125f},
           {0.21875f,1.0f,0.78125f},
           {0.25f,1.0f,0.75f},
           {0.28125f,1.0f,0.71875f},
           {0.3125f,1.0f,0.6875f},
           {0.34375f,1.0f,0.65625f},
           {0.375f,1.0f,0.625f},
           {0.40625f,1.0f,0.59375f},
           {0.4375f,1.0f,0.5625f},
           {0.46875f,1.0f,0.53125f},
           {0.5f,1.0f,0.5f},
           {0.53125f,1.0f,0.46875f},
           {0.5625f,1.0f,0.4375f},
           {0.59375f,1.0f,0.40625f},
           {0.625f,1.0f,0.375f},
           {0.65625f,1.0f,0.34375f},
           {0.6875f,1.0f,0.3125f},
           {0.71875f,1.0f,0.28125f},
           {0.75f,1.0f,0.25f},
           {0.78125f,1.0f,0.21875f},
           {0.8125f,1.0f,0.1875f},
           {0.84375f,1.0f,0.15625f},
           {0.875f,1.0f,0.125f},
           {0.90625f,1.0f,0.09375f},
           {0.9375f,1.0f,0.0625f},
           {0.96875f,1.0f,0.03125f},
           {1.0f,1.0f,0.0f},
           {1.0f,0.96875f,0.0f},
           {1.0f,0.9375f,0.0f},
           {1.0f,0.90625f,0.0f},
           {1.0f,0.875f,0.0f},
           {1.0f,0.84375f,0.0f},
           {1.0f,0.8125f,0.0f},
           {1.0f,0.78125f,0.0f},
           {1.0f,0.75f,0.0f},
           {1.0f,0.71875f,0.0f},
           {1.0f,0.6875f,0.0f},
           {1.0f,0.65625f,0.0f},
           {1.0f,0.625f,0.0f},
           {1.0f,0.59375f,0.0f},
           {1.0f,0.5625f,0.0f},
           {1.0f,0.53125f,0.0f},
           {1.0f,0.5f,0.0f},
           {1.0f,0.46875f,0.0f},
           {1.0f,0.4375f,0.0f},
           {1.0f,0.40625f,0.0f},
           {1.0f,0.375f,0.0f},
           {1.0f,0.34375f,0.0f},
           {1.0f,0.3125f,0.0f},
           {1.0f,0.28125f,0.0f},
           {1.0f,0.25f,0.0f},
           {1.0f,0.21875f,0.0f},
           {1.0f,0.1875f,0.0f},
           {1.0f,0.15625f,0.0f},
           {1.0f,0.125f,0.0f},
           {1.0f,0.09375f,0.0f},
           {1.0f,0.0625f,0.0f},
           {1.0f,0.03125f,0.0f},
           {1.0f,0.0f,0.0f},
           {0.96875f,0.0f,0.0f},
           {0.9375f,0.0f,0.0f},
           {0.90625f,0.0f,0.0f},
           {0.875f,0.0f,0.0f},
           {0.84375f,0.0f,0.0f},
           {0.8125f,0.0f,0.0f},
           {0.78125f,0.0f,0.0f},
           {0.75f,0.0f,0.0f},
           {0.71875f,0.0f,0.0f},
           {0.6875f,0.0f,0.0f},
           {0.65625f,0.0f,0.0f},
           {0.625f,0.0f,0.0f},
           {0.59375f,0.0f,0.0f},
           {0.5625f,0.0f,0.0f},
           {0.53125f,0.0f,0.0f},
           {0.5f,0.0f,0.0f}
       };

//           for (int i=0; i<90240; i++)
//          {

//           }


           QByteArray coloredDepth(376*240*3, 0);
           QImage image4;

           for (uint32_t r = 0; r < 240; ++r)
           {
               //const float* depth = reinterpret_cast<const float*>(rgbdImage.imagedata2().c_str() + r * rgbdImage.step2());
               uint8_t* pixel = reinterpret_cast<uint8_t*>(coloredDepth.data()) + r * 376 * 3;
               for (uint32_t c = 0; c < 376; ++c)
               {
                   if (imageRecBuffer4[c+r * 376] != 0)
                   {
                       //int idx = fminf(depth[c], 7.0f) / 7.0f * 127.0f;
                       //idx = 127 - idx;
                        int idx = (uint8_t)imageRecBuffer4[c+r * 376]/2;

                       pixel[0] = colormapJet[idx][0] * 255.0f;
                       pixel[1] = colormapJet[idx][1] * 255.0f;
                       pixel[2] = colormapJet[idx][2] * 255.0f;
                   }

                   pixel += 3;
               }
           }

           image4 = QImage(reinterpret_cast<const uchar*>(coloredDepth.constData()),376, 240, QImage::Format_RGB888);

         ///////////////////

        QByteArray tmpImage1(header.toStdString().c_str(), header.toStdString().size());
        tmpImage1.append(imageRecBuffer1);
        QByteArray tmpImage2(header.toStdString().c_str(), header.toStdString().size());
        tmpImage2.append(imageRecBuffer2);
        QByteArray tmpImage3(header.toStdString().c_str(), header.toStdString().size());
        tmpImage3.append(imageRecBuffer3);
        //QByteArray tmpImage4(header.toStdString().c_str(), header.toStdString().size());
        //tmpImage4.append(imageRecBuffer4);

        // Load image into window
        //QImage test(":images/patterns/lenna.jpg");
        QImage image1;
        QImage image2;
        QImage image3;
        //QImage image4;



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
       /* if (imageRecBuffer4.isNull())
        {
            qDebug()<< "could not convertToPGM()";

        }

        if (!image4.loadFromData(tmpImage4, "PGM"))
        {
            qDebug()<< "could not create extracted image3";

        }*/
        tmpImage1.clear();
        tmpImage2.clear();
        tmpImage3.clear();
        //tmpImage4.clear();


        //save images
        QString imagename1 = ".bmp";
        QString imagename2 = ".bmp";
        imagename1.prepend(imageid);
        imagename2.prepend(imageid);
        imagename1.prepend("leftoriginal");
        imagename2.prepend("rightoriginal");
        QString imagename3 = ".bmp";
        QString imagename4 = ".bmp";
        imagename3.prepend(imageid);
        imagename4.prepend(imageid);
        imagename3.prepend("leftcorrected");
        imagename4.prepend("rightcorrected");

       // image1.save(imagename1,0,-1);
       // image2.save(imagename2,0,-1);
       // image3.save(imagename3,0,-1);
       // image4.save(imagename4,0,-1);

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

