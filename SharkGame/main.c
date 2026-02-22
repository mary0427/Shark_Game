//
//  main.c
//  sharkGame
//

#include <stdio.h>
// ----- EX. 1 : Preparation------------
#include <stdlib.h>
#include <time.h>
// ----- EX. 1 : Preparation------------
#include "board.h"

// ----- EX. 4 : player ------------
#define N_PLAYER            3
// ----- EX. 4 : player ------------

#define MAX_CHARNAME        200

// ----- EX. 3 : board ------------
#define MAX_DIE             6

// ----- EX. 4 : player ------------
#define PLAYERSTATUS_LIVE   0
#define PLAYERSTATUS_DIE    1
#define PLAYERSTATUS_END    2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER]; //0 - live, 1 - die, 2 - end
char player_statusString[3][MAX_CHARNAME] = {"LIVE", "DIE", "END"};
// ----- EX. 4 : player ------------

// ----- EX. 3 : board ------------
int rolldie(void)
{
    return rand()%MAX_DIE+1;
}
// ----- EX. 3 : board ------------

// ----- EX. 1 : Preparation------------
void opening(void)
{
    printf("==============================================================\n");
    printf("==============================================================\n");
    printf("==========                SHARK GAME!!!                =======\n");
    printf("==============================================================\n");
    printf("==============================================================\n");
}
// ----- EX. 1 : Preparation------------

// ----- EX. 6 : game end ------------
int game_end(void)
{
    int i;
    int flag_end = 1;
    
    //if all the players are died or end?
    for (i=0;i<N_PLAYER;i++) //하나라도 살아있는지 검사 
    {
        if (player_status[i] == PLAYERSTATUS_LIVE)
        {
            flag_end = 0; //그러면 게임 종료 아님  
            break;
        }
    }
    
    return flag_end; //종료 여부 (1,0)반환 
}
// ----- EX. 6 : game end ------------

// ----- EX. 4 : player ------------
void printPlayerPosition(int player)
{
    int i;
    
    for (i=0;i<N_BOARD;i++)
    {
        printf("|");
        if (player_position[player] == i)
            printf("%c", player_name[player][0]);
        else
        {
            if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
                printf("X");
            else
                printf(" ");
        }
    }
    printf("|\n");
}

