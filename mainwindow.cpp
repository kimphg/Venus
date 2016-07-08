#include "mainwindow.h"
#include "ui_mainwindow.h"
//
//#define mapWidth 2000
//#define mapWidth mapWidth
//#define mapHeight mapWidth
#define CONST_NM 1.825f
#define CONST_PI 3.141592654f
#define MAX_VIEW_RANGE_KM 50
#define PI 3.141592654f
#define PI_NHAN2 6.28f
#define DEGREE_2_KM 111.31949079327357f
#include <queue>
short scrCtX, scrCtY;
short scrCtLat, scrCtLon;
RECT trackingRect;
bool onConnectVideo = false;
QList<CTarget*> targetList;
short selected_target_index = 0;
QRect toBeTracked;
bool isSelecting = false;
float           scale;
CConfig         config;
//pcap
//pcap_if_t *alldevs;
//pcap_if_t *d;
//pcap_t *adhandle;
//static bool                 isAddingTarget=false;
static QPixmap              *pMap=NULL;
//static QImage               *sgn_img = new QImage(RADAR_MAX_RESOLUTION*2,RADAR_MAX_RESOLUTION*2,QImage::Format_RGB32);
//dataProcessingThread        *processing;
static Q_vnmap              vnmap;
QTimer*                     scrUpdateTimer,*readBuffTimer, *videoTimer;
QTimer*                     syncTimer1s ;
QTimer*                     timer_lrad_control ;
QThread*                    t,*t1 ;
CvCapture                   *g_Capture = NULL;
IplImage                    *g_TrueFrame = NULL;
IplImage                    *g_Frame = NULL;
IplImage                    *g_FrameHalf = NULL;
bool                        g_IsTracking = false;
CTracker                    g_Tracker;
bool                        g_IsIR = false;
bool displayAlpha = false;
//static short                currMaxRange = RADAR_MAX_RESOLUTION;
//static short                currMaxAzi = MAX_AZIR,currMinAzi = 0;
static short                dxMax,dyMax;
//static C_ARPA_data          arpa_data;
//static short                curAzir=-1;//,drawAzir=0;
static float                signsize,sn_scale;
static short                 dx =0,dy=0,dxMap=0,dyMap=0;//mouseX,mouseY;
static short                mousePointerX,mousePointerY;
static char                 gridOff = false;
static char                 udpFailure = 0;//config file !!!
static bool                 isScreenUp2Date,isSettingUp2Date,isDraging = false;
static bool                 isScaleChanged =true;
bool                        isNewTTM = false;
QMutex                      mutex;
QImage *img = NULL;
static QStandardItemModel   modelTargetList;
enum drawModes{
    SGN_DIRECT_DRAW,SGN_IMG_DRAW,NOTERR_DRAW
}drawMode = SGN_IMG_DRAW;
short range = 1;

//typedef struct {
//    unsigned char        bytes[8];
//}
//Command_Control;
//typedef std::queue<Command_Control> CommandList;
//static CommandList command_queue;
bool isDrawSubTg = true;
//static unsigned short cur_object_index = 0;
int wait_time = 0;
//void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
//{
//    //pcap_breakloop(adhandle);

//    (VOID)(param);
//    (VOID)(pkt_data);
//    wait_time++;
//    if(wait_time>50){
//        pcap_breakloop(adhandle);
//        wait_time = 0;
//    }
///*

//    if(header->len<=500)return;
//    if(((*(pkt_data+36)<<8)|(*(pkt_data+37)))!=HR2D_UDP_PORT)
//    {
//        //printf("\nport:%d",((*(pkt_data+36)<<8)|(*(pkt_data+37))));
//        return;
//    }
//    dataB[iRec].len = header->len - UDP_HEADER_LEN;
//    memcpy(&dataB[iRec].data[0],pkt_data+UDP_HEADER_LEN,dataB[iRec].len);
//    iRec++;
//    if(iRec>=MAX_IREC)iRec = 0;
//    *pIsDrawn = false;
//    //printf("nhan duoc:%x\n",dataB[iRec].data[0]);

//    return;

//*/
//}
 short lon2x(float lon)
{

    //printf("scalse:%f",scale);
    return  (- dx + scrCtX + ((lon - config.m_config.m_long) * DEGREE_2_KM)*scale);
}
 short lat2y(float lat)
{

    return (- dy + scrCtY - ((lat - config.m_config.m_lat) * DEGREE_2_KM)*scale);
}
 double y2lat(short y)
{
    return (y +dy - scrCtY )/scale/DEGREE_2_KM + config.m_config.m_lat;
}
 double x2lon(short x)
{
    return (x +dx - scrCtX )/scale/DEGREE_2_KM + config.m_config.m_long;
}
int char2int( char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}
void hex2bin(const char* src,unsigned char* target)
{
  while(*src && src[1])
  {
    *(target++) = char2int(*src)*16 + char2int(src[1]);
    src += 2;
  }
  *(target++)=0;
}
void bin2hex(unsigned char byte, char* str)
{
    switch (byte>>4) {
    case 0:
        *str = '0';
        break;
    case 1:
        *str = '1';
        break;
    case 2:
        *str = '2';
        break;
    case 3:
        *str = '3';
        break;
    case 4:
        *str = '4';
        break;
    case 5:
        *str = '5';
        break;
    case 6:
        *str = '6';
        break;
    case 7:
        *str = '7';
        break;
    case 8:
        *str = '8';
        break;
    case 9:
        *str = '9';
        break;
    case 10:
        *str = 'A';
        break;
    case 11:
        *str = 'B';
        break;
    case 12:
        *str = 'C';
        break;
    case 13:
        *str = 'D';
        break;
    case 14:
        *str = 'E';
        break;
    case 15:
        *str = 'F';
        break;
    default:
        break;
    }
    switch (byte&(0x0F)) {
    case 0:
        *(str+1) = '0';
        break;
    case 1:
        *(str+1) = '1';
        break;
    case 2:
        *(str+1) = '2';
        break;
    case 3:
        *(str+1) = '3';
        break;
    case 4:
        *(str+1) = '4';
        break;
    case 5:
        *(str+1) = '5';
        break;
    case 6:
        *(str+1) = '6';
        break;
    case 7:
        *(str+1) = '7';
        break;
    case 8:
        *(str+1) = '8';
        break;
    case 9:
        *(str+1) = '9';
        break;
    case 10:
        *(str+1) = 'A';
        break;
    case 11:
        *(str+1) = 'B';
        break;
    case 12:
        *(str+1) = 'C';
        break;
    case 13:
        *(str+1) = 'D';
        break;
    case 14:
        *(str+1) = 'E';
        break;
    case 15:
        *(str+1) = 'F';
        break;
    default:
        break;
    }

}
void inline DrawTrackingRgn(IplImage* img, RECT rect)
{
    if (!g_IsTracking)
        return;

    CvRect cvRectBox = cvRect(0, 0, 0, 0);

    utl_ConvertRectToBox(rect, &cvRectBox);

    if ((cvRectBox.width <= 10) ||(cvRectBox.height <= 10))
    {
        g_IsTracking = false;
        return;
    }

    CvPoint ltPoint = cvPoint(cvRectBox.x, cvRectBox.y);                                        // left top point of region
    CvPoint bdPoint = cvPoint(cvRectBox.x + cvRectBox.width, cvRectBox.y + cvRectBox.height);	// bottom down point of region
    CvPoint rtPoint = cvPoint(cvRectBox.x + cvRectBox.width, cvRectBox.y);                      // right top point of region
    CvPoint lbPoint = cvPoint(cvRectBox.x, cvRectBox.y + cvRectBox.height);                     // left bottom point of region

    CvScalar trackScalar = cvScalar(0, 255, 0);

    cvLine(img, ltPoint, cvPoint(ltPoint.x + 10, ltPoint.y), trackScalar, 2);
    cvLine(img, ltPoint, cvPoint(ltPoint.x, ltPoint.y + 10), trackScalar, 2);

    cvLine(img, bdPoint, cvPoint(bdPoint.x - 10, bdPoint.y), trackScalar, 2);
    cvLine(img, bdPoint, cvPoint(bdPoint.x, bdPoint.y - 10), trackScalar, 2);

    cvLine(img, rtPoint, cvPoint(rtPoint.x - 10, rtPoint.y), trackScalar, 2);
    cvLine(img, rtPoint, cvPoint(rtPoint.x, rtPoint.y + 10), trackScalar, 2);

    cvLine(img, lbPoint, cvPoint(lbPoint.x, lbPoint.y - 10), trackScalar, 2);
    cvLine(img, lbPoint, cvPoint(lbPoint.x + 10, lbPoint.y), trackScalar, 2);

    return;
}
void Mainwindow::updateTargets()
{
    for(short i = 0;i<targetList.size();i++)
    {
        float x	= lon2x(targetList.at(i)->m_lon) ;

        float y	= lat2y(targetList.at(i)->m_lat);
        float w = scrCtY-20;
        float dx = x-scrCtX;
        float dy = y-scrCtY;
        if(dx*dx+dy*dy>(w*w))
        {
                targetList.at(i)->hide();
        }
        else
        {
            targetList.at(i)->show();
            targetList.at(i)->setPosistion(x,y);
        }

        if(targetList.at(i)->clicked)
        {

            selected_target_index = i;

            targetList.at(i)->clicked = false;
        }
        if(selected_target_index == i)
        {
            ui->label_radar_id->setText((targetList.at(i)->id));
            ui->label_range_radar->setText(QString::number(targetList.at(i)->range)+"Nm");
            ui->label_status_azi_radar->setText( QString::number(targetList.at(i)->azi)+"\xB0");
            ui->label_status_lat_radar->setText( QString::number((short)targetList.at(i)->m_lat)+"\xB0"+QString::number((targetList.at(i)->m_lat-(short)targetList.at(i)->m_lat)*60,'g',4)+"N");
            ui->label_status_long_radar->setText(QString::number((short)targetList.at(i)->m_lon)+"\xB0"+QString::number((targetList.at(i)->m_lon-(short)targetList.at(i)->m_lon)*60,'g',4)+"E");
            ui->label_status_speed_radar->setText(QString::number(targetList.at(i)->speed)+"Kn");
        }
        else
        {
            targetList.at(i)->setSelected(false);// = false;
        }
        //printf("\nx:%f y:%f",x,y);
    }
    //ui->
    //t1.setGeometry(400,400,20,20);
    //targetList.append(t1);
}

