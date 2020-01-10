#include "SimpleSDL.h"
#include "SimpleSDLException.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;




SimpleSDL::SimpleSDL(const char* title, int x, int y, int w, int h)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw SimpleSDLException(SDL_GetError());
    }
 	if (x==-1 && y==-1) {
    	SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
    		x=10;
    		y=10;
    	} else {
    		x = (dm.w-w)/2;
    		y = (dm.h-h)/2;
		}
	}

    atexit(SDL_Quit);

    wnd = SDL_CreateWindow(title, x, y, w, h, 0);
    if (wnd == NULL)
    {
        throw SimpleSDLException(SDL_GetError());
    }
    SDL_GetWindowSize(wnd,&screen_w,&screen_h);
    rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    if (rnd == NULL)
    {
        throw SimpleSDLException(SDL_GetError());
    }

    setColor("bg",0,0,0);
    setColor("fg",255,255,255);
    useColor("fg");

    if (TTF_Init()!=0)
    {
        throw SimpleSDLException(TTF_GetError());
    }
    font = NULL;
    fontSize = 12;
    textSize(fontSize);
    if (font == NULL)
    {
        throw SimpleSDLException(TTF_GetError());
    }

    hasEvent = false;

    clear();
    redraw();
}


int SimpleSDL::clear()
{
    return SDL_RenderClear(rnd);
}

void SimpleSDL::redraw()
{
    SDL_RenderPresent(rnd);
}

