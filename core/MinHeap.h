#ifndef STOCKMINHEAP_H
#define STOCKMINHEAP_H

#include "core/BaseNod.h"


/**
 * @brief The MinHeap class
 *Klasa kopca typu min.
 */
class MinHeap{

public:
    MinHeap();

public:

    /**
     * @brief MIN_HEAPIFY
     *MIN-HEAPIFY to procedura służąca do utrzymywania własności kopca typu min.
     *Używana po ściągnięciu najmniejszego elementu i wstawieniu na jego miejesc ostatniego.
     *Wywołuje się rekurencyjnie po każdej zmianie elementów, jeżli któryś z liści pod-kopca
     *jest mniejszy niż element i.
     * @param A
     * @param i
     */
    static inline void MIN_HEAPIFY(std::vector<BaseNod*> *A, int i, long &counter){
        std::cout << "MIN_HEAPIFY I: " << i << std::endl;
        BaseNod *min;
        BaseNod *AI = (BaseNod*) A->at(i);
        BaseNod *l = NULL;
        BaseNod *r = NULL;
        unsigned int c = AI->indexHeap << 1;

        if(c < A->size()-1) l = A->at(c);
        c++;
        if(c < A->size()-1) r = A->at(c);

        if(l!=NULL && AI->heuristicValue > l->heuristicValue){
            min = l;
        }
        else min = AI;
        if(r!=NULL && min->heuristicValue > r->heuristicValue){
            min = r;
        }
        if(min != AI){
            counter++;
            int id = min->indexHeap;
            A->at(i) = min;
            min->indexHeap = AI->indexHeap;
            A->at(id) = AI;
            AI->indexHeap = id;

            MIN_HEAPIFY(A, id, counter);
        }
    }

    /**
     * @brief HEAP_MIN
     * Zwraca element o najmniejszej wartości.
     * @param A
     * @return
     */
    static inline BaseNod* HEAP_MIN(std::vector<BaseNod*> *A){
        if(A->size()==0) return NULL;
        return A->at(0);
    }

    /**
     * @brief HEAP_EXTRACT_MIN
     *Zwraca element o najmniejszej wartości i usuwa go z kopca.
     * @param A
     * @param counter
     * @return
     */
    static inline BaseNod* HEAP_EXTRACT_MIN(std::vector<BaseNod*> *A, long counter){

        if(A->size()==0) return NULL;
        BaseNod *min = (BaseNod*) A->at(0);
        BaseNod *end = (BaseNod*) A->at(A->size()-1);

        A->pop_back();

        // Wstaw element ostatni na początek stosu.
        if(A->size()>0){
            A->at(0) = end;
            end->indexHeap=0;
            // i sprawdź czy nie jest większy niż jego liście.
            MIN_HEAPIFY(A, 0, counter);
        }

        // Element ściągnięty ze stosu traci referencje.
        min->indexHeap=-1;
        return min;
    }

    /**
     * @brief HEAP_DECREASE_KEY
     *Procedura HEAP-DECREASE-KEY dla elementu i (nowego) tablicy A , ponieważ jego dodanie
     *mogło naruszyć strukturę kopca typu min, przebiega ścieżkę prostą od
     *elementu i w stronę korzenia zamieniając element A[i] z jego rodzicem A[ A[i].PARENT ]
     *do puki rodzic ma większą wartość.
     * @param A
     * @param i
     * @param counter
     */
    static inline void HEAP_DECREASE_KEY(std::vector<BaseNod*> *A, int i, long &counter){
        BaseNod *AI = (BaseNod*)A->at(i);

        while(i>0){
            BaseNod *pr = A->at(AI->indexHeap >> 1);
            if(AI->heuristicValue >= pr->heuristicValue) break;
            counter++;

            A->at(i) = pr;
            A->at(pr->indexHeap) = AI;
            int id = pr->indexHeap;
            pr->indexHeap = AI->indexHeap;
            AI->indexHeap = id;
            i = id;
        }
    }

    /**
     * @brief MIN_HEAP_INSERT
     *Dodaje wierzchołek do kopca.
     * @param A
     * @param nod
     */
    static inline void MIN_HEAP_INSERT(std::vector<BaseNod*> *A, BaseNod *nod, long &counter){

        // Jeżeli prevHeap nie jest null to wierzchołek znajduje się już w kopcu
        if(nod->indexHeap==-1){
            A->push_back(nod);
            nod->indexHeap = A->size()-1;
        }
        HEAP_DECREASE_KEY(A, nod->indexHeap, counter);
    }
};

#endif // STOCKMINHEAP_H
