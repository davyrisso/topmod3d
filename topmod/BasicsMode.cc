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

/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>
#include <algorithm>
#include <queue>

#include "BasicsMode.hh"

/*!
	\ingroup gui
	@{
	
	\class BasicsMode
	\brief Basics Operations including insert-edge and delete-edge.
	
	\note 
	
	\see BasicsMode
*/


// This function should be a member function of the super class for all tools.
QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
	// label = new QLabel(this);(s);
	label->setText(s);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox;
	spinbox->setAccelerated(true);
	spinbox->setRange(low, high);
	spinbox->setSingleStep(step);
	spinbox->setValue(value);
	spinbox->setDecimals(decimals);
	spinbox->setMaximumSize(75,25);
	layout->addWidget(label,row,col);
  layout->addWidget(spinbox,row,col+1);

	return spinbox;
}

// Implementation of the class GeometricTool.
// We should move it to a file named geometric_tool.cc later.
// Constructor.
GeometricTool::GeometricTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
	layout_->setHorizontalSpacing(1);

  translation_x_label_ = new QLabel(parent_);
  translation_x_spin_ = createDoubleSpinBox(
      layout_, translation_x_label_, tr("X-translate"),
      -100.0, 100.0, 0.1, 0.0, 3, 1, 0);
  connect(translation_x_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  translation_y_label_ = new QLabel(parent_);
  translation_y_spin_ = createDoubleSpinBox(
      layout_, translation_y_label_, tr("Y-translate"),
      -100.0, 100.0, 0.1, 0.0, 3, 2, 0);
  connect(translation_y_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  translation_z_label_ = new QLabel(parent_);
  translation_z_spin_ = createDoubleSpinBox(
      layout_, translation_z_label_, tr("Z-translate"),
      -100.0, 100.0, 0.1, 0.0, 3, 3, 0);
  connect(translation_z_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  rotation_x_label_ = new QLabel(parent_);
  rotation_x_spin_ = createDoubleSpinBox(
      layout_, rotation_x_label_, tr("X-Rotate"),
      0, 360, 10, 0.0, 0, 4, 0);
  connect(rotation_x_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  rotation_y_label_ = new QLabel(parent_);
  rotation_y_spin_ = createDoubleSpinBox(
      layout_, rotation_y_label_, tr("Y-Rotate"),
      0, 360, 10, 0.0, 0, 5, 0);
  connect(rotation_y_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  rotation_z_label_ = new QLabel(parent_);
  rotation_z_spin_ = createDoubleSpinBox(
      layout_, rotation_z_label_, tr("Z-Rotate"),
      0, 360, 10, 0.0, 0, 6, 0);
  connect(rotation_z_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  scaling_x_label_ = new QLabel(parent_);
  scaling_x_spin_ = createDoubleSpinBox(
      layout_, scaling_x_label_, tr("X-Scale"),
      0.1, 10.0, 0.1, 1.0, 3, 7, 0);
  connect(scaling_x_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  scaling_y_label_ = new QLabel(parent_);
  scaling_y_spin_ = createDoubleSpinBox(
      layout_, scaling_y_label_, tr("Y-Scale"),
      0.1, 10.0, 0.1, 1.0, 3, 8, 0);
  connect(scaling_y_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  scaling_z_label_ = new QLabel(parent_);
  scaling_z_spin_ = createDoubleSpinBox(
      layout_, scaling_z_label_, tr("Z-Scale"),
      0.1, 10.0, 0.1, 1.0, 3, 9, 0);
  connect(scaling_z_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  reset_button_ = new QPushButton(tr("&Reset"));
	connect(reset_button_, SIGNAL(clicked()), this, SLOT(Reset()));
  layout_->addWidget(reset_button_, 10, 0, 1, 2);

  apply_button_ = new QPushButton(tr("&Apply"));
	connect(apply_button_, SIGNAL(clicked()), this, SLOT(Apply()));
  layout_->addWidget(apply_button_, 11, 0, 1, 2);

  layout_->setRowStretch(11,1);
	layout_->setColumnStretch(2,1);
  widget_ = new QWidget;
	widget_->setWindowTitle(tr("Geometry Mode"));
	widget_->setLayout(layout_);	

  action_ = new QAction(tr("Geometry"), parent_);
	action_->setIcon(QIcon(":/images/transforms.png"));
	action_->setCheckable(true);
	action_->setStatusTip(tr("Enter Geometry Mode"));
	action_->setToolTip(tr("Geometry Mode"));
	connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));

  active_ = true;
}

// Do the action, show the changes on screen.
// TODO(fhzhang): get rid of the argument.
void GeometricTool::DoAction(double) {
  if (!active_) return;
  double translate_x = translation_x_spin_->value();
  double translate_y = translation_y_spin_->value();
  double translate_z = translation_z_spin_->value();
  Vector3d translate(translate_x, translate_y, translate_z);
  
  double scaling_x = scaling_x_spin_->value();
  double scaling_y = scaling_y_spin_->value();
  double scaling_z = scaling_z_spin_->value();
  Vector3d scaling(scaling_x, scaling_y, scaling_z);

  double rotate_x = rotation_x_spin_->value() * M_PI / 180;
  double rotate_y = rotation_y_spin_->value() * M_PI / 180;
  double rotate_z = rotation_z_spin_->value() * M_PI / 180;
  // cout << "Scaling is " << scaling << endl;

  GLWidget *glw = ((MainWindow*)parent_)->getActive();
  // Collect all select vertices.
  DLFLVertexPtrArray selected_vertices = glw->getSelectedVertices();
  DLFLEdgePtrArray selected_edges = glw->getSelectedEdges();
  DLFLFacePtrArray selected_faces = glw->getSelectedFaces();

  for (DLFLEdgePtrArray::iterator it = selected_edges.begin();
      it != selected_edges.end(); ++it) {
    DLFLVertexPtr u, v;
    (*it)->getVertexPointers(u, v);
    if (std::find(selected_vertices.begin(), selected_vertices.end(), u)
        == selected_vertices.end())
      selected_vertices.push_back(u);
    if (std::find(selected_vertices.begin(), selected_vertices.end(), v)
        == selected_vertices.end())
      selected_vertices.push_back(u);
  }

  for (DLFLFacePtrArray::iterator it = selected_faces.begin();
      it != selected_faces.end(); ++it) {
    DLFLFaceVertexPtr current = (*it)->front()->next();
    if (std::find(selected_vertices.begin(), selected_vertices.end(), (*it)->front()->getVertexPtr())
        == selected_vertices.end())
      selected_vertices.push_back((*it)->front()->getVertexPtr());
    while (current != (*it)->front()) {
      DLFLVertexPtr u = current->getVertexPtr();
      if (std::find(selected_vertices.begin(), selected_vertices.end(), u)
          == selected_vertices.end())
        selected_vertices.push_back(u);
      current = current->next();
    }
  }
  if (selected_vertices.size() <= 0) return;

  Vector3d centroid(0.0, 0.0, 0.0);
  for (DLFLVertexPtrArray::iterator it = selected_vertices.begin();
       it != selected_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    if (init_positions_.find(*it) == init_positions_.end()) {
      init_positions_[*it] = p;
    }
    centroid += p;
  }
  Vector3d real_centroid = centroid / selected_vertices.size() + translate;
  for (DLFLVertexPtrArray::iterator it = selected_vertices.begin();
       it != selected_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    // Translate them.
    p = init_positions_[*it] + translate;

    // Rotate them around the centroid.
    p = p * cos(rotate_x) + (1 - cos(rotate_x)) * (p * Vector3d(1, 0, 0)) * Vector3d(1, 0, 0) +
        sin(rotate_x) *  (p % Vector3d(1, 0, 0));
    p = p * cos(rotate_y) + (1 - cos(rotate_y)) * (p * Vector3d(0, 1, 0)) * Vector3d(0, 1, 0) +
        sin(rotate_y) *  (p % Vector3d(0, 1, 0));
    p = p * cos(rotate_z) + (1 - cos(rotate_z)) * (p * Vector3d(0, 0, 1)) * Vector3d(0, 0, 1) +
        sin(rotate_z) *  (p % Vector3d(0, 0, 1));
    
    // Scale them around the centroid.
    p = real_centroid + product(p - real_centroid, scaling);
    // Update the coordinates.
    (*it)->setCoords(p);
  }
  glw->recomputeNormals();
  ((MainWindow*)parent_)->redraw();
}

// Apply the current changes.
void GeometricTool::Apply() {
  // Duplicate the current selected connected component.
  // Move it out after it works.

  GLWidget *glw = ((MainWindow*)parent_)->getActive();
  DLFLObjectPtr object = ((MainWindow*)parent_)->GetObject();
  ((MainWindow*)parent_)->saveFile();
  // Collect all selected vertices.
  DLFLVertexPtrSet old_vertices;
  DLFLEdgePtrSet old_edges;
  DLFLFacePtrSet old_faces;

  DLFLVertexPtrArray selected_vertices = glw->getSelectedVertices();
  DLFLEdgePtrArray selected_edges = glw->getSelectedEdges();
  DLFLFacePtrArray selected_faces = glw->getSelectedFaces();

  if (selected_vertices.size() > 0) {
    old_vertices.insert(*selected_vertices.begin());
  } else if (selected_edges.size() > 0) {
    DLFLVertexPtr u, v;
    (*selected_edges.begin())->getVertexPointers(u, v);
    old_vertices.insert(u);
  } else if (selected_faces.size() > 0) {
    DLFLFaceVertexPtr current = (*selected_faces.begin())->front()->next();
    old_vertices.insert((*selected_faces.begin())->front()->getVertexPtr());
  }

  //get all faces, edges, and vertices in the connected component.
  std::queue<DLFLVertexPtr> Q;
  Q.push(*old_vertices.begin());

  while (!Q.empty()) {
    DLFLVertexPtr u = Q.front();
    Q.pop();
    DLFLEdgePtrArray edges;
    u->getEdges(edges);
    //loop through edges, select all vertices connected to these edges
    for(DLFLEdgePtrArray::iterator eit = edges.begin();
        eit != edges.end(); eit++) {
      old_edges.insert(*eit);
      DLFLFacePtr face1, face2;
      (*eit)->getFacePointers(face1, face2);
      old_faces.insert(face1);
      old_faces.insert(face2);

      DLFLVertexPtr vp1, vp2;
      (*eit)->getVertexPointers(vp1,vp2);
      if (old_vertices.count(vp1) <= 0) {
        old_vertices.insert(vp1);
        Q.push(vp1);
      }
      if (old_vertices.count(vp2) <= 0) {
        old_vertices.insert(vp2);
        Q.push(vp2);
      }
    }
  }
  /*
  Vector3d centroid(0.0, 0.0, 0.0);
  for (DLFLVertexPtrSet::iterator it = old_vertices.begin();
       it != old_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    centroid += p;
  }
  centroid / = old_vertices.size();
  // Vector3d real_centroid = centroid / selected_vertices.size() + translate;
  Vertor3d translate = centroid + Vertor3d(4, 4, 4);
  */
  Vector3d translate(4, 4, 4);

  // Now we have all the vertices, edges and faces.
  // Add vertices first, we need a map to reconstruct the new component.
  map<DLFLVertexPtr, DLFLVertexPtr> old2new_vertices;
  for (DLFLVertexPtrSet::iterator it = old_vertices.begin();
       it != old_vertices.end(); ++it) {
    DLFLVertexPtr new_vertex = new DLFLVertex(translate + (*it)->getCoords());
    object->addVertexPtr(new_vertex);
    old2new_vertices[*it] = new_vertex;
  }
  cout << old_faces.size() << " face to be duplicated." << endl;

  // Trace all faces to insert edges and corners.
  for (DLFLFacePtrSet::iterator it = old_faces.begin();
      it != old_faces.end(); ++it) {
    DLFLFacePtr face = *it;

    DLFLFaceVertexPtr face_vertex = face->front();
    if (face_vertex == NULL) continue; // Ignore empty faces.
    DLFLFacePtr new_face = new DLFLFace;

    // New vertices.
    DLFLVertexPtr u, v;
    // New corner to create.
    DLFLFaceVertexPtr u_corner;
    do {
      u = old2new_vertices[face_vertex->getVertexPtr()];
      v = old2new_vertices[face_vertex->next()->getVertexPtr()];
      cout << "Looking at corner (" << u->getID() << "," << v->getID() << ")" << endl;

      // Create corners for new_v.
      u_corner = new DLFLFaceVertex;
      u_corner->vertex = u;
      new_face->addVertexPtr(u_corner);
      DLFLEdgePtr new_edge = u->getEdgeTo(v);
      if (new_edge == NULL) { // Insert the new edge.
        new_edge = new DLFLEdge;
        // Does the order matter?
        // new_edge->setFaceVertexPointers(u_corner, v_corner);
        new_edge->setFaceVertexPtr1(u_corner, false);
      } else {
        new_edge->setFaceVertexPtr2(u_corner, false);
        new_edge->updateFaceVertices();
      }
      object->addEdgePtr(new_edge);

      face_vertex = face_vertex->next();
    } while (face_vertex != face->front());

    new_face->setMaterial(face->material());
    new_face->updateFacePointers();
    new_face->addFaceVerticesToVertices();
    object->addFacePtr(new_face);
  }


  /*
  // Translate them.
  for (DLFLVertexPtrArray::iterator it = selected_vertices.begin();
       it != selected_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    p = init_positions_[*it] + translate;

    // Update the coordinates.
    (*it)->setCoords(p);
  }
  */
  
  // ((MainWindow*)parent_)->saveFile();
  cout << "duplicate done, update the GUI" << endl;
  // recomputeNormals() crashes.
  glw->recomputeNormals();
  ((MainWindow*)parent_)->redraw();

  /*
  active_ = false;
  init_positions_.clear();
  Reset();
  active_ = true;
  */
}
// Discard the current changes.
void GeometricTool::Reset() {
  translation_x_spin_->setValue(0.0);
  translation_y_spin_->setValue(0.0);
  translation_z_spin_->setValue(0.0);
  rotation_x_spin_->setValue(0.0);
  rotation_y_spin_->setValue(0.0);
  rotation_z_spin_->setValue(0.0);
  scaling_x_spin_->setValue(1.0);
  scaling_y_spin_->setValue(1.0);
  scaling_z_spin_->setValue(1.0);
}
// This is called when the user activate this tool.
void GeometricTool::Activate() {
  ((MainWindow*)parent_)->setToolOptions(widget_);
	((MainWindow*)parent_)->setMode(MainWindow::NormalMode);
}
/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
BasicsMode::BasicsMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent) {
		
	setParent(0);
	mParent = parent;
  geometric_tool_ = new GeometricTool(parent);
	actionList->addAction(geometric_tool_->action_);	
	
	//here we set the default mode for when the application is executed.
	// ((MainWindow*)mParent)->setMode(MainWindow::InsertEdge);
	
	mInsertEdgeWidget = new QWidget;
  mDeleteEdgeWidget = new QWidget;
  mCollapseEdgeWidget = new QWidget;
  mSubdivideEdgeWidget = new QWidget;
  mConnectEdgesWidget = new QWidget;
  mSpliceCornersWidget = new QWidget;
  // mTransformsWidget = new QWidget;
  mSelectionOptionsWidget = new QWidget;

	//each mode widget will each be added to the 
	//ToolOptionsDockWidget of the MainWindow class
	setupInsertEdge();
	setupDeleteEdge();
	setupCollapseEdge();
	setupSubdivideEdge();
	setupConnectEdges();
	setupSpliceCorners();
	// setupTransforms();
	setupSelectionOptions();
	

	mInsertEdgeAction = new QAction(tr("Insert Edge"),this);
	mInsertEdgeAction->setIcon(QIcon(":/images/insert_edge.png"));
	mInsertEdgeAction->setCheckable(true);
	// mInsertEdgeAction->setChecked(true);
	// sm->registerAction(mInsertEdgeAction, "Basics Modes", "9");
	mInsertEdgeAction->setStatusTip(tr("Enter Insert Edge Mode"));
	mInsertEdgeAction->setToolTip(tr("Insert Edge Mode"));
	connect(mInsertEdgeAction, SIGNAL(triggered()), this, SLOT(triggerInsertEdge()));
	connect(mInsertEdgeAction, SIGNAL(hovered()), ((MainWindow*)mParent), SLOT(setAnimatedHelpImage()));
	actionList->addAction(mInsertEdgeAction);	

	mDeleteEdgeAction = new QAction(tr("Delete Edge"),this);
	mDeleteEdgeAction->setIcon(QIcon(":/images/delete_edge.png"));
	mDeleteEdgeAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mDeleteEdgeAction->setStatusTip(tr("Enter Delete Edge Mode"));
	mDeleteEdgeAction->setToolTip(tr("Delete Edge Mode"));
	connect(mDeleteEdgeAction, SIGNAL(triggered()), this, SLOT(triggerDeleteEdge()));
	actionList->addAction(mDeleteEdgeAction);

	mCollapseEdgeAction = new QAction(tr("Collapse Edge"),this);
	mCollapseEdgeAction->setIcon(QIcon(":/images/collapse_edge.png"));
	mCollapseEdgeAction->setCheckable(true);
	// sm->registerAction(mCollapseEdgeAction, "Basics Modes", "9");
	mCollapseEdgeAction->setStatusTip(tr("Enter Collapse Edge Mode"));
	mCollapseEdgeAction->setToolTip(tr("Collapse Edge Mode"));
	connect(mCollapseEdgeAction, SIGNAL(triggered()), this, SLOT(triggerCollapseEdge()));
	actionList->addAction(mCollapseEdgeAction);

	mSubdivideEdgeAction = new QAction(tr("Subdivide Edge"),this);
	mSubdivideEdgeAction->setIcon(QIcon(":/images/subdivide_edge.png"));
	mSubdivideEdgeAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mSubdivideEdgeAction->setStatusTip(tr("Enter Subdivide Edge Mode"));
	mSubdivideEdgeAction->setToolTip(tr("Subdivide Edge Mode"));
	connect(mSubdivideEdgeAction, SIGNAL(triggered()), this, SLOT(triggerSubdivideEdge()));
	actionList->addAction(mSubdivideEdgeAction);
	
	mConnectEdgesAction = new QAction(tr("Connect Edges"),this);
	mConnectEdgesAction->setIcon(QIcon(":/images/connect_edges.png"));
	mConnectEdgesAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mConnectEdgesAction->setStatusTip(tr("Enter Connect Edges Mode"));
	mConnectEdgesAction->setToolTip(tr("Connect Edges Mode"));
	connect(mConnectEdgesAction, SIGNAL(triggered()), this, SLOT(triggerConnectEdges()));
	actionList->addAction(mConnectEdgesAction);
	
	mSpliceCornersAction = new QAction(tr("Splice Corners"),this);
	mSpliceCornersAction->setIcon(QIcon(":/images/splice_corners.png"));
	mSpliceCornersAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mSpliceCornersAction->setStatusTip(tr("Enter Splice Corners Mode"));
	mSpliceCornersAction->setToolTip(tr("Splice Corners Mode"));
	connect(mSpliceCornersAction, SIGNAL(triggered()), this, SLOT(triggerSpliceCorners()));
	actionList->addAction(mSpliceCornersAction);

  /*
	mTransformsAction = new QAction(tr("Transforms"),this);
	mTransformsAction->setIcon(QIcon(":/images/transforms.png"));
	mTransformsAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mTransformsAction->setStatusTip(tr("Enter Transforms Mode"));
	mTransformsAction->setToolTip(tr("Transforms Mode"));
	connect(mTransformsAction, SIGNAL(triggered()), this, SLOT(triggerTransforms()));
	actionList->addAction(mTransformsAction);
  */

	mSelectionOptionsAction = new QAction(tr("Selection Options"),this);
	mSelectionOptionsAction->setIcon(QIcon(":/images/selection-options.png"));
	mSelectionOptionsAction->setCheckable(true);
	mSelectionOptionsAction->setStatusTip(tr("Enter Selection Options Mode"));
	mSelectionOptionsAction->setToolTip(tr("Selection Options Mode"));
	connect(mSelectionOptionsAction, SIGNAL(triggered()), this, SLOT(triggerSelectionOptions()));
	actionList->addAction(mSelectionOptionsAction);
		
}

QMenu* BasicsMode::getMenu(){
	mBasicsMenu = new QMenu(tr("Basics"));
	
	mBasicsMenu->addAction(mInsertEdgeAction);
	mBasicsMenu->addAction(mDeleteEdgeAction);	
	mBasicsMenu->addAction(mCollapseEdgeAction);	
	mBasicsMenu->addAction(mSubdivideEdgeAction);
	mBasicsMenu->addAction(mConnectEdgesAction);	
	mBasicsMenu->addAction(mSpliceCornersAction);
	// mBasicsMenu->addAction(mTransformsAction);
	mBasicsMenu->addAction(geometric_tool_->action_);
	mBasicsMenu->addAction(mSelectionOptionsAction);
	
	return mBasicsMenu;
}

void BasicsMode::triggerInsertEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mInsertEdgeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::InsertEdge);
}

void BasicsMode::triggerDeleteEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mDeleteEdgeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::DeleteEdge);
}

void BasicsMode::triggerCollapseEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mCollapseEdgeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::CollapseEdge);
}

