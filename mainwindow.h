#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QPen>
#include <QStandardItemModel>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QScrollBar>
#include <QToolTip>
#include "stdinc.h"
#include "include.h"


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 *Główna klasa aplikacji, wprowadza i obsługuje wszystkie komponenty GUI.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:

    void on_painter_click_start(int newX, int newY);

    void on_painter_click_end(int newX, int newY);

    void on_actionAppClose_triggered();

    void on_actionNowa_triggered();

    void endAlg();

    void on_scaleSpin_valueChanged(double arg1);

    void on_scaleSlider_valueChanged(int value);

    void on_delaySlider_valueChanged(int value);

    void on_scaleSlider_sliderReleased();

    void on_sXSpin_valueChanged(int arg1);

    void on_sYSpin_valueChanged(int arg1);

    void on_checkStart_stateChanged(int arg1);

    void on_checkEnd_stateChanged(int arg1);

    void on_startButton_clicked();

    void on_stepButton_clicked();

    void on_stopButton_clicked();

    void on_pushButton_5_clicked();

    void on_algComboBox_currentIndexChanged(int index);

    void spinBoxEnd_valueChanged(int arg1);

    void spinBoxEnd_valueChanged2(int arg1);

    void on_actionZ_pliku_triggered();

    void on_pushButton_6_clicked();

    void on_checkRandPrio_stateChanged(int arg1);

    void on_prioriUpBut_clicked();

    void on_prioriDownBut_clicked();

    void chanegeXY(int x, int y, int x2, int y2);

    void on_prevStackBut_clicked();

    void on_nextStackBut_clicked();

    void on_scaleSpin_editingFinished();

    void on_radioOperators8_clicked();

    void on_radioOperators4_clicked();

    void on_dataUpdate_clicked();

    void on_mapUpdate_clicked();

    void readAlgData();

    void on_checkPatcTree_clicked();

    void on_checkSTree_clicked();

    void on_checkSNod_clicked();

    void on_delaySpin_editingFinished();


private:
    void changeStatus(string message, int timeout=0);
    void clearRows();
    void insertRow();
    bool searchAndDelRow(int x, int y);
    void disableMapConf(bool b);
    void disableAlgConf(bool b);
    void checkCords(int x, int y, bool xc);
    void updateMark();
    void updateStats();
    void showMess(string status, string title, string message, int timeout=10000, QMessageBox::Icon icon=QMessageBox::Warning);
    void deleteBaseAlg();
    void connectBaseAlg();
    void closeEvent(QCloseEvent *event);
    bool cleanUp();
    void reloadPList(bool all);
    void enableRandPrio(bool enable);
    void enableDFSConf(bool enable);
    void enableBestFirstConf(bool enable);
    void enableAConf(bool enable);
    inline std::string getstring(double b, string ss="");
    inline std::string getstring(long b, string ss="");
    inline std::string getstring(unsigned long b, string ss="");

private:

    Ui::MainWindow *ui;
    FilesStructure *fileStr;
    //CNF *cnf;
    BaseMap *bMap;
    QMessageBox *msbox;
    QLabel *statusLabel;
    Painter *painter;
    BaseAlg *baseAlg;
    bool start;
    bool pause;
    bool painteve;
    bool simulation;
    int currRow;
    map<QSpinBox, int> *spPrev;
    QSignalMapper *signalMapperEndSVC;
    Monitor *monitor;
    QStandardItem* prioNorth;
    QStandardItem* prioWeast;
    QStandardItem* prioEast;
    QStandardItem* prioSouth;
    QStandardItem* prioNW;
    QStandardItem* prioNE;
    QStandardItem* prioSW;
    QStandardItem* prioSE;
    QStandardItemModel* pListModel;
    std::map<QStandardItem*, int> *prioMap;
    QLabel *toolPos;
};

#endif // MAINWINDOW_H
