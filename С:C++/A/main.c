/*
 Чтобы показать информацию о пришедшем письме, нужно сначала её найти.
 Для этого в файле письма необходимо найти специальные заголовки.
 
 Составить программу построчной фильтрации текста.
 Суть фильтра — отбор строк, начинающихся с одного из следующих выражений: «Date:», «From:»,«To:», «Subject:».
 Текстовые строки подаются на стандартный ввод программы, результат программы должен подаваться на стандартный вывод.
 
 Процедура фильтрации должна быть оформлена в виде отдельной функции, которой подается на вход массив строк, выделенных в динамической памяти и его длина.
 На выходе функция возвращает указатель на NULL-терминированный массив с найденными строками (последним элементом массива добавлен NULL для обозначения, что данных больше нет).
 
 Программа должна уметь обрабатывать возникающие ошибки (например, ошибки выделения памяти). В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** _filter(char** text, size_t n) {
    
    char** result = (char**)malloc(11*sizeof(char*));
    size_t size = 10;
    size_t count = 0;
    
    const char* keys[] = {"Date:", "From:", "To:", "Subject:"};
    for (size_t i = 0; i < n; i++) {
        char* line = text[i];
        for (size_t i_key = 0; i_key < 4; i_key++) {
            size_t status = 1;
            size_t length_key = strlen(keys[i_key]);
            if (line[0] == keys[i_key][0]) {
                size_t j = 1;
                while ((line[j] != '\n') && (j < length_key)) {
                    if (line[j] != keys[i_key][j]) {
                        status = 0;
                        break;
                    }
                    j++;
                }
                if (status == 1) {
                    //                    for (; *line != '\n'; line++)
                    //                        printf("%c", *line);
                    //                    printf("\n");
                    if (count == size-1)
                    {
                        char** buffer = (char**)realloc(result, (count*2+1)*sizeof(char*));
                        if (buffer == NULL)
                        {
                            free(result);
                            printf("[error]");
                            return NULL;
                        }
                        size = 2*count+1;
                        result = buffer;
                    }
                    result[count++] = strdup(line);
                }
            }
        }
    }
    result[count] = NULL;
    return result;
}

int main() {
    size_t n = 0;
    size_t m = 0;
    char** input = (char**)malloc(sizeof(char*));
    
    if (input == NULL) {
        printf("[error]");
        return 0;
    }
    
    input[n] = (char*)malloc(sizeof(char));
    if (input[n] == NULL) {
        for (size_t i = 0; i < n; i++)
            free(input[i]);
        free(input);
        printf("[error]");
        return 0;
    }
    
    char c;
    while (scanf("%c", &c) != EOF) {
        if (c != EOF) {
            if (c == '\n') {
                input[n][m] = '\0';
                //                input[n][m+1] = '\0';
                n++;
                m = 0;
                // input = (char**)realloc(input, (n + 1) * sizeof(char*));
                char** buffer = (char**)realloc(input, (n + 1) * sizeof(char*));
                if (buffer == NULL) {
                    for (size_t i = 0; i < n + 1; i++)
                        free(input[i]);
                    free(input);
                    
                    printf("[error]");
                    return 0;
                } else
                    input = buffer;
                
                input[n] = (char*)malloc(sizeof(char));
                if (input[n] == NULL) {
                    for (size_t i = 0; i < n; i++)
                        free(input[i]);
                    free(input);
                    
                    printf("[error]");
                    return 0;
                }
            } else {
                input[n][m] = c;
                m++;
                char* buffer = (char*)realloc(input[n], (m + 1) * sizeof(char));
                if (buffer == NULL) {
                    for (size_t i = 0; i < n + 1; i++)
                        free(input[i]);
                    free(input);
                    
                    printf("[error]");
                    return 0;
                } else
                    input[n] = buffer;
            }
        } else
            break;
    }
    free(input[n]);
    char** result = _filter(input, n);
    if (result == NULL){
        for (size_t i = 0; i < n; i++)
            free(input[i]);
        free(input);
        return 0;
    }
    
    size_t i = 0;
    while(result[i]!=NULL)
    {
        for (size_t j = 0; result[i][j] != '\0'; j++)
            printf("%c", result[i][j]);
        printf("\n");
        free(result[i]);
        i++;
    }
    
    for (size_t i = 0; i < n; i++)
        free(input[i]);
    free(input);
    free(result);
    
    return 0;
}
