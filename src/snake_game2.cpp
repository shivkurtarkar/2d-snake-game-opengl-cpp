#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<sys/ioctl.h>
#include<termios.h>

#include<chrono>
#include<thread>
#include<deque>

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
int snakePos[2];
int fruitPos[2]; 
int score; 
std::deque<std::pair<int,int>> tail;
int tailLength;

enum eDirection{
    STOP=0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir,lastDir;

void Setup(){
	gameOver=false;
	dir=STOP;
	snakePos[0]=width/2;
	snakePos[1]=height/2;
	fruitPos[0]=rand()%width;
	fruitPos[1]=rand()%height;
	score=0;
}

void Draw(){
	system("clear");
	for(int i(0); i<width+10; i++)	std::cout<<'#';
	std::cout<<std::endl;
	
	for(int i(0); i<height; i++){
		for(int j(0); j<width; j++) {
			if(j==0) std::cout<<"====#";
			if(j==snakePos[0] && i==snakePos[1])
				std::cout<<'O';
			else if(j==fruitPos[0] && i==fruitPos[1]) 
				std::cout<<'B';
			else {
				bool print =false;
				//print body
				for(int k=0;k<tail.size();k++){
					if(j==tail[k].first && i==tail[k].second){
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
		lastDir=dir;
		switch(getchar()) { 
			case 'a':
				if(lastDir!=RIGHT)
					dir=LEFT;
				break;
			case 'd':
				if(lastDir!=LEFT)
					dir=RIGHT;
				break;
			case 'w':
				if(lastDir!=DOWN)
					dir=UP;
				break;
			case 's':
				if(lastDir!=UP)
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
	tail.push_front(std::pair<int,int>(snakePos[0],snakePos[1]));
	for(int i=(tail.size()-tailLength);i>0;i--)
		tail.pop_back();
	switch(dir){
		case LEFT:
			snakePos[0]--;
			break;
		case RIGHT:
			snakePos[0]++;
			break;
		case UP:
			snakePos[1]--;
			break;
		case DOWN:
			snakePos[1]++;
			break;
		default:
			break;		
	}	
	if(snakePos[0]>=width) snakePos[0]=0;
	else if(snakePos[0]<0) snakePos[0]=(width-1);
	
	if(snakePos[1]>=height) snakePos[1]=0;
	else if(snakePos[1]<0) snakePos[1]=(height-1);
	
	//biting tail
	for(int i(0); i<tail.size(); i++){
		if(snakePos[0]==tail[i].first && snakePos[1]==tail[i].second)
			gameOver=true;
	}	

	//food
	if(snakePos[0]==fruitPos[0] && snakePos[1]==fruitPos[1]){
		score+=10;
		fruitPos[0] = rand()%width;
		fruitPos[1] = rand()%height;
		tailLength++;
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
	std::cout<<"snake pos:"<<snakePos[0]<<' '<<snakePos[1]<<std::endl;
	std::cout<<"fruit pos:"<<fruitPos[0]<<' '<<fruitPos[1]<<std::endl;
	std::cout<<"Score: "<<score<<std::endl;
	};

	return 0;
}
