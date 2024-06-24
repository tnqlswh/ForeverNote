#include "Bubble.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>
using namespace std;

Bubble::Bubble(SDL_Surface* image, int power, int HP, int x, int y, int w, int h) {

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
}

void Bubble::render(SDL_Surface* surface) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음

    SDL_BlitSurface(image, NULL, surface, &rect);
}

void Bubble::setImage(SDL_Surface* image) {
    this->image = image;
}

void Bubble::set_direction() {

    //화면 밖 안 넘어가게
    if (rect.x <= 0) direction[0] = 1;
    else if (rect.x + rect.w >= 1200) direction[0] = -1;
    if (rect.y <= 0) direction[1] = 1;
    else if (rect.y + rect.h >= 675) direction[1] = -1;

}

void Bubble::move() {

    this->set_direction();

    rect.x += direction[0] * speed;
    rect.y += direction[1] * speed;

}

void Bubble::setVisible(bool visible) {
    isVisible = visible;
}

SDL_Rect Bubble::getRect() {
    return rect;
}

void Boss::set_direction(SDL_Rect player) {

    //플레이어 따라가게
    if (rect.x < player.x)
        this->direction[0] = 1;
    else if (rect.x > player.x)
        this->direction[0] = -1;
    else
        this->direction[0] = 0;

    if (rect.y < player.y)
        this->direction[1] = 1;
    else if (rect.y > player.y)
        this->direction[1] = -1;
    else
        this->direction[1] = 0;

}