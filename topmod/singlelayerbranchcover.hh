#ifndef SINGLELAYERBRANCHCOVER_HH
#define SINGLELAYERBRANCHCOVER_HH

#include <QGridLayout>
#include <QWidget>
class SingleLayerBranchCover : public QWidget {
  Q_OBJECT
  QGridLayout *layout;

public:
  explicit SingleLayerBranchCover(QWidget *parent = NULL);

signals:

public slots:
};

#endif // SINGLELAYERBRANCHCOVER_HH
