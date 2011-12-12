#ifndef ZEROMODEL_H
#define ZEROMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QTextStream>
#include "measure.h"
#include "common.h"

#define NVARS_ZERO 6

class ZeroModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZeroModel(QObject *parent = 0);

//    void load(QTextStream *in);
//    void save(QTextStream *out);
//    void save_csv(QTextStream *out,bool header);

    QVector<double> force[NVARS_ZERO];

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // metadata about the measurement
    QString name;
    QString description;
    int dvm_time;
    matrix_t matrix;
    int average_number;
    double set_alpha;
    double set_beta;
    double set_wind;

public slots:
    void GetMeasure(measure m);

};

#endif // ZEROMODEL_H




