#include "Stage.h"
#include "image.h"
#include "Player.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>

using namespace std;

typedef struct ITEM {
    SDL_Rect rect;
    SDL_Surface* image;
    int spirit_up;
    int health_up;
    bool isVisible;
}ITEM;

ITEM Item[6] = {
    { {600, 400, 90, 90}, itemImage[SHAMPOO], 7, 0, true}, // ��Ǫ
    { {400, 300, 90, 90}, itemImage[CANDY], 0, 3, true}, // ��ĵ��
    { {600, 200, 90, 90}, storyImage[0], 0, 0, true}, // ���丮 1
    { {200, 600, 90, 90}, storyImage[1], 0, 0, true}, // ���丮 2
    { {600, 300, 90, 90}, storyImage[2], 0, 0, true}, // ���丮 3
    { {300, 300, 90, 90}, itemImage[3], 0, 0, true} // Ʃ�丮��
};

Stage::Stage(STAGE_INFO info, int item) {

    this->item = item;

    this->info = info;

    int bx, by = 0; //�񴰹�� x�� ��ġ, y�� ��ġ

    random_device rd; //�õ尪 ��� ���� random_device ����
    mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ

    for (int i = 0; i < info.Bubble_Num; i++) {
        uniform_int_distribution<int> BX(0, 1080); // 170 ~ 730 ���� ����
        bx = BX(rd);
        uniform_int_distribution<int> BY(0, 555); //42 ~ 360 ���� ����
        by = BY(rd);

        bubble[i] = new Bubble(bubbleImage, info.Bubble_Power, info.Bubble_HP, bx, by); //�񴰹��
    }

    isVisible = true;
}

void Stage::render(SDL_Surface* surface) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����

    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->move(); //�񴰹�� �̵�
    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->render(surface); //�񴰹�� ���

    if (Item[item].isVisible) {
        if (item >= 2 && item <= 4) {
            SDL_BlitSurface(itemImage[2], NULL, surface, &Item[this->item].rect);
        }
        else if (item == 5) {
            SDL_BlitSurface(itemImage[3], NULL, surface, &Item[this->item].rect);
        }
        else {
            SDL_BlitSurface(itemImage[item], NULL, surface, &Item[this->item].rect);
        }
    }
}

void Stage::setVisible(bool visible) {
    isVisible = visible;
    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->setVisible(visible);
}

Bubble* Stage::getBubble(int i) {
    return bubble[i];
}

int Stage::updateBubble(int damage, int speed, SDL_Surface* image) {

    int exp = 0; //������ exp

    for (int i = 0; i < info.Bubble_Num; i++) {

        if (!bubble[i]->isVisible) //�̹� ���� ����̸�
            continue; //�ؿ��� ����

        bubble[i]->HP -= damage; //�񴰹�￡ ������
        bubble[i]->speed = speed; //�񴰹�� ���ǵ� ����
        bubble[i]->setImage(image); //�񴰹�� �̹��� ����

        if (bubble[i]->HP <= 0)
            bubble[i]->setVisible(false);
            exp += 4;
    }

    return exp;
}

void Stage::deleteBubble(int i) {
    bubble[i]->setVisible(false);
}

void Stage::Bubble_angry(SDL_Rect player) {

    for (int i = 0; i < info.Bubble_Num; i++) {

        if (bubble[i]->getRect().x < player.x)
            bubble[i]->direction[0] = 1;
        else if (bubble[i]->getRect().x > player.x)
            bubble[i]->direction[0] = -1;
        else
            bubble[i]->direction[0] = 0;

        if (bubble[i]->getRect().y < player.y)
            bubble[i]->direction[1] = 1;
        else if (bubble[i]->getRect().y > player.y)
            bubble[i]->direction[1] = -1;
        else
            bubble[i]->direction[1] = 0;

    }
    
}

