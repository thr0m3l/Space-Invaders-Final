//Version 1.3 Final

/*
Bangladesh University of Engineering and Technology
Department of Computer Science and Engineering
Level 1, Term 1
CSE 102 iGraphics Term Project

Section: B-1
Group: 04

Project Title: Space Invaders
Project Started on: July 5th, 2018
Project Submitted on: July 22nd, 2018
Group Members:
1. Nishat Farhana Purbasha (1705067)
2. Saadman Ahmed (1705068)
3. Tanzim Hossain Romel (1705069)

Project Supervisor:
Dr. Md. Shohrab Hossain
Associate Professor,
Department of Computer Science and Engineering,
Bangladesh University of Engineering and Technology
*/

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define NUMBEROFENEMY 400
#define NUMBEROFCANNON 500
#define NUMBEROFBOSSCANNON 200
#define screenheight 552
#define screenwidth 910
#define NUMBEROFBONUS 20
#pragma warning(disable : 4996)

int mposx,  mposy;
int cannon_count = 0, temp = 20, temp1 = 0;
char str[30],str2[30];
int len = 0, max = 0, state;
int on = 0;
int laser_on = 6, wave_on = 6, inv_count=0;
/*modes:
0 main menu
1 new game
2 load game
3 leaderboard
4 credits
5 exit

10 pause menu
11 game over
12 instructions
13 level 2 complete

*/
int mode = 0;

struct wave{
	int x;
	int y;
}wave;
struct highscore{
	char name[30];
	int score;
}highscore[1000];

struct line{
	int latest_enemy = 0;
	int latest_cannon = 0;
	int y;
	int cannon_count = 0;
	int enemycannon_count = 0;
}line[6];


struct player{
	int x=20;
	int y=20;
	int score = 0;
	int lives = 5;
	int pause = 0;
	int kills = 0;
	int invincible = 0;
	int shockwave = 2;
	int laser = 2;
	int difficulty = 1;
	int invincibilty = 1;
}player;

struct enemy{
	int x;
	int y;
	int show;
	int type;

}enemy[6][NUMBEROFENEMY];

struct cannon{
	int x;
	int y;
	int show;
	int active;
}cannon[6][NUMBEROFCANNON];

struct enemy_cannon{
	int x;
	int y;
	int show = 0;
	int detached=0;

}enemy_cannon[6][NUMBEROFENEMY];

struct boss{
	float x;
	int y;
	int life;

}boss[2];

struct bonus{
	int x=-100;
	int y=0;
	int type;
}bonus[6][NUMBEROFBONUS];

struct boss_cannon
{
	float x;
	int y;
	int show = 0;
	int detached = 0;

}boss_cannon[6][NUMBEROFENEMY];

void instructions()
{
	if (mode == 12)
	{
		iShowBMP(0, 0, "instructions.bmp");
	}
}
void sortleaderboard()
{
	for (int i = 0; i < state - 1; i++){
		for (int j = i + 1; j < state; j++){
			if (highscore[i].score < highscore[j].score){
				int temp;
				temp = highscore[i].score;
				highscore[i].score = highscore[j].score;
				highscore[j].score = temp;
				//
				char temp3[100];
				strcpy(temp3, highscore[i].name);
				strcpy(highscore[i].name, highscore[j].name);
				strcpy(highscore[j].name, temp3);
			}
		}
	}
}
void updateleaderboard()
{
	sortleaderboard();
	FILE *fp;
	fp = fopen("score.txt", "w");
	for (int i = 0; i < state; i++){
		fprintf(fp, "%s %d\n", highscore[i].name, highscore[i].score);
	}
	fclose(fp);
	fp = fopen("state.txt", "w");
	fprintf(fp,"%d", state);
	fclose(fp);
}

