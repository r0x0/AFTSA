#include "core/BaseNod.h"


BaseNod::BaseNod(){
    this->value=-1;
    state=NOTVISITED;
    distance=INT_MAX;
    distanceValue=0xFFFFFFFF;
    heuristicValue=0xFFFFFFFF;
    indexHeap=-1;
    prev=NULL;

}


BaseNod::BaseNod(unsigned char value, int x, int y, int maxNod, bool end) : BaseNod(){
    this->value=value;
    this->x=x;
    this->y=y;
    this->endNode=end;
    this->MAX_NODS=maxNod;
    this->nodList=new BaseNod*[MAX_NODS];
    for(int i=0;i<MAX_NODS; i++){
        nodList[i]=NULL;
    }
}

void BaseNod::init(){
    distanceValue=0xFFFFFFFF;
    distance=INT_MAX;
    heuristicValue=0xFFFFFFFF;
    indexHeap=-1;
    prev=NULL;
    state=NOTVISITED;
}
