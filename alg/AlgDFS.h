#ifndef ALGDFS_H
#define ALGDFS_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "alg/BaseAlg.h"

/**
 * @brief The AlgDFS class
 *Klasa implementacji algorytmu Depth First Search.
 */
class AlgDFS : public BaseAlg{

public:

    AlgDFS(QObject *parent);

    ~AlgDFS(){   }

public:

    /**
     * @brief Search
     *Deklaracja funkcj klasy bazowej, zawiera implementacje algorytmu.
     */
    void Search();

    /**
     * @brief Visit
     *Rekurencyjna funkcja odwiedzająca wierzchołek.
     * @param u - wierzcholek do odwiedzenia
     */
    void Visit(BaseNod *u);
};

#endif // ALGDFS_H
