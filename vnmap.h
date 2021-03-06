#ifndef Q_VNMAP_H
#define Q_VNMAP_H

#define N_LAYER 6
/*
  map definition
  Layer 0 :Land area
  Layer 1 :Urban object
  Layer 2 :Lakes and water reservoirs
  Layer 3 :Rivers
  Layer 4 :landLines 1(road)
  Layer 5 :landLines 2(reserved)
  */
#include <vector>
#include <string>

#include <cstdio>
#include <cstring>
//#include "shapelib\shapefil.h"
using namespace std;
typedef struct {
    float m_Lat;
    float m_Long;
} Point_t;
typedef struct {
    float m_Lat;
    float m_Long;
    std::wstring text;
} Place_t;

typedef std::vector<Place_t> PlaceList;
typedef std::vector<Point_t> C2_Poly;
typedef std::vector<C2_Poly> C2_PolyList;


class Q_vnmap
{
public:
    Q_vnmap();
    ~Q_vnmap();
   // float scale;
    //int mouseX,mouseY;
    C2_PolyList		layers[N_LAYER];
    //CC_MapVN pCC_MapVN;
    PlaceList placeList;
    //int dx,dy;
    //void OpenShpFile(const char* filename, short layerIndex);
    void setUp(float ctLat, float ctLong,unsigned short size,const char* fileName);
    void ConvDegToScr(float *x, float *y, float *m_Long, float *m_Lat);
    void ConvScrToDeg(float* x, float *y, float *m_Long, float *m_Lat);
    void LoadPlaces(const char* fileName);
    void LoadBinFile(const char* fileName);
    void ClearData();
private:
    bool checkInside(Point_t  *point);
    void getElement(short nlayer,short npoly,short npoint,Point_t* point);

    Point_t currPos,ulPos,brPos;
};

#endif // Q_VNMAP_H
