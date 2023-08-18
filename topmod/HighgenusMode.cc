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

#include <QtGui>

#include "HighgenusMode.hh"

/*!
	\ingroup gui
	@{
	
	\class HighgenusMode
	\brief High Genus Operations GUI elements.
	
	\note 
	
	\see HighgenusMode
*/

/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s,
    double low, double high, double step, double value, double decimals, int row, int col);

// Implementation of the class CreateTorusTool.
// We should move it to a file named create_torus_tool.cc later.
// Constructor.
CreateTorusTool::CreateTorusTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
	layout_->setHorizontalSpacing(1);

	QLabel *createTorusNumSegmentsLabel = new QLabel(this);
	num_segment_spin_ = createDoubleSpinBox(
      layout_, createTorusNumSegmentsLabel, 
      tr("# Segments"), 3, 100, 1, 8, 0, 0,0);
    connect(num_segment_spin_, SIGNAL(valueChanged(double)), (MainWindow*)parent_,
          SLOT(changeNumSegments(double)));

	QLabel *createTorusRadiusLabel = new QLabel(this);
	radius_spin_ = createDoubleSpinBox(
      layout_, createTorusRadiusLabel,
      tr("Radius"), 0.01, 10, 0.1, 4.0, 3, 1,0);	

	QLabel *createTorusSizeCrossSectionLabel = new QLabel(this);
	size_cross_section_spin_ = createDoubleSpinBox(
      layout_, createTorusSizeCrossSectionLabel,
      tr("#Vertices in CS\n(Cross Sectoin)"), 3, 64, 1, 8, 0, 2,0);
	connect(size_cross_section_spin_, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeSizeCrossSection(double)));

	QLabel *createTorusCrossSectionRadiusLabel = new QLabel(this);
	cross_section_radius_spin_ = createDoubleSpinBox(
      layout_, createTorusCrossSectionRadiusLabel,
      tr("Radius of CS"), 0.1, 5, 0.1, 2.0, 3, 3,0);	

	QLabel *createTorusCrossSectionRotationLabel = new QLabel(this);
	cross_section_rotation_spin_ = createDoubleSpinBox(
      layout_, createTorusCrossSectionRotationLabel,
      tr("Rotation of CS"), 0, 360, 10, 0, 0, 4,0);	

	QLabel *createTorusNumTwistLabel = new QLabel(this);
	num_twist_spin_ = createDoubleSpinBox(
      layout_, createTorusNumTwistLabel,
      tr("# Twists"), 0, 10, 1, 0, 0, 5, 0);	

	QLabel *createTorusNumPartialTwistLabel = new QLabel(this);
	num_partial_twist_spin_ = createDoubleSpinBox(
      layout_, createTorusNumPartialTwistLabel,
      tr("# Partial twist"), 0, 7, 1, 0, 0, 6, 0);	

	triangulate_new_faces_check_ = new QCheckBox(tr("Triangulate New Faces"), this);	
	connect(triangulate_new_faces_check_, SIGNAL(stateChanged(int)), this,
          SLOT(ToggleTriangulateNewFace(int)));
	layout_->addWidget(triangulate_new_faces_check_, 7, 0, 1, 2);

	scherk_collins_check_ = new QCheckBox(tr("Scherk Collins"), this);	
	connect(scherk_collins_check_, SIGNAL(stateChanged(int)), this,
          SLOT(ToggleScherkCollins(int)));
	layout_->addWidget(scherk_collins_check_, 8, 0, 1, 2);

	QLabel *createTorusPinchLabel = new QLabel(this);
	pinch_spin_ = createDoubleSpinBox(
      layout_, createTorusPinchLabel,
      tr("Pinch"), -1, 10, 0.1, 1.0, 2, 9, 0);	

	QLabel *createTorusPinchCenterLabel = new QLabel(this);
	pinch_center_spin_ = createDoubleSpinBox(
      layout_, createTorusPinchCenterLabel,
      tr("Pinch Center"), 0.1, 1, 0.1, 0.5, 3, 10, 0);	

	QLabel *createTorusPinchWidthLabel = new QLabel(this);
	pinch_width_spin_ = createDoubleSpinBox(
      layout_, createTorusPinchWidthLabel,
      tr("Pinch Width"), 0.001, 10, 0.1, 1.0, 3, 11, 0);	

	QLabel *createTorusHoleNumSegmentsLabel = new QLabel(this);
	hole_num_seg_spin_ = createDoubleSpinBox(
      layout_, createTorusHoleNumSegmentsLabel,
      tr("#segs in holes"), 1, 10, 1, 2.0, 0, 12, 0);	

	QLabel *createTorusHoleInitSkipLabel = new QLabel(this);
	hole_init_skip_spin_ = createDoubleSpinBox(
      layout_, createTorusHoleInitSkipLabel,
      tr("Init skip"), 0, 10, 1, 1.0, 0, 13, 0);	

	QLabel *createTorusHoleSkipLabel = new QLabel(this);
	hole_skip_spin_ = createDoubleSpinBox(
      layout_, createTorusHoleSkipLabel,
      tr("Skip"), 0, 10, 1, 0, 0, 14, 0);	

	QLabel *createTorusHoleTwistLabel = new QLabel(this);
	hole_twist_spin_ = createDoubleSpinBox(
      layout_, createTorusHoleTwistLabel,
      tr("Twist of holes"), 0, 10, 1, 1.0, 0, 15, 0);	

	QLabel *createTorusHoleSegSkipLabel = new QLabel(this);
	hole_seg_skip_spin_ = createDoubleSpinBox(
      layout_, createTorusHoleSegSkipLabel,
      tr("Skip in segments"), 0, 10, 1, 0.0, 0, 16, 0);	

  apply_button_ = new QPushButton(tr("&Create Torus"));
	connect(apply_button_, SIGNAL(clicked()), this, SLOT(Apply()));
  layout_->addWidget(apply_button_, 17, 0, 1, 2);

  layout_->setRowStretch(17,1);
	layout_->setColumnStretch(2,1);
  widget_ = new QWidget;
	widget_->setWindowTitle(tr("Create Torus"));
	widget_->setLayout(layout_);	

  action_ = new QAction(tr("Create Torus"), parent_);
	action_->setIcon(QIcon(":/images/prim_torus.png"));
	action_->setCheckable(true);
	action_->setStatusTip(tr("Enter Geometry Mode"));
	action_->setToolTip(tr("Geometry Mode"));
	connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));

  pinch_spin_->setEnabled(false);
  pinch_center_spin_->setEnabled(false);
  pinch_width_spin_->setEnabled(false);
  hole_num_seg_spin_->setEnabled(false);
 	hole_init_skip_spin_->setEnabled(false);
  hole_skip_spin_->setEnabled(false);
 	hole_twist_spin_->setEnabled(false);
  hole_seg_skip_spin_->setEnabled(false);
}

void CreateTorusTool::ToggleTriangulateNewFace(int state) {
  if ((bool)state) {
    scherk_collins_check_->setChecked(Qt::Unchecked);
  }
}

void CreateTorusTool::ToggleScherkCollins(int state) {
  if ((bool)state) {
    triangulate_new_faces_check_->setChecked(Qt::Unchecked);
    pinch_spin_->setEnabled(true);
    pinch_center_spin_->setEnabled(true);
    pinch_width_spin_->setEnabled(true);
	  hole_num_seg_spin_->setEnabled(true);
  	hole_init_skip_spin_->setEnabled(true);
	  hole_skip_spin_->setEnabled(true);
  	hole_twist_spin_->setEnabled(true);
	  hole_seg_skip_spin_->setEnabled(true);
  } else {
    pinch_spin_->setEnabled(false);
    pinch_center_spin_->setEnabled(false);
    pinch_width_spin_->setEnabled(false);
	  hole_num_seg_spin_->setEnabled(false);
  	hole_init_skip_spin_->setEnabled(false);
	  hole_skip_spin_->setEnabled(false);
  	hole_twist_spin_->setEnabled(false);
	  hole_seg_skip_spin_->setEnabled(false);
  }
}

void CreateTorusTool::ChangeSizeCrossSection(double value) {
	num_partial_twist_spin_->setRange(0, value - 1);
  if ((int)value % 2 != 0) {
    scherk_collins_check_->setEnabled(false);
    pinch_spin_->setEnabled(false);
    pinch_center_spin_->setEnabled(false);
    pinch_width_spin_->setEnabled(false);
    hole_num_seg_spin_->setEnabled(false);
  	hole_init_skip_spin_->setEnabled(false);
	  hole_skip_spin_->setEnabled(false);
  	hole_twist_spin_->setEnabled(false);
	  hole_seg_skip_spin_->setEnabled(false);
  } else {
    scherk_collins_check_->setEnabled(true);
    pinch_spin_->setEnabled(true);
    pinch_center_spin_->setEnabled(true);
    pinch_width_spin_->setEnabled(true);
	  hole_num_seg_spin_->setEnabled(true);
  	hole_init_skip_spin_->setEnabled(true);
	  hole_skip_spin_->setEnabled(true);
  	hole_twist_spin_->setEnabled(true);
	  hole_seg_skip_spin_->setEnabled(true);
  }
}

