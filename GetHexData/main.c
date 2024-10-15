#include "HexReader.h"

int main(){
    FILE *fp = fopen("RR.hex","r"); // mở file
    uint8_t count;
    char data;
    if(fp == NULL){
        printf("Loi khi mo file");
    }
    else{
        GetHexData(fp, LineData);
        for (uint32_t i = 0; i < CurrentNumberData; i++) {
            printf("%02x ", HexData[i]);
        }
        printf("\n");
    }
    fclose(fp); // đóng file
    return 0;
}
// gcc -c main.c -o main.o
// gcc -c HexReader.c -o HexReader.o
// gcc main.o HexReader.o -o main