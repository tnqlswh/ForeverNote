#include <SDL.h>
#include <SDL_ttf.h>
#include "Status.h"


Status::Status(SDL_Surface* image, int level, int exp, int spirit, int health) {

    rect = { 890, 0, 305, 248 };

    this->image = image;

    this->level = to_string(level);
    this->exp = to_string(exp);
    this->spirit = to_string(spirit);
    this->health = to_string(health);
}


void Status::render(SDL_Surface* surface, TTF_Font* font) {

    SDL_BlitSurface(image, NULL, surface, &rect);

    SDL_Color textColor = { 0, 0, 0 }; // 검정색

    SDL_Surface* textSurface; //텍스트
    SDL_Rect textRect; //텍스트 출력할 위치
    int textWidth, textHeight; //텍스트 가로 세로

    //textRect에 넣을 거임
    int textX = rect.x + 220; 
    int textY = rect.y + 65;

    //레벨 출력
    textSurface = TTF_RenderText_Solid(font, level.c_str(), textColor);
    textRect = { textX , textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);

    //경험치 출력
    textY += 30; // 다음줄
    textSurface = TTF_RenderText_Solid(font, exp.c_str(), textColor);
    textRect = { textX, textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);

    //롹커의 자존심 출력
    textY += 30; // 다음줄
    textSurface = TTF_RenderText_Solid(font, spirit.c_str(), textColor);
    textRect = { textX, textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);

    //목 건강 출력
    textY += 30; // 다음줄
    textSurface = TTF_RenderText_Solid(font, health.c_str(), textColor);
    textRect = { textX, textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    SDL_FreeSurface(textSurface);

}


void Status::setText(int level, int exp, int spirit, int health) {
    this->level = to_string(level);
    this->exp = to_string(exp);
    this->spirit = to_string(spirit);
    this->health = to_string(health);
}
