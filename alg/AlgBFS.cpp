#include "alg/AlgBFS.h"


AlgBFS::AlgBFS(QObject *parent) : BaseAlg::BaseAlg(parent){}


void AlgBFS::Search(){

    //  Inicjalizacja i odwiedzenie stanu początkowego.
    startPoint->distance=0;
    startPoint->distanceValue=0;
    startPoint->prev=NULL;
    startPoint->state=BaseNod::VISITED;
    startPoint->timeVisit=ctime;
    open->push_back(startPoint);
    update(startPoint);

    //  Sprawdza czy stan początkowy jest stanem końcowym.
    if(startPoint->endNode){
        status=ST_FOUND;
        current= startPoint;
        return;
    }

    //  Główna pętla programu.
    while(!open->empty()){

        //  Wybiera wierzchołek z listy open.
        prev=open->front();
        open->pop_front();
        int *prli;
        if(randPriori){
            prli = new int[prev->MAX_NODS];
            suffle(prli,prev->MAX_NODS);
        }

        // Pętla przetwarzania listy sąsiedztwa.
        for(int i=0; i<prev->MAX_NODS;i++){

            int index = priorityList[i];
            if(randPriori)index = prli[i];
            current = prev->nodList[index];
            if(current==NULL || current->state!=BaseNod::NOTVISITED) continue;
            countNodList++;
            // Odwiedza wierzchołek.
            current->state=BaseNod::VISITED;
            current->timeVisit=ctime;
            current->distance=prev->distance+1;
            current->distanceValue=prev->distanceValue + getMoveCost(prev, index);
            current->prev=prev;
            open->push_back(current);
            update(current);

            //  Sprawdza czy wybrany wierzchołek jest stanem końcowym.
            if(current->endNode){
                status=ST_FOUND;
                return;
            }
        }

        //  Oznacza wierzchołek jako przetworzony.
        prev->state=BaseNod::VISALL;
        prev->timeAllVisit=ctime;
        update(prev);
    }
}
