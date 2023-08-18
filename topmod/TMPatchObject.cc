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

#include "TMPatchObject.hh"
#include <QDebug>

 bool TMPatchObject::RenderIncludesPatchEdges = false;
 bool TMPatchObject::RenderIncludesDooSabinPolygons = false;
 bool TMPatchObject::RenderIncludesModifiedDooSabinPolygons = false;

 void TMPatchObject::SetRenderIncludesPatchEdges(bool value){
     TMPatchObject::RenderIncludesPatchEdges = value;
 }

 void TMPatchObject::SetRenderIncludesDooSabinPolygons(bool value){
     TMPatchObject::RenderIncludesDooSabinPolygons = value;
 }

 void TMPatchObject::SetRenderIncludesModifiedDooSabinPolygons(bool value){
     TMPatchObject::RenderIncludesModifiedDooSabinPolygons = value;
 }

 bool TMPatchObject::GetRenderIncludesPatchEdges(){
     return TMPatchObject::RenderIncludesPatchEdges;
 }

 bool TMPatchObject::GetRenderIncludesDooSabinPolygons(){
     return TMPatchObject::RenderIncludesDooSabinPolygons;
 }

 bool TMPatchObject::GetRenderIncludesModifiedDooSabinPolygons(){
     return TMPatchObject::RenderIncludesModifiedDooSabinPolygons;
 }