void CreateTorusTool::Activate() {
	((MainWindow*)parent_)->setToolOptions(widget_);
}

// This function contains the core logic for creating a torus. This logic
// should be moved to a class called CreateTorusOperation later.
void CreateTorusTool::Apply() {
  MainWindow *main_window = (MainWindow*)parent_;
  DLFLObjectPtr object = main_window->GetObject();

  GLWidget *active = main_window->getActive();

	active->createPatchObject( );
  // The next six parameters define properties of the torus to be created
  // Note: CS is an abbreviation for Cross Section
  int segmentCount = num_segment_spin_->value();
  int CS_VertCount = size_cross_section_spin_->value();
  int CS_Rotation = cross_section_rotation_spin_->value();
  float CS_Radius = cross_section_radius_spin_->value();
  float torusRadius = radius_spin_->value();
  int numTwists = num_twist_spin_->value();
  // numPartialTwists allows finer twisting control, allowed by
  // rotationally-symmetric cross sections.
  // <CS_VerCount> partial twists is equivalent to one full twist.
  int numPartialTwists = num_partial_twist_spin_->value();
  bool triangulate_new_faces = triangulate_new_faces_check_->isChecked();
  bool scherk_collins = scherk_collins_check_->isChecked();
  float scherk_collins_pinch = pinch_spin_->value();
  float scherk_collins_pinch_center = pinch_center_spin_->value();
  float scherk_collins_pinch_width = pinch_width_spin_->value();
  int scherk_collins_hole_init_skip = hole_init_skip_spin_->value();
  int scherk_collins_hole_skip = hole_skip_spin_->value();
  int scherk_collins_hole_seg_skip = hole_seg_skip_spin_->value();
  int scherk_collins_hole_num_segs = hole_num_seg_spin_->value();
  int scherk_collins_hole_twist = hole_twist_spin_->value();

	main_window->setModified(false);
	main_window->clearSelected();

  // these variables are used to communicate state between loop bodies
  vector<Vector3d> CS_VertList(CS_VertCount);

  // calculate vertex positions of a cross section centered at origin
  float ca = cos(CS_Rotation * M_PI / 360);
  float sa = sin(CS_Rotation * M_PI / 360);
  for (int i = 0; i < CS_VertCount; ++i) {
    float degree = 2 * M_PI * i / CS_VertCount;
    float y = CS_Radius * cos(degree);
    float z = CS_Radius * sin(degree);
    float y_real = y * ca - z * sa;
    float z_real = y * sa + z * ca;
    CS_VertList[i].set(0.0, y_real, z_real);
    // cout << CS_VertList[i] << endl;
  }

  // create cross section faces at regular intervals along torus
  object->reset();
  DLFLFacePtrArray basic_faces;
  DLFLVertexPtr first_vertex, prev_vertex, cur_vertex;
  for (int i = 0; i < segmentCount; ++i) {
    float c = cos(2 * M_PI * i / segmentCount);
    float s = sin(2 * M_PI * i / segmentCount);
    vector<Vector3d> CS_Trans(CS_VertCount);
    for (int j = 0; j < CS_VertCount; ++j) {
      // build about-to-be-transformed cross section vertex list
      CS_Trans[j].set(CS_VertList[j][0], CS_VertList[j][1], CS_VertList[j][2]);
      // rotate a bit more about x-axis to generate twist
      float degree = (numTwists + (float)numPartialTwists / CS_VertCount) * 2 * M_PI * i / segmentCount;
		  float cT = cos(degree);
		  float sT = sin(degree);
		  CS_Trans[j].set(CS_Trans[j][0],
                      CS_Trans[j][1] * cT + CS_Trans[j][2] * sT,
                      -CS_Trans[j][1] * sT + CS_Trans[j][2] * cT);
		  // translate cross section center by torus radius
		  CS_Trans[j].set(CS_Trans[j][0], CS_Trans[j][1], CS_Trans[j][2] + torusRadius);
		  // rotate cross section about z axis
      CS_Trans[j].set(CS_Trans[j][0] * c - CS_Trans[j][2] * s,
                      CS_Trans[j][1],
                      CS_Trans[j][0] * s + CS_Trans[j][2] * c);
    }
    DLFLFacePtrArray new_faces = object->createFace(CS_Trans);
    basic_faces.push_back(new_faces[0]);
    basic_faces.push_back(new_faces[1]);
  }
  // connect each of the cross section faces with connectCorners() to create the torus
  // Some thing might go wrong if we have odd number of faces.
  int twist = 0;
  int num_segments_processed = 0;
  for (int i = 0; i < basic_faces.size() / 2; ++i){
    DLFLFacePtr f1 = basic_faces[2 * i + 1];
    DLFLFacePtr f2 = basic_faces[(2 * i + 2) % basic_faces.size()];
    DLFLFaceVertexPtr fvp1 = f1->firstVertex();
    DLFLFaceVertexPtr fvp2 = f2->firstVertex();
    // dualConnectFaces(&object, f1->firstVertex(), f2->firstVertex());
    if ((2 * i + 2) % basic_faces.size() == 0)
      for (int j = 0; j < CS_VertCount - numPartialTwists; ++j) fvp1 = fvp1->next();
    if (triangulate_new_faces) dualConnectFaces(object, fvp1, fvp2);
    else {
        DLFLEdgePtrArray new_edges = connectFaces(object, fvp1, fvp2);
        if (scherk_collins && num_segments_processed % (scherk_collins_hole_seg_skip + 1) == 0) {
          ScherkCollinsHandle(object, new_edges, scherk_collins_pinch,
              scherk_collins_pinch_center, scherk_collins_pinch_width,
              twist, scherk_collins_hole_init_skip,
              scherk_collins_hole_skip, scherk_collins_hole_num_segs);
          twist += scherk_collins_hole_twist;
      }
    }
    ++num_segments_processed;
  }
  if (active->isInPatchMode()) {
    cout << "computing patches for torus..." << endl;
    active->recomputePatches(main_window->getPatchParameters());
  }
  active->recomputeNormals();
  active->redraw();
}



// The implementation of HighgenusMode starts here.

