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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

#include "TMPatchFace.hh"
#include<QMessageBox>
#include <QStringBuilder>


//the indices of the neighbors of the corners in a bezier patch
static const int TMPatchFace::CORNER_MAP[][3][2]=
{
    {
        {0,1},
        {1,0},
        {1,1}
    },
    {
        {0,2},
        {1,2},
        {1,3}},
    {
        {2,2},
        {2,3},
        {3,2}
    },
    {
        {2,0},
        {2,1},
        {3,1}
    },
};

static const int TMPatchFace::CORNERS[][2] =
{
    {0,0},
    {0,3},
    {3,3},
    {3,0}
};

void TMPatchFace::extractPatchDataFromFace(const BiCubicBezierPatchToDLFLData* dataExtractor){
    TMPatchPtrArray::iterator begin = this->patcharray.begin(), end = this->patcharray.end();
    while (begin!=end) {
        TMPatch* patchPntr = *begin;
        dataExtractor->processBezierPatch(*patchPntr);

        begin++;
    }
}

// Resize the patch array depending on number of corners in the DLFLFace
void TMPatchFace::resizePatchArray(void) {
  if ( dlflface != NULL ) {
      //delete the existing patches
      for (int i = 0; i < patcharray.size(); ++i) {
          delete patcharray[i];
          patcharray[i] = NULL;
      }
    patcharray.resize(dlflface->size());
    for (uint i=0; i < patcharray.size(); ++i) {
      patcharray[i]=new TMPatch();
      patcharray[i]->resizePatch(patchsize);
		}
  }
}

// Resize all patches
void TMPatchFace::resizePatches(int psize) {
  if ( psize != patchsize ) {
    patchsize = psize;
    for (uint i=0; i < patcharray.size(); ++i) {
      patcharray[i]->resizePatch(patchsize);
		}
  }
}
     