void Mainwindow::sendToRadar(const char* hexdata)
{
    short len = strlen(hexdata)/2+1;
    unsigned char* sendBuff = new unsigned char[len];
    hex2bin(hexdata,sendBuff);
    udpSendSocket->writeDatagram((char*)sendBuff,8,QHostAddress("192.168.0.44"),2572);
    delete[] sendBuff;

}
void Mainwindow::sendToRadar(unsigned char* hexdata)
{

    udpSendSocket->writeDatagram((char*)hexdata,8,QHostAddress("192.168.0.44"),2572);
    //delete[] sendBuff;

}

void Mainwindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(isSelecting)
    {
        QRect videoRect = ui->groupBox_video->geometry();
        trackingRect.right = event->x() - videoRect.left();
        trackingRect.bottom = event->y() - videoRect.top();
        short temp;
        if(trackingRect.right<trackingRect.left)
        {
            temp = trackingRect.right;
            trackingRect.right  = trackingRect.left;
            trackingRect.left = temp;
        }
        if(trackingRect.bottom<trackingRect.top)
        {
            temp = trackingRect.bottom;
            trackingRect.bottom =trackingRect.top;
            trackingRect.top = temp;
        }
        if(trackingRect.left<0)trackingRect.left=0;
        if(trackingRect.right>799)trackingRect.right=799;
        if(trackingRect.top<0)trackingRect.top=0;
        if(trackingRect.bottom>599)trackingRect.bottom=599;
        StartTracking(trackingRect);
        ui->toolButton_radar_tracking->setChecked(false);
        isSelecting = false;
    }
    ui->label_cursor_lat->setText(QString::number( (short)y2lat(event->y()))+"\xB0"+
                                  QString::number(((float)y2lat(event->y())-(short)(y2lat(event->y())))*60,'g',4)+"N");
    //ui->label_status_lat_radar->setText( QString::number((short)targetList.at(i)->m_lat)+"\xB0"+QString::number((targetList.at(i)->m_lat-(short)targetList.at(i)->m_lat)*60,'g',4)+"N");
//    ui->label_cursor_lon->setText(QString::number( x2lon(event->x())));
     ui->label_cursor_lon->setText(QString::number( (short)x2lon(event->x()))+"\xB0"+
                                   QString::number(((float)x2lon(event->x())-(short)(x2lon(event->x())))*60,'g',4)+"E");
    float x = (event->x() - scrCtX+dx)/scale/CONST_NM;
    float y = -(event->y() - scrCtY+dy)/scale/CONST_NM;
    float azi =  atanf(x/y);
            if(y<0)azi+=PI;
            if(azi<0)azi += PI_NHAN2;
    float range = sqrt(x*x+y*y);
    ui->label_cursor_azi->setText(QString::number( azi/PI*180));
    ui->label_cursor_azi->setText(QString::number( azi/PI*180,'g',4)+"\xB0");
    ui->label_cursor_range->setText(QString::number( range,'g',2)+"Nm");
    //ui->label_cursor_azi->setText(QString::number( y2azi(event->y())));
    //ui->label_cursor_lon->setText(QString::number( x2lon(event->x())));
//    if(isAddingTarget)
//    {
//        float xRadar = (mouseX - scrCtX+dx)/signsize ;//coordinates in  radar xy system
//        float yRadar = -(mouseY - scrCtY+dy)/signsize;
//        processing->radarData->addTrack(xRadar,yRadar);
//        ui->actionAddTarget->toggle();
//        isScreenUp2Date = false;
//        return;
//    }
    //processing->radarData->updateZoomRect(mousePointerX - scrCtX+dx,mousePointerY - scrCtY+dy);
    if(isDraging)
    {
        DrawMap();
        isScreenUp2Date = false;
        isDraging = false;
    }
    QMainWindow::mouseReleaseEvent(event);
    /*currMaxRange = (sqrtf(dx*dx+dy*dy)+scrCtY)/signsize;
    if(currMaxRange>RADAR_MAX_RESOLUTION)currMaxRange = RADAR_MAX_RESOLUTION;
    if((dx*dx+dy*dy)*3>scrCtX*scrCtX)
    {
        if(dx<0)
        {
            currMaxAzi = (unsigned short)((atanf((float)dy/(float)dx)-processing->radarData->trueN)/PI_NHAN2*4096.0f);
            if(currMaxAzi<0)currMaxAzi+=MAX_AZIR;
            if(currMaxAzi>MAX_AZIR)currMaxAzi-=MAX_AZIR;
        }
        if(dx>0)
        {
            currMaxAzi = (unsigned short)(((atanf((float)dy/(float)dx)+PI-processing->radarData->trueN))/PI_NHAN2*4096.0f);
            if(currMaxAzi>MAX_AZIR)currMaxAzi-=MAX_AZIR;
            if(currMaxAzi<0)currMaxAzi+=MAX_AZIR;
        }
        currMinAzi = currMaxAzi - MAX_AZIR/2;
        if(currMinAzi<0)currMinAzi+=MAX_AZIR;
        //printf("\n currMinAzi:%d currMaxAzi:%d ",currMinAzi,currMaxAzi);
    }else
    {
        currMaxAzi = MAX_AZIR;
        currMinAzi = 0;
    }*/
}
void Mainwindow::wheelEvent(QWheelEvent *event)
{
    //if(event->delta()>0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
    //if(event->delta()<0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
}
void Mainwindow::mouseMoveEvent(QMouseEvent *event) {
    //if(!isDraging)return;
    //mouseX = (event->x());
    //mouseY = (event->y());
    if(isSelecting)
    {
        toBeTracked.setRight(event->x()) ;
        toBeTracked.setBottom(event->y());
        //if(!toBeTracked.contains(event->x(),event->y())) isSelecting = false;
    }
    if(isDraging&&(event->buttons() & Qt::LeftButton))
    {

        while(dx*dx+dy*dy>dxMax*dxMax)
        {
            if(abs(dx)>abs(dy))
            {
                if(dx>0){dx--;dxMap--;}else {dx++;dxMap++;}}
            else
            {
                if(dy>0){dy--;dyMap--;}else {dy++;dyMap++;}
            }
        }
        dx+= mousePointerX-event->x();
        dy+= mousePointerY-event->y();
        dxMap += mousePointerX-event->x();
        dyMap += mousePointerY-event->y();
        mousePointerX=event->x();
        mousePointerY=event->y();
        isScreenUp2Date = false;
    }
}
void Mainwindow::mousePressEvent(QMouseEvent *event)
{
    if(event->x()>height())
    {
        if(event->buttons() & Qt::LeftButton)
        {
            QRect videoRect = ui->groupBox_video->geometry();
            //videoRect.adjust(4,30,-5,-5);
            if(videoRect.contains(event->x(),event->y()))
            {
                g_IsTracking = false;
                isSelecting = true;
                trackingRect.left = event->x() - videoRect.left();
                trackingRect.top = event->y() - videoRect.top();
                toBeTracked.setLeft(event->x());
                toBeTracked.setTop(event->y());
                toBeTracked.setRight(event->x()) ;
                toBeTracked.setBottom( event->y());
            }
        }
        else
        {
            g_IsTracking = false;
        }


    }
    else
    {
        mousePointerX = (event->x());
        mousePointerY = (event->y());

        //ui->frame_RadarViewOptions->hide();
        if(event->buttons() & Qt::LeftButton) {
            isDraging = true;
            mousePointerX=event->x();
            mousePointerY=event->y();
            //printf("mouseX %d\n",mouseX);
        }
    }
    QMainWindow::mousePressEvent(event);
//    if(selectobject) {


//    }

}
/*void MainWindow::wheelEvent(QWheelEvent *event)
{
//    if(event->delta()>0)ui->horizontalSlider->raise();
//    if(event->delta()<0)ui->horizontalSlider->setValue(3);
//    if(scale>SCALE_MAX)scale=SCALE_MAX;
//    if(scale<SCALE_MIN)scale=SCALE_MIN;
//    //signsize = SIGNAL_SCALE/scale;
//    DrawMap();
//    update();
}*/
Mainwindow::Mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->frame_RadarViewOptions->hide();
    QFont font;
    font.setPointSize(12);
    //ui->listTargetWidget->setFont(font);
    //ui->frame_2->setStyleSheet("#frame_2 { border: 2px solid darkgreen; }");
    //ui->frame_3->setStyleSheet("#frame_3 { border: 2px solid darkgreen; }");
    //if(!this->isFullScreen())this->showFullScreen();
    InitNetwork();
    InitTimer();
    setFocusPolicy(Qt::StrongFocus);
    InitSetting();
    setRadarState(DISCONNECTED);
    //init drawing context

    //this->setFixedSize(900 + ui->toolBar_Main->width()*3,850);
    //scale = SCALE_MIN;



    //isSettingUp2Date = false;
    //UpdateSetting();

}

void Mainwindow::DrawSignal(QPainter *p)
{
}


void Mainwindow::gpsOption()
{
    //GPSDialog *dlg = new GPSDialog;
    //dlg->show();
}

void Mainwindow::PlaybackRecFile()//
{


}
//void MainWindow::createActions()
//{
//    a_openShp = new QAction(tr("&Open Shp"), this);
//    a_openShp->setShortcuts(QKeySequence::Open);
//    a_openShp->setStatusTip(tr("Open shp file"));
//    connect(a_openShp, SIGNAL(triggered()), this, SLOT(openShpFile()));
//    //______________________________________//
//    a_openPlace = new QAction(tr("&Open place file"), this);
//    a_openPlace->setStatusTip(tr("Open place file"));
//    connect(a_openPlace, SIGNAL(triggered()), this, SLOT(openPlaceFile()));
//    //______________________________________//
//    a_gpsOption = new QAction(tr("&GPS option"), this);
//    a_gpsOption->setStatusTip(tr("GPS option"));
//    connect(a_gpsOption, SIGNAL(triggered()), this, SLOT(gpsOption()));
//    //______________________________________//
//    a_openSignal = new QAction(tr("&Open signal file"), this);
//    a_openSignal->setStatusTip(tr("Mở file tín hiệu đã lưu."));
//    connect(a_openSignal, SIGNAL(triggered()), this, SLOT(openSignalFile()));