HighgenusMode::HighgenusMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
  create_torus_tool_ = new CreateTorusTool(parent);
  actionList->addAction(create_torus_tool_->action_);
	
	mAddHoleHandleWidget = new QWidget;
  mAddHoleHandleCVWidget = new QWidget;
  mAddHandleSIWidget = new QWidget;
  mRindModelingScalingWidget = new QWidget;
  mRindModelingThicknessWidget = new QWidget;
  mWireframeModelingWidget = new QWidget;
  mWireframeModeling2Widget = new QWidget;
  mColumnModelingWidget = new QWidget;
	mSierpinskyWidget = new QWidget;
	mMultiFaceHandleWidget = new QWidget;                
	mMengerSpongeWidget = new QWidget;  

	setupAddHoleHandle();
	setupAddHoleHandleCV();
	setupAddHandleSI();
	setupRindModelingScaling();
	setupRindModelingThickness();
	setupWireframeModeling();
	setupWireframeModeling2();
	setupColumnModeling();
	setupSierpinsky();
	setupMultiFaceHandle();
	setupMengerSponge();
	
	//setup the 10 modes for high genus modeling
	mAddHoleHandleAction = new QAction(QIcon(":images/highgenus_addholehandle.png"),tr("Add Hole/Handle"),this);
	mAddHoleHandleAction->setCheckable(true);
	sm->registerAction(mAddHoleHandleAction, "High Genus Operations", "");
	mAddHoleHandleAction->setStatusTip(tr("Enter Add Hole/Handle Mode"));
	mAddHoleHandleAction->setToolTip(tr("Add Hole/Handle Mode"));
	connect(mAddHoleHandleAction, SIGNAL(triggered()), this, SLOT(triggerAddHoleHandle()));
	actionList->addAction(mAddHoleHandleAction);

	mAddHoleHandleCVAction = new QAction(QIcon(":images/highgenus_addholehandlecv.png"),tr("Add Hole/Handle CV"),this);
	mAddHoleHandleCVAction->setCheckable(true);
	sm->registerAction(mAddHoleHandleCVAction, "High Genus Operations", "");
	mAddHoleHandleCVAction->setStatusTip(tr("Enter Add Hole/Handle CV Mode"));
	mAddHoleHandleCVAction->setToolTip(tr("Add Hole/Handle CV Mode"));
	connect(mAddHoleHandleCVAction, SIGNAL(triggered()), this, SLOT(triggerAddHoleHandleCV()));
	actionList->addAction(mAddHoleHandleCVAction);

	mAddHandleSIAction = new QAction(QIcon(":images/highgenus_addhandlesi.png"),tr("Add Handle SI"),this);
	mAddHandleSIAction->setCheckable(true);
	sm->registerAction(mAddHandleSIAction, "High Genus Operations", "");
	mAddHandleSIAction->setStatusTip(tr("Enter Add Handle (Shape Interpolation) Mode"));
	mAddHandleSIAction->setToolTip(tr("Add Handle (Shape Interpolation) Mode"));
	connect(mAddHandleSIAction, SIGNAL(triggered()), this, SLOT(triggerAddHandleSI()));
	actionList->addAction(mAddHandleSIAction);

	mRindModelingScalingAction = new QAction(QIcon(":images/highgenus_rindscaling.png"),tr("Rind Modeling Scaling"),this);
	mRindModelingScalingAction->setCheckable(true);
	sm->registerAction(mRindModelingScalingAction, "High Genus Operations", "");
	mRindModelingScalingAction->setStatusTip(tr("Enter Rind Modeling Scaling Mode"));
	mRindModelingScalingAction->setToolTip(tr("Rind Modeling Scaling Mode"));
	connect(mRindModelingScalingAction, SIGNAL(triggered()), this, SLOT(triggerRindModelingScaling()));
	actionList->addAction(mRindModelingScalingAction);

	mRindModelingThicknessAction = new QAction(QIcon(":images/highgenus_rindthickness.png"),tr("Rind Modeling Thickness"),this);
	mRindModelingThicknessAction->setCheckable(true);
	sm->registerAction(mRindModelingThicknessAction, "High Genus Operations", "");
	mRindModelingThicknessAction->setStatusTip(tr("Enter Rind Modeling Thickness Mode"));
	mRindModelingThicknessAction->setToolTip(tr("Rind Modeling Thickness Mode"));
	connect(mRindModelingThicknessAction, SIGNAL(triggered()), this, SLOT(triggerRindModelingThickness()));
	actionList->addAction(mRindModelingThicknessAction);

	mWireframeModelingAction = new QAction(QIcon(":images/highgenus_wireframe.png"),tr("Wireframe Modeling"),this);
	mWireframeModelingAction->setCheckable(true);
	sm->registerAction(mWireframeModelingAction, "High Genus Operations", "");
	mWireframeModelingAction->setStatusTip(tr("Enter Wireframe Modeling Mode"));
	mWireframeModelingAction->setToolTip(tr("Wireframe Modeling Mode"));
	connect(mWireframeModelingAction, SIGNAL(triggered()), this, SLOT(triggerWireframeModeling()));
	actionList->addAction(mWireframeModelingAction);

	mWireframeModeling2Action = new QAction(QIcon(":images/highgenus_wireframe.png"),tr("Wireframe Modeling 2"),this);
	mWireframeModeling2Action->setCheckable(true);
	sm->registerAction(mWireframeModeling2Action, "High Genus Operations", "");
	mWireframeModeling2Action->setStatusTip(tr("Enter Wireframe Modeling 2 Mode"));
	mWireframeModeling2Action->setToolTip(tr("Wireframe Modeling 2 Mode"));
	connect(mWireframeModeling2Action, SIGNAL(triggered()), this, SLOT(triggerWireframeModeling2()));
	actionList->addAction(mWireframeModeling2Action);

	mColumnModelingAction = new QAction(QIcon(":images/highgenus_column.png"),tr("Column Modeling"),this);
	mColumnModelingAction->setCheckable(true);
	sm->registerAction(mColumnModelingAction, "High Genus Operations", "");
	mColumnModelingAction->setStatusTip(tr("Enter Column Modeling Mode"));
	mColumnModelingAction->setToolTip(tr("Column Modeling Mode"));
	connect(mColumnModelingAction, SIGNAL(triggered()), this, SLOT(triggerColumnModeling()));
	actionList->addAction(mColumnModelingAction);

	mSierpinskyAction = new QAction(QIcon(":images/highgenus_sierpinsky.png"),tr("Sierpinsky"),this);
	mSierpinskyAction->setCheckable(true);
	sm->registerAction(mSierpinskyAction, "High Genus Operations", "");
	mSierpinskyAction->setStatusTip(tr("Enter Sierpinsky Mode"));
	mSierpinskyAction->setToolTip(tr("Sierpinsky Mode"));
	connect(mSierpinskyAction, SIGNAL(triggered()), this, SLOT(triggerSierpinsky()));
	actionList->addAction(mSierpinskyAction);

	mMultiFaceHandleAction = new QAction(QIcon(":images/highgenus_multifacehandle.png"),tr("Multi-face Handle"),this);
	mMultiFaceHandleAction->setCheckable(true);
	sm->registerAction(mMultiFaceHandleAction, "High Genus Operations", "");
	mMultiFaceHandleAction->setStatusTip(tr("Enter Multi-face Handle Mode"));
	mMultiFaceHandleAction->setToolTip(tr("Multi-face Handle Mode"));
	connect(mMultiFaceHandleAction, SIGNAL(triggered()), this, SLOT(triggerMultiFaceHandle()));
	actionList->addAction(mMultiFaceHandleAction);

	mMengerSpongeAction = new QAction(QIcon(":images/highgenus_mengersponge.png"),tr("Menger Sponge"),this);
	mMengerSpongeAction->setCheckable(true);
	sm->registerAction(mMengerSpongeAction, "High Genus Operations", "");
	mMengerSpongeAction->setStatusTip(tr("Enter Menger Sponge Mode"));
	mMengerSpongeAction->setToolTip(tr("Menger Sponge Mode"));
	connect(mMengerSpongeAction, SIGNAL(triggered()), this, SLOT(triggerMengerSponge()));
	actionList->addAction(mMengerSpongeAction);
}

QMenu* HighgenusMode::getMenu(){
	mHighgenusMenu = new QMenu(tr("High Genus"));
	
	mHighgenusMenu->addAction(create_torus_tool_->action_);
	mHighgenusMenu->addAction(mAddHoleHandleAction);
	mHighgenusMenu->addAction(mAddHoleHandleCVAction);
	mHighgenusMenu->addAction(mAddHandleSIAction);
	mHighgenusMenu->addAction(mRindModelingScalingAction); 
	mHighgenusMenu->addAction(mRindModelingThicknessAction);
	mHighgenusMenu->addAction(mWireframeModelingAction); 
	mHighgenusMenu->addAction(mWireframeModeling2Action); 
	mHighgenusMenu->addAction(mColumnModelingAction);	
	mHighgenusMenu->addAction(mSierpinskyAction);
	mHighgenusMenu->addAction(mMultiFaceHandleAction);
	mHighgenusMenu->addAction(mMengerSpongeAction);	
	
	return mHighgenusMenu;
	
}

void HighgenusMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(create_torus_tool_->action_);
	actionGroup->addAction(mAddHoleHandleAction);
	actionGroup->addAction(mAddHoleHandleCVAction);
	actionGroup->addAction(mAddHandleSIAction);
	actionGroup->addAction(mRindModelingScalingAction); 
	actionGroup->addAction(mRindModelingThicknessAction);
	actionGroup->addAction(mWireframeModelingAction); 
	actionGroup->addAction(mWireframeModeling2Action); 
	actionGroup->addAction(mColumnModelingAction);	
	actionGroup->addAction(mSierpinskyAction);
	actionGroup->addAction(mMultiFaceHandleAction);
	actionGroup->addAction(mMengerSpongeAction);
	
	toolBar->addAction(create_torus_tool_->action_);
	toolBar->addAction(mAddHoleHandleAction);
	toolBar->addAction(mAddHoleHandleCVAction);
	toolBar->addAction(mAddHandleSIAction);
	toolBar->addAction(mRindModelingScalingAction); 
	toolBar->addAction(mRindModelingThicknessAction);
	toolBar->addAction(mWireframeModelingAction); 
	toolBar->addAction(mWireframeModeling2Action); 
	toolBar->addAction(mColumnModelingAction);	
	toolBar->addAction(mSierpinskyAction);
	toolBar->addAction(mMultiFaceHandleAction);
	toolBar->addAction(mMengerSpongeAction);
	
	stackedWidget->addWidget(create_torus_tool_->widget_);
	stackedWidget->addWidget(mAddHoleHandleWidget);
	stackedWidget->addWidget(mAddHoleHandleCVWidget);
	stackedWidget->addWidget(mAddHandleSIWidget);
	stackedWidget->addWidget(mRindModelingScalingWidget); 
	stackedWidget->addWidget(mRindModelingThicknessWidget);
	stackedWidget->addWidget(mWireframeModelingWidget); 
	stackedWidget->addWidget(mWireframeModeling2Widget); 
	stackedWidget->addWidget(mColumnModelingWidget);	
	stackedWidget->addWidget(mSierpinskyWidget);
	stackedWidget->addWidget(mMultiFaceHandleWidget);
	stackedWidget->addWidget(mMengerSpongeWidget);
	
}

