#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    msbox = new QMessageBox(this);
    ui->setupUi(this);
    this->statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(this->statusLabel);

    // Inicjalizacja
    changeStatus("Przygotowanie");
    start=false;
    pause=false;
    painteve=false;
    simulation=false;
    currRow=-1;
    spPrev= new map<QSpinBox, int>;
    this->bMap = new BaseMap();
    this->fileStr = new FilesStructure();
    //this->cnf = new CNF();
    this->painter = new Painter(ui->scrollAreaWidgetContents);
    this->baseAlg = NULL;
    this->monitor = new Monitor(this);

    ui->gridLayout_3->addWidget(painter, 0, 0, 1, 1);
    ui->algComboBox->addItem("BFS");
    ui->algComboBox->addItem("DFS");
    ui->algComboBox->addItem("DFS z pogłębianiem");
    ui->algComboBox->addItem("Dijkstry");
    ui->algComboBox->addItem("Best first search");
    ui->algComboBox->addItem("A*");
    ui->algComboBox->setCurrentIndex(0);

    QPalette pagePalette = ui->page_2->palette();
    pagePalette.setColor(backgroundRole(), QColor( 0, 0, 0 ) );
    ui->page_2->setPalette( pagePalette );
    ui->page_2->setAutoFillBackground( true );

    prioEast = new QStandardItem("E");
    prioWeast = new QStandardItem("W");
    prioNorth = new QStandardItem("N");
    prioSouth = new QStandardItem("S");
    prioNW = new QStandardItem("NW");
    prioNE = new QStandardItem("NE");
    prioSW = new QStandardItem("SW");
    prioSE = new QStandardItem("SE");

    prioMap = new map<QStandardItem*, int>;
    pListModel = new QStandardItemModel();

    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Background, QBrush(QColor(0,0,0,180)));
    palette.setBrush(QPalette::Active, QPalette::Foreground, QBrush(QColor(0,0,0,180)));
    palette.setBrush(QPalette::Active, QPalette::Window, QBrush(QColor(0,0,0,180)));
    palette.setBrush(QPalette::Active, QPalette::WindowText, QBrush(QColor(250,220,20,180)));
    palette.setBrush(QPalette::Active, QPalette::Text, QBrush(QColor(250,220,20,180)));

    painter->setScale(ui->scaleSlider->value());
    reloadPList(true);

    connect(painter, SIGNAL(posChanged(int, int)), this, SLOT(on_painter_click_start(int,int)));
    connect(painter, SIGNAL(endPosChanged(int, int)), this, SLOT(on_painter_click_end(int,int)));
    connect(ui->scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), painter, SLOT(pUp()));
    connect(ui->scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), painter, SLOT(pUp()));
    connect(painter, SIGNAL(changeXY(int,int,int,int)), this, SLOT(chanegeXY(int,int,int,int)));
    connect(monitor, SIGNAL(makeUpdate()), this, SLOT(readAlgData()));

    signalMapperEndSVC = new QSignalMapper(this);

    disableMapConf(false);
    ui->mapComboBox->setEnabled(true);
    ui->loadMapBut->setEnabled(true);
    ui->radioManhattam->setChecked(true);

    changeStatus("Sprawdzanie struktury plików");
    fileStr->checkStructure();

    //changeStatus("Ładowanie ustawień");
    //cnf->loadConfiguration();

    changeStatus("Gotowy");
}


MainWindow::~MainWindow(){
    //delete cnf;
    delete prioMap;
    delete prioEast;
    delete prioWeast;
    delete prioNorth;
    delete prioSouth;
    delete prioNW;
    delete prioNE;
    delete prioSW;
    delete prioSE;
    prioEast = NULL;
    prioWeast = NULL;
    prioNorth = NULL;
    prioSouth = NULL;
    prioNW = NULL;
    prioNE = NULL;
    prioSW = NULL;
    prioSE = NULL;
    delete fileStr;
    delete pListModel;
    delete spPrev;
    delete bMap;
    delete baseAlg;
    delete painter;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(cleanUp()) event->accept();
    else event->ignore();
}

