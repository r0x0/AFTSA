#ifndef BASEMAP_H
#define BASEMAP_H

#include "stdinc.h"
#include "core/BaseNod.h"


/**
 * @brief The BaseMap class
 *Klasa obiektu mapy. Zawiera macierz wierzchołków, metody jej tworzenia i usuwania.
 */
class BaseMap{

protected:

    /**> Nazwa mapy. */
    std::string name;

    /**> Opis mapy. */
    std::string descr;

    /** Rozmiar mapy w bajtach. */
    //long size;

    /** Ścieżka do pliku bmp. */
    std::string BMPFile;

    /** Wskaźnik na macierz wierzchołków. */
    BaseNod ***map;

    /** Wskaźnik na macierz wartości wierzchołków. */
    unsigned char **mapv;

    /** Ilość wierzchołków w poziomie mapy. */
    int X;

    /** Ilość wierzchołków w pionie mapy. */
    int Y;

public:
    BaseMap();
    ~BaseMap(){ deleteMap(true); }

public:
    /**
     * @brief generateMapModel
     *Generuje model grafu dla 4 operatorów.
     * @param mapv - Macierz wartości pól mapy
     * @param sizeX - rozmiar macierzy w poziomie
     * @param sizeY - rozmiar macierzy w pionie
     */
    void generateMapModel4(unsigned char **mapv, int sizeX, int sizeY);

    /**
     * @brief generateMapModel8
     *Generuje model grafu dla 8 operatorów.
     * @param mapv - Macierz wartości pól mapy
     * @param sizeX - rozmiar macierzy w poziomie
     * @param sizeY - rozmiar macierzy w pionie
     */
    void generateMapModel8(unsigned char **mapv, int sizeX, int sizeY);

public:

    /**
     * @brief getX
     *Zwraca szerokość mapy.
     * @return
     */
    int getX(){ return X; }

    /**
     * @brief getY
     *Zwraca Długość mapy.
     * @return
     */
    int getY(){ return Y; }

    /**
     * @brief getMapV
     *Zwraca macierz wartości wierzchołków.
     * @return
     */
    unsigned char** getMapV(){ return mapv; }

    /**
     * @brief getSize
     *Zwraca rozmiar mapy w bajtach.
     * @return
     */
    int getSize(){ return (sizeof(BaseNod)+sizeof(BaseNod*))*X*Y; }

    /**
     * @brief setEnd
     *Ustaw pole "wierzchołek końcowy" dla punktu (x,y)
     * @param x - współrzędna w poziomie.
     * @param y - współrzędna w pionie.
     * @param end - czy ma być wierzchołkiem końcowym.
     * @return
     */
    void setEnd(int x, int y, bool end){
        map[x][y]->endNode=end;
    }

    /**
     * @brief getNodAt
     *Zwraca wierzchołek o współrzędnych (x, y).
     * @param x
     * @param y
     * @return
     */
    inline BaseNod* getNodAt(int x, int y){
        return map[x][y];
    }

    /**
     * @brief initialiseMap
     *Inicjuje zmienne stanu w instacjach obiektów klasy BaseNod (wierzchołek).
     */
    void initialiseMap();

    /**
     * @brief deleteMap
     *Usuwa istniejącą macierz wierzchołków wraz z wierzchołkami.
     * @param all
     */
    void deleteMap(bool all);
};

#endif // BASEMAP_H
