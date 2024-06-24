#include "Player.h"
#include "image.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>
using namespace std;

Player::Player(SDL_Surface* image, int x, int y, int w, int h) {

    for(int i = 0; i < 4; i++)
        this->image = image; //�̹��� �ֱ�

    isVisible = true;   //ó���� �� ����. screen Ŀ���� ����.

    rect = { x, y, w, h };

}

void Player::render(SDL_Surface* surface) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����

    SDL_BlitSurface(image, NULL, surface, &rect);
}

void Player::setImage(SDL_Surface* image) {
    this->image = image;
}

void Player::move(SDL_Keycode key) {

    int i = 0;

    if (level == 5)
        i = 8;
    else if (level >= 3)
        i = 4;
    //�׳� �̹��� ���� ���缭 �������� �ϴ� ��

    switch (key) {
    case SDLK_DOWN:
    {
        setImage(playerImage[0 + i]);
        rect.y += speed;
        break;
    }
    case SDLK_UP:
    {
        setImage(playerImage[1 + i]);
        rect.y -= speed;
        break;
    }
    case SDLK_LEFT:
    {
        setImage(playerImage[2 + i]);
        rect.x -= speed;
        break;
    }
    case SDLK_RIGHT:
    {
        setImage(playerImage[3 + i]);
        rect.x += speed;
        break;
    }

    }

}

SDL_Rect Player::getRect() {
    return rect;
}

void Player::rock(SDL_Surface* surface) {
    int i = 0;

    if (level == 5)
        i = 2;
    else if (level >= 3)
        i = 1;
    //�׳� �̹��� ���� ���缭 �������� �ϴ� ��

    SDL_Rect E_rect = { rect.x - 20, rect.y, 117, 117 };

    SDL_BlitSurface(effectImage[i], NULL, surface, &E_rect);
}

void Player::setRect(int x, int y) {
    rect.x = x;
    rect.y = y;
}