//}
//void MainWindow::openSignalFile()
//{
//    //printf("shp file max ");
//    QString fileName = QFileDialog::getOpenFileName(this,
//        tr("Open signal file"), NULL, tr("Signal data Files (*.dat)"));
//    rawData.OpenFile(fileName.toStdString().c_str());

//    //SHPHandle hSHP = SHPOpen(fileName.toStdString().c_str(), "rb" );
//    //if(hSHP == NULL) return;
//}
/*
static short curMapLayer=0;

void MainWindow::openShpFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open SHP file"), NULL, tr("Shp Files (*.shp)"));
    if(!fileName.size())return;
    vnmap.OpenShpFile(fileName.toStdString().c_str(), curMapLayer );
    vnmap.LoadPlaces(fileName.toStdString().c_str());
    curMapLayer++;
    DrawMap();
    //DrawToPixmap(pPixmap);
    update();

}*/

Mainwindow::~Mainwindow()
{
    delete ui;

    if(pMap)delete pMap;
}

void Mainwindow::DrawMap()
{

    if(!pMap) return;

    dxMap = 0;
    dyMap = 0;
    QPainter p(pMap);
    pMap->fill(QColor(0,0,0,255));
    //pMap->fill(Qt::transparent);
    //if(ui->toolButton_map->isChecked()==false)return;
    QPen pen(QColor(255,255,255,180));
    QColor color[5];
    color[0].setRgb(143,137,87,255);//land
    color[1].setRgb( 34,52,60,255);//lake
    color[2].setRgb(60,50,10,255);//building
    color[3].setRgb( 34,52,60,255);//river
    color[4].setRgb(70,70,70,150);//road
    //color[0].setRgb(120,120,120,150);//land
    //color[1].setRgb( 120,120,120,150);//lake
    //color[2].setRgb(120,120,120,150);//building
    //color[3].setRgb( 120,120,120,150);//river
    //color[4].setRgb(0,100,120,150);//road

    short centerX = pMap->width()/2-dx;
    short centerY = pMap->height()/2-dy;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(pen);
    //-----draw provinces in polygons


    for(uint i = 0; i < N_LAYER; i++) {
        //printf("vnmap.layers[i].size()%d\n",vnmap.layers[i].size());
        if(i<3)
        {
            for(uint j = 0; j < vnmap.layers[i].size(); j++) {
                QPolygon poly;
                for(uint k = 0; k < vnmap.layers[i][j].size(); k++) { // Polygon
                    QPoint int_point;
                    float x,y;
                    vnmap.ConvDegToScr(&x,&y,&vnmap.layers[i][j][k].m_Long,&vnmap.layers[i][j][k].m_Lat);
                    int_point.setX((int)(x*scale)+centerX);
                    int_point.setY((int)(y*scale)+centerY);
                    poly<<int_point;
                }
                p.setBrush(color[i]);
                pen.setColor(color[i]);
                p.setPen(pen);
                p.drawPolygon(poly);
            }
        }else
        {
            //pen.setColor(color[i]);
            if(i==3)pen.setWidth(2);else pen.setWidth(1);
            p.setPen(pen);
            for(uint j = 0; j < vnmap.layers[i].size(); j++) {

                QPoint old_point;

                for(uint k = 0; k < vnmap.layers[i][j].size(); k++) { // Polygon
                    QPoint int_point;
                    float x,y;
                    vnmap.ConvDegToScr(&x,&y,&vnmap.layers[i][j][k].m_Long,&vnmap.layers[i][j][k].m_Lat);
                    int_point.setX((int)(x*scale)+centerX);
                    int_point.setY((int)(y*scale)+centerY);
                    if(k)p.drawLine(old_point,int_point);
                    old_point=int_point;
                }
                //p.setBrush(color[i]);


            }
        }

    }
    //DrawGrid(&p,centerX,centerY);
    //draw text
    pen.setColor(QColor(150,130,110,150));
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    p.setPen(pen);
    //QPen pen2(Qt::red);
    //pen2.setWidth(2);
    for(uint i = 0; i < vnmap.placeList.size(); i++) {
            QPoint int_point;
            float x,y;
            vnmap.ConvDegToScr(&x,&y,&vnmap.placeList[i].m_Long,&vnmap.placeList[i].m_Lat);
            int_point.setX((int)(x*scale)+centerX);
            int_point.setY((int)(y*scale)+centerY);
            //p.drawEllipse(int_point,2,2);
            p.drawText(int_point.x()+5,int_point.y(),QString::fromWCharArray(vnmap.placeList[i].text.c_str()));
            //printf("toa do hien tai lat %f long %f\n",m_textList[i].m_Lat,m_textList[i].m_Long);
    }
    isScreenUp2Date=false;

}
void Mainwindow::DrawGrid(QPainter* p,short centerX,short centerY)
{
    //return;
    QPen pen(QColor(0x7f,0x7f,0x7f,0xff));
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    p->setBrush(QBrush(Qt::NoBrush));
    p->setPen(pen);
    p->drawLine(centerX-5,centerY,centerX+5,centerY);
    p->drawLine(centerX,centerY-5,centerX,centerY+5);
    //pen.setColor(QColor(30,90,150,120));
    float step = ui->label_range->text().split(' ').at(0).toFloat()/4.0;
    //printf("step:%f",step);
    pen.setWidth(2);
    p->setPen(pen);

    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*CONST_NM*scale),
                  (short)(step*CONST_NM*scale));
    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*2*CONST_NM*scale),
                  (short)(step*2*CONST_NM*scale));
    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*3*CONST_NM*scale),
                  (short)(step*3*CONST_NM*scale));
    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*4*CONST_NM*scale),
                  (short)(step*4*CONST_NM*scale));
    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*5*CONST_NM*scale),
                  (short)(step*5*CONST_NM*scale));
    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*6*CONST_NM*scale),
                  (short)(step*6*CONST_NM*scale));
    p->drawEllipse(QPoint(centerX,centerY),
                  (short)(step*7*CONST_NM*scale),
                  (short)(step*7*CONST_NM*scale));
    return;
    if(gridOff == 0)//with frame
    {
        //p->drawLine(0,centerY,width(),centerY);
        //p->drawLine(centerX,0,centerX,height());
    }
    else
    {
        //p.drawEllipse(QPoint(centerX,centerY),(int)(20*CONST_NM*scale),(int)(20*CONST_NM*scale));
        //p.drawEllipse(QPoint(centerX,centerY),(int)(5*CONST_NM*scale),(short)(5*CONST_NM*scale));
        //pen.setWidth(1);
        //p->setPen(pen);
        short theta;
        short gridR = height()*0.7;
        for(theta=0;theta<360;theta+=90){
            QPoint point1,point2;
                short dx = gridR*cosf(CONST_PI*theta/180);
                short dy = gridR*sinf(CONST_PI*theta/180);
                point1.setX(centerX+0.33*dx);
                point1.setY(centerY+0.33*dy);
                point2.setX(centerX+dx);
                point2.setY(centerY+dy);
                p->drawLine(point1,point2);

        }
        for(theta=0;theta<360;theta+=10){
            QPoint point1,point2;
                short dx = gridR*cosf(CONST_PI*theta/180);
                short dy = gridR*sinf(CONST_PI*theta/180);
                point1.setX(centerX+0.95*dx);
                point1.setY(centerY+0.95*dy);
                point2.setX( centerX+dx);
                point2.setY(centerY+dy);
                p->drawLine(point1,point2);
                point2.setX(centerX+dx*1.02-9);
                point2.setY(centerY+dy*1.02+5);
                if(theta<270)p->drawText(point2,QString::number(theta+90));
                else p->drawText(point2,QString::number(theta-270));

        }
        for(theta=0;theta<360;theta+=2){
            QPoint point1,point2;
                short dx = gridR*cosf(CONST_PI*theta/180);
                short dy = gridR*sinf(CONST_PI*theta/180);
                point1.setX(centerX+0.98*dx);
                point1.setY(centerY+0.98*dy);
                point2.setX(centerX+dx);
                point2.setY(centerY+dy);
                p->drawLine(point1,point2);

        }
        //end grid
    }


}

void Mainwindow::drawAisTarget(QPainter *p)
{


}
void Mainwindow::paintEvent(QPaintEvent *event)
{
    event = event;
    isScreenUp2Date = true;
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing, true);

    if(pMap)
    {
        p.drawPixmap(scrCtX-scrCtY,0,height(),height(),
                         *pMap,
                         dxMap,dyMap,height(),height());
    }
    //draw signal
    //DrawSignal(&p);

    //drawAisTarget(&p);
    //draw cursor
    QPen penmousePointer(QColor(0x50ffffff));
    /*if(!isDraging)
    {
        pencurPos.setWidth(1);
        p.setPen(pencurPos);
        p.drawLine(mouseX-10,mouseY,mouseX+10,mouseY);
        p.drawLine(mouseX,mouseY-10,mouseX,mouseY+10);
    }*/
    penmousePointer.setWidth(2);
    p.setPen(penmousePointer);
    p.drawLine(mousePointerX-15,mousePointerY,mousePointerX-10,mousePointerY);
    p.drawLine(mousePointerX+15,mousePointerY,mousePointerX+10,mousePointerY);
    p.drawLine(mousePointerX,mousePointerY-10,mousePointerX,mousePointerY-15);
    p.drawLine(mousePointerX,mousePointerY+10,mousePointerX,mousePointerY+15);
    QPoint point(height()/2-dx,height()/2-dy);
    p.drawEllipse(point,10,10);
    QPoint point2(height()/2-dx+40*sin(config.m_config.trueN/180.0*PI),height()/2-dy-40*cos(config.m_config.trueN/180.0*PI));
    p.drawLine(point,point2);
    //draw zooom

    //draw frame

    DrawViewFrame(&p);
    if(true)
    {
        if(onConnectVideo&&(img))
        {
        //QMutexLocker locker(&mutex);
        QRect rect = ui->groupBox_video->geometry();

        //p.setPen(QPen(Qt::black));
        //p.setBrush(QBrush(Qt::black));
        //p.drawRect(rect);
        p.drawImage(rect,*img,img->rect());
        }
        else
        {
            QRect rect = ui->groupBox_video->geometry();
            p.setPen(QPen(Qt::red));
            QFont font;
            font.setPointSize(50);
            p.setFont(font);
            rect.adjust(250,250,0,0);
            p.drawText(rect,"NO VIDEO");
        }
    }


    if(isSelecting)
    {
        p.setPen(QPen(Qt::red));
        p.setBrush(QBrush(Qt::transparent));
        p.drawRect(toBeTracked);
    }
}
//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_F1)
//    {
//    selectobject = true;
//    }
//    switch(event->key())
//    {
//    case Qt::Key_Alt:
//        if(ui->menuBar->isVisible())
//            ui->menuBar->hide();
//        else
//            ui->menuBar->show();
//        break;
//    default:
//        break;
//    }

