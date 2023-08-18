/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

#ifndef _TM_PATCH_HH_
#define _TM_PATCH_HH_

/* Class for a TopMod Bezier Patch */

#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include <Light.hh>
#include <QDebug>
#include "patchtodlfl.hh"

#define CHILD_COUNT 4
#define STRING(value) QString::number(value, 'g', 8)

using namespace DLFL;

class Point3d;
class FourUValues;
class BiCubicBezierPatchToDLFLData;
typedef Vector3d Grid[4][4];

struct IndexTransform{
private:
    bool Forward;
public:
    IndexTransform(bool forward):Forward(forward){
    }
    int Get(const int &i)const{
      if(this->Forward)
        return i;
      return 3 - i;
    }
  };

void GetGrid(const Vector3dGrid &inputGrid,
                         const IndexTransform &X,
                         const IndexTransform &Y,
                         const bool &normalize,
                         Vector3dGrid &outputGrid);


void GetNextLevelPoint(const Vector3dGrid &inputGrid, const Matrix4x4 &weights,
                         const IndexTransform &X,
                         const IndexTransform &Y,
                         const bool &normalize,
                         Vector3d &out);





class TMPatch;


typedef TMPatch* TMPatchPtr;
typedef vector<TMPatchPtr> TMPatchPtrArray;

// This stuff is to map the patches to the face vertices by index
struct compare {
  bool operator()( DLFLFaceVertexPtr a, DLFLFaceVertexPtr b ) const {
        return ( a->getID() < b->getID() );
    }
};

typedef	std::map<DLFLFaceVertexPtr, TMPatchPtr, compare> TMPatchMap;

static void setPatchPtr( TMPatchMap &map, TMPatchPtr p, DLFLFaceVertexPtr fvp ) {
  map[fvp] = p;
}

static TMPatchPtr getPatchPtr( TMPatchMap &map, DLFLFaceVertexPtr fvp )  {
  std::map<DLFLFaceVertexPtr, TMPatchPtr, compare>::iterator it;
  it = map.find(fvp);
  return (*it).second;
}

static const int BEZIER_COEFFICIENTS[4] = {1,3,3,1};




class TMPatch {
friend class BiCubicBezierPatchToDLFLData;
private:
    static IndexTransform FORWARD,BACKWARD;
protected :
  static GLint PatchGridDimension;
  static int PatchNestingFactor;
  static bool LocalPatchNesting;
  static bool ShowControlMesh;
  static vector<GLint> GridDimensions;
  int patchsize;			   // Size of the control grid
  Vector3dGrid ctrlpts;		 // The grid of control points
  Vector3dGrid ctrlptnormals;      // Normal for each control point
  Vector4dGrid ctrlptcolors; // Color for each control point (RGBA)
  GLdouble * glctrlpts;		  // Control points for OpenGL
  GLdouble * glctrlptcolors;     // Control point colors for OpenGL


private:
  TMPatch* childs;              //this array represents the subdivided patches
  int level;                    //shows the level of nested patches down a tree;
                                //the lowest level is a level whose childs are NULL
  bool faceCornerIsFine;       //whether or not this patch belongs to a face with four edges

  bool vertexCornerIsFine;     //whether or not this patch belongs to a vertex whose valence is 4



  void allocateGLArray() {
    // Allocate memory for the GLdouble array to be sent to OpenGL
    // Uses patchsize to determine size of grid
    // Releases existing memory
    delete [] glctrlpts; glctrlpts = NULL;
    delete [] glctrlptcolors; glctrlptcolors = NULL;
    if ( patchsize > 0 ) {
            glctrlpts = new GLdouble[patchsize*patchsize*3];
            glctrlptcolors = new GLdouble[patchsize*patchsize*4];
        }
  }

  void populateGLArray()
  {
    // Copy values from the Vector3dGrid to the GLdouble array
    // Assumes that the GLdouble array has enough space
    // Assumes that the Vector3dGrid is a square grid of size patchsize
    if ( glctrlpts != NULL ) {
        // The order of traversal may have to be changed
        int index, colorindex;
        for (int i=0; i < patchsize; ++i)
            for (int j=0; j < patchsize; ++j) {
                index = (i*patchsize+j)*3;
                glctrlpts[index+0] = ctrlpts[j][i][0];
                glctrlpts[index+1] = ctrlpts[j][i][1];
                glctrlpts[index+2] = ctrlpts[j][i][2];

                colorindex = (i*patchsize+j)*4;
                glctrlptcolors[colorindex+0] = ctrlptcolors[j][i][0];
                glctrlptcolors[colorindex+1] = ctrlptcolors[j][i][1];
                glctrlptcolors[colorindex+2] = ctrlptcolors[j][i][2];
                glctrlptcolors[colorindex+3] = ctrlptcolors[j][i][3];
            }
        }
  }

public :

