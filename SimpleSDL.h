#ifndef SIMPLESDL_H
#define SIMPLESDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
struct planet{
	int x;		//X coord
	int y; 		//Y coord
	float xs;		//X speed
	float xa;	//X accelleration
	float ys;		//Y speed
	float ya;	//Y accelleration
	int r;		//Radius
	int mass;	//Mass		(Multiply the radius for a constant you define)
};

const float G = 10;			//Use as universal gravitational constant
const float DT = 1;			//Delta time i use in moviment equation
const float SPD_RED = 1;	//Speed reductionc onstant when the planet it's near to the BH

//Planets data
int const PLAN_NUM = 8;			//Max numbers of planets on the map at the same time
int const PLAN_MAX_R = 20;		//Max radius of each plan
int const PLAN_DEN = 2;			//Planet density					(CALIBRATE)


//Screen coordinates
const int SCREEN_X=800;
const int SCREEN_Y=600;

/**
 * Classe che wrappa alcune funzionalit√† della libreria [SDL2](http://www.libsdl.org).
 *
 * Utilizzando SimpleSDL √® possibile utilizzare le funzionalit√† grafiche
 * (gestione colori, primitive base, font) e catturare i principali eventi
 * (mouse,tastiera) presenti nella libreria SDL2.
 *
 * @author Marco Vassura
 * @version 1.1
 */
class SimpleSDL {

protected:
    int screen_w;
    int screen_h;
    SDL_Window* wnd;
    SDL_Renderer* rnd;
    SDL_Event event;
    bool hasEvent;
    std::map<std::string,SDL_Color> colors;
    std::map<std::string,SDL_Texture*> images;

    TTF_Font* font;
    int fontSize;

public:
	
	

    /**
     * Inizializza le componenti SDL e crea una finestra grafica.
     *
     * Questo costruttore effettua le operazioni di inizializzazione e creazione della finestra grafica nel
     * seguente ordine:
     * - inizializza sottosistema video di SDL;
     * - crea la finestra grafica;
     * - imposta i colori predefiniti "bg" e "fg" rispettivamente a nero e bianco;
     * - inizializza sottosistema truetype di SDL;
     * - imposta il font a DejaVuSansMono e la dimensione a 12 punti;
     * - imposta il colore iniziale a "fg".
     * - imposta lo sfondo dello schermo al colore "bg".
     *
     * @param title il titolo della finestra.
     * @param x la coordinata x in pixel dell'angolo in alto a sinistra della finestra.
     * @param y la coordinata y in pixel dell'angolo in alto a sinistra della finestra.
     * @param w la larghezza in pixel della finestra.
     * @param h l'altezza in in pixel della finestra.
     * @exception SimpleSDLException se non √® possibile creare la finestra grafica.
     */
    SimpleSDL(const char* title, int x, int y, int w, int h);

    /**
     * Inizializza le componenti SDL e crea una finestra grafica con origine (0,0).
     *
     * @param title il titolo della finestra.
     * @param w la larghezza in pixel della finestra.
     * @param h l'altezza in in pixel della finestra.
     * @exception SimpleSDLException se non √® possibile creare la finestra grafica.
     */
    SimpleSDL(const char* title, int w, int h): SimpleSDL(title,-1,-1,w,h) {}

    /**
     * Definisce un colore.
     *
     * Permette di associare ad un nome un colore con le componenti r,g,b.
     * successivamente √® possibile utilizzare il nome come riferimento.
     * Nel caso esista gi√† un colore con lo stesso nome sar√† sostituito.
     *
     * @param name il nome del colore.
     * @param r il byte con la componente rosso del colore.
     * @param g il byte con la componente verde del colore.
     * @param b il byte con la componente blue del colore.
     */
    int setColor(const std::string name,Uint8 r,Uint8 g,Uint8 b);

    /**
     * Imposta il colore corrente.
     *
     * Permette di definire quale colore sar√† utilizzato per il disegno delle
     * successive primitive grafiche.
     *
     * @param name il nome del colore.
     * @return 0 se il colore esiste, 1 se il colore non esiste.
     */
    int useColor(const std::string name);

    /**
     * Imposta il colore corrente al colore di sfondo.
     */
    void useColorBg()
    {
        useColor("bg");
    }