//}


bool Mainwindow::LoadISMapFile()
{
    if(config.m_config.mapFilename.size())
    {
        vnmap.LoadBinFile((config.m_config.mapFilename).data());
    }else return false;
    return true;
}
void Mainwindow::SaveBinFile()
{
    //vnmap.SaveBinFile();

}
void Mainwindow::InitSetting()
{
    QRect rec = QApplication::desktop()->screenGeometry(0);
    setFixedSize(1920, 1080);
    if((rec.height()==1080)&&(rec.width()==1920))
    {
        this->showFullScreen();
        this->setGeometry(QApplication::desktop()->screenGeometry(0));//show on first screen
    }
    else
    {

        rec = QApplication::desktop()->screenGeometry(1);
        if((rec.height()==1080)&&(rec.width()==1920))
        {
            this->showFullScreen();
            //printf("error");
            this->setGeometry(QApplication::desktop()->screenGeometry(1));//show on second screen
            //setFixedSize(QApplication::desktop()->screenGeometry(1));
        }

    }
    mousePointerX = width()/2;
    mousePointerY = height()/2;
    dxMax = height()/4-10;
    dyMax = height()/4-10;
    ui->groupBox_video->setAttribute(Qt::WA_TransparentForMouseEvents);
    range = 6; UpdateScale();
    if(true)
    {
        ui->textEdit_heading->setText(QString::number(config.m_config.trueN));
        SetGPS(config.m_config.m_lat, config.m_config.m_long);
        vnmap.setUp(config.m_config.m_lat, config.m_config.m_long, 50,"outputMap4layer.ism");
        if(pMap)delete pMap;
        pMap = new QPixmap(height(),height());

        DrawMap();


    }else
    {
        vnmap.ClearData();
        if(pMap)delete  pMap;
        pMap = NULL;
    }

    update();
}
void Mainwindow::ReloadSetting()
{

        scrCtX = height()/2;//+ ui->toolBar_Main->width()+20;//ENVDEP
        scrCtY = height()/2;


    UpdateScale();
    isSettingUp2Date = true;

}
void Mainwindow::UpdateSetting()
{
    isSettingUp2Date = false;
}

void Mainwindow::DrawViewFrame(QPainter* p)
{
    //draw grid

    if(ui->toolButton_grid->isChecked())
    {
        DrawGrid(p,scrCtX-dx,scrCtY-dy);
    }
    short d = height()-50;
    QPen penBackground(QColor(40,60,100,255));
    short linewidth = 0.6*height();
    penBackground.setWidth(linewidth/10);
    p->setPen(penBackground);
    for (short i=linewidth/12;i<linewidth;i+=linewidth/6)
    {
        p->drawEllipse(-i/2+(scrCtX-scrCtY)+25,-i/2+25,d+i,d+i);
    }
    penBackground.setWidth(0);
    p->setPen(penBackground);
    p->setBrush(QColor(40,60,100,255));
    p->drawRect(scrCtX+scrCtY,0,width()-scrCtX-scrCtY,height());
    p->drawRect(0,0,scrCtX-scrCtY,height());
    p->setBrush(Qt::NoBrush);
    QPen pengrid(QColor(128,128,0,255));
    pengrid.setWidth(4);
    p->setPen(pengrid);
    p->drawEllipse(scrCtX-scrCtY+25,25,d,d);
    pengrid.setWidth(2);
    p->setPen(pengrid);
    QFont font = p->font() ;
    font.setPointSize(8);
    font.setBold(true);
    p->setFont(font);
    //short theta;
    for(short theta=0;theta<360;theta+=10){
        QPoint point0,point1,point2;
        float tanA = tanf(theta/57.295779513f);
        float sinA = sinf(theta/57.295779513f);
        float cosA = cosf(theta/57.295779513f);
        float a = (1+1.0f/tanA/tanA);//4*(dy/tanA-dx)*(dy/tanA-dx) -4*(1+1/tanA)*(dx*dx+dy*dy-width()*width()/4);
        float b= 2.0f*(dy/tanA - dx);
        float c= dx*dx+dy*dy-d*d/4.0f;
        float delta = b*b-4.0f*a*c;
        if(delta<30.0f)continue;
        delta = sqrtf(delta);

        if(theta==0)
                {
                    point2.setX(scrCtX  - dx);
                    point2.setY(scrCtY - sqrt((d*d/4.0f- dx*dx)));
                    point1.setX(point2.x());
                    point1.setY(point2.y()-5.0);
                    point0.setX(point2.x());
                    point0.setY(point2.y()-18);
                }
        else if (theta<180)
        {
            short rx = (-b + delta)/2.0f/a;
            short ry = -rx/tanA;
            if(abs(rx)<100&&abs(ry)<100)continue;
            point2.setX(scrCtX + rx -dx);
            point2.setY(scrCtY + ry-dy);
            point1.setX(point2.x()+5.0*sinA);
            point1.setY(point2.y()-5.0*cosA);
            point0.setX(point2.x()+18.0*sinA);
            point0.setY(point2.y()-18.0*cosA);
        }
        else if(theta==180)
                {

                    point2.setX(scrCtX  - dx);
                    point2.setY(scrCtY + sqrt((d*d/4.0- dx*dx)));
                    point1.setX(point2.x());
                    point1.setY(point2.y()+5.0);
                    point0.setX(point2.x());
                    point0.setY(point2.y()+18.0);
                }
        else
        {
            short rx;
            short ry;
            rx =  (-b - delta)/2.0f/a;
            ry = -rx/tanA;
            if(abs(rx)<100&&abs(ry)<100)continue;
            point2.setX(scrCtX + rx - dx);
            point2.setY(scrCtY + ry - dy);
            point1.setX(point2.x()+5.0*sinA);
            point1.setY(point2.y()-5.0*cosA);
            point0.setX(point2.x()+18.0*sinA);
            point0.setY(point2.y()-18.0*cosA);
        }

        p->drawLine(point1,point2);
        /*if(theta%10==0)*/p->drawText(point0.x()-25,point0.y()-10,50,20,
                   Qt::AlignHCenter|Qt::AlignVCenter,
                   QString::number(theta));

    }

    //HDC dc = ui->tabWidget->getDC();
}
void Mainwindow::setScaleNM(unsigned short rangeNM)
{
    float oldScale = scale;
    scale = (float)height()/((float)rangeNM*CONST_NM)*0.7f;
    //printf("scale:%f- %d",scale,rangeNM);
    isScaleChanged = true;// scale*SIGNAL_RANGE_KM/2048.0f;

    dyMax = MAX_VIEW_RANGE_KM*scale;
    dxMax = dyMax;
    dx =short(scale/oldScale*dx);
    dy =short(scale/oldScale*dy);
    DrawMap();
    /*currMaxRange = (sqrtf(dx*dx+dy*dy)+scrCtY)/signsize;
    if(currMaxRange>RADAR_MAX_RESOLUTION)currMaxRange = RADAR_MAX_RESOLUTION;*/
    isScreenUp2Date = false;
}
void Mainwindow::UpdateRadarData()
{

    //SendCommandControl();
    if(!isSettingUp2Date)
    {
        ReloadSetting();
    }

    if(true)
    {
        updateTargets();
        update();
        //isScreenUp2Date = true;
    }




    /*QStandardItemModel* model = new QStandardItemModel(processing->radarData->mTrackList.size(), 5);
    for (int row = 0; row < processing->radarData->mTrackList.size(); ++row)
    {
       for (int column = 0; column < 5; ++column)
       {
           QString text = QString('A' + row) + QString::number(column + 1);
           QStandardItem* item = new QStandardItem(text);
           model->setItem(row, column, item);
       }
    }
    ui->tableTargetList->setModel(model);*/
}
void Mainwindow::updateData()
{
    //processing->ReadDataBuffer();
    updateTargets();
}
void Mainwindow::InitTimer()
{
    scrUpdateTimer = new QTimer();
    syncTimer1s = new QTimer();
    readBuffTimer = new QTimer();
    timer_lrad_control = new QTimer();
    videoTimer  = new QTimer();
    t = new QThread();
    t1 = new QThread();
//    connect(fullScreenTimer, SIGNAL(timeout()), this, SLOT(UpdateSetting()));
//    fullScreenTimer->setSingleShot(true);
//    fullScreenTimer->start(1000);
    connect(syncTimer1s, SIGNAL(timeout()), this, SLOT(sync1()));
    syncTimer1s->start(1000);
    syncTimer1s->moveToThread(t);
    //
    connect(readBuffTimer,SIGNAL(timeout()),this,SLOT(updateData()));
    readBuffTimer->start(200);
    readBuffTimer->moveToThread(t1);
    //
    connect(scrUpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateRadarData()));

    scrUpdateTimer->start(30);//ENVDEP
    scrUpdateTimer->moveToThread(t);
