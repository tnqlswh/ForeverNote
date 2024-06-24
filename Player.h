#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

#define PLAYER_WIDTH 75 //가로 픽셀 크기
#define PLAYER_HEIGHT 117 //세로 픽셀 크기

class Player {
protected:
    SDL_Rect rect; // {x축special feature 위치, y축 위치, 가로폭, 세로폭}
    SDL_Surface* image; //이미지
    int speed = 10; //속도
   
public:

    int guilty = 0; //그냥 말풍선 뜨는 기간 설정하려고 만든 거임.

    int level = 1; //레벨
    int power = 1; //공격력
    int exp = 0; //경험치

    int rock_spirit = 100; //락커의 자존심
    int throat_health = 50; //목상태

    bool isVisible;
    bool isRocking; //공격중인지

    Player(SDL_Surface* image, int x, int y, int w = PLAYER_WIDTH, int h = PLAYER_HEIGHT);

    void render(SDL_Surface* surface);

    void setImage(SDL_Surface* image); //이미지 설정

    void move(SDL_Keycode key); //움직임

    void setVisible(bool visible);

    void rock(SDL_Surface* surface); //락 공격

    void setRect(int x, int y); //위치 설정

    SDL_Rect getRect(); //위치 정보


};