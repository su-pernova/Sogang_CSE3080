#include <stdio.h>
#include <stdlib.h>
#define MAIN_MAJOR 26
#define INTER_MAJOR 59

// FILE *fp;
// FILE *fresult;
FILE *fps;
FILE *fresults;
int sample[INTER_MAJOR][MAIN_MAJOR]; //전체 설계전공 데이터 받는 배열
// int array[MAIN_MAJOR];
// void create_data();
void create_sample();

int main(void){
    create_sample();

    return 0;
}

void create_sample(){ //전체 설계전공 데이터 결과 생성
    fps = fopen("major_percentage.txt", "r");
    fresults = fopen("major_percentage_result.txt", "w");
    for (int i = 0; i < INTER_MAJOR; i++){
        for (int j = 0; j < MAIN_MAJOR; j++){
            fscanf(fps, "%d", &sample[i][j]);
        }
    }
    for (int i = 0; i < INTER_MAJOR; i++){
        for (int j = 0; j < MAIN_MAJOR; j++){
            if (sample[i][j] >= 20){
                fprintf(fresults, "%d ", 1);
            }
            else{
                fprintf(fresults, "%d ", 0);
            }
        }
        fprintf(fresults, "\n");
    }

    fclose(fps);
    fclose(fresults);
    return;
}

// void create_data(){ //사용자가 입력하는 한 줄 짜리 데이터 결과 생성
//     fp = fopen("user_data.txt", "r");
//     fresult = fopen("result.txt", "w");
//     for (int i = 0; i < MAIN_MAJOR; i++){
//         fscanf(fp, "%d", &array[i]);
//     }
//     for (int i = 0; i < MAIN_MAJOR; i++){
//         if (array[i] >= 20){
//             fprintf(fresult, "%d ", 1);
//         }
//         else{
//             fprintf(fresult, "%d ", 0);
//         }
//     }

//     fclose(fp);
//     fclose(fresult);
//     return;
// }