void MainWindow::on_actionAppClose_triggered(){
    this->close();
}


void MainWindow::on_actionNowa_triggered(){

}


void MainWindow::on_delaySlider_valueChanged(int value){
    ui->delaySpin->setValue(value);
    if(baseAlg!=NULL) baseAlg->setDelay(value);
    if(value<=5){
        ui->mapUpdate->setChecked(false);
        ui->checkPatcTree->setChecked(false);
        painter->setNoSearchTree(false);
        painter->setNoUpdate(false);
    }
}



void MainWindow::on_scaleSlider_valueChanged(int value){
    ui->scaleSpin->setValue(value);
}


void MainWindow::on_scaleSpin_valueChanged(double arg1){
    ui->scaleSlider->setValue(arg1);
}



void MainWindow::on_scaleSlider_sliderReleased(){
    this->painter->setScale(ui->scaleSlider->value());
}

void MainWindow::on_scaleSpin_editingFinished(){
    this->painter->setScale(ui->scaleSlider->value());
}


void MainWindow::on_actionZ_pliku_triggered(){
    changeStatus("Wybieranie pliku");

    string fileMap = QFileDialog::getOpenFileName(this,tr("Wybierz plik"),"maps/",tr("Obraz (*.bmp *.BMP)")).toStdString();

    if(fileMap.length()==0){
        changeStatus("Anulowano",10000);
        return;
    }

    Bitmap *bmp = new Bitmap();
    changeStatus("Wczytywanie pliku BMP: " + fileMap);
    bmp->readFile(fileMap);
    unsigned char **mapv = bmp->getMap();

    if(mapv==NULL){
        showMess("Anulowano", "Nie udało się wczytać pliku\n"+fileMap, bmp->getMessage());
        changeStatus("Anulowano",10000);
        msbox->setText(QString::fromStdString("Nie udało się wczytać pliku\n"+fileMap));
        msbox->setInformativeText(QString::fromStdString(bmp->getMessage()));
        msbox->setIcon(QMessageBox::Warning);
        msbox->show();
//        return;
    }

    if(bMap!=NULL){
        delete bMap;
    }

    bMap = new BaseMap();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackTitle->setText("Widok ustawień");

    if(ui->radioOperators8->isChecked()) bMap->generateMapModel8(mapv, bmp->getX(), bmp->getY());
    else bMap->generateMapModel4(mapv, bmp->getX(), bmp->getY());
    this->painter->generateMap(mapv, bmp->getX(), bmp->getY());
    ui->sXSpin->setValue(0);
    ui->sYSpin->setValue(0);
    ui->sXSpin->setMaximum(bmp->getX());
    ui->sYSpin->setMaximum(bmp->getY());
    clearRows();
    disableMapConf(true);
//    ui->tableWidget->seth
//    ui->checkStart->setEnabled(true);
//    ui->checkEnd->setEnabled(true);
//    ui->startButton->setEnabled(true);
//    painter->setemitXY(true);
    delete bmp;
    changeStatus("Gotowy");
}


void MainWindow::on_sXSpin_valueChanged(int arg1){
    if(arg1==0){
        ui->sYSpin->setValue(arg1);
        return;
    }
    checkCords(arg1, ui->sYSpin->value(), true);
}


void MainWindow::on_sYSpin_valueChanged(int arg1){
    if(arg1==0){
        ui->sXSpin->setValue(arg1);
        return;
    }
    checkCords(ui->sXSpin->value(), arg1, false);
}


void MainWindow::on_checkStart_stateChanged(int arg1)
{

    ui->sXSpin->setEnabled(arg1);
    ui->sYSpin->setEnabled(arg1);
    if(arg1){
        ui->checkEnd->setChecked(false);
        painter->setStartEve(true);
        painter->setEndEve(false);
        ui->tableWidget->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        painter->clearSearch();
        painter->update();
    }else{
        painter->setStartEve(false);
        painter->fixMap();
    }
}


