#ifndef BITMAP_H
#define BITMAP_H

#include "stdinc.h"


/**** BMP file header structure ****/
#pragma pack(push, 1)                /** Pakowanie struktury */
typedef struct
{
    unsigned short bfType;           /** Magic number for file */
    unsigned int   bfSize;           /** Size of file */
    unsigned short bfReserved1;      /** Reserved */
    unsigned short bfReserved2;      /** ... */
    unsigned int   bfOffBits;        /** Offset to bitmap data */
} BITMAPFILEHEADER;


/**** BMP file info structure ****/
typedef struct
{
    unsigned int   biSize;           /** Rozmiar tego nagłówka */
    int   biWidth;                   /** Szerokość obrazu */
    int   biHeight;                  /** Wysokość obrazu */
    unsigned short biPlanes;         /** Ilość warstw */
    unsigned short biBitCount;       /** Bitów na pixel */
    unsigned int   biCompression;    /** Typ kompresji */
    unsigned int   biSizeImage;      /** Rozmiar tabeli pikseli */
    unsigned int   biXPixPerM;       /** X pikseli na metr */
    unsigned int   biYPixPerM;       /** Y pikseli na metr */
    unsigned int   biClrUsed;        /** Number of colors used */
    unsigned int   biClrImportant;   /** Number of important colors */
} BITMAPINFOHEADER;


/**** Struktura RGB + ALPHA ****/
typedef struct
{
    unsigned char  rgbBlue;          /** NIEBIESKI */
    unsigned char  rgbGreen;         /** ZIELONY */
    unsigned char  rgbRed;           /** CZERWONY */
    unsigned char  rgbReserved;      /** ALFA */
} RGBQUAD;


/**** Struktura RGB ****/
typedef struct
{
    unsigned char  rgbBlue;          /** NIEBIESKI */
    unsigned char  rgbGreen;         /** ZIELONY */
    unsigned char  rgbRed;           /** CZERWONY */
} RGB;
#pragma pack(pop)


/**
 * Constants for the biCompression field...
 */
#  define BI_RGB       0             /** No compression - straight BGR data */
#  define BI_RLE8      1             /** 8-bit run-length compression */
#  define BI_RLE4      2             /** 4-bit run-length compression */
#  define BI_BITFIELDS 3             /** RGB bitmap with RGB masks */
#  define BI_JPG       4             /** JPEG or RLE-24 compression for BITMAPCOREHEADER2 */
#  define BI_PNG       5             /** PNG */
#  define BI_ALPHABITFIELDS 6        /** Bit field */

#  define BF_TYPE 0x4D42             /* "MB" */


/**
 * @brief The Bitmap class
 *Klasa obsługująca pliki BMP.
 */
class Bitmap {

private:

    /**> Struktura nagłówka pliku. */
    BITMAPFILEHEADER fh;

    /**> Struktura nagłówka informacyjnego. */
    BITMAPINFOHEADER ih;

    /**> Ostatni komunikat wczytywania pliku. */
    std::stringstream message;

    /**> rozmiar struktury nagłówka pliku. */
    const int BMP_INFO_SIZE=40;

    /**> Rozmiar pliku. */
    long fileSize;

    /**> Ilość pikseli w poziomie. */
    int X;

    /**> Ilość pikseli w pionie. */
    int Y;

    /**> Macierz wartości dla pikseli po transformacji z RGB */
    unsigned char **map;

public:
    Bitmap();
    ~Bitmap(){   }


public:
    /**
     * @brief getCompDesc
     *Dla podanej wartości liczbowej zwraca opis kodowania pliku.
     * @param compressionType
     * BI_RGB       0             No compression - straight BGR data
     * BI_RLE8      1             8-bit run-length compression
     * BI_RLE4      2             4-bit run-length compression
     * BI_BITFIELDS 3             RGB bitmap with RGB masks
     * BI_JPG       4             JPEG or RLE-24 compression for BITMAPCOREHEADER2
     * BI_PNG       5             PNG
     * BI_ALPHABITFIELDS 6        Bit field
     * @return
     */
    std::string getCompDesc(int compressionType);

    /**
     * @brief readFile
     *Wczytuje plik ze ścieżki filname i na podstawie odczytanych pikseli
     *generuje mape w postaci short **. Dla mapy kolorowej kolory zostaną
     *uśrednione. Plik BMP powinien być zapisany bez kompresji w postaci RGB
     *z 24 bitami na piksel.
     * @param filename
     */
    void readFile(std::string filename);


    //  ***********************************************************************
    //  **                     GETTERS
    //  ***********************************************************************

    /**
     * @brief getMessage
     *Pobiera ostatnio zachowaną wiadomość diagnostyczną.
     * @return
     */
    std::string getMessage(){ return this->message.str(); }

    /**
     * @brief getX
     *Zwraca szerokość wczytanej mapy.
     * @return
     */
    int getX() { return X; }

    /**
     * @brief getY
     *Zwraca wysokość wczytanej mapy.
     * @return
     */
    int getY() { return Y; }

    /**
     * @brief getMap
     *Zwraca utworzony model mapy.
     * @return
     */
    unsigned char** getMap(){ return map; }
};

#endif // BITMAP_H