void HighgenusMode::triggerAddHoleHandle(){
	
	((MainWindow*)mParent)->setToolOptions(mAddHoleHandleWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ConnectFaceVertices);
}

void HighgenusMode::triggerAddHoleHandleCV() {
	((MainWindow*)mParent)->setToolOptions(mAddHoleHandleCVWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ConnectFaces);	
}

void HighgenusMode::triggerAddHandleSI(){
	
	((MainWindow*)mParent)->setToolOptions(mAddHandleSIWidget);
	((MainWindow*)mParent)->setMode(MainWindow::HermiteConnectFaces);
}

void HighgenusMode::triggerRindModelingScaling(){
	
	((MainWindow*)mParent)->setToolOptions(mRindModelingScalingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SelectFaceLoop);
	// ((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerRindModelingThickness(){
	
	((MainWindow*)mParent)->setToolOptions(mRindModelingThicknessWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SelectFaceLoop);
}

void HighgenusMode::triggerWireframeModeling(){
	
	((MainWindow*)mParent)->setToolOptions(mWireframeModelingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerWireframeModeling2(){
	
	((MainWindow*)mParent)->setToolOptions(mWireframeModeling2Widget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerColumnModeling(){
	
	((MainWindow*)mParent)->setToolOptions(mColumnModelingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerSierpinsky(){
	
	((MainWindow*)mParent)->setToolOptions(mSierpinskyWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerMultiFaceHandle(){
	
	((MainWindow*)mParent)->setToolOptions(mMultiFaceHandleWidget);
	((MainWindow*)mParent)->setMode(MainWindow::MultiSelectFace);
}

void HighgenusMode::triggerMengerSponge(){
	
	((MainWindow*)mParent)->setToolOptions(mMengerSpongeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::toggleCrustCleanupFlag(int state)
{
	rindModelingThicknessCleanupCheckBox->setChecked(state);
	rindModelingScalingCleanupCheckBox->setChecked(state);
	
	((MainWindow*)mParent)->toggleCrustCleanupFlag(state);
}

void HighgenusMode::toggleWireframeSplit(int state){
	wireframeSplitCheckBox->setChecked(state);
	wireframe2SplitCheckBox->setChecked(state);
	
	((MainWindow*)mParent)->toggleWireframeSplit(state);
}


QDoubleSpinBox *HighgenusMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
	label->setText(s);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
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

void HighgenusMode::setupAddHoleHandle(){
	
	mAddHoleHandleLayout = new QGridLayout;
	mAddHoleHandleLayout->setVerticalSpacing(1);
	mAddHoleHandleLayout->setHorizontalSpacing(1);
	// mAddHoleHandleLayout->setMargin(0);
	addHoleHandleNumSegmentsLabel = new QLabel(this);
	addHoleHandleNumSegmentsSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandleNumSegmentsLabel, tr("# Segments"), 1, 100, 1, 10, 0, 0,0);
	connect(addHoleHandleNumSegmentsSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsValueChanged(double)));
	
	//addHoleHandleNumSegmentsConnectLabel = new QLabel(this);
	//addHoleHandleNumSegmentsConnectSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandleNumSegmentsConnectLabel, tr("# Segments to\nConnect (-1=all)"), -1, 10, 1, -1, 0, 1,0);
	//connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsConnectValueChanged(double)));

	addHoleHandlePinching = new QLabel(this);
	addHoleHandlePinchingConnectSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandlePinching, tr("Pinching"), -1, 10, 0.1, 1.0, 3, 2,0);	
	connect(addHoleHandlePinchingConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeHoleHandlePinch(double)));

	addHoleHandlePinchingCenter = new QLabel(this);
	addHoleHandlePinchingCenterConnectSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandlePinchingCenter, tr("Pinch Center"), 0.1, 1, 0.1, 0.5, 3, 3,0);	
	connect(addHoleHandlePinchingCenterConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeHoleHandlePinchCenter(double)));

    addHoleHandlePinchingWidth = new QLabel(this);
	addHoleHandlePinchingWidthConnectSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandlePinchingWidth, tr("Pinch Width"), 0.001, 10, 0.1, 1.0, 3, 4,0);	
	connect(addHoleHandlePinchingWidthConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeHoleHandlePinchWidth(double)));

	mAddHoleHandleLayout->setRowStretch(6,1);
	mAddHoleHandleLayout->setColumnStretch(2,1);
	mAddHoleHandleWidget->setWindowTitle(tr("Add Hole/Handle"));
    mAddHoleHandleWidget->setLayout(mAddHoleHandleLayout);
}

//slot for num segments to reset the max segments range
void HighgenusMode::numSegmentsValueChanged(double value){
	
	((MainWindow*)mParent)->changeNumSegments(value);

	addHoleHandleNumSegmentsSpinBox->setValue(value);
	addHoleHandleCVNumSegmentsSpinBox->setValue(value);
	addHandleSINumSegmentsSpinBox->setValue(value);
	
	//set the # segments to connect range on all three hole/handle panels
	//addHoleHandleNumSegmentsConnectSpinBox->setRange(0,value);
	//addHoleHandleCVNumSegmentsConnectSpinBox->setRange(0,value);
	//addHandleSINumSegmentsConnectSpinBox->setRange(0,value);
	
	//if (addHoleHandleNumSegmentsConnectSpinBox->value() > value ||
			//addHoleHandleCVNumSegmentsConnectSpinBox->value() > value) {// ||
			//addHandleSINumSegmentsConnectSpinBox->value() > value) {
				
		// addHoleHandleNumSegmentsConnectSpinBox->setValue(value);
		//addHoleHandleCVNumSegmentsConnectSpinBox->setValue(value);
		//addHandleSINumSegmentsConnectSpinBox->setValue(value);
	//}
}
	
void HighgenusMode::ChangeScherkCollinsPinch(double value) {
	((MainWindow*)mParent)->changeScherkCollinsPinch(value);
}
void HighgenusMode::ChangeScherkCollinsPinchCenter(double value) {
	((MainWindow*)mParent)->changeScherkCollinsPinchCenter(value);
}
void HighgenusMode::ChangeScherkCollinsPinchWidth(double value) {
	((MainWindow*)mParent)->changeScherkCollinsPinchWidth(value);
}
void HighgenusMode::ChangeScherkCollinsHoleNumSegments(double value) {
	((MainWindow*)mParent)->changeScherkCollinsHoleNumSegments(value);
}

void HighgenusMode::ChangeScherkCollinsHoleInitSkip(double value) {
	((MainWindow*)mParent)->changeScherkCollinsHoleInitSkip(value);
}

void HighgenusMode::ChangeScherkCollinsHoleSkip(double value) {
	((MainWindow*)mParent)->changeScherkCollinsHoleSkip(value);
}

void HighgenusMode::ChangeScherkCollinsHoleTwist(double value) {
	((MainWindow*)mParent)->changeScherkCollinsHoleTwist(value);
}

void HighgenusMode::ChangeScherkCollinsHoleSegSkip(double value) {
	((MainWindow*)mParent)->changeScherkCollinsHoleSegSkip(value);
}


//slot for num segments to set the max segments
void HighgenusMode::numSegmentsConnectValueChanged(double value){
	
	((MainWindow*)mParent)->changeMaxSegments(value);
	
	// addHoleHandleNumSegmentsConnectSpinBox->setValue(value);
	//addHoleHandleCVNumSegmentsConnectSpinBox->setValue(value);
	//addHandleSINumSegmentsConnectSpinBox->setValue(value);	
}

//slot for change hold/handle pinch amount
void HighgenusMode::changeHoleHandlePinch(double value){
	((MainWindow*)mParent)->changeHoleHandlePinchValue(value);
}
//slot for change hold/handle pinch amount
void HighgenusMode::changeHoleHandlePinchCenter(double value){
	
	((MainWindow*)mParent)->changeHoleHandlePinchCenterValue(value);
}
//slot for change hold/handle pinch amount
void HighgenusMode::changeHoleHandlePinchWidth(double value){
	
	((MainWindow*)mParent)->changeHoleHandlePinchWidthValue(value);
}

void HighgenusMode::setupAddHoleHandleCV(){
	
	mAddHoleHandleCVLayout = new QGridLayout;
	// mAddHoleHandleCVLayout->setMargin(0);
	mAddHoleHandleCVLayout->setVerticalSpacing(1);
	mAddHoleHandleCVLayout->setHorizontalSpacing(1);

	addHoleHandleCVNumSegmentsLabel = new QLabel(this);
	addHoleHandleCVNumSegmentsSpinBox = createDoubleSpinBox(mAddHoleHandleCVLayout, addHoleHandleCVNumSegmentsLabel, tr("# Segments"), 1, 100, 1, 10, 0, 0,0);
	connect(addHoleHandleCVNumSegmentsSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsValueChanged(double)));
	
	//addHoleHandleCVNumSegmentsConnectLabel = new QLabel(this);
	//addHoleHandleCVNumSegmentsConnectSpinBox = createDoubleSpinBox(mAddHoleHandleCVLayout, addHoleHandleCVNumSegmentsConnectLabel, tr("# Segments to\nConnect (-1=all)"), 0, 10, 1, -1, 0, 1,0);
	//connect(addHoleHandleCVNumSegmentsConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsConnectValueChanged(double)));
	
	mAddHoleHandleCVLayout->setRowStretch(4,1);
	mAddHoleHandleCVLayout->setColumnStretch(2,1);
	mAddHoleHandleCVWidget->setWindowTitle(tr("Add Hole/Handle (Closest Vertex)"));
	mAddHoleHandleCVWidget->setLayout(mAddHoleHandleCVLayout);	
}

void HighgenusMode::setupAddHandleSI(){
	
	mAddHandleSILayout = new QGridLayout;
	// mAddHandleSILayout->setMargin(0);
	mAddHandleSILayout->setVerticalSpacing(1);
	mAddHandleSILayout->setHorizontalSpacing(1);
	
	//number of segments
	addHandleSINumSegmentsLabel = new QLabel(this);
	addHandleSINumSegmentsSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSINumSegmentsLabel, tr("# Segments"), 1, 100, 1, 10, 0, 0,0);
	connect(addHandleSINumSegmentsSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsValueChanged(double)));

	//number of segments to connect
  /*
	addHandleSINumSegmentsConnectLabel = new QLabel(this);
	addHandleSINumSegmentsConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSINumSegmentsConnectLabel, tr("# Segments to\nConnect (-1=all)"), 0, 10, 1, -1, 0, 1,0);
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsConnectValueChanged(double)));
  */
	//weight 1
	addHandleSIWeight1Label = new QLabel(this);
	addHandleSIWeight1SpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIWeight1Label, tr("Weight 1"), -100, 100, 0.1, 25.0, 3, 2,0);
	connect(addHandleSIWeight1SpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWeight1(double)));
	//symmetric weights checkbox
	addHandleSISymmetricWeightsCheckBox = new QCheckBox(tr("Symmetric Weights"),this);
	addHandleSISymmetricWeightsCheckBox->setChecked(Qt::Checked);
	connect(addHandleSISymmetricWeightsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleSymmetricWeightsFlag(int)));
	mAddHandleSILayout->addWidget(addHandleSISymmetricWeightsCheckBox,3,0,1,2);
	//weight 2 - must be disabled when the checkbox is unchecked...
	addHandleSIWeight2Label = new QLabel(this);
	addHandleSIWeight2SpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIWeight2Label, tr("Weight 2"), -100, 100, 0.1, 25.0, 3, 4,0);	
	addHandleSIWeight2SpinBox->setEnabled(false);
	connect(addHandleSIWeight2SpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWeight2(double)));

	//add twists?
	addHandleSITwistsConnectLabel = new QLabel(this);
	addHandleSITwistsConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSITwistsConnectLabel, tr("Extra Twists"), 0, 100, 1, 0, 0, 5,0);	
	connect(addHandleSITwistsConnectSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeExtraTwists(double)));

	//pinching controls
	addHandleSIPinchConnectLabel = new QLabel(this);
	addHandleSIPinchConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIPinchConnectLabel, tr("Pinching"), -1, 10, 0.1, 1.0, 3, 6,0);	
	connect(addHandleSIPinchConnectSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changePinch(double)));

	//pinching center
	addHandleSIPinchCenterConnectLabel = new QLabel(this);
	addHandleSIPinchCenterConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIPinchCenterConnectLabel, tr("Pinch Center"), 0.001, 0.999, 0.05, 0.5, 3, 7,0);	
	connect(addHandleSIPinchCenterConnectSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changePinchCenter(double)));

	//bubble controls
	addHandleSIBubbleConnectLabel = new QLabel(this);
	addHandleSIBubbleConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIBubbleConnectLabel, tr("Pinch Width"), 0.001, 10.0, 0.1, 1.0, 3, 8,0);	
	connect(addHandleSIBubbleConnectSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeBubble(double)));
		
	addHandleTriangulateNewFacesCheckBox = new QCheckBox(tr("Triangulate New Faces"), this);	
	connect(addHandleTriangulateNewFacesCheckBox, SIGNAL(stateChanged(int)), this,
          SLOT(toggleTriangulateFace(int)));
	mAddHandleSILayout->addWidget(addHandleTriangulateNewFacesCheckBox, 9, 0, 1, 2);

	addHandleSIScherkCollinsCheckBox = new QCheckBox(tr("Scherk Collins"), this);	
	connect(addHandleSIScherkCollinsCheckBox, SIGNAL(stateChanged(int)), this,
          SLOT(toggleScherkCollins(int)));
	mAddHandleSILayout->addWidget(addHandleSIScherkCollinsCheckBox, 10, 0, 1, 2);

	QLabel *addHandleSIPinchLabel = new QLabel(this);
	addHandleSIPinchSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIPinchLabel,
      tr("Pinch"), -1, 10, 0.1, 1.0, 2, 11, 0);	
	connect(addHandleSIPinchSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsPinch(double)));

	QLabel *addHandleSIPinchCenterLabel = new QLabel(this);
	addHandleSIPinchCenterSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIPinchCenterLabel,
      tr("Pinch Center"), 0.1, 1, 0.1, 0.5, 3, 12, 0);	
	connect(addHandleSIPinchCenterSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsPinchCenter(double)));

	QLabel *addHandleSIPinchWidthLabel = new QLabel(this);
	addHandleSIPinchWidthSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIPinchWidthLabel,
      tr("Pinch Width"), 0.001, 10, 0.1, 1.0, 3, 13, 0);	
	connect(addHandleSIPinchWidthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsPinchWidth(double)));

	QLabel *addHandleSIHoleNumSegmentsLabel = new QLabel(this);
	addHandleSIHoleNumSegmentsSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIHoleNumSegmentsLabel,
      tr("#segs in holes"), 1, 10, 1, 2.0, 0, 14, 0);	
	connect(addHandleSIHoleNumSegmentsSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsHoleNumSegments(double)));

	QLabel *addHandleSIHoleInitSkipLabel = new QLabel(this);
	addHandleSIHoleInitSkipSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIHoleInitSkipLabel,
      tr("Init skip"), 0, 10, 1, 1.0, 0, 15, 0);	
	connect(addHandleSIHoleInitSkipSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsHoleInitSkip(double)));

	QLabel *addHandleSIHoleSkipLabel = new QLabel(this);
	addHandleSIHoleSkipSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIHoleSkipLabel,
      tr("Skip"), 0, 10, 1, 0.0, 0, 16, 0);	
	connect(addHandleSIHoleSkipSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsHoleSkip(double)));

	QLabel *addHandleSIHoleTwistLabel = new QLabel(this);
	addHandleSIHoleTwistSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIHoleTwistLabel,
      tr("Twist of holes"), 0, 10, 1, 1.0, 0, 17, 0);	
	connect(addHandleSIHoleTwistSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsHoleTwist(double)));

	QLabel *addHandleSIHoleSegSkipLabel = new QLabel(this);
	addHandleSIHoleSegSkipSpinBox = createDoubleSpinBox(
      mAddHandleSILayout, addHandleSIHoleSegSkipLabel,
      tr("Skip in segments"), 0, 10, 1, 0.0, 0, 18, 0);	
	connect(addHandleSIHoleSegSkipSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(ChangeScherkCollinsHoleSegSkip(double)));

    //added by Saied

    normalTwistFactor_lable = new QLabel(this);
    normalTwistFactor_lable->setText("Twist Normals");
    normalTwistFactor_slider = new QSlider(Qt::Horizontal, this);
    normalTwistFactor_slider->setMaximum(180);
    normalTwistFactor_slider->setMinimum(-180);
    connect(normalTwistFactor_slider, SIGNAL(sliderMoved(int)), this, SLOT(changeNormalTwistingFactor(int)));
    mAddHandleSILayout->addWidget(normalTwistFactor_lable, 19,0);
    mAddHandleSILayout->addWidget(normalTwistFactor_slider, 19,1);
