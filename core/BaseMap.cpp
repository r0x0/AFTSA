#include "core/BaseMap.h"


BaseMap::BaseMap(){
    this->mapv=NULL;
    this->map=NULL;
    X=0;
    Y=0;
}


void BaseMap::generateMapModel4(unsigned char **mapv, int sizeX, int sizeY){
    deleteMap(false);
    this->X=sizeX;
    this->Y=sizeY;
    BaseNod ***newMap = new BaseNod**[this->X];

    for(int x=0; x<this->X; x++){
        newMap[x] = new BaseNod*[this->Y];
        for(int y=0; y<this->Y; y++){
            BaseNod *nod = new BaseNod(BaseNod::MAX_VALUE-mapv[x][y],x,y);
            newMap[x][y]= nod;
            if(y!=0){
                if(newMap[x][y-1]->value < BaseNod::MAX_VALUE) nod->nodList[0]=(newMap[x][y-1]);
                if(nod->value < BaseNod::MAX_VALUE) newMap[x][y-1]->nodList[2]=(nod);
            }
            if(x!=0){
                if(newMap[x-1][y]->value < BaseNod::MAX_VALUE) nod->nodList[1]=(newMap[x-1][y]);
                if(nod->value < BaseNod::MAX_VALUE) newMap[x-1][y]->nodList[3]=(nod);
            }
        }

    }
    this->map = newMap;
    this->mapv = mapv;
}

void BaseMap::generateMapModel8(unsigned char **mapv, int sizeX, int sizeY){
    this->X=sizeX;
    this->Y=sizeY;
    BaseNod ***newMap = new BaseNod**[this->X];

    for(int x=0; x<this->X; x++){
        newMap[x] = new BaseNod*[this->Y];
        for(int y=0; y<this->Y; y++){
            BaseNod *nod = new BaseNod(BaseNod::MAX_VALUE-mapv[x][y],x,y,8);
            newMap[x][y]= nod;
        }
    }

    for(int x=0; x<this->X; x++){
        for(int y=0; y<this->Y; y++){
            if(y!=0){
                if(newMap[x][y-1]->value < BaseNod::MAX_VALUE) newMap[x][y]->nodList[0]=(newMap[x][y-1]);
                if(newMap[x][y]->value < BaseNod::MAX_VALUE) newMap[x][y-1]->nodList[4]=(newMap[x][y]);

                if(x-1>-1){
                    if(newMap[x-1][y-1]->value < BaseNod::MAX_VALUE) newMap[x][y]->nodList[1]=(newMap[x-1][y-1]);
                    if(newMap[x][y]->value < BaseNod::MAX_VALUE) newMap[x-1][y-1]->nodList[5]=(newMap[x][y]);
                }

                if(x+1<X){
                    if(newMap[x+1][y-1]->value < BaseNod::MAX_VALUE) newMap[x][y]->nodList[7]=(newMap[x+1][y-1]);
                    if(newMap[x][y]->value < BaseNod::MAX_VALUE) newMap[x+1][y-1]->nodList[3]=(newMap[x][y]);
                }
            }
            if(x!=0){
                if(newMap[x-1][y]->value < BaseNod::MAX_VALUE) newMap[x][y]->nodList[2]=(newMap[x-1][y]);
                if(newMap[x][y]->value < BaseNod::MAX_VALUE) newMap[x-1][y]->nodList[6]=(newMap[x][y]);
            }
        }
    }
    this->map = newMap;
    this->mapv = mapv;
}

void BaseMap::initialiseMap(){
    if(map==NULL) return;
    for(int x=0; x<X; x++){
        for(int y=0;y<Y;y++){
            map[x][y]->init();
        }
    }
}


void BaseMap::deleteMap(bool all){
    if(map!=NULL){
        for(int x=0; x<X; x++){
            for(int y=0; y<Y; y++){
                delete map[x][y];
                map[x][y]=NULL;
            }
            delete[] map[x];
            if(all) delete[] mapv[x];
            map[x]=NULL;
            if(all) mapv[x]=NULL;
        }
        delete[] map;
        map=NULL;
        if(all){
            delete[] mapv;
            mapv=NULL;
        }
    }
}
