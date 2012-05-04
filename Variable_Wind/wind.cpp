#include "wind.h"
#include "arduino-serial.h"

#include <QSettings>

#define WIND_SENSITIVITY 3.98
#define MMH2O_TO_PASCAL 9.80665

#define ARDUINO_ANALOG_REF 5.0
#define SETTINGS_ARDUINO_PATH "arduino_path"
#define SETTINGS_ARDUINO_PATH_DEFAULT "/dev/ttyUSB0"

#define NUM_CHANNELS 5
#define SETTINGS_WIND "wind_channel"
#define SETTINGS_WIND_DEFAULT 1
#define TAG_WIND_CHANNEL "wind_channel"

bool WindMeta::is_controlable() {
    return false;
}
bool WindMeta::has_zero() {
    return false;
}
int WindMeta::get_num() {
    return 1;
}
QString WindMeta::get_general_name(){
    return "Wind";
}
QString WindMeta::get_general_name_tr(){
    return QString(QObject::tr("Wind"));
}
QString WindMeta::get_name(int n){
    Q_UNUSED(n);
    return "Wind";
}
QString WindMeta::get_name_tr(int n){
    Q_UNUSED(n);
    return QString(QObject::tr("Wind"));
}
QString WindMeta::get_units(int n) {
    Q_UNUSED(n);
    return QString(QObject::tr("mmH20"));
}
double WindMeta::get_lower_bound(int n) {
    Q_UNUSED(n);
    return 0;
}
double WindMeta::get_upper_bound(int n) {
    Q_UNUSED(n);
    return 0;
}
double WindMeta::get_smaller_step(int n) {
    Q_UNUSED(n);
    return 0;
}
double WindMeta::get_default_step(int n) {
    Q_UNUSED(n);
    return 0;
}
double WindMeta::get_default_start(int n) {
    Q_UNUSED(n);
    return 0;
}


WindPreferences::WindPreferences() {
    meta = new WindMeta();
}
QWidget* WindPreferences::get_widget() {
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    QSettings settings;
    QAbstractButton *button;
    group = new QButtonGroup;
    int half = (NUM_CHANNELS%2==0)? NUM_CHANNELS/2 : (NUM_CHANNELS+1)/2;
    for (int k=1; k<= NUM_CHANNELS; k++){
        button = new QRadioButton(QString(QObject::tr("channel %1")).arg(k));
        group->addButton(button,k);
        layout->addWidget(button,(k-1)%half,((k<=half)? 0:1));
    }
    button = group->button(settings.value(SETTINGS_WIND,SETTINGS_WIND_DEFAULT).toInt());
    if(button != 0){
        button->setChecked(true);
    } else {
        group->buttons().first()->setChecked(true);
    }

    widget->setLayout(layout);
    return widget;
}
bool WindPreferences::accept_config() {
    QSettings settings;
    settings.setValue(SETTINGS_WIND,group->checkedId());
    return true;
}
bool WindPreferences::is_configurable() {
    return true;
}


