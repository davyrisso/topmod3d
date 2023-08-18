#ifndef PATCHMODE_HH
#define PATCHMODE_HH

#include <QtGui>
#include <QtCore>
#include <QWidget>
class QGridLayout;
class QPushButton;
class QSlider;
class QRadioButton;
class QSpinBox;
class QCheckBox;
class QLabel;

enum ROTATION_METHOD{
    ALL_UNIFORMLY_R = 0,
    ALTERNATE_R = 1,
    EDGES_ONLY_R = 2,
    VERTEX_FACE_ONLY_R = 3
};

struct PatchParameters{
    ROTATION_METHOD rotation_method;
    double angle, sin_angle, cos_angle, scale;
    int grid_size, patch_nesting_factor,polygonRegularizationIteration;
    bool local_patch_nesting, regularizePolygons;
    PatchParameters():
        rotation_method(ALL_UNIFORMLY_R),

        angle(0.0),
        sin_angle(0.0),
        cos_angle(1.0),
        scale(4.0/3.0),

        grid_size(6),
        patch_nesting_factor(0),
        polygonRegularizationIteration(0),

        local_patch_nesting(false),
        regularizePolygons(false)
    {}
};

class PatchMode : public QWidget
{
    Q_OBJECT
public:
    explicit PatchMode(QWidget *parent = NULL);
    //outside this class only the value of this struct matters
    struct PatchParameters parameters;
private:
    QGridLayout *layout;
    QPushButton *resetBtn, *convert, *saveAsObj;
    QSlider *angleSlider, *scaleSlider, *sudivisionSlider, *polygonRegularizationIterationSlider;
    QRadioButton *allOpt, *alternateOpt, *edgesOpt, *vertexOpt;
    QLabel *angleLable, *scaleLable, *rotationLable, *angleTitle, *scaleTitle, *subDivisionLabel,
        *patchNestingFactorLabel, *patchNestingFactorNumberLabel, *polygonRegularizationIterationStringValue;
    QSpinBox *subDivisionBox;
    QCheckBox *localPatchNestingChecked, *renderPatchBoundaries, *showPatchControlMesh,
    *renderDooSabinPolygons, *renderModifiedDooSabinPolygons, *regularizePolygons;
    void updated();
    bool resetMode;

signals:
    void ParametersUpdated();
    void ConvertToTopMode();
    void PatchRenderUpdateRequest();
    void SaveAsOBJ();

private slots:
    void showPatchControlMeshChanged(bool value);
    void renderPatchBoundariesChanged(bool value);
    void renderDooSabinPolygonsChanged(bool value);
    void renderModifiedDooSabinPolygonsChanged(bool value);
    void regularizePolygonsChanged(bool value);
    void polygonRegularizationIterationSliderChanged(const int &value);

    //void vertexMovementFactorChanged(int value);
    void patchNestingMethodChanged(const bool &value);
    void subDivisionChanged(const int &value);
    void patchNestingFactorChanged(const int &value);
    void resetParameters();
    void scaleValueChanged(int value);
    void angleValueChanged(int value);
    void allOptSelected();
    void alternateOptSelected();
    void edgesOptSelected();
    void vertexOptSelected();
    void convertTriggered();
    void saveAsObjTriggered();
};

#endif // PATCHMODE_HH