void MainWindow::on_checkEnd_stateChanged(int arg1)
{
    ui->tableWidget->setEnabled(arg1);
    ui->pushButton_5->setEnabled(arg1);
    ui->pushButton_6->setEnabled(arg1);
    if(arg1){
        ui->checkStart->setChecked(false);
        painter->setStartEve(false);
        painter->setEndEve(true);
        ui->sXSpin->setEnabled(false);
        ui->sYSpin->setEnabled(false);
        painter->clearSearch();
        painter->update();
    }else{
        painter->setEndEve(false);
        painter->fixMap();
    }
}


void MainWindow::on_startButton_clicked(){
    if(baseAlg==NULL){
        showMess("Anulowano", "Brak danych", "Wybierz algorytm");
        return ;
    }
    int endCount = ui->tableWidget->rowCount();
    int startX = ui->sXSpin->value();
    int startY = ui->sYSpin->value();
    if(startX==0 || startY==0){
        showMess("Anulowano", "Brak danych", "Wybierz punkt początkowy");
        return ;
    }
    if(!start && !pause){
        start=true;
        simulation=true;
        ui->startButton->setText("Pauza");
        painter->clearAndUpdate();
        disableMapConf(false);
        disableAlgConf(false);
        baseAlg->setStart(bMap->getNodAt(startX-1,startY-1));
        baseAlg->setNoCollectData(ui->dataUpdate->isChecked());
        baseAlg->setDelay(ui->delaySlider->value());
        baseAlg->setRandPrioi(ui->checkRandPrio->isChecked());
        baseAlg->setMaxDepth(ui->depthSpin->value());
        baseAlg->setDepthStep(ui->depthStepSpin->value());
        baseAlg->setHeuristicScale(ui->doubleSpinHeur->value());

        if(ui->radioIntSub->isChecked()) baseAlg->setInterpretation(BaseAlg::INTERPRET_DIFF);
        else baseAlg->setInterpretation(BaseAlg::INTERPRET_VALUE);

        if(ui->radiHeap->isChecked()) baseAlg->setQueueType(BaseAlg::QUEUE_HEAP);
        else baseAlg->setQueueType(BaseAlg::QUEUE_LIST);

        std::list<BaseNod*> *endlist = new std::list<BaseNod*>;
        for(int i=0;i<endCount;i++){
            int endX = static_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,0))->value();
            int endY = static_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,1))->value();
            bMap->setEnd(endX-1,endY-1,true);
            endlist->push_back(bMap->getNodAt(endX-1, endY-1));
        }

        int * pl = new int[prioMap->size()];
        for(int i=0; i<ui->priorityListView->model()->rowCount(); i++){
            pl[i]=(*prioMap)[static_cast<QStandardItemModel*>(ui->priorityListView->model())->item(i)];
        }
        baseAlg->setPriorityList(pl);
        baseAlg->setMap(bMap);
        baseAlg->setEndList(endlist);
        if(ui->radioEuklides->isChecked()) baseAlg->setHeuristic(BaseAlg::HEURISTIC_EUKLIDES);
        else baseAlg->setHeuristic(BaseAlg::HEURISTIC_MANHATTAN);
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackTitle->setText("Widok danych");
        monitor->start();
        baseAlg->start();
    }else if(start && !pause){
        baseAlg->setPause(true);
        pause=true;
        ui->startButton->setText("Wznów");
        ui->stepButton->setEnabled(true);
    }else if(start && pause){
        baseAlg->setPause(false);
        pause=false;
        ui->startButton->setText("Pauza");
        ui->stepButton->setEnabled(false);
    }
}


void MainWindow::on_stepButton_clicked(){
    if(baseAlg==NULL) return;
    if(start && pause){
            baseAlg->setStep(true);
    }
}


void MainWindow::on_stopButton_clicked(){
    if(baseAlg!=NULL){
        baseAlg->setStop(true);
        if(pause) baseAlg->setStep(true);
    }
}


