#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <random>
#include <stdio.h>
#include <windows.h>
#include <thread>

using namespace std;

#include "image.h" //�̹���

#include "Player.h" // �÷��̾�
#include "Stage.h" //��������
#include "Status.h" //�������ͽ� â

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

STAGE_INFO STAGE[10] = {
    // {�񴰹�� ���ݷ�, ����, ü��}
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
    // {���� ���ݷ�, ������ ����� �񴰹�� ����, ���� ü��}
    {1, 1, 50},
    {1, 3, 100}
};

class SDLApp {  //����
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
        SDL_BlitSurface(backgroundImage, NULL, screenSurface, NULL); // ���ȭ�� �׸���

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return;
        }

        TTF_Init(); //��Ʈ

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
        sdlApp.init();  //����

        redColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 0, 0);     //��(�ӽ�)
        greenColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 255, 0);
        blueColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 255);
        grayColor = SDL_MapRGB(sdlApp.screenSurface->format, 128, 128, 128);
        lightGrayColor = SDL_MapRGB(sdlApp.screenSurface->format, 200, 200, 200);
        blackColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 0);
        whiteColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 255, 255);

        font = TTF_OpenFont("C:\\Windows\\Fonts\\H2PORM.TTF", 24);   //��Ʈ, ũ��
        sfont = TTF_OpenFont("C:\\Windows\\Fonts\\H2PORM.TTF", 12);

        createStages(); //�������� ����
    }

    ~Game() {
        sdlApp.close(); //����
    }

    void createStages() {
        stage[0] = new Stage(STAGE[0], 5); //Ʃ�丮��
        stage[1] = new Stage(STAGE[1], CANDY);
        stage[2] = new Stage(STAGE[2], 2); //���丮 1
        stage[3] = new Stage(STAGE[3], SHAMPOO);
        stage[4] = new Stage(STAGE[4], CANDY);
        stage[5] = new Stage(STAGE[5], 3);//���丮 2
        stage[6] = new Stage(STAGE[6], SHAMPOO);
        stage[7] = new Stage(STAGE[7], CANDY);
        stage[8] = new Stage(STAGE[8], 4);//���丮 3
        stage[9] = new Stage(STAGE[9], SHAMPOO);

        boss_stage[0] = new Boss_Stage(BOSS_STAGE[0], bossImage[0]);
        boss_stage[1] = new Boss_Stage(BOSS_STAGE[1], bossImage[1]); //���� ��������
    }

    void render() { //�׸���
        SDL_BlitSurface(backgroundImage, NULL, sdlApp.screenSurface, NULL); //���ȭ��

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
            int T; //���� ���
            int RX; //x��ġ
            int RY; //y��ġ

            random_device rd; //�õ尪 ��� ���� random_device ����
            mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ
            uniform_int_distribution<int> random(0, 2); // 0~2 ���� ����
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
            SDL_BlitSurface(endingImage[0], NULL, sdlApp.screenSurface, NULL); //���ӿ�������
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
                            quit = true; //�����̽�Ű ������ ���� Ż��
                        }
                        break;
                    }
                }

            }
        }
        SDL_UpdateWindowSurface(sdlApp.window);     //ȭ�� ������Ʈ
    }

    bool stage_change() {
        //ȭ�� ���� �����ϸ� ���� ���������� �̵�
        if (player->getRect().x + player->getRect().w > 1200) {
            player->setRect(0, player->getRect().y);
            if (currentStage != 10 && currentStage != 5) { //���� ���������� �������� �̵� �Ұ�
                currentStage++;
                cout << "���� �������� : " << currentStage + 1 << endl; //������
                return true;
            }
        }
        else if (player->getRect().y + player->getRect().h > 675) {
            player->setRect(player->getRect().x, 0);
            if (currentStage != 10 && currentStage != 5) { //���� ���������� �������� �̵� �Ұ�
                currentStage++;
                cout << "���� �������� : " << currentStage + 1 << endl; //������
                return true;
            }
        }
        else if (player->getRect().x < 0) {
            player->setRect(1125, player->getRect().y);
            if (currentStage != 10 && currentStage != 5) { //���� ���������� �������� �̵� �Ұ�
                currentStage++;
                cout << "���� �������� : " << currentStage + 1 << endl; //������
                return true;
            }
        }
        else if (player->getRect().y < 0) {
            player->setRect(player->getRect().x, 558);
            if (currentStage != 10 && currentStage != 5) { //���� ���������� �������� �̵� �Ұ�
                currentStage++;
                cout << "���� �������� : " << currentStage + 1 << endl; //������
                return true;
            }
        }
        return false;
    }

    void run() {
        int getEXP;
        bool is_stage_change;
        

        while (!quit) {
            render(); //�׸���
            
            while (SDL_PollEvent(&e) != 0) { // Ű���� �̺�Ʈ ó��
                if (e.type == SDL_QUIT)
                    quit = true;
                else if (e.type == SDL_KEYDOWN) {  //Ű�� ������ ��
                    if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP ||
                        e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) //����Ű��
                    {
                        player->move(e.key.keysym.sym); //�÷��̾� �̵�
                        player->isRocking = false;
                        if (player->guilty > 0) {
                            player->guilty--;
                        }
                        
                        getEXP = stage[currentStage]->updateBubble(0, 2, bubbleImage);

                        is_stage_change = stage_change();
                    }
                    else if (player->throat_health >= 1 && e.key.keysym.sym == SDLK_d) { //��. �ٵ� ����°� 20 �̻��̾�� ����
                        Mix_PlayChannel(-1, sdlApp.rock, 0);
                        cout << "��!!!!!!!!!!" << endl; //������

                        player->throat_health -= 1; // �� ���� ��ȭ

                        cout << "����� : " << player->throat_health << endl; //������

                        player->isRocking = true;

                        if (currentStage == 5) {
                            getEXP = boss_stage[currentStage - 5]->updateBubble(player->power, 1, shaking_bubbleImage);
                            //��� �񴰹��� ������ ������
                            if (boss_stage[currentStage - 5]->boss->HP <= 25)
                                boss_stage[currentStage - 5]->boss->setImage(sick_bossImage[0]);
                            //���� ü�¿� ���� �̹��� ����

                            player->exp += getEXP;
                        }
                        else if (currentStage == 10) {
                            getEXP = boss_stage[currentStage - 9]->updateBubble(player->power, 1, shaking_bubbleImage);
                            //��� �񴰹��� ������ ������
                            if (boss_stage[currentStage - 9]->boss->HP <= 60)
                                boss_stage[currentStage - 9]->boss->setImage(sick_bossImage[1]);
                            else if (boss_stage[currentStage - 9]->boss->HP <= 30)
                                boss_stage[currentStage - 9]->boss->setImage(sick_bossImage[2]);
                            //���� ü�¿� ���� �̹��� ����

                            player->exp += getEXP;
                        }
                        else if (currentStage >= 0 && currentStage <= 9) {
                            getEXP = stage[currentStage]->updateBubble(player->power, 1, shaking_bubbleImage);
                            //��� �񴰹�￡ ������

                            player->exp += getEXP;
                        }
                        
                        status->setText(player->level, player->exp, player->rock_spirit, player->throat_health);
                    }
                    stage[currentStage]->get_item(*player, sdlApp.screenSurface, sdlApp.window);
                    
                }
                
            }

            if (currentStage == 5) { //�߰� ���� ��������
                boss_stage[currentStage - 5]->boss_crash(*player); //������ �浹 ó��
                boss_stage[currentStage - 5]->crash(*player); //�񴰹��� �浹 ó��
                boss_stage[currentStage - 5]->create_bubble(); //�񴰹�� ����� (Ȯ��������)

                if (boss_stage[currentStage - 5]->boss->HP <= 0) {
                    currentStage++;
                }
                    

            }
            else if (currentStage == 10) { //���� ���� ��������
                boss_stage[currentStage - 9]->boss_crash(*player); //������ �浹 ó��
                boss_stage[currentStage - 9]->crash(*player); //�񴰹��� �浹 ó��
                boss_stage[currentStage - 9]->create_bubble(); //�񴰹�� ����� (Ȯ��������)

                if (boss_stage[currentStage - 9]->boss->HP <= 0) {
                    player->rock_spirit = 1000; //�� ����. ���ǿ���
                }
            }
            else { //�Ϲ� ��������
                stage[currentStage]->crash(*player);
            }

            if (player->exp >= 100 && player->level < 5) { //������
                player->level++;
                player->power += 1;
                cout << "������! " << player->level << "�� ��" << endl; //������
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
                   continueLoop = false; //�ƹ�Ű�� ������ ���� Ż��
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
                    continueLoop = false; //�ƹ�Ű�� ������ ���� Ż��
                    break;
                }
            }

        }
   }


};


int main(int argc, char* args[]) {

        Game game;
        game.opening();
        game.run();    //���� ����
        return 0;

}