void leaderboard()
{
	FILE *fp;
	fp = fopen("state.txt", "r");
	fscanf(fp, "%d", &state);
	fclose(fp);
	fp = fopen("score.txt", "r");
	for (int i = 0; i < state; i++){
		fscanf(fp, "%s %d", highscore[i].name, &highscore[i].score);
	}
}
void showleaderboard()
{
	if (mode == 3){
		iShowBMP(0, 0, "Leaderboard.bmp");
		int k = 10, x = 82, y = 350;
		if (state < 10) k = state;
		for (int i = 0; i < k; i++, y -= 30){
			char temp4[10], temp5[5];
			_itoa(highscore[i].score, temp4, 10);
			_itoa(i + 1, temp5, 10);
			iSetColor(136, 0, 27);
			iText(161, y, temp5, GLUT_BITMAP_TIMES_ROMAN_24);
			iText(325, y, highscore[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
			iText(515, y, temp4, GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
}
void savegame()
{
	FILE *fp;
	//saving player
	fp = fopen("player.txt", "w");
	fprintf(fp, "%d %d %d %d %d %d %d %d %d %d\n", player.x, player.y, player.score, player.lives, player.kills, player.difficulty, player.invincible, player.laser, player.shockwave, player.pause);
	fclose(fp);

	//saving enemy
	fp = fopen("enemy.txt", "w");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY; j++){
			fprintf(fp, "%d %d %d %d\n", enemy[i][j].x, enemy[i][j].y, enemy[i][j].show, enemy[i][j].type);
		}
	}
	fclose(fp);
	//saving enemy-cannon
	fp = fopen("enemy_cannon.txt", "w");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY; j++){
			fprintf(fp, "%d %d %d %d\n", enemy_cannon[i][j].x, enemy_cannon[i][j].y, enemy_cannon[i][j].show, enemy_cannon[i][j].detached);
		}
	}
	fclose(fp);

	//saving bonus
	fp = fopen("bonus.txt", "w");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFBONUS; j++){
			fprintf(fp, "%d %d %d\n", bonus[i][j].x, bonus[i][j].y, bonus[i][j].type);
		}
	}
	fclose(fp);

	//saving player_cannon
	fp = fopen("cannon.txt", "w");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFCANNON; j++){
			fprintf(fp, "%d %d %d %d\n", cannon[i][j].x, cannon[i][j].y, cannon[i][j].show, cannon[i][j].active);
		}
	}
	fclose(fp);

	//saving mode
	fp = fopen("mode.txt", "w");
	fprintf(fp, "%d\n", mode);

	//saving boss
	fp = fopen("boss.txt", "w");
	fprintf(fp, "%f %d %d", boss[0].x, boss[0].y, boss[0].life);
	fclose(fp);

	fp = fopen("boss_cannon.txt", "w");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY; j++){
			fprintf(fp, "%f %d %d %d\\n", boss_cannon[i][j].x, boss_cannon[i][j].y, boss_cannon[i][j].show, boss_cannon[i][j].detached);
		}
	}
	fclose(fp);
}

