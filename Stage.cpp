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
    { {600, 400, 90, 90}, itemImage[SHAMPOO], 7, 0, true}, // 샴푸
    { {400, 300, 90, 90}, itemImage[CANDY], 0, 3, true}, // 목캔디
    { {600, 200, 90, 90}, storyImage[0], 0, 0, true}, // 스토리 1
    { {200, 600, 90, 90}, storyImage[1], 0, 0, true}, // 스토리 2
    { {600, 300, 90, 90}, storyImage[2], 0, 0, true}, // 스토리 3
    { {300, 300, 90, 90}, itemImage[3], 0, 0, true} // 튜토리얼
};

Stage::Stage(STAGE_INFO info, int item) {

    this->item = item;

    this->info = info;

    int bx, by = 0; //비눗방울 x축 위치, y축 위치

    random_device rd; //시드값 얻기 위한 random_device 생성
    mt19937 gen(rd()); //난수 생성 엔진 초기화

    for (int i = 0; i < info.Bubble_Num; i++) {
        uniform_int_distribution<int> BX(0, 1080); // 170 ~ 730 사이 정수
        bx = BX(rd);
        uniform_int_distribution<int> BY(0, 555); //42 ~ 360 사이 정수
        by = BY(rd);

        bubble[i] = new Bubble(bubbleImage, info.Bubble_Power, info.Bubble_HP, bx, by); //비눗방울
    }

    isVisible = true;
}

void Stage::render(SDL_Surface* surface) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음

    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->move(); //비눗방울 이동
    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->render(surface); //비눗방울 출력

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

    int exp = 0; //돌려줄 exp

    for (int i = 0; i < info.Bubble_Num; i++) {

        if (!bubble[i]->isVisible) //이미 터진 방울이면
            continue; //밑에거 무시

        bubble[i]->HP -= damage; //비눗방울에 데미지
        bubble[i]->speed = speed; //비눗방울 스피드 설정
        bubble[i]->setImage(image); //비눗방울 이미지 설정

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

        SDL_Rect B = bubble[i]->getRect(); //비눗방울의 위치
        SDL_Rect P = player.getRect(); //플레이어 

        if (B.x + 10 < P.x + P.w && B.x + B.w - 10 > P.x &&
            B.y + 10 < P.y + P.h && B.y + B.h - 10 > P.y) { //충돌 확인

            deleteBubble(i); //비눗방울 터짐

            player.guilty = 3; //말풍선 나오게 할 거임

            player.rock_spirit -= info.Bubble_Power; //데미지
            player.exp += 10; //경험치 획득

            cout << "롹스피릿 : " << player.rock_spirit << endl; //디버깅용
            cout << "경험치 : " << player.exp << endl; //디버깅용

            Bubble_angry(P); //비눗방울들이 플레이어에게 모여들음

            i--;
        }



    }

}

void Stage::get_item(Player& player, SDL_Surface* surface, SDL_Window* window) {

    if (!Item[this->item].isVisible)
        return;

        SDL_Rect I = Item[this->item].rect; //아이템의 위치
        SDL_Rect P = player.getRect(); //플레이어 

        if (I.x + 10 < P.x + P.w && I.x + I.w - 10 > P.x &&
            I.y + 10 < P.y + P.h && I.y + I.h - 10 > P.y) { //충돌 확인

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
                while (continueLoop) { // 키보드 눌릴때까지 보여줄 거임
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
                while (continueLoop) { // 키보드 눌릴때까지 보여줄 거임
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

            

            cout << "롹스피릿 : " << player.rock_spirit << endl; //디버깅용
            cout << "목건강 : " << player.throat_health << endl; //디버깅용

        }

}

void Boss_Stage::render(SDL_Surface* surface, SDL_Rect player) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음

    boss->set_direction(player);
    boss->move();
    boss->render(surface);

    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->move(); //비눗방울 이동
    for (int i = 0; i < info.Bubble_Num; i++) bubble[i]->render(surface); //비눗방울 출력
    
}

void Boss_Stage::boss_crash(Player& player) {

        SDL_Rect B = boss->getRect(); //보스의 위치
        SDL_Rect P = player.getRect(); //플레이어 

        if (B.x < P.x + P.w && B.x + B.w > P.x &&
            B.y < P.y + P.h && B.y + B.h > P.y) { //충돌 확인

            player.rock_spirit -= info.Bubble_Power; //데미지

            cout << "롹스피릿 : " << player.rock_spirit << endl; //디버깅용

        }

}

void Boss_Stage::create_bubble() {



    int random; //비눗방울 만들까말까

    random_device rd; //시드값 얻기 위한 random_device 생성
    mt19937 gen(rd()); //난수 생성 엔진 초기화
    uniform_int_distribution<int> R(0, 99); // 0~99 사이 정수
    random = R(rd);
        
        
    if (random < 3) {
        for (int i = 0; i < info.Bubble_Num; i++) {
            bubble[i]->setVisible(true);
        }
    }
    
}

int Boss_Stage::updateBubble(int damage, int speed, SDL_Surface* bubble_image) {

    int exp = 0; //돌려줄 exp

    for (int i = 0; i < info.Bubble_Num; i++) {

        if (!bubble[i]->isVisible) //이미 터진 방울이면
            continue; //밑에거 무시

        bubble[i]->HP -= damage; //비눗방울에 데미지
        bubble[i]->speed = speed; //비눗방울 스피드 설정
        bubble[i]->setImage(bubble_image); //비눗방울 이미지 설정

        if (bubble[i]->HP <= 0)
            bubble[i]->setVisible(false);
        exp += 10;
    }

    boss->HP -= damage; //보스에게 데미지
    boss->speed = speed;

    if (boss->HP <= 0) {
        boss->setVisible(false);
        exp += 50;
    }

    return exp;
}