void Stage::crash(Player& player) {

    for (int i = 0; i < info.Bubble_Num; i++) {

        if (!bubble[i]->isVisible)
            continue;

        SDL_Rect B = bubble[i]->getRect(); //�񴰹���� ��ġ
        SDL_Rect P = player.getRect(); //�÷��̾� 

        if (B.x + 10 < P.x + P.w && B.x + B.w - 10 > P.x &&
            B.y + 10 < P.y + P.h && B.y + B.h - 10 > P.y) { //�浹 Ȯ��

            deleteBubble(i); //�񴰹�� ����

            player.guilty = 3; //��ǳ�� ������ �� ����

            player.rock_spirit -= info.Bubble_Power; //������
            player.exp += 10; //����ġ ȹ��

            cout << "�����Ǹ� : " << player.rock_spirit << endl; //������
            cout << "����ġ : " << player.exp << endl; //������

            Bubble_angry(P); //�񴰹����� �÷��̾�� �𿩵���

            i--;
        }



    }

}

void Stage::get_item(Player& player, SDL_Surface* surface, SDL_Window* window) {

    if (!Item[this->item].isVisible)
        return;

        SDL_Rect I = Item[this->item].rect; //�������� ��ġ
        SDL_Rect P = player.getRect(); //�÷��̾� 

        if (I.x + 10 < P.x + P.w && I.x + I.w - 10 > P.x &&
            I.y + 10 < P.y + P.h && I.y + I.h - 10 > P.y) { //�浹 Ȯ��

            Item[this->item].isVisible = false;

            SDL_Event e;
            bool continueLoop = true;

            if (item == SHAMPOO || item == CANDY) {
                player.rock_spirit += Item[this->item].spirit_up;
                player.throat_health += Item[this->item].health_up;
            }
            else if (item == 5) {
                SDL_BlitSurface(tutorialOpenImage, NULL, surface, NULL);
                SDL_UpdateWindowSurface(window);
                while (continueLoop) { // Ű���� ���������� ������ ����
                    while (SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_KEYDOWN) {
                            if (e.key.keysym.sym == SDLK_SPACE)
                                continueLoop = false;
                                break;
                        }
                    }
                        
                }
            }
            else {
                SDL_BlitSurface(storyImage[item - 2], NULL, surface, NULL);
                SDL_UpdateWindowSurface(window);
                while (continueLoop) { // Ű���� ���������� ������ ����
                    while (SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_KEYDOWN) {
                            if (e.key.keysym.sym == SDLK_SPACE)
                                continueLoop = false;
                                break;
                        }
                    }
                }
            }

            

            cout << "�����Ǹ� : " << player.rock_spirit << endl; //������
            cout << "��ǰ� : " << player.throat_health << endl; //������

        }

}

void Boss_Stage::render(SDL_Surface* surface, SDL_Rect player) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����

    boss->set_direction(player);
    boss->move();
    boss->render(surface);

    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->move(); //�񴰹�� �̵�
    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->render(surface); //�񴰹�� ���
    
}

void Boss_Stage::boss_crash(Player& player) {

        SDL_Rect B = boss->getRect(); //������ ��ġ
        SDL_Rect P = player.getRect(); //�÷��̾� 

        if (B.x < P.x + P.w && B.x + B.w > P.x &&
            B.y < P.y + P.h && B.y + B.h > P.y) { //�浹 Ȯ��

            player.rock_spirit -= info.Bubble_Power; //������

            cout << "�����Ǹ� : " << player.rock_spirit << endl; //������

        }

}

void Boss_Stage::create_bubble() {



    int random; //�񴰹�� ������

    random_device rd; //�õ尪 ��� ���� random_device ����
    mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ
    uniform_int_distribution<int> R(0, 99); // 0~99 ���� ����
    random = R(rd);
        
        
    if (random < 3) {
        for (int i = 0; i < info.Bubble_Num; i++) {
            bubble[i]->setVisible(true);
        }
    }
    
}

int Boss_Stage::updateBubble(int damage, int speed, SDL_Surface* bubble_image) {

    int exp = 0; //������ exp

    for (int i = 0; i < info.Bubble_Num; i++) {

        if (!bubble[i]->isVisible) //�̹� ���� ����̸�
            continue; //�ؿ��� ����

        bubble[i]->HP -= damage; //�񴰹�￡ ������
        bubble[i]->speed = speed; //�񴰹�� ���ǵ� ����
        bubble[i]->setImage(bubble_image); //�񴰹�� �̹��� ����

        if (bubble[i]->HP <= 0)
            bubble[i]->setVisible(false);
        exp += 10;
    }

    boss->HP -= damage; //�������� ������
    boss->speed = speed;

    if (boss->HP <= 0) {
        boss->setVisible(false);
        exp += 50;
    }

    return exp;
}