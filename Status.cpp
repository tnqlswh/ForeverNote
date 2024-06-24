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

    SDL_Color textColor = { 0, 0, 0 }; // ������

    SDL_Surface* textSurface; //�ؽ�Ʈ
    SDL_Rect textRect; //�ؽ�Ʈ ����� ��ġ
    int textWidth, textHeight; //�ؽ�Ʈ ���� ����

    //textRect�� ���� ����
    int textX = rect.x + 220; 
    int textY = rect.y + 65;

    //���� ���
    textSurface = TTF_RenderText_Solid(font, level.c_str(), textColor);
    textRect = { textX , textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);

    //����ġ ���
    textY += 30; // ������
    textSurface = TTF_RenderText_Solid(font, exp.c_str(), textColor);
    textRect = { textX, textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);

    //��Ŀ�� ������ ���
    textY += 30; // ������
    textSurface = TTF_RenderText_Solid(font, spirit.c_str(), textColor);
    textRect = { textX, textY, 305, 248 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);

    //�� �ǰ� ���
    textY += 30; // ������
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