void MainWindow::on_pushButton_5_clicked(){
    insertRow();
}


void MainWindow::on_pushButton_6_clicked(){
    if(currRow==-1){
        //FIXME
        cout << "selectedItems().size(): " << ui->tableWidget->selectedItems().size() << endl;
        cout << "selectionModel()->currentIndex().row(): " << ui->tableWidget->selectionModel()->currentIndex().row() << endl;
        cout << "currentIndex().row(): " << ui->tableWidget->currentIndex().row() << endl;
        cout << "currentRow(): " << ui->tableWidget->currentRow() << endl;
        cout << "selectionModel()->hasSelection(): " << ui->tableWidget->selectionModel()->hasSelection() << endl;

        changeStatus("Anulowano",10000);
        return;
    }
    QSpinBox *sp = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(ui->tableWidget->currentRow(),0));
    QSpinBox *sp2 = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(ui->tableWidget->currentRow(),1));
    painter->changePos(sp->value(), sp2->value(), true);
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


void MainWindow::on_algComboBox_currentIndexChanged(int index){
    deleteBaseAlg();
    if(index==0){
        baseAlg=new AlgBFS(this);
        painter->setReverseNod(false);
    }else if(index==1){
        baseAlg=new AlgDFS(this);
        painter->setReverseNod(false);
    }else if(index==2){
        baseAlg=new AlgDFSP(this);
        painter->setReverseNod(false);
    }else if(index==3){
        baseAlg=new AlgDijkstra(this);
        painter->setReverseNod(true);
    }else if(index==4){
        baseAlg=new AlgBestFirst(this);
        painter->setReverseNod(true);
    }else if(index==5){
        baseAlg=new AlgAStar(this);
        painter->setReverseNod(true);
    }
    if(baseAlg!=NULL){
        connectBaseAlg();
        painter->clearSearch();
        painter->update();
    }
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackTitle->setText("Widok ustawień");
    disableAlgConf(true);
}

void MainWindow::enableRandPrio(bool enable){
    ui->checkRandPrio->setChecked(false);
    ui->checkRandPrio->setEnabled(enable);
    ui->prioriDownBut->setEnabled(enable);
    ui->prioriUpBut->setEnabled(enable);
    ui->priorityListView->setEnabled(enable);
    ui->radiHeap->setEnabled(!enable);
    ui->radioList->setEnabled(!enable);
}

void MainWindow::enableDFSConf(bool enable){
    ui->depthSpin->setEnabled(enable);
    ui->depthStepSpin->setEnabled(enable);
}

void MainWindow::enableBestFirstConf(bool enable){
    ui->radioEuklides->setEnabled(enable);
    ui->radioManhattam->setEnabled(enable);
}

void MainWindow::enableAConf(bool enable){
    ui->doubleSpinHeur->setEnabled(enable);
    ui->radioEuklides->setEnabled(enable);
    ui->radioManhattam->setEnabled(enable);
}

void MainWindow::deleteBaseAlg(){
    if(baseAlg!=NULL){
        disconnect(baseAlg, SIGNAL(repaintField(BaseNod*,uchar)), painter, SLOT(repaintField(BaseNod*,uchar)));
        disconnect(baseAlg, SIGNAL(endTask()), this, SLOT(endAlg()));
        disconnect(baseAlg, SIGNAL(reproducPatch(BaseNod*)), painter, SLOT(reproducPatch(BaseNod*)));
        disconnect(baseAlg, SIGNAL(drawTreeLine(BaseNod*)), painter, SLOT(drawTreeLine(BaseNod*)));
    }
    delete baseAlg;
    baseAlg=NULL;
}