int SimpleSDL::setColor(const std::string name, Uint8 r,Uint8 g,Uint8 b)
{
    try
    {
        SDL_Color c = {r,g,b,SDL_ALPHA_OPAQUE};
        colors[name] = c;
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
    return 0;
}

int SimpleSDL::useColor(const std::string name)
{
    try
    {
        SDL_Color c = colors.at(name);
        return SDL_SetRenderDrawColor(rnd,c.r, c.g, c.b, c.a);
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
    return 0;
}



	
int SimpleSDL::drawLine(int x1, int y1, int x2, int y2)                                                        
{																											   
    return SDL_RenderDrawLine(rnd,x1,y1,x2,y2);
					 
}			  																						  
																											 
int SimpleSDL::drawRect(int x1, int y1, int w, int h)														  
{																											   
    SDL_Rect r = {x1,y1,w,h};																				  
    return SDL_RenderDrawRect(rnd, &r);
}

int SimpleSDL::drawOval(int x1, int y1, int w, int h)
{
    float theta, angle_increment;
    float w2 = w/2;
    float h2 = h/2;
    int xc = x1+w2;
    int yc = y1+h2;
    int n=10;

    SDL_Point points[4*n];

    angle_increment = M_PI_2 / n;

    int i=0;
    for (theta = 0.0f; theta <= M_PI_2; theta += angle_increment)
    {
        points[i].x = w2 * SDL_cos(theta);
        points[i].y = h2 * SDL_sin(theta);
        points[2*n-i-1].x = xc - points[i].x;
        points[2*n-i-1].y = yc + points[i].y;
        points[2*n+i].x = xc - points[i].x;
        points[2*n+i].y = yc - points[i].y;
        points[4*n-i-1].x = xc + points[i].x;
        points[4*n-i-1].y = yc - points[i].y;
        points[i].x += xc;
        points[i].y += yc;
        i++;
    }
    return SDL_RenderDrawLines(rnd,points,4*n);
}

int SimpleSDL::drawPoint(int x, int y)
{
    return SDL_RenderDrawPoint(rnd, x,y);
}

int SimpleSDL::fillRect(int x1, int y1, int w, int h)
{
    SDL_Rect r = {x1,y1,w,h};
    return SDL_RenderFillRect(rnd, &r);
}

int SimpleSDL::fillOval(int x1, int y1, int w, int h)
{
    float a = w/2;
    float b = h/2;
    int xc = x1+a;
    int yc = y1+b;
    float a2 = a*a;
    float b2 = b*b;
    float dq;
    if (a<b)
    {
        for(float x=0; x<=a; x++)
        {
            dq = b*SDL_sqrt(a2-x*x)/a;
            drawLine(xc+x,yc-dq,xc+x,yc+dq);
            drawLine(xc-x,yc-dq,xc-x,yc+dq);
        }
    }
    else
    {
        for(int y=0; y<=b; y++)
        {
            dq = a*SDL_sqrt(b2-y*y)/b;
            drawLine(xc-dq,yc+y,xc+dq,yc+y);
            drawLine(xc-dq,yc-y,xc+dq,yc-y);
        }
    }
    return 0;
}

bool SimpleSDL::checkEvent()
{
    hasEvent = (SDL_PollEvent(&event) == 1);
    return hasEvent;
}


bool SimpleSDL::isMouseMotionEvent()
{
    return (hasEvent && event.type == SDL_MOUSEMOTION);
}

bool SimpleSDL::isMouseButtonEvent()
{
    return (hasEvent && (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP));
}

bool SimpleSDL::isKeyEvent()
{
    return (hasEvent && event.type == SDL_KEYDOWN);
}

Sint32 SimpleSDL::getMouseX()
{
    if (isMouseMotionEvent()||isMouseButtonEvent())
    {
        return event.motion.x;
    }
    else
    {
        return -1;
    }
}

Sint32 SimpleSDL::getMouseY()
{
    if (isMouseMotionEvent()||isMouseButtonEvent())
    {
        return event.motion.y;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::getMouseButton()
{
    if (isMouseButtonEvent())
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            return 1;
        case SDL_BUTTON_MIDDLE:
            return 2;
        case SDL_BUTTON_RIGHT:
            return 3;
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::getMouseButtonCount()
{
    if (isMouseButtonEvent())
    {
        return event.button.clicks;
    }
    else
    {
        return 0;
    }
}

SDL_Keycode SimpleSDL::getKeyCode()
{
    if (isKeyEvent())
    {
        return event.key.keysym.sym;
    }
    else
    {
        return 0;
    }
}

void SimpleSDL::delay(int ms)
{
    SDL_Delay(ms);
}

void SimpleSDL::flushEvents()
{
	SDL_FlushEvents(SDL_KEYDOWN, SDL_MOUSEWHEEL);
}

int SimpleSDL::textSize(int size)
{
    if (size<=0 || size>screen_w)
    {
        return -1;
    }
    if (font != NULL && size==TTF_FontHeight(font))
        return 0;
    if (font != NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
    font = TTF_OpenFont("DejaVuSansMono.ttf", size); //this opens a font style and sets a size
    if (font != NULL )
    {
        fontSize = size;
        return 0;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::getTextSize()
{
    return fontSize;
}

int SimpleSDL::text(int x, int y, const std::string msg)
{
    return text(x,y, msg.c_str());
}

int SimpleSDL::text(int x, int y, const char* msg)
{
    if (font!=NULL)
    {
        SDL_Color c;
        SDL_GetRenderDrawColor(rnd,&c.r, &c.g, &c.b, &c.a);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, msg, c);
        SDL_Texture* text = SDL_CreateTextureFromSurface(rnd, textSurface);
        int text_width = textSurface->w;
        int text_height = textSurface->h;
        SDL_FreeSurface(textSurface);
        SDL_Rect renderQuad = { x, y, text_width, text_height };
        SDL_RenderCopy(rnd, text, NULL, &renderQuad);
        SDL_DestroyTexture(text);
        return 0;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::text(int x, int y, const std::string msg, int size)
{
    return text(x,y,msg.c_str(),size);
}

int SimpleSDL::text(int x, int y, const char* msg, int size)
{
    textSize(size);
    return text(x,y,msg);
}


int SimpleSDL::text(const std::string msg, int size)
{
    return text(msg.c_str(), size);
}

int SimpleSDL::text(const char* msg, int size)
{
    textSize(size);
    return text(msg);
}


int SimpleSDL::text(const std::string msg)
{
    return text(msg.c_str());
}

int SimpleSDL::text(const char* msg)
{
    int w,h;
    TTF_SizeText(font,msg,&w,&h);
    return text((screen_w-w)/2,(screen_h-h)/2,msg);
}

int SimpleSDL::setImage(const std::string name, const std::string filename)
{
    try
    {
        SDL_Surface* img = SDL_LoadBMP(filename.c_str());
        if (img != NULL)
        {
            SDL_Color bg = colors["bg"];
            SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format,bg.r,bg.g,bg.b));
            images[name] = SDL_CreateTextureFromSurface(rnd, img);
            SDL_FreeSurface(img);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
}

int SimpleSDL::drawImage(int x, int y, const std::string name)
{
    try
    {
        SDL_Texture* img = images.at(name);
        int w,h;
        SDL_QueryTexture(img, NULL, NULL, &w, &h);
        SDL_Rect dstRect = {x,y,w,h};
        SDL_RenderCopy(rnd, img, NULL, &dstRect);
        return 0;
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
}



int SimpleSDL::drawHLine(int x1, int y1, int x2, int y2, int h)                                                        
{	
	if(h>0){
			if(h%2!=0){h++;}
			y1=y1-(h/2);
			y2=y2-(h/2);
			for( ; h!=0; h--){
						      drawLine(x1,y1,x2,y2);
							  y1++;
							  y2++;}
			return 0;}
			else{
				  return -1;
				  }
}		

int SimpleSDL::drawWLine(int x1, int y1, int x2, int y2, int w)                                                        
{	
	if(w>0){
			if(w%2!=0){w++;}
			x1=x1-(w/2);
			x2=x2-(w/2);
			for( ; w!=0; w--){
						      drawLine(x1,y1,x2,y2);
							  x1++;
							  x2++;}
			return 0;}
			else{
				  return -1;
				  }
}	

int SimpleSDL::drawHalfOval(int x1, int y1, int w, int h, const char* bgcol, const char* fgcol)
{	
	y1=y1-h;
	h=2*h;
    
    drawOval(x1, y1, w, h);
    
    useColor(bgcol);
    
    fillRect(x1, y1+(h/2), w+1, h/2);
    
    useColor(fgcol);
    
    return 0;
    
}

int SimpleSDL::fillHalfOval(int x1, int y1, int w, int h, const char* bgcol, const char* fgcol)
{	
	y1=y1-h;
	h=h*2;
    
    fillOval(x1, y1, w, h);
    
    useColor(bgcol);
    
    fillRect(x1, y1+(h/2), w+1, (h/2)+1);
    
    useColor(fgcol);
    
    return 0;
    
}

int SimpleSDL::drawTrian(int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x3, y3);
	drawLine(x3, y3, x1, y1);
	
	return 0;
}


int SimpleSDL::getKeyCode(int &key)
{
	if(checkEvent()){
						if(isKeyEvent()){
										  key=getKeyCode();
										 }
					  }
	return key;
}


SimpleSDL::~SimpleSDL()
{
    for (std::map<std::string,SDL_Texture*>::iterator img=images.begin(); img!=images.end(); ++img)
    {
        SDL_DestroyTexture(img->second);
    }
    SDL_DestroyRenderer(rnd);
    SDL_DestroyWindow(wnd);
    SDL_Quit();
    if (TTF_WasInit())
    {
        if (font != NULL)
        {
            TTF_CloseFont(font);
        }
        TTF_Quit();
    }
}

float SimpleSDL::roundFloat(float f){
				
					 f=f*100;
					 f=round(f);
					 f=f/100;
					 
					 return f;
}

int SimpleSDL::drawPlanet(planet a){
	
	if(a.r<=0){return 1;}
	
	fillOval(a.x-a.r, a.y-a.r, 2*a.r, 2*a.r);

	return 0;
	
}

int SimpleSDL::pointDist(int ax, int ay, int bx, int by){
	int d;
	
	d=sqrt(pow(ax-bx, 2)+pow(ay-by,2));
	d=round(d);

	return d;
}

float SimpleSDL::gravityStrnght(planet a, planet bh){
	
	float f;
	
	f=a.mass*bh.mass;
 //////////////aggiungi costante G
	
	f=f/pow(pointDist(a.x, a.y, bh.x, bh.y), 2);
	//Ho tolto il mio roundFloat
	f=roundFloat(f);
	
	cout<<"G_FORCE "<<f<<endl;
	
	return f;
	
}


float SimpleSDL::planetAngle(planet a, planet bh){
	
	float dx, dy;
	
	//I set special cases
	if(a.y==bh.y && a.x>bh.x){return 0;}
	if(a.x==bh.x && a.y<bh.y){return 90;}
	if(a.y==bh.y && a.x<bh.x){return 180;}
	if(a.x==bh.x && a.y>bh.y){return 270;}
	
	dx=abs(a.x-bh.x);
	dy=abs(a.y-bh.y);
	
	float ang;

	ang=atan(dy/dx);
	
	ang=roundFloat(ang);
	
	if(a.y<bh.y && a.x<bh.x){return 180-ang;}
	if(a.y>bh.y && a.x<bh.x){return 180+ang;}
	if(a.y>bh.y && a.x>bh.x){return 360-ang;}
	
	return ang;
	
}

bool SimpleSDL::planetTouch(planet a, planet bh){
	
	int d;
	
	d=pointDist(a.x, a.y, bh.x, bh.y);
	
	if(d<=(a.r+bh.r)){return true;}
		else{return false;}
}

float SimpleSDL::setAccX(planet a, planet bh){   
	
	float f,fx;
	float ang;
	
	ang=planetAngle(a,bh);
	
	f=gravityStrnght(a,bh);

	if(ang==0){return -f/a.mass;}
	if(ang==180){return f/a.mass;}
	if(ang==90 || ang==270){return 0;}
	
	fx=(-1)*cos(ang)*f;

	float xa;
	
	xa=fx/a.mass;

	
	return xa;
	
}

float SimpleSDL::setAccY(planet a, planet bh){   
	
	float f,fy;
	float ang;
	
	ang=planetAngle(a,bh);

	
	f=gravityStrnght(a,bh);
	
	if(ang==90){return f/a.mass;}
	if(ang==270){return -f/a.mass;}
	if(ang==0 || ang==180){return 0;}
	
	fy=sin(ang)*f;

	float ya;
	
	ya=fy/a.mass;

		
	return ya;
	
}

void SimpleSDL::buildPlan(planet* a){ 						//Qui è da passare per riferimento
	
	int box;
	
	a->x=0;
	a->y=0;
	a->xs=0;
	a->ys=0;
	a->xa=0;
	a->ya=0;
	
	box=rand()%4;		
				if(box==0){
					a->x=-20+rand()%20;
					a->y=rand()%SCREEN_Y;
				}
				if(box==1){
					a->x=rand()%SCREEN_X;
					a->y=-30+rand()%30;
				}
				if(box==2){
					a->x=SCREEN_X+rand()%20;
					a->y=rand()%SCREEN_Y;
				}
				if(box==3){
					a->x=rand()%SCREEN_X;
					a->y=SCREEN_Y+rand()%30;
				}
		
				a->r=1+rand()%PLAN_MAX_R;
				a->mass=a->r * PLAN_DEN;
}

void SimpleSDL::updateSpd(planet* a){
	a->xs = a->xs + a->xa * DT;
	a->ys = a->ys + a->ya * DT;
}

void SimpleSDL::updateCoord(planet* a){
	a->x = a->x + a->xs * DT;
	a->y = a->y + a->ys * DT;
}

void SimpleSDL::bhAtmosp(planet* a, planet bh){
	a->xs = a->xs * (SPD_RED *( 1 / pow( pointDist(a->x, a->y, bh.x, bh.y), 2)));
	a->ys = a->ys * (SPD_RED *( 1 / pow( pointDist(a->x, a->y, bh.x, bh.y), 2)));
}