void loadgame()
{
	FILE *fp;
	//saving player
	fp = fopen("player.txt", "r");
	fscanf(fp, "%d %d %d %d %d %d %d %d %d %d\n", &player.x, &player.y, &player.score, &player.lives, &player.kills, &player.difficulty, &player.invincible, &player.laser, &player.shockwave, &player.pause);
	fclose(fp);

	//saving enemy
	fp = fopen("enemy.txt", "r");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY; j++){
			fscanf(fp, "%d %d %d %d\n", &enemy[i][j].x, &enemy[i][j].y, &enemy[i][j].show, &enemy[i][j].type);
		}
	}
	fclose(fp);
	//saving enemy-cannon
	fp = fopen("enemy_cannon.txt", "r");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY; j++){
			fscanf(fp, "%d %d %d %d\n", &enemy_cannon[i][j].x, &enemy_cannon[i][j].y, &enemy_cannon[i][j].show, &enemy_cannon[i][j].detached);
		}
	}
	fclose(fp);

	//saving bonus
	fp = fopen("bonus.txt", "r");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFBONUS; j++){
			fscanf(fp, "%d %d %d\n", &bonus[i][j].x, &bonus[i][j].y, &bonus[i][j].type);
		}
	}
	fclose(fp);

	//saving player_cannon
	fp = fopen("cannon.txt", "r");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFCANNON; j++){
			fscanf(fp, "%d %d %d %d\n", &cannon[i][j].x, &cannon[i][j].y, &cannon[i][j].show, &cannon[i][j].active);
		}
	}
	fclose(fp);

	//saving boss
	fp = fopen("boss.txt", "r");
	fscanf(fp, "%f %d %d", &boss[0].x, &boss[0].y, &boss[0].life);
	fclose(fp);

	fp = fopen("boss_cannon.txt", "r");
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY; j++){
			fscanf(fp, "%f %d %d %d\\n", &boss_cannon[i][j].x, &boss_cannon[i][j].y, &boss_cannon[i][j].show, &boss_cannon[i][j].detached);
		}
	}
	fclose(fp);
}
void gameover()
{
	if (player.lives <= 0 && mode == 1){
		mode = 11;
		player.lives = 5;
	}
	if (mode == 11){

		iShowBMP(0, 0, "game_over.bmp");

		iText(340, 343, "Your  Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
		char score[10];
		_itoa(player.score, score, 10);
		iText(490, 343, score, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(392, 181, str, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}
void bonusinitialize()
{
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFBONUS; j++){
			bonus[i][j].x = screenwidth + rand() % 70000;
			bonus[i][j].y = line[i].y;
			bonus[i][j].type = rand() % 5;
			//printf("%d %d %d %d %d\n", i, j, bonus[i][j].x, bonus[i][j].y, line[i].y);
		}
	}
}
void credits()
{
	if (mode == 4){
		iShowBMP(0, 0, "credits.bmp");
	}
}
void pausemenu()
{
	if (mode == 10){
		iShowBMP(0, 0, "pause.bmp");
	}
}
void scoreboard()
{
	if (mode == 1){
		iSetColor(136 ,0 , 27);
		iText(20, screenheight-30, "Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
		char score[10],lives[10],shockwave[10];
		_itoa(player.score, score, 10);
		iText(100, screenheight - 30, score, GLUT_BITMAP_TIMES_ROMAN_24);
		_itoa(player.lives, lives, 10);
		iText(200, screenheight - 30, "Lives: ", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(300, screenheight - 30, lives, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(700, screenheight - 30, "Press P to Pause", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}
void menu()
{
	if (mode == 0){
		iShowBMP(0, 0, "menu.bmp");
	}
}
void setenemycannonvariables()
{
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < NUMBEROFENEMY;  j++){
			if (j % 5 == 0 && !enemy_cannon[i][j].detached){
				enemy_cannon[i][j].x = enemy[i][j].x;
				enemy_cannon[i][j].y = enemy[i][j].y;
				enemy_cannon[i][j].show = 0;
			}
		}
	}
}
void setbosscannonvariables()
{
	for (int i = 0; i < NUMBEROFENEMY; i++){
		for (int j = 0; j < 6; j++){
			boss_cannon[j][i].x = screenwidth + rand() % 100000;
			boss_cannon[j][i].y = line[j].y;
			boss_cannon[j][i].detached = 0;

		}
	}
}
void bosscannonmovement()
{
	if (player.kills >= 300 && player.kills <= 400 && mode == 1)
	{
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < NUMBEROFBOSSCANNON; j++){
				if (boss_cannon[i][j].show){
					iShowBMP2(boss_cannon[i][j].x, boss_cannon[i][j].y - 15, "bomb.bmp", 0);
				}
			}
		}
	}
	if (player.kills >= 800 && player.kills < 1000 && mode == 1){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < NUMBEROFBOSSCANNON; j++){
				if (boss_cannon[i][j].show){
					iShowBMP2(boss_cannon[i][j].x, boss_cannon[i][j].y - 15, "bomb.bmp", 0);
				}
			}
		}
	}
}
void enemycannonmovement()
{
	if (player.kills < 300 || (player.kills > 400 && player.kills <800) && (mode == 1))
	{
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < NUMBEROFENEMY; j++){
				if (j == 0 && (player.kills > 400 && player.kills < 800)) continue;
				if (j % 5 == 0 && enemy_cannon[i][j].x <= screenwidth - 50){
					enemy_cannon[i][j].detached = 1;
					enemy_cannon[i][j].show = 1;
				}
				if (enemy_cannon[i][j].show){
					iShowBMP2(enemy_cannon[i][j].x-10, enemy_cannon[i][j].y - 15, "enemy_cannon.bmp", 0);
				}
				if (enemy_cannon[i][j].x <= -500) {
					enemy_cannon[i][j].show = 0;
					enemy_cannon[i][j].detached = 0;
				}
			}
		}
	}
}
void bonusmovement()
{
	if (player.kills < 300 ||(player.kills > 400 && player.kills<800) && mode == 1){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < NUMBEROFBONUS; j++){
				if (bonus[i][j].type == 0){
					iShowBMP2(bonus[i][j].x, bonus[i][j].y, "bonuscoin.bmp", 0);
				}
				else if (bonus[i][j].type == 1){
					iShowBMP2(bonus[i][j].x, bonus[i][j].y, "bonushealth.bmp", 0);
				}
				else if (bonus[i][j].type == 2){

				}
				else if (bonus[i][j].type == 3){

				}
				else if (bonus[i][j].type == 4){

				}
			}
		}
	}
}
void lineinitialize()
{
	for (int i = 0; i < 6; i++){
		line[i].y = temp;
		line[i].latest_cannon = 0;
		line[i].latest_enemy = 0;
		temp += 90;
	}
}
void drawplayer()
{
	if (mode == 1 && !player.invincible && player.kills <400) iShowBMP2(player.x - 10, player.y - 10, "ship1.bmp", 0);
	if (mode == 1 && player.invincible && player.kills <400 ) iShowBMP2(player.x - 10, player.y - 10, "ship2.bmp", 0);
	if (mode == 1 && !player.invincible && player.kills >= 400 && player.kills <= 1000) iShowBMP2(player.x-30, player.y-20, "ship3.bmp", 0);
	if (mode == 1 && player.invincible && player.kills >= 400 && player.kills <= 1000) iShowBMP2(player.x - 30, player.y - 20, "ship4.bmp", 0);
}
void enemymovement()
{
	//level 1
	if (mode == 1 && player.kills<300 && player.lives>0){
		for (int i = 0; i < NUMBEROFENEMY; i++){
			for (int j = 0; j < 6; j++){
				if (enemy[j][i].show){
					if (i % 5 == 0) iShowBMP2(enemy[j][i].x - 10, enemy[j][i].y - 10, "enemy_ship.bmp", 0);
					else iShowBMP2(enemy[j][i].x, enemy[j][i].y, "enemy_ship2.bmp", 0);
				}
			}
		}
	}

	//level 2
	if (mode == 1 && player.kills>400 && player.kills<800 && player.lives>0){
		for (int i = 0; i < NUMBEROFENEMY; i++){
			for (int j = 0; j < 6; j++){
				if (enemy[j][i].show){
					if (i % 5 == 0) iShowBMP2(enemy[j][i].x - 10, enemy[j][i].y-15, "enemy_ship4.bmp", 0);
					else iShowBMP2(enemy[j][i].x, enemy[j][i].y-15, "enemy_ship3.bmp", 0);
				}
			}
		}
	}
}
void setcannonvariables()
{
	for (int i = 0; i < NUMBEROFCANNON; i++){
		for (int j = 0; j < 6; j++) {
			cannon[j][i].active = 0;
			cannon[j][i].show = 0;
		}
	}
}
void cannonmovement()
{
	if (mode == 1){
		for (int i = 0; i < NUMBEROFCANNON; i++){
			for (int j = 0; j < 6; j++){
				if (cannon[j][i].show){
					if(player.kills<400) iShowBMP2(cannon[j][i].x, cannon[j][i].y, "friendly_beam.bmp", 0);
					if (player.kills >= 400) iShowBMP2(cannon[j][i].x, cannon[j][i].y, "friendly_beam3.bmp", 0);
				}
			}
		}
	}
}
void shockwavecounter()
{
	if(wave_on<10) wave_on++;
}
void shockwave(){
	if (player.shockwave){
		wave_on = 0;
		wave.x = player.x;
		wave.y = 0;
		int kill[6] = { 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < NUMBEROFENEMY; j++){
				if (enemy[i][j].x <= screenwidth && enemy[i][j].x >= 0){
					enemy[i][j].x = 12000;
					kill[i]++;
					player.score += 20;
					if (kill[i] == 15) break;
				}
			}
		}
		player.shockwave--;
	}
}
void lasercounter()
{
	if(laser_on <10)laser_on++;
}

