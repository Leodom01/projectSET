#include "SimpleSDL.h"
#include <time.h>
#include <stdio.h>
#include <iostream>

using namespace std;



//Planets array
planet all_plan[PLAN_NUM];

//General var
int i;
int box;
int eat_plan;					//Planets eaten by the black hole


int main(int argc, char* args[]){
	SimpleSDL s("Project_SET", SCREEN_X, SCREEN_Y);
	
	srand(time(NULL));

	s.setColor("sky", 23, 30, 63);
	s.setColor("plan", 102, 48, 17);
	s.setColor("bh", 48, 48, 48);

	s.useColor("sky");
	s.clear();
	s.useColor("bh");
	
	//I create the black hole 
	planet bh;
	bh.x=SCREEN_X/2;
	bh.y=SCREEN_Y/2;
	bh.xs=0;
	bh.ys=0;
	bh.xa=0;
	bh.ya=0;
	bh.r=35;
	bh.mass=100000;
	eat_plan=0;
	//I set the black hole
	s.drawPlanet(bh);
	
	s.redraw();
	
	i=0;

	s.useColor("plan");
	s.buildPlan(&all_plan[i]);
	s.redraw();
	do{
		s.useColor("bh");
		s.drawPlanet(bh);
		
		all_plan[i].xa=s.setAccX(all_plan[i], bh);
		all_plan[i].ya=s.setAccY(all_plan[i], bh);
		
		s.updateSpd(&all_plan[i]);
		s.updateCoord(&all_plan[i]);
		s.bhAtmosp(&all_plan[i], bh);
		
		s.useColor("plan");
		s.drawPlanet(all_plan[i]); 
		cout<<"X "<<all_plan[i].x<<endl<<"Y "<<all_plan[i].y<<endl<<"X_SPD "<<all_plan[i].xs<<endl<<"Y_SPD "<<all_plan[i].ys<<endl<<"X_ACC "<<all_plan[i].xa<<endl<<"Y_ACC "<<all_plan[i].ya<<endl<<"-----------------"<<endl;
		s.redraw();
		s.delay(50);
		
		s.useColor("sky");
		s.clear();
		
	}while(true);


	
	system("pause");
	
	return 0;

}

