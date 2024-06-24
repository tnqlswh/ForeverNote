#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <windows.h>
#include <random>
#include "image.h"
using namespace std;

#define BUBBLE_WIDTH 120 // ���� �ȼ� ũ��
#define BUBBLE_HEIGHT 120 // ���� �ȼ� ũ��

class Bubble {
protected:
    SDL_Rect rect; // {x��special feature ��ġ, y�� ��ġ, ������, ������}
    SDL_Surface* image; //�̹���
    
    int power; //���ݷ�
    

public:
    int HP; //ü��
    int speed = 2; //�ӵ�

    bool isVisible;
    int direction[2]; //������ ����. [0]�� x�� ����, [1]�� y�� ����

    Bubble(SDL_Surface* image, int power, int HP, int x, int y, int w = BUBBLE_WIDTH, int h = BUBBLE_HEIGHT);

    void render(SDL_Surface* surface);

    void setImage(SDL_Surface* image); //�̹��� ����

    void move(); //������

    void setVisible(bool visible);
    void set_direction();

    SDL_Rect getRect();


};

class Boss : public Bubble {

public:
    Boss(SDL_Surface* image, int power, int HP, int x, int y, int w = BUBBLE_WIDTH, int h = BUBBLE_HEIGHT) 
        : Bubble(bubbleImage, power, 10, x, y, w, h) {

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

    };

    void set_direction(SDL_Rect player); //�÷��̾ ���󰡴� ��������
};