#include "virtual_force.h"

#define SETTINGS_DEFAULT_MATRIX "default_matrix"
#define SETTINGS_DEFAULT_DVM_TIME "default_dvm_time"
#define SETTINGS_FORCES_MATRIX_FLOOR "forces/matrix_floor"
#define SETTINGS_FORCES_MATRIX_FLOOR_DEFAULT "matrix 1.coe"
#define SETTINGS_FORCES_MATRIX_MIDDLE "forces/matrix_middle"
#define SETTINGS_FORCES_MATRIX_MIDDLE_DEFAULT "matrix 2.coe"
#define SETTINGS_MULTIMETER_PATH "multimeter_path"
#define SETTINGS_MULTIMETER_PATH_DEFAULT "voltmeter"
#define SETTINGS_MATRIX_ALLOW "matrix_allow"

#define TAG_DVM_TIME "dvm_time"
#define TAG_MATRIX "matrix"

bool ForceMeta::is_controlable() {return false;}
bool ForceMeta::has_zero() {return true;}
int ForceMeta::get_num() {return 6;}
QString ForceMeta::get_general_name(){ return "Forces";}
QString ForceMeta::get_general_name_tr(){
    return QString(QObject::tr("Forces"));
}
QString ForceMeta::get_name(int n){
    switch (n){
    case 0: return "Fx"; break;
    case 1: return "Fy"; break;
    case 2: return "Fz"; break;
    case 3: return "Mx"; break;
    case 4: return "My"; break;
    case 5: return "Mz"; break;
    }
    return "";
}
QString ForceMeta::get_name_tr(int n){
    switch (n){
    case 0: return QString(QObject::tr("Fx")); break;
    case 1: return QString(QObject::tr("Fy")); break;
    case 2: return QString(QObject::tr("Fz")); break;
    case 3: return QString(QObject::tr("Mx")); break;
    case 4: return QString(QObject::tr("My")); break;
    case 5: return QString(QObject::tr("Mz")); break;
    }
    return "";
}
QString ForceMeta::get_units(int n) {
    switch (n){
    case 0:
    case 1:
    case 2:
        return QString(QObject::tr("N")); break;
    case 3:
    case 4:
    case 5:
        return QString(QObject::tr("N.m")); break;
    }
    return "";
}
QString ForceMeta::get_raw_units(int n) {
    Q_UNUSED(n);
    return QString(QObject::tr("V"));
}
double ForceMeta::get_lower_bound(int n) {Q_UNUSED(n); return 0;}
double ForceMeta::get_upper_bound(int n) {Q_UNUSED(n); return 0;}
double ForceMeta::get_smaller_step(int n) {Q_UNUSED(n); return 0;}
double ForceMeta::get_default_step(int n) {Q_UNUSED(n); return 0;}
double ForceMeta::get_default_start(int n) {Q_UNUSED(n); return 0;}


ForcePreferences::ForcePreferences(){
}
ForcePreferences::~ForcePreferences(){
}
QWidget* ForcePreferences::get_widget() {
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    QSettings settings;
    layout->addWidget(new QLabel(QObject::tr("Multimeter")),0,0);
    edit = new QLineEdit(settings.value(SETTINGS_MULTIMETER_PATH, SETTINGS_MULTIMETER_PATH_DEFAULT).toString());
    layout->addWidget(edit,0,1);
    layout->addWidget(new QLabel(QObject::tr("Default Multimeter time")),1,0);
    combo_time = new QComboBox;
    combo_time->addItem(QObject::tr("50 ms"),    1);
    combo_time->addItem(QObject::tr("100 ms"),   2);
    combo_time->addItem(QObject::tr("500 ms"),   3);
    combo_time->addItem(QObject::tr("1 s"),      4);
    combo_time->addItem(QObject::tr("5 s"),      5);
    combo_time->addItem(QObject::tr("10 s"),     6);
    int index = combo_time->findData(settings.value(SETTINGS_DEFAULT_DVM_TIME,4).toInt());
    combo_time->setCurrentIndex(index);
    layout->addWidget(combo_time,1,1);
    layout->addWidget(new QLabel(QObject::tr("Default Matrix")),2,0);
    combo_matrix =  new QComboBox;
    combo_matrix->addItem(QObject::tr("Middle"), MIDDLE);
    combo_matrix->addItem(QObject::tr("Floor"),  FLOOR);
    index = combo_matrix->findData(settings.value(SETTINGS_DEFAULT_MATRIX,MIDDLE).toInt());
    combo_matrix->setCurrentIndex(index);
    layout->addWidget(combo_matrix,2,1);

    checkbox = new QCheckBox(QObject::tr("Allow different matrixes between measurement and reference"));
    checkbox->setChecked(settings.value(SETTINGS_MATRIX_ALLOW,false).toBool());
    layout->addWidget(checkbox,3,0,1,2);

    widget->setLayout(layout);
    return widget;
}
bool ForcePreferences::accept_config() {
    QSettings settings;
    int index = combo_time->currentIndex();
    settings.setValue(SETTINGS_DEFAULT_DVM_TIME, combo_time->itemData(index).toInt());
    settings.setValue(SETTINGS_MULTIMETER_PATH, edit->text());
    index = combo_matrix->currentIndex();
    settings.setValue(SETTINGS_DEFAULT_MATRIX, combo_matrix->itemData(index).toInt());
    settings.setValue(SETTINGS_MATRIX_ALLOW,checkbox->isChecked());
    return true;
}
bool ForcePreferences::is_configurable() {return true;}


