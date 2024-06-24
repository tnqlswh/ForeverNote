#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
using namespace std;

class Status {
protected:
    SDL_Rect rect;

    string level;
    string exp;
    string spirit;
    string health;

    SDL_Surface* image;
public:

    //생성자
    Status(SDL_Surface* image, int level, int exp, int spirit, int health);

    //사각형 그리기
    void render(SDL_Surface* surface, TTF_Font* font);

    void setText(int level, int exp, int spirit, int health);

};