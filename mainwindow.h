#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define STR_RADAR_NOT_CONNECTED QString::fromUtf8("Chưa kết nối")
#define STR_RADAR_CONNECTED     QString::fromUtf8("Đã kết nối")
#define STR_RADAR_TRANSMITING   QString::fromUtf8("Đang phát")
#define SCR_X_MIN_LIMIT   60
#define HR_FILE_EXTENSION ".r2d"
#define HAVE_REMOTE
//#include <pcap.h>
#include <QtNetwork>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <vnmap.h>
//#include <CpView.h>
#include <Config.h>
#include <CLocal.h>
#include <gpsdialog.h>
//#include <dataprocessingthread.h>
#include <onexitdialog.h>
//#include <C_radar_data.h>
#include <QFileDialog>
#include <QImage>
#include <QTcpSocket>
#include <QHostAddress>
#include "ctarget.h"
#include "Tracker.h"
#include "CTrack.h"
#include "CLocal.h"
//#include "radarcontroldialog.h"
//#include "c_arpa_data.h"
//#include <QtSerialPort/QSerialPort>
QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE
namespace Ui {
class MainWindow;
//class QLabel;
//class QPushButton;
//class QUdpSocket;
}
class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mainwindow(QWidget *parent = 0);
    ~Mainwindow();
protected:
    //void contextMenuEvent(QContextMenuEvent *event);
//    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    enum radarSate   { DISCONNECTED,CONNECTED,CONNECTED_ROTATE9_TXOFF,CONNECTED_ROTATE12_TXOFF, CONNECTED_ROTATE9_TXON,CONNECTED_ROTATE12_TXON } radar_state;
private:
    bool ProcDataAIS(BYTE *szBuff, int nLeng );
    //void createActions();
    //void createMenus();
    void DrawGrid(QPainter* p,short centerX,short centerY);
    void CameraControl(int x,int y, int zoom);
    void CameraControl(int direction);
    void InitSetting();
    void updateTargets();
//    bool ProcDataAIS(BYTE *szBuff, int nLeng );
    void sendToRadar(const char *hexdata);
    void sendToRadar(unsigned char* hexdata);
//    void SetSnScale(short value);
    Ui::MainWindow* ui;
//    QMenu   *       m_fileMenu;
//    QMenu   *       m_connectionMenu;
//    QAction *       a_openShp;
//    QAction *       a_openPlace;
//    QAction *       a_savePlace;
//    QAction *       a_gpsOption;
//    QAction *       a_openSignal;

    //network
    //QUdpSocket      *udpSocket;//raymarine
    QUdpSocket      *udpARPA;//ARPA
    QUdpSocket      *udpSendSocket;//radar control
    QTcpSocket      *tcpSender;
    //
    //CConfig         m_config;
    //CpView  *       m_view;
    //float           scale;
    QTimer          *drawTimer;
    QPoint          view_pos;

<<<<<<< HEAD
=======
    C2_TrackLst     m_trackList;
    C2_Local        m_CLocal;
    bool ProcDataAIS(BYTE *szBuff, int nLeng);

>>>>>>> origin/master
    bool LoadISMapFile();
    void SaveBinFile();
    void InitNetwork();
    //void sendFrame(const char* hexdata,QHostAddress host,int port );
    void InitTimer();
    void showTime();
    void DrawViewFrame(QPainter *p);
    void DrawSignal(QPainter *p);
    void drawAisTarget(QPainter *p);
//    void DrawTarget(QPainter* p);
    void DrawMap();
    void ReloadSetting();
    void SendCommandControl();
    void SetGPS(float mlat,float mlong);
    void StartTracking(RECT inputRECT);

    C2_Local          m_CLocal;
    C2_TrackLst m_trackList;
public slots:
    void UpdateSetting();
//    void UpdateSignScale();
    void UpdateScale();
    void setCodeType(short index);
private:
    void setRadarState(radarSate radarState);
public:

    void setScaleNM(unsigned short rangeNM);
private slots:
    void LradControl();
    void ShowVideoCam();
    void updateData();
    void sync1();
    void ExitProgram();
    void gpsOption();
    void processFrame();
    void processARPA();
    void on_actionExit_triggered();
    void UpdateRadarData();
    void PlaybackRecFile();
    void on_actionConnect_triggered();
    void on_actionTx_On_triggered();
    void on_actionTx_Off_triggered();
    void on_actionRecording_toggled(bool arg1);
    void on_actionOpen_rec_file_triggered();
    void on_actionOpen_map_triggered();
    void on_actionSaveMap_triggered();


    void on_actionSetting_triggered();

    void on_actionAddTarget_toggled(bool arg1);

    void on_actionClear_data_triggered();

    void on_actionView_grid_triggered(bool checked);


//    void on_actionPlayPause_toggled(bool arg1);

    void on_actionRecording_triggered();

    void on_comboBox_temp_type_currentIndexChanged(int index);

//    void on_horizontalSlider_brightness_actionTriggered(int action);

//    void on_horizontalSlider_brightness_valueChanged(int value);

//    void on_horizontalSlider_signal_scale_valueChanged(int value);

    void on_actionSector_Select_triggered();

    //void on_toolButton_13_clicked();

    //void on_toolButton_14_clicked();

//    void on_actionRotateStart_toggled(bool arg1);


    void on_toolButton_exit_clicked();

    //void on_toolButton_setting_clicked();

    //void on_toolButton_replay_2_clicked();

    void on_toolButton_centerView_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_img_mode_currentIndexChanged(int index);


    void on_toolButton_send_command_clicked();

    void on_toolButton_map_toggled(bool checked);

    void on_toolButton_zoom_in_clicked();

    void on_toolButton_zoom_out_clicked();

    void on_toolButton_reset_clicked();


    void on_toolButton_send_command_2_clicked();


    void on_toolButton_map_select_clicked();

    void on_dial_valueChanged(int value);

    void on_toolButton_set_heading_clicked();

    void on_toolButton_gps_update_clicked();

    //void on_comboBox_code_type_currentIndexChanged(const QString &arg1);

    //void on_comboBox_code_type_currentIndexChanged(int index);

    void on_toolButton_centerZoom_clicked();

    //void on_toolButton_xl_dopler_clicked();

    //void on_toolButton_xl_dopler_toggled(bool checked);

    //void on_toolButton_xl_nguong_2_toggled(bool checked);

    //void on_tabWidget_2_currentChanged(int index);

    void OnVideoConnect(bool checked);

    void on_toolButton_video_connect_toggled(bool checked);

    void on_toolButton_video_connect_2_toggled(bool checked);

    void on_toolButton_ir_toggled(bool checked);

    void on_toolButton_video_connect_clicked();

    void on_toolButton_radar_tracking_toggled(bool checked);

    void on_toolButton_cursor_control_toggled(bool checked);

    void on_toolButton_clear_target_clicked();

    void on_toolButton_grid_clicked();

private:
    void initActionsConnections();
};

#endif // MAINWINDOW_H
