#include "core/Bitmap.h"


Bitmap::Bitmap() {
    map=NULL;
    X=0;
    Y=0;
}

void Bitmap::readFile(std::string filename){
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    map=NULL;

    if (file.bad()) {
        message << "Nie można odczytać pliku";
        return;
    }

    if (file.is_open() == false) {
        message << "Nie można otworzyć pliku";
        return;
    }


    file.seekg(0 , std::ios::end);
    this->fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if(fileSize<54){
        message << "Plik uszkodzony\nniepoprawna wilkość pliku: " << fileSize <<" bajtów";
        file.close();
        return;
    }



    /** Odczyt nagłówka */
    file.read((char*) &fh, sizeof(BITMAPFILEHEADER));

    if(fh.bfType != BF_TYPE){
        message << "Plik uszkodzony\nNiepoprawny typ pliku: " << fh.bfType;
        file.close();
        return;
    }

    std::cout.fill('.');


    file.read((char*) &ih, sizeof(BITMAPINFOHEADER));


    if(ih.biSizeImage == 0){
        message << "Mapa ma za mały rozmiar: 0";
        file.close();
        return;
    }

    if(ih.biCompression > BI_RGB){
        message << "Nieobsługiwany typ kompresji: " << getCompDesc(ih.biCompression) << std::endl <<
                   "Obsługiwane typy kompresji: " << getCompDesc(BI_RGB);
        file.close();
        return;
    }

    if(ih.biBitCount!=24){
        message << "Nieobsługiwany liczba bit/pix: " << ih.biBitCount << std::endl <<
                   "Obsługiwane formaty: 24";
        file.close();
        return;
    }

    this->X = ih.biWidth;
    this->Y = ih.biHeight;

    file.seekg(fh.bfOffBits, std::ios::beg);

    ////////////////////////////////////////////////////////////////////////

    short bytesPP = ih.biBitCount / 8;
    unsigned char **map = new unsigned char*[ih.biWidth];

    for(int x = 0; x< ih.biWidth; x++){
        map[x] = new unsigned char[ih.biHeight];
    }

    RGB tmp;
    int ttmp=0;
    int ssize = ((((ih.biBitCount * ih.biWidth) + 31 )/32 )*4) - (bytesPP * ih.biWidth);

    /** Wczytanie pliku do tablicy */
    for(int y = ih.biHeight-1; y>=0; y--){


        for(int x = 0; x< ih.biWidth; x++){
            file.read((char*) &tmp, bytesPP);
            map[x][y] = (tmp.rgbRed+tmp.rgbGreen+tmp.rgbBlue)/3;
        }
        file.read((char*) &ttmp, ssize);
    }

    file.close();
    this->map = map;

}


std::string Bitmap::getCompDesc(int compressionType){
    std::string ret;
    switch(compressionType){
        case BI_RGB : ret= "Bez kompresji [RGB]"; break;
        case BI_RLE8 : ret= "RLE8"; break;
        case BI_RLE4 : ret= "RLE4"; break;
        case BI_BITFIELDS : ret= "BITFIELDS"; break;
        case BI_JPG : ret= "JPG albo RLE24"; break;
        case BI_PNG : ret= "PNG"; break;
        case BI_ALPHABITFIELDS : ret= "ALPHABITFIELDS"; break;
        default : ret = "Nieznany typ"; break;
    }
    return ret;
}
