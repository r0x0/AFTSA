#ifndef SALG_H
#define SALG_H

#include "stdinc.h"
#include "core/BaseNod.h"
#include "core/BaseMap.h"
#include "core/MinHeap.h"
#include <QThread>
#include <QObject>


/**
 * @brief Klasa BaseAlg.
 * Klasa bazowa dla wszystkich algorytmów wyszukiwania.
 * Zapewnia wykonanie algorytmu oraz wszystkich niezbędnych operacji
 * przed i po.
 */
class BaseAlg : public QThread{

    Q_OBJECT

public:
    //  *****************************************************************
    //  **                     Stałe
    //  *****************************************************************

    /**> Algorytm zakończony bez wyniku */
    static const char ST_NOT_FOUND = 0;

    /**> Znaleziono rozwiązanie */
    static const char ST_FOUND = 1;

    /**> Algorytm został przerwany z innego powodu niż przez użytkownika */
    static const char ST_BREAKED = -1;

    /**> Algorytm nie został jeszcze uruchomiony */
    static const char ST_NOT_STARTED = -2;

    /**> Heurystyka odległości Manhattan */
    static const char HEURISTIC_MANHATTAN = 0;

    /**> Heurystyka odległości Euklidesowej */
    static const char HEURISTIC_EUKLIDES = 1;

    /**> Heurystyka odległości Manhattam */
    static const char INTERPRET_VALUE = 0;

    /**> Heurystyka odległości Euklidesowej */
    static const char INTERPRET_DIFF = 1;

    /**> Kolejka wierzchołków do rozpatrzenia jako lista */
    static const char QUEUE_LIST = 0;

    /**> Kolejka wierzchołków do rozpatrzenia jako kopiec */
    static const char QUEUE_HEAP = 1;

    /**> Pierwiastek z 2 */
    static const double SQRT_TWO;

    /**> Precyzja zaokrąglania */
    static const double ROUND;

    /**> Maksymalna wartość double */
    static const double MAX_DOUBLE;

protected:
    //  *****************************************************************
    //  **                     Zmienne
    //  *****************************************************************

    /**> Ustawienie true spowoduje nulowanie działania algorytmu. */
    bool stop;

    /**> Ustawienie true spowoduje wstrzymanie działania algorytmu. */
    bool pause;

    /**> Ustawienie true spowoduje zezwolenie na wykonanie jednej iteracji jeżeli algorytm jest wstrzymany. */
    bool step;

    /**> Wybierz operator losowo. */
    bool randPriori;

    /**> Realny czas działania algorytmu. */
    long rtime;

    /**> Ilość iteracji algorytmu. */
    long ctime;

    /**> Suma czasów iteracji. */
    long ttime;

    /**> False wyłącza zbieranie większości danych. */
    bool noDataCollect;

    /**> Opóźnienie działania algorytmu. */
    long delay;

    /**> Ilość wierzchołków w znalezionej ścieżce. */
    long countPatchNod;

    /**> Maksymalnie wierzchołków w kolejce open. */
    unsigned long maxOpenNod;

    /**> Odwiedzone wierzchołki. */
    long countVisited;

    /**> Waga znalezionej ścieżki. */
    double patchCost;

    /**> Ilość przeszukań listy open. */
    long countSearchOpen;

    /**> Ilość wywołani rekurencyjnych. */
    long countRecursion;

    /**> Ilość przetworzeń listy sąsiadów. */
    long countNodList;

    /**> Kolejka wierzchołków otwartych. */
    std::list<BaseNod*> *open;

    /**> Kolejka wierzchołków końcowych. */
    std::list<BaseNod*> *endlist;

    /**> Ostatnio rozpatrywany wierzchołek. */
    BaseNod *prev;

    /**> Obecnie rozpatrywany wierzchołek. */
    BaseNod *current;

    /**> Punkt rozpoczynający, root. */
    BaseNod *startPoint;

    /**> Referencja do obiektu mapy. */
    BaseMap *bmap;

    /**> Przechowuje ostatni komunikat. */
    std::string message;

    /**> Status wykonania algorytmu. */
    int status;

    /**> Lista priorytetu wyboru operatora. */
    int *priorityList;

    /**> Obiekt pozwalający na odmierzanie czasu. */
    QElapsedTimer *timer;

    /**> Obiekt zegara. */
    QTime time;

    /**> Maksymalna głębokość dla algorytmu DFSP. */
    int maxDepth;

    /**> Krok pogłębiania dla algorytmu DFSP. */
    int depthStep;

    /**> Mnożnik dla funkcji heurystycznej h(n). */
    double heuristicScale;

