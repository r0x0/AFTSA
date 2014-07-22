#include "AlgAStar.h"


AlgAStar::AlgAStar(QObject *parent) : BaseAlg::BaseAlg(parent){}


void AlgAStar::Search(){

    //  Inicjacja stanu początkowego.
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
            double hv = h(current, prev->distanceValue, mc);

            /*  Jeżeli dotychczasowa ocena jest większa niż aktualnie wyliczona
             *  zostanie zaktualizowany i dodany do listy open wierzchołek z listy sąsiedztwa. */
            if(current->heuristicValue > hv){
                countNodList++;
                current->distanceValue = prev->distanceValue + mc;
                current->heuristicValue = hv;
                current->prev=prev;
                current->distance=prev->distance+1;
                current->state=BaseNod::VISALL;
                update(current);

                // Wstaw wierzchołek do kolejki open
                inserNodToList(current);
            }
        }
    }
}

inline double AlgAStar::h(BaseNod *nod, double prevValue, double value){
    double min = MAX_DOUBLE;

    //  Oblicza wartość funkcji heurystycznej według odległości Manhattan lub Euklidesowej,
    if(heuristic==HEURISTIC_MANHATTAN){

        // dla wszystkich wierzchołków końcowych
        for(std::list<BaseNod*>::const_iterator i = endlist->begin(); i!=endlist->end(); ++i){
            BaseNod *end = *i;
            double x = nod->x-end->x;
            if(x<0) x=x*(-1);
            double y = nod->y-end->y;
            if(y<0) y=y*(-1);
            x=((x+y)*(heuristicScale))+prevValue+value;

            // i wybiera ten o najmniejszym oszacowaniu.
            if(min>x) min =x;
        }
    }else{
        for(std::list<BaseNod*>::const_iterator i = endlist->begin(); i!=endlist->end(); ++i){
            BaseNod *end = *i;
            double x = nod->x-end->x;
            if(x<0) x=x*(-1);
            double y = nod->y-end->y;
            if(y<0) y=y*(-1);
            x=(std::sqrt(x*x+y*y)*heuristicScale)+prevValue+value;
            if(min>x) min = x;
        }
    }
    return min;
}