void MainWindow::connectBaseAlg(){
    connect(baseAlg, SIGNAL(repaintField(BaseNod*,uchar)), painter, SLOT(repaintField(BaseNod*,uchar)), Qt::ConnectionType::BlockingQueuedConnection);
    connect(baseAlg, SIGNAL(endTask()), this, SLOT(endAlg()));
    connect(baseAlg, SIGNAL(reproducPatch(BaseNod*)), painter, SLOT(reproducPatch(BaseNod*)), Qt::ConnectionType::BlockingQueuedConnection);
    connect(baseAlg, SIGNAL(drawTreeLine(BaseNod*)), painter, SLOT(drawTreeLine(BaseNod*)));
}


void MainWindow::on_painter_click_start(int newX, int newY){
    painteve=true;
    ui->sXSpin->setValue(newX);
    ui->sYSpin->setValue(newY);
//    PointEvent *start=new PointEvent("Start", false);
//    bMap->setEnd(newX,newY, true);

    painteve=false;
}


void MainWindow::on_painter_click_end(int newX, int newY){
    if(!searchAndDelRow(newX, newY)){
        painteve=true;
        insertRow();
        QSpinBox *sp = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,0));
        sp->setValue(newX);
        QSpinBox *sp2 = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,1));
        sp2->setValue(newY);
//        PointEvent *end=new PointEvent("End", true);
//        bMap->setEvent(newX,newY, true);
        painteve=false;
    }
}


void MainWindow::spinBoxEnd_valueChanged(int arg1){
    checkCords(arg1, -1, true);
}


void MainWindow::spinBoxEnd_valueChanged2(int arg1){
    checkCords(-1, arg1, false);
}


void MainWindow::on_checkRandPrio_stateChanged(int arg1){
    if(arg1){
        ui->priorityListView->setEnabled(false);
        ui->prioriUpBut->setEnabled(false);
        ui->prioriDownBut->setEnabled(false);
        baseAlg->setRandPrioi(true);
    }else{
        ui->priorityListView->setEnabled(true);
        ui->prioriUpBut->setEnabled(true);
        ui->prioriDownBut->setEnabled(true);
        baseAlg->setRandPrioi(false);
    }
}


void MainWindow::on_prioriUpBut_clicked(){
    int index = ui->priorityListView->currentIndex().row();
    if(index>0){
        QStandardItemModel *qm = static_cast<QStandardItemModel*>(ui->priorityListView->model());
        QStandardItem *qsu = qm->takeItem(index,0);
        QStandardItem *qso = qm->takeItem(index-1,0);
        qm->setItem(index-1,qsu );
        qm->setItem(index,qso);
//        qm->index(index-1,0);
        ui->priorityListView->setCurrentIndex(qm->index(index-1,0));
    }
}


void MainWindow::on_prioriDownBut_clicked(){
    int index = ui->priorityListView->currentIndex().row();
    if(index<ui->priorityListView->model()->rowCount()-1 && index>-1){
        QStandardItemModel *qm = static_cast<QStandardItemModel*>(ui->priorityListView->model());
        QStandardItem *qsu = qm->takeItem(index,0);
        QStandardItem *qso = qm->takeItem(index+1,0);
        qm->setItem(index+1,qsu );
        qm->setItem(index,qso);
//        qm->index(index+1,0);
        ui->priorityListView->setCurrentIndex(qm->index(index+1,0));
    }
}


bool MainWindow::searchAndDelRow(int x, int y){
    for(int i=0; i<ui->tableWidget->rowCount();i++){
        if((static_cast<QSpinBox *>(ui->tableWidget->cellWidget(i,0)))->value()==x &&
            (static_cast<QSpinBox *>(ui->tableWidget->cellWidget(i,1)))->value()==y){
            ui->tableWidget->removeRow(i);
            return true;
        }
    }
    return false;
}