void BasicsMode::triggerSubdivideEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mSubdivideEdgeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SubdivideEdge);
}

void BasicsMode::triggerConnectEdges(){
	
	((MainWindow*)mParent)->setToolOptions(mConnectEdgesWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ConnectEdges);
}

void BasicsMode::triggerSpliceCorners(){
	
	((MainWindow*)mParent)->setToolOptions(mSpliceCornersWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SpliceCorners);
}

/*
void BasicsMode::triggerTransforms(){
	
	((MainWindow*)mParent)->setToolOptions(mTransformsWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}
*/

void BasicsMode::triggerSelectionOptions(){
	
	((MainWindow*)mParent)->setToolOptions(mSelectionOptionsWidget);
	// ((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void BasicsMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mInsertEdgeAction);	
	actionGroup->addAction(mDeleteEdgeAction);	
	actionGroup->addAction(mCollapseEdgeAction);	
	actionGroup->addAction(mSubdivideEdgeAction);	
	actionGroup->addAction(mConnectEdgesAction);	
	actionGroup->addAction(mSpliceCornersAction);
	// actionGroup->addAction(mTransformsAction);
	actionGroup->addAction(mSelectionOptionsAction);
	actionGroup->addAction(geometric_tool_->action_);
		
	toolBar->addAction(mInsertEdgeAction);
	toolBar->addAction(mDeleteEdgeAction);	
	toolBar->addAction(mCollapseEdgeAction);	
	toolBar->addAction(mSubdivideEdgeAction);	
	toolBar->addAction(mConnectEdgesAction);	
	toolBar->addAction(mSpliceCornersAction);
	// toolBar->addAction(mTransformsAction);
	toolBar->addAction(mSelectionOptionsAction);
	toolBar->addAction(geometric_tool_->action_);
	
	stackedWidget->addWidget(mInsertEdgeWidget);
	stackedWidget->addWidget(mDeleteEdgeWidget);	
	stackedWidget->addWidget(mCollapseEdgeWidget);	
	stackedWidget->addWidget(mSubdivideEdgeWidget);	
	stackedWidget->addWidget(mConnectEdgesWidget);	
	stackedWidget->addWidget(mSpliceCornersWidget);
	// stackedWidget->addWidget(mTransformsWidget);
	stackedWidget->addWidget(mSelectionOptionsWidget);
	stackedWidget->addWidget(geometric_tool_->widget_);

}

