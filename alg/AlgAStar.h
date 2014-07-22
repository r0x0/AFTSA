#ifndef ALGASTAR_H
#define ALGASTAR_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "alg/BaseAlg.h"


/**
 * @brief The AlgAStar class
 *Klasa implementacji algorytmu A*.
 */
class AlgAStar : public BaseAlg{

public:

    /**
     * @brief AlgAStar
     *Standardowy konstruktor.
     * @param parent
     */
    AlgAStar(QObject *parent);
    ~AlgAStar(){}

public:

    /**
     * @brief Search
     *Deklaracja funkcj klasy bazowej, zawiera implementacje algorytmu.
     */
    void Search();

    /**
     * @brief h
     *Funkcja heurystyczna f(n).
     * @param nod - stan przestrzeni do oceny
     * @param prevValue - wartość oceny heurystycznej poprzednika w drzewie wyszukiwania
     * @param value - wartość przejścia do tego wierzchołka
     * @return
     */
    inline double h(BaseNod *nod, double prevValue, double value);
};

#endif // ALGASTAR_H
