#ifndef ALGBESTFIRST_H
#define ALGBESTFIRST_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "alg/BaseAlg.h"


/**
 * @brief The AlgBestFirst class
 *Klasa implementacji algorytmu Best First Search.
 */
class AlgBestFirst : public BaseAlg{

public:

    AlgBestFirst(QObject *parent);
    ~AlgBestFirst(){}

public:

   /**
    * @brief Search
    *Deklaracja funkcj klasy bazowej, zawiera implementacje algorytmu.
    */
    void Search();

    /**
     * @brief h
     *Funkcja heurystyczna h(n).
     * @param nod
     * @return
     */
    inline double h(BaseNod *nod);
};

#endif // ALGBESTFIRST_H
