#include "BranchCovering_CornerCutting_Widget.hh"
#include <QDebug>
#include <QString>
#define SLIDER_MAX 10000
#define PI 3.14159265358979323846264338327950288419716939937510582097494
static QString SYMBOLS[3] = {"Forward", "Cross", "backward"};

BranchCovering_CornerCutting_Widget::BranchCovering_CornerCutting_Widget(
    QWidget *parent)
    : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  shrinking_factor_title = new QLabel(
      "<b>Shrink:</b> " + QString::number(parameters.shrinking_factor, 'g', 3),
      this);
  QSlider *shrinking_factor_slider = new QSlider(Qt::Horizontal, this);
  shrinking_factor_slider->setMinimum(1);
  shrinking_factor_slider->setMaximum(SLIDER_MAX);
  shrinking_factor_slider->setValue(
      static_cast<int>(parameters.shrinking_factor * SLIDER_MAX));
  connect(shrinking_factor_slider, SIGNAL(valueChanged(int)), this,
          SLOT(update_shrinking_factor(int)));
  {
    QHBoxLayout *row = new QHBoxLayout;
    row->addWidget(shrinking_factor_title);
    row->addStretch(2);
    row->addWidget(shrinking_factor_slider);
    layout->addLayout(row);
  }

  rotation_factor_title =
      new QLabel("<b>Rotate:</b> " +
                     QString::number(parameters.rotation * 180 / PI, 'g', 3),
                 this);
  QSlider *rotation_factor_slider = new QSlider(Qt::Horizontal, this);
  rotation_factor_slider->setMinimum(-SLIDER_MAX);
  rotation_factor_slider->setMaximum(SLIDER_MAX);
  rotation_factor_slider->setValue((int)parameters.rotation);
  connect(rotation_factor_slider, SIGNAL(valueChanged(int)), this,
          SLOT(update_rotation_factor(int)));
  {
    QHBoxLayout *row = new QHBoxLayout;
    row->addWidget(rotation_factor_title);
    row->addStretch(2);
    row->addWidget(rotation_factor_slider);
    layout->addLayout(row);
  }

  {
    QLabel *thickness_title = new QLabel(tr("<b>Thickness:</b>"), this);
    QDoubleSpinBox *thickness_box = new QDoubleSpinBox(this);
    thickness_box->setMinimum(0.001);
    thickness_box->setMaximum(100);
    thickness_box->setValue(parameters.thickness);
    thickness_box->setDecimals(3);
    thickness_box->setSingleStep(0.1);
    connect(thickness_box, SIGNAL(valueChanged(double)), this,
            SLOT(update_thickness(double)));
    {
      QHBoxLayout *row = new QHBoxLayout;
      row->addWidget(thickness_title);
      row->addStretch(2);
      row->addWidget(thickness_box);
      layout->addLayout(row);
    }
  }

  {
    QLabel *panel_offset_title = new QLabel(tr("<b>Offset:</b>"), this);
    QDoubleSpinBox *panel_offset_box = new QDoubleSpinBox(this);
    panel_offset_box->setMinimum(0.000);
    panel_offset_box->setMaximum(100);
    panel_offset_box->setValue(parameters.panel_offset);
    panel_offset_box->setDecimals(3);
    panel_offset_box->setSingleStep(0.1);
    connect(panel_offset_box, SIGNAL(valueChanged(double)), this,
            SLOT(update_panel_offset(double)));
    {
      QHBoxLayout *row = new QHBoxLayout;
      row->addWidget(panel_offset_title);
      row->addStretch(2);
      row->addWidget(panel_offset_box);
      layout->addLayout(row);
    }
  }

  {
    QLabel *panel_alternation_offset_title =
        new QLabel(tr("<b>Alternation Offset:</b>"), this);
    QDoubleSpinBox *panel_alternation_offset_box = new QDoubleSpinBox(this);
    panel_alternation_offset_box->setMinimum(0.000);
    panel_alternation_offset_box->setMaximum(100);
    panel_alternation_offset_box->setValue(parameters.panel_alternation_offset);
    panel_alternation_offset_box->setDecimals(3);
    panel_alternation_offset_box->setSingleStep(0.1);
    connect(panel_alternation_offset_box, SIGNAL(valueChanged(double)), this,
            SLOT(update_panel_alternation_offset(double)));
    {
      QHBoxLayout *row = new QHBoxLayout;
      row->addWidget(panel_alternation_offset_title);
      row->addStretch(2);
      row->addWidget(panel_alternation_offset_box);
      layout->addLayout(row);
    }
  }

  {
    QWidget *line = new QWidget(this);
    line->setFixedHeight(1);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet(QString("background-color: #c0c0c0;"));
    layout->addWidget(line);
  }

  QPushButton *add_forward_btn = new QPushButton(tr("Forward"), this);
  connect(add_forward_btn, SIGNAL(pressed()), this, SLOT(add_forward_symbol()));
  QPushButton *add_backward_btn = new QPushButton(tr("Backward"), this);
  connect(add_backward_btn, SIGNAL(pressed()), this,
          SLOT(add_backward_symbol()));
  QPushButton *add_cross_btn = new QPushButton(tr("Cross"), this);
  connect(add_cross_btn, SIGNAL(pressed()), this, SLOT(add_cross_symbol()));
  QPushButton *move_up_symbol_btn = new QPushButton(tr("Move Up"), this);
  connect(move_up_symbol_btn, SIGNAL(pressed()), this, SLOT(move_up_symbol()));
  QPushButton *move_down_symbol_btn = new QPushButton(tr("Move Down"), this);
  connect(move_down_symbol_btn, SIGNAL(pressed()), this,
          SLOT(move_down_symbol()));
  QPushButton *remove_symbol_btn = new QPushButton(tr("Remove"), this);
  connect(remove_symbol_btn, SIGNAL(pressed()), this, SLOT(remove_symbol()));

  mapping_view = new QListWidget(this);
  mapping_view->setSelectionMode(QAbstractItemView::SingleSelection);
  {
    new QListWidgetItem(SYMBOLS[0], mapping_view);
    new QListWidgetItem(SYMBOLS[1], mapping_view);
    new QListWidgetItem(SYMBOLS[2], mapping_view);
  }
  update_stack();
  {
    QHBoxLayout *mapping_layout = new QHBoxLayout;
    layout->addLayout(mapping_layout);
    {
      QVBoxLayout *column = new QVBoxLayout;
      column->addStretch(2);
      column->addWidget(new QLabel(tr("<b>Add</b>"), this), 0, Qt::AlignLeft);
      column->addWidget(add_forward_btn);
      column->addWidget(add_cross_btn);
      column->addWidget(add_backward_btn);
      column->addStretch(2);
      column->addWidget(new QLabel(tr("<b>Edit</b>"), this), 0, Qt::AlignLeft);
      column->addWidget(move_up_symbol_btn);
      column->addWidget(move_down_symbol_btn);
      column->addWidget(remove_symbol_btn);
      column->addStretch(2);
      mapping_layout->addLayout(column);
    }
    {
      QVBoxLayout *column = new QVBoxLayout;
      column->addWidget(new QLabel(tr("<b>Edge Mapping</b>"), this), 0,
                        Qt::AlignLeft);
      column->addWidget(mapping_view);
      mapping_layout->addLayout(column);
    }
  }

  {
    QWidget *line = new QWidget(this);
    line->setFixedHeight(1);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet(QString("background-color: #c0c0c0;"));
    layout->addWidget(line);
  }
  {
    QPushButton *apply_btn = new QPushButton(tr("Apply"), this);
    connect(apply_btn, SIGNAL(pressed()), this, SLOT(apply()));
    layout->addWidget(apply_btn, 0, Qt::AlignHCenter);
  }
}

