#ifndef FORCE_H
#define FORCE_H

#include "variable.h"
#include <QString>
#include <QtGui>

#define NUMCHANNELS 6
#define ANZ_QUA_ELE 21

enum matrix_t{
    MIDDLE=0,
    FLOOR
};

#pragma pack(1)
struct matrix		      /* abgespeicherte Kalibriermatrix */
{
        double coef_lin[NUMCHANNELS][NUMCHANNELS];
        double coef_qua [ANZ_QUA_ELE][NUMCHANNELS];
} ;
#pragma pack()


class ForceMeta: public VariableMeta{
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

class ForcePreferences: public VariablePreferences{
public:
    ForcePreferences();
    ~ForcePreferences();
    QWidget* get_widget();
    bool accept_config();
    bool is_configurable();
private:
    QComboBox *combo_time;
    QLineEdit *edit;
    QComboBox *combo_matrix;
    QCheckBox *checkbox;
};
class ForceModel : public VariableModel {
public:
    ForceModel();
    ~ForceModel();
    QWidget* view_get_widget();
    QWidget* measurement_get_widget();
    bool measurement_accept_config(VariableModel *m);
    bool measurement_is_configurable();
    void save_xml(QDomElement root);
    void load_xml(QDomElement root);

    int dvm_time;
    matrix_t matrix;
private:
    QComboBox *combo_time;
    QComboBox *combo_matrix;
};

class ForceHardware: public VariableHardware {
public:
    ForceHardware(VariableModel* v);
    ~ForceHardware();
    void read();
    double get_value(int n);
    void set_value(int n ,double value);
    double get_raw_value(int n);
    bool isReady(void);
    bool has_set_final() ;
    void set_final() ;
    void set_zero(QVector<double> zero);
private:
    QVector<double> zero;
    int dvm_time;
    matrix_t matrix;
    QString filename;

    double forces[NUMCHANNELS];
    double dvm_si[NUMCHANNELS];
//    double zero[NUMCHANNELS];
    double dvm_si_zero[NUMCHANNELS];
    double dvm[NUMCHANNELS];

    double nominal_load[NUMCHANNELS];
    struct matrix coe;
    struct matrix mat;

    void initialize();

    double ascii2newton (char *buf);
    void read_dvm(void);
    void convert_dvm();

    void calc_jacobi(double force[],double jm[NUMCHANNELS][NUMCHANNELS]);
    void calc_function(double F[NUMCHANNELS]);
    bool check_tolerance(double F[NUMCHANNELS]);
    void newton_method();

    int g_id;
    int k;
};
class ForceFactory: public QObject,public Factory {
    Q_OBJECT
    Q_INTERFACES(Factory)
public:
    VariableMeta* CreateVariableMeta();
    VariablePreferences* CreateVariableGUI();
    VariableModel* CreateVariableModel();
    VariableHardware* CreateVariableHardware(VariableModel *v);
};

#endif // FORCE_H