  // Default constructor
  TMPatch()
    : patchsize(0), ctrlpts(), ctrlptnormals(), ctrlptcolors(), glctrlpts(NULL), glctrlptcolors(NULL),
      childs(NULL),level(0), faceCornerIsFine(false),vertexCornerIsFine(false)
  {
    resizePatch(4);
  }

private:
  TMPatch(const TMPatch& patch)
    : patchsize(patch.patchsize), ctrlpts(patch.ctrlpts), ctrlptnormals(patch.ctrlptnormals),
      ctrlptcolors(patch.ctrlptcolors),
      glctrlpts(NULL), glctrlptcolors(NULL),
      childs(NULL), level(0), faceCornerIsFine(patch.faceCornerIsFine),vertexCornerIsFine(patch.vertexCornerIsFine)
  {
    //qDebug()<<"Path copy constructor called";
    allocateGLArray();
    populateGLArray();
    fillNestedPatchPntrs();
  }
public:
  // Destructor
  ~TMPatch()
  {
    //qDebug()<<"Destructor call at Level: "<< level;
    delete [] glctrlpts;
    delete [] glctrlptcolors;
    if(childs != NULL){
        delete[] childs;    //this will recursively delete all of the childs down the nested tree.
        childs = NULL;
    }
  }

  void destroy(){
      //qDebug()<<"Destructor call at Level: "<< level;
      delete [] glctrlpts;
      glctrlpts = NULL;
      delete [] glctrlptcolors;
      glctrlptcolors = NULL;
      delete childs;
      childs = NULL;
  }
private:
  // Assignment operator
  TMPatch& operator = (const TMPatch& patch)
  {
    patchsize = patch.patchsize;
    ctrlpts = patch.ctrlpts; ctrlptnormals = patch.ctrlptnormals;
    ctrlptcolors = patch.ctrlptcolors;
    level = patch.level;
    faceCornerIsFine = patch.faceCornerIsFine;
    vertexCornerIsFine = patch.vertexCornerIsFine;
    allocateGLArray(); populateGLArray();
    if (patch.childs != NULL) {
        fillNestedPatchPntrs();
    }
    return (*this);
  }

public :
  static void SetPatchGridDimension(int size);
  static int GetPatchGridDimension(void);
  static void SetPatchNestingFactor(int maxLevel);
  static int GetPatchNestingFactor(void);
  static void SetLocalPatchNesting(bool value);
  static bool GetLocalPatchNesting();
  static void UpdatePatchGridDimensions();
  static bool GetShowControlMesh();
  static void SetShowControlMesh(bool value);

  // Resize the patch of control points
  void resizePatch(int size) {
    if ( size != patchsize ) {
            patchsize = size;
            ctrlpts.resize(patchsize); ctrlptnormals.resize(patchsize); ctrlptcolors.resize(patchsize);
            for (int i=0; i < ctrlpts.size(); ++i) {
                    ctrlpts[i].resize(patchsize);
                    ctrlptnormals[i].resize(patchsize);
                    ctrlptcolors[i].resize(patchsize);
            }
        }
    allocateGLArray(); // Has to be reallocated if size changes
  }
private:

