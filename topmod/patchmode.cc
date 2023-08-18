#include "patchmode.hh"
#include "TMPatch.hh"
#include "TMPatchObject.hh"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <qdebug.h>

PatchMode::PatchMode(QWidget *parent) : QWidget(parent) {
  this->setWindowTitle("Patch Modeling");
  //    parameters.angle = 0;
  //    parameters.cos_angle = 1;
  //    parameters.sin_angle = 0;
  //    parameters.scale = 4.0/3;
  //    parameters.rotation_method = ALL_UNIFORMLY_R;
  //    parameters.grid_size = 6;
  //    parameters.regularizePolygons = false;
  //    parameters.polygonRegularizationIteration=0;

  this->layout = new QGridLayout(this);
  this->setLayout(layout);

  this->resetBtn = new QPushButton(this);
  resetBtn->setText("Reset");
  resetBtn->setMaximumWidth(100);
  this->resetMode = false;
  connect(resetBtn, SIGNAL(pressed()), this, SLOT(resetParameters()));

  convert = new QPushButton(this);
  convert->setText("Convert to TopMode Model");
  // convert->setMaximumWidth(100);
  connect(convert, SIGNAL(pressed()), this, SLOT(convertTriggered()));

  saveAsObj = new QPushButton(this);
  saveAsObj->setText("Save as OBJ File");
  connect(saveAsObj, SIGNAL(pressed()), this, SLOT(saveAsObjTriggered()));

  scaleTitle = new QLabel("<b>Scale</b>", this);
  // Scale is multiplied by 100 to convert it to a double and get a smoother
  // tick
  this->scaleSlider = new QSlider(Qt::Horizontal, this);
  this->scaleSlider->setMaximum(1500);
  this->scaleSlider->setMinimum(800);
  this->scaleSlider->setValue(4000 / 3);
  connect(scaleSlider, SIGNAL(valueChanged(int)), this,
          SLOT(scaleValueChanged(int)));
  this->scaleLable = new QLabel("1.333", this);

  angleTitle = new QLabel("<b>Angle</b>", this);
  this->angleSlider = new QSlider(Qt::Horizontal, this);
  this->angleSlider->setMaximum(180);
  this->angleSlider->setMinimum(-180);
  this->angleSlider->setValue(0);
  connect(angleSlider, SIGNAL(valueChanged(int)), this,
          SLOT(angleValueChanged(int)));
  this->angleLable = new QLabel("0", this);

  rotationLable = new QLabel("<b>Rotation Options</b>");
  allOpt = new QRadioButton(this);
  allOpt->setText("All Control Points Uniformly");
  alternateOpt = new QRadioButton(this);
  alternateOpt->setText("Alternate Control Points");
  edgesOpt = new QRadioButton(this);
  edgesOpt->setText("Edge Control Points");
  vertexOpt = new QRadioButton(this);
  vertexOpt->setText("Vertex and Face-Center Control Points");
  allOpt->setChecked(true);
  connect(allOpt, SIGNAL(pressed()), this, SLOT(allOptSelected()));
  connect(alternateOpt, SIGNAL(pressed()), this, SLOT(alternateOptSelected()));
  connect(edgesOpt, SIGNAL(pressed()), this, SLOT(edgesOptSelected()));
  connect(vertexOpt, SIGNAL(pressed()), this, SLOT(vertexOptSelected()));

  QLabel *renderLabel = new QLabel("<b>Render Options</b>", this);

  subDivisionLabel = new QLabel(this);
  subDivisionLabel->setText("Number of UV Samples:");
  subDivisionBox = new QSpinBox(this);
  subDivisionBox->setRange(1, 256);
  subDivisionBox->setValue(6);
  connect(subDivisionBox, SIGNAL(valueChanged(int)), this,
          SLOT(subDivisionChanged(int)));

  patchNestingFactorLabel = new QLabel(this);
  patchNestingFactorLabel->setText("<b>Sub-Patches<b>");
  patchNestingFactorNumberLabel = new QLabel("0", this);
  sudivisionSlider = new QSlider(Qt::Horizontal, this);
  sudivisionSlider->setMaximum(6);
  sudivisionSlider->setMinimum(0);
  sudivisionSlider->setValue(0);
  connect(sudivisionSlider, SIGNAL(valueChanged(int)), this,
          SLOT(patchNestingFactorChanged(int)));

  localPatchNestingChecked =
      new QCheckBox("Locally subdivide nested patches", this);

  renderPatchBoundaries = new QCheckBox("Render patch boundaries", this);
  connect(renderPatchBoundaries, SIGNAL(clicked(bool)), this,
          SLOT(renderPatchBoundariesChanged(bool)));

  renderDooSabinPolygons = new QCheckBox("Show Doo-Sabin Polygons", this);
  connect(renderDooSabinPolygons, SIGNAL(clicked(bool)), this,
          SLOT(renderDooSabinPolygonsChanged(bool)));

  renderModifiedDooSabinPolygons =
      new QCheckBox("Show Modified Doo-Sabin Polygons", this);
  connect(renderModifiedDooSabinPolygons, SIGNAL(clicked(bool)), this,
          SLOT(renderModifiedDooSabinPolygonsChanged(bool)));

  localPatchNestingChecked->setChecked(false);
  connect(localPatchNestingChecked, SIGNAL(clicked(bool)), this,
          SLOT(patchNestingMethodChanged(bool)));

  QLabel *polygonTreatment = new QLabel("<b>Polygon Regularization</b>", this);
  regularizePolygons = new QCheckBox("Regularize Doo Sabin Faces", this);
  connect(regularizePolygons, SIGNAL(clicked(bool)), this,
          SLOT(regularizePolygonsChanged(bool)));

  polygonRegularizationIterationStringValue = new QLabel("Iterations: 0", this);
  polygonRegularizationIterationSlider = new QSlider(Qt::Horizontal, this);
  polygonRegularizationIterationSlider->setMaximum(30);
  polygonRegularizationIterationSlider->setMinimum(0);
  connect(polygonRegularizationIterationSlider, SIGNAL(valueChanged(int)), this,
          SLOT(polygonRegularizationIterationSliderChanged(int)));
  polygonRegularizationIterationSlider->setEnabled(false);

  /*
  vertexMovementFactorLabel = new QLabel(this);
  vertexMovementFactorLabel->setText("<b>Vertex Movement Factor<b>");

  vertexMovementFactorStringValue = new QLabel("0", this);
  vertexMovementFactorSlider = new QSlider(Qt::Horizontal, this);
  vertexMovementFactorSlider->setMinimum(0);
  vertexMovementFactorSlider->setMaximum(1000);
  connect(vertexMovementFactorSlider, SIGNAL(valueChanged(int)), this,
  SLOT(vertexMovementFactorChanged(int)));
  */

  this->showPatchControlMesh = new QCheckBox("Show Patch Control Mesh", this);
  connect(showPatchControlMesh, SIGNAL(clicked(bool)), this,
          SLOT(showPatchControlMeshChanged(bool)));

  int row = 0;
  layout->addWidget(scaleTitle, row, 0);
  row++;
  layout->addWidget(scaleLable, row, 0);
  layout->addWidget(scaleSlider, row, 1, 1, 2);
  row++;
  layout->addWidget(angleTitle, row, 0);
  row++;
  layout->addWidget(angleLable, row, 0);
  layout->addWidget(angleSlider, row, 1, 1, 2);
  row++;
  layout->addWidget(rotationLable, row, 0);
  row++;
  layout->addWidget(allOpt, row, 0, 1, 3);
  row++;
  layout->addWidget(alternateOpt, row, 0, 1, 3);
  row++;
  layout->addWidget(edgesOpt, row, 0, 1, 3);
  row++;
  layout->addWidget(vertexOpt, row, 0, 1, 3);
  //    row++;
  //    layout->addWidget(vertexMovementFactorLabel, row,0);
  //    row++;
  //    layout->addWidget(vertexMovementFactorStringValue, row,0);
  //    layout->addWidget(vertexMovementFactorSlider, row,1,1,2);
  row++;
  layout->addWidget(polygonTreatment, row, 0);
  row++;
  layout->addWidget(regularizePolygons, row, 0);
  row++;
  layout->addWidget(polygonRegularizationIterationStringValue, row, 0);
  layout->addWidget(polygonRegularizationIterationSlider, row, 1, 1, 2);
  row++;
  layout->addWidget(renderLabel, row, 0);
  row++;
  layout->addWidget(subDivisionLabel, row, 0);
  layout->addWidget(subDivisionBox, row, 2);
  row++;
  layout->addWidget(showPatchControlMesh, row, 0);
  row++;
  layout->addWidget(renderPatchBoundaries, row, 0);
  row++;
  layout->addWidget(renderDooSabinPolygons, row, 0);
  row++;
  layout->addWidget(renderModifiedDooSabinPolygons, row, 0);
  row++;
  layout->addWidget(patchNestingFactorLabel, row, 0);
  row++;
  layout->addWidget(patchNestingFactorNumberLabel, row, 0);
  layout->addWidget(sudivisionSlider, row, 1, 1, 2);
  row++;
  layout->addWidget(localPatchNestingChecked, row, 0);
  row++;
  layout->addWidget(resetBtn, row, 0, 1, 3, Qt::AlignCenter);
  row++;
  layout->addWidget(convert, row, 0, 1, 3, Qt::AlignCenter);
  row++;
  layout->addWidget(saveAsObj, row, 0, 1, 3, Qt::AlignCenter);
  row++;
}

