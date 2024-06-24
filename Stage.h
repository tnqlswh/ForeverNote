#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Bubble.h"
#include "image.h"
#include "Player.h"

#define BUBBLE_PCS 15

#define SHAMPOO 0
#define CANDY 1



typedef struct STAGE_INFORMATION { //���������� ���� ����

    int Bubble_Power; //�񴰹�� ���ݷ�
    int Bubble_Num; //�񴰹�� ����
    int Bubble_HP; //�񴰹�� ü��

}STAGE_INFO;



class Stage{
protected:
    Bubble* bubble[BUBBLE_PCS]; //�񴰹��

    STAGE_INFO info; //�������� ����

    bool isVisible;

    int item; //���������� �ִ� ������ ���� (0 : ��Ǫ, 1 : ĵ��, 2~4: ���丮��, 5:Ʃ�丮��)

public:
    Stage(STAGE_INFO info, int item);

    void render(SDL_Surface* surface); //ȭ�鿡 ���

    void setVisible(bool visible);

    void crash(Player& player); //�񴰹�� �浹�� ó��
    void get_item(Player& player, SDL_Surface* surface, SDL_Window* window); //������ ȹ��� ó��

    Bubble* getBubble(int i); //�񴰹��

    int updateBubble(int damage, int speed, SDL_Surface* image); 
    //��� �񴰹���� ���� ���� (��!!!!�� �� ���� ��� ����� ����)
    //�÷��̾ ���� exp ����

    void deleteBubble(int i); //�񴰹�� ����

    void Bubble_angry(SDL_Rect player); // �񴰹����� ����
};


class Boss_Stage : public Stage{

public:
    Boss* boss; //����

    Boss_Stage(STAGE_INFO info, SDL_Surface* boss_image) : Stage(info, 0) {

        this->info = info;

        boss = new Boss(boss_image, info.Bubble_Power, info.Bubble_HP, 600, 300);

        isVisible = true;

    };

    void create_bubble(); //�񴰹�� ����

    void boss_crash(Player& player); //���� �浹���� �� ó��

    int updateBubble(int damage, int speed, SDL_Surface* bubble_image);
    //��� �񴰹���� ���� ���� (��!!!!�� �� ���� ��� ����� ����)
    //�÷��̾ ���� exp ����

    void render(SDL_Surface* surface, SDL_Rect player); //ȭ�鿡 ���

};