void BranchCovering_CornerCutting_Widget::update_shrinking_factor(
    const int &value) {
  parameters.shrinking_factor = (static_cast<double>(value)) / SLIDER_MAX;
  shrinking_factor_title->setText(
      "<b>Shrink:</b> " + QString::number(parameters.shrinking_factor, 'g', 3));
}

void BranchCovering_CornerCutting_Widget::update_rotation_factor(
    const int &value) {
  parameters.rotation = (static_cast<double>(value)) / SLIDER_MAX;
  rotation_factor_title->setText(
      "<b>Rotate:</b> " + QString::number(parameters.rotation * 180, 'g', 3));
  parameters.rotation *= PI;
}

void BranchCovering_CornerCutting_Widget::update_thickness(
    const double &value) {
  parameters.thickness = value;
}

void BranchCovering_CornerCutting_Widget::update_panel_offset(
    const double &value) {
  parameters.panel_offset = value;
}

void BranchCovering_CornerCutting_Widget::update_panel_alternation_offset(
    const double &value) {
  parameters.panel_alternation_offset = value;
}

void BranchCovering_CornerCutting_Widget::add_forward_symbol() {
  int row = mapping_view->currentRow();
  if (row < 0) {
    new QListWidgetItem(SYMBOLS[0], mapping_view);
  } else {
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(SYMBOLS[0]);
    mapping_view->insertItem(row, newItem);
  }
  mapping_view->setCurrentRow(-1);
  update_stack();
}

