#include "AlgDijkstra.h"

AlgDijkstra::AlgDijkstra(QObject *parent) : BaseAlg::BaseAlg(parent){
}

void AlgDijkstra::Search(){

    //  Inicjalizacja stanu początkowego.
    startPoint->distance=0;
    startPoint->distanceValue=0;
    startPoint->heuristicValue=0;
    startPoint->prev=NULL;
    inserNodToList(startPoint);

    //  Główna pętla programu.
    while(checkIsOpenEmpty()){

        //  Wybiera i odwiedza wierzchołek z listy open.
        prev=getMinFromOpen();
        prev->state=BaseNod::VISITED;
        prev->timeVisit=ctime;
        update(prev);

        //  Sprawdza czy wybrany wierzchołek jest stanem końcowym.
        if(prev->endNode){
            status=ST_FOUND;
            current=prev;
            return;
        }

        // Pętla przetwarzania listy sąsiedztwa.
        for(int i = 0; i<prev->MAX_NODS; i++){

            current = prev->nodList[i];
            if(current==NULL || current->state==BaseNod::VISITED) continue;
            double mc = getMoveCost(prev, i);

            /*  Jeżeli dotychczasowa ocena jest większa niż aktualnie wyliczona
             *  zostanie zaktualizowany i dodany do listy open wierzchołek z listy sąsiedztwa. */
            if(current->distanceValue > prev->distanceValue + mc){
                countNodList++;
                current->distanceValue = prev->distanceValue + mc;
                current->heuristicValue = current->distanceValue;
                current->prev=prev;
                current->distance=prev->distance+1;
                current->state=BaseNod::VISALL;
                update(current);
                inserNodToList(current);
            }
        }
    }
}
