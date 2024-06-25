#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <random>
#include <stdio.h>
#include <windows.h>
#include <thread>

using namespace std;

#include "image.h" //이미지

#include "Player.h" // 플레이어
#include "Stage.h" //스테이지
#include "Status.h" //스테이터스 창

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

STAGE_INFO STAGE[10] = {
    // {비눗방울 공격력, 개수, 체력}
    {3,3,10},
    {4,4,10},
    {5,5,10},
    {5,6,20},
    {5,7,20},
    {5,8,20},
    {5,9,30},
    {5,11,30},
    {5,13,30},
    {5,15,30}
};

STAGE_INFO BOSS_STAGE[2]{
    // {보스 공격력, 보스가 만드는 비눗방울 개수, 보스 체력}
    {1, 1, 50},
    {1, 3, 100}
};

class SDLApp {  //세팅
public:
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    Mix_Music* gMusic = Mix_LoadMUS("sexy.wav");
    Mix_Chunk* rock;
    

    void init() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return;
        }


        window = SDL_CreateWindow("ForeverNote", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_BlitSurface(backgroundImage, NULL, screenSurface, NULL); // 배경화면 그리기

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return;
        }

        TTF_Init(); //폰트

        rock = Mix_LoadWAV("rock_0.wav");

    }

    void close() {
        Mix_FreeChunk(rock);
        rock = NULL;
        Mix_FreeMusic(gMusic);
        gMusic = NULL;

        SDL_DestroyWindow(window);
        window = NULL;
        TTF_Quit();
        SDL_Quit();
    }

    
    
    
};

class Game {
public:
    SDLApp sdlApp;
    SDL_Event e;
    bool quit = false;

    Uint32 redColor, greenColor, blueColor, grayColor, lightGrayColor, blackColor, whiteColor;
    TTF_Font* font;
    TTF_Font* sfont;

    Player* player = new Player(playerImage[0], 600, 335);
    Stage* stage[10];
    Boss_Stage* boss_stage[2];

    Status* status = new Status(statusImage, player->level, player->exp, player->rock_spirit, player->throat_health);

    int currentStage = 0;

