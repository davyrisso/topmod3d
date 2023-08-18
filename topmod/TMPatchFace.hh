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

#ifndef _TM_PATCH_FACE_HH_
#define _TM_PATCH_FACE_HH_
#include "patchmode.hh"
#include "TMPatch.hh"
#include "patchtodlfl.hh"
/* Class for a TopMod Bezier Face containing several Patches */

class TMPatchFace;
typedef TMPatchFace *TMPatchFacePtr;

typedef list<TMPatchFacePtr> TMPatchFacePtrList;
typedef vector<TMPatchFacePtr> TMPatchFacePtrArray;

typedef list<TMPatchFace> TMPatchFaceList;
typedef vector<TMPatchFace> TMPatchFaceArray;


class TMPatchFace {
protected :

  int patchsize;		// Grid size of each patch in the face
  TMPatchPtrArray patcharray;	// Array of pointer to Bezier patches for this face
  DLFLFacePtr dlflface;	// DLFLFace associated with this Bezier face

  // Resize the patch array depending on number of corners in the DLFLFace
  void resizePatchArray(void);

public :

  // Default & 1-arg constructor
  TMPatchFace(int psize=4)
    : patchsize(psize), patcharray(), dlflface(NULL) {}

  // Destructor
  ~TMPatchFace() {
      for (int i = 0; i < (int)patcharray.size(); ++i) {
          delete patcharray[i];
          patcharray[i] = NULL;
      }
      patcharray.clear();
  }

  void destroy(){
      for (int i = 0; i < (int)patcharray.size(); ++i) {
          delete patcharray[i];
          patcharray[i] = NULL;
      }
      patcharray.clear();
  }

private:
  // Copy constructor
  TMPatchFace(const TMPatchFace& tmpf)
    : patchsize(tmpf.patchsize), patcharray(tmpf.patcharray), dlflface(tmpf.dlflface) {}

  // Assignment operator
  TMPatchFace&  operator = (const TMPatchFace& tmpf) {
    patchsize = tmpf.patchsize; patcharray = tmpf.patcharray; dlflface = tmpf.dlflface;
    return (*this);
  }
public:
  //The indices of the corners in a bezier patch
  static const int CORNERS[][2];
  //The indices of the neighbors of the corners in a bezier patch
  static const int CORNER_MAP[][3][2];

  // Resize all patches
  void resizePatches(int psize);

  // Set the DLFLFace pointer
  void setDLFLFace(DLFLFacePtr fp) {
    dlflface = fp;
    resizePatchArray();
  }
     
  // Create the patches using face information
  void createPatches(TMPatchMap &patchMap, const struct PatchParameters &parameters);

  // Adjust the edge points for each patch in the face
  void adjustEdgePoints(TMPatchMap &patchMap);
              
  // Compute lighting for the patches in this face
  void computeLighting(LightPtr lightptr) {
    DLFLMaterialPtr matl;
    for (uint i=0; i < patcharray.size(); ++i) {
      matl = dlflface->material();
      patcharray[i]->computeLighting(matl->color,matl->Ka,matl->Kd,matl->Ks,lightptr);
    }
  }
     
  // Render the patches using filled polygons
  void render(void) {
   for (uint i=0; i < patcharray.size(); ++i) {
    patcharray[i]->render();
   }
  }

  void renderControlMeshInternalEdges(){
      for (uint i=0; i < patcharray.size(); ++i) {
       patcharray[i]->renderControlMeshInternalEdges();
      }
  }

  void renderControlMeshExternalEdges(){
      for (uint i=0; i < patcharray.size(); ++i) {
       patcharray[i]->renderControlMeshExternalEdges();

      }
  }

  // Render the patches using outlined polygons
  void outline(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i]->outline();
  }

  // Render the boundaries of the patches
  void patch_boundary(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i]->patch_boundary();
  }

  // Render the boundaries of the patches
  void face_boundary(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i]->face_boundary();
  }

  // Render the control grids of the patches
  void controlgrid(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i]->controlgrid();
  }

  // Show the conrtol points
  void controlpoints(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i]->controlpoints();
  }

  // Show the normals
  void renderNormals(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i]->renderNormals();
  }
     
  /* stuart - bezier export */ 
  int print( ostream &stream ) {
    for( int i = 0; i < patcharray.size(); i++ ) {
      patcharray[i]->printControlPoints( stream );
      stream << std::endl;
    }
    return patcharray.size( );
  }

  void extractPatchDataFromFace(const BiCubicBezierPatchToDLFLData* dataExtractor);
};

#endif /* #ifndef _TM_PATCH_FACE_HH_ */

