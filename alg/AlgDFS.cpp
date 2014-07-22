#include "AlgDFS.h"

AlgDFS::AlgDFS(QObject *parent) : BaseAlg::BaseAlg(parent){}

void AlgDFS::Search(){
    // Inicjacja punktu początkowego.
    startPoint->distance=0;
    startPoint->distanceValue=0;
    Visit(startPoint);
}

void AlgDFS::Visit(BaseNod *u){
    bool allv = true;

    //  Odwiedź u
    u->timeVisit=ctime;
    u->state=BaseNod::VISITED;
    update(u);
    countRecursion++;

    //  Sprawdza czy wybrany wierzchołek jest stanem końcowym.
    if(u->endNode){
        status=ST_FOUND;
        return;
    }
    int *prli;
    if(randPriori){
        prli = new int[u->MAX_NODS];
        suffle(prli,u->MAX_NODS);
    }

    // Pętla przetwarzania listy sąsiedztwa.
    for(int i=0; i<u->MAX_NODS;i++){

        int ind = priorityList[i];
        if(randPriori) ind = prli[i];

        // Zakończ funkcje jeżeli znaleziono rozwiązanie.
        if(status==ST_FOUND){
            return;
            if(u->nodList[ind]!=NULL && u->nodList[ind]->state==BaseNod::NOTVISITED){
                allv=false;
                break;
            }
            continue;
        }
        current = u->nodList[ind];
        if(current==NULL || current->state!=BaseNod::NOTVISITED){
            continue;
        }
        countNodList++;

        //  Wywołaj rekurencyjnie funkcje.
        current->prev=u;
        current->distance=u->distance+1;
        current->distanceValue = u->distanceValue + getMoveCost(u, ind);
        Visit(current);

    }

    // Oznacz wierzchołke jako przetworzony.
    if(allv){
        u->state=BaseNod::VISALL;
        update(u);
        u->timeAllVisit=ctime;
    }
}
