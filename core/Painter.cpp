#include "Painter.h"


Painter::Painter(QWidget *parent) : QWidget(parent){
    rectMap=NULL;
    mapv=NULL;
    start =NULL;
    start2 =NULL;
    prev=NULL;
    delStart1=NULL;
    endR=NULL;
    delEndRect=NULL;
    step=NULL;
    specialstart=NULL;
    specialEnd=NULL;
    lineT=NULL;
    lineTP=NULL;
    X=0;
    Y=0;
    startEve=false;
    endEve=false;
    draw =true;
    noSimulation=true;
    saerchTree=false;
    block =false;
    NoUpdate = true;
    NoSearchTree = true;
    reverseNod = false;
    drawSTree = true;
    drawSNods = true;
    colorDiv=120;
    lastState=-1;
    scale=0;
    prevX=-1;
    prevY=-1;
    emitXY=false;

    endRect = new std::list<QRect*>;
    visited=new std::list<QRect*>;
    allvisited=new std::list<QRect*>;
    lines=new std::list<QLine*>;
    linesTree = new std::list<QLine*>;

    color = new QColor(95,179,228);
    pen = new QPen();
    pen->setColor(*color);
    pen->setWidth(1);
    brush = new QBrush();
    cStart = new QColor(0,255,0);
    cEnd = new QColor(0,0,255);
    cVisit = new QColor(245,200,20);
    cAllV = new QColor(255,0,0);
    cStart2 = new QColor(0,255,0,colorDiv);
    cEnd2 = new QColor(0,0,255,colorDiv);
    cVisit2 = new QColor(245,200,20,colorDiv);
    cAllV2 = new QColor(255,0,0,colorDiv);
    nullColor =new QColor(255,255,255,0);
    linePenn = new QPen();
    cBlack = new QColor(0,0,0,255);
    linePenn->setWidth(2);
    linePenn->setColor(*cBlack);
    lineTPen = new QPen();
    cGray = new QColor(150,150,150,255);
    lineTPen->setWidth(2);
    lineTPen->setColor(*cGray);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

void Painter::pUp(){
    delete prev;
    prev=NULL;
    delete start2;
    start2=NULL;
    delete endR;
    endR=NULL;
    update();
}

void Painter::paintEvent(QPaintEvent *){

    QPainter painter(this);

    if(rectMap==NULL || mapv==NULL || block){
        return;
    }
    if(noSimulation){
        if(draw){
            drawMap(painter);
        }

        if(delStart1!=NULL){
            colorRect(painter, delStart1, *cAllV2, cStart);
        }else if(start!=NULL){
            int r = mapv[(long)floor(start->x()/LINE)][(long)floor(start->y()/LINE)];
            QColor coll(r,r,r);
            colorRect(painter, start, coll, color, true);
            colorRect(painter, start, *cStart2, cStart);
        }
        if(start2!=NULL){
            int r = mapv[(long)floor(start2->x()/LINE)][(long)floor(start2->y()/LINE)];
            QColor coll(r,r,r);
            colorRect(painter, start2, coll, color, true);
            colorRect(painter, start2, *nullColor, cStart);
        }

        if(endR!=NULL){
            int r = mapv[(long)floor(endR->x()/LINE)][(long)floor(endR->y()/LINE)];
            QColor coll(r,r,r);
            colorRect(painter, endR, coll, color, true);
            colorRect(painter, endR, *nullColor, cEnd);
        }
        if(delEndRect!=NULL){
            colorRect(painter, delEndRect, *cAllV2, cEnd);
        }else{
            for(QRect *rect : *endRect){
                int r = mapv[(long)floor(rect->x()/LINE)][(long)floor(rect->y()/LINE)];
                QColor coll(r,r,r);
                colorRect(painter, rect, coll, color, true);
                colorRect(painter, rect, *cEnd2, cEnd);
            }
        }

        if(drawSNods){
            for(QRect *rect : *visited){

                colorRect(painter, rect, *cVisit2, cVisit);
            }
            for(QRect *rect : *allvisited){
                colorRect(painter, rect, *cAllV2, cAllV);
            }
        }
        if(drawSTree){
            for(QLine *line : *linesTree){
                colorLine(painter, line, true);
            }
        }
        for(QLine *line : *lines){
            colorLine(painter, line, false);
        }

        if(prev!=NULL){
            int x=(long)floor(prev->x()/LINE);
            int y=(long)floor(prev->y()/LINE);
            int r = mapv[x][y];
            QColor coll(r,r,r);
            colorRect(painter, rectMap[x][y], coll, color, true);
        }
    }else{
         if(step!=NULL){
             int r = mapv[(long)floor(step->x()/LINE)][(long)floor(step->y()/LINE)];
             QColor coll(r,r,r);
             QColor *c = NULL;
             QColor *c2 = NULL;
             if(lastState==BaseNod::VISITED){
                 c= cVisit2;
                 c2=cVisit;
             }
             else{
                 c = cAllV2;
                 c2 = cAllV;
             }
             colorRect(painter, step, coll, color, true);
             colorRect(painter, step, *c, c2);
         }
         if(lineT!=NULL){
             colorLine(painter, lineT, true);
         }
         if(lineTP!=NULL){
             colorLine(painter, lineTP, true);
         }
    }
    if(specialstart!=NULL){
        colorRect(painter, specialstart, *nullColor, cStart);
    }
    if(specialEnd!=NULL){
        colorRect(painter, specialEnd, *nullColor, cEnd);
    }

    draw=true;
    noSimulation=true;
}

void inline Painter::colorRect(QPainter &painter, QRect *rect, QColor &colorB, QColor *colorP, bool std){
    if(std){
        painter.setPen(*this->pen);
    }else{
        QPen penn;
        penn.setWidth(2);
        penn.setColor(*colorP);
        painter.setPen(penn);
    }
    painter.setBrush(QBrush(colorB));
    painter.drawRect(*rect);
}

void inline Painter::colorLine(QPainter &painter, QLine *line, bool tree){
//    if(DEBUG>=2) std::cout << "Painter::colorLine: " << line << std::endl ;
    if(!tree) painter.setPen(*linePenn);
    else painter.setPen(*lineTPen);
    painter.drawLine(*line);
}


void Painter::mousePressEvent(QMouseEvent *){
//    if(checkRects()) return;

    // Malowanie dla pola początkowego
    if(startEve){
        if(start!=NULL && delStart1!=NULL && start->x()==delStart1->x() && start->y()==delStart1->y()){
            delete prev;
            prev=new QRect(start->x(),start->y(), LINE-3, LINE-3);
            delete start;
            start=NULL;
            delete delStart1;
            delStart1=NULL;
            emit posChanged(0,0);
            return;
        }
        if(start2!=NULL){
            if(start!=NULL){
                delete prev;
                prev=new QRect(start->x(),start->y(), LINE-3, LINE-3);
                rePP(prev);
            }
            delete start;
            start = new QRect(start2->x(),start2->y(), LINE-3, LINE-3);
            delete start2;
            start2=NULL;
            delStart1=new QRect(start->x(),start->y(), LINE-3, LINE-3);
            int nx = (start->x())/LINE+1;
            int ny = (start->y())/LINE+1;
            emit posChanged(nx,ny);
            return;
        }
    }
    // Malowanie dla pola końcowgo
    if(endEve){
        for(QRect *rect : *endRect){
            if(delEndRect!=NULL && rect->x()==delEndRect->x() && rect->y()==delEndRect->y()){
                delete prev;
                prev=new QRect(delEndRect->x(),delEndRect->y(), LINE-3, LINE-3);
                delete delEndRect;
                delEndRect=NULL;
                endRect->remove(rect);
                delete rect;
                int nx = (prev->x())/LINE+1;
                int ny = (prev->y())/LINE+1;
                emit endPosChanged(nx,ny);
                return;
            }
        }
        if(endR!=NULL){
            QRect  *rect = new QRect(endR->x(),endR->y(), LINE-3, LINE-3);
            delete endR;
            endR=NULL;
            delEndRect=new QRect(rect->x(),rect->y(), LINE-3, LINE-3);
            int nx = (rect->x())/LINE+1;
            int ny = (rect->y())/LINE+1;
            endRect->push_back(rect);
            emit endPosChanged(nx,ny);
            return;
        }
    }
}


void Painter::mouseReleaseEvent(QMouseEvent *){
    if(startEve){
        if(prev!=NULL) rePP(prev);
        if(start2!=NULL) rePP(start2);
        if(start!=NULL) rePP(start);
    }
    if(endEve){
        if(prev!=NULL) rePP(prev);
        if(endR!=NULL) rePP(endR);
        if(delEndRect!=NULL) rePP(delEndRect);
    }
    delete prev;
    prev=NULL;
}


void Painter::mouseMoveEvent(QMouseEvent *event){
    if(emitXY){
        int xx = floor(event->x()/LINE);
        int yy = floor(event->y()/LINE);
        if(xx>((X-1)*LINE)) xx=-1;
        if(yy>((Y-1)*LINE)) yy=-1;
        if(prevX!= xx || prevY!=yy){
            emit changeXY(xx,yy, event->x(), event->y());
            prevX=xx;
            prevY=yy;
        }
    }
    if(startEve){

        if(start2!=NULL){
            delete prev;
            prev=new QRect(start2->x(),start2->y(), LINE-3, LINE-3);
        }

        delete delStart1;
        delStart1=NULL;
        delete start2;
        start2=NULL;

        int xx = floor(event->x()/LINE)*LINE+2;
        int yy = floor(event->y()/LINE)*LINE+2;
        if(xx>((X-1)*LINE)) xx=(X-1)*LINE+2;
        if(yy>((Y-1)*LINE)) yy=(Y-1)*LINE+2;
        for(QRect *rect : *endRect){
            if(rect->x()==xx && rect->y()==yy){
                return;
            }
        }
//        if(mapv[xx][yy]==BaseNod::MAX_VALUE) return;
        start2 = new QRect(xx,yy,LINE-3,LINE-3);

        if(prev!=NULL && prev->x()==xx && prev->y()==yy){
            delete prev;
            prev=NULL;
        }
        if(prev!=NULL){
            rePP(prev);
        }
        if(start!=NULL && (start->x()==start2->x() && start->y()==start2->y())){
            delStart1=new QRect(start2->x(),start2->y(), LINE-3, LINE-3);
            delete start2;
            start2=NULL;
            delete prev;
            prev=NULL;
            rePP(delStart1);
            return;
        }
        if(start!=NULL){
            rePP(start);
        }
        rePP(start2);
        delete prev;
        prev=NULL;
    }else if(endEve){
        if(endR!=NULL){
            delete prev;
            prev=new QRect(endR->x(),endR->y(), LINE-3, LINE-3);
        }

        delete delEndRect;
        delEndRect=NULL;
        delete endR;
        endR=NULL;

        int xx = floor(event->x()/LINE)*LINE+2;
        int yy = floor(event->y()/LINE)*LINE+2;
        if(xx>((X-1)*LINE)) xx=(X-1)*LINE+2;
        if(yy>((Y-1)*LINE)) yy=(Y-1)*LINE+2;
        if(start!=NULL && start->x()==xx && start->y()==yy) return;
        endR = new QRect(xx,yy,LINE-3,LINE-3);

        if(prev!=NULL && prev->x()==xx && prev->y()==yy){
            delete prev;
            prev=NULL;
        }
        if(prev!=NULL){
            rePP(prev);
        }
        for(QRect *rect : *endRect){
            if(rect->x()==endR->x() && rect->y()==endR->y()){
                delEndRect=new QRect(endR->x(),endR->y(), LINE-3, LINE-3);
                delete endR;
                endR=NULL;
                delete prev;
                prev=NULL;
                rePP(delEndRect);
                return;
            }
        }
        for(QRect *rect : *endRect){
            rePP(rect);
        }
        rePP(endR);
        delete prev;
        prev=NULL;
    }
}


void Painter::repaintField(BaseNod* nod, uchar nodState){
//    if(DEBUG>=2) std::cout << "Painter::repaintField" << std::endl;
    int x=nod->x*LINE+2;
    int y=nod->y*LINE+2;

    if(NoSearchTree && nod->prev!=NULL){
        int xx=nod->prev->x*LINE+2;
        int yy=nod->prev->y*LINE+2;
        step = new QRect(xx,yy,LINE-3,LINE-3);;
        lastState = nod->prev->state;
        rePP(step,false);
        delete step;
        step =NULL;
    }
    if(nodState==0){
        step=new QRect(x,y,LINE-3,LINE-3);
        if(nod->state==BaseNod::VISITED){
            lastState=BaseNod::VISITED;
            visited->push_back(step);
            if(reverseNod){
                for(QRect *rect : *allvisited){
                    if(x==rect->x() && y==rect->y()){
                        allvisited->remove(rect);
                        delete rect;
                        break;
                    }
                }
            }
        }else if(nod->state==BaseNod::VISALL){
            lastState=BaseNod::VISALL;
            allvisited->push_back(step);
            if(!reverseNod){
                for(QRect *rect : *visited){
                    if(x==rect->x() && y==rect->y()){
                        visited->remove(rect);
                        delete rect;
                        break;
                    }
                }
            }
        }else{
            delete step;
            step=NULL;
        }
    }else{
        if(nodState==BaseNod::SPECSTART){
            delete specialstart;
            specialstart=new QRect(x,y,LINE-3,LINE-3);
            if(NoUpdate) rePP(specialstart,false);
        }else if(nodState==BaseNod::SPECEND){
            delete specialEnd;
            specialEnd=new QRect(x,y,LINE-3,LINE-3);
            if(NoUpdate) rePP(specialEnd,false);
        }
        return;
    }
    if(step!=NULL && NoUpdate){
            rePP(step,false);
    }
}


void Painter::rePP(QRect *rect, int simu){
    draw=false;
    noSimulation=simu;
    repaint(rect->x()-1,rect->y()-1,LINE-1,LINE-1);
}


void Painter::fixMap(){
    if(start2!=NULL){
        delete prev;
        prev=new QRect(start2->x(),start2->y(), LINE-3, LINE-3);
        delete start2;
        start2=NULL;
        rePP(prev);
    }
    if(endR!=NULL){
        delete prev;
        prev=new QRect(endR->x(),endR->y(), LINE-3, LINE-3);
        delete endR;
        endR=NULL;
        rePP(prev);
    }
}


void Painter::drawMap(QPainter &painter){

    painter.setPen(*this->pen);
    painter.setBrush(*brush);

    for(int x=0; x<X; x++){
        for(int y=0; y<Y; y++){
            int col = mapv[x][y];
            painter.setBrush(QBrush(QColor(col,col,col)));
            painter.drawRect(*rectMap[x][y]);
        }
    }
}


void Painter::generateMap(unsigned char **mapv, int x, int y){
    delRM();
    setMinimumSize(LINE*x+1,LINE*y+1);
    setMaximumSize(LINE*x+1,LINE*y+1);
    this->X = x;
    this->Y = y;
    this->mapv = mapv;
    rectMap = new QRect**[x];
    for(int i=0; i<x; i++){
        rectMap[i] = new QRect*[y];
        for(int j=0; j<y; j++){
            rectMap[i][j]=new QRect(i*LINE,j*LINE,LINE,LINE);
        }
    }
    update();
}


void Painter::reproducPatch(BaseNod* endNod){
//    if(DEBUG>=2) std::cout << "Painter::reproducPatch(BaseNod* endNod)" <<std::endl;
    BaseNod *previ = endNod;

    while(true){
        BaseNod *cur = previ->prev;
//        if(DEBUG>=2) std::cout << "WHILE cur: " << cur << std::endl;
        if(cur==NULL) break;
        QLine *line = new QLine(previ->x*LINE+HLINE,previ->y*LINE+HLINE,cur->x*LINE+HLINE,cur->y*LINE+HLINE);
        lines->push_front(line);

//        repaint(previ->x,previ->y,cur->x,cur->y);
        previ=cur;
    }
    std::list<QRect*> *remlist = new std::list<QRect*>;
    for(std::list<QRect*>::const_iterator i = visited->begin(); i!=visited->end(); ++i){
        std::list<QRect*>::const_iterator j = i;
        QRect *qr = *i;
        ++j;
        for(;j!=visited->end();++j){
            QRect *r = *j;
            if(qr->x()==r->x() && qr->y()==r->y()){
                remlist->push_back(r);
            }
        }
    }
    for(QRect *r : *remlist){
        visited->remove(r);
    }
//    remlist->unique();
//    while(!remlist->empty()) {
//        QRect *r = remlist->front();
//        remlist->pop_front();
//        delete r;
//        r=NULL;
//    }

    for(std::list<QRect*>::const_iterator i = allvisited->begin(); i!=allvisited->end(); ++i){
        std::list<QRect*>::const_iterator j = i;
        QRect *qr = *i;
        ++j;
        for(;j!=allvisited->end();++j){
            QRect *r = *j;
            if(qr->x()==r->x() && qr->y()==r->y()){
                remlist->push_back(r);
            }
        }
    }
    for(QRect *r : *remlist){
        allvisited->remove(r);
    }
//    remlist->unique();
//    while(!remlist->empty()) {
//        QRect *r = remlist->front();
//        remlist->pop_front();
//        delete r;
//        r=NULL;
//    }
    delete remlist;


    std::list<QRect*> *master;
    std::list<QRect*> *slave;
    if(reverseNod){
        master = visited;
        slave = allvisited;

        for(QRect *rect : *master){
            for(QRect *srect : *slave){
                if(srect->x()==rect->x() && srect->y()==rect->y()){
                    slave->remove(srect);
                    delete srect;
                    break;
                }
            }
        }
    }
    update();
}

void Painter::drawTreeLine(BaseNod *nod){
     BaseNod *cur = nod->prev;
     if(cur==NULL) return;
     QLine *line = new QLine(nod->x*LINE+HLINE,nod->y*LINE+HLINE,cur->x*LINE+HLINE,cur->y*LINE+HLINE);
     linesTree->push_back(line);
     if(NoSearchTree){
         lineTP=lineT;
         lineT=line;
     }
//     if(nod->x==cur->x) repaint(line->x1(),line->y1(), LINE, 2);
//     else repaint(line->x1(),line->y1(), 2, LINE);
}


void Painter::generateMap(){
    for(int i=0; i<X; i++){
        for(int j=0; j<Y; j++){
            delete rectMap[i][j];
            rectMap[i][j]=new QRect(i*LINE,j*LINE,LINE,LINE);
        }
    }
}


void Painter::clearSearch(){

    while(!visited->empty()) {
            QRect *r = visited->front();
            visited->pop_front();
            delete r;
    }

    while(!allvisited->empty()) {
        QRect *r =  allvisited->front();
        allvisited->pop_front();
        delete r;
    }

    while(!lines->empty()) {
        QLine * l = lines->front();
        lines->pop_front();
        delete l;
    }

    while(!linesTree->empty()) {
        QLine * l = linesTree->front();
        linesTree->pop_front();
        delete l;
    }
    delete specialEnd;
    delete specialstart;
    specialstart=NULL;
    specialEnd=NULL;
    lastState=-1;
    step=NULL;
    lineT=NULL;
}

void Painter::delRM(){
    clearSearch();

    endRect->clear();
    delete start;
    delete start2;
    delete prev;
    delete delEndRect;
    delete delStart1;
    delete endR;

    start =NULL;
    start2 =NULL;
    prev=NULL;
    delStart1=NULL;
    endR=NULL;
    delEndRect=NULL;
    for(int x=0; x<X; x++){
        for(int y=0; y<Y; y++){
            delete rectMap[x][y];
            rectMap[x][y]=NULL;
        }
        delete[] rectMap[x];
        rectMap[x]=NULL;
    }
    delete[] rectMap;
    rectMap=NULL;
}


void Painter::setScale(int scale){
    if(this->scale==scale) return;
    block=true;
//    if(DEBUG>=2) std::cout << "Painter::setScale: " << scale << std::endl;
    resizeRect(start, scale);
//    if(DEBUG>=2) std::cout << "Painter::setScale start: "<< start << std::endl;
    resizeRect(start2, scale);
    resizeRect(delStart1, scale);
    resizeRect(prev, scale);
    resizeRect(delEndRect, scale);
    resizeRect(endR, scale);
    for(QRect *rect : *endRect){
        resizeRect(rect, scale);
    }
    for(QRect *rect : *visited){
        resizeRect(rect, scale);
    }
    for(QRect *rect : *allvisited){
        resizeRect(rect, scale);
    }
    for(QLine *line : *lines){
        resizeLine(line, scale);
    }
    for(QLine *line : *linesTree){
        resizeLine(line, scale);
    }
    this->scale=scale;
    this->LINE=scale*RECT_MIN_LENGTH;
    this->HLINE=(LINE/2)+1;

    generateMap();
    setMinimumSize(LINE*X+1,LINE*Y+1);
    setMaximumSize(LINE*X+1,LINE*Y+1);
    block=false;
    update();
}


void Painter::resizeLine(QLine *line, int scl){
    if(line==NULL) return;
//    if(DEBUG>=2) std::cout << "Painter::resizeLine"<< line << std::endl;
    int newLine =(scl*RECT_MIN_LENGTH);
    int newLineH =newLine/2+1;
    int x=line->x1()/LINE*newLine+newLineH;
    int y=line->y1()/LINE*newLine+newLineH;
    int x2=line->x2()/LINE*newLine+newLineH;
    int y2=line->y2()/LINE*newLine+newLineH;
    delete line;
    line=new QLine(x,y,x2,y2);
}


void Painter::resizeRect(QRect *rect, int scl){
    if(rect==NULL) return;
//    if(DEBUG>=2) std::cout << "Painter::resizeRect"<< rect << std::endl;
    int line =scl*RECT_MIN_LENGTH;
    int x=rect->x()/LINE*line+2;
    int y=rect->y()/LINE*line+2;
    delete rect;
    rect=new QRect(x,y,line-3,line-3);
}


void Painter::changePos(int x, int y, bool del){
    int xx = (x-1)*LINE+2;
    int yy = (y-1)*LINE+2;
    if(startEve){
        if(start!=NULL){
            delete prev;
            prev=new QRect(start->x(),start->y(), LINE-3, LINE-3);
            rePP(prev);
        }
        start = new QRect(xx, yy, LINE-3, LINE-3);
        rePP(start);
    }else if(endEve && del){
        for(std::list<QRect*>::const_iterator i = endRect->end(); i!=endRect->begin();++i){
            QRect *rect = *i;
            if(rect->x()==xx && rect->y()==yy){
                delete prev;
                prev = new QRect(xx,yy,LINE-3, LINE-3);
                endRect->remove(rect);
                delete rect;
                rePP(prev);
            }
        }
    }
    delete prev;
    prev=NULL;
}


void Painter::changePoss(std::list<int> *tmp){
    for(QRect *rect : *endRect){
        delete prev;
        prev=new QRect(rect->x(),rect->y(), LINE-3, LINE-3);
        rePP(prev);
    }
    delete prev;
    prev=NULL;
    endRect->clear();

    for(std::list<int>::iterator i=tmp->begin(); i != tmp->end();++i){
        int xx = (*i-1)*LINE+2;
        i++;
        int yy = (*i-1)*LINE+2;
        if(xx<0 || yy<0) return;
        QRect *rect = new QRect(xx,yy,LINE-3, LINE-3);
        endRect->push_back(rect);
        rePP(rect);
    }
}


void Painter::clearAndUpdate(){
    clearSearch();
    repaint();
}