    /**
     * Imposta il colore corrente al colore base.
     */
    void useColorFg()
    {
        useColor("fg");
    }

    /**
     * Cancella lo schermo.
     *
     * @return 0 se Ë tutto andato bene, <0 se qualcosa Ë andato storto.
     */
    int clear();

    /**
     * Aggiorna la finestra grafica con gli oggetti disegnati dall'ultimo redraw.
     */
    void redraw();

    /**
	 * Disegna punto.
	 *
	 * Permette di disegnare un punto sullo schermo indicando le sue coordinate.
	 * 
	 * @param x la coordinata delle x del punto.
	 * @param y la coordinata delle y del punto.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 */
    int drawPoint(int x, int y);
    
	/**
	 * Disegna un segmento.
	 *
	 * Permette di disegnare un segmento indicando le coordinate di partenza dell'origine
	 * e le coordinate di arrivo del segmento.
	 *
	 * @param x1 la coordinata x dell'origine.
	 * @param y1 la coordinata y dell'origine.
	 * @param x2 la coordinata x del punto d'arrivo.
     * @param y2 la coordinata y del punto d'arrivo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore. 
	 */
    int drawLine(int x1, int y1, int x2, int y2);
    
	/**
	 * Disegna rettangolo .
	 * 
	 * Disegna un rettangolo disegnando delle linee che collegano i 4
	 * vertici, che ha coma vertice in altro a sinistra delle coordinate sullo schermo
	 * e come altezza e larghezza un numero di pixel inseriti.
	 *
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 */
    int drawRect(int x1, int y1, int w, int h);
    
    /**
	 * Disegna ovale.
	 * Disegna un ovale inscritto in un rettangolo del quale Ë necessario
	 * definire la coordinate del vertice in alto a sinistra, la larghezza e l'altezza.
	 * 
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */    
	int drawOval(int x1, int y1, int w, int h);

	/**
	 * Disegna rettangolo colorato.
	 *
	 * Disegna un rettangolo che viene completamente colorato internamente
	 * con il colore selezionato al momento del call della funzione
	 * e applica lo stesso funzionamento di drawRect ovvero inserendo coordinate di 
	 * un vertice e aggiungendo poi la larghezza e l'altezza.
	 *
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 */    
	int fillRect(int x1, int y1, int w, int h);
    
    /**
	 * Disegna un ovale colorato 
	 *
	 * Disegna un ovale che viene completamente colorato internamente
	 * con il colore selezionato al momento del call della funzione
	 * e applica lo stesso funzionamento di drawOval ovvero inserendo coordinate di 
	 * un vertice e aggiungendo poi la larghezza e l'altezza del rettangolo all'interno del quale 
	 * verr‡ disegnato l'ovale.
	 *
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
	int fillOval(int x1, int y1, int w, int h);
	
	/**
	 * Verifica l'avvenimento di un evento di input di qualsiasi tipo.
	 *
	 * @return 1 nel caso in cui sia accaduto un evento.
	 * @return 0 nel caso in cui non sia accaduto un evento.
	 */
    bool checkEvent();

	/**
	 * Verifica che l'azione compiuta sia un movimento del mouse.
	 *
	 * @return TRUE se c'Ë stato un movimento del mouse, FALSE se non c'Ë stato.
	 */
    bool isMouseMotionEvent();
    
	/**
	 * Verifica che l'azione compiuta sia un click del mouse.
	 *
	 * @return TRUE se c'e stato un click del mouse, FALSE se non c'Ë stato.
	 */
	bool isMouseButtonEvent();
	
	/**
	 * verifica che l'azione sia la pressione di un tasto.
	 *
	 * @return TRUE se c'Ë stato un'abbassamento del tasto, FALSE se non c'Ë stato.
	 */
    bool isKeyEvent();

  	/**
  	 * Cattura la coordinata X del click del mouse.
  	 *
  	 * @return la coordinata X del punto in cui Ë stato cliccato il mouse, -1 se non c'Ë stato nessun evento o se non era un click del mouse.
  	 */
    Sint32 getMouseX();
    