void TMPatchObject::updateForPatches( const struct PatchParameters &parameters, DLFLObjectPtr obj ) {


    dooSabinPolygons.clear();
    modifiedDooSabinPolygons.clear();

  DLFLFacePtrList face_list = obj->getFaceList( );
  DLFLEdgePtrList edge_list = obj->getEdgeList( );
  DLFLVertexPtrList vertex_list = obj->getVertexList( );

  // Update information stored at each face, vertex, edge and corner for patch rendering

  // Compute doo-sabin coordinates for each face and store them in the auxcoord field of the corner
  // Update the auxcoord field of the face
  Vector3dArray coords;
  DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
  DLFLFacePtr fp;
  DLFLFaceVertexPtrArray corners;
  int valence;
       
  while ( ffirst != flast ) {
    fp = *ffirst; ++ffirst;
    fp->getCornersAndCoords(corners,coords);
    valence = coords.size();

    if ( valence > 0 ) {
      // Compute Doo-Sabin coordinates - Level 1
      DLFL::computeDooSabinCoords(coords);
      for (int i=0; i < valence; ++i) 
				corners[i]->setAuxCoords(coords[i]);

      // Compute Doo-Sabin coordinates - Level 2
      DLFL::computeDooSabinCoords(coords);

      Vector3dArray originalPolygon(coords);
      DLFL::scale(originalPolygon, parameters.scale);
      dooSabinPolygons.push_back(originalPolygon);

      //regularize
      if (parameters.regularizePolygons && parameters.polygonRegularizationIteration > 0 && valence>2) {

          qDebug()<<"Regulizing Face "<< coords.size() <<" sized polygon in "<< parameters.polygonRegularizationIteration <<" iterations";
          if (coords.size() != 3) {
              qDebug()<<"Polygon";
              DLFL::regularizePolygon(coords, parameters.polygonRegularizationIteration);
          }
          if (coords.size() == 3) {
              qDebug()<<"Triangle";
              DLFL::regularizeTriangle(coords, parameters.polygonRegularizationIteration);
          }
      }

      // Compute the patch point and patch normal
      Vector3d pp, pn;
      DLFL::computeCentroidAndNormal(coords,pp,pn);
      //DLFL::computeEdgeBasedCentroidAndNormal(coords,pp,pn);
      fp->setAuxCoords(pp); fp->setAuxNormal(pn);


      //planarize the Doo-Sabin polygon only if it is not a quad
      if (fp->numFaceVertexes() != 4) {
          DLFL::planarProject(coords, pp, pn);
          Vector3dArray modifiedPolygon(coords);
          DLFL::scale(modifiedPolygon, parameters.scale);
          modifiedDooSabinPolygons.push_back(modifiedPolygon);
      }
      else{
          if (parameters.regularizePolygons && parameters.polygonRegularizationIteration > 0 && valence >2) {
              Vector3dArray modifiedPolygon(coords);
              DLFL::scale(modifiedPolygon, parameters.scale);
              modifiedDooSabinPolygons.push_back(modifiedPolygon);
          }
      }



      for (int i=0; i < valence; ++i) {
				corners[i]->setDS2Coord2(coords[i]);
      }

    }
  }

  // Compute patch point and normal for all edges
  DLFLEdgePtrList::iterator efirst=edge_list.begin(), elast=edge_list.end();
  DLFLEdgePtr ep;
  while ( efirst != elast ) {
    ep = *efirst; ++efirst;

    Vector3dArray p;
    ep->getEFCornersAuxCoords(p);
            
    // Compute Doo-Sabin coordinates - Level 2
    DLFL::computeDooSabinCoords(p);

    Vector3dArray polygon(p);
    DLFL::scale(polygon, parameters.scale);
    dooSabinPolygons.push_back(polygon);

    if (parameters.regularizePolygons && parameters.polygonRegularizationIteration > 0) {

        qDebug()<<"Regulizing EDGE "<< p.size() <<" sized polygon in "<< parameters.polygonRegularizationIteration <<" iterations";
        DLFL::regularizePolygon(p,parameters.polygonRegularizationIteration);
    }

    Vector3d pp,pn;
    computeCentroidAndNormal(p,pp,pn);
    ep->setAuxCoords(pp); ep->setAuxNormal(pn);

    DLFLFaceVertexPtrArray fvp;
    ep->getEFCorners(fvp);
    fvp[0]->setDS2Coord3(p[0]); fvp[1]->setDS2Coord1(p[1]);
    fvp[2]->setDS2Coord3(p[2]); fvp[3]->setDS2Coord1(p[3]);
    //there will not be modified polygons

    if (parameters.regularizePolygons && parameters.polygonRegularizationIteration > 0) {
        Vector3dArray modified(p);
        DLFL::scale(modified, parameters.scale);
        modifiedDooSabinPolygons.push_back(modified);
    }

  }

  // Compute patch point and normal for all vertices
  DLFLVertexPtrList::iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
  DLFLVertexPtr vp;
  while ( vfirst != vlast ) {
    vp = *vfirst; ++vfirst;
    Vector3dArray p;
    vp->getOrderedCornerAuxCoords(p);

    // Compute Doo-Sabin coordinates - Level 2
    DLFL::computeDooSabinCoords(p);
    Vector3dArray polygon(p);
    DLFL::scale(polygon, parameters.scale);
    dooSabinPolygons.push_back(polygon);

    //regularize
    if (parameters.regularizePolygons && parameters.polygonRegularizationIteration > 0 && p.size()>2) {
        qDebug()<<"Regulizing Vertex "<< p.size() <<" sized polygon in "<< parameters.polygonRegularizationIteration <<" iterations";
        if (p.size() != 3) {
            qDebug()<<"Polygon";
            DLFL::regularizePolygon(p, parameters.polygonRegularizationIteration);
        }
        if (p.size() == 3) {
            qDebug()<<"Triangle";
            DLFL::regularizeTriangle(p, parameters.polygonRegularizationIteration);
        }
    }

    Vector3d pp,pn;
    DLFL::computeCentroidAndNormal(p,pp,pn);
    //DLFL::computeEdgeBasedCentroidAndNormal(p,pp,pn);
    vp->setAuxCoords(pp); vp->setAuxNormal(-pn); // Reverse the normal since the rotation order around the vertex is clockwise



    //planarize the Doo-Sabin polygon only if it is not a quad
    if (vp->valence() != 4) {
        DLFL::planarProject(p, pp, pn);//we do not care if the true normal is reversed
        Vector3dArray modifiedPolygon(p);
        DLFL::scale(modifiedPolygon, parameters.scale);
        modifiedDooSabinPolygons.push_back(modifiedPolygon);
    }
    else{
        if (parameters.regularizePolygons && parameters.polygonRegularizationIteration > 0 && p.size()>2) {
            Vector3dArray modifiedPolygon(p);
            DLFL::scale(modifiedPolygon, parameters.scale);
            modifiedDooSabinPolygons.push_back(modifiedPolygon);
        }
    }


    DLFLFaceVertexPtrArray fvp;
    vp->getOrderedCorners(fvp);
    for (int i=0; i < fvp.size(); ++i) {
      // cout << "TMPatchObject::updateForPatches() check corner " << fvp[i]->getID() << endl;
      // cout << "--->setDS2Coord0()" << endl;
      fvp[i]->setDS2Coord0(p[i]);
    }

  }
}