void MainWindow::disableMapConf(bool b){
    ui->checkEnd->setChecked(false);
    ui->checkStart->setChecked(false);
    ui->checkEnd->setEnabled(b);
    ui->checkStart->setEnabled(b);
    ui->mapComboBox->setEnabled(b);
    ui->stepButton->setEnabled(false);
    ui->stopButton->setEnabled(!b);
    ui->loadMapBut->setEnabled(b);
    ui->scaleSlider->setEnabled(b);
    ui->scaleSpin->setEnabled(b);
    ui->loadMapConfBut->setEnabled(b);
    ui->algComboBox->setEnabled(b);
    painter->setemitXY(b);
//    ui->tableWidget->setEnabled(b);
//    ui->sXSpin->setEnabled(b);
//    ui->sYSpin->setEnabled(b);
//    ui->pushButton_5->setEnabled(b);
//    ui->pushButton_6->setEnabled(b);
}


void MainWindow::disableAlgConf(bool b){
    ui->radioOperators4->setEnabled(b);
    ui->radioOperators8->setEnabled(b);
    ui->radioIntSub->setEnabled(b);
    ui->radioIntValue->setEnabled(b);
    ui->dataUpdate->setEnabled(b);
    int index = ui->algComboBox->currentIndex();
    if(index==0){
        enableAConf(false);
        enableDFSConf(false);
        enableRandPrio(b);
        ui->radioList->setChecked(true);
    }else if(index==1){
        enableAConf(false);
        enableDFSConf(false);
        enableRandPrio(b);
        ui->radioList->setChecked(true);
    }else if(index==2){
        enableAConf(false);
        enableDFSConf(b);
        enableRandPrio(b);
        ui->radioList->setChecked(true);
    }else if(index==3){
        enableAConf(false);
        enableDFSConf(false);
        enableRandPrio(false);
    }else if(index==4){
        enableAConf(false);
        enableBestFirstConf(b);
        enableDFSConf(false);
        enableRandPrio(false);
    }else if(index==5){
        enableAConf(b);
        enableDFSConf(false);
        enableRandPrio(false);
    }
}


void MainWindow::checkCords(int x, int y, bool xc){
    if(painteve) return;
    if(ui->checkStart->isChecked()){
        if(!(x==0 || y==0)){
            for(int i=0; i<ui->tableWidget->rowCount(); i++){
                QSpinBox *sp = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(i,0));
                QSpinBox *sp2 = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(i,1));
                if(sp->value()==x && sp2->value()==y){
                    if(xc){
                        if(x+1<=bMap->getX()) ui->sXSpin->setValue(x+1);
                        else ui->sXSpin->setValue(x-1);
                    }else{
                        if(y+1<=bMap->getY()) ui->sYSpin->setValue(y+1);
                        else ui->sYSpin->setValue(y-1);
                    }
                    return;
                }
            }
            painter->changePos(ui->sXSpin->value(), ui->sYSpin->value());
        }else{
            painter->changePos(ui->sXSpin->value(), ui->sYSpin->value(), true);
        }
    }else if(ui->checkEnd->isChecked()){
        if(currRow!=-1){
            //FIXME
            QSpinBox *sp = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(currRow,0));
            QSpinBox *sp2 = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(currRow,1));
            if(ui->sXSpin->value()==sp->value() && ui->sYSpin->value()==sp2->value()){
                if(xc){
                    if(x+1<=bMap->getX()) sp->setValue(x+1);
                    else sp->setValue(x-1);
                }else{
                    if(y+1<=bMap->getY()) sp2->setValue(y+1);
                    else sp2->setValue(y-1);
                }
                return;
            }
        }
        list<int> *tmp = new list<int>;
        for(int i=0; i<ui->tableWidget->rowCount(); i++){
            QSpinBox *sp = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(i,0));
            QSpinBox *sp2 = static_cast<QSpinBox *>(ui->tableWidget->cellWidget(i,1));
            tmp->push_back(sp->value());
            tmp->push_back(sp2->value());
        }
        painter->changePoss(tmp);
    }

}


void MainWindow::endAlg(){
    start = false;
    pause = false;
    ui->startButton->setText("Start");
    disableMapConf(true);
    disableAlgConf(true);
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        int endX = static_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,0))->value();
        int endY = static_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,1))->value();
        bMap->setEnd(endX-1,endY-1,false);
    }
    if(!baseAlg->getStop()){
        fileStr->saveMapConf();
        updateMark();
        updateStats();
    }
    monitor->setStop();
    simulation=false;
}


