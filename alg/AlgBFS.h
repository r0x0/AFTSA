#ifndef ALGBFS_H
#define ALGBFS_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "alg/BaseAlg.h"


/**
 * @brief The AlgBFS class
 *Klasa implementacji algorytmu Breadth First Search.
 */
class AlgBFS : public BaseAlg{

public:

    AlgBFS(QObject *parent);
    ~AlgBFS(){   }

public:

    /**
     * @brief Search
     *Deklaracja funkcj klasy bazowej, zawiera implementacje algorytmu.
     */
    void Search();
};

#endif // ALGBFS_H