  void allocateChilds(){
      delete[] childs;
      childs = NULL;
      childs = new TMPatch[CHILD_COUNT];
      int childCount =0;
      //vertex corner
      childs[childCount].level = level+1;
      GetGrid(ctrlpts, FORWARD,FORWARD,false,childs[childCount].ctrlpts);
      GetGrid(ctrlptnormals, FORWARD,FORWARD,true,childs[childCount].ctrlptnormals);
      childs[childCount].faceCornerIsFine = true;
      childs[childCount].vertexCornerIsFine = this->vertexCornerIsFine;
      childs[childCount].fillNestedPatchPntrs();

      childCount++;

      childs[childCount].level = level+1;
      GetGrid(ctrlpts, BACKWARD,FORWARD,false,childs[childCount].ctrlpts);
      GetGrid(ctrlptnormals, BACKWARD,FORWARD,true,childs[childCount].ctrlptnormals);
      childs[childCount].faceCornerIsFine = true;
      childs[childCount].vertexCornerIsFine = true;
      childs[childCount].fillNestedPatchPntrs();

      childCount++;

      childs[childCount].level = level+1;
      GetGrid(ctrlpts, FORWARD,BACKWARD,false,childs[childCount].ctrlpts);
      GetGrid(ctrlptnormals, FORWARD,BACKWARD,true,childs[childCount].ctrlptnormals);
      childs[childCount].faceCornerIsFine = true;
      childs[childCount].vertexCornerIsFine = true;
      childs[childCount].fillNestedPatchPntrs();


      childCount++;
      //face corner
      childs[childCount].level = level+1;
      GetGrid(ctrlpts, BACKWARD,BACKWARD,false,childs[childCount].ctrlpts);
      GetGrid(ctrlptnormals, BACKWARD,BACKWARD,true,childs[childCount].ctrlptnormals);
      childs[childCount].faceCornerIsFine = this->faceCornerIsFine;
      childs[childCount].vertexCornerIsFine = true;
      childs[childCount].fillNestedPatchPntrs();
  }


public:
  //this function will recursively fill the child patches that belong to a patch
  void fillNestedPatchPntrs(){
      //the condition to terminate the recursion
      if (this->level == TMPatch::PatchNestingFactor) {
          allocateGLArray();
          populateGLArray();
          if (this->childs != NULL) {
              qDebug()<<"lastr layer includes childs!!! WHYYYYY???";
          }
          return;
      }
      if (TMPatch::LocalPatchNesting) {
        if (faceCornerIsFine == false|| vertexCornerIsFine == false) {
            this->allocateChilds();
        }
        else{
            allocateGLArray();
            populateGLArray();
        }
      }
      //globally subdivide all of the paches. The numbers increase exponentially
      else{
          this->allocateChilds();
      }
}
const Vector3d& getControlPoint(int i, int j) const
  {
    // Return the control point at the specified location
    // Does not check for index out of bounds errors
    // Flips the i and j since it is flipped when control points are stored (see calculatePatchPoints method)
    return ctrlpts[j][i];
  }

  void setControlPoint(int i, int j, const Vector3d& p)
  {
    // Set the control point at the specified location
    // Does not check for index out of bounds errors
    // Flips the i and j since it is flipped when control points are stored (see calculatePatchPoints method)
    ctrlpts[j][i] = p;
  }

  // Calculate the points of the patch given the base grid points and base grid normals
  void calculatePatchPoints(const Vector3d (&cp)[4][4], const Vector3d (&cn)[4][4],
  const bool &fourSidedFace, const bool &fourValenceVertex)
  {
    // resize the patch to be 4x4
    resizePatch(4);
    int index;
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j)
    {
      ctrlpts[j][i] = cp[i][j];
      ctrlptnormals[j][i] = cn[i][j];
      index = (i*patchsize+j)*3;
      glctrlpts[index+0] = ctrlpts[j][i][0];
      glctrlpts[index+1] = ctrlpts[j][i][1];
      glctrlpts[index+2] = ctrlpts[j][i][2];
    }
    this->faceCornerIsFine = fourSidedFace;
    this->vertexCornerIsFine = fourValenceVertex;

  }

  Point3d point_at(const FourUValues &u_values, const FourUValues &v_values)const;

  Point3d point_at(const double &u, const double &v)const;

  Vector3d normal_at(const double &u, const double &v)const;

  void updateGLPointArray(void)
  {
        if( glctrlpts == NULL )
            return;
    int index;
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j) {
                index = (i*patchsize+j)*3;
                glctrlpts[index+0] = ctrlpts[j][i][0];
                glctrlpts[index+1] = ctrlpts[j][i][1];
                glctrlpts[index+2] = ctrlpts[j][i][2];
            }
  }

  void fillGLLightingBuffer(const RGBColor& basecolor, double Ka, double Kd, double Ks, LightPtr lightptr)
  {
    // Calculate lighting at each control point and update the color
    int colorindex;
    RGBColor color;
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j)
    {
      color = Kd * lightptr->illuminate(ctrlpts[j][i],ctrlptnormals[j][i]);
      color += (1.0 - Kd) * basecolor;
      colorindex = (i*patchsize+j)*4;
      glctrlptcolors[colorindex+0] = color.r;
      glctrlptcolors[colorindex+1] = color.g;
      glctrlptcolors[colorindex+2] = color.b;
      glctrlptcolors[colorindex+3] = 1.0;
    }
  }

  void computeLighting(const RGBColor& basecolor, double Ka, double Kd, double Ks, LightPtr lightptr){
      if (this->childs == NULL) {
          fillGLLightingBuffer(basecolor, Ka, Kd, Ks, lightptr);
          return;
      }
      else {
          for (int i = 0; i < CHILD_COUNT; ++i) {
              childs[i].computeLighting(basecolor, Ka, Kd, Ks, lightptr);
          }
      }
  }


  // Render this patch using filled polygons