 	/**
  	 * Cattura la coordinata Y del click del mouse.
  	 *
  	 * @return la coordinata Y del punto in cui Ë stato cliccato il mouse, -1 se non c'Ë stato nessun evento o se non era un click del mouse.
  	 */
    Sint32 getMouseY();
    
	/**
	 * Indica quale tasto del mouse Ë stato cliccato.
	 *
	 * @return 1 se era il tasto sinistro.
	 * @return 2 se era il tasto centrale.
	 * @return 3 se era il tasto destro.
	 * @return 0 se non era nË destro, nË sinistro, nË centrale.
	 * @return -1 se non c'Ë stato nessun click del mouse.
	 */
	int getMouseButton();
	
	/**
	* Conta il numero di click del mouse.
	*
	* @return il numero di click del mouse.
	* @return 0 se non ci sono stati click del mouse.
	*/
    int getMouseButtonCount();

	/**
	 * Restituisce il tasto premuto dall'utente.
	 *
	 * @return SDLK_a se il tasto premuto altrimenti dopo SDLK_ c'Ë il tasto premuto (https://wiki.libsdl.org/SDL_Keycode)
	 * @return 0 se non Ë stato premuto nessun tasto.
	 */
    SDL_Keycode getKeyCode();

	/**
	 * Restituisce la larghezza dello schermo.
	 *
	 * @return la larghezza dello schermo in pixel.
	 *
	 */
    int width()
    {
        return screen_w;
    }
   
    /**
	 * Restituisce l'altezza dello schermo.
	 *
	 * @return restituisce l'altezza dello schermo in altezza.
	 *
	 */   
    int height()
    {
        return screen_h;
    }

	/**
	 * Imposta delay.
	 * 
	 * Blocca il programma per una durata di millisecondi e poi lo fa riprendere.
	 *
	 * @param ms il numero di millesecondi per il quale il programma si deve mettere in pausa.
	 * 
	 */
    void delay(int ms);

	/**
	 * Svuota buffer input.
	 * 
	 * Svuota il buffer che registra il movimento della rotella del mouse
	 * e quello che registra i tasti premuti.
	 *
	 */
	void flushEvents();

	/**
	 * Stampa stringa in un punto dello schermo con una grandezza del carattere definito.
	 *
	 * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare e definendo la grandezza del carattere.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param std::string msg la stringa che deve essere stampata .
	 * @param size la grandezza del carattere del testo.
	 *
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 * 
	 */
    int text(int x, int y, const std::string msg, int size);
    
    /**
     * Stampa stringa in un punto dello schermo.
     *
     * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param std::string msg la stringa che deve essere stampata .
	 *
	 * @return -1 nel caso in cui non ci sia nessun font selezionato.
	 * @return 0 se Ë andato tutto bene.
	 *
     */
    int text(int x, int y, const std::string msg);
    
    /**
     * Stampa un a stringa di caratteri di grandezza definita al centro dello schermo.
     *
     * @param std::string msg indica la stringa che verr‡ stampata.
     * @param size indica la grandezza dei caratteri.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const std::string msg, int size);
    
    /**
     * Stampa una stringa nel centro dello schermo.
     *
     * @param std::string msg indica la stringa da stampare.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const std::string msg);

	/**
	 * Stampa stringa in un punto dello schermo con una grandezza del carattere definito.
	 *
	 * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare e definendo la grandezza del carattere.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param std::string msg la stringa che deve essere stampata .
	 * @param size la grandezza del carattere del testo.
	 *
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore. 
	 */
    int text(int x, int y, const char* msg, int size);
    
	/**
     * Stampa stringa in un punto dello schermo.
     *
     * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param msg il messaggio che deve essere stampato.
	 *
	 * @return -1 nel caso in cui non ci sia nessun font selezionato, 0 se tutto Ë andato bene.
     */
    int text(int x, int y, const char* msg);
    
    /**
     * Stampa un a stringa di caratteri di grandezza definita al centro dello schermo.
     *
     * @param msg il messaggio che deve essere stampato.
     * @param size indica la grandezza dei caratteri.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const char* msg, int size);
    
    /**
     * Stampa una stringa nel centro dello schermo.
     *
     * @param msg il messaggio che deve essere stampato.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const char* msg);

	/**
	 * Per indicare la grandezza di default del font.
	 *
	 * @param size indica la grandezza del font.
	 *
	 * @return -1 la grandezza del font Ë <=0 o >della grandezza dello schermo.
	 */
    int textSize(int size);
    