//    processing = new dataProcessingThread();
//    processing->start();
//    connect(this,SIGNAL(destroyed()),processing,SLOT(deleteLater()));
    connect(timer_lrad_control,SIGNAL(timeout()),this,SLOT(LradControl()));
    timer_lrad_control->start(200);
    //dataPlaybackTimer->moveToThread(t);
    connect(t,SIGNAL(finished()),t,SLOT(deleteLater()));

    t1->start(QThread::TimeCriticalPriority);

    connect(videoTimer, SIGNAL(timeout()), this, SLOT(ShowVideoCam()));
    //videoTimer->start(40);
    //videoTimer->moveToThread(t);
    t->start(QThread::TimeCriticalPriority);

}
bool controling = false;
float control_sensitive;
int ncount =0;
void Mainwindow::LradControl()
{


    if(tcpSender->state()==QAbstractSocket::ConnectedState)
    {
        if(g_IsTracking)//camera tracking
        {
            if(g_IsIR)control_sensitive = 0.6f;
            else
            {control_sensitive = 0.2f;}
            controling = true;
            char dx = ((g_Tracker.m_RectCurrent.right + g_Tracker.m_RectCurrent.left)/2 - 400)/8.0*control_sensitive;

            char dy = -((g_Tracker.m_RectCurrent.bottom + g_Tracker.m_RectCurrent.top)/2 - 300)/6.0*control_sensitive;
            unsigned char bytes[8];
            bytes[0] = 0x02;
            bytes[1] = 0x04;
            bytes[2] = 0x20;
            bytes[3] = dx;
            bytes[4] = dy;
            bytes[5] = 0x03;

            tcpSender->write((char*)&bytes[0],6);
            printf("data send \n");
        }
        else//camera traking off
        if(controling == true)
        {
            controling=false;
            unsigned char bytes[8];
            bytes[0] = 0x02;
            bytes[1] = 0x04;
            bytes[2] = 0x20;
            bytes[3] = 0;
            bytes[4] = 0;
            bytes[5] = 0x03;
            tcpSender->write((char*)&bytes[0],6);
            printf("data send \n");
        }
        else if(isNewTTM)// radar tracking
        {
            isNewTTM = false;
            for(short i = 0;i<targetList.size();i++)
            {
                if(selected_target_index == i)
                {
                    if(tcpSender->state()==QAbstractSocket::ConnectedState&&(ui->toolButton_radar_tracking->isChecked()))
                    {
                        short azi = (targetList.at(i)->azi-config.m_config.trueN)*100;
                        if(azi>18000)azi-=36000;
                        unsigned char bytes[8];
                        bytes[0] = 0x02;
                        bytes[1] = 0x06;
                        bytes[2] = 0x21;
                        bytes[3] = azi>>8;
                        bytes[4] = azi&0xff;
                        bytes[5] = 0x00;
                        bytes[6] = 0x00;
                        bytes[7] = 0x03;

                        tcpSender->write((char*)&bytes[0],8);
                        printf("radar track data\n");
                        //printf()
                    }else
                    {
                        printf("radar track data off \n");
                    }
                }
            }
        }
        else if(ui->toolButton_cursor_control->isChecked())
        {

            //ui->label_cursor_lat->setText(QString::number( y2lat(event->y())));
            //ui->label_cursor_lon->setText(QString::number( x2lon(event->x())));
            float x = (mousePointerX- scrCtX+dx)/scale/CONST_NM;
            float y = -(mousePointerY - scrCtY+dy)/scale/CONST_NM;
            float azi =  atanf(x/y);
            if(y<0)azi+=PI;
            azi = azi/PI*180;
            printf("\nazi:%f",azi);
            if(tcpSender->state()==QAbstractSocket::ConnectedState)
            {
                short kazi = (azi-config.m_config.trueN)*100;
                if(kazi>18000)kazi-=36000;
                if(kazi<-18000)kazi+=36000;
                unsigned char bytes[8];
                bytes[0] = 0x02;
                bytes[1] = 0x06;
                bytes[2] = 0x21;
                bytes[3] = kazi>>8;
                bytes[4] = kazi&0xff;
                bytes[5] = 0x00;
                bytes[6] = 0x00;
                bytes[7] = 0x03;
                tcpSender->write((char*)&bytes[0],8);
                printf("radar track data\n");
                //printf()
            }else
            {
                printf("radar track data off \n");
            }

        }

    }
    else
    {

        ui->label_connect_state->setText("LRAD: Not connected");
        if(tcpSender->state()!=QAbstractSocket::ConnectingState)
        {
            tcpSender->connectToHost("192.168.1.140",10100);
            printf("Reconnect \n");
        }
    }
    ncount++;
    if(ncount==25)
    {
        ncount=0;
        unsigned char bytes[8];
        bytes[0] = 0x02;
        bytes[1] = 0x02;
        bytes[2] = 0x5c;
        bytes[3] = 0x03;
        if(tcpSender->state()==QAbstractSocket::ConnectedState)
        {
            ui->label_connect_state->setText("LRAD: Connected");
            tcpSender->write((char*)&bytes[0],4);
        }
        else
        {
            ui->label_connect_state->setText("LRAD: Not connected");
        }

    }

}
void Mainwindow::InitNetwork()
{
    //connect(&playbackTimer, SIGNAL(timeout()), this, SLOT(drawSign()));

    //countdown = new CountdownReceiverDlg();

    //QHostAddress mHost = QHostAddress("224.110.210.226");
//    udpSocket = new QUdpSocket(this);
//    udpSocket->bind(5000, QUdpSocket::ShareAddress);
//    //udpSocket->joinMulticastGroup(mHost);
//    connect(udpSocket, SIGNAL(readyRead()),
//            this, SLOT(processFrame()));

        udpSendSocket = new QUdpSocket(this);
        if(!udpSendSocket->bind(8900))
        {
            if(!udpSendSocket->bind(8901))
            {
                udpSendSocket->bind(8902);
            }
        }
        udpSendSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 10);
        tcpSender = new QTcpSocket(this);
//        tcpSender->connectToHost(ui->textEdit_pt_port_2->text(),ui->textEdit_pt_port->text().toInt());tcpSender->connectToHost("192.168.1.140",10100);
        udpARPA = new QUdpSocket(this);
        udpARPA->bind(4444,QUdpSocket::ShareAddress);
        connect(udpARPA, SIGNAL(readyRead()),
                this, SLOT(processARPA()));
        //pcap

//        char errbuf[PCAP_ERRBUF_SIZE];
//        //
//        /* Retrieve the device list on the local machine */
//        if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
//        {

//            printf( errbuf); return;
//        }


//        d=alldevs;
//        if ( (adhandle= pcap_open(d->name,          // name of the device
//                                      65536,            // portion of the packet to capture
//                                                        // 65536 guarantees that the whole packet will be captured on all the link layers
//                                      PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
//                                      1000,             // read timeout
//                                      NULL,             // authentication on the remote machine
//                                      errbuf            // error buffer
//                                      ) ) == NULL)
//            {
//                /* Free the device list */
//                pcap_freealldevs(alldevs);
//                return ;
//            }
//        printf("\nlistening on %s...\n", d->name);

        /* start the capture */

}
void Mainwindow::processARPA()
{
    //return;
    while (udpARPA->hasPendingDatagrams())
    {

        QByteArray datagram;
        datagram.resize(udpARPA->pendingDatagramSize());
        udpARPA->readDatagram(datagram.data(), datagram.size());
        //printf(datagram.data());
        QString str(datagram.data());
        QStringList list = str.split(",");
        for(int i = 0;i<list.size();i++)
        {
           if(list.at(i).contains("$RATTM"))
           {
               if(i+5>list.length())break;
               QString tId = ((list.at(i+1)));
               float tRange = ((list.at(i+2))).toFloat();
               float tAzi = ((list.at(i+3))).toFloat();
               float speed = ((list.at(i+5))).toFloat();
               //arpa_data.addARPA(tNum,tDistance,tRange);
               float tX = tRange*cos(PI*tAzi/180.0)*CONST_NM;
               float tY = tRange*sin(PI*tAzi/180.0)*CONST_NM;
               float tLat = config.m_config.m_lat + tX/DEGREE_2_KM;
               float tLon = config.m_config.m_long + tY/DEGREE_2_KM;
               short j = 0;
               for(;j<targetList.size();j++)
               {
                   if(targetList.at(j)->id == tId)
                   {
                       targetList.at(j)->speed = speed;
                       targetList.at(j)->setCoordinates(tLat,tLon,tRange,tAzi); break;
                   }
               }
               if( j == targetList.size())
               {
                   CTarget*  tg1 = new CTarget(this);
                   tg1->show();
                   tg1->id = tId;
                   tg1->setCoordinates(tLat,tLon,tRange,tAzi);
                   targetList.append(tg1);
               }
               isScreenUp2Date = false;
               isNewTTM = true;

           }
        }
    }

}
void Mainwindow::processFrame()
{
//    while (udpSocket->hasPendingDatagrams()) {
//        unsigned short len = udpSocket->pendingDatagramSize();
//        QByteArray buff;
//        buff.resize(len);
//        udpSocket->readDatagram(buff.data(), len);
//        if((len==1422))//hr2d
//        {
//            ProcHR(&buff);
//        }
//    }
}




//void MainWindow::on_pauseButton_clicked()
//{
//    if(playbackTimer.isActive()){
//        playbackTimer.stop();
//        //ui->pauseButton->setText("Start");
//    }else
//    {
//        playbackTimer.start(10);
//        //ui->pauseButton->setText("Stop");
//    }
//}



//void MainWindow::on_comboBoxViewMode_currentIndexChanged(int index)
//{
//    viewMode=index;
//}
/*
void MainWindow::CameraControl(int x,int y, int zoom)
{
    char* sendBuff = new char[25];
    sprintf(sendBuff,"PTZSET %05d %05d %05d", x, y, zoom);
    udpSocket->writeDatagram(sendBuff,24,QHostAddress("127.0.0.1"),1989);
    delete[] sendBuff;
}
void MainWindow::CameraControl(int direction)
{
    char* sendBuff = new char[12];
    switch(direction)
    {
    case 1:

        sprintf(sendBuff,"PTZMOV IN  ");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);

        break;
    case 2:
        sprintf(sendBuff,"PTZMOV OUT ");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);
        break;
    case 3:
        sprintf(sendBuff,"PTZMOV LEFT");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);
        break;
    case 4:
        sprintf(sendBuff,"PTZMOV RGHT");
        udpSocket->writeDatagram(sendBuff,11,QHostAddress("127.0.0.1"),1989);
        break;
    default:
        break;
    }
    delete[] sendBuff;
}
*/


/*
void MainWindow::sendFrame(const char* hexdata,QHostAddress host,int port )
{
    short len = strlen(hexdata)/2+1;
    unsigned char* sendBuff = new unsigned char[len];
    hex2bin(hexdata,sendBuff);
    udpSendSocket->writeDatagram((char*)sendBuff,len-1,host,port);
    delete[] sendBuff;
}
*/
void Mainwindow::on_actionExit_triggered()
{

    cvReleaseCapture(&g_Capture);

    ExitProgram();
}
void Mainwindow::ExitProgram()
{
    config.SaveToFile();
    QApplication::quit();
#ifdef _WIN32
    //QProcess::startDetached("shutdown -s -f -t 0");
#else
    //system("/sbin/halt -p");
#endif
}