void PatchMode::regularizePolygonsChanged(bool value) {
  this->parameters.regularizePolygons = value;
  this->polygonRegularizationIterationSlider->setEnabled(value);
  this->updated();
}

void PatchMode::polygonRegularizationIterationSliderChanged(const int &value) {
  this->parameters.polygonRegularizationIteration = value;
  QString s = "Iterations: " +
              QString::number(this->parameters.polygonRegularizationIteration);
  this->polygonRegularizationIterationStringValue->setText(s);
  this->updated();
}

void PatchMode::showPatchControlMeshChanged(bool value) {
  TMPatch::SetShowControlMesh(value);
  emit PatchRenderUpdateRequest();
}

void PatchMode::renderPatchBoundariesChanged(bool value) {
  TMPatchObject::SetRenderIncludesPatchEdges(value);
  emit PatchRenderUpdateRequest();
}

void PatchMode::renderDooSabinPolygonsChanged(bool value) {
  TMPatchObject::SetRenderIncludesDooSabinPolygons(value);
  emit PatchRenderUpdateRequest();
}

void PatchMode::renderModifiedDooSabinPolygonsChanged(bool value) {
  TMPatchObject::SetRenderIncludesModifiedDooSabinPolygons(value);
  emit PatchRenderUpdateRequest();
}

