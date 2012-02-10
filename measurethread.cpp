#include "measurethread.h"

#include <cstdlib> // for RAND_MAX

#include <QEventLoop>
#include <QDebug>
#include <QSettings>
#include <stdexcept>
#include "helper.h"

#include "virtualvariables.h"

MeasureThread::MeasureThread(MeasurementsModel *measurement,QObject *parent) :
    QObject(parent),
    start_hash(measurement->start_hash),
    average_number(measurement->average_number),
    settling_time(measurement->settling_time),
    end(measurement->end),
    step(measurement->step),
    control(measurement->control),
    n(measurement->n)
{
    m_stop = false;
    m_parent_thread = thread();
    isZero = false;

    QSettings settings;
    virtual_measures = settings.value(SETTINGS_VIRTUAL_MEASURES,false).toBool();
    settings.setValue(SETTINGS_VIRTUAL_MEASURES,virtual_measures);
    if (!virtual_measures){
//        variables.append(new Force(measurement->matrix,measurement->dvm_time,zero.force));
//        variables.append(new Alpha);
//        variables.append(new Beta);
//        variables.append(new Temperature);
//        variables.append(new Motor);
//        variables.append(new Wind);
    } else {
        variables.append(new Virtual_ForceHardware);
        variables.append(new Virtual_AlphaHardware);
        variables.append(new Virtual_BetaHardware);
        variables.append(new Virtual_TemperatureHardware);
        variables.append(new Virtual_MotorHardware);
        variables.append(new Virtual_WindHardware);
    }

    // set zero to each variable
    foreach (VariableHardware *hard_var, variables) {
        foreach (VariableModel *var, measurement->variables) {
            if(var->meta->has_zero()){
                if (var->meta->get_general_name() == hard_var->meta->get_general_name()){
                    hard_var->set_zero(var->get_zero());
                }
            }
        }
    }

    foreach (VariableModel *model, measurement->variables) {
        for (int k=0; k< model->meta->get_num(); k++){
            if (control == model->meta->get_name(k) && start_hash.contains(model->meta->get_name(k))){
                current = start_hash[model->meta->get_name(k)];
                break;
            }
        }
    }

}

MeasureThread::MeasureThread(ZeroModel *measurement,QObject *parent) :
    QObject(parent),
    start_hash(measurement->start_hash),
    average_number(measurement->average_number)
{
    m_stop = false;
    m_parent_thread = thread();
    isZero = true;
    n=1;
    QSettings settings;
    virtual_measures = settings.value(SETTINGS_VIRTUAL_MEASURES,false).toBool();
    settings.setValue(SETTINGS_VIRTUAL_MEASURES,virtual_measures);

    if (!virtual_measures){
//        variables.append(new Force(measurement->matrix,measurement->dvm_time));
//        variables.append(new Alpha);
//        variables.append(new Beta);
//        variables.append(new Temperature);
//        variables.append(new Motor);
//        variables.append(new Wind);

    } else {
        variables.append(new Virtual_ForceHardware);
        variables.append(new Virtual_AlphaHardware);
        variables.append(new Virtual_BetaHardware);
        variables.append(new Virtual_TemperatureHardware);
        variables.append(new Virtual_MotorHardware);
        variables.append(new Virtual_WindHardware);
    }

    settling_time = 0;
    end           = 0;
    step          = 0;
    current       = 0;

    control = "";
}

MeasureThread::~MeasureThread(){
    qDeleteAll(variables);
}

void MeasureThread::isReady(void){
    VariableHardware *variable;
    foreach (variable, variables) {
        if (variable->isReady() == false){
            throw std::runtime_error("Something is not ready.");
        }
    }
}

void MeasureThread::produce(){
    int k = 1;
    timer.start();
    QEventLoop eloop;
    set_initial();
    while(!m_stop) {
        set_m();
        Helper::msleep(settling_time*1000);
        read_m();
        if(control == ""){
            if (n != 0 && k>= n ){
                m_stop = true;
            }
        } else {
            current = current + step;
            if (( step > 0 && current > end) || (step < 0 && current < end)){
                m_stop = true;
            }
        }

        k++;
        emit MeasureDone(m_hash);
        eloop.processEvents(QEventLoop::AllEvents, 50);
    }

    // set variables to final safe values

    foreach (VariableHardware *hardware, variables) {
        VariableMeta *var = hardware->meta;
        if (var->is_controlable() && hardware->has_set_final()){
            hardware->set_final();
        }
    }

    if (m_parent_thread != thread()){
        thread()->quit();
        moveToThread(m_parent_thread);
    }
}

void MeasureThread::set_initial(){

    foreach (VariableHardware *var, variables) {
        if (var->meta->is_controlable()){
            for (int k = 0 ; k< var->meta->get_num(); k++){
                if(start_hash.contains(var->meta->get_name(k))){
                    var->set_value(k,start_hash[var->meta->get_name(k)]);
                }
            }
        }
    }
}

void MeasureThread::set_m(void){
    VariableHardware *var;
    foreach (var, variables) {
        if (var->meta->is_controlable()){
            for (int k = 0 ; k< var->meta->get_num(); k++){
                if (control == var->meta->get_name(k)){
                    var->set_value(k,current);
                    return ;
                }
            }
        }
    }
}

void MeasureThread::read_m(void){

    double tempo = timer.elapsed()/1000.0;

    QHash<QString,double> value_h;


    // clear values
    foreach (VariableHardware *var, variables) {
        for (int k=0 ; k< var->meta->get_num(); k++){
            value_h[var->meta->get_name(k)] = 0;
        }
    }

    for (int n=0;n < average_number; n++){
        foreach (VariableHardware *var, variables) {
            var->read();

            // get the result and add it
            for (int k=0 ; k< var->meta->get_num(); k++){
                value_h[var->meta->get_name(k)] += var->get_value(k);
            }
        }
    }

    // Divide by N
    QHashIterator<QString, double> i(value_h);
     while (i.hasNext()) {
         i.next();
         value_h[i.key()] = i.value() / average_number;
     }

    m_hash = value_h;
    m_hash["Time"] = tempo;
}


void MeasureThread::stop(){
    m_stop = true;
}

void MeasureThread::manual_control(QHash<QString, double> hash){

    foreach (VariableHardware *var, variables) {
        if (var->meta->is_controlable()){
            for (int k = 0 ; k< var->meta->get_num(); k++){
                if(hash.contains(var->meta->get_name(k))){
                    var->set_value(k,hash[var->meta->get_name(k)]);
                }
            }
        }
    }
}