private:
  void renderFilledPolygons(void)
  //void render(void)
  {
    glMap2d(GL_MAP2_VERTEX_3,0,1,3,patchsize,0,1,patchsize*3,patchsize,glctrlpts);
    glMap2d(GL_MAP2_COLOR_4,0,1,4,patchsize,0,1,patchsize*4,patchsize,glctrlptcolors);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_MAP2_COLOR_4);
    glMapGrid2d(TMPatch::GridDimensions[this->level],0.0,1.0,TMPatch::GridDimensions[this->level],0.0,1.0);
    glEvalMesh2(GL_FILL,0,TMPatch::GridDimensions[this->level],0,TMPatch::GridDimensions[this->level]);
    glDisable(GL_MAP2_COLOR_4);
    glDisable(GL_MAP2_VERTEX_3);
  }
public:
  void render(void)
  {
      if (this->childs == NULL) {
          renderFilledPolygons();
      }
      else{
          for (int i = 0; i < CHILD_COUNT; ++i) {
              this->childs[i].render();
          }
      }
  }
private:
  // Render this patch using outlined polygons
  void renderPatchOutlinedPolygons(void)
  {
    glMap2d(GL_MAP2_VERTEX_3,0,1,3,patchsize,0,1,patchsize*3,patchsize,glctrlpts);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2d(TMPatch::GridDimensions[this->level],0.0,1.0,TMPatch::GridDimensions[this->level],0.0,1.0);
    glEvalMesh2(GL_LINE,0,TMPatch::GridDimensions[this->level],0,TMPatch::GridDimensions[this->level]);
    glDisable(GL_MAP2_VERTEX_3);
  }
public:
  void outline(void)
  {
      if (this->childs == NULL) {
          renderPatchOutlinedPolygons();
      }
      else{
          for (int i = 0; i < CHILD_COUNT; ++i) {
              this->childs[i].outline();
          }
      }
  }

private:
  // Render the patch boundary
  void renderPatchBoundary(void)
  {
    glMap1d(GL_MAP1_VERTEX_3,0,1,3,patchsize,glctrlpts);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(TMPatch::GridDimensions[this->level],0.0,1.0);
    glEvalMesh1(GL_LINE,0,TMPatch::GridDimensions[this->level]);
    glDisable(GL_MAP1_VERTEX_3);

    glMap1d(GL_MAP1_VERTEX_3,0,1,patchsize*3,patchsize,glctrlpts+patchsize*3-3);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(TMPatch::GridDimensions[this->level],0.0,1.0);
    glEvalMesh1(GL_LINE,0,TMPatch::GridDimensions[this->level]);
    glDisable(GL_MAP1_VERTEX_3);
  }
public:
  // Render the patch boundary
  void patch_boundary(void)
  {
      if (this->childs == NULL) {
          renderPatchBoundary();
      }
      else{
          for (int i = 0; i < CHILD_COUNT; ++i) {
              this->childs[i].patch_boundary();
          }
      }
  }

public:
  // Render the patch boundary
  void face_boundary(void)
  {
    glMap1d(GL_MAP1_VERTEX_3,0,1,3,patchsize,glctrlpts);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(TMPatch::GridDimensions[this->level],0.0,1.0);
    glEvalMesh1(GL_LINE,0,TMPatch::GridDimensions[this->level]);
    glDisable(GL_MAP1_VERTEX_3);
  }

private:
  // Render the control patch as a grid of lines
  void renderControlPatchAsGridLines(void)
  {
    for (int i=0; i < patchsize; ++i)
      {
    glBegin(GL_LINE_STRIP);
    for (int j=0; j < patchsize; ++j)
      glVertex3dv( (ctrlpts[i][j]).getCArray() );
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int j=0; j < patchsize; ++j)
      glVertex3dv( (ctrlpts[j][i]).getCArray() );
    glEnd();
      }
  }

