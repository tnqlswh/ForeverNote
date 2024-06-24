#include "Bubble.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>
using namespace std;

Bubble::Bubble(SDL_Surface* image, int power, int HP, int x, int y, int w, int h) {

    this->image = image; //�̹��� �ֱ�
    isVisible = true;

    this->power = power; //���ݷ� ����
    this->HP = HP; //ü�� ����

    rect = { x, y, w, h };

    int bx = 0; 
    int by = 0;
    //�񴰹�� x�� ����, y�� ����

    while (bx == 0 && by == 0) {

        random_device rd; //�õ尪 ��� ���� random_device ����
        mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ

        uniform_int_distribution<int> BX(-1, 1); // -1 ~ 1 ���� ����
        bx = BX(rd);
        uniform_int_distribution<int> BY(-1, 1); // -1 ~ 1 ���� ����
        by = BY(rd);

    }

    direction[0] = bx;
    direction[1] = by;
}

void Bubble::render(SDL_Surface* surface) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����

    SDL_BlitSurface(image, NULL, surface, &rect);
}

void Bubble::setImage(SDL_Surface* image) {
    this->image = image;
}

void Bubble::set_direction() {

    //ȭ�� �� �� �Ѿ��
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

    //�÷��̾� ���󰡰�
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