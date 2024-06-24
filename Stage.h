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



typedef struct STAGE_INFORMATION { //스테이지에 대한 정보

    int Bubble_Power; //비눗방울 공격력
    int Bubble_Num; //비눗방울 개수
    int Bubble_HP; //비눗방울 체력

}STAGE_INFO;



class Stage{
protected:
    Bubble* bubble[BUBBLE_PCS]; //비눗방울

    STAGE_INFO info; //스테이지 정보

    bool isVisible;

    int item; //스테이지에 있는 아이템 종류 (0 : 샴푸, 1 : 캔디, 2~4: 스토리북, 5:튜토리얼)

public:
    Stage(STAGE_INFO info, int item);

    void render(SDL_Surface* surface); //화면에 출력

    void setVisible(bool visible);

    void crash(Player& player); //비눗방울 충돌시 처리
    void get_item(Player& player, SDL_Surface* surface, SDL_Window* window); //아이템 획득시 처리

    Bubble* getBubble(int i); //비눗방울

    int updateBubble(int damage, int speed, SDL_Surface* image); 
    //모든 비눗방울의 상태 변경 (롹!!!!과 롹 해제 등에서 사용할 것임)
    //플레이어가 받을 exp 리턴

    void deleteBubble(int i); //비눗방울 삭제

    void Bubble_angry(SDL_Rect player); // 비눗방울들이 공격
};


class Boss_Stage : public Stage{

public:
    Boss* boss; //보스

    Boss_Stage(STAGE_INFO info, SDL_Surface* boss_image) : Stage(info, 0) {

        this->info = info;

        boss = new Boss(boss_image, info.Bubble_Power, info.Bubble_HP, 600, 300);

        isVisible = true;

    };

    void create_bubble(); //비눗방울 생산

    void boss_crash(Player& player); //보스 충돌했을 때 처리

    int updateBubble(int damage, int speed, SDL_Surface* bubble_image);
    //모든 비눗방울의 상태 변경 (롹!!!!과 롹 해제 등에서 사용할 것임)
    //플레이어가 받을 exp 리턴

    void render(SDL_Surface* surface, SDL_Rect player); //화면에 출력

};