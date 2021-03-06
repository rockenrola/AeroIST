#ifndef REFERENCEMODEL_H
#define REFERENCEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QTextStream>
#include <QDomElement>
#include <QStringList>


#include "common.h"
#include "variable.h"

class ReferenceModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ReferenceModel(QObject *parent = 0);
    explicit ReferenceModel(QDomElement root,QObject *parent = 0);
    ~ReferenceModel();
    void load_xml(QDomElement root);
    void save_xml(QDomElement root);

    QList<VariableModel*> variables;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    bool insertRow ( int row, const QModelIndex & parent = QModelIndex() );

    // metadata about the measurement
    QString name;
    QString description;
    int measures_per_iteration;

public slots:
    void GetMeasure(QHash<QString, double> m_hash, QHash<QString,double> raw_hash);
private:
    void init(void);
};

#endif // REFERENCEMODEL_H




