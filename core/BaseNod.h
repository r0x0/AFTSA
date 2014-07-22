#ifndef NODBASE_H
#define NODBASE_H

#include "stdinc.h"


/**
 * @brief The BaseNod class
 * Klasa wierzchołka grafu i jednocześnie stanu w przestrzeni wyszukiwania.
 */
class BaseNod{

    //  *****************************************************************
    //  **                     STAŁE
    //  *****************************************************************
public:

    /**> Wartość dla wierzchołka nieodwiedzonego. */
    static const char NOTVISITED = 0;

    /**> Wartość dla wierzchołka odwiedzonego. */
    static const char VISITED = 1;

    /**> Wartość dla wierzchołka przetworzonego. */
    static const char VISALL = 2;

    /**> Wartość dla wierzchołka odwiedzonego początkowego. */
    static const char SPECSTART = 5;

    /**> Wartość dla wierzchołka odwiedzonego końcowego. */
    static const char SPECEND = 6;

    /**> Maksymalna wartość wierzchołka, brak przejścia. */
    static const unsigned char MAX_VALUE = 255;

    //  *****************************************************************
    //  **                     ZMIENNE
    //  *****************************************************************

    /**> Maksymalna ilość wierzchołków dla listy sąsiadów. */
    int MAX_NODS;

    /**> Status odwiedzin wierzchołka. */
    char state;

    /**> Odległość od punktu początkowego. */
    int distance;

    /**> Waga przejść od punktu początkowego. */
    double distanceValue;

    /**> Waga ocen funkcji heurystycznych. */
    double heuristicValue;

    /**> Czas odwiedzenia. */
    int timeVisit;

    /**> Czas przetworzenia listy sąsiadów. */
    int timeAllVisit;

    /**> Wartość kosztu przejścia do tego wierzchołka. */
    unsigned char value;

    /**> Jeżeli True wierzchołek jest wierzchołkiem końcowym. */
    bool endNode;

    /** Pozycja wierzchołka na mapie w poziomie */
    int x;

    /** Pozycja wierzchołka na mapie w pionie */
    int y;

    /**> Lista sąsiadow */
    BaseNod **nodList;

    /**> Rodzic w drzewie odwiedzin */
    BaseNod *prev;

    /**> indeks tablicy w kopcu wierzchołków do odwiedzenia */
    int indexHeap;

public:
    BaseNod();

    /**
     * @brief BaseNod
     *Konstruktor, którego należy używać do prawidłowego tworzenia wierzchołków.
     * @param value - wartość wierzchołka.
     * @param x - pozycja w poziomie na mapie
     * @param y - pozycja w pionie na mapie.
     * @param maxNod - maksymalna liczba wierzchołków w liście sąsiedztwa.
     * @param end - True - wierzchołek jest stanem końcowym.
     */
    BaseNod(unsigned char value, int x, int y,int maxNod=4, bool end=false);

    ~BaseNod(){ delete[] nodList; }

    /**
     * @brief init
     *Inicjuje zmienne stanu.
     */
    void init();
};

#endif // NODBASE_H