QDoubleSpinBox *BasicsMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
	// label = new QLabel(this);(s);
	label->setText(s);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox;
	spinbox->setAccelerated(true);
	spinbox->setRange(low, high);
	spinbox->setSingleStep(step);
	spinbox->setValue(value);
	spinbox->setDecimals(decimals);
	spinbox->setMaximumSize(75,25);
	layout->addWidget(label,row,col);
  layout->addWidget(spinbox,row,col+1);

	return spinbox;
}

void BasicsMode::setupInsertEdge() {
	
	mInsertEdgeLayout = new QGridLayout;
	// mInsertEdgeLayout->setMargin(0);
	noOptionsInsertEdgeLabel = new QLabel(this);(tr("No Options for this tool."));
	mInsertEdgeLayout->addWidget(noOptionsInsertEdgeLabel,0,0);
	
	mInsertEdgeLayout->setRowStretch(4,1);
	mInsertEdgeLayout->setColumnStretch(2,1);
	mInsertEdgeWidget->setWindowTitle(tr("Insert Edge"));
	mInsertEdgeWidget->setLayout(mInsertEdgeLayout);	
}

void BasicsMode::setupDeleteEdge() {
	
	mDeleteEdgeLayout = new QGridLayout;
	// mDeleteEdgeLayout->setMargin(0);
	
	//cleanup checkbox
	cleanupDeleteEdgeCheckBox = new QCheckBox(tr("Cleanup"),this);
	connect(cleanupDeleteEdgeCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)mParent), SLOT(toggleDeleteEdgeCleanupFlag(int)));
	mDeleteEdgeLayout->addWidget(cleanupDeleteEdgeCheckBox,0,0);	
	
	mDeleteEdgeLayout->setRowStretch(1,1);
	mDeleteEdgeLayout->setColumnStretch(2,1);
	mDeleteEdgeWidget->setWindowTitle(tr("Delete Edge Mode"));
	mDeleteEdgeWidget->setLayout(mDeleteEdgeLayout);	
}