void laser()
{
	int count=0;
	if (player.laser){
		laser_on = 0;
		for (int i = 0; i < 6; i++){
			if (player.y == line[i].y){
				for (int j = 0; j < NUMBEROFENEMY; j++){
					if (enemy[i][j].x >= 0 && enemy[i][j].x <= screenwidth){
						enemy[i][j].x = 10000;
						player.score += 20;
					}

					if (count >= 50) break;
				}
			}
		}
		player.laser--;
	}
}

void detectcollision()
{
	if ((player.kills <= 300 || (player.kills > 400 && player.kills < 800 )) && mode == 1){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j <= line[i].cannon_count; j++){
				for (int k = 0; k < NUMBEROFENEMY; k++){
					if ((cannon[i][j].x + 25 >= enemy[i][k].x) && (abs(cannon[i][j].y - enemy[i][k].y) == 0) && (cannon[i][j].x >= 0) && (cannon[i][j].x <= screenwidth)){
						enemy[i][k].x = 18000;
						cannon[i][j].y = 1500;
						if(k%10==0)player.score += 20;
						else player.score += 10;
						player.kills++;
						break;
					}

					//enemy-player collision
					if ((enemy[i][k].x - player.x <= 5) && (enemy[i][k].y == player.y) && !player.invincible){
						enemy[i][k].y = 5000 + rand() % 1000;
						player.lives--;
					}
					//enemy_cannon - cannon collision
					if ((enemy_cannon[i][k].x - cannon[i][j].x <= 5) && (enemy_cannon[i][k].y == cannon[i][j].y)){
						cannon[i][j].y = 1500;
						enemy_cannon[i][k].y = -1000;
					}

				}

			}
			//player-enemy_cannon collision
			for (int k = 0; k<NUMBEROFENEMY; k++){
				if ((enemy_cannon[i][k].x - player.x <= 10)&& (enemy_cannon[i][k].x >= player.x) && (enemy_cannon[i][k].y == player.y) && !player.invincible){
					enemy_cannon[i][k].x = -1000;
					player.lives--;
				}
			}
			for (int k = 0; k<NUMBEROFBONUS; k++){
				if ((bonus[i][k].x - player.x <= 5) && (bonus[i][k].y == player.y)){
					bonus[i][k].y = -1000;
					if (bonus[i][k].type == 0){
						player.score += 250;
					}
					if (bonus[i][k].type == 1){
						player.lives++;
					}
				}
			}
		}
	}
	else if ((player.kills > 300 && player.kills <= 400) && boss[0].life && mode==1){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j <= line[i].cannon_count; j++){
				if ((boss[0].x >= cannon[i][j].x) && (boss[0].x - cannon[i][j].x <= 25) && (cannon[i][j].y - boss[0].y <= 250) && (cannon[i][j].y >= boss[0].y) && (cannon[i][j].x >= 0) && (cannon[i][j].x <= screenwidth)){
					cannon[i][j].y = 1500;
					player.kills++;
					boss[0].life--;
					player.score += 50;
				}
				for (int k = 0; k <= NUMBEROFBOSSCANNON; k++){
					if ((boss_cannon[i][k].x - cannon[i][j].x <= 20) && (boss_cannon[i][k].x >= cannon[i][j].x) && (boss_cannon[i][k].y == cannon[i][j].y)){
						cannon[i][j].y = 1500;
						boss_cannon[i][k].x = screenwidth + rand() % 10000;
						break;
					}
				}
			}
			for (int k = 0; k <= NUMBEROFBOSSCANNON; k++){
				if ((boss_cannon[i][k].x - player.x <= 15) && (boss_cannon[i][k].x >= player.x) && (boss_cannon[i][k].y == player.y) && !player.invincible && boss_cannon[i][k].show){
					boss_cannon[i][k].x = screenwidth + rand() % 100000;
					player.lives--;
				}
			}
		}
	}
	else if ((player.kills >= 800 && player.kills <= 1000) && boss[1].life && mode == 1){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j <= line[i].cannon_count; j++){
				if (((boss[1].x >= cannon[i][j].x) && boss[1].x - cannon[i][j].x <= 25) && (cannon[i][j].y - boss[1].y <= 250) && (cannon[i][j].y >= boss[1].y) && (cannon[i][j].x >= 0) && (cannon[i][j].x <= screenwidth)){
					cannon[i][j].y = 1500;
					player.kills++;
					boss[1].life--;
					player.score += 100;
				}
				for (int k = 0; k <= NUMBEROFBOSSCANNON; k++){
					if ((boss_cannon[i][k].x - cannon[i][j].x <= 20) && (boss_cannon[i][k].x >= cannon[i][j].x) && (boss_cannon[i][k].y == cannon[i][j].y)){
						cannon[i][j].y = 1500;
						boss_cannon[i][k].x = screenwidth + rand() % 10000;
						break;
					}
				}
			}
			for (int k = 0; k <= NUMBEROFBOSSCANNON; k++){
				if ((boss_cannon[i][k].x - player.x <= 15) && (boss_cannon[i][k].x >= player.x) && (boss_cannon[i][k].y == player.y) && !player.invincible){
					boss_cannon[i][k].x = screenwidth + rand() % 12000;
					player.lives--;
				}
			}
		}
	}
}
void levelcomplete()
{
	if (player.kills >= 1000 && mode == 1){
		mode = 13;
	}
	if (mode == 13)
	{
		iShowBMP(0, 0, "game_end.bmp");
		iText(400, 150, str, GLUT_BITMAP_TIMES_ROMAN_24);
	}

}
void change()
{
	//change in enemy's position
	if (!player.pause && mode==1 ){
		if(player.kills<=300 ||(player.kills>400 && player.kills<800)){
			for (int j = 0; j < 6; j++){
				for (int i = 0; i < NUMBEROFENEMY; i++){
					if(player.kills<100) enemy[j][i].x -= 4;
					else if (player.kills<200) enemy[j][i].x -= 5;
					else if (player.kills<300) enemy[j][i].x -= 7;
					else if (player.kills<600) enemy[j][i].x -= 8;
					else if (player.kills<750) enemy[j][i].x -= 9;
					else if (player.kills<800) enemy[j][i].x -= 10;
					if (enemy[j][i].x <= 0){
						enemy[j][i].x = 8000;
						player.score--;
					}
					if (enemy[j][i].x >= 1500) enemy[j][i].show = 0;
					if (enemy[j][i].x <= 1500) enemy[j][i].show = 1;
				}
				for (int i = 0; i < NUMBEROFBONUS; i++){
					bonus[j][i].x -= 10;
				}
			}
		}

		//change in cannon's position
		for (int i = 0; i < NUMBEROFCANNON; i++){
			for (int j = 0; j < 6; j++){
				if(player.score<=2000) cannon[j][i].x += 5;
				else if (player.score <= 4000 && player.score>2000) cannon[j][i].x += 7;
				else if (player.score <= 6000 && player.score>4000) cannon[j][i].x += 10;
				else if (player.score >= 6000) cannon[j][i].x += 12;
				if (cannon[j][i].x >= screenwidth) cannon[j][i].show = 0;
			}
		}
		if (player.kills < 300 || (player.kills > 400 && player.kills <800)){


			for (int i = 0; i < 6; i++){
				for (int j = 0; j<NUMBEROFENEMY; j++){
					if (enemy_cannon[i][j].detached) enemy_cannon[i][j].x -= 10;
				}
			}
		}
		if (player.kills >= 300 && player.kills < 400)
		{
			if (boss[0].x <= 0) mode = 11;
			boss[0].x -= 0.2;
			for (int i = 0; i < 6; i++){
				for (int j = 0; j<NUMBEROFBOSSCANNON; j++){
					boss_cannon[i][j].x -= 10;

					if (boss_cannon[i][j].x <= 800 && !boss_cannon[i][j].detached && boss_cannon[i][j].y >= boss[0].y && boss_cannon[i][j].y - boss[0].y <= 300){
						boss_cannon[i][j].x = boss[0].x;
						boss_cannon[i][j].show = 1;
						boss_cannon[i][j].detached = 1;
					}
					if (boss_cannon[i][j].x <= -500) {
					boss_cannon[i][j].show = 0;
					boss_cannon[i][j].detached = 0;
					}

				}
			}
		}
		if (player.kills >= 800 && player.kills < 1000)
		{
			if (boss[1].x <= 0) mode = 11;
			boss[1].x -= 0.4;
			for (int i = 0; i < 6; i++){
				for (int j = 0; j<NUMBEROFBOSSCANNON; j++){
					boss_cannon[i][j].x -= 8;
					if (boss_cannon[i][j].x <= 1200 && !boss_cannon[i][j].detached && boss_cannon[i][j].y >= boss[1].y && boss_cannon[i][j].y - boss[1].y <= 300){
						boss_cannon[i][j].x = boss[1].x;
						boss_cannon[i][j].show = 1;
						boss_cannon[i][j].detached = 1;
					}
					if (boss_cannon[i][j].x <= -50000) {
						boss_cannon[i][j].show = 0;
						boss_cannon[i][j].detached = 0;
					}

				}
			}
		}
		if (player.kills <= 300 || player.kills>400){
			for (int i = 0; i < 6; i++){
				for (int j = 0; j<NUMBEROFBONUS; j++){
					bonus[i][j].x -= 5;
				}
			}
		}
		if(wave.x >= 0 && wave.x <=1200)wave.x += 50;
	}
}