ForceModel::ForceModel(){
}
ForceModel::~ForceModel(){
}
QWidget* ForceModel::view_get_widget(){
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(QObject::tr("Multimeter time")),0,0);
    layout->addWidget(new QLabel(),0,1);

    switch (dvm_time){
    case 1:
        layout->addWidget(new QLabel(QObject::tr("50 ms")),0,1); break;
    case 2:
        layout->addWidget(new QLabel(QObject::tr("100 ms")),0,1); break;
    case 3:
        layout->addWidget(new QLabel(QObject::tr("500 ms")),0,1); break;
    case 4:
        layout->addWidget(new QLabel(QObject::tr("1 s")),0,1); break;
    case 5:
        layout->addWidget(new QLabel(QObject::tr("5 s")),0,1); break;
    case 6:
        layout->addWidget(new QLabel(QObject::tr("10 s")),0,1); break;
    }
    layout->addWidget(new QLabel(QObject::tr("Test Type")),1,0);
    switch (matrix){
    case MIDDLE:
        layout->addWidget(new QLabel(QObject::tr("Middle")),1,1); break;
    case FLOOR:
        layout->addWidget(new QLabel(QObject::tr("Floor")),1,1); break;
    }
    layout->addWidget(new QLabel(),1,1);
    widget->setLayout(layout);
    return widget;
}
QWidget* ForceModel::measurement_get_widget(){
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    QSettings settings;
    layout->addWidget(new QLabel(QObject::tr("Multimeter time")),0,0);
    combo_time = new QComboBox;
    combo_time->addItem(QObject::tr("50 ms"),    1);
    combo_time->addItem(QObject::tr("100 ms"),   2);
    combo_time->addItem(QObject::tr("500 ms"),   3);
    combo_time->addItem(QObject::tr("1 s"),      4);
    combo_time->addItem(QObject::tr("5 s"),      5);
    combo_time->addItem(QObject::tr("10 s"),     6);
    int index = combo_time->findData(settings.value(SETTINGS_DEFAULT_DVM_TIME,4).toInt());
    combo_time->setCurrentIndex(index);
    layout->addWidget(combo_time,0,1);
    layout->addWidget(new QLabel(QObject::tr("Test Type")),1,0);
    combo_matrix =  new QComboBox;
    combo_matrix->addItem(QObject::tr("Middle"), MIDDLE);
    combo_matrix->addItem(QObject::tr("Floor"),  FLOOR);
    index = combo_matrix->findData(settings.value(SETTINGS_DEFAULT_MATRIX).toInt());
    combo_matrix->setCurrentIndex(index);
    layout->addWidget(combo_matrix,1,1);
    widget->setLayout(layout);
    return widget;
}
bool ForceModel::measurement_accept_config(VariableModel *m){
    dvm_time = combo_time->itemData(combo_time->currentIndex()).toInt();
    matrix = (matrix_t) combo_matrix->currentIndex();
    QSettings settings;
    bool allow = settings.value(SETTINGS_MATRIX_ALLOW,false).toBool();
    if (m != NULL && allow == false){
        if (matrix != dynamic_cast<ForceModel*>(m)->matrix){
            QMessageBox message;
            message.setText("The reference matrix and the measurement matrix are different.");
            message.exec();
            return false;
        }
    }
    return true;
}
bool ForceModel::measurement_is_configurable(){return true;}
void ForceModel::save_xml(QDomElement root){
    QDomElement dvm_time = root.ownerDocument().createElement(TAG_DVM_TIME);
    dvm_time.appendChild(root.ownerDocument().createTextNode(QString::number(this->dvm_time)));
    root.appendChild(dvm_time);

    QDomElement matrix = root.ownerDocument().createElement(TAG_MATRIX);
    matrix.appendChild(root.ownerDocument().createTextNode(QString::number(this->matrix)));
    root.appendChild(matrix);
}
void ForceModel::load_xml(QDomElement root){
    QDomNodeList nodelist = root.childNodes();
    QDomNode node;
    QDomElement element;
    for (int k=0; k< nodelist.count();k++){
        node = nodelist.at(k);
        element = node.toElement();


        if (element.tagName() == TAG_DVM_TIME){
            this->dvm_time = element.text().toInt();
            continue;
        }
        if (element.tagName() == TAG_MATRIX){
            int m = element.text().toInt();
            switch (m){
            case FLOOR: this->matrix = FLOOR; break;
            case MIDDLE: this->matrix = MIDDLE; break;
            }
            continue;
        }
    }
}


ForceHardware::ForceHardware(VariableModel* v){
    dvm_time = dynamic_cast<ForceModel*>(v)->dvm_time;
    matrix = dynamic_cast<ForceModel*>(v)->matrix;
}
ForceHardware::~ForceHardware(){
}
void ForceHardware::read() { for (int k=0; k<6; k++) {raw[k]=(1.0 * qrand() / RAND_MAX); value[k] = ( 123.4 * (k+1) * raw[k] )- zero.value(k);}}
double ForceHardware::get_value(int n) { return value[n];}
double ForceHardware::get_raw_value(int n) {return raw[n];}
void ForceHardware::set_value(int n ,double value) {Q_UNUSED(n); Q_UNUSED(value);  }
bool ForceHardware::isReady(void) {return true;}
bool ForceHardware::has_set_final() {return meta->is_controlable() && false;}
void ForceHardware::set_final() {}
void ForceHardware::set_zero(QVector<double> zero) {this->zero = zero;}

VariableMeta* ForceFactory::CreateVariableMeta() { return new ForceMeta;}
VariablePreferences* ForceFactory::CreateVariableGUI() { return new ForcePreferences;}
VariableModel* ForceFactory::CreateVariableModel() { return new ForceModel;}
VariableHardware* ForceFactory::CreateVariableHardware(VariableModel *v) { return new ForceHardware(v);}

Q_EXPORT_PLUGIN2(10_force, ForceFactory);