void MainWindow::updateMark(){
    //TODO
}


void MainWindow::updateStats(){
    //TODO
}


void MainWindow::showMess(string status, string title, string message, int timeout, QMessageBox::Icon icon){
    changeStatus(status,timeout);
    msbox->setText(QString::fromStdString(title));
    msbox->setInformativeText(QString::fromStdString(message));
    msbox->setIcon(icon);
    msbox->setMinimumSize(200,100);
    msbox->setBaseSize(200,100);
    msbox->show();
}


void MainWindow::clearRows(){
    int i = ui->tableWidget->rowCount()-1;
    for(;i>=0; i--){
        ui->tableWidget->removeRow(i);
    }
}


void MainWindow::changeStatus(string message, int timeout){
    if(timeout==0){
        this->statusLabel->setText(QString(message.c_str()));
        this->statusLabel->update();
    }
    else{
        ui->statusBar->showMessage(QString(message.c_str()), timeout);
        this->statusLabel->setText("Gotowy");
        this->statusLabel->update();
    }
}


void MainWindow::insertRow(){
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QSpinBox *sp = new QSpinBox(ui->tableWidget);
    QSpinBox *sp2 = new QSpinBox(ui->tableWidget);
    sp->setMinimum(0);
    sp2->setMinimum(0);
    sp->setMaximum(bMap->getX());
    sp2->setMaximum(bMap->getY());
    sp->setVisible(true);
    sp2->setVisible(true);

    connect(sp, SIGNAL(valueChanged(int)), this, SLOT(spinBoxEnd_valueChanged(int)));
    connect(sp2, SIGNAL(valueChanged(int)), this, SLOT(spinBoxEnd_valueChanged2(int)));
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,sp);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1,sp2);
}


bool MainWindow::cleanUp(){
    painter->setBlock(true);
    if(monitor!=NULL){
        if(monitor->isRunning()){
            monitor->setStop();
//            monitor->terminate();
            while(monitor->isRunning()){}
        }
    }
    if(baseAlg!=NULL){
        if(baseAlg->isRunning()){
            baseAlg->setStop(true);
//            baseAlg->terminate();
            while(baseAlg->isRunning()){}
        }
    }
    return true;
}


void MainWindow::chanegeXY(int x, int y, int x2, int y2){
    if(x==-1 || y==-1){

        return;
    }else{
        stringstream str;
        str << "Pozycja: (" << x <<","<<y<<")";

    }
}


void MainWindow::on_prevStackBut_clicked(){
    int index = ui->stackedWidget->currentIndex();
    if(index==1) index = 0;
    else index++;
    ui->stackedWidget->setCurrentIndex(index);
    if(index==0) ui->stackTitle->setText("Widok ustawień");
    if(index==1) ui->stackTitle->setText("Widok danych");
}


void MainWindow::on_nextStackBut_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    if(index==1) index = 0;
    else index++;
    ui->stackedWidget->setCurrentIndex(index);
    if(index==0) ui->stackTitle->setText("Widok ustawień");
    if(index==1) ui->stackTitle->setText("Widok danych");
}

void MainWindow::on_radioOperators8_clicked(){
    reloadPList(true);
}

void MainWindow::on_radioOperators4_clicked(){
    reloadPList(false);
}

void MainWindow::reloadPList(bool all){
    for(int i =pListModel->rowCount()-1; i>=0; i--){
        pListModel->takeRow(i);
    }
    pListModel->appendRow(prioNorth);
    if(all) pListModel->appendRow(prioNW);
    pListModel->appendRow(prioWeast);
    if(all) pListModel->appendRow(prioSW);
    pListModel->appendRow(prioSouth);
    if(all) pListModel->appendRow(prioSE);
    pListModel->appendRow(prioEast);
    if(all) pListModel->appendRow(prioNE);
    prioMap->clear();
    for(int i =0; i< pListModel->rowCount();i++){
        prioMap->insert(prioMap->end(), pair<QStandardItem*, int>(pListModel->item(i),i));
    }
    ui->priorityListView->setModel(pListModel);
    if(all) bMap->generateMapModel8(bMap->getMapV(), bMap->getX(), bMap->getY());
    else bMap->generateMapModel4(bMap->getMapV(), bMap->getX(), bMap->getY());
}