    /**
     * Restituisce la grandezza del font di default al momento.
     *
     * @return grandezza del font del testo
     */
    int getTextSize();

    int setImage(const std::string name, const std::string filename);
    
	int drawImage(int x, int y, const std::string name);
	
	/**
	 * Disegna una linea con altezza.
	 *
	 * Devo inserire le coordinate x e y del punto di partenza e di arrivo e l'altezza della riga che voglio disegnare.
	 *
	 * @param x1 Indica la coordinata x del punto di partenza del centro della linea.
	 * @param y1 Indica la coordinata y del punto di partenza del centro della linea.
	 * @param x2 Indica la coordinata x del punto di arrivo del centro della linea.
	 * @param y2 Indica la coordinata y del punto di arrivo del centro della linea.
	 * @param h Indica l'altezza della linea.
	 *
	 * @return 0 Se tutto Ë andato bene.
	 * @return -1 Se lo spessore della linea Ë <0.
	 */	  
	int drawHLine(int x1, int y1, int x2, int y2, int h);
	
	/**
	 * Disegna una linea largha.
	 *
	 * @param x1 Indica la coordinata x del punto di partenza del centro della linea.
	 * @param y1 Indica la coordinata y del punto di partenza del centro della linea.
	 * @param x2 Indica la coordinata x del punto di arrivo del centro della linea.
	 * @param y2 Indica la coordinata y del punto di arrivo del centro della linea.
	 * @param w indica la larghezza della linea.
	 *
	 * @return 0 Se tutto Ë andato bene.
	 * @return -1 Se la larghezza della linea Ë <0.
	 */	  
	int drawWLine(int x1, int y1, int x2, int y2, int w);
	
	/** 
	 * Disegna una semi ellisse aperta.
	 *
	 * @param x1 La coordinata x del punto pi˘ a sinistra della semiellisse.
	 * @param y1 La coordinata y della base della semiellisse.
	 * @param w La lunghezza della base dell'ellisse.
	 * @param h Á'altezza dell'ellisse.
	 * @param bgcolor Bisogna inserire il colore che si sta usando in questo momento come sfondo.
	 * @param fgcol Bisogna inserire il colore che si ha selezionato ora come foreground.
	 *
	 * @return 0 se tutto Ë andato bene, 1 se il colore non esiste.
	 */
	int drawHalfOval(int x1, int y1, int w, int h, const char* bgcol, const char* fgcol);

	/**
	 * Disegna una semi ellisse con l'interno colorato.
	 *
	 * @param x1 La coordinata x del punto pi˘ a sinistra della semiellisse.
	 * @param y1 La coordinata y della base della semiellisse.
	 * @param w La lunghezza della base dell'ellisse.
	 * @param h Á'altezza dell'ellisse.
	 * @param bgcolor Bisogna inserire il colore che si sta usando in questo momento come sfondo.
	 * @param fgcol Bisogna inserire il colore che si ha selezionato ora come foreground.
	 *
	 * @return 0 se tutto Ë andato bene, 1 se il colore non esiste.
	 */
	int fillHalfOval(int x1, int y1, int w, int h, const char* bgcol, const char* fgcol);
	
	/**
	 * Disegna un triangolo.
	 *
	 * @param x1 Indica la coordinata x del primo vertice.
	 * @param y1 Indica la coordinata y del primo vertice.
	 * @param x2 Indica la coordinata x del secondo vertice.
	 * @param y1 Indica la coordinata y del secondo vertice.
	 * @param x3 Indica la coordinata x del terzo vertice.
	 * @param y3 Indica la coordinata y del terzo vertice.
	 *
	 * @retun 0 Se Ë andato tutto bene, !=0 se Ë qualcosa Ë andato storto.
	 */
	int drawTrian(int x1, int y1, int x2, int y2, int x3, int y3);
	
