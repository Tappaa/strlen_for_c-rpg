#include <stdio.h>
#include <windows.h>

/// @Note : in the console, ascii takes up 1 space and UTF-8 takes up 2 spaces.
unsigned __int64 utf8Strlen(const char* str) {
    int length = 0;
    unsigned char c;

    while (*str) {
        c = (unsigned char)*str;

        if (c < 0x80) {
            // ASCII Code (0x00 ~ 0x7F): +1
            length += 1;
            str += 1;
        } else if ((c & 0xE0) == 0xC0) {
            // 2 Bytes UTF-8 Character (110xxxxx): +2
            length += 2;
            str += 2; // move to next character
        } else if ((c & 0xF0) == 0xE0) {
            // 3 Bytes UTF-8 Character (1110xxxx): +2
            length += 2;
            str += 3; // move to next character
        } else if ((c & 0xF8) == 0xF0) {
            // 4 Bytes UTF-8 Character (11110xxx): +2
            length += 2;
            str += 4; // move to next character
        } else {
            // Invalid UTF-8 Character: +1
            str += 1;
        }
    }

    return length;
}

int main(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    system("mode con cols=160 lines=41 | title STRLEN_INFO");

    // 파일 가져오기
    FILE *fp = fopen("input.txt", "rb");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다.\n");
        system("pause");
        return 1;
    }

    // 파일 크기 구하기
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 파일 내용 읽기
    char *buffer = (char *)malloc(size + 1);
    fread(buffer, size, 1, fp);
    buffer[size] = '\0';

    // 파일 닫기
    fclose(fp);

    char *str = (char *)malloc(size + 1);
    strcpy(str, buffer);
    char *token;  // 각 토큰을 저장할 포인터
    const char delim[] = "\n";  // 구분자
    int count = 0;  // 토큰 개수

    // 첫 번째 토큰 추출
    token = strtok(str, delim);

    while (token != NULL) {
        printf("%d / 40번째 글자 길이 : %llu / 160\n", count + 1, utf8Strlen(token));
        token = strtok(NULL, delim);  // 다음 토큰 추출
        count++;  // 토큰 개수 증가
    }

    // 파일 내용 출력
    printf("아무 키나 누르면 미리보기를 출력합니다. ");
    system("pause");
    system("cls");
    printf("%s", buffer);
    system("pause");
    free(buffer);
    free(str);
    return 0;
}
