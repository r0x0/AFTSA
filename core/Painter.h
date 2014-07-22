#ifndef PAINTER_H
#define PAINTER_H

#include <stdinc.h>
#include "core/BaseNod.h"
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>


/**
 * @brief The Painter class
 *Klasa obsługująca rysowanie w obszerze mapy.
 */
class Painter : public QWidget{

    Q_OBJECT

public:
    explicit Painter(QWidget *parent = 0);
    ~Painter(){
        delete color;
        delete pen;
        delete brush;
        delRM();

        delete endRect;
        delete visited;
        delete allvisited;
        delete lines;
        delete cAllV;
        delete cVisit;
        delete cStart;
        delete cEnd;
        delete cStart2;
        delete cEnd2;
        delete cVisit2;
        delete cAllV2;
        delete nullColor;
        delete cBlack;
        delete cGray;
        delete linePenn;
        delete lineTPen;
        delete linesTree;
    }

    
signals:
    void posChanged(int newX, int newY);
    void endPosChanged(int newX, int newY);
    void changeXY(int newX, int newY, int x, int y);
    

public slots:
    void pUp();
    void repaintField(BaseNod *nod, uchar nodState);
    void reproducPatch(BaseNod* endNod);
    void clearAndUpdate();
    void drawTreeLine(BaseNod *nod);

private:

    /**> Skala rysowania */
    int scale;

    /**> Ilość pól poziomo */
    int X;

    /**> Ilość pól pionowo */
    int Y;

    /**> Długość lini dla pola w pikselach */
    int LINE;

    /**> Połowa długości lini :D */
    int HLINE;

    /**> Prześwitywanie kolorów */
    unsigned char colorDiv;

    /**> Zaznaczanie kratki pod kursorem przy
     *wyborze punktu początkowego */
    bool startEve;

    /**> Zaznaczanie kratki pod kursorem przy
    *wyborze punktu końcowego */
    bool endEve;

    /**> Dla true pozwala na odmalowanie całej planszy */
    bool draw;

    /**> Dla true pozwala na odmalowanie planszy i zaznaczonych punktów */
    bool noSimulation;

    bool block;
    bool NoUpdate;
    bool NoSearchTree;

    /**> Ostatni stan odmalowanego wierzchołka przejścia */
    unsigned char lastState;

    /**> referencja do macierzy wartości wierzchołków,
     *używana do wyznaczania koloru pola */
    unsigned char **mapv;

    int prevX;

    int prevY;

    bool emitXY;

    bool saerchTree;

    bool reverseNod;

    bool drawSTree;

    bool drawSNods;

    /**> Podstawowy kolor */
    QColor *color;

    QColor *cStart;

    QColor *cEnd;

    QColor *cVisit;

    QColor *cAllV;

    QColor *cStart2;

    QColor *cEnd2;

    QColor *cVisit2;

    QColor *cAllV2;

    QColor *nullColor;

    QColor *cBlack;

    QColor *cGray;

    /**> Podstawowy pędzel (szerokość rysowania + kolor) */
    QPen *pen;

    QPen *linePenn;

    QPen *lineTPen;

    QLine *lineT;

    QLine *lineTP;

    /**> Podstawowe wypełnienie pola */
    QBrush *brush;

    /**> Macierz pól do rysowania */
    QRect ***rectMap;

    /**> Pole startu */
    QRect *start;

    /**> Pole pod kursorem przy wyborze punktu początkowego */
    QRect *start2;

    /**> Poprzednio zaznaczone pole */
    QRect *prev;

    /**> Pole zaznaczone do usunięcia dla wierzchołka początowego */
    QRect *delStart1;

    /**> Pole pod kursorem przy wyborze punktu końcowego */
    QRect *endR;

    /**> Lista wierzchołków końcowych */
    std::list<QRect*> *endRect;

    /**> Oznaczenie wierzchołka końcowego, do usunięcia */
    QRect *delEndRect;

    /**> Nowy wierzchołek przejścia */
    QRect *step;

