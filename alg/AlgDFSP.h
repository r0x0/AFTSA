#ifndef ALGDFSP_H
#define ALGDFSP_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "alg/BaseAlg.h"


/**
 * @brief The AlgDFSP class
 *Klasa implementacji algorytmu Depth First Search z ograniczeniem i pogłębianiem.
 */
class AlgDFSP : public BaseAlg{

private:
    /**> Zmienna pomocnicza, informuje, że wszystkie stany na liście open zostału juz sprawdzone. */
    bool alv;

public:    
    AlgDFSP(QObject *parent);
    ~AlgDFSP(){   }

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

#endif // ALGDFSP_H