void Mainwindow::on_actionConnect_triggered()
{

}
void Mainwindow::sync1()//period 1 second
{
    updateTargets();
    // display radar temperature:
    //ui->label_temp->setText(QString::number(processing->radarData->tempType)+"|"+QString::number(processing->radarData->temp)+"\260C");
//    int n = 32*256.0f/((processing->radarData->noise_level[0]*256 + processing->radarData->noise_level[1]));
//    int m = 256.0f*((processing->radarData->noise_level[2]*256 + processing->radarData->noise_level[3]))
//            /((processing->radarData->noise_level[4]*256 + processing->radarData->noise_level[5]));
    //QByteArray array(processing->radarData->getFeedback(), 8);
    //ui->label_command->setText(QString(array.toHex()));
    //display target list:
    /*for(uint i=0;i<processing->radarData->mTrackList.size();i++)
    {
        if(processing->radarData->mTrackList[i].state==0)
        {
            QList<QListWidgetItem *> items = (ui->listTargetWidget->findItems(QString::number(i+1),Qt::MatchStartsWith));
            if(items.size())delete items[0];
            continue;
        }
        QList<QListWidgetItem *> items = (ui->listTargetWidget->findItems(QString::number(i+1),Qt::MatchStartsWith));
        QString str;
        float targetSpeed = processing->radarData->mTrackList[i].velocity*3600*signsize/scale/CONST_NM;//mile per hours
        // check track parameters

        if(targetSpeed>TARGET_MAX_SPEED)
        {
            //processing->radarData->deleteTrack(i);
            continue;
        }//
        if(processing->radarData->mTrackList[i].tclass==RED_OBJ)
        {
            str.append(QString::number(i+1)+":");
            str.append(QString::number(processing->radarData->mTrackList[i].estR*signsize/scale/CONST_NM,'g',3)+" | ");
            str.append(QString::number((short)(processing->radarData->mTrackList[i].estA*57.2957795f),'g',3)+" | ");
            str.append(QString::number((short)(targetSpeed),'g',4)+" | ");
            str.append(QString::number((short)(processing->radarData->mTrackList[i].course*57.2957795f),'g',3)+" | ");
            if(items.size())
            {
                (items[0])->setText(str);
            }else
            {
                ui->listTargetWidget->addItem(str);
            }
        }

    }*/
    //check if data too large
    /*if(processing->radarData->getDataOverload())
    {
        if(config.m_config.cfarThresh<30)config.m_config.cfarThresh++;
        ui->horizontalSlider_2->setValue(config.m_config.cfarThresh);
        isCfarThreshChanged = true;
    }*/
    //update cfar thresh value:

    //update signsize
    if(isScaleChanged ) {

        //processing->radarData->setScalePPI(scale);
        isScaleChanged = false;
    }
    //update signal code:

    //display time
    showTime();
    // require temperature

    //display radar state


}
void Mainwindow::setRadarState(radarSate radarState)
{

}

void Mainwindow::on_actionTx_On_triggered()
{
    //sendFrame("aaab030200000000", QHostAddress("192.168.0.44"),2573);
    //on_actionRotateStart_toggled(true);
//    Command_Control new_com;
//    new_com.bytes[0] = 0xaa;
//    new_com.bytes[1] = 0xab;
//    new_com.bytes[2] = 0x02;
//    new_com.bytes[3] = 0x01;
//    new_com.bytes[4] = 0x00;
//    new_com.bytes[5] = 0x00;
//    new_com.bytes[6] = 0x00;
//    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//    command_queue.push(new_com);
//    new_com.bytes[0] = 0xaa;
//    new_com.bytes[1] = 0xab;
//    new_com.bytes[2] = 0x00;
//    new_com.bytes[3] = 0x01;
//    new_com.bytes[4] = 0x00;
//    new_com.bytes[5] = 0x00;
//    new_com.bytes[6] = 0x00;
//    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//    command_queue.push(new_com);

}

void Mainwindow::on_actionTx_Off_triggered()
{
//    //on_actionRotateStart_toggled(false);
//    Command_Control new_com;
//    new_com.bytes[0] = 0xaa;
//    new_com.bytes[1] = 0xab;
//    new_com.bytes[2] = 0x00;
//    new_com.bytes[3] = 0x00;
//    new_com.bytes[4] = 0x00;
//    new_com.bytes[5] = 0x00;
//    new_com.bytes[6] = 0x00;
//    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//    command_queue.push(new_com);
//    new_com.bytes[0] = 0xaa;
//    new_com.bytes[1] = 0xab;
//    new_com.bytes[2] = 0x02;
//    new_com.bytes[3] = 0x00;
//    new_com.bytes[4] = 0x00;
//    new_com.bytes[5] = 0x00;
//    new_com.bytes[6] = 0x00;
//    new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//    command_queue.push(new_com);
}

void Mainwindow::on_actionRecording_toggled(bool arg1)
{
    if(arg1)
    {
        QDateTime now = QDateTime::currentDateTime();
        //processing->startRecord(now.toString("dd.MM-hh.mm.ss")+HR_FILE_EXTENSION);
    }
    else
    {        
        //processing->stopRecord();
    }
}

void Mainwindow::on_actionOpen_rec_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,    tr("Open signal file"), NULL, tr("HR signal record files (*.r2d)"));
    if(!filename.size())return;
    //processing->startReplay(filename);
}



void Mainwindow::on_actionOpen_map_triggered()
{
    //openShpFile();
}
void Mainwindow::showTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString text = time.toString("hh:mm:ss");
    ui->label_date->setText(text);
    text = time.toString("dd-MM-yy");
    ui->label_time->setText(text);
}

void Mainwindow::on_actionSaveMap_triggered()
{
    //vnmap.SaveBinFile();
}

void Mainwindow::on_actionSetting_triggered()
{
    GPSDialog *dlg = new GPSDialog(this);
    dlg->setModal(false);
    dlg->loadConfig(&config);
    dlg->show();
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    connect(dlg, SIGNAL(destroyed(QObject*)), SLOT(UpdateSetting()));
    connect(dlg, SIGNAL(destroyed(QObject*)), SLOT(setCodeType()));
}
void Mainwindow::on_actionAddTarget_toggled(bool arg1)
{
    //isAddingTarget=arg1;

}




void Mainwindow::on_actionClear_data_triggered()
{
    //processing->radarData->resetData();
    isScreenUp2Date = false;
}

void Mainwindow::on_actionView_grid_triggered(bool checked)
{
    gridOff = checked;
    dx=0;dy=0;
    DrawMap();
    //UpdateSetting();
}





/*
void MainWindow::on_pushButton_clicked()
{

    Command_Control new_com;
    hex2bin(ui->lineEdit_byte_1->text().toStdString().data(),&new_com.bytes[0]);
    hex2bin(ui->lineEdit_byte_2->text().toStdString().data(),&new_com.bytes[1]);
    hex2bin(ui->lineEdit_byte_3->text().toStdString().data(),&new_com.bytes[2]);
    hex2bin(ui->lineEdit_byte_4->text().toStdString().data(),&new_com.bytes[3]);
    hex2bin(ui->lineEdit_byte_5->text().toStdString().data(),&new_com.bytes[4]);
    hex2bin(ui->lineEdit_byte_6->text().toStdString().data(),&new_com.bytes[5]);
    hex2bin(ui->lineEdit_byte_7->text().toStdString().data(),&new_com.bytes[6]);
    hex2bin(ui->lineEdit_byte_8->text().toStdString().data(),&new_com.bytes[7]);
    command_queue.push(new_com);
}
*/

void Mainwindow::SendCommandControl()
{/*
      if(command_queue.size())
      {

          if(processing->radarData->checkFeedback(&command_queue.front().bytes[0]))// check if the radar has already recieved the command
          {


              command_queue.pop();
              udpFailure = 0;

          }
          else
          {
            if(udpFailure<20)//ENVDEP 20*50ms = 1s
            {udpFailure++;}
            else{
                setRadarState( DISCONNECTED);
                udpFailure = 0;
            }
            udpSendSocket->writeDatagram((char*)&command_queue.front().bytes[0],8,QHostAddress("192.168.0.44"),2572);
            //
            char xx[3];
            xx[2]=0;
            QString str;
            for(short i =0;i<8;i++)
            {
                bin2hex(command_queue.front().bytes[i],&xx[0]);
                str.append(xx);
                str.append('-');
            }

            ui->label_command->setText(str);
            //printf((const char*)str.data());
            //

          }

      }*/

}

void Mainwindow::on_actionRecording_triggered()
{

}


void Mainwindow::on_comboBox_temp_type_currentIndexChanged(int index)
{

 //!!!
}

//void RadarGui::on_horizontalSlider_brightness_actionTriggered(int action)
//{

//}



/*void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    switch (value) {
    case 1:
        Command_Control new_com;
        new_com.bytes[0] = 4;
        new_com.bytes[1] = 0xab;
        new_com.bytes[2] = 0;
        new_com.bytes[3] = 0;
        new_com.bytes[4] = 1;
        new_com.bytes[5] = 0;
        new_com.bytes[6] = 0;
        new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
        command_queue.push(new_com);
        break;
    case 2:
        printf("2");
        break;
    case 3:
        printf("3");
        break;
    default:
        break;
    }
}*/



//void MainWindow::on_toolButton_toggled(bool checked)
//{
//    //if(checked)ui->toolBar_Main->show();
//    //else ui->toolBar_Main->hide();
//}

void Mainwindow::on_actionSector_Select_triggered()
{

}


//void MainWindow::on_toolButton_10_clicked()
//{
//    //if(ui->frame_RadarViewOptions->isHidden())ui->frame_RadarViewOptions->show();
//    //else ui->frame_RadarViewOptions->hide();
//}