void TMPatchObject::for_each(void (TMPatchFace::*func)(void)) {
  for( it = patch_list.begin(); it != patch_list.end(); it++ ) {
    TMPatchFacePtr pfp = *it;
    (pfp->*func)();
  }
}

// Free the memory allocated for the patches
void TMPatchObject::destroyPatches() {
	TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
	TMPatchFacePtr pfp = NULL;
	while ( first != last ) {
		pfp = (*first); ++first;
        pfp->destroy();
		delete pfp;
        pfp=NULL;
	}
	patch_list.clear();
    patchMap.clear();
}

// Build the list of patch faces
void TMPatchObject::createPatches( const struct PatchParameters &parameters, DLFLObjectPtr obj ) {
  destroyPatches();

  DLFLFacePtrList face_list = obj->getFaceList( );
	DLFLFacePtrList::iterator ffirst = face_list.begin(), flast = face_list.end();
	DLFLFacePtr fp = NULL;
	TMPatchFacePtr pfp = NULL;

	while ( ffirst != flast ) {
		fp = (*ffirst); ++ffirst;
		pfp = new TMPatchFace(patchsize);
		pfp->setDLFLFace(fp); 
        pfp->createPatches(patchMap, parameters);
		patch_list.push_back(pfp);
	}
}

void TMPatchObject::setPatchSize( const struct PatchParameters &parameters, DLFLObjectPtr obj ){
  if ( parameters.grid_size != patchsize && parameters.grid_size > 0 ) {
    patchsize = parameters.grid_size;
    if( !obj ) { obj = mObj; }
    if( !obj ) { return; }
    createPatches( parameters, obj );
  }
}

/* stuart - bezier export */
void TMPatchObject::objPatchWrite( ostream& o ) {
  o << "g patches" << std::endl
    << "mg 1 0.5"  << std::endl << std::endl;
  TMPatchFacePtrList::const_iterator first = patch_list.begin(), last = patch_list.end();
  int i = 0;
  int v = 0;
  while( first != last ) {
    o << "# Face " << i+1 << std::endl;
    int npatches = (*first)->print(o);
    int j = 0;
    while( j < npatches ) {
      o << "# Patch" << i+j+1 << std::endl;
      o << "cstype bezier" << std::endl
	<< "deg 3 3" << std::endl
	<< "surf 0.000000 1.000000 0.000000 1.000000 " 
	<< 1+v << " " << 2+v << " " << 3+v << " " << 4+v << " " << 5+v << " " << 6+v << " " << 7+v << " " << 8+v << " " << 9+v << " " << 10+v << " " << 11+v << " " << 12+v << " " << 13+v << " " << 14+v << " " << 15+v << " " << 16+v << std::endl
	<< "parm u 0.000000 1.000000" << std::endl
	<< "parm v 0.000000 1.000000" << std::endl
	<< "end" << std::endl << std::endl;
      j++;
      v += 16;
    }
    ++first;
    i++;
  }
}
