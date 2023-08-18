#ifndef WIDGET_H
#define WIDGET_H

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

enum HALF_EDGE_MAPPING {
  HALF_EDGE_FORWARD = 0,
  HALF_EDGE_CROSS = 1,
  HALF_EDGE_BACKWARD = 2
};

struct BranchCovering_CornerCutting_Parameters {
  double thickness, shrinking_factor, rotation, panel_offset,
      panel_alternation_offset;
  BranchCovering_CornerCutting_Parameters()
      : thickness(0.1), shrinking_factor(0.8), rotation(0), panel_offset(0.0),
        panel_alternation_offset(0.2) {}
};

class BranchCovering_CornerCutting_Widget : public QWidget {
  Q_OBJECT

  QListWidget *mapping_view;

  QLabel *shrinking_factor_title, *rotation_factor_title;
  // QStringList operations_string;
  /*PARAMETERS*/
  BranchCovering_CornerCutting_Parameters parameters;
  std::vector<HALF_EDGE_MAPPING> mapping_operations;

public:
  BranchCovering_CornerCutting_Widget(QWidget *parent = NULL);
  ~BranchCovering_CornerCutting_Widget() {}

signals:
  void Apply(const std::vector<HALF_EDGE_MAPPING> &mapping_guid,
             const BranchCovering_CornerCutting_Parameters &parameters);
private slots:
  void update_shrinking_factor(const int &value);
  void update_rotation_factor(const int &value);
  void update_thickness(const double &value);
  void update_panel_offset(const double &value);
  void update_panel_alternation_offset(const double &value);
  void add_forward_symbol();
  void add_backward_symbol();
  void add_cross_symbol();
  void move_up_symbol();
  void move_down_symbol();
  void remove_symbol();
  void apply();

private:
  void update_stack();
};

#endif // WIDGET_H
