#include "SDL/SDL_image.h"
#include "SDL/SDL.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

const int SCREEN_WIDTH = 900; //screen size
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

SDL_Surface *turret = NULL; //ptr for turret
SDL_Surface *enemy = NULL; //ptr for enemy
SDL_Surface *background = NULL; //ptr for background
SDL_Surface *screen = NULL; //ptr for the screen
SDL_Surface *square = NULL; //ptr for blue square
SDL_Surface *wrongsquare = NULL; //ptr for red square
SDL_Surface *bullet = NULL;
SDL_Surface *explode = NULL;
SDL_Surface *wrecked = NULL;

vector< vector<int> >path;

//event structure
SDL_Event event;

//class for Enemy objects
class Enemy
{
  public:
		Enemy();
		Enemy(int,int);
		void move();
		void show();
		int getX();
		int getY();
		void hit();
		bool isDone();

	private:
		int xPos,yPos,xVel,yVel,health,done;
};

class Turret
{
	public:
		Turret();
		Turret(int,int);
		int getCenterX();
		int getCenterY();
		void show();
	private:
		int x;
		int y;
};

class Bullet
{
	public:
		Bullet(Turret,Enemy);
		void move();
		void show();
		bool isDone();
	private:
		Turret t;
		Enemy e;
		int startX, startY, targetX, targetY, done;
		double xVel, yVel, distance, speed;
};

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
	SDL_Rect offset;
	offset.x=x;
	offset.y=y;
	SDL_BlitSurface(source,NULL,destination,&offset);
}

SDL_Surface *load_image(std::string filename)
{
	SDL_Surface* loadedImage=NULL;
	SDL_Surface* optimizedImage=NULL;
	loadedImage=IMG_Load(filename.c_str());
	if(loadedImage!=NULL)
	{
		optimizedImage=SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
		if(optimizedImage != NULL)
		{
			Uint32 colorkey = SDL_MapRGB(optimizedImage->format,255,255,255);
			SDL_SetColorKey(optimizedImage,SDL_SRCCOLORKEY,colorkey );
		}
		return optimizedImage;
	}
}

bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) return false;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if(screen==NULL) return false;
	return true;
}

bool load_files()
{
	enemy = load_image("enemy1.png");
	turret = load_image("turret1.png");
	background = load_image("background.png");
	square = load_image("square.bmp");
	wrongsquare = load_image("squarewrong.bmp");
	bullet = load_image("Bullet.bmp");
	explode = load_image("explosion.bmp");
	wrecked = load_image("destroyed.bmp");
	SDL_SetAlpha(square,SDL_SRCALPHA,100);//transparency
	SDL_SetAlpha(wrongsquare,SDL_SRCALPHA,100);
	if(background==NULL || enemy==NULL || turret==NULL || square==NULL || wrongsquare==NULL || bullet==NULL || explode==NULL || wrecked==NULL) return false;
	return true;
}

void clean_up()
{
	SDL_FreeSurface(turret);
	SDL_FreeSurface(enemy);
	SDL_FreeSurface(background);
	SDL_FreeSurface(square);
	SDL_FreeSurface(wrongsquare);
	SDL_FreeSurface(bullet);
	SDL_FreeSurface(explode);
	SDL_FreeSurface(wrecked);
	SDL_Quit();
}

void load_text(const char *filename)
{
	vector<int> single_row;
	string line;
	int num;
	ifstream pathfile(filename);
	if(pathfile.is_open())
	{
		while(pathfile.good()){
			getline(pathfile,line);
			stringstream lineStream(line);
			while(lineStream>>num){
				single_row.push_back(num);
			}
			path.push_back(single_row);
			single_row.clear();
		}
		pathfile.close();
	}
}

Enemy::Enemy() //default constructor
{
	//initial starting position and velocities
	xPos=100;
	yPos=-20;
	xVel=0;
	yVel=1;
	health=5;
	done=0;
}

Enemy::Enemy(int x1, int y1) //nondefault constructor
{
	//initial starting position and velocities
	xPos=x1;
	yPos=y1;
	xVel=0;
	yVel=1;
	health=5;
	done=0;
}

bool Enemy::isDone()
{
	if(done) return true;
	return false;
}

void Enemy::hit()
{
	health--;
}

int Enemy::getX()
{
	return xPos;
}

int Enemy::getY()
{
	return yPos;
}

//move enemies
void Enemy::move()
{
	xPos+=xVel;
	yPos+=yVel;
	if(xPos==100 && yPos==400)
	{
		//corners for the road, where the enemy turns
		xVel=1;
		yVel=0;
	}
	if(xPos==600 && yPos==400)
	{
		xVel=0;
		yVel=-1;
	}
	if(xPos==600 && yPos==100)
	{
		xVel=-1;
		yVel=0;
	}
	if(xPos==300 && yPos==100)
	{
		xVel=0;
		yVel=1;
	}
	if(xPos==300 && yPos==200)
	{
		done=1;
	}
}

