/*
 * main.cpp

 *
 *  Created on: Nov 20, 2013
 *      Author: timur
 */
#include <iostream>
#include <vector>
#include <algorithm>

#include "SDL2/SDL.h"

using namespace std;

//Start SDL
class Solved_Puzzle{
public:
	SDL_Window* gWindow;
	SDL_Surface* gScreenSurface;
	SDL_Surface* gSolved_Puzzle_For_Kids;
	struct coord{
		int x,y;
	};
	coord Current_Position;
	coord game_map[5][5];
	float x_step;
	float y_step;
	SDL_Rect source;
	SDL_Rect dest;

	Solved_Puzzle(){
		if(init())
			if(loadMedia())
			{
				vector<int> check_coord;
				Current_Position.x = 4;
				Current_Position.y = 4;
				SDL_GetClipRect(gSolved_Puzzle_For_Kids, &source);
				x_step = source.w / 5;
				y_step = source.h / 5;
				source.w = x_step;
				source.h = y_step;
				dest.w = x_step;
				dest.h = y_step;
				srand(time(NULL));
				for (int y = 0;y < 5; y++){
					for(int x = 0;x < 5;){
						int check_result = rand()%24;
						vector<int>::iterator it = find(check_coord.begin(),
								check_coord.end(), check_result);
						if(it == check_coord.end()){
							game_map[x][y].x = check_result / 5;
							game_map[x][y].y = check_result % 5;
							source.x = game_map[x][y].x * x_step;
							source.y = game_map[x][y].y * y_step;
							dest.x = x * x_step + 450;
							dest.y = y * y_step + 400;
							SDL_BlitSurface(gSolved_Puzzle_For_Kids, &source, gScreenSurface, &dest);
					//		cout << game_map[x][y].x << " " << game_map[x][y].y << endl;
							check_coord.push_back(check_result);
							x++;
							if((x == 4 ) && (y == 4))break;
						}
					}
				}

			//	SDL_Rect dest;
			//	dest.x=100;
			//	dest.y=100;

		//		SDL_BlitSurface(gSolved_Puzzle_For_Kids, NULL, gScreenSurface, &dest);
				SDL_UpdateWindowSurface(gWindow);
				SDL_Event event;
				bool quit = false;
				while(!quit){
					while(SDL_PollEvent(&event)){
						switch(event.type){
						case SDL_KEYDOWN:
						//	cout<< &event.key << endl;
							if (event.key.keysym.sym == SDLK_ESCAPE)quit = true;
							if ((event.key.keysym.sym == SDLK_UP) &&
									(Current_Position.y != 4))Move_Small_Pic(SDLK_UP);
							if ((event.key.keysym.sym == SDLK_RIGHT) &&
									(Current_Position.x != 0))Move_Small_Pic(SDLK_RIGHT);
							if ((event.key.keysym.sym == SDLK_LEFT) &&
									(Current_Position.x != 4))Move_Small_Pic(SDLK_LEFT);
							if ((event.key.keysym.sym == SDLK_DOWN) &&
									(Current_Position.y != 0))Move_Small_Pic(SDLK_DOWN);
							break;
						}
					}
				}
			//	SDL_Delay(8000);
				close();
			}
	}
	void Move_Small_Pic(SDL_Keycode key){
		int offset_x = (key == SDLK_RIGHT ?-1:key == SDLK_LEFT ? 1:0);
		int offset_y = (key == SDLK_DOWN ?-1:key == SDLK_UP ? 1:0);
		source.x = game_map[Current_Position.x + offset_x]
		                    [Current_Position.y + offset_y].x * x_step;
		source.y = game_map[Current_Position.x + offset_x]
		                    [Current_Position.y + offset_y].y * y_step;
		source.w = x_step;
		source.h = y_step;
//		cout<< Current_Position.y +
 //               (key == SDLK_DOWN?-1:key == SDLK_UP ? 1:0) << endl;
		dest.x = (Current_Position.x + offset_x) * x_step + 450;
		dest.y = (Current_Position.y + offset_y) * y_step + 400;
		dest.w = x_step;
		dest.h = y_step;
		while(dest.y != Current_Position.y * y_step + 400 ||
				(dest.x != Current_Position.x * x_step + 450)){
			//	&&
			//	(dest.x != Current_Position.x * x_step + 450)){
			SDL_FillRect(gScreenSurface,&dest, 0);
			switch(key){
			case SDLK_UP:
				dest.y--;
				break;
			case SDLK_RIGHT:
				dest.x++;
				break;
			case SDLK_LEFT:
				dest.x--;
				break;
			case SDLK_DOWN:
				dest.y++;
				break;
			default:
				break;
			}
			SDL_BlitSurface(gSolved_Puzzle_For_Kids, &source, gScreenSurface, &dest);
			SDL_UpdateWindowSurface(gWindow);
		//	SDL_Delay(1);
		}
		game_map[Current_Position.x][Current_Position.y].x =
				game_map[Current_Position.x + offset_x]
				         [Current_Position.y + offset_y].x;
		game_map[Current_Position.x][Current_Position.y].y =
				game_map[Current_Position.x + offset_x]
				         [Current_Position.y + offset_y].y;

		Current_Position.y = Current_Position.y + offset_y;
		Current_Position.x = Current_Position.x + offset_x;

	}
	~Solved_Puzzle(){};
	bool init()
	{
		bool success = true;
		if (SDL_Init (SDL_INIT_VIDEO) < 0){
			success = false;
			cout<< SDL_GetError()<<endl;
			printf("Get the SDL Library!");
		}else
		{
			SDL_DisplayMode set_proper_mode;
			gWindow = SDL_CreateWindow("Solved Puzzle for Kids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 1024, SDL_WINDOW_FULLSCREEN);
			set_proper_mode.w = 1280;
			set_proper_mode.h = 1024;
			set_proper_mode.refresh_rate = 0;
			set_proper_mode.driverdata = 0;
			set_proper_mode.format = SDL_PIXELFORMAT_RGBA8888;
			SDL_SetWindowDisplayMode(gWindow, &set_proper_mode);
			if(gWindow == NULL)
			{
				printf("Please check the video driver!");
			}
			else
			{
				gScreenSurface = SDL_GetWindowSurface (gWindow);
			}
		}
		return success;
	}
	bool loadMedia()
	{
		bool success = true;
		gSolved_Puzzle_For_Kids = SDL_LoadBMP("/.Beauty-Panda-working.bmp");
/*		SDL_Rect size;
		SDL_GetClipRect(gSolved_Puzzle_For_Kids, &size);
		x_step = size.w / 10;
		y_step = size.y / 10;

		for(int y=0;y<10;y++){
			for(int x=0;x<10;x++){
				size.x = x * x_step;
				size.y = y * y_step;
				size.w = x_step;
				size.h = y_step;
				SDL_BlitSurface(gSolved_Puzzle_For_Kids, &size, &Good_Game[x][y], NULL);

				//	cout<< x<< " "<< y<<endl;
			}
		}
*/
//		cout<<size.w<<" "<<size.h<<endl;

		if (gSolved_Puzzle_For_Kids == NULL)
		{
			printf("Good image, check if everything correct with install!");
			success = false;
		}
		return success;
	}
	void close()
	{
		SDL_FreeSurface(gSolved_Puzzle_For_Kids);
		gSolved_Puzzle_For_Kids = NULL;

		SDL_DestroyWindow(gWindow);
		gWindow = NULL;

		SDL_Quit();
	}
};
int main( int argc, char* args[] )
{
	Solved_Puzzle game;
	return 0;
}
