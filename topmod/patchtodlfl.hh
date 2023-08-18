#ifndef PATCHTODLFL_HH
#define PATCHTODLFL_HH

#include "Vector3d.hh"
#include <QVector>
#include <QHash>
#include <QString>
//#include "TMPatch.hh"
#include <qdebug.h>

#define STRING(value) QString::number(value, 'g', 8)
class TMPatch;

class Point3d{
public:
    static constexpr double const POSITIVE_EPSILON = 0.00001;
    static constexpr double const NEGATIVE_EPSILON = -0.00001;
    static constexpr uint const FACTOR = 100000;
    Point3d(){
        elems[0]=elems[1]=elems[2] = 0.0;
        id = 0;
    }
    Point3d(const Point3d &pnt){
        elems[0]=pnt.elems[0];
        elems[1]=pnt.elems[1];
        elems[2]=pnt.elems[2];
        id = pnt.id;
    }
    Point3d(const double &x,
            const double &y,
            const double &z){
        elems[0]=x;
        elems[1]=y;
        elems[2]=z;
        id=0;
    }
    uint id;
    double& operator [] (uint index)
      {
        return elems[index];
      }

    double operator [] (uint index) const
      {
        return elems[index];
      }
    Vector3d toVector3d(){
        return Vector3d(elems[0],elems[1], elems[2]);
    }
    void print(){
        qDebug() << "x: " << elems[0] << "; y: " << elems[1] << "; z: " << elems[2];
    }

private:
    double elems[3];

};
inline bool operator==(const Point3d &p1, const Point3d &p2)
{
    double _x = p1[0]-p2[0];
    if (_x>Point3d::POSITIVE_EPSILON || _x<Point3d::NEGATIVE_EPSILON) {
        return false;
    }
    double _y = p1[1]-p2[1];
    if (_y>Point3d::POSITIVE_EPSILON || _y<Point3d::NEGATIVE_EPSILON) {
        return false;
    }
    double _z = p1[2]-p2[2];
    if (_z>Point3d::POSITIVE_EPSILON || _z<Point3d::NEGATIVE_EPSILON) {

        return false;
    }
    return true;
}

inline uint qHash(const Point3d &p1, uint seed = Point3d::FACTOR)
{
    uint hash = 7;
    hash = 71 * hash + (uint)(p1[0]*seed);
    hash = 71 * hash + (uint)(p1[1]*seed);
    hash = 71 * hash + (uint)(p1[2]*seed);
    return hash;
}

struct point3d_comparer {
  bool operator() (Point3d p1,Point3d p2) { return (p1.id<p2.id);}
};

class QuadFace{
public:
    QuadFace(uint index0, uint index1, uint index2, uint index3){
        indices[0] = index0;
        indices[1] = index1;
        indices[2] = index2;
        indices[3] = index3;
    }
    QuadFace(const QuadFace &quadface){
        indices[0] = quadface.indices[0];
        indices[1] = quadface.indices[1];
        indices[2] = quadface.indices[2];
        indices[3] = quadface.indices[3];
    }
    QuadFace& operator = (const QuadFace& quadface){
        indices[0] = quadface.indices[0];
        indices[1] = quadface.indices[1];
        indices[2] = quadface.indices[2];
        indices[3] = quadface.indices[3];
    }
private:
    uint indices[4];
public:
    uint& operator [] (uint index)
      {
        return indices[index];
      }

    uint operator [] (uint index) const
      {
        return indices[index];
      }
};

struct patch_data{
    Vector3d * vertices;
    uint *face_indices;
    int vertex_count, face_count;
    void release_memory(){
        delete[] vertices;
        delete[] face_indices;
    }
};

class FourUValues{
private:
    double values[4];
public:
    FourUValues(){}
    FourUValues(const double &u){
        values[0]=u*u*u;
        values[1]=u*u*(1-u);
        values[2]=u*(1-u)*(1-u);
        values[3]=(1-u)*(1-u)*(1-u);
    }
    FourUValues(const FourUValues &other){
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
        values[3] = other.values[3];
    }
    FourUValues& operator = (const FourUValues& other){
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
        values[3] = other.values[3];
    }
    //element access only
    double& operator [] (uint index){
        return values[index];
    }
    void print()const{
        qDebug()<<STRING(values[0])<<"\t"<<STRING(values[1])<<"\t"<<STRING(values[2])<<"\t"<<STRING(values[3]);
    }
};

typedef FourUValues doubleArray;
typedef QVector<doubleArray> doubleArrayArray;
typedef QVector<Point3d> arrayOfPoints;
typedef QVector<arrayOfPoints> Point3dArrayArray;


class BiCubicBezierPatchToDLFLData
{

public:
    BiCubicBezierPatchToDLFLData();
    QVector<QuadFace*> faces;
    doubleArrayArray UV_values;
    QVector<FourUValues> all_uv_values;
    void processBezierPatch(const TMPatch &patch);
    void release_memory(void);
    void toFile(QString address);
    struct patch_data get_patch_data();

private:
    uint uv_size;
    QHash<Point3d, uint> point_to_index;
    uint vertexCount;
};

#endif // PATCHTODLFL_HH
