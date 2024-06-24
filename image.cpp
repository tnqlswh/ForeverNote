#include "image.h"

SDL_Surface* backgroundImage = IMG_Load("background.png");	//���ȭ�� �̹���
SDL_Surface* playerImage[12] = { IMG_Load("player_front.png"), IMG_Load("player_back.png"),
								IMG_Load("player_left.png"), IMG_Load("player_right.png"),
								IMG_Load("player_front_3.png"), IMG_Load("player_back_3.png"),
								IMG_Load("player_left_3.png"), IMG_Load("player_right_3.png"), 
								IMG_Load("player_front_5.png"), IMG_Load("player_back_5.png"),
								IMG_Load("player_left_5.png"), IMG_Load("player_right_5.png")}; //�÷��̾� �̹���
SDL_Surface* bubbleImage = IMG_Load("bubble.png"); //�񴰹�� �̹���
SDL_Surface* shaking_bubbleImage = IMG_Load("shaking_bubble.png"); //��鸮�� �񴰹�� �̹���
SDL_Surface* effectImage[3] = {IMG_Load("effect_1.png"), IMG_Load("effect_3.png"), 
								IMG_Load("effect_5.png")}; //����Ʈ �̹���
SDL_Surface* bossImage[2] = { IMG_Load("middle_boss.png"), IMG_Load("final_boss.png") }; //���� �̹���
SDL_Surface* sick_bossImage[3] = { IMG_Load("sick_middle_boss.png"),
										IMG_Load("sick1_final_boss.png"), IMG_Load("sick2_final_boss.png"), }; //���ݹ޴� ���� �̹���
SDL_Surface* endingImage[4] = {IMG_Load("GameOver.png"), IMG_Load("HappyEnding.png"), IMG_Load("Happy.png"), IMG_Load("pung.png") }; //���� �̹���
SDL_Surface* statusImage = IMG_Load("state.png"); //�������ͽ� â �̹���
SDL_Surface* talkImage[3] = { IMG_Load("talk1.png"), IMG_Load("talk2.png"), IMG_Load("talk3.png") }; //��ǳ�� �̹���
SDL_Surface* itemImage[4] = { IMG_Load("shampoo.png"), IMG_Load("candy.png"), IMG_Load("book.png"), IMG_Load("tutorial.png") }; //������ �̹���
SDL_Surface* tutorialOpenImage = IMG_Load("tutorial_open.png"); //Ʃ�丮�� ����� �� ȭ�� �̹���
SDL_Surface* storyImage[3] = { IMG_Load("story1.png"), IMG_Load("story2.png"), IMG_Load("story3.png") }; //���丮 �̹���
SDL_Surface* TitleImage = IMG_Load("Title.png"); //����ȭ�� �̹���
SDL_Surface* OpeningImage = IMG_Load("opening.png"); //������ �̹���