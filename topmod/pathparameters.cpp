#include "pathparameters.h"
#include "ui_pathparameters.h"
#include "math.h"
PatchParameterSetting::PatchParameterSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PathParameters)
{
    ui->setupUi(this);
}

PatchParameterSetting::~PatchParameterSetting()
{
    delete ui;
}

struct PatchParams PatchParameterSetting::get_parameters(void){
    struct PatchParams params;
    QDoubleSpinBox* angleBox = this->findChild<QDoubleSpinBox*>("AngleBox");
    params.angle= angleBox->value()*M_PI/180;
    QDoubleSpinBox* scaleBox = this->findChild<QDoubleSpinBox*>("ScaleBox");
    params.scale = scaleBox->value();

    QRadioButton* rotationStyle = this->findChild<QRadioButton*>("rotateAll");
    if (rotationStyle->isChecked()) {
        params.rotation_method = ALL_UNIFORMLY;
        return params;
    }
    rotationStyle = this->findChild<QRadioButton*>("rotateEdge");
    if (rotationStyle->isChecked()) {
        params.rotation_method = EDGES_ONLY;
        return params;
    }
    rotationStyle = this->findChild<QRadioButton*>("rotateVertexFace");
    if (rotationStyle->isChecked()) {
        params.rotation_method = VERTEX_FACE_ONLY;
        return params;
    }
    rotationStyle = this->findChild<QRadioButton*>("rotateAlternatively");
    if (rotationStyle->isChecked()) {
        params.rotation_method = ALTERNATE_ROTATION;
        return params;
    }
    return params;
}
