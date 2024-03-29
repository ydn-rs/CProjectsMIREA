#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"
#include "xor.h"
#include <time.h>

int isPrime(int n) {
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int randNumPrime(int min, int max) {
    srand(time(NULL));
    int result = rand() % (max - min + 1) + min;
    while (!isPrime(result)) {
        result = rand() % (max - min + 1) + min;
    }
    return result;
}


int main(int argc, char *argv[]) {
    const char *inputFile = argv[1];
    size_t keyLength = 32;
    char table[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789";
    char *key, *resultRsaCrypt, *cValue, *resultDRsaCrypt;
    int p = randNumPrime(2, 83), q = randNumPrime(2, 89), e, n, d, input;
    FILE *crypt, *debug;

    debug = fopen("debug.txt", "w");
    printf("Выберите режим работы:\n[1] Зашифровать;\n[2] Расшифровать;\n[0] Выход\n");
    while (1) {
        printf("Ваш выбор: ");
        scanf("%d", &input);
        getchar();

        switch (input) {
            case 0:
                printf("Выход...\n");
                break;
            case 1:
                key = generateKey(keyLength, table);
                cValue = cryptKey(key, table);
                xor(inputFile, key);
                printf("Файл зашифрован\n");
                rsa(p, q, &e, &n);
                resultRsaCrypt = rsaCrypt(cValue, e, n);
                fprintf(debug, "%s\n%s\n(%d %d)\n%s", key, cValue, e, n, resultRsaCrypt);
                free(key);
                free(cValue);
                break;
            case 2:
                crypt = fopen("crypt.txt", "r");
                fscanf(crypt, "%d", &d);
                fclose(crypt);
                resultDRsaCrypt = rsaDecrypt(resultRsaCrypt, d, n);
                key = decryptKey(resultDRsaCrypt, table);
                xor(inputFile, key);
                printf("Файл расшифрован\n");
                fprintf(debug, "\n%s\n%s", resultDRsaCrypt, key);
                fclose(debug);
                free(resultDRsaCrypt);
                free(resultRsaCrypt);
                resultRsaCrypt = NULL;
                free(key);
                input = 0;
                break;
            default:
                printf("Некорректный ввод\n");
                break;
        }
        if (input == 0) {
            break;
        }
    }
}