    /**> Rodzaj używanej heurystyki. */
    char heuristic;

    /**> Interpretacja wartości wierzchołka. */
    char interpretation;

    /**> Typ kolejki wierzchołków do rozpatrzenia*/
    char queueType;

    /**> Lista dla kopca typu min, wierzchołków do rozpatrzenia */
    std::vector<BaseNod*> *listHeap;

public:
    BaseAlg(QObject *parent=0);

    virtual ~BaseAlg(){
        delete[] priorityList;
        delete timer;
        delete open;
    }

signals:
    //  *****************************************************************
    //  **                     Sygnały
    //  *****************************************************************

    /**
     * @brief endTask
     *Sygnał informuję główny wątek aplikacji o zakończeniu funkcji algorytmu.
     */
    void endTask();

    /**
     * @brief repaintField
     *Sygnał do przsemalowania pola (x,y) o statusie nodState.
     * @param nod
     */
    void repaintField(BaseNod* nod, uchar state=0);

    /**
     * @brief reproducPatch
     *Sygnał do narysowania znalezionej ścieżki, której punktem końcowym jest BaseNod *end.
     * @param end
     */
    void reproducPatch(BaseNod* end);

    /**
     * @brief drawTreeLine
     *Sygnał do narysowanie linii przejścia pomiędzy wierzchołkami.
     * @param nod
     */
    void drawTreeLine(BaseNod *nod);

protected:
    //  *****************************************************************
    //  **                     Funkcje
    //  *****************************************************************

    /**
     * @brief update
     *
     *Zbiera informacje i wysyła sygnał do przemalowania
     *odpowiednich pól na mapie. Funkcja powinna zostać uruchomiona w Search po
     *każdej iteracji algorytmu.
     * @param nod
     */
    inline void update(BaseNod *nod){
        ctime++;
        ttime+=timer->nsecsElapsed()/1000;

        // Zbieranie danych o odwiedzonych wierzchołkach
        if(noDataCollect){
            if(nod->state==BaseNod::VISITED) emit drawTreeLine(nod);
            emit repaintField(nod);
            if(nod==startPoint){
                emit repaintField(nod, BaseNod::SPECSTART);
            }
            else if(nod->endNode){
                emit repaintField(nod, BaseNod::SPECEND);
            }

        }
        unsigned int osize = getOpenSize();
        if(maxOpenNod < osize) maxOpenNod = osize;
        if(nod->state==BaseNod::VISITED) countVisited++;

        // Obsługa procesu
        if(pause){
            while(true){
                if(!pause || step) break;
                this->msleep(100);
            }
            step=false;
        }
        if(stop){
            this->terminate();
            emit endTask();
        }

        if(delay>0) this->msleep(delay);
        timer->start();
    }

    /**
     * @brief suffle
     *Losowo przestawia wartości int w tablicy *p.
     * @param p tablica int[max]
     * @param max
     */
    inline void suffle(int *p, int max){
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        int *d = new int[max];
        int *w = new int[max];
        for(int i =max-1 ; i>=0;i--){
            d[i]=i;
            w[i] = qrand() % ((i + 1) - 0) + 0;
        }
        for(int i =max-1 ; i>=0;i--){
            int *s=new int[i];
            p[i]=d[w[i]];
            int k=0;
            for(int j=0;j<=i;j++){
                if(j!=w[i]){
                    s[k]=d[j];
                    k++;
                }
            }
            delete d;
            d=s;
        }
    }

    /**
     * @brief checkContains
     *Sprawdza czy podany wierzchołek istnieje juz w liście wierzchołków otwartych.
     * @param nod - wierzchołek do sprawdzenia
     * @return
     */
    inline bool checkContains(BaseNod *nod){
        for(std::list<BaseNod*>::const_iterator i = open->begin(); i!=open->end(); ++i){
            BaseNod *nodd = *i;
            countSearchOpen++;
            if(nodd->x==nod->x && nodd->y==nod->y) return true;
        }
        return false;
    }

    /**
     * @brief getMoveCost
     *Oblicza koszt przejścia z pr do wierzchołka o indeksie i naliście sąsiedztwa pr.
     * @param pr - wierzchołek wyjścia
     * @param i - indeks wierzchołka do obliczenia kosztu przejścia.
     * @return
     */
    inline double getMoveCost(BaseNod *pr, int i){
        BaseNod *nod = pr->nodList[i];
        double ret;
        ret = nod->value;
        if(interpretation==INTERPRET_DIFF){
            ret -= pr->value;
            if(ret<0) ret*=-1;
            ret++;
        }
        if(pr->MAX_NODS==8 && (i==1 || i==3 || i==5 || i==7)){
            ret= ret*SQRT_TWO;
        }
        return ret;
    }

