#include "AlgDFSP.h"


AlgDFSP::AlgDFSP(QObject *parent) : BaseAlg::BaseAlg(parent){

}

void AlgDFSP::Search(){
    alv = true;

    // Inicjalizacja punktu początkowego.
    startPoint->distance=0;
    startPoint->distanceValue=0;
    Visit(startPoint);

    // Pętle pogłębiania (jeżeli pogłębianie ustawiono na 0 zostanie przerwana).
    while(status!=ST_FOUND && !alv){
        alv=true;
        if(depthStep<=0) return;
        maxDepth+=depthStep;
        int i = open->size();

        // Pętla przetwarzania kolejki open
        while(status!=ST_FOUND && !open->empty()){
            i--;
            if(i<0) break;
            if(depthStep<=0) return;
            BaseNod *u = open->front();
            open->pop_front();
            Visit(u);
        }
    }
}

void AlgDFSP::Visit(BaseNod *u){

    if(u->state!=BaseNod::VISITED){
        //  Odwiedź u
        u->timeVisit=ctime;
        u->state=BaseNod::VISITED;
        update(u);
    }
    countRecursion++;

    // Jeżeli głębokość u jest za duża dodaje do listy i wychodzi z funkcji.
    if(u->distance==maxDepth){
            open->push_back(u);
        return;
    }
    bool allv = true;

    //  Sprawdza czy wybrany wierzchołek jest stanem końcowym.
    if(u->endNode){
        status=ST_FOUND;
        current=u;
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
        alv=false;
        countNodList++;

        //  Wywołaj rekurencyjnie funkcje.
        current->prev=u;
        current->distance=u->distance+1;
        current->distanceValue = u->distanceValue + getMoveCost(u, ind);

        Visit(current);

    }
    if(u->distance==maxDepth){
        return;
    }

    // Oznacz wierzchołke jako przetworzony.
    if(allv){
        u->state=BaseNod::VISALL;
        update(u);
        u->timeAllVisit=ctime;
    }
}