void BasicsMode::setupCollapseEdge() {
	
	mCollapseEdgeLayout = new QGridLayout;
	// mCollapseEdgeLayout->setMargin(0);
	noOptionsCollapseEdgeLabel = new QLabel(tr("No Options for this tool."), this);
	mCollapseEdgeLayout->addWidget(noOptionsCollapseEdgeLabel,0,0);
	
	mCollapseEdgeLayout->setRowStretch(4,1);
	mCollapseEdgeLayout->setColumnStretch(2,1);
	mCollapseEdgeWidget->setWindowTitle(tr("Collapse Edge"));
	mCollapseEdgeWidget->setLayout(mCollapseEdgeLayout);	
}

void BasicsMode::setupSubdivideEdge() {
	
	mSubdivideEdgeLayout = new QGridLayout;
	// mSubdivideEdgeLayout->setMargin(0);
	
	//number of subdivisions spinbox
	numSubdivsLabel = new QLabel(this);
	numSubdivsSpinBox = createDoubleSpinBox(mSubdivideEdgeLayout, numSubdivsLabel, tr("# Subdivisions"), 1, 10, 1, 1, 0, 0,0);
	connect(numSubdivsSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeNumSubDivs(double)));

	// mSubdivideEdgeLayout->addStretch(1);
	mSubdivideEdgeLayout->setRowStretch(1,1);
	mSubdivideEdgeLayout->setColumnStretch(2,1);
	mSubdivideEdgeWidget->setWindowTitle(tr("Subdivide Edge Mode"));
	mSubdivideEdgeWidget->setLayout(mSubdivideEdgeLayout);	
	
}