/*
void MainWindow::on_toolButton_14_clicked()
{
    //if(event->delta()>0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
}

void MainWindow::on_toolButton_13_clicked()
{
    //if(event->delta()<0)ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
}
*/
void Mainwindow::UpdateScale()
{
    //float oldScale = scale;
    switch(range)

    {
    case 0:
        scale = (height()/2-5)/(CONST_NM*0.5 );
        ui->label_range->setText("0.5 NM");
        break;
    case 1:
        scale = (height()/2-5)/(CONST_NM );
        ui->label_range->setText("1 NM");
        break;
    case 2:
        scale = (height()/2-5)/(CONST_NM*2 );
        ui->label_range->setText("2 NM");
        break;
    case 3:
        scale = (height()/2-5)/(CONST_NM*4 );
        ui->label_range->setText("4 NM");
        break;
    case 4:
        scale = (height()/2-5)/(CONST_NM*8 );
        ui->label_range->setText("8 NM");
        break;
    case 5:
        scale = (height()/2-5)/(CONST_NM*16 );
        ui->label_range->setText("16 NM");
        break;
    case 6:
        scale = (height()/2-5)/(CONST_NM*24 );
        ui->label_range->setText("24 NM");
        break;
    case 7:
        scale = (height()/2-5)/(CONST_NM*48 );
        ui->label_range->setText("48 NM");
        break;

    default:
        scale = (height()/2-5)/(90 );
        ui->label_range->setText("48 NM");
        break;
    }
    isScaleChanged = true;
    isScreenUp2Date = false;

    //dx /=short(scale/oldScale);
    //dy /=short(scale/oldScale);
}




//void MainWindow::on_toolButton_10_toggled(bool checked)
//{

//}

//void MainWindow::on_actionRotateStart_toggled(bool arg1)
//{
//    if(arg1)
//    {
//        Command_Control new_com;
//        new_com.bytes[0] = 0xaa;
//        new_com.bytes[1] = 0xab;
//        new_com.bytes[2] = 0x03;
//        new_com.bytes[3] = 0x02;
//        new_com.bytes[4] = 0x00;
//        new_com.bytes[5] = 0x00;
//        new_com.bytes[6] = 0x00;
//        new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//        command_queue.push(new_com);
//    }
//    else
//    {

//        Command_Control new_com;
//        new_com.bytes[0] = 0xaa;
//        new_com.bytes[1] = 0xab;
//        new_com.bytes[2] = 0x03;
//        new_com.bytes[3] = 0x00;
//        new_com.bytes[4] = 0x00;
//        new_com.bytes[5] = 0x00;
//        new_com.bytes[6] = 0x00;
//        new_com.bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
//        command_queue.push(new_com);
//    }
//}


//void MainWindow::on_comboBox_temp_type_2_currentIndexChanged(int index)
//{



//}

//void MainWindow::on_toolButton_11_toggled(bool checked)
//{


//}

//void MainWindow::on_pushButton_removeTarget_2_clicked()
//{

//}

//void MainWindow::on_pushButton_removeTarget_2_released()
//{
//    processing->radarData->resetTrack();
//}

//void MainWindow::on_pushButton_avtodetect_toggled(bool checked)
//{
//    isDrawSubTg = !checked;
//    processing->radarData->avtodetect = checked;
//    processing->radarData->terrain_init_time = 3;
//}


void Mainwindow::setCodeType(short index)// chuyen ma
{
    unsigned char bytes[8];
    bytes[0] = 1;
    bytes[1] = 0xab;

    //printf("\n code:%d",index);
    switch (index)
    {
    case 0://M32
        bytes[2] = 2;
        bytes[3] = 0;
        break;
    case 1://M64
        bytes[2] = 2;
        bytes[3] = 1;
        break;
    case 2://M128
        bytes[2] = 2;
        bytes[3] = 2;
        break;
    case 3://M255
        bytes[2] = 2;
        bytes[3] = 3;
        break;
    case 4://M32x2
        bytes[2] = 2;
        bytes[3] = 4;
        break;
    case 5://M64x2
        bytes[2] = 2;
        bytes[3] = 5;
        break;
    case 6://M128x2
        bytes[2] = 2;
        bytes[3] = 6;
        break;
    case 7://baker
        bytes[2] = 1;
        bytes[3] = 1;
        break;
    case 8://single pulse
        bytes[2] = 0;
        bytes[3] = 1;

        break;
    default:
        bytes[2] = 0;
        bytes[3] = 0;
        break;
    }
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;//new_com.bytes[0]+new_com.bytes[1]+new_com.bytes[2]+new_com.bytes[3]+new_com.bytes[4]+new_com.bytes[5]+new_com.bytes[6];
    sendToRadar(&bytes[0]);

}
//void MainWindow::on_toolButton_4_toggled(bool checked)
//{
//    if(checked)
//    {
//        this->on_actionTx_On_triggered();
//    }
//    else
//    {
//        this->on_actionTx_Off_triggered();
//    }

//}

void Mainwindow::on_toolButton_exit_clicked()
{
    cvReleaseCapture(&g_Capture);
    g_Capture = NULL;
    close();
    qApp->quit();
    on_actionExit_triggered();
}

//void Mainwindow::on_toolButton_setting_clicked()
//{
//    this->on_actionSetting_triggered();
//}





void Mainwindow::on_toolButton_centerView_clicked()
{
    dx = 0;
    dy = 0;
    DrawMap();
    isScreenUp2Date = false;
}

void Mainwindow::on_comboBox_currentIndexChanged(int index)
{


}

void Mainwindow::on_comboBox_img_mode_currentIndexChanged(int index)
{
//    processing->radarData->imgMode = imgDrawMode(index) ;
}


void Mainwindow::on_toolButton_send_command_clicked()
{/*
    unsigned char        bytes[8];
    hex2bin(ui->lineEdit_byte_1->text().toStdString().data(),&bytes[0]);
    hex2bin(ui->lineEdit_byte_2->text().toStdString().data(),&bytes[1]);
    hex2bin(ui->lineEdit_byte_3->text().toStdString().data(),&bytes[2]);
    hex2bin(ui->lineEdit_byte_4->text().toStdString().data(),&bytes[3]);
    hex2bin(ui->lineEdit_byte_5->text().toStdString().data(),&bytes[4]);
    hex2bin(ui->lineEdit_byte_6->text().toStdString().data(),&bytes[5]);
    hex2bin(ui->lineEdit_byte_7->text().toStdString().data(),&bytes[6]);
    hex2bin(ui->lineEdit_byte_8->text().toStdString().data(),&bytes[7]);
    udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);*/
}

void Mainwindow::on_toolButton_map_toggled(bool checked)
{
    DrawMap();
}

void Mainwindow::on_toolButton_zoom_in_clicked()
{
    isScreenUp2Date=false;
    if(range<7)range++;

    UpdateScale();
    DrawMap();
}

void Mainwindow::on_toolButton_zoom_out_clicked()
{
    isScreenUp2Date=false;
    if(range>0)range--;

    UpdateScale();
    DrawMap();
}

void Mainwindow::on_toolButton_reset_clicked()
{
//    processing->radarData->resetData();
}

void Mainwindow::on_toolButton_send_command_2_clicked()
{
    unsigned char        bytes[8] = {0xaa,0xab,0x02,0x02,0x0a,0,0,0};
    udpSendSocket->writeDatagram((char*)&bytes[0],8,QHostAddress("192.168.0.44"),2572);
//    bytes[0] = 0xaa;
//    bytes[1] = 0xab;
//    bytes[2] = 0x02;
//    bytes[3] = 0x02;
//    bytes[4] = 0x0a;
//    bytes[5] = 0x00;
//    bytes[6] = 0x00;
//    bytes[7] = 0x00;

}

void Mainwindow::SetGPS(float mlat,float mlong)
{
    config.m_config.m_lat = mlat;
    config.m_config.m_long = mlong;
    ui->text_latInput_2->setText(QString::number(mlat));
    ui->text_longInput_2->setText(QString::number(mlong));
    vnmap.setUp(config.m_config.m_lat, config.m_config.m_long, 100,config.m_config.mapFilename.data());
    DrawMap();
    update();
}
void Mainwindow::on_toolButton_map_select_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,    QString::fromUtf8("Open Map"), NULL, tr("ISM file (*.ism)"));
    if(!filename.size())return;
    config.m_config.mapFilename =  filename.toStdString();
    vnmap.ClearData();
    if(pMap)delete pMap;
    pMap = new QPixmap(height(),height());
    vnmap.setUp(config.m_config.m_lat, config.m_config.m_long, 200,config.m_config.mapFilename.data());//100km  max range
    DrawMap();
}

void Mainwindow::on_dial_valueChanged(int value)
{
    float heading = value/100.0f;
    ui->textEdit_heading->setText(QString::number(heading));

}

void Mainwindow::on_toolButton_set_heading_clicked()
{
    float heading = ui->textEdit_heading->text().toFloat();
    config.m_config.trueN = heading;
//    processing->radarData->setTrueN(config.m_config.trueN);
}

void Mainwindow::on_toolButton_gps_update_clicked()
{
    SetGPS(ui->text_latInput_2->text().toFloat(),ui->text_longInput_2->text().toFloat());
}


void Mainwindow::on_toolButton_centerZoom_clicked()
{
//    processing->radarData->updateZoomRect(mousePointerX - scrCtX+dx,mousePointerY - scrCtY+dy);
}

uchar *qImageBuffer = NULL;

