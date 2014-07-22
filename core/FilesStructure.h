#ifndef FILESSTRUCTURE_H
#define FILESSTRUCTURE_H

#include "stdinc.h"


//TODO
/**
 * @brief The FilesStructure class
 *Klasa obsługująca wczytywanie i zapisywanie danych, w tym również uproszczony format mapy.
 *
 */
class FilesStructure{

public:
    FilesStructure();

public:
    void checkStructure();
    void loadConfiguration();
    void saveConfiguration();
    void loadMapConf();
    void saveMapConf();
    void loadMap();
    void saveMap();

};

#endif // FILESSTRUCTURE_H