void BasicsMode::setupConnectEdges(){
	
	mConnectEdgesLayout = new QGridLayout;
	noOptionsConnectEdgesLabel = new QLabel(tr("No Options for this tool."), this);
	mConnectEdgesLayout->addWidget(noOptionsConnectEdgesLabel,0,0);
	
	mConnectEdgesLayout->setRowStretch(0,1);
	mConnectEdgesLayout->setColumnStretch(2,1);
	mConnectEdgesWidget->setWindowTitle(tr("Connect Edges"));
	mConnectEdgesWidget->setLayout(mConnectEdgesLayout);	
	
}

void BasicsMode::setupSpliceCorners(){
	
	mSpliceCornersLayout = new QGridLayout;
	// mSpliceCornersLayout->setMargin(0);
	mSpliceCornersLayout->setRowStretch(4,1);
	mSpliceCornersLayout->setColumnStretch(2,1);
	
	noOptionsSpliceCornersLabel = new QLabel(tr("No Options for this tool."),this);
	mSpliceCornersLayout->addWidget(noOptionsSpliceCornersLabel,0,0);
	
	mSpliceCornersWidget->setWindowTitle(tr("Splice Corners"));
	mSpliceCornersWidget->setLayout(mSpliceCornersLayout);
	
}

/*
void BasicsMode::setupTransforms(){
	
	mTransformsLayout = new QGridLayout;
	mTransformsLayout->setVerticalSpacing(1);
	mTransformsLayout->setHorizontalSpacing(1);
	// mTransformsLayout->setMargin(0);
	
	// transformLabel = new QLabel(tr("Translate:"));
	// mTransformsLayout->addWidget(transformLabel,0,0,1,2);
	
	xPosLabel = new QLabel(this);
	xPosSpinBox = createDoubleSpinBox(mTransformsLayout, xPosLabel, tr("X-translate"), -100.0, 100.0, 0.5, 0.0, 3, 1,0);
	connect(xPosSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(translatex(double)));
	
	yPosLabel = new QLabel(this);
	yPosSpinBox = createDoubleSpinBox(mTransformsLayout, yPosLabel, tr("Y-translate"), -100.0, 100.0, 0.5, 0.0, 3, 2,0);
	connect(yPosSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(translatey(double)));
	
	zPosLabel = new QLabel(this);
	zPosSpinBox = createDoubleSpinBox(mTransformsLayout, zPosLabel, tr("Z-translate"), -100.0, 100.0, 0.5, 0.0, 3, 3,0);
	connect(zPosSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(translatez(double)));
	
	// scaleLabel = new QLabel(this);(tr("Scale:"));
	// mTransformsLayout->addWidget(scaleLabel,4,0,1,2);
	
	//x scale
	xScaleLabel = new QLabel(this);
	xScaleSpinBox = createDoubleSpinBox(mTransformsLayout, xScaleLabel, tr("X-scale"), 0.1, 10.0, 0.1, 1.0, 3, 5,0);
	connect(xScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(scalex(double)));

	yScaleLabel = new QLabel(this);
	yScaleSpinBox = createDoubleSpinBox(mTransformsLayout, yScaleLabel, tr("Y-scale"), 0.1, 10.0, 0.1, 1.0, 3, 6,0);
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(scaley(double)));

 	zScaleLabel = new QLabel(this);
	zScaleSpinBox = createDoubleSpinBox(mTransformsLayout, zScaleLabel, tr("Z-scale"), 0.1, 10.0, 0.1, 1.0, 3, 7,0);
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(scalez(double)));
	
	freezeTransformsButton = new QPushButton(tr("&Freeze Transforms"));
	connect(freezeTransformsButton, SIGNAL(clicked()), this, SLOT(freezeTransforms()));
	mTransformsLayout->addWidget(freezeTransformsButton,8,0,1,2);

	mTransformsLayout->setRowStretch(9,1);
	mTransformsLayout->setColumnStretch(2,1);
	mTransformsWidget->setWindowTitle(tr("Transforms Mode"));
	mTransformsWidget->setLayout(mTransformsLayout);	
}


void BasicsMode::freezeTransforms()
{
	((MainWindow*)mParent)->freezeTransforms();
	xPosSpinBox->setValue(0.0);
	yPosSpinBox->setValue(0.0);
	zPosSpinBox->setValue(0.0);
	xScaleSpinBox->setValue(1.0);
	yScaleSpinBox->setValue(1.0);
	zScaleSpinBox->setValue(1.0);
}
*/

