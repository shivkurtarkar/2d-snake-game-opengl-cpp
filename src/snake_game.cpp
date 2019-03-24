#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<sys/ioctl.h>
#include<termios.h>

#include<chrono>
#include<thread>

void enable_raw_mode(){
	termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &term);
}
void disable_raw_mode(){
	termios term;
	tcgetattr(0, &term);
	term.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &term);
}
bool kbhit(){
	int bytewaiting;
	ioctl(0, FIONREAD, &bytewaiting);
	return bytewaiting >0;
}

bool gameOver;
const int width = 50;
const int height = 20;
int snakeX,snakeY;
int fruitX,fruitY; 
int score; 
int tailX[100],tailY[100];
int nTail;

enum eDirection{
    STOP=0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;

void Setup(){
	gameOver=false;
	dir=STOP;
	snakeX=width/2;
	snakeY=height/2;
	fruitX=rand()%width;
	fruitY=rand()%height;
	score=0;
}

void Draw(){
	system("clear");
	for(int i(0); i<width+10; i++)	std::cout<<'#';
	std::cout<<std::endl;
	
	for(int i(0); i<height; i++){
		for(int j(0); j<width; j++) {
			if(j==0) std::cout<<"====#";
			if(j==snakeX && i==snakeY)
				std::cout<<'O';
			else if(j==fruitX && i==fruitY) 
				std::cout<<'B';
			else {
				bool print =false;
				//print body
				for(int k=0;k<nTail;k++){
					if(j==tailX[k] && i==tailY[k]){
						print=true;
						break;
					}
				}
				if(print) std::cout<<'o';
				else std::cout<<' ';
			}
			if(j==(width-1)) std::cout<<"#====";
		}
		std::cout<<std::endl;
	}

	for(int i(0); i<width+10; i++)	std::cout<<'#';
	std::cout<<std::endl;
       

}

void Input(){
	enable_raw_mode();
	if(kbhit()){
		switch(getchar()) { 
			case 'a':
				dir=LEFT;
				break;
			case 'd':
				dir=RIGHT;
				break;
			case 'w':
				dir=UP;
				break;
			case 's':
				dir=DOWN;
				break;
			case 'x':
				gameOver=true;
				break;
		}
		std::cin.clear();
		
		std::cout<<"Keypress "<<dir<<' ';
	}
	else std::cout<<"\b-";
	disable_raw_mode();
}
void Logic(){
	//move the tail
	for(int i(nTail-1);i>0;i--){
		tailX[i]=tailX[i-1];
		tailY[i]=tailY[i-1];
	}
	tailX[0]=snakeX;
	tailY[0]=snakeY;
	switch(dir){
		case LEFT:
			snakeX--;
			break;
		case RIGHT:
			snakeX++;
			break;
		case UP:
			snakeY--;
			break;
		case DOWN:
			snakeY++;
			break;
		default:
			break;		
	}	
	if(snakeX>=width) snakeX=0;
	else if(snakeX<0) snakeX=(width-1);
	
	if(snakeY>=height) snakeY=0;
	else if(snakeY<0) snakeY=(height-1);
	
	//biting tail
	for(int i(0); i<nTail; i++){
		if(snakeX==tailX[i] && snakeY==tailY[i])
			gameOver=true;
	}	

	//food
	if(snakeX==fruitX && snakeY==fruitY){
		score+=10;
		fruitX = rand()%width;
		fruitY= rand()%height;
		nTail++;
	}
}

int main(){

	Setup();


	std::cout<<"Snake game"<<std::endl;	
	Draw();
	while(!gameOver){
		Draw();
		Input();
		Logic();
		std::this_thread::sleep_for(std::chrono::milliseconds(60));
	std::cout<<"snake pos:"<<snakeX<<' '<<snakeY<<std::endl;
	std::cout<<"fruit pos:"<<fruitX<<' '<<fruitY<<std::endl;
	std::cout<<"Score: "<<score<<std::endl;
	};

	return 0;
}
