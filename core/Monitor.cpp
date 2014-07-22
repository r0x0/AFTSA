#include "Monitor.h"

Monitor::Monitor(QObject *parent) : QThread (parent){
    delay = 500;
    stop = false;
}

void Monitor::run(){
    stop = false;
    while(true){
        this->msleep(delay);
        emit makeUpdate();
        if(stop) break;
    }
}

