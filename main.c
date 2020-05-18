//
//  main.c
//  malware_assign
//
//  Created by 노혜윤 on 2020/05/13.
//  Copyright © 2020 hxxyxxn. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#define N 256*256
#include <string.h>//메모리 블록이나 문자열을 다룰수 있는 함수
#define BUFLEN 10000 //버퍼의 최대크기를 10000으로 지정해준다

int encoding(char num[N])
{
    FILE*fp = fopen("enconding.txt", "wt");
    int i = 0; int cnt = 1, idx = 0;
    char results[N] = { 0, };
    printf("SERVER 주소 인코딩 완료: ");
    for (i = 0; i < N; i++)
    {
        if (num[i] != num[i + 1])
        {
            results[idx] = cnt;
            results[idx + 1] = num[i];
            cnt = 1;
            printf("%d%c", results[idx], results[idx + 1]);
            fprintf(fp, "%c%c", results[idx], results[idx + 1]);
            idx = idx + 2;
        }
        else
            cnt++;
        if (num[i] == 0)
            break;
    }
    fclose(fp);
    printf("\n");
    return 1;
}

int main(void)
{
    int client_socket;
    struct sockaddr_in server_addr;
    int slen = sizeof(server_addr);
    char buf[BUFLEN];
    char SERVER[] = "114.70.37.17"; //C&C 서버의 주소
    int PORT = 10004; //C&C서버와 통신할 포트 번호
    int i;
    printf("SERVER 주소 인코딩 전 : %s\n",SERVER);
    encoding(SERVER);

    //client_socket 이름의 소켓생성
    client_socket= socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket== -1){
        printf("Client socket error!");
        exit(0);
    }else printf("소켓생성 완료!\n");
    
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; //udp
    server_addr.sin_port = htons(PORT); //포트번호
    server_addr.sin_addr.s_addr = inet_addr(SERVER); //서버주소
    
    //sendbuf에 IMHACKER_2016111497 넣어서 서버로 전송
    char sendbuf[20] = "IMHACKER_2016111497";
    if (sendto(client_socket, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&server_addr, slen) == -1){
        printf("sendto() failed!!!");
        exit(0);
    }else printf("학번전송완료!\n");
        
    //server 로 부터 받아오는 값을 저장할 수 있는 buf 배열
    memset(buf, '\0', BUFLEN);
    if (recvfrom(client_socket, buf, BUFLEN, 0, (struct sockaddr*)&server_addr, (socklen_t*)&slen) == -1){
        printf("recvfrom() failed!!!");
        exit(0);
    }
    printf("%s",buf);
       
    //server 로부터 받은 문자열을 자르기 위해 strtok 함수를 사용하여 _ 와 : 조건을 만나면 문자열을 자르도록 했다.
    char*ptr;
    char *sArr[3];
    int k = 0;
    ptr = strtok(buf, "_:");
       
    while (ptr){
        sArr[k] = ptr;
        k++;
        ptr = strtok(NULL, "_:");
    }
//    printf("%s",sArr[1]);
        
    int num1;
    num1 = atoi(sArr[2]);
            //port 번호는 문자열이아닌 정수값이기 때문에 문자열로 받아온 값을 정수형변환을 위해 atoi 함수를 사용
    
    // 새로운 데이터를 전송하기 위한 구조화
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(num1);
    server_addr.sin_addr.s_addr= inet_addr(SERVER);
    
    
    //sendbuf2 에 2016111497 의 문자열로 초기화하고 for 문안에서 10번을 반복한다.
    for (i = 1; i < 11; i++) {
        char sendbuf2[11] = "2016111497";
        if (sendto(client_socket, sendbuf2, strlen(sendbuf2), 0, (struct sockaddr*)&server_addr, slen) == -1){
            printf("sendto() failed!!!");
            exit(0);
        } //else printf("10완료");
    }

    close(client_socket);//socket을 닫기
    return 0;
}