void setEnemyvariables()
{
	if (player.kills < 300){
		for (int i = 0; i < NUMBEROFENEMY; i++){
			for (int j = 0; j < 6; j++){
				enemy[j][i].x = screenwidth + rand() % 100000;
				enemy[j][i].y = line[j].y;
				enemy[j][i].show = 1;

			}
		}
	}
	if (player.kills >= 400){
		for (int i = 0; i < NUMBEROFENEMY; i++){
			for (int j = 0; j < 6; j++){
				enemy[j][i].x = screenwidth + rand() % 95000;
				enemy[j][i].y = line[j].y;
				enemy[j][i].show = 1;

			}
		}
	}
}
void bossmovement()
{

	if (player.kills > 300 && player.kills <400 && mode == 1)
	{
		iShowBMP2(boss[0].x, boss[0].y, "boss.bmp", 0);
	}
	if (player.kills >= 800 && player.kills < 1000 && mode == 1){
		iShowBMP2(boss[1].x, boss[1].y, "boss2.bmp", 0);
	}
}
void bossinitialize()
{
	boss[0].x = screenwidth - 400;
	boss[0].y = screenheight / 2 - 100;
	boss[0].life = 100;

	boss[1].x = screenwidth - 350;
	boss[1].y = screenheight / 2 - 100;
	boss[1].life = 200;
}