// Create the patches using face information
void TMPatchFace::createPatches(TMPatchMap &patchMap, const struct PatchParameters &params) {
  if ( dlflface == NULL ) return;

  // patcharray will be resized here
  // This is to take care of situations where the no. of corners in the face has
  // changed after it was associated with a TMPatchFace
  resizePatchArray();
	 
  // Get the centroid and normal for the face - these will be used in all patches
  Vector3d vface = dlflface->getAuxCoords();
  Vector3d nface = dlflface->getAuxNormal();
	// std::cout << vface << " " << nface << "\n";

  // Get the corners of the face as an array
  // A patch will be created for each corner
  DLFLFaceVertexPtrArray corners;
  int size;
  dlflface->getCorners(corners);
  size = corners.size();
  bool isFourSidedFace = size == 4;

  Vector3d cp[4][4]; // Grid of control points
  Vector3d cn[4][4]; // Grid of control normals used to modify the control points

  for (int i=0; i < size; ++i) {
    bool isFourValenceVertex = corners[i]->getVertexPtr()->valence() == 4;

    /*VERTEX*/
    cp[0][0] = corners[i]->vertex->getAuxCoords();
    cn[0][0] = corners[i]->vertex->getAuxNormal();
    //Doo-Sabin polygon vertex
    cp[1][1] = corners[i]->getDS2Coord(0);
    cn[1][1] = cn[0][0];
    //middle point of Doo-Sabin polygon Edge
    cp[1][0] = corners[i]->vnext()->getDS2Coord(0) + cp[1][1];
    cp[1][0]/=2.0;
    cn[1][0] = cn[0][0];
    //middle point of Doo-Sabin polygon Edge
    cp[0][1] = corners[i]->vprev()->getDS2Coord(0) + cp[1][1];
    cp[0][1]/= 2.0;
    cn[0][1] = cn[0][0];

    /*EDGE*/
    cp[3][0] = corners[i]->getEdgePtr()->getAuxCoords();
    cn[3][0] = corners[i]->getEdgePtr()->getAuxNormal();
    //Doo-Sabin polygon vertex
    cp[2][1] = corners[i]->getDS2Coord(1);
    cn[2][1] = cn[3][0];
    //middle point of Doo-Sabin polygon Edge
    cp[3][1] = corners[i]->next()->getDS2Coord(3) + cp[2][1];
    cp[3][1]/=2.0;
    cn[3][1] = cn[3][0];
    //middle point of Doo-Sabin polygon Edge
    cp[2][0] = corners[i]->vnext()->getDS2Coord(3) + cp[2][1];
    cp[2][0] /=2.0;
    cn[2][0] = cn[3][0];
              
    /*FACE*/
    cp[3][3] = vface;
    cn[3][3] = nface;
    //Doo-Sabin polygon vertex
    cp[2][2] = corners[i]->getDS2Coord(2);
    cn[2][2] = cn[3][3];
    //middle point of Doo-Sabin polygon Edge
    cp[2][3] = corners[i]->prev()->getDS2Coord(2) + cp[2][2];
    cp[2][3]/=2.0;
    cn[2][3] = cn[3][3];
    //middle point of Doo-Sabin polygon Edge
    cp[3][2] = corners[i]->next()->getDS2Coord(2) + cp[2][2];
    cp[3][2]/=2.0;
    cn[3][2] = cn[3][3];

    /*EDGE*/
    cp[0][3] = corners[i]->prev()->getEdgePtr()->getAuxCoords();
    cn[0][3] = corners[i]->prev()->getEdgePtr()->getAuxNormal();
    //Doo-Sabin polygon vertex
    cp[1][2] = corners[i]->getDS2Coord(3);
    cn[1][2] = cn[0][3];
    //middle point of Doo-Sabin polygon Edge
    cp[0][2] = corners[i]->vprev()->getDS2Coord(1) + cp[1][2];
    cp[0][2]/=2.0;
    cn[0][2] = cn[0][3];
    //middle point of Doo-Sabin polygon Edge
    cp[1][3] = corners[i]->prev()->getDS2Coord(1) + cp[1][2];
    cp[1][3]/=2.0;
    cn[1][3] = cn[0][3];

    //apply scaling factor
    cp[1][0] = cp[0][0] + (cp[1][0]-cp[0][0])*params.scale;
    cp[1][1] = cp[0][0] + (cp[1][1]-cp[0][0])*params.scale;
    cp[0][1] = cp[0][0] + (cp[0][1]-cp[0][0])*params.scale;

    cp[2][0] = cp[3][0] + (cp[2][0]-cp[3][0])*params.scale;
    cp[2][1] = cp[3][0] + (cp[2][1]-cp[3][0])*params.scale;
    cp[3][1] = cp[3][0] + (cp[3][1]-cp[3][0])*params.scale;

    cp[3][2] = cp[3][3] + (cp[3][2]-cp[3][3])*params.scale;
    cp[2][2] = cp[3][3] + (cp[2][2]-cp[3][3])*params.scale;
    cp[2][3] = cp[3][3] + (cp[2][3]-cp[3][3])*params.scale;

    cp[0][2] = cp[0][3] + (cp[0][2]-cp[0][3])*params.scale;
    cp[1][2] = cp[0][3] + (cp[1][2]-cp[0][3])*params.scale;
    cp[1][3] = cp[0][3] + (cp[1][3]-cp[0][3])*params.scale;

    // Adjust the normals for interior and edge middle points
    cn[1][0] = normalized(3*cn[0][0]+cn[3][0]);
    cn[0][1] = normalized(3*cn[0][0]+cn[0][3]);
    cn[1][1] = normalized(6*cn[0][0]+cn[0][3]+cn[3][0]);

    cn[2][0] = normalized(3*cn[3][0]+cn[0][0]);
    cn[3][1] = normalized(3*cn[3][0]+cn[3][3]);
    cn[2][1] = normalized(6*cn[3][0]+cn[0][0]+cn[3][3]);

    cn[0][2] = normalized(3*cn[0][3]+cn[0][0]);
    cn[1][3] = normalized(3*cn[0][3]+cn[3][3]);
    cn[1][2] = normalized(6*cn[0][3]+cn[0][0]+cn[3][3]);

    cn[2][3] = normalized(3*cn[3][3]+cn[0][3]);
    cn[3][2] = normalized(3*cn[3][3]+cn[3][0]);
    cn[2][2] = normalized(6*cn[3][3]+cn[0][3]+cn[3][0]);


    /*Start of a section applies the rotation to the points*/
    for (int corner_index = 0; corner_index < 4; ++corner_index) {
        for (int neighbor_index = 0; neighbor_index < 3; ++neighbor_index) {
            int corner_i = CORNERS[corner_index][0];
            int corner_j = CORNERS[corner_index][1];
            int neighbor_x = CORNER_MAP[corner_index][neighbor_index][0];
            int neighbor_y = CORNER_MAP[corner_index][neighbor_index][1];
            Vector3d v = cp[neighbor_x][neighbor_y]-cp[corner_i][corner_j];
            switch(params.rotation_method){
                case ALL_UNIFORMLY_R:
                    DLFL::rotateAroundVector(cn[corner_i][corner_j], params.angle, true,v);
                    cp[neighbor_x][neighbor_y] = cp[corner_i][corner_j] + v;
                    break;
                case ALTERNATE_R:
                    if (corner_index % 2 == 0) {
                        DLFL::rotateAroundVector(cn[corner_i][corner_j], params.angle, true, v);
                        cp[neighbor_x][neighbor_y] = cp[corner_i][corner_j] + v;
                    }
                    else {
                        DLFL::rotateAroundVector(cn[corner_i][corner_j], params.angle, true, v);
                        cp[neighbor_x][neighbor_y] = cp[corner_i][corner_j] + v;
                    }
                break;
                case VERTEX_FACE_ONLY_R:
                    if (corner_index % 2 == 0) {
                        DLFL::rotateAroundVector(cn[corner_i][corner_j], params.angle, true, v);
                        cp[neighbor_x][neighbor_y] = cp[corner_i][corner_j] + v;
                    }
                    break;
                case EDGES_ONLY_R:
                    if (corner_index % 2 != 0) {
                        DLFL::rotateAroundVector(cn[corner_i][corner_j], params.angle, true, v);
                        cp[neighbor_x][neighbor_y] = cp[corner_i][corner_j] + v;
                    }
                    break;
            }
        }
    }
    /*End of sectionin which rotation is applied*/
    patcharray[i]->calculatePatchPoints(cp,cn, isFourSidedFace, isFourValenceVertex);
    setPatchPtr(patchMap, patcharray[i], corners[i] );
  }

  //load the patch childs
  for (int i = 0; i < patcharray.size(); ++i) {
      patcharray[i]->fillNestedPatchPntrs();
  }
}

