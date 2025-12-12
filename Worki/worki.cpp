#include <bits/stdc++.h>
#include "worki.h"
using namespace std;

worek *biurko;
int n; //liczba worków
//wskaźniki na listę wszystich stworzonych elementów
lista *glowa, *ogon;

void ikea(){
        if (biurko == NULL){
                biurko = new worek();
                biurko->size = 0;
                biurko->numer = -1;
                biurko->father = NULL;
                biurko->atrapa = new worek();
                biurko->atrapa->father = biurko;
                //wrzucanie biurka i jego atrapy na listę
                ogon = new lista();
                glowa = new lista();
                glowa->type = 'w';
                ogon->type = 'w';
                glowa->w = biurko;
                ogon->w = biurko->atrapa;
                glowa->next = ogon;
        }
        return;
}

//Wkłada przedmiot do worka lub kładzie na biurku
void umiesc(przedmiot *co, worek *gdzie){
        co->father = gdzie->atrapa;
        gdzie->size++;
        return;
}

//Wkłada worek do worka lub kładzie na biurku
void umiesc(worek *co, worek *gdzie){
        co->father = gdzie->atrapa;
        gdzie->size += co->size;
        return;
}

// Nowy przedmiot na biurku
przedmiot *nowy_przedmiot(){
        ikea();
        przedmiot *nowy = new przedmiot();
        //wrzucamy przedmiod na listę elementów
        lista *temp = new lista();
        temp->type = 'p';
        temp->p = nowy;
        ogon->next = temp;
        ogon = temp;
        //kładziemy go na biurku
        umiesc(nowy, biurko);
        return nowy;
}

// Nowy worek na biurku; otrzymuje kolejny numer, począwszy od 0.
worek *nowy_worek(){
        ikea();
        worek *nowy = new worek();
        //wrzucamy worek na listę elementów
        lista *temp = new lista();
        temp->w = nowy;
        temp->type = 'w';
        ogon->next = temp;
        ogon = temp;
        //ustawiamy parametry nowego worka i kładziemy go na biurku
        nowy->size = 0;
        nowy->numer = n; ++n;
        umiesc(nowy, biurko);
        //tworzenie atrapy
        nowy->atrapa = new worek();
        nowy->atrapa->father = nowy;
        //wrzucanie atrapy na listę
        temp = new lista();
        temp->type = 'w';
        temp->w = nowy->atrapa;
        ogon->next = temp;
        ogon = temp;
        return nowy;
}

// Wkłada przedmiot co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(przedmiot *co, worek *gdzie){
        co->father = gdzie->atrapa;
        gdzie->size++;
        return;
}

// Wkłada worek co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(worek *co, worek *gdzie){
        co->father = gdzie->atrapa;
        gdzie->size += co->size;
        return;
}

// Wyjmuje przedmiot p z worka i kładzie na biurku.
// Założenie: Przedmiot p był w worku leżącym na biurku.
void wyjmij(przedmiot *p){
        p->father->father->size--;
        p->father = biurko->atrapa;
        return;
}

// Wyjmuje worek w z worka i kładzie na biurku.
// Założenie: Worek w był w worku leżącym na biurku.
void wyjmij(worek *w){
        w->father->father->size -= w->size;
        w->father = biurko->atrapa;
        return;
}

// Wynik: numer worka, w którym znajduje się przedmiot p (-1 jeśli na biurku).
int w_ktorym_worku(przedmiot *p){ return p->father->father->numer; }

// Wynik: numer worka, w którym znajduje się worek w (-1 jeśli na biurku).
int w_ktorym_worku(worek *w){ return w->father->father->numer; }

// Wynik: liczba przedmiotów zawartych (bezpośrednio i pośrednio) w worku w
int ile_przedmiotow(worek *w){ return w->size; }

// Cała zawartość worka w ląduje na biurku, a wszystko, co poza workiem w
// znajdowało się bezpośrednio na biurku, ląduje wewnątrz worka w.
void na_odwrot(worek *w){
        swap (w->atrapa, biurko->atrapa);
        swap (w->atrapa->father, biurko->atrapa->father);
        w->size = biurko->size - w->size;
        w->father = biurko->atrapa;
        return;
}

// Kończy i zwalnia pamięć
void gotowe(){
        do{
                ogon = glowa->next;
                if(glowa->type == 'w') delete glowa->w; //usuwanie worka
                else delete glowa->p; //usuwanie przedmiotu
                delete glowa;
                glowa = ogon;
        } while(ogon);
        return;
}