void invincibilitycounter()
{
	if (player.invincible) inv_count++;
	if (inv_count >= 20) player.invincibilty=0;
	if (!player.invincibilty) player.invincible = 0;
}
void bosschange()
{
	boss[0].y = rand() % (screenheight-300);

	boss[1].y = rand() % (screenheight - 200);
	//not related to boss
	lasercounter();
	shockwavecounter();
	invincibilitycounter();

}
void initializer()
{
	bossinitialize();
	if (!on)lineinitialize();
	bonusinitialize();
	setEnemyvariables();
	setcannonvariables();
	setbosscannonvariables();
	player.lives = 5;
	if(player.kills<300)player.score = 0;
	player.x = 20;
	player.y = 20;
	player.shockwave = 2;
	player.laser = 2;
	player.invincibilty = 1;
	player.invincible = 0;
}
int level = 0;
void level2()
{
	if (player.kills == 400 && level == 0 ){
		level = 1;
		initializer();
		player.invincibilty += 4;
		player.laser += 5;
		player.shockwave += 5;
	}
}

void iDraw()
{
	iClear();
	if (mode == 1 && player.kills <400) iShowBMP(0, 0, "bck4.bmp");
	if (mode == 1 && player.kills >= 400) iShowBMP(0, 0, "bck6.bmp");
	if (laser_on<1) iShowBMP2(player.x, player.y-20, "laser3.bmp", 0);
	if (wave_on < 5) iShowBMP2(wave.x, wave.y, "shockwave.bmp", 0);
	level2();
	instructions();
	menu();
	gameover();
	levelcomplete();
	showleaderboard();
	pausemenu();
	credits();
	scoreboard();
	drawplayer();
	bonusmovement();
	enemymovement();
	setenemycannonvariables();
	enemycannonmovement();
	bosscannonmovement();
	cannonmovement();
	bossmovement();
	detectcollision();

}
void iMouseMove(int mx, int my)
{
}
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx>=258 && mx<= 582 && my >= 355 && my<= 423 && mode == 0)
	{
		initializer();
		mode = 1;
		on = 1;
		PlaySound("bck.wav", NULL, SND_LOOP | SND_ASYNC);
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && my>= 20 && my <= 72 && mx >= 265 && mx <= 592 && mode == 0)
	{
		exit(0);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 262 && mx <= 594 && my >= 102 && my <= 161 && mode == 0){
		mode = 4;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 315 && mx <= 559 && my >= 50 && my <= 97 && mode == 11){
		mode = 0;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 292 && mx <= 605 && my >= 230 && my <= 552-266 && mode == 10){
		mode = 12;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 289 && mx <= 593 && my >= 34 && my <= 100 && mode == 13){
		mode = 0;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 367 && mx <= 560 && my >= 20 && my <= 72 && mode == 12){
		mode = 10;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 262 && mx <= 594 && my >= 189 && my <= 248 && mode == 0){
		mode = 3;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 656 && mx <= 883 && my >= 42 && my <= 94 && mode == 3){
		mode = 0;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx>= 328 && mx <= 534 && my>= 15 && my <= 78 && mode == 4){
		mode = 0;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 292 && mx <= 605 && my >= 395 && my <= 445 && mode == 10){
		mode = 1;
		PlaySound("bck.wav", NULL, SND_LOOP | SND_ASYNC);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 292 && mx <= 605 && my >= 150 && my <= 205 && mode == 10){
		mode = 0;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 292 && mx <= 605 && my >= 68 && my <= 123 && mode == 10){
		exit(0);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 292 && mx <= 605 && my >= 312 && my <= 366 && mode == 10){
		savegame();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 258 && mx <= 582 && my >= 277 && my <= 335 && mode == 0){
		initializer();
		loadgame();
		mode = 1;
		PlaySound("bck.wav", NULL, SND_LOOP | SND_ASYNC);
	}


}
void iPassiveMouseMove(int mx,int my)
{
}