void printPlayerStatus(void)
{
    int i;
    printf("player status ---\n");
    for (i=0;i<N_PLAYER;i++)
    {
        printf("%s : pos %i, coin %i, status %s\n", player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
        printPlayerPosition(i);
    }
    printf("-----------------\n");
}
// ----- EX. 4 : player ------------

// ----- EX. 5 : shark ------------
void checkDie(void)
{
    int i;
    for (i=0;i<N_PLAYER;i++)
    {
        if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
        {
            printf("%s in pos %i has died!! (coin %i)\n", player_name[i], player_position[i], player_coin[i]);
            player_status[i] = PLAYERSTATUS_DIE;
        }
    }
}
// ----- EX. 5 : shark ------------

// ----- EX. 6 : game end ------------
int getAlivePlayer(void)
{
	int i;
	int cnt=0;
	for(i=0;i<N_PLAYER;i++)
	{
		if(player_status[i]==PLAYERSTATUS_END)
		{
			cnt++; //생존player 수 
		}
	}
	
	return cnt;
}

int END_order[N_PLAYER]; //도착 순위 담음 
int getWinner(void)
{
	int i;
    int winner=0;
    int max_coin=-1;
    
    for (i=0;i<N_PLAYER;i++)
    {
    	if (player_status[i] != PLAYERSTATUS_DIE) //생존player 중 승자 체크 
    	{
    		if (player_coin[i]>max_coin)
    		{
    			max_coin=player_coin[i];
    			winner=i;
			}
			
			else if (player_coin[i]==max_coin) //coin 개수 같을 경우
			{
				if (END_order[i] < END_order[i-1])
				{
					winner= i;  //도착순서 더 빠르면 승자
				}
				else
					winner = (i-1);
			 } 
		}
			
	}	
	
    return winner;
}
// ----- EX. 6 : game end ------------




int main(int argc, const char * argv[]) {
    
    int i;
    int turn=0;

// ----- EX. 1 : Preparation------------
    // main() 함수윗부분에rand 초기화함수 호출 
	srand((unsigned int)time(NULL));
    opening();
// ----- EX. 1 : Preparation------------


// ----- EX. 2 : structuring ------------
    //step1 : initialization
    //step1-1 : board initialization
    	board_initBoard();

// ----- EX. 4 : player ------------
    //step1-2 : initialize player
    for (i=0;i<N_PLAYER;i++) //N_PLAYER =3
    {
        player_position[i] = 0;
        player_coin[i] = 0;
        player_status[i] = PLAYERSTATUS_LIVE;
        printf("Player %i's name: ", i);
        scanf("%s", player_name[i]);
    }
// ----- EX. 4 : player ------------
    
    
    //step 2. : game start, turn loop(do-while)
    do {
        int dieResult;
        int coinResult;
        int dum;
		
// ----- EX. 4 : player ------------
        //Player 상태가PLAYERSTATUS_LIVE인 경우만 반복문을 수행
		//죽었거나, 끝난 player면 다음turn으로 넘기는 구문 
		if (player_status[turn] != PLAYERSTATUS_LIVE)  
        {
            turn = (turn + 1)%N_PLAYER; 
            //shark moving 까지 건너뛰지 않도록 함 
			printf("\n");   	
	    	if (turn ==0) 
	        {
	        	int shark_pos = board_stepShark();
	        	printf("Shark moved to pos %i!\n",shark_pos);
	        	//check die
	        	checkDie();
			}
			
            continue;  //while 조건문으로 바로(이하 코드 건너뜀) 
        }
// ----- EX. 4 : player ------------
        
        
        //step 2-1. status printing
// ----- EX. 3 : board ------------
		printf("\n");
		board_printBoardStatus();
// ----- EX. 3 : board ------------
		
// ----- EX. 4 : player ------------
		printPlayerStatus();
// ----- EX. 4 : player ------------


        //step 2-2. rolling die
// ----- EX. 4 : player ------------
        printf("%s turn!! ", player_name[turn]);
        printf("Press any key to roll a die!\n");
        scanf("%d", &dum);
        fflush(stdin);
// ----- EX. 4 : player ------------
        dieResult = rolldie(); //주사위 굴려서 이동횟수 지정
        
        //step 2-3. moving
        int t=1;
        
   		if (player_position[turn]+dieResult < N_BOARD)
   			player_position[turn]+=dieResult;
   		else{
   			// player가 보드 맨끝까지 간 경우 처리** 
   			player_position[turn]=(N_BOARD-1);
   			player_status[turn]=PLAYERSTATUS_END;
   			
   			END_order[turn] = t; //도착 순위 담는 배열(for getWinner()함수)
			t++; 
			}
			
			//이동 결과 출력
			printf("Die result is %d. \n",dieResult);
        	printf("Player %s moved to pos %d. (%s)\n",player_name[turn],player_position[turn],player_statusString[player_status[turn]]);
        
		
		//step 2-4. coin
			//coin 습득
		coinResult=board_getBoardCoin(player_position[turn]); 
		player_coin[turn]+=coinResult;
        	//coin 존재하는경우coin 습득정보 출력
		if(coinResult>0)
			printf("Player %s got %d coin!\n",player_name[turn],coinResult);
		
		
        //step 2-5. end process
        	//다음 player로넘김 (turn 1 증가) 
    	turn = (turn + 1)%N_PLAYER; // 0~(N_PLAYER-1)의 숫자로 도는 중 
    	
//shark moving (2번째 turn=0부터) 
		printf("\n");   	
    	if (turn ==0) 
        {	 
        	int shark_pos = board_stepShark();
        	printf("Shark moved to pos %i!\n",shark_pos);
        	//check die
        	checkDie();
		}
    	
// ----- EX. 6 : game end ------------
    } while(game_end() == 0); // 하나라도 살아있으면 TRUE 
    
    //step 3. game end process
    printf("\n");
	printf("GAME END!!\n");
     if(getAlivePlayer()!=0)
    {
    	printf("%i player(s) is(are) alive! winner is %s.\n", getAlivePlayer(), player_name[getWinner()]);
	}
    else
    	printf("No winner."); //No Alive player case
		 
// ----- EX. 6 : game end ------------

// ----- EX. 2 : structuring ------------

    return 0;
}