	/**
	 * Restituisce il tasto premuto dall'utente.
	 *
	 * @param key Modifica key inserendo SDLK_<tasto premuto>.
	 * 
	 * @return SDLK_a se il tasto premuto altrimenti dopo SDLK_ c'Ë il tasto premuto (https://wiki.libsdl.org/SDL_Keycode)
	 * @return 0 se non Ë stato premuto nessun tasto.
	 */
	int getKeyCode(int &key);
	
	/**
	 * Restituisce il numero arrotondato alla seconda cifra decimale.
	 *
	 * @param f Il numero da arrotondare.
	 *
	 * @return il numero arrotondato.
	 */
	float roundFloat(float f);
	
	/**
 	 * Permette di disegnare un pianeta.
 	 *
 	 * @param a Il pianeta da disegnare.
 
	 * @return -1 Nel caso in cui il raggio sia <=0.
	 * @return 0  Se Ë andato tutto bene.
	 */
	int drawPlanet(planet a);
	
	/**
	 * Ritorna la distanza su due punti in un piano.
	 *
	 * @param ax La coordinata x del punto A.
	 * @param ay La coordinata y del punto A.
	 * @param bx La coordinata x del punto B.
	 * @param by La coordinata y del punto B.
	 *
	 * @return La distanza fra i due punti.˘
	 */
	int pointDist(int ax, int ay, int bx, int by);
	
	/** 
	 * Calcola la forrza di gravit‡ in Newton secondo la costante G universale.
	 *
	 * @param a Il primo pianeta
	 * @param b il secondo pianeta
	 *
	 * @return I newtown di forza arrotondati al secondo decimale.
	 */
	float gravityStrnght(planet a, planet bh);
	
	/**
	 * Calcola l'angolazione per la distrubuzione delle forze di atraione dei pianeti.
	 *
	 * @param a Il primo pianeta.
	 * @param bh Il secondo pianeta.
	 *
	 * @return a L'angolo in gradi sessaggesimali.
	 */
	float planetAngle(planet a, planet bh);
	
	/**
	 * Verifica se due pianeti si toccano e se quindi hanno almeno un punto in comune.
	 *
	 * @param a Il primo pianeta.
	 * @param bh Il secondo pianeta.
	 *
	 * @return true Se i due pianeti si toccano.
	 * @return false Se i due pianeti non si toccano.
	 */
	bool planetTouch(planet a, planet bh);
	
	/**
	 * Ritorna l'accellerazione sull'asse x del pianeta a attratto da bh
	 *
	 * @param a il pianeta che viene attratto dal buco nero.
	 * @param bh Il buco nero che attrae il pianeta.
	 *
	 * @return Il valore in m/s^2 dell'attrazione gravitazionale.
	 */
	float setAccX(planet a, planet bh);

	/**
	 * Ritorna l'accellerazione sull'asse y del pianeta attratto da bh.
	 *
	 * @param a il pianeta che viene attratto dal buco nero.
	 * @param bh Il buco nero che attrae il pianeta.
	 *
	 * @return Il valore in m/s^2 dell'attrazione gravitazionale.
	 */	
	float setAccY(planet a, planet bh);
	
	/**
	 * Costruisce un pianeta esterno allo schermo.
	 * Genera un numero casuale compreso fra 0 e 3 e in base al risulato decide se costruirlo a sinistra, sopra, a sinistra o sotto lo schermo, 
	 * genera anche poi un raggio casuale minore della costante reggio sopra indicata e aggiorna la massa secondo la costante indicata sopra moltiplicando 
	 * il raggio per la costante di densit‡ dei pianeti prima specificata.
	 *
	 * @param a Il pianeta al quale devo modificare i dati.
	 */
	void buildPlan(planet* a);							//Check if works!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	void updateSpd(planet* a);							//Check if works!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	void updateCoord(planet* a);						//Check if works!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	/**
	 * It allplyes an equation to reduce the speed when it gets near to the atmosphere of the black hole, using the SPD_RED.
	 * It multiply the speed for a value which is found by this equation x=SPD_RED*distance(planet, bh)^2.
	 *
	 * @param a The planet you want to reduce the speed of.
	 * @param bh The black hole, where the planet is running to.
	 */
	void bhAtmosp(planet* a, planet bh );


    ~SimpleSDL();
};

#endif // SIMPLESDL_H