    /**
     * @brief AlgDijkstra::getMin
     *Wybiera element o najmniejszej wartości heuristicValue z listy open.
     * @return
     */
    inline BaseNod* getMin(){
        double min = MAX_DOUBLE;
        BaseNod *ret=NULL;
        for(std::list<BaseNod*>::const_iterator i = open->begin(); i!=open->end(); ++i){
            BaseNod *nod = *i;
            if(min > nod->heuristicValue){
                min=nod->heuristicValue;
                ret=nod;
            }
            countSearchOpen++;
        }
        open->remove(ret);
        return ret;
    }

    /**
     * @brief getMinFromOpen
     *Pobierz i usuń z jednej z kolejek (według ustawień) najmniejszy element.
     * @return
     */
    inline BaseNod* getMinFromOpen(){
        if(queueType==QUEUE_HEAP){
            return MinHeap::HEAP_EXTRACT_MIN(listHeap, countSearchOpen);
        }else return getMin();
    }

    /**
     * @brief getOpenSize
     *Zwraca ilość elementów kolejki open.
     * @return
     */
    inline unsigned int getOpenSize(){
        if(queueType==QUEUE_HEAP){
            return listHeap->size();
        }else return open->size();
    }

    /**
     * @brief inserNodToList
     *Wstaw element do używanej listy open->
     * @param nod
     */
    inline void inserNodToList(BaseNod *nod){
        if(queueType==QUEUE_HEAP){
            MinHeap::MIN_HEAP_INSERT(listHeap, nod, countSearchOpen);
        }else{
            if(nod->MAX_NODS>4 || interpretation==INTERPRET_DIFF){
                if(checkContains(nod)){
                    return;
                }
            }
            open->push_back(nod);
        }
    }

    /**
     * @brief checkIsOpenEmpty
     *Sprawdź, czy używana lista nie jest pusta.
     * @return
     */
    inline bool checkIsOpenEmpty(){
        if(queueType==QUEUE_HEAP){
            return !listHeap->empty();
        }else{
            return !open->empty();
        }
    }

    /**
     * @brief clean
     *Inicjuje dane.
     */
    void clean();

private:
    /**
     * @brief run
     *Metoda wątku, zwierająca niezbędne funkcje do wykonania algorytmu.
     *wywoływana przez metodę QThread.start().
     */
    void run();

    /**
     * @brief Search
     * Funkcja algorytmu, która powinna zostać nadpisana i
     *zawierać implementacje algorytmu oraz wywołanie funkcji
     *update() po każdej iteracji;
     * @return
     */
    virtual void Search()=0;

public:

    //  *****************************************************************
    //  **                     GETTERS
    //  *****************************************************************

    long getRtime(){ return rtime; }

    long getCtime(){ return ctime; }

    long getTtime(){ return ttime; }

    long getCountPatchNod(){ return countPatchNod; }

    unsigned long getMaxOpenNod(){ return maxOpenNod; }

    long getCountVisited(){ return countVisited; }

    double getPatchCost(){ return patchCost; }

    long getCountRecursion(){ return countRecursion; }

    long getCountSearchOpen(){ return countSearchOpen; }

    long getCountNodList(){ return countNodList; }

    long getDelay(){ return delay; }

    bool getStop(){ return stop; }

    std::string getMessage() { return message; }


    //  *****************************************************************
    //  **                     SETTERS
    //  *****************************************************************

    void setDelay(long delay){ this->delay=delay; }

    void setStop(bool stop){ this->stop=stop; }

    void setPause(bool pause){ this->pause=pause; }

    void setStep(bool step){ this->step=step; }

    void setStart(BaseNod *start){ this->startPoint=start; }

    void setPriorityList(int *list){
        delete[] priorityList;
        priorityList=list;
    }

    void setNoCollectData( bool noData){ noDataCollect=noData; }

    void setRandPrioi(bool randP){ randPriori=randP; }

    void setMaxDepth(int max){ maxDepth=max; }

    void setDepthStep(int step){ depthStep=step; }

    void setMap(BaseMap *bMap){ bmap=bMap; }

    void setHeuristicScale(double scale){ heuristicScale=scale; }

    void setHeuristic(char h){ heuristic=h; }

    void setInterpretation(char i){ interpretation=i; }

    void setEndList(std::list<BaseNod*> *list){ endlist=list; }

    void setQueueType(char i){ queueType=i; }
};

#endif // SALG_H
