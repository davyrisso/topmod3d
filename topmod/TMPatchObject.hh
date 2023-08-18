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

#ifndef _TM_PATCH_OBJECT_HH_
#define _TM_PATCH_OBJECT_HH_

#include <QtCore>
//#include "MainWindow.hh"
#include "TMPatchFace.hh"
#include "patchmode.hh"
//typedef MainWindow* MainWindowPtr;
class TMPatchObject;
typedef TMPatchObject* TMPatchObjectPtr;

typedef vector<Vector3dArray> PolygonArray;

class TMPatchObject {

protected :

 // MainWindowPtr mainWindow;
  uint uid;
  TMPatchFacePtrList patch_list;		// List of patch faces
  TMPatchFacePtrList::iterator it;
  int patchsize;				 // Size of each patch

public :

  // Default constructor
  TMPatchObject( uint id ) : uid(id), patch_list(), patchsize(4) { }

  uint id( ) { return uid; }
  int size( ) { return patchsize; }
  const TMPatchFacePtrList& list( ) { return patch_list; }
  void for_each( void (TMPatchFace::*func)(void));

	/* stuart - bezier export */
	void objPatchWrite( ostream& o );

private :

  // Copy constructor - private to prevent access
  TMPatchObject(const TMPatchObject& tmpo)
    : patch_list(), patchsize(tmpo.patchsize) { }

  // Assignment operator - private to prevent access
  void operator=(const TMPatchObject& tmpo) { }
       
  PolygonArray dooSabinPolygons, modifiedDooSabinPolygons;


public :     
  // Destructor
  ~TMPatchObject() {
      destroyPatches();

  }
  // Free the memory allocated for the patches
  void destroyPatches();

protected :
  DLFLObjectPtr mObj; // the last obj created from

	TMPatchMap patchMap;



  // Build the list of patch faces
  void createPatches( const struct PatchParameters &parameters, DLFLObjectPtr obj );

public :

  // Set the patch size
  void setPatchSize(const struct PatchParameters &parameters, DLFLObjectPtr obj = NULL );

private:
  void updateForPatches( const struct PatchParameters &parameters, DLFLObjectPtr obj );

public:
  // Update the patches
  void updatePatches(const struct PatchParameters &parameters, DLFLObjectPtr obj = NULL ) {
    if( !obj ) { obj = mObj; }
    if( !obj ) { return; } // never set an obj to update
    updateForPatches(parameters, obj);
    createPatches(parameters, obj);
  }

private:
  static bool RenderIncludesPatchEdges;
  static bool RenderIncludesDooSabinPolygons;
  static bool RenderIncludesModifiedDooSabinPolygons;

public:
  static void SetRenderIncludesPatchEdges(bool value);
  static void SetRenderIncludesDooSabinPolygons(bool value);
  static void SetRenderIncludesModifiedDooSabinPolygons(bool value);

  static bool GetRenderIncludesPatchEdges();
  static bool GetRenderIncludesDooSabinPolygons();
  static bool GetRenderIncludesModifiedDooSabinPolygons();

  void renderDooSabinPolygons(void){
      glPushMatrix();
      transform();
      PolygonArray::iterator begin = dooSabinPolygons.begin(), end = dooSabinPolygons.end();
      while (begin != end) {
          if (begin->size() >0) {
              glBegin(GL_LINE_LOOP);
              for (int i = 0; i < begin->size(); ++i) {
                  glVertex3dv(begin->at(i).getCArray());
              }
              glEnd();
          }
          begin++;
      }
      glPopMatrix();
  }

  void renderModifiedDooSabinPolygons(void){
      glPushMatrix();
      transform();
      PolygonArray::iterator begin = modifiedDooSabinPolygons.begin(), end = modifiedDooSabinPolygons.end();
      while (begin != end) {
          if (begin->size() >0) {
              glBegin(GL_LINE_LOOP);
              for (int i = 0; i < begin->size(); ++i) {
                  glVertex3dv(begin->at(i).getCArray());
              }
              glEnd();
          }
          begin++;
      }
      glPopMatrix();
  }

  // Render the patches
  void renderPatches(void) {
    glPushMatrix();
    transform();
    for_each(&TMPatchFace::render);
    glPopMatrix();
  }

  void renderControlMeshExternalEdges(){
      glPushMatrix();
      transform();
      for_each(&TMPatchFace::renderControlMeshExternalEdges);
      glPopMatrix();
  }

  void renderControlMeshInternalEdges(){
      glPushMatrix();
      transform();
      for_each(&TMPatchFace::renderControlMeshInternalEdges);
      glPopMatrix();
  }

  // Render the object using wireframe patches
  void renderWireframePatches(void) {
    glPushMatrix();
    transform();
    //for_each(face_list.begin(), face_list.end(), outline);
    for_each(&TMPatchFace::outline);
    glPopMatrix();
  }

  // Render the object using point patches
  void renderPointPatches(void) {
    glPushMatrix();
    transform();
    //for_each(face_list.begin(), face_list.end(), controlpoints);
    for_each(&TMPatchFace::controlpoints);
    glPopMatrix();
  }

  void renderPatchBoundaries(void) {
    glPushMatrix();
    transform();
    //for_each(patch_list.begin(), patch_list.end(), tmpf_patch_boundary);
    for_each(&TMPatchFace::patch_boundary);
    glPopMatrix();
  }

  void renderPatchFaceBoundaries(void) {
    glPushMatrix();
    transform();
    //for_each(patch_list.begin(), patch_list.end(), tmpf_face_boundary);
    for_each(&TMPatchFace::face_boundary);
    glPopMatrix();
  }

  void renderPatchNormals(void) {
    glPushMatrix();
    transform();
    //for_each(patch_list.begin(), patch_list.end(), tmpf_render_normals);
    for_each(&TMPatchFace::renderNormals);
    glPopMatrix();
  }

  // Compute lighting for patches and the base object
  void computeLighting(DLFLObjectPtr obj, LightPtr lightptr) {
    cout << "TMPatchObject::computeLighting" << endl;
    computeLighting(obj,lightptr);
    TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while ( first != last ) {
      pfp = (*first); ++first;
      pfp->computeLighting(lightptr);
    }
  }

  /* stuart - bezier export */
  void printPatchCVList( ) const {
    cout << "Patch CVs" << endl;
    TMPatchFacePtrList::const_iterator first = patch_list.begin(), last = patch_list.end();
    while( first != last ) {
      (*first)->print(cout);
      ++first;
    }
  }

	void transform( ) {
		double mat[16];
		mObj->tr.fillArrayColumnMajor(mat);
		glMultMatrixd(mat);		
	}

};


#endif /* #ifndef _TM_PATCH_OBJECT_HH_ */

