#ifndef MONITOR_H
#define MONITOR_H

#include <include.h>
#include <QThread>
#include <QLabel>


/**
 * @brief The Monitor class
 *Klasa zegaru uruchamiana wraz z procesem algorytmu, wysyła sygnał do odczytania danych, co określo interwał czasu.
 */
class Monitor : public QThread {

    Q_OBJECT
public:
    explicit Monitor(QObject *parent = 0);

private:
    bool stop;
    long delay;


signals:
    void makeUpdate();
    
public slots:

public:
    void setDelay(long delay) { this->delay=delay; }
    void setStop(){ stop=true; }


private:

    void run();

    
};

#endif // MONITOR_H