void BasicsMode::setupSelectionOptions(){
	
	mSelectionOptionsLayout = new QGridLayout;
	mSelectionOptionsLayout->setVerticalSpacing(1);
	mSelectionOptionsLayout->setHorizontalSpacing(1);
		
	mFaceAreaToleranceLabel = new QLabel(this);
	mFaceAreaToleranceSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, mFaceAreaToleranceLabel, tr("Face Area Sel.\nTolerance"), 0.0, 5.0, 0.001, 0.05, 3, 0,0);
	connect(mFaceAreaToleranceSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeFaceAreaTolerance(double)));

	mSplit2ValenceVertexOffsetLabel = new QLabel(this);
	mSplit2ValenceVertexOffsetSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, mSplit2ValenceVertexOffsetLabel, tr("Valence-2 Split\nOffset"), -0.1, 5.0, 0.001, -0.1, 3, 1,0);
	connect(mSplit2ValenceVertexOffsetSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeValence2SplitOffset(double)));
	
	// yPosSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, yPosLabel, tr("Y-translate"), -100.0, 100.0, 0.5, 0.0, 1, 2,0);
	// connect(yPosSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(translatey(double)));
	// 
	// zPosSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, zPosLabel, tr("Z-translate"), -100.0, 100.0, 0.5, 0.0, 1, 3,0);
	// connect(zPosSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(translatez(double)));
	// 
	// // scaleLabel = new QLabel(this);(tr("Scale:"));
	// // mSelectionOptionsLayout->addWidget(scaleLabel,4,0,1,2);
	// 
	// //x scale
	// xScaleSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, xScaleLabel, tr("X-scale"), 0.1, 10.0, 0.1, 1.0, 1, 5,0);
	// connect(xScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(scalex(double)));
	// 
	// yScaleSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, yScaleLabel, tr("Y-scale"), 0.1, 10.0, 0.1, 1.0, 1, 6,0);
	// connect(yScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(scaley(double)));
	// 
	// zScaleSpinBox = createDoubleSpinBox(mSelectionOptionsLayout, zScaleLabel, tr("Z-scale"), 0.1, 10.0, 0.1, 1.0, 1, 7,0);
	// connect(zScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(scalez(double)));
	// 
	// QPushButton *freezeTransformsButton = new QPushButton(tr("&Freeze Transforms"));
	// connect(freezeTransformsButton, SIGNAL(clicked()), this, SLOT(freezeTransforms()));
	// mSelectionOptionsLayout->addWidget(freezeTransformsButton,8,0,1,2);

	mSelectionOptionsLayout->setRowStretch(2,1);
	mSelectionOptionsLayout->setColumnStretch(2,1);
	mSelectionOptionsWidget->setWindowTitle(tr("Selection Options Mode"));
	mSelectionOptionsWidget->setLayout(mSelectionOptionsLayout);	
}