//end

	mAddHandleSILayout->setRowStretch(18,1);
	mAddHandleSILayout->setColumnStretch(2,1);
	mAddHandleSIWidget->setWindowTitle(tr("Add Handle (Shape Interpolation)"));
	mAddHandleSIWidget->setLayout(mAddHandleSILayout);	
	
}
void HighgenusMode::toggleTriangulateFace(int state){
	((MainWindow*)mParent)->toggleTriangulateNewFaces(state);
  if ((bool)state) {
    addHandleSIScherkCollinsCheckBox->setChecked(Qt::Unchecked);
  }
}

void HighgenusMode::toggleScherkCollins(int state){
	((MainWindow*)mParent)->toggleScherkCollins(state);
  if ((bool)state) {
    addHandleTriangulateNewFacesCheckBox->setChecked(Qt::Unchecked);
  }
}
void HighgenusMode::toggleSymmetricWeightsFlag(int state){
	
	((MainWindow*)mParent)->toggleSymmetricWeightsFlag(state);
	
	if (state) addHandleSIWeight2SpinBox->setEnabled(false);
	else addHandleSIWeight2SpinBox->setEnabled(true);
}

void HighgenusMode::setupRindModelingScaling(){
	
	mRindModelingScalingLayout = new QGridLayout;
	// mRindModelingScalingLayout->setMargin(0);
	mRindModelingScalingLayout->setVerticalSpacing(1);
	mRindModelingScalingLayout->setHorizontalSpacing(1);

	//scale factor - 0.00-10.00
	rindModelingScalingLabel = new QLabel(this);
	rindModelingScalingSpinBox = createDoubleSpinBox(mRindModelingScalingLayout, rindModelingScalingLabel, tr("Scale"), 0, 10.0, 0.01, 0.9, 3, 0,0);	
	connect(rindModelingScalingSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeCrustScaleFactor(double)));
	//symmetric weights checkbox
	rindModelingScalingCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
	rindModelingScalingCleanupCheckBox->setChecked(Qt::Unchecked);
	connect(rindModelingScalingCleanupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCrustCleanupFlag(int)));
	mRindModelingScalingLayout->addWidget(rindModelingScalingCleanupCheckBox,1,0,1,2);
	//create crust button
	rindModelingScalingCreateCrustButton = new QPushButton(tr("Create Crust"), this);
	connect(rindModelingScalingCreateCrustButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(crustModeling4())); // ((MainWindow*)mParent), SLOT(crust_modeling1())); //crust_modeling3
	mRindModelingScalingLayout->addWidget(rindModelingScalingCreateCrustButton,2,0,1,2);	
	//set layout and add stretch
	mRindModelingScalingLayout->setRowStretch(3,1);
	mRindModelingScalingLayout->setColumnStretch(2,1);
	mRindModelingScalingWidget->setWindowTitle(tr("Rind Modeling (Scaling)"));
	mRindModelingScalingWidget->setLayout(mRindModelingScalingLayout);	
}