void BranchCovering_CornerCutting_Widget::add_backward_symbol() {
  int row = mapping_view->currentRow();
  if (row < 0) {
    new QListWidgetItem(SYMBOLS[2], mapping_view);
  } else {
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(SYMBOLS[2]);
    mapping_view->insertItem(row, newItem);
  }
  mapping_view->setCurrentRow(-1);
  update_stack();
}

void BranchCovering_CornerCutting_Widget::add_cross_symbol() {
  int row = mapping_view->currentRow();
  if (row < 0) {
    new QListWidgetItem(SYMBOLS[1], mapping_view);
  } else {
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(SYMBOLS[1]);
    mapping_view->insertItem(row, newItem);
  }
  mapping_view->setCurrentRow(-1);
  update_stack();
}

void BranchCovering_CornerCutting_Widget::move_up_symbol() {
  int row = mapping_view->currentRow();
  if (row < 1)
    return;
  QListWidgetItem *item = mapping_view->takeItem(row);
  mapping_view->insertItem(row - 1, item);
  mapping_view->setCurrentRow(row - 1);
  update_stack();
}

void BranchCovering_CornerCutting_Widget::move_down_symbol() {
  int row = mapping_view->currentRow();
  if (row == mapping_view->count() - 1)
    return;
  QListWidgetItem *item = mapping_view->takeItem(row);
  mapping_view->insertItem(row + 1, item);
  mapping_view->setCurrentRow(row + 1);
  update_stack();
}

void BranchCovering_CornerCutting_Widget::remove_symbol() {
  int row = mapping_view->currentRow();
  if (row < 0)
    return;
  QListWidgetItem *item = mapping_view->takeItem(row);
  delete item;
  item = NULL;
  mapping_view->setCurrentRow(-1);
  update_stack();
}

void BranchCovering_CornerCutting_Widget::apply() {
  emit Apply(mapping_operations, parameters);
}

void BranchCovering_CornerCutting_Widget::update_stack() {
  mapping_operations.clear();
  for (int i = 0; i < mapping_view->count(); ++i) {
    QString str = mapping_view->item(i)->text();
    if (str == SYMBOLS[0])
      mapping_operations.push_back(HALF_EDGE_FORWARD);
    else if (str == SYMBOLS[1])
      mapping_operations.push_back(HALF_EDGE_CROSS);
    else /*(str == SYMBOLS[2])*/
      mapping_operations.push_back(HALF_EDGE_BACKWARD);
  }
  //    for(HALF_EDGE_MAPPING op: mapping_operations){
  //        switch (op) {
  //        case HALF_EDGE_FORWARD:
  //            qDebug()<<"Forward";
  //            break;
  //        case HALF_EDGE_CROSS:
  //            qDebug()<<"Cross";
  //            break;
  //        case HALF_EDGE_BACKWARD:
  //            qDebug()<<"Backward";
  //            break;
  //        }
  //    }
  //    qDebug()<<"\n\n\n\n\n";
}