    Game() {
        sdlApp.init();  //시작

        redColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 0, 0);     //색(임시)
        greenColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 255, 0);
        blueColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 255);
        grayColor = SDL_MapRGB(sdlApp.screenSurface->format, 128, 128, 128);
        lightGrayColor = SDL_MapRGB(sdlApp.screenSurface->format, 200, 200, 200);
        blackColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 0);
        whiteColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 255, 255);

        font = TTF_OpenFont("C:\\Windows\\Fonts\\H2PORM.TTF", 24);   //폰트, 크기
        sfont = TTF_OpenFont("C:\\Windows\\Fonts\\H2PORM.TTF", 12);

        createStages(); //스테이지 생성
    }

    ~Game() {
        sdlApp.close(); //종료
    }

    void createStages() {
        stage[0] = new Stage(STAGE[0], 5); //튜토리얼
        stage[1] = new Stage(STAGE[1], CANDY);
        stage[2] = new Stage(STAGE[2], 2); //스토리 1
        stage[3] = new Stage(STAGE[3], SHAMPOO);
        stage[4] = new Stage(STAGE[4], CANDY);
        stage[5] = new Stage(STAGE[5], 3);//스토리 2
        stage[6] = new Stage(STAGE[6], SHAMPOO);
        stage[7] = new Stage(STAGE[7], CANDY);
        stage[8] = new Stage(STAGE[8], 4);//스토리 3
        stage[9] = new Stage(STAGE[9], SHAMPOO);

        boss_stage[0] = new Boss_Stage(BOSS_STAGE[0], bossImage[0]);
        boss_stage[1] = new Boss_Stage(BOSS_STAGE[1], bossImage[1]); //보스 스테이지
    }

    void render() { //그리기
        SDL_BlitSurface(backgroundImage, NULL, sdlApp.screenSurface, NULL); //배경화면

        if (player->isRocking) {
            player->rock(sdlApp.screenSurface);
        }

        player->render(sdlApp.screenSurface);
        SDL_Delay(30);

        if (currentStage == 5)
            boss_stage[currentStage - 5]->render(sdlApp.screenSurface, player->getRect());
        else if(currentStage == 10)
            boss_stage[currentStage - 9]->render(sdlApp.screenSurface, player->getRect());
        else if (currentStage >= 0 && currentStage <= 9)
            stage[currentStage]->render(sdlApp.screenSurface);
        
        if (player->guilty > 0) {
            int T; //무슨 대사
            int RX; //x위치
            int RY; //y위치

            random_device rd; //시드값 얻기 위한 random_device 생성
            mt19937 gen(rd()); //난수 생성 엔진 초기화
            uniform_int_distribution<int> random(0, 2); // 0~2 사이 정수
            T = random(rd);
            uniform_int_distribution<int> random1(-50, 50);
            RX = random1(rd);
            uniform_int_distribution<int> random2(-50, 50);
            RY = random2(rd);

            SDL_Rect talkRect = { player->getRect().x + RX, player->getRect().y + RY};

            SDL_Delay(40);
            SDL_BlitSurface(talkImage[T], NULL, sdlApp.screenSurface, &talkRect);
        }


        status->render(sdlApp.screenSurface, font);

        if (player->rock_spirit <= 0) {
            SDL_BlitSurface(endingImage[0], NULL, sdlApp.screenSurface, NULL); //게임오버엔딩
            Mix_PlayChannel(-1, Mix_LoadWAV("gameover.wav"), 0);

        }
        if(player->rock_spirit == 1000)
        {
            SDL_BlitSurface(endingImage[3], NULL, sdlApp.screenSurface, NULL);
            SDL_UpdateWindowSurface(sdlApp.window);
            Sleep(1000);
            SDL_BlitSurface(endingImage[2], NULL, sdlApp.screenSurface, NULL);
            SDL_UpdateWindowSurface(sdlApp.window);
            Sleep(1000);
            SDL_BlitSurface(endingImage[1], NULL, sdlApp.screenSurface, NULL);
            SDL_UpdateWindowSurface(sdlApp.window);
            Mix_PlayChannel(-1, Mix_LoadWAV("rock_2.wav"), 0);
            while (!quit) {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_SPACE) {
                            Mix_PlayChannel(-1, Mix_LoadWAV("rock_0.wav"), 0);
                            quit = true; //스페이스키 누르면 루프 탈출
                        }
                        break;
                    }
                }

            }
        }
        SDL_UpdateWindowSurface(sdlApp.window);     //화면 업데이트
    }

    bool stage_change() {
        //화면 끝에 도달하면 다음 스테이지로 이동
        if (player->getRect().x + player->getRect().w > 1200) {
            player->setRect(0, player->getRect().y);
            if (currentStage != 10 && currentStage != 5) { //보스 스테이지는 스테이지 이동 불가
                currentStage++;
                cout << "현재 스테이지 : " << currentStage + 1 << endl; //디버깅용
                return true;
            }
        }
        else if (player->getRect().y + player->getRect().h > 675) {
            player->setRect(player->getRect().x, 0);
            if (currentStage != 10 && currentStage != 5) { //보스 스테이지는 스테이지 이동 불가
                currentStage++;
                cout << "현재 스테이지 : " << currentStage + 1 << endl; //디버깅용
                return true;
            }
        }
        else if (player->getRect().x < 0) {
            player->setRect(1125, player->getRect().y);
            if (currentStage != 10 && currentStage != 5) { //보스 스테이지는 스테이지 이동 불가
                currentStage++;
                cout << "현재 스테이지 : " << currentStage + 1 << endl; //디버깅용
                return true;
            }
        }
        else if (player->getRect().y < 0) {
            player->setRect(player->getRect().x, 558);
            if (currentStage != 10 && currentStage != 5) { //보스 스테이지는 스테이지 이동 불가
                currentStage++;
                cout << "현재 스테이지 : " << currentStage + 1 << endl; //디버깅용
                return true;
            }
        }
        return false;
    }

    void run() {
        int getEXP;
        bool is_stage_change;
        

        while (!quit) {
            render(); //그리기
            
            while (SDL_PollEvent(&e) != 0) { // 키보드 이벤트 처리
                if (e.type == SDL_QUIT)
                    quit = true;
                else if (e.type == SDL_KEYDOWN) {  //키가 눌렸을 때
                    if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP ||
                        e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) //방향키면
                    {
                        player->move(e.key.keysym.sym); //플레이어 이동
                        player->isRocking = false;
                        if (player->guilty > 0) {
                            player->guilty--;
                        }
                        
                        getEXP = stage[currentStage]->updateBubble(0, 2, bubbleImage);

                        is_stage_change = stage_change();
                    }
                    else if (player->throat_health >= 1 && e.key.keysym.sym == SDLK_d) { //롹. 근데 목상태가 20 이상이어야 가능
                        Mix_PlayChannel(-1, sdlApp.rock, 0);
                        cout << "롹!!!!!!!!!!" << endl; //디버깅용

                        player->throat_health -= 1; // 목 상태 악화

                        cout << "목상태 : " << player->throat_health << endl; //디버깅용

                        player->isRocking = true;

                        if (currentStage == 5) {
                            getEXP = boss_stage[currentStage - 5]->updateBubble(player->power, 1, shaking_bubbleImage);
                            //모든 비눗방울과 보스에 데미지
                            if (boss_stage[currentStage - 5]->boss->HP <= 25)
                                boss_stage[currentStage - 5]->boss->setImage(sick_bossImage[0]);
                            //보스 체력에 따른 이미지 변경

                            player->exp += getEXP;
                        }
                        else if (currentStage == 10) {
                            getEXP = boss_stage[currentStage - 9]->updateBubble(player->power, 1, shaking_bubbleImage);
                            //모든 비눗방울과 보스에 데미지
                            if (boss_stage[currentStage - 9]->boss->HP <= 60)
                                boss_stage[currentStage - 9]->boss->setImage(sick_bossImage[1]);
                            else if (boss_stage[currentStage - 9]->boss->HP <= 30)
                                boss_stage[currentStage - 9]->boss->setImage(sick_bossImage[2]);
                            //보스 체력에 따른 이미지 변경

                            player->exp += getEXP;
                        }
                        else if (currentStage >= 0 && currentStage <= 9) {
                            getEXP = stage[currentStage]->updateBubble(player->power, 1, shaking_bubbleImage);
                            //모든 비눗방울에 데미지

                            player->exp += getEXP;
                        }
                        
                        status->setText(player->level, player->exp, player->rock_spirit, player->throat_health);
                    }
                    stage[currentStage]->get_item(*player, sdlApp.screenSurface, sdlApp.window);
                    
                }
                
            }

            if (currentStage == 5) { //중간 보스 스테이지
                boss_stage[currentStage - 5]->boss_crash(*player); //보스와 충돌 처리
                boss_stage[currentStage - 5]->crash(*player); //비눗방울과 충돌 처리
                boss_stage[currentStage - 5]->create_bubble(); //비눗방울 만들기 (확률적으로)

                if (boss_stage[currentStage - 5]->boss->HP <= 0) {
                    currentStage++;
                }
                    

            }
            else if (currentStage == 10) { //최종 보스 스테이지
                boss_stage[currentStage - 9]->boss_crash(*player); //보스와 충돌 처리
                boss_stage[currentStage - 9]->crash(*player); //비눗방울과 충돌 처리
                boss_stage[currentStage - 9]->create_bubble(); //비눗방울 만들기 (확률적으로)

                if (boss_stage[currentStage - 9]->boss->HP <= 0) {
                    player->rock_spirit = 1000; //롹 각성. 해피엔딩
                }
            }
            else { //일반 스테이지
                stage[currentStage]->crash(*player);
            }

            if (player->exp >= 100 && player->level < 5) { //레벨업
                player->level++;
                player->power += 1;
                cout << "레벨업! " << player->level << "이 됨" << endl; //디버깅용
                player->exp -= 100;
            }
            if (player->level == 3) {
                sdlApp.rock = Mix_LoadWAV("rock_1.wav");
            }
            else if (player->level == 4) {
                sdlApp.rock = Mix_LoadWAV("rock_2.wav");
            }
            else if (player->level == 5) {
                sdlApp.rock = Mix_LoadWAV("rock_3.wav");
                player->exp = 0;
            }
            
            status->setText(player->level, player->exp, player->rock_spirit, player->throat_health);
        }
    }

    void opening() {
        SDL_BlitSurface(TitleImage, NULL, sdlApp.screenSurface, NULL);
        SDL_UpdateWindowSurface(sdlApp.window);

        bool continueLoop = true;
        

        while (continueLoop) { 
           while (SDL_PollEvent(&e) != 0)
           {
               if (e.type == SDL_KEYDOWN) {
                   continueLoop = false; //아무키나 누르면 루프 탈출
                   break;
               }
           }

       }

        continueLoop = true;

        SDL_BlitSurface(OpeningImage, NULL, sdlApp.screenSurface, NULL);
        SDL_UpdateWindowSurface(sdlApp.window);

        while (continueLoop) { 
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_KEYDOWN) {
                    continueLoop = false; //아무키나 누르면 루프 탈출
                    break;
                }
            }

        }
   }


};


int main(int argc, char* args[]) {

        Game game;
        game.opening();
        game.run();    //게임 실행
        return 0;

}