#ifndef ALPHA_H
#define ALPHA_H

#include "variable.h"
#include <QString>
#include <QtGui>

#include "serialport.h"

class AnglesMeta : public VariableMeta {
public:
    bool is_controlable();
    bool has_zero();
    int get_num();
    QString get_general_name();
    QString get_general_name_tr();
    QString get_name(int n);
    QString get_name_tr(int n);
    QString get_units(int n);
    QString get_raw_units(int n);
    double get_lower_bound(int n);
    double get_upper_bound(int n);
    double get_smaller_step(int n);
    double get_default_step(int n);
    double get_default_start(int n);
};

class AnglesPreferences: public VariablePreferences {
public:
    AnglesPreferences();
    ~AnglesPreferences();
    QWidget* get_widget();
    bool accept_config();
    bool is_configurable();
private:
    QLineEdit *edit_angles;
};

class AnglesModel : public VariableModel {
public:
    AnglesModel();
    ~AnglesModel();
    QWidget* view_get_widget();
    QWidget* measurement_get_widget();
    bool measurement_accept_config(VariableModel *m);
    bool measurement_is_configurable();
    void save_xml(QDomElement root);
    void load_xml(QDomElement root);
};
class AnglesHardware: public VariableHardware {
public:
    AnglesHardware();
    ~AnglesHardware();
    void read();
    double get_value(int n);
    void set_value(int n ,double value);
    double get_raw_value(int n);
    bool isReady(void);
    bool has_set_final();
    void set_final();
    void set_zero(QVector<double> zero);
private:
    double sensitivity[2];
    SerialPort *port;
    char relay_increase[2];
    char relay_decrease[2];
    int arduinofd;
    double precision[2];
    int digits[2];
    int zero[2];
    double anglemax[2];
    double angle[2];
    int displays[2];
    void increase(int n,double angle_dest);
    void decrease(int n,double angle_dest);
    void convert_alpha(void);
    void convert_beta(void);
};
class AnglesFactory: public QObject,public Factory {
    Q_OBJECT
    Q_INTERFACES(Factory)
public:
    VariableMeta* CreateVariableMeta() ;
    VariablePreferences* CreateVariableGUI() ;
    VariableModel* CreateVariableModel() ;
    VariableHardware* CreateVariableHardware(VariableModel *v);
};

#endif // ALPHA_H