void BasicsMode::retranslateUi(){
	mInsertEdgeAction->setText(tr("Insert Edge"));
	mInsertEdgeAction->setStatusTip(tr("Enter Insert Edge Mode"));
	mInsertEdgeAction->setToolTip(tr("Insert Edge Mode"));
	mDeleteEdgeAction->setText(tr("Delete Edge"));
	mDeleteEdgeAction->setStatusTip(tr("Enter Delete Edge Mode"));
	mDeleteEdgeAction->setToolTip(tr("Delete Edge Mode"));
	mCollapseEdgeAction->setText(tr("Collapse Edge"));
	mCollapseEdgeAction->setStatusTip(tr("Enter Collapse Edge Mode"));
	mCollapseEdgeAction->setToolTip(tr("Collapse Edge Mode"));
	mSubdivideEdgeAction->setText(tr("Subdivide Edge"));
	mSubdivideEdgeAction->setStatusTip(tr("Enter Subdivide Edge Mode"));
	mSubdivideEdgeAction->setToolTip(tr("Subdivide Edge Mode"));
	mConnectEdgesAction->setText(tr("Connect Edges"));
	mConnectEdgesAction->setStatusTip(tr("Enter Connect Edges Mode"));
	mConnectEdgesAction->setToolTip(tr("Connect Edges Mode"));
	mSpliceCornersAction->setText(tr("Splice Corners"));
	mSpliceCornersAction->setStatusTip(tr("Enter Splice Corners Mode"));
	mSpliceCornersAction->setToolTip(tr("Splice Corners Mode"));
  /*
	mTransformsAction->setText(tr("Transforms"));
	mTransformsAction->setStatusTip(tr("Enter Transforms Mode"));
	mTransformsAction->setToolTip(tr("Transforms Mode"));
  */
	mSelectionOptionsAction->setText(tr("Selection Options"));
	mSelectionOptionsAction->setStatusTip(tr("Enter Selection Options Mode"));
	mSelectionOptionsAction->setToolTip(tr("Selection Options Mode"));
	mBasicsMenu->setTitle(tr("Basics"));

	//mode spinbox labels etc...
	noOptionsInsertEdgeLabel->setText(tr("No Options for this tool."));
	mInsertEdgeWidget->setWindowTitle(tr("Insert Edge"));
	cleanupDeleteEdgeCheckBox->setText(tr("Cleanup"));
	mDeleteEdgeWidget->setWindowTitle(tr("Delete Edge Mode"));
	noOptionsCollapseEdgeLabel->setText(tr("No Options for this tool."));
	mCollapseEdgeWidget->setWindowTitle(tr("Collapse Edge"));
	numSubdivsLabel->setText(tr("# Subdivisions"));
	mSubdivideEdgeWidget->setWindowTitle(tr("Subdivide Edge Mode"));
	noOptionsConnectEdgesLabel->setText(tr("No Options for this tool."));
	mConnectEdgesWidget->setWindowTitle(tr("Connect Edges"));
	noOptionsSpliceCornersLabel->setText(tr("No Options for this tool."));
	mSpliceCornersWidget->setWindowTitle(tr("Splice Corners"));
  /*
	xPosLabel->setText(tr("X-translate"));
	yPosLabel->setText(tr("Y-translate"));
	zPosLabel->setText(tr("Z-translate"));
	xScaleLabel->setText(tr("X-scale"));
	yScaleLabel->setText(tr("Y-scale"));
	zScaleLabel->setText(tr("Z-scale"));
	freezeTransformsButton->setText(tr("&Freeze Transforms"));
	mTransformsWidget->setWindowTitle(tr("Transforms Mode"));	
  */
	mFaceAreaToleranceLabel->setText(tr("Face Area Sel.\nTolerance"));
	mSplit2ValenceVertexOffsetLabel->setText(tr("Valence-2 Split\nOffset"));
}
