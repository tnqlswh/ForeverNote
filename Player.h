#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

#define PLAYER_WIDTH 75 //���� �ȼ� ũ��
#define PLAYER_HEIGHT 117 //���� �ȼ� ũ��

class Player {
protected:
    SDL_Rect rect; // {x��special feature ��ġ, y�� ��ġ, ������, ������}
    SDL_Surface* image; //�̹���
    int speed = 10; //�ӵ�
   
public:

    int guilty = 0; //�׳� ��ǳ�� �ߴ� �Ⱓ �����Ϸ��� ���� ����.

    int level = 1; //����
    int power = 1; //���ݷ�
    int exp = 0; //����ġ

    int rock_spirit = 100; //��Ŀ�� ������
    int throat_health = 50; //�����

    bool isVisible;
    bool isRocking; //����������

    Player(SDL_Surface* image, int x, int y, int w = PLAYER_WIDTH, int h = PLAYER_HEIGHT);

    void render(SDL_Surface* surface);

    void setImage(SDL_Surface* image); //�̹��� ����

    void move(SDL_Keycode key); //������

    void setVisible(bool visible);

    void rock(SDL_Surface* surface); //�� ����

    void setRect(int x, int y); //��ġ ����

    SDL_Rect getRect(); //��ġ ����


};