void PatchMode::subDivisionChanged(const int &value) {
  this->parameters.grid_size = value;
  TMPatch::SetPatchGridDimension((uint)value);
  emit PatchRenderUpdateRequest();
}

// void PatchMode::vertexMovementFactorChanged(int value){
//     double t = double(value)/1000.0;
//     this->parameters.vertex_movement_factor = t;
//     this->vertexMovementFactorStringValue->setText(QString::number(this->parameters.vertex_movement_factor,
//     'g', 3)); this->updated();
// }

void PatchMode::patchNestingMethodChanged(const bool &value) {
  this->parameters.local_patch_nesting = value;
  TMPatch::SetLocalPatchNesting(value);
  this->updated();
}

void PatchMode::patchNestingFactorChanged(const int &value) {
  this->patchNestingFactorNumberLabel->setText(QString::number(value));
  this->parameters.patch_nesting_factor = value;
  TMPatch::SetPatchNestingFactor(value);
  this->updated();
}

void PatchMode::resetParameters() {
  //    qDebug()<<"reset------------------------------";
  this->resetMode = true;
  this->scaleSlider->setValue(4000 / 3);
  this->angleSlider->setValue(0);
  this->allOpt->setChecked(true);
  this->parameters.rotation_method = ALL_UNIFORMLY_R;
  this->subDivisionBox->setValue(6);
  this->sudivisionSlider->setValue(0);
  this->localPatchNestingChecked->setChecked(false);
  // this->vertexMovementFactorSlider->setValue(0);
  this->resetMode = false;
  //    qDebug()<<"reset";
  this->polygonRegularizationIterationSlider->setValue(0);
  this->regularizePolygons->setChecked(false);
  this->updated();
}

void PatchMode::updated() {
  if (this->resetMode) {
    return;
  }
  //    qDebug() << "UPDATED: Angle " << QString::number(this->parameters.angle,
  //    'g', 3)
  //             << "; Cos "<< QString::number(this->parameters.cos_angle, 'g',
  //             3)
  //             << "; Sin "<< QString::number(this->parameters.sin_angle, 'g',
  //             3)
  //             << "; Scale "<< QString::number(this->parameters.scale, 'g', 3)
  //             << "; Mode "<<
  //             QString::number(this->parameters.rotation_method, 'g', 3)
  //             << "; Subdivision " <<
  //             QString::number(this->parameters.grid_size);
  emit this->ParametersUpdated();
}

void PatchMode::angleValueChanged(int value) {
  this->parameters.angle = value * M_PI / 180;
  this->parameters.cos_angle = cos(this->parameters.angle);
  this->parameters.sin_angle = sin(this->parameters.angle);
  this->angleLable->setText(QString::number(value));

  //    qDebug() << "angleValueChanged";
  this->updated();
}

void PatchMode::scaleValueChanged(int value) {
  this->parameters.scale = value / 1000.0;
  this->scaleLable->setText(QString::number(this->parameters.scale, 'g', 3));
  //    qDebug() << "scaleValueChanged";
  this->updated();
}

void PatchMode::allOptSelected() {
  this->parameters.rotation_method = ALL_UNIFORMLY_R;
  //    qDebug() << "allOptSelected";
  this->updated();
}
void PatchMode::alternateOptSelected() {
  this->parameters.rotation_method = ALTERNATE_R;
  //    qDebug() << "alternateOptSelected";
  this->updated();
}
void PatchMode::edgesOptSelected() {
  this->parameters.rotation_method = EDGES_ONLY_R;
  //    qDebug() << "edgesOptSelected";
  this->updated();
}
void PatchMode::vertexOptSelected() {
  this->parameters.rotation_method = VERTEX_FACE_ONLY_R;
  //    qDebug() << "vertexOptSelected";
  this->updated();
}

void PatchMode::convertTriggered() { emit ConvertToTopMode(); }

void PatchMode::saveAsObjTriggered() { emit SaveAsOBJ(); }