void HighgenusMode::setupRindModelingThickness(){

	mRindModelingThicknessLayout = new QGridLayout;
	// mRindModelingThicknessLayout->setMargin(0);
	mRindModelingThicknessLayout->setVerticalSpacing(1);
	mRindModelingThicknessLayout->setHorizontalSpacing(1);
	
	//thickness
	rindModelingThicknessLabel = new QLabel(this);
	rindModelingThicknessSpinBox = createDoubleSpinBox(mRindModelingThicknessLayout, rindModelingThicknessLabel, tr("Thickness:"), -1.0, 1.0, 0.01, 0.5, 3, 0,0);	
	connect(rindModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeCrustThickness(double)));
	//symmetric weights checkbox
	rindModelingThicknessCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
	rindModelingThicknessCleanupCheckBox->setChecked(Qt::Unchecked);
	connect(rindModelingThicknessCleanupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCrustCleanupFlag(int)));
	mRindModelingThicknessLayout->addWidget(rindModelingThicknessCleanupCheckBox, 1,0,1,2);
	//create crust button
	rindModelingThicknessCreateButton = new QPushButton(tr("Create Crust"), this);
	connect(rindModelingThicknessCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(crustModeling3()));
	mRindModelingThicknessLayout->addWidget(rindModelingThicknessCreateButton,2,0,1,2);	
	//set layout and add stretch
	mRindModelingThicknessLayout->setRowStretch(3,1);
	mRindModelingThicknessLayout->setColumnStretch(2,1);
	mRindModelingThicknessWidget->setWindowTitle(tr("Rind Modeling (Thickness)"));
	mRindModelingThicknessWidget->setLayout(mRindModelingThicknessLayout);
}


void HighgenusMode::setupWireframeModeling(){
	
	mWireframeModelingLayout = new QGridLayout;
	// mWireframeModelingLayout->setMargin(0);
	mWireframeModelingLayout->setVerticalSpacing(1);
	mWireframeModelingLayout->setHorizontalSpacing(1);
	//thickness
	wireframeModelingThicknessLabel = new QLabel(this);
	wireframeModelingThicknessSpinBox = createDoubleSpinBox(mWireframeModelingLayout, wireframeModelingThicknessLabel, tr("Thickness:"), 0.0, 1.0, 0.01, 0.25, 3, 0,0);	
	connect(wireframeModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWireframeThickness(double)));
	
	//split 2 valence checkbox
	wireframeSplitCheckBox = new QCheckBox(tr("Split Valence-2 Vertices"),this);
	wireframeSplitCheckBox->setChecked(Qt::Checked);
	connect(wireframeSplitCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleWireframeSplit(int)));
	mWireframeModelingLayout->addWidget(wireframeSplitCheckBox,1,0,1,2);
	
	//create wireframe button
	wireframeModelingCreateButton = new QPushButton(tr("Create Wireframe"), this);
	connect(wireframeModelingCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(makeWireframe()));
	mWireframeModelingLayout->addWidget(wireframeModelingCreateButton,2,0,1,2);	
	//set layout and add stretch
	mWireframeModelingLayout->setRowStretch(3,1);
	mWireframeModelingLayout->setColumnStretch(2,1);
	mWireframeModelingWidget->setWindowTitle(tr("Wireframe Modeling"));
	mWireframeModelingWidget->setLayout(mWireframeModelingLayout);
}

void HighgenusMode::setupWireframeModeling2(){
	
	mWireframeModeling2Layout = new QGridLayout;
	// mWireframeModeling2Layout->setMargin(0);
	mWireframeModeling2Layout->setVerticalSpacing(1);
	mWireframeModeling2Layout->setHorizontalSpacing(1);
	//thickness
	wireframeModeling2ThicknessLabel = new QLabel(this);
	wireframeModeling2ThicknessSpinBox = createDoubleSpinBox(mWireframeModeling2Layout, wireframeModeling2ThicknessLabel, tr("Thickness:"), 0.0, 1.0, 0.01, 0.25, 3, 0,0);	
	connect(wireframeModeling2ThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWireframe2Thickness(double)));
	wireframeModeling2WidthLabel = new QLabel(this);
	wireframeModeling2WidthSpinBox = createDoubleSpinBox(mWireframeModeling2Layout, wireframeModeling2WidthLabel, tr("Width:"), 0.0, 1.0, 0.01, 0.25, 3, 1,0);	
	connect(wireframeModeling2WidthSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWireframe2Width(double)));
	
	//split 2 valence checkbox
	wireframe2SplitCheckBox = new QCheckBox(tr("Split Valence-2 Vertices"),this);
	wireframe2SplitCheckBox->setChecked(Qt::Checked);
	connect(wireframe2SplitCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleWireframeSplit(int)));
	mWireframeModeling2Layout->addWidget(wireframe2SplitCheckBox,2,0,1,2);
	
	//create wireframe button
	wireframeModeling2CreateButton = new QPushButton(tr("Create Wireframe 2"), this);
	connect(wireframeModeling2CreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(makeWireframe2()));
	mWireframeModeling2Layout->addWidget(wireframeModeling2CreateButton,3,0,1,2);	
	//set layout and add stretch
	mWireframeModeling2Layout->setRowStretch(4,1);
	mWireframeModeling2Layout->setColumnStretch(2,1);
	mWireframeModeling2Widget->setWindowTitle(tr("Wireframe Modeling 2"));
	mWireframeModeling2Widget->setLayout(mWireframeModeling2Layout);
}

void HighgenusMode::setupColumnModeling(){
	
	mColumnModelingLayout = new QGridLayout;	
	// mColumnModelingLayout->setMargin(0);
	mColumnModelingLayout->setVerticalSpacing(1);
	mColumnModelingLayout->setHorizontalSpacing(1);
	
	//thickness
	columnModelingThicknessLabel = new QLabel(this);
	columnModelingThicknessSpinBox = createDoubleSpinBox(mColumnModelingLayout, columnModelingThicknessLabel, tr("Thickness:"), 0.0, 1.0, 0.01, 0.25, 3, 0,0);	
	connect(columnModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeColumnThickness(double)));
	//number of segments
	columnModelingNumSegmentsLabel = new QLabel(this);
	columnModelingNumSegmentsSpinBox = createDoubleSpinBox(mColumnModelingLayout, columnModelingNumSegmentsLabel, tr("# Segments:"), 4.0, 100, 1, 4, 0, 1,0);	
	connect(columnModelingNumSegmentsSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeColumnSegments(double)));
	//create column button
	columnModelingCreateButton = new QPushButton(tr("Create Columns"), this);
	connect(columnModelingCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(makeWireframeWithColumns()));
	mColumnModelingLayout->addWidget(columnModelingCreateButton,2,0,1,2);	
	//set layout and add stretch
	mColumnModelingLayout->setRowStretch(3,1);
	mColumnModelingLayout->setColumnStretch(2,1);
	mColumnModelingWidget->setWindowTitle(tr("Column Modeling"));
	mColumnModelingWidget->setLayout(mColumnModelingLayout);	
}