    /**> Wierzchołek startu odwiedzony */
    QRect *specialstart;

    /** Wierzchołek końcowy odwiedzony */
    QRect *specialEnd;

    /**> Lista wierzchołków odwiedzonych do malowania */
    std::list<QRect*> *visited;

    /**> Lista wierzchołków, kórych wszyscy sąsiedzi zostali odwiedzeni */
    std::list<QRect*> *allvisited;

    /**> Linie przejść od punktu początkowego do końcowego */
    std::list<QLine*> *lines;

    /**> Linie przejść od punktu początkowego do końcowego */
    std::list<QLine*> *linesTree;






public:
    /**> Minimalna długość krawędzi kwadratu w pikselach */
    const int RECT_MIN_LENGTH =6;


private:

    /**
     * @brief paintEvent
     *??
     * @param event
     */
    void paintEvent(QPaintEvent *);

    /**
     * @brief mouseMoveEvent
     * ??
     *@param event
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mousePressEvent
     *??
     */
    void mousePressEvent(QMouseEvent *);

    /**
     * @brief mouseReleaseEvent
     *??
     */
    void mouseReleaseEvent(QMouseEvent *);

    /**
     * @brief rePP
     * Przerysowuje podany kwadrat.
     *@param rect
     *@param simu - mod symulacji
     */
    void rePP(QRect *rect, int simu=true);

    /**
     * @brief delRM
     *Usuwa macierz kwadratów rectMap i nie tylko.
     */
    void delRM();

    /**
     * @brief drawMap
     *Rysuje całą macierz rectMap kwadratów.
     *Używane w funkcji paintEvent(QPaintEvent *event).
     * @param painter - QPainter utworzony w zdarzeniu przerysowywania.
     */
    void drawMap(QPainter &painter);

    /**
     * @brief colorRect
     *Rysuje podany kwadrat według ustawień.
     *Używane w funkcji paintEvent(QPaintEvent *event).
     * @param painter - QPainter utworzony w zdarzeniu przerysowywania.
     * @param rect - QRect, który należy narysować
     * @param colorB - kolor wypełnienia (nie uwzględniane dla std==true)
     * @param colorP - kolor pędzla (nie uwzględniane dla std==true)bool emitXY;
     * @param std - jeżeli true: używa globalnyego pędzla i kolorów
     *jeżli false: szerokość pędzala = 2.
     */
    void inline colorRect(QPainter &painter, QRect *rect, QColor &colorB, QColor *colorP, bool std=false);

    void inline colorLine(QPainter &painter, QLine *line, bool tree);

    /**
     * @brief generateMap
     *Przegenruje rectMap z posiadanych danych bez odświeżania obrazu.
     *Wykorzystywane przy zmianie skali.
     */
    void generateMap();

    /**
     * @brief resizeRect
     *Zmienia skale podanego kwadratu według nowego prametru scale.
     * @param rect
     * @param scale
     */
    void resizeRect(QRect *rect, int scale);

public:

    void generateMap(unsigned char **mapv, int x, int y);
    void setScale(int scale);
    void setStartEve(bool arg){ startEve=arg; }
    void setEndEve(bool arg){ endEve=arg; }
    void fixMap();
    void changePos(int x, int y, bool del=false);
    void changePoss(std::list<int> *tmp);
    void clearSearch();
    void resizeLine(QLine *line, int scl);
    void setemitXY(bool b){ emitXY=b; }
//    void setSearchTree(bool search){ this->saerchTree=search; }
    void setBlock(bool b){ block = b; }
    void setNoUpdate(bool b){ NoUpdate=b; }
    void setNoSearchTree(bool b){ NoSearchTree=b; }
    void setReverseNod(bool reverseNod){ this->reverseNod=reverseNod; }
    void setDrawSTree(bool drawSTree){ this->drawSTree=drawSTree; }
    void setDrawSNod(bool drawSNods){ this->drawSNods=drawSNods; }
};

#endif // PAINTER_H
