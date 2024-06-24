#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <windows.h>
#include <random>
#include "image.h"
using namespace std;

#define BUBBLE_WIDTH 120 // 가로 픽셀 크기
#define BUBBLE_HEIGHT 120 // 세로 픽셀 크기

class Bubble {
protected:
    SDL_Rect rect; // {x축special feature 위치, y축 위치, 가로폭, 세로폭}
    SDL_Surface* image; //이미지
    
    int power; //공격력
    

public:
    int HP; //체력
    int speed = 2; //속도

    bool isVisible;
    int direction[2]; //움직일 방향. [0]이 x축 방향, [1]이 y축 방향

    Bubble(SDL_Surface* image, int power, int HP, int x, int y, int w = BUBBLE_WIDTH, int h = BUBBLE_HEIGHT);

    void render(SDL_Surface* surface);

    void setImage(SDL_Surface* image); //이미지 설정

    void move(); //움직임

    void setVisible(bool visible);
    void set_direction();

    SDL_Rect getRect();


};

class Boss : public Bubble {

public:
    Boss(SDL_Surface* image, int power, int HP, int x, int y, int w = BUBBLE_WIDTH, int h = BUBBLE_HEIGHT) 
        : Bubble(bubbleImage, power, 10, x, y, w, h) {

        this->image = image; //이미지 넣기
        isVisible = true;

        this->power = power; //공격력 설정
        this->HP = HP; //체력 설정

        rect = { x, y, w, h };

        int bx = 0;
        int by = 0;
        //비눗방울 x축 방향, y축 방향

        while (bx == 0 && by == 0) {

            random_device rd; //시드값 얻기 위한 random_device 생성
            mt19937 gen(rd()); //난수 생성 엔진 초기화

            uniform_int_distribution<int> BX(-1, 1); // -1 ~ 1 사이 정수
            bx = BX(rd);
            uniform_int_distribution<int> BY(-1, 1); // -1 ~ 1 사이 정수
            by = BY(rd);

        }

        direction[0] = bx;
        direction[1] = by;

    };

    void set_direction(SDL_Rect player); //플레이어를 따라가는 방향으로
};