WindModel::WindModel(){
    meta = new WindMeta;
}
int WindModel::get_size() {
    return contents.size();
}
double WindModel::get_value(int n,int row) {
    Q_UNUSED(n);
    return contents.value(row);
}
QVector<double> WindModel::get_vector(int n) {
    Q_UNUSED(n);
    return contents;
}
void WindModel::set_value(int n ,int row, double value) {
    Q_UNUSED(n);
    contents.replace(row,value);
}
void WindModel::insert_value(int n, int row, int count, double value) {
    Q_UNUSED(n);
    contents.insert(row,count,value);
}
void WindModel::append_value(int n, double value) {
    Q_UNUSED(n);
    contents.append(value);
}
double WindModel::get_raw_value(int n,int row){
    Q_UNUSED(n);
    return raw.value(row);
}
void WindModel::set_raw_value(int n ,int row, double value){
    Q_UNUSED(n);
    raw.replace(row,value);
}
void WindModel::insert_raw_value(int n, int row, int count, double value) {
    Q_UNUSED(n);
    raw.insert(row,count,value);
}
void WindModel::append_raw_value(int n, double value) {
    Q_UNUSED(n);
    raw.append(value);
}
void WindModel::set_zero(QVector<double> zero) {
    Q_UNUSED(zero);
}
QVector<double> WindModel::get_zero() {
    return QVector<double>();
}
QWidget* WindModel::view_get_widget(){
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(new QLabel(QObject::tr("Channel")));
    layout->addWidget(new QLabel(QString::number(channel)));
    widget->setLayout(layout);
    return widget;
}
QWidget* WindModel::measurement_get_widget(){
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    QSettings settings;
    QAbstractButton *button;
    group = new QButtonGroup;
    int half = (NUM_CHANNELS%2==0)? NUM_CHANNELS/2 : (NUM_CHANNELS+1)/2;
    for (int k=1; k<= NUM_CHANNELS; k++){
        button = new QRadioButton(QString(QObject::tr("channel %1")).arg(k));
        group->addButton(button,k);
        layout->addWidget(button,(k-1)%half,((k<=half)? 0:1));
    }
    button = group->button(settings.value(SETTINGS_WIND,SETTINGS_WIND_DEFAULT).toInt());
    if(button != 0){
        button->setChecked(true);
    } else {
        group->buttons().first()->setChecked(true);
    }
    widget->setLayout(layout);
    return widget;
}
bool WindModel::measurement_accept_config(VariableModel *m){
    Q_UNUSED(m);
    channel = group->checkedId();
    // it really does not make sense to check unless it is included in the reference
/*
    if (m != NULL){
        if (channel != dynamic_cast<WindModel*>(m)->channel){
            QMessageBox message;
            message.setText("The channels are different.");
            message.exec();
        }
    }
*/
    return true;
}
bool WindModel::measurement_is_configurable(){
    return true;
}
void WindModel::save_xml(QDomElement root){
    QDomElement channel_element = root.ownerDocument().createElement(TAG_WIND_CHANNEL);
    channel_element.appendChild(root.ownerDocument().createTextNode(QString::number(this->channel)));
    root.appendChild(channel_element);
}
void WindModel::load_xml(QDomElement root){
    QDomNodeList nodelist = root.childNodes();
    QDomNode node;
    QDomElement element;
    for (int k=0; k< nodelist.count();k++){
        node = nodelist.at(k);
        element = node.toElement();
        if (element.tagName() == TAG_WIND_CHANNEL){
            this->channel = element.text().toInt();
            continue;
        }
    }
}


WindHardware::WindHardware(VariableModel* v) {
    meta = new WindMeta;
    wind=0;
    wind_raw=0;
    channel = dynamic_cast<WindModel*>(v)->channel;
    QSettings settings;
    arduinofd = serialport_init(settings.value(SETTINGS_ARDUINO_PATH).toString().toStdString().c_str(),SERIALRATE);

    char buffer[20];
    sprintf(buffer,"$CS%dxxx\n",channel);
    serialport_write(arduinofd, buffer);
    serialport_write(arduinofd, "$CExxxx\n");


}
WindHardware::~WindHardware() {
  serialport_write(arduinofd, "$CDxxxx\n");
  close(arduinofd);
}
void WindHardware::read() {
    char buffer_read[256]="", buffer_aux[256];
    char buffer[] = "$A5xxxx\n";               // read from analog 5
    bool sucess=false;

    // response form: $CIXXXX\n
    // $ is a start byte
    // \n is a stop byte

    while (sucess == false){
        serialport_flush(arduinofd);
        if( serialport_write(arduinofd, buffer) == -1){
            perror("Wind writing");
        }

        serialport_read_until(arduinofd, buffer_read, '\n');

        if (strncmp(buffer,buffer_read,3)==0){
            strncpy(buffer_aux,buffer_read + 3,4);
            wind_raw = atoi(buffer_aux);
            sucess=true;
        }
    }
    // 5.0 V
    // 10 bits = 1024
    // 19.9 mm H20 = 5 V => 3.98 mm H20/V
    // 1 mm H20 = 9.80665 Pa
//    wind = wind_raw * ARDUINO_ANALOG_REF/1024.0 * WIND_SENSITIVITY * MMH2O_TO_PASCAL ;
    wind = wind_raw * ARDUINO_ANALOG_REF/1024.0 * WIND_SENSITIVITY ;
}
double WindHardware::get_value(int n) {
    Q_UNUSED(n);
    return wind;
}
void WindHardware::set_value(int n ,double value) {
    Q_UNUSED(n);
    Q_UNUSED(value);
}
double WindHardware::get_raw_value(int n){
    Q_UNUSED(n);
    return wind_raw;
}
bool WindHardware::isReady(void) {
    return true;
}
bool WindHardware::has_set_final() {
    return meta->is_controlable() && false;
}
void WindHardware::set_final() {
}
void WindHardware::set_zero(QVector<double> zero) {
    Q_UNUSED(zero);
}



VariableMeta* WindFactory::CreateVariableMeta() {
    return new WindMeta;
}
VariablePreferences* WindFactory::CreateVariableGUI() {
    return new WindPreferences;
}
VariableModel* WindFactory::CreateVariableModel() {
    return new WindModel;
}
VariableHardware* WindFactory::CreateVariableHardware(VariableModel *v) {
    return new WindHardware(v);
}


Q_EXPORT_PLUGIN2(40_wind, WindFactory);

