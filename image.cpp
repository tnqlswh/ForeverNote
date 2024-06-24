#include "image.h"

SDL_Surface* backgroundImage = IMG_Load("background.png");	//배경화면 이미지
SDL_Surface* playerImage[12] = { IMG_Load("player_front.png"), IMG_Load("player_back.png"),
								IMG_Load("player_left.png"), IMG_Load("player_right.png"),
								IMG_Load("player_front_3.png"), IMG_Load("player_back_3.png"),
								IMG_Load("player_left_3.png"), IMG_Load("player_right_3.png"), 
								IMG_Load("player_front_5.png"), IMG_Load("player_back_5.png"),
								IMG_Load("player_left_5.png"), IMG_Load("player_right_5.png")}; //플레이어 이미지
SDL_Surface* bubbleImage = IMG_Load("bubble.png"); //비눗방울 이미지
SDL_Surface* shaking_bubbleImage = IMG_Load("shaking_bubble.png"); //흔들리는 비눗방울 이미지
SDL_Surface* effectImage[3] = {IMG_Load("effect_1.png"), IMG_Load("effect_3.png"), 
								IMG_Load("effect_5.png")}; //이펙트 이미지
SDL_Surface* bossImage[2] = { IMG_Load("middle_boss.png"), IMG_Load("final_boss.png") }; //보스 이미지
SDL_Surface* sick_bossImage[3] = { IMG_Load("sick_middle_boss.png"),
										IMG_Load("sick1_final_boss.png"), IMG_Load("sick2_final_boss.png"), }; //공격받는 보스 이미지
SDL_Surface* endingImage[4] = {IMG_Load("GameOver.png"), IMG_Load("HappyEnding.png"), IMG_Load("Happy.png"), IMG_Load("pung.png") }; //엔딩 이미지
SDL_Surface* statusImage = IMG_Load("state.png"); //스테이터스 창 이미지
SDL_Surface* talkImage[3] = { IMG_Load("talk1.png"), IMG_Load("talk2.png"), IMG_Load("talk3.png") }; //말풍선 이미지
SDL_Surface* itemImage[4] = { IMG_Load("shampoo.png"), IMG_Load("candy.png"), IMG_Load("book.png"), IMG_Load("tutorial.png") }; //아이템 이미지
SDL_Surface* tutorialOpenImage = IMG_Load("tutorial_open.png"); //튜토리얼 얻었을 시 화면 이미지
SDL_Surface* storyImage[3] = { IMG_Load("story1.png"), IMG_Load("story2.png"), IMG_Load("story3.png") }; //스토리 이미지
SDL_Surface* TitleImage = IMG_Load("Title.png"); //시작화면 이미지
SDL_Surface* OpeningImage = IMG_Load("opening.png"); //오프닝 이미지