void Enemy::show()
{
	if(health <= 0) apply_surface(xPos,yPos,wrecked,screen);
	else if(!done) apply_surface(xPos,yPos,enemy,screen);
}

Turret::Turret()
{
}

Turret::Turret(int xPos, int yPos)
{
	x=xPos;
	y=yPos;
}

int Turret::getCenterX()
{
	return x+50;
}

int Turret::getCenterY()
{
	return y+50;
}

void Turret::show()
{
	apply_surface(x,y,turret,screen);
}

Bullet::Bullet(Turret t1, Enemy e1)
{
	t=t1;
	e=e1;
	startX=t.getCenterX();
	startY=t.getCenterY();
	targetX=e.getX();
	targetY=e.getY();
	speed=10;
	done=0;

	double x_Dist=targetX-startX;
	double y_Dist=targetY-startY;

	distance=sqrt(x_Dist*x_Dist+y_Dist*y_Dist);

	double angle = atan(y_Dist/x_Dist);

	if(targetX>=startX){
		xVel=speed*cos(angle);
		yVel=speed*sin(angle);
	}
	else {
		xVel=-1*speed*cos(angle);
		yVel=-1*speed*sin(angle);
	}
}

void Bullet::move()
{
	startX+=xVel;
	startY+=yVel;
	distance-=speed;
}

void Bullet::show()
{
	if(distance>0) apply_surface(startX,startY,bullet,screen);
	else if(distance==0) e.hit();
	else if(distance>-50) apply_surface(targetX,targetY,explode,screen);
	else done=1;
}

bool Bullet::isDone()
{
	if(done) return true;
	return false;
}

int main(int argx, char* args[])
{
	bool quit=false;
	int x=0;
	int y=0;
	int mouse_x=0;
	int mouse_y=0;

	vector<Turret> towers;
	vector<Bullet> shoot;
	vector<Enemy> tanks;

	load_text("lvl0.txt");

	int clock=0;

	if(init() == false) return 1;
	if(load_files() == false) return 1;

	apply_surface(0,0,background,screen);//display background

	while(quit==false)
	{
		clock++;
		if(clock==100000) clock=0;
		apply_surface(0,0,background,screen); //draw background
		for(int i=0;i<towers.size();i++){ //draw turrets
			towers[i].show();
			if(clock%50==0){
				double dist=300;
				Enemy closest;
				for(int y=0;y<tanks.size();y++){
					double dist2=sqrt(pow(towers[i].getCenterX()-tanks[y].getX(),2)+pow(towers[i].getCenterY()-tanks[y].getY(),2));
					if(dist2<dist){
						closest=tanks[y];
						dist=dist2;
					}
				}
				if(dist<300) shoot.push_back(Bullet(towers[i],closest));
			}
		}
		for(int j=0;j<shoot.size();j++){
			shoot[j].move();
			shoot[j].show();
		}
		if(clock%200==0) tanks.push_back(Enemy());
		for(int k=0;k<tanks.size();k++){
			tanks[k].move();
			tanks[k].show();
		}
		for(int k=0;k<tanks.size();k++){
			if(tanks[k].isDone()) tanks.erase(tanks.begin()+k);
		}
		for(int j=0;j<shoot.size();j++){
			if(shoot[j].isDone()) shoot.erase(shoot.begin()+j);
		}
		if(path[mouse_y/100][mouse_x/100]==0) apply_surface(mouse_x,mouse_y,square,screen);
		else apply_surface(mouse_x,mouse_y,wrongsquare,screen);
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_MOUSEMOTION) //move cursor
			{
				x = event.motion.x;//set coords
				y = event.motion.y;
				for(int i=0;i<=800;i+=100) //goes by 100's becuase that is where each grid starts
				{
					for(int j=0;j<=500;j+=100)
					{
						if(x>i && x<i+100 && y>j && y<j+100)//if the mouse is between these values
						{
							mouse_x = i;//sets coords to corner of box for proper turret and
							mouse_y = j;//selction box placement
						}
					}
				}
			}				
			if(event.button.button == SDL_BUTTON_LEFT) //if mouse left button is clicked
			{
				x=event.button.x; //get/set coords
				y=event.button.y;
				for(int i=0;i<=7;i+=1){
					for(int j=0;j<=5;j+=1){
						if(x>i*100 && x<i*100+100 && y>j*100 && y<j*100+100){
							if(!path[j][i]){
								int alreadythere=0;
								for(int x=0;x<towers.size();x++){
									if(towers[i].getCenterX()-50==i*100 && towers[i].getCenterY()-50==j*100) alreadythere=1;
								}
								if(!alreadythere) towers.push_back(Turret(i*100,j*100));
							}
						}
					}
				}
			}	
			//quit program event
			if(event.type == SDL_QUIT) quit = true;
		}
		//Update
		if(SDL_Flip(screen) == -1) return 1;
	}
	clean_up();
	return 0;
}