QImage *IplImageToQImage(const IplImage * iplImage, uchar **data, double mini, double maxi)
{

    int width = iplImage->width;
    int widthStep = iplImage->widthStep;
    int height = iplImage->height;
    QImage *qImage;
    switch (iplImage->depth)
    {
        case IPL_DEPTH_8U:
        if (iplImage->nChannels == 1)
        {
        // OpenCV image is stored with one byte grey pixel. We convert it
       // to an 8 bit depth QImage.
        qImage = new QImage(width,height,QImage::Format_Indexed8);
        uchar *QImagePtr = qImage->scanLine(0);
        qImageBuffer = qImage->scanLine(0);

        const uchar *iplImagePtr = (const uchar *) iplImage->imageData;

        for (int y = 0; y < height; y++)
        {
            // Copy line by line
            QImagePtr = qImage->scanLine(y);
            memcpy(QImagePtr, iplImagePtr, width);
            iplImagePtr += widthStep;

        }
        /*
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                // We take only the highest part of the 16 bit value. It is
                //similar to dividing by 256.
                //*QImagePtr++ = ((*iplImagePtr++) >> 8);
                *QImagePtr = *iplImagePtr;
                QImagePtr++;
                iplImagePtr++;
                }

                iplImagePtr += widthStep/sizeof(uchar)-width;
            }*/
        }
        else if (iplImage->nChannels == 3)
            {
            /* OpenCV image is stored with 3 byte color pixels (3 channels).
            We convert it to a 32 bit depth QImage.
            */
            qImageBuffer = (uchar *) malloc(width*height*4*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const uchar *iplImagePtr = (const uchar *) iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // We cannot help but copy manually.
                    QImagePtr[0] = iplImagePtr[0];
                    QImagePtr[1] = iplImagePtr[1];
                    QImagePtr[2] = iplImagePtr[2];
                    QImagePtr[3] = 0;

                    QImagePtr += 4;
                    iplImagePtr += 3;
                }
            iplImagePtr += widthStep-3*width;
            }

        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=8U and %d channels\n", iplImage->nChannels);
        }
        break;
        case IPL_DEPTH_16U:
        if (iplImage->nChannels == 1)
        {
        /* OpenCV image is stored with 2 bytes grey pixel. We convert it
        to an 8 bit depth QImage.
        */
            qImage = new QImage(width,height,QImage::Format_Indexed8);
            uchar *QImagePtr = qImage->scanLine(0);
            qImageBuffer = qImage->scanLine(0);

            //const uint16_t *iplImagePtr = (const uint16_t *);
            const unsigned short *iplImagePtr = (const unsigned short *)iplImage->imageData;


            for (int y = 0; y < height; y++)
            {
                QImagePtr = qImage->scanLine(y);
                for (int x = 0; x < width; x++)
                {
                // We take only the highest part of the 16 bit value. It is
                //similar to dividing by 256.
                //*QImagePtr++ = ((*iplImagePtr++) >> 8);
                //change here 16 bit could be everything !! set max min to your desire
                *QImagePtr = 255*(((*iplImagePtr) - mini) / (maxi - mini));
                QImagePtr++;
                iplImagePtr++;
                }

                iplImagePtr += widthStep/sizeof(unsigned short)-width;
            }

        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=16U and %d channels\n", iplImage->nChannels);

        }
        break;
        case IPL_DEPTH_32F:
         if (iplImage->nChannels == 1)
         {
        /* OpenCV image is stored with float (4 bytes) grey pixel. We
        convert it to an 8 bit depth QImage.
        */
             qImage = new QImage(width,height,QImage::Format_Indexed8);
             uchar *QImagePtr = qImage->scanLine(0);
             qImageBuffer = qImage->scanLine(0);

             const float *iplImagePtr = (const float *) iplImage->imageData;
             for (int y = 0; y < height; y++)
             {
             QImagePtr = qImage->scanLine(y);
                 for (int x = 0; x < width; x++)
                 {
                     uchar p;
                     float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
                     if (pf < 0) p = 0;
                     else if (pf > 255) p = 255;
                     else p = (uchar) pf;

                     *QImagePtr++ = p;
                  }

             iplImagePtr += widthStep/sizeof(float)-width;
             }
         }
         else
         {
             qDebug("IplImageToQImage: image format is not supported : depth=32F and %d channels\n", iplImage->nChannels);
         }
       break;
       case IPL_DEPTH_64F:
         if (iplImage->nChannels == 1)
         {
            /* OpenCV image is stored with double (8 bytes) grey pixel. We
            convert it to an 8 bit depth QImage.
            */
             qImage = new QImage(width,height,QImage::Format_Indexed8);
             uchar *QImagePtr = qImage->scanLine(0);
             qImageBuffer = qImage->scanLine(0);

            const double *iplImagePtr = (const double *) iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
             QImagePtr = qImage->scanLine(y);
                for (int x = 0; x < width; x++)
                {
                    uchar p;
                    double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);

                    if (pf < 0) p = 0;
                    else if (pf > 255) p = 255;
                    else p = (uchar) pf;

                    *QImagePtr++ = p;
                }

            }

        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=64F and %d channels\n", iplImage->nChannels);
        }
        break;
        default:
        qDebug("IplImageToQImage: image format is not supported : depth=%d and %d channels\n", iplImage->depth, iplImage->nChannels);
    }

    QVector<QRgb> vcolorTable;
    if (iplImage->nChannels == 1)
    {
        // We should check who is going to destroy this allocation.
        vcolorTable.resize(256);
        for (int i = 0; i < 256; i++)
        {
           vcolorTable[i] = qRgb(i, i, i);
        }
        //Qt vector is difficult to use... start with std to qvector
        //here I allocate QImage using qt constructor (Forma_Indexed8 adds sometimes 2 bytes on the right side !!! o.O not specified nowhere !!!)
        //qImage = new QImage(tmpImg->scanLine(0), width, height, QImage::Format_Indexed8);
        qImage->setColorTable(vcolorTable);
    }
    else
    {
        //if(qImage)delete qImage;
        qImage = new QImage(qImageBuffer, width, height, QImage::Format_RGB32);

    }
    //*data = qImageBuffer;
    return qImage;
}

void Mainwindow::ShowVideoCam()
{
    if (g_Capture ==  NULL)
    {

        if (g_IsIR)
            g_Capture = cvCaptureFromFile("rtsp://192.168.1.140:1554/ch0");
//            g_Capture = cvCaptureFromCAM(0);
        else
            g_Capture = cvCaptureFromFile("rtsp://192.168.1.140:554/axis-media/media.amp");
//            g_Capture = cvCaptureFromCAM(1);
    }

    if (!g_Capture) // Capture fail
    {
        onConnectVideo = false;
        ui->toolButton_video_connect->setChecked(false);
        ui->toolButton_video_connect->setText("Connect");
        videoTimer->stop();
        return;
    }


    if (g_Frame == NULL)
        g_Frame = cvCreateImage(cvSize(800, 600), 8, 3);

    g_TrueFrame = cvQueryFrame(g_Capture);
    //cv::imshow("img",g_TrueFrame->);
    if (!g_TrueFrame)
        return;

     cvResize(g_TrueFrame, g_Frame);

     if (g_IsTracking) // if tracking opt is ON
     {
         cvResize(g_Frame, g_FrameHalf, CV_INTER_LINEAR); // g_FrameHalf is initial when StartTracking function is called
         g_Tracker.TrackNextFrame(g_FrameHalf, g_Tracker.gRectCurrentHalf, &g_Tracker.m_TrkResult);
         g_Tracker.gRectCurrentHalf = g_Tracker.m_TrkResult.targetBox;
         utl_RectCheckBound(&g_Tracker.gRectCurrentHalf, g_Tracker.m_ImageMaxX/2, g_Tracker.m_ImageMaxY/2);

         //update full size RECT for display
         g_Tracker.m_RectCurrent.left	= g_Tracker.gRectCurrentHalf.left*2;
         g_Tracker.m_RectCurrent.top	= g_Tracker.gRectCurrentHalf.top*2;
         g_Tracker.m_RectCurrent.right	= g_Tracker.gRectCurrentHalf.right*2;
         g_Tracker.m_RectCurrent.bottom	= g_Tracker.gRectCurrentHalf.bottom*2;
         DrawTrackingRgn(g_Frame, g_Tracker.m_RectCurrent);
     }

     if(qImageBuffer)
         delete qImageBuffer;
     img = IplImageToQImage(g_Frame,NULL,0,255);
     //cvShowImage("test",g_Frame);
     update();

}




void Mainwindow::StartTracking(RECT inputRECT)
{
    if (!g_Frame)
        return;

    g_Tracker.InitForFirstFrame1(g_Frame, inputRECT);
    g_FrameHalf = cvCreateImage(cvSize(g_Tracker.m_ImageMaxX/2, g_Tracker.m_ImageMaxY/2), 8, 3);
    g_IsTracking = true;
    
}

void Mainwindow::OnVideoConnect(bool checked)
{
    onConnectVideo = checked;
    if(checked)
    {
        //ui->toolButton_video_connect->setText("Disconnect");

            videoTimer->stop();
            cvReleaseCapture(&g_Capture);
            g_Capture = NULL;
            g_IsTracking = false;
            // change url string for capture video
            //......
            if(g_IsIR)videoTimer->start(10);
            else videoTimer->start(20);
    }
    else
    {
       // ui->toolButton_video_connect->setText("Connect");
        videoTimer->stop();
        cvReleaseCapture(&g_Capture);
        g_Capture = NULL;
        g_IsTracking = false;

    }

}

void Mainwindow::on_toolButton_video_connect_toggled(bool checked)
{
    //pcap_loop(adhandle, 0, packet_handler, NULL);
    OnVideoConnect(checked);
    if(checked)
    {
        ui->toolButton_video_connect->setText("Disconnect");
    }else
    {
         ui->toolButton_video_connect->setText("Connect");
    }
}

void Mainwindow::on_toolButton_video_connect_2_toggled(bool checked)
{
    OnVideoConnect(checked);
}

void Mainwindow::on_toolButton_ir_toggled(bool checked)
{
    g_IsIR = checked;
    if(checked)
    {
        ui->toolButton_ir->setText("Daylight Cam");
    }else
    {
         ui->toolButton_ir->setText("Night Cam");
    }
    if (!onConnectVideo)
    {
        ui->toolButton_video_connect->setChecked(false);
        return;
    }
    videoTimer->stop();
    cvReleaseCapture(&g_Capture);
    g_Capture = NULL;
    g_IsTracking = false;
    // change url string for capture video
    //......
    if(g_IsIR)videoTimer->start(10);
    else videoTimer->start(20);
    ui->toolButton_video_connect->setChecked(true);
    return;
}

void Mainwindow::on_toolButton_video_connect_clicked()
{

}

void Mainwindow::on_toolButton_radar_tracking_toggled(bool checked)
{
    if(checked)
    {
        ui->toolButton_cursor_control->setChecked(false);
    }
}

void Mainwindow::on_toolButton_cursor_control_toggled(bool checked)
{
    if(checked)
    {
        ui->toolButton_radar_tracking->setChecked(false);
    }
}

void Mainwindow::on_toolButton_clear_target_clicked()
{
    for(short i = 0;i<targetList.size();i++)
    {

            targetList.at(i)->deleteLater();
    }
    targetList.clear();
}

void Mainwindow::on_toolButton_grid_clicked()
{
    isScreenUp2Date=false;
}
