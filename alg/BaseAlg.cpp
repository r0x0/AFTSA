#include "alg/BaseAlg.h"

const double BaseAlg::SQRT_TWO = 1.4142;//1356;

const double BaseAlg::ROUND = 0.00001;

const double BaseAlg::MAX_DOUBLE = 0xFFFFFFFF;

BaseAlg::BaseAlg(QObject *parent) : QThread(parent){
    open = NULL;
    priorityList=NULL;
    endlist=NULL;
    listHeap=NULL;
    rtime=0;
    ctime=0;
    ttime=0;
    countPatchNod=0;
    maxOpenNod=0;
    countVisited=0;
    patchCost=0;
    countSearchOpen=0;
    countRecursion=0;
    countNodList=0;
    status=ST_NOT_STARTED;
    heuristic=HEURISTIC_MANHATTAN;
    stop=false;
    pause=false;
    step=false;
    noDataCollect=false;
    timer=new QElapsedTimer();
    message="Przygotowany";
}


void BaseAlg::run(){
    clean();
    message="Start";
    if(startPoint==NULL){
        message="Niepoprawne dane";
        emit endTask();
        return;
    }

    QElapsedTimer rtimer;
    rtimer.start();
    timer->start();
    ctime++;
    std::cout << "TIMER START: " << timer->elapsed() << std::endl;

    //Uruchami funkcje przeszukującą przestrzeń stanów.
    Search();
    if(status==ST_NOT_STARTED)status=ST_NOT_FOUND;
    rtime=rtimer.elapsed();
    //TODO

    if(current!=NULL && status==ST_FOUND){
        BaseNod *cc = current;
        countPatchNod++;
        patchCost=cc->distanceValue;
        while(cc->prev!=NULL){
            countPatchNod++;
            cc=cc->prev;
        }
        emit reproducPatch(current);
    }
    endlist->clear();
    delete endlist;
    endlist = new std::list<BaseNod*>;

    emit endTask();
}

void BaseAlg::clean(){
    message="Sprzątam";
    rtime=0;
    ctime=0;
    ttime=0;
    countPatchNod=0;
    maxOpenNod=0;
    countVisited=0;
    patchCost=0;
    countSearchOpen=0;
    countRecursion=0;
    countNodList=0;
    status=ST_NOT_STARTED;
    current=NULL;
    delete open;
    open = new std::list<BaseNod*>;
    delete listHeap;
    listHeap = new std::vector<BaseNod*>;
    stop=false;
    pause=false;
    step=false;
    bmap->initialiseMap();
}