void HighgenusMode::setupSierpinsky(){
	
	mSierpinskyLayout = new QGridLayout;	
	// mSierpinskyLayout->setMargin(0);
	mSierpinskyLayout->setVerticalSpacing(1);
	mSierpinskyLayout->setHorizontalSpacing(1);	
	//create column button
	sierpinskyButton = new QPushButton(tr("Create Sierpinsky\nTetrahedra"), this);
	connect(sierpinskyButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(multiConnectMidpoints()));
	mSierpinskyLayout->addWidget(sierpinskyButton,0,0);
	
	mSierpinskyLayout->setRowStretch(1,1);
	mSierpinskyLayout->setColumnStretch(1,1);
	mSierpinskyWidget->setWindowTitle(tr("Sierpinsky Tetrahedra"));
	mSierpinskyWidget->setLayout(mSierpinskyLayout);
}

void HighgenusMode::setupMultiFaceHandle(){
	
	mMultiFaceHandleLayout = new QGridLayout;
	// mMultiFaceHandleLayout->setMargin(0);
	mMultiFaceHandleLayout->setVerticalSpacing(1);
	mMultiFaceHandleLayout->setHorizontalSpacing(1);
	
	multiFaceAlgorithmGroupBox = new QGroupBox(tr("Use Convex Hull\nAlgorithm"));
	multiFaceAlgorithmGroupBox->setCheckable(true);
	multiFaceAlgorithmGroupBox->setChecked(false);
	
	mMultiFaceAlgorithmLayout = new QGridLayout;
	mMultiFaceAlgorithmLayout->setVerticalSpacing(2);
	mMultiFaceAlgorithmLayout->setHorizontalSpacing(2);

	multiFaceAlgorithmGroupBox->setAlignment(Qt::AlignLeft);

	connect(multiFaceAlgorithmGroupBox, SIGNAL(toggled(bool)),
	          this, SLOT(changeMultiFaceAlgorithm(bool)));
	
	//scaling
	multiFaceHandleScaleLabel = new QLabel(this);
	multiFaceHandleScaleSpinBox = new QDoubleSpinBox;
	multiFaceHandleScaleSpinBox->setRange(0.0,5.0);
	multiFaceHandleScaleSpinBox->setSingleStep(0.01);
	multiFaceHandleScaleSpinBox->setValue(1.0);
	multiFaceHandleScaleSpinBox->setDecimals(2);
	multiFaceHandleScaleSpinBox->setMaximumSize(75,25);
	mMultiFaceAlgorithmLayout->addWidget(multiFaceHandleScaleLabel,0,0);
	mMultiFaceAlgorithmLayout->addWidget(multiFaceHandleScaleSpinBox,0,1);
	multiFaceHandleScaleLabel = new QLabel(this);
	multiFaceHandleScaleSpinBox = createDoubleSpinBox(mMultiFaceAlgorithmLayout, multiFaceHandleScaleLabel, tr("Scale Factor:"), 0.0, 5.0, 0.01, 1.0, 3, 0,0);	
	connect(multiFaceHandleScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMultiFaceHandleScaleFactor(double)));

	//extrude distance
	multiFaceHandleExtrudeDistanceLabel = new QLabel(this);
	multiFaceHandleExtrudeDistanceSpinBox = new QDoubleSpinBox;
	multiFaceHandleExtrudeDistanceSpinBox->setRange(-2.0,2.0);
	multiFaceHandleExtrudeDistanceSpinBox->setSingleStep(0.01);
	multiFaceHandleExtrudeDistanceSpinBox->setValue(0.5);
	multiFaceHandleExtrudeDistanceSpinBox->setDecimals(1);
	multiFaceHandleExtrudeDistanceSpinBox->setMaximumSize(75,25);
	mMultiFaceAlgorithmLayout->addWidget(multiFaceHandleExtrudeDistanceLabel,1,0);
	mMultiFaceAlgorithmLayout->addWidget(multiFaceHandleExtrudeDistanceSpinBox,1,1);
	multiFaceHandleExtrudeDistanceLabel = new QLabel(this);
	multiFaceHandleExtrudeDistanceSpinBox = createDoubleSpinBox(mMultiFaceAlgorithmLayout, multiFaceHandleExtrudeDistanceLabel, tr("Extrude Dist.\nFactor:"), -2.0, 2.0, 0.1, 0.5, 3, 1,0);	
	connect(multiFaceHandleExtrudeDistanceSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMultiFaceHandleExtrudeDist(double)));	

	//use max. offsets
	multiFaceHandleMaxOffsetsCheckBox = new QCheckBox(tr("Use max offsets"),this);
	connect(multiFaceHandleMaxOffsetsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleMultiFaceHandleUseMaxOffsetFlag(int)));
	mMultiFaceAlgorithmLayout->addWidget(multiFaceHandleMaxOffsetsCheckBox,2,0,1,2);
	multiFaceAlgorithmGroupBox->setLayout(mMultiFaceAlgorithmLayout);
	
	//add the groupbox to the multiface layout
	mMultiFaceHandleLayout->addWidget(multiFaceAlgorithmGroupBox,0,0);
	multiFaceHandleButton = new QPushButton(tr("Connect Selected Faces"), this);
	connect(multiFaceHandleButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(createMultiFaceHandle()));
	mMultiFaceHandleLayout->addWidget(multiFaceHandleButton,1,0,1,2);
	
	mMultiFaceHandleLayout->setRowStretch(4,1);
	mMultiFaceHandleLayout->setColumnStretch(2,1);
	mMultiFaceHandleWidget->setWindowTitle(tr("Multi-Face Handle"));
	mMultiFaceHandleWidget->setLayout(mMultiFaceHandleLayout);
}

void HighgenusMode::toggleMultiFaceHandleUseMaxOffsetFlag(int state){
	
	((MainWindow*)mParent)->toggleMultiFaceHandleUseMaxOffsetFlag(state);
	
	if (state){
		multiFaceHandleExtrudeDistanceSpinBox->setEnabled(false);
		multiFaceHandleExtrudeDistanceLabel->setEnabled(false);
	}
	else {
		multiFaceHandleExtrudeDistanceLabel->setEnabled(true);
		multiFaceHandleExtrudeDistanceSpinBox->setEnabled(true);
	}
}

void HighgenusMode::changeMultiFaceAlgorithm(bool on){
	
	if (!on) {
		((MainWindow*)mParent)->mfh_use_closest_edge_algo();	
		// //set all three widgets to be disabled
		multiFaceHandleScaleLabel->setEnabled(false);
		multiFaceHandleExtrudeDistanceLabel->setEnabled(false);
		multiFaceHandleExtrudeDistanceSpinBox->setEnabled(false);
		multiFaceHandleMaxOffsetsCheckBox->setEnabled(false);
		multiFaceHandleScaleSpinBox->setEnabled(false);		
		
	}
	else { 		
		((MainWindow*)mParent)->mfh_use_convex_hull_algo();	
		
		multiFaceHandleScaleLabel->setEnabled(true);
		multiFaceHandleScaleSpinBox->setEnabled(true);
		multiFaceHandleMaxOffsetsCheckBox->setEnabled(true);
		
		//check if the checkbox is checked first or not, then enable widgets accordingly
		if (multiFaceHandleMaxOffsetsCheckBox->checkState() == Qt::Checked){
			multiFaceHandleExtrudeDistanceLabel->setEnabled(true);
			multiFaceHandleExtrudeDistanceSpinBox->setEnabled(true);
		}
	}
}