void iKeyboard(unsigned char key)
{

	if (mode == 1){
		if (key == 's')
		{
			if (player.y == line[0].y){
				cannon[0][line[0].cannon_count].y = player.y;
				cannon[0][line[0].cannon_count].x = player.x;
				cannon[0][line[0].cannon_count].active = 1;
				cannon[0][line[0].cannon_count].show = 1;
				line[0].cannon_count++;
				if (line[0].cannon_count > NUMBEROFCANNON - 1) line[0].cannon_count = 0;
			}
			else if (player.y == line[1].y){
				cannon[1][line[1].cannon_count].y = player.y;
				cannon[1][line[1].cannon_count].x = player.x;
				cannon[1][line[1].cannon_count].active = 1;
				cannon[1][line[1].cannon_count].show = 1;
				line[1].cannon_count++;
				if (line[1].cannon_count > NUMBEROFCANNON - 1) line[1].cannon_count = 0;
			}
			else if (player.y == line[2].y){
				cannon[2][line[2].cannon_count].y = player.y;
				cannon[2][line[2].cannon_count].x = player.x;
				cannon[2][line[2].cannon_count].active = 1;
				cannon[2][line[2].cannon_count].show = 1;
				line[2].cannon_count++;
				if (line[2].cannon_count > NUMBEROFCANNON - 1) line[2].cannon_count = 0;
			}
			else if (player.y == line[3].y){
				cannon[3][line[3].cannon_count].y = player.y;
				cannon[3][line[3].cannon_count].x = player.x;
				cannon[3][line[3].cannon_count].active = 1;
				cannon[3][line[3].cannon_count].show = 1;
				line[3].cannon_count++;
				if (line[3].cannon_count > NUMBEROFCANNON - 1) line[3].cannon_count = 0;
			}
			else if (player.y == line[4].y){
				cannon[4][line[4].cannon_count].y = player.y;
				cannon[4][line[4].cannon_count].x = player.x;
				cannon[4][line[4].cannon_count].active = 1;
				cannon[4][line[4].cannon_count].show = 1;
				line[4].cannon_count++;
				if (line[4].cannon_count > NUMBEROFCANNON - 1) line[4].cannon_count = 0;
			}
			else if (player.y == line[5].y){
				cannon[5][line[5].cannon_count].y = player.y;
				cannon[5][line[5].cannon_count].x = player.x;
				cannon[5][line[5].cannon_count].active = 1;
				cannon[5][line[5].cannon_count].show = 1;
				line[5].cannon_count++;
				if (line[5].cannon_count > NUMBEROFCANNON - 1) line[4].cannon_count = 0;
			}
		}
		if (key == 'r'){
			player.lives = 5;
			player.score = 0;
			player.y = 20;
			player.x = 0;
		}
		if (key == 'q'){
			exit(0);
		}
		if (key == 'w'){
			shockwave();
		}
		if (key == 'p'){
			//player.pause = ~player.pause;
			mode = 10;
			PlaySound("bck2.wav", NULL, SND_LOOP | SND_ASYNC);
		}
		if (key == 'a'){
			laser();
		}
		if (key == 'e' && player.invincibilty){
			player.invincible = ~player.invincible;
		}
		if (key == 'b'){
			player.kills = 400;
		}
		if (key == 'd'){
			player.kills = 800;
		}
		if (key == 'c'){
			printf("%d\n", player.kills);
		}
	}
	if (mode == 11 || mode == 13){
		if (key == '\r')
		{
			strcpy(str2, str);
			strcpy(highscore[state].name, str2);
			highscore[state].score = player.score;
			state++;
			updateleaderboard();
			for (int i = 0; i < len; i++){
				str[i] = 0;
			}
			mode = 0;
		}
		else if (key == '\b'){
			str[len - 1] = 0;
			len--;
		}
		else{
			str[len] = key;
			len++;
		}
	}
}
/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

	if(key == GLUT_KEY_DOWN && player.y>=90)
	{
		player.y -= 90;
	}
	if (key == GLUT_KEY_UP && player.y<screenheight-90)
	{
		player.y += 90;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		player.x += 50;
	}
	if (key == GLUT_KEY_LEFT && player.x>=50)
	{
		player.x -= 50;
	}
}
int main()
{
	PlaySound("bck2.wav", NULL, SND_LOOP | SND_ASYNC);
	leaderboard();
	iSetTimer(1, change);
	iSetTimer(500, bosschange);
	iInitialize(screenwidth, screenheight, "Space Invaders");
	return 0;
}
