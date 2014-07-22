#ifndef ALGDIJKSTRA_H
#define ALGDIJKSTRA_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "alg/BaseAlg.h"


/**
 * @brief The AlgDijkstra class
 *Klasa implementacji algorytmu Dijkstry.
 */
class AlgDijkstra : public BaseAlg{

public:
    AlgDijkstra(QObject *parent);
    ~AlgDijkstra(){}

public:

    /**
    * @brief Search
    *Deklaracja funkcj klasy bazowej, zawiera implementacje algorytmu.
    */
    void Search();
};

#endif // ALGDIJKSTRA_H