void HighgenusMode::setupMengerSponge(){
	
	mMengerSpongeLayout = new QGridLayout;
	// mMengerSpongeLayout->setMargin(0);
	mMengerSpongeLayout->setVerticalSpacing(1);
	mMengerSpongeLayout->setHorizontalSpacing(1);	
	//thickness
	mengerSpongeThicknessLabel = new QLabel(this);
	mengerSpongeThicknessSpinBox = createDoubleSpinBox(mMengerSpongeLayout, mengerSpongeThicknessLabel, tr("Thickness:"), 0.01, 1.0, 0.01, 0.67, 3, 0,0);	
	connect(mengerSpongeThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMengerSpongeThickness(double)));
	//threshold
	mengerSpongeThresholdLabel = new QLabel(this);
	mengerSpongeThresholdSpinBox = createDoubleSpinBox(mMengerSpongeLayout, mengerSpongeThresholdLabel, tr("Edge Collapse\nThreshold Factor:"), 0.0, 10.0, 0.1, 0.0, 3, 1,0);	
	connect(mengerSpongeThresholdSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMengerSpongeCollapseThreshold(double)));
	//fractional thickness checkbox
	mengerSpongFractionalThicknessCheckBox = new QCheckBox(tr("Fractional Thickness"),this);
	connect(mengerSpongFractionalThicknessCheckBox, SIGNAL(stateChanged(int)), ((MainWindow*)mParent), SLOT(toggleMengerSpongeFractionalThicknessFlag(int)));
	mMengerSpongeLayout->addWidget(mengerSpongFractionalThicknessCheckBox,2,0,1,2);
	//create menger sponge button
	mengerSpongeButton = new QPushButton(tr("Create Menger Sponge"), this);
	connect(mengerSpongeButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(createSponge()));
	mMengerSpongeLayout->addWidget(mengerSpongeButton,3,0,1,2);	
	
	mMengerSpongeLayout->setRowStretch(4,1);
	mMengerSpongeLayout->setColumnStretch(2,1);
	mMengerSpongeWidget->setWindowTitle(tr("Menger Sponge"));
	mMengerSpongeWidget->setLayout(mMengerSpongeLayout);
}

void HighgenusMode::retranslateUi(){
	//actions
	mAddHoleHandleAction->setText(tr("Add Hole/Handle"));
	mAddHoleHandleAction->setStatusTip(tr("Enter Add Hole/Handle Mode"));
	mAddHoleHandleAction->setToolTip(tr("Add Hole/Handle Mode"));
	mAddHoleHandleCVAction->setText(tr("Add Hole/Handle CV"));
	mAddHoleHandleCVAction->setStatusTip(tr("Enter Add Hole/Handle CV Mode"));
	mAddHoleHandleCVAction->setToolTip(tr("Add Hole/Handle CV Mode"));
	mAddHandleSIAction->setText(tr("Add Handle SI"));
	mAddHandleSIAction->setStatusTip(tr("Enter Add Handle (Shape Interpolation) Mode"));
	mAddHandleSIAction->setToolTip(tr("Add Handle (Shape Interpolation) Mode"));
	mRindModelingScalingAction->setText(tr("Rind Modeling Scaling"));
	mRindModelingScalingAction->setStatusTip(tr("Enter Rind Modeling Scaling Mode"));
	mRindModelingScalingAction->setToolTip(tr("Rind Modeling Scaling Mode"));
	mRindModelingThicknessAction->setText(tr("Rind Modeling Thickness"));
	mRindModelingThicknessAction->setStatusTip(tr("Enter Rind Modeling Thickness Mode"));
	mRindModelingThicknessAction->setToolTip(tr("Rind Modeling Thickness Mode"));
	mWireframeModelingAction->setText(tr("Wireframe Modeling"));
	mWireframeModelingAction->setStatusTip(tr("Enter Wireframe Modeling Mode"));
	mWireframeModelingAction->setToolTip(tr("Wireframe Modeling Mode"));
	mWireframeModeling2Action->setText(tr("Wireframe Modeling 2"));
	mWireframeModeling2Action->setStatusTip(tr("Enter Wireframe Modeling 2 Mode"));
	mWireframeModeling2Action->setToolTip(tr("Wireframe Modeling 2 Mode"));
	mColumnModelingAction->setText(tr("Column Modeling"));
	mColumnModelingAction->setStatusTip(tr("Enter Column Modeling Mode"));
	mColumnModelingAction->setToolTip(tr("Column Modeling Mode"));
	mSierpinskyAction->setText(tr("Sierpinsky"));
	mSierpinskyAction->setStatusTip(tr("Enter Sierpinsky Mode"));
	mSierpinskyAction->setToolTip(tr("Sierpinsky Mode"));
	mMultiFaceHandleAction->setText(tr("Multi-face Handle"));
	mMultiFaceHandleAction->setStatusTip(tr("Enter Multi-face Handle Mode"));
	mMultiFaceHandleAction->setToolTip(tr("Multi-face Handle Mode"));
	mMengerSpongeAction->setText(tr("Menger Sponge"));
	mMengerSpongeAction->setStatusTip(tr("Enter Menger Sponge Mode"));
	mMengerSpongeAction->setToolTip(tr("Menger Sponge Mode"));
	//menus
	mHighgenusMenu->setTitle(tr("High Genus"));
	
	//!< \todo add buttons and spinbox labels to retranslate function for highgenusmode.cc
	addHoleHandleNumSegmentsLabel->setText(tr("# Segments"));
	//addHoleHandleNumSegmentsConnectLabel->setText(tr("# Segments to\nConnect (-1=all)"));
	mAddHoleHandleWidget->setWindowTitle(tr("Add Hole/Handle"));
	addHoleHandleCVNumSegmentsLabel->setText(tr("# Segments"));
	//addHoleHandleCVNumSegmentsConnectLabel->setText(tr("# Segments to\nConnect (-1=all)"));
	mAddHoleHandleCVWidget->setWindowTitle(tr("Add Hole/Handle (Closest Vertex)"));
	addHandleSINumSegmentsLabel->setText(tr("# Segments"));
	//addHandleSINumSegmentsConnectLabel->setText(tr("# Segments to\nConnect (-1=all)"));
	addHandleSIWeight1Label->setText(tr("Weight 1"));
	addHandleSISymmetricWeightsCheckBox->setText(tr("Symmetric Weights"));
	addHandleSIWeight2Label->setText(tr("Weight 2"));	
	addHandleSITwistsConnectLabel->setText(tr("Extra Twists"));	
	mAddHandleSIWidget->setWindowTitle(tr("Add Handle (Shape Interpolation)"));
	rindModelingScalingLabel->setText(tr("Scale"));	
	rindModelingScalingCleanupCheckBox->setText(tr("Cleanup when peeling"));
	rindModelingScalingCreateCrustButton->setText(tr("Create Crust"));
	mRindModelingScalingWidget->setWindowTitle(tr("Rind Modeling (Scaling)"));
	rindModelingThicknessLabel->setText(tr("Thickness:"));	
	rindModelingThicknessCleanupCheckBox->setText(tr("Cleanup when peeling"));
	rindModelingThicknessCreateButton->setText(tr("Create Crust"));
	mRindModelingThicknessWidget->setWindowTitle(tr("Rind Modeling (Thickness)"));
	wireframeModelingThicknessLabel->setText(tr("Thickness:"));	
	wireframeModelingCreateButton->setText(tr("Create Wireframe"));
	mWireframeModelingWidget->setWindowTitle(tr("Wireframe Modeling"));
	wireframeModeling2ThicknessLabel->setText(tr("Thickness:"));	
	wireframeModeling2WidthLabel->setText(tr("Width:"));	
	wireframeModeling2CreateButton->setText(tr("Create Wireframe 2"));
	mWireframeModeling2Widget->setWindowTitle(tr("Wireframe Modeling 2"));
	columnModelingThicknessLabel->setText(tr("Thickness:"));	
	columnModelingNumSegmentsLabel->setText(tr("# Segments:"));	
	columnModelingCreateButton->setText(tr("Create Columns"));
	mColumnModelingWidget->setWindowTitle(tr("Column Modeling"));
	sierpinskyButton->setText(tr("Create Sierpinsky\nTetrahedra"));
	mSierpinskyWidget->setWindowTitle(tr("Sierpinsky Tetrahedra"));
	multiFaceAlgorithmGroupBox->setTitle(tr("Use Convex Hull\nAlgorithm"));
	multiFaceHandleScaleLabel->setText(tr("Scale Factor:"));
	multiFaceHandleExtrudeDistanceLabel->setText(tr("Extrude Dist.\nFactor:"));
	multiFaceHandleMaxOffsetsCheckBox->setText(tr("Use max offsets"));
	multiFaceHandleButton->setText(tr("Connect Selected Faces"));
	mMultiFaceHandleWidget->setWindowTitle(tr("Multi-Face Handle"));

	mengerSpongeThicknessLabel->setText(tr("Thickness:"));	
	mengerSpongeThresholdLabel->setText(tr("Edge Collapse\nThreshold Factor:"));	

	mengerSpongFractionalThicknessCheckBox->setText(tr("Fractional Thickness"));
	mengerSpongeButton->setText(tr("Create Menger Sponge"));
    mMengerSpongeWidget->setWindowTitle(tr("Menger Sponge"));
}

void HighgenusMode::changeNormalTwistingFactor(int angle)
{
    ((MainWindow*)mParent)->changeNormalTwistingFactor(angle);
}