void MainWindow::on_dataUpdate_clicked(){
    if(ui->dataUpdate->isChecked()){
        ui->checkPatcTree->setChecked(true);
        ui->checkPatcTree->setEnabled(true);
        ui->mapUpdate->setChecked(true);
        ui->mapUpdate->setEnabled(true);
        ui->checkSTree->setChecked(true);
        ui->checkSTree->setEnabled(true);
        ui->checkSNod->setChecked(true);
        ui->checkSNod->setEnabled(true);
    }else{
        ui->checkPatcTree->setChecked(false);
        ui->checkPatcTree->setEnabled(false);
        ui->mapUpdate->setChecked(false);
        ui->mapUpdate->setEnabled(false);
        ui->checkSTree->setChecked(false);
        ui->checkSTree->setEnabled(false);
        ui->checkSNod->setChecked(false);
        ui->checkSNod->setEnabled(false);
    }
}

void MainWindow::on_mapUpdate_clicked(){
    if(ui->mapUpdate->isChecked()){
        ui->checkPatcTree->setChecked(true);
        ui->checkPatcTree->setEnabled(true);
    }else{
        ui->checkPatcTree->setChecked(false);
        ui->checkPatcTree->setEnabled(false);
    }
    painter->setNoUpdate(ui->mapUpdate->isChecked());
    painter->setNoSearchTree(ui->checkPatcTree->isChecked());
}


void MainWindow::readAlgData(){

    ui->lcountPatchNod->setText(QString::fromStdString(getstring(baseAlg->getCountPatchNod())));
    ui->lmaxOpen->setText(QString::fromStdString(getstring(baseAlg->getMaxOpenNod())));
    ui->lCountVisited->setText(QString::fromStdString(getstring(baseAlg->getCountVisited())));
    ui->lPatchCost->setText(QString::fromStdString(getstring(baseAlg->getPatchCost())));
    ui->lCountSearchOpen->setText(QString::fromStdString(getstring(baseAlg->getCountSearchOpen())));
    ui->lRtime->setText(QString::fromStdString(getstring(baseAlg->getRtime(), " ms")));
    ui->lTtime->setText(QString::fromStdString(getstring(baseAlg->getTtime(), " µs")));
    ui->lCtime->setText(QString::fromStdString(getstring(baseAlg->getCtime())));
    ui->lCountRecu->setText(QString::fromStdString(getstring(baseAlg->getCountRecursion())));
    ui->lCountNod->setText(QString::fromStdString(getstring(baseAlg->getCountNodList())));
}

inline std::string MainWindow::getstring(double b, std::string ss){
    stringstream s;
    s << std::fixed << b << ss;
    return s.str();
}

inline std::string MainWindow::getstring(long b, std::string ss){
    stringstream s;
    s << b << ss;
    return s.str();
}

inline std::string MainWindow::getstring(unsigned long b, std::string ss){
    stringstream s;
    s << b << ss;
    return s.str();
}

void MainWindow::on_checkPatcTree_clicked(){
    painter->setNoSearchTree(ui->checkPatcTree->isChecked());
}

void MainWindow::on_checkSTree_clicked(){
    painter->setDrawSTree(ui->checkSTree->isChecked());
    painter->update();
}

void MainWindow::on_checkSNod_clicked(){
    painter->setDrawSNod(ui->checkSNod->isChecked());
    painter->update();
}

void MainWindow::on_delaySpin_editingFinished(){
    ui->delaySlider->setValue(ui->delaySpin->value());
}

