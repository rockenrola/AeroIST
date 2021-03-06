#include "virtual_motor.h"

#define MOTOR_MIN    0
#define MOTOR_MAX    100 //Percentage
#define MOTOR_MIN_STEP 1
#define DEFAULT_MOTOR_STEP   1

#define SETTINGS_MOTOR_PATH "motor_path"
#define SETTINGS_MOTOR_PATH_DEFAULT "/dev/ttyS0"


bool MotorMeta::is_controlable() {return true;}
bool MotorMeta::has_zero() {return false;}
int MotorMeta::get_num() {return 1;}
QString MotorMeta::get_general_name(){ return "Motor";}
QString MotorMeta::get_general_name_tr(){
    return QString(QObject::tr("Motor"));
}
QString MotorMeta::get_name(int n){Q_UNUSED(n);  return "Motor"; }
QString MotorMeta::get_name_tr(int n){
    Q_UNUSED(n);
    return QString(QObject::tr("Motor"));
}
QString MotorMeta::get_units(int n) {
    Q_UNUSED(n);
    return QString(QObject::tr("%"));
}
QString MotorMeta::get_raw_units(int n) {
    Q_UNUSED(n);
    return QString(QObject::tr("%"));
}
double MotorMeta::get_lower_bound(int n) {Q_UNUSED(n); return MOTOR_MIN;}
double MotorMeta::get_upper_bound(int n) {Q_UNUSED(n); return MOTOR_MAX;}
double MotorMeta::get_smaller_step(int n) {Q_UNUSED(n); return MOTOR_MIN_STEP;}
double MotorMeta::get_default_step(int n) {Q_UNUSED(n); return DEFAULT_MOTOR_STEP;}
double MotorMeta::get_default_start(int n) {Q_UNUSED(n); return 0;}

MotorPreferences::MotorPreferences() {
}
MotorPreferences::~MotorPreferences(){
}
QWidget* MotorPreferences::get_widget() {
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    QSettings settings;
    layout->addWidget(new QLabel(QObject::tr("Motor device")),0,0);
    edit_motor = new QLineEdit;
    edit_motor->setText(settings.value(SETTINGS_MOTOR_PATH,SETTINGS_MOTOR_PATH_DEFAULT).toString());
    layout->addWidget(edit_motor,0,1);
    button = new QToolButton;
    QObject::connect(button,SIGNAL(clicked()),this,SLOT(button_slot()));
    layout->addWidget(button,0,2);
    widget->setLayout(layout);
    return widget;
}

bool MotorPreferences::accept_config() {
    QSettings settings;
    settings.setValue(SETTINGS_MOTOR_PATH, edit_motor->text());
    return true;
}
bool MotorPreferences::is_configurable() {return true;}


void MotorPreferences::button_slot(){
    edit_motor->setText(QFileDialog::getOpenFileName(NULL, tr("Choose device"),"/dev", ""));
}

MotorModel::MotorModel(){
}
MotorModel::~MotorModel(){
}
QWidget* MotorModel::view_get_widget(){ return NULL;}
QWidget* MotorModel::measurement_get_widget(){return NULL;}
bool MotorModel::measurement_accept_config(VariableModel *m){Q_UNUSED(m); return true;}
bool MotorModel::measurement_is_configurable(){return false;}
void MotorModel::save_xml(QDomElement root){Q_UNUSED(root);}
void MotorModel::load_xml(QDomElement root){Q_UNUSED(root);}


MotorHardware::MotorHardware () {
    value=0;
    control_set=false;
}
MotorHardware::~MotorHardware(){
}
void MotorHardware::read() {if(control_set==false){raw = (1.0 * qrand() / RAND_MAX); value = -30.0 * raw + 5;}}
double MotorHardware::get_value(int n) {Q_UNUSED(n); return value;}
double MotorHardware::get_raw_value(int n) {Q_UNUSED(n); return raw;}
void MotorHardware::set_value(int n ,double value) {Q_UNUSED(n);  control_set=true; this->value = value;this->raw = value/-30.0 - 5;}
QString MotorHardware::get_name(int n) {Q_UNUSED(n); return "Motor";}
bool MotorHardware::isReady(void) {return true;}
bool MotorHardware::has_set_final() {return meta->is_controlable() && true;}
void MotorHardware::set_final() {set_value(0,0);}
void MotorHardware::set_zero(QVector<double> zero) {Q_UNUSED(zero);}


VariableMeta* MotorFactory::CreateVariableMeta() { return new MotorMeta;}
VariablePreferences* MotorFactory::CreateVariableGUI() { return new MotorPreferences;}
VariableModel* MotorFactory::CreateVariableModel() { return new MotorModel;}
VariableHardware* MotorFactory::CreateVariableHardware(VariableModel *v) {Q_UNUSED(v); return new MotorHardware;}

Q_EXPORT_PLUGIN2(30_motor, MotorFactory);
