#ifndef MEASUREMENTSPREFERENCES_H
#define MEASUREMENTSPREFERENCES_H

#include <QDialog>
#include "measurementsmodel.h"
#include "referencelist.h"
#include <QDoubleSpinBox>
#include <QButtonGroup>
#include <QRadioButton>

namespace Ui {
    class MeasurementsPreferences;
}

class MeasurementsPreferences : public QDialog
{
    Q_OBJECT

public:
    MeasurementsPreferences(MeasurementsModel *measurement, ReferenceList *list,QWidget *parent = 0);
    ~MeasurementsPreferences();

private slots:
    void accept(void);
    void maxminstep_enabled(QAbstractButton* button);

private:
    Ui::MeasurementsPreferences *ui;
    MeasurementsModel *measurement;
    ReferenceList *list;
    QButtonGroup *group;
    QSpinBox *spin_iterations;
    QRadioButton *radio_none;
    QList<QDoubleSpinBox*> list_start;
    QList<QDoubleSpinBox*> list_end;
    QList<QDoubleSpinBox*> list_step;
    QList<QRadioButton*> list_radio;
};

#endif // MEASUREMENTSPREFERENCES_H