public:
  void controlgrid(void)
  {
      if (this->childs == NULL) {
          renderControlPatchAsGridLines();
      }
      else{
          for (int i = 0; i < CHILD_COUNT; ++i) {
              this->childs[i].controlgrid();
          }
      }
  }

private:
  // Show the control points alone
  void showControlPointsAlone(void)
  {
    glBegin(GL_POINTS);
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j)
    glVertex3dv( (ctrlpts[i][j]).getCArray() );
    glEnd();
  }

public:
  void controlpoints(void){
      if (this->childs == NULL) {
          showControlPointsAlone();
      }
      else{
          for (int i = 0; i < CHILD_COUNT; ++i) {
              this->childs[i].controlpoints();
          }
      }
  }

private:
  void showNormals(void)
  {
    glBegin(GL_LINES);
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j) {
      glVertex3dv( (ctrlpts[i][j]).getCArray() );
      Vector3d vec = ctrlpts[i][j] + 0.25*ctrlptnormals[i][j];
      glVertex3dv( vec.getCArray() );
    }
  }

public:
  void renderNormals(void){
      if (this->childs == NULL) {
          showNormals();
      }
      else{
          for (int i = 0; i < CHILD_COUNT; ++i) {
              this->childs[i].renderNormals();
          }
      }
  }


  void renderControlMeshExternalEdges(){
      qDebug()<<"rendering external edges";
      glBegin(GL_LINE_LOOP);
      glVertex3dv( (ctrlpts[0][0]).getCArray());

      glVertex3dv( (ctrlpts[0][1]).getCArray());
      glVertex3dv( (ctrlpts[0][2]).getCArray());

      glVertex3dv( (ctrlpts[0][3]).getCArray());

      glVertex3dv( (ctrlpts[1][3]).getCArray());
      glVertex3dv( (ctrlpts[2][3]).getCArray());

      glVertex3dv( (ctrlpts[3][3]).getCArray());

      glVertex3dv( (ctrlpts[3][2]).getCArray());
      glVertex3dv( (ctrlpts[3][1]).getCArray());

      glVertex3dv( (ctrlpts[3][0]).getCArray());

      glVertex3dv( (ctrlpts[2][0]).getCArray());
      glVertex3dv( (ctrlpts[1][0]).getCArray());
      glEnd();
  }

  void renderControlMeshInternalEdges(){
      glBegin(GL_LINE_STRIP);
      glVertex3dv( (ctrlpts[1][0]).getCArray());
      glVertex3dv( (ctrlpts[1][1]).getCArray());
      glVertex3dv( (ctrlpts[1][2]).getCArray());
      glVertex3dv( (ctrlpts[1][3]).getCArray());
      glEnd();
      glBegin(GL_LINE_STRIP);
      glVertex3dv( (ctrlpts[2][0]).getCArray());
      glVertex3dv( (ctrlpts[2][1]).getCArray());
      glVertex3dv( (ctrlpts[2][2]).getCArray());
      glVertex3dv( (ctrlpts[2][3]).getCArray());
      glEnd();
      glBegin(GL_LINE_STRIP);
      glVertex3dv( (ctrlpts[0][1]).getCArray());
      glVertex3dv( (ctrlpts[1][1]).getCArray());
      glVertex3dv( (ctrlpts[2][1]).getCArray());
      glVertex3dv( (ctrlpts[3][1]).getCArray());
      glEnd();
      glBegin(GL_LINE_STRIP);
      glVertex3dv( (ctrlpts[0][2]).getCArray());
      glVertex3dv( (ctrlpts[1][2]).getCArray());
      glVertex3dv( (ctrlpts[2][2]).getCArray());
      glVertex3dv( (ctrlpts[3][2]).getCArray());
      glEnd();
  }


  /* stuart - bezier export */
  void printControlPoints( ostream &stream ) {
    for( int i = 0; i < patchsize; i++ ) {
      for( int j = 0; j < patchsize; j++ ) {
    stream << "v "
           << ctrlpts[i][j][0] << " "
           << ctrlpts[i][j][1] << " "
           << ctrlpts[i][j][2]
           << std::endl;
      }
    }
  }




};

#endif /* #ifndef _TM_PATCH_HH_ */

