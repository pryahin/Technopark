#define _GNU_SOURCE
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

struct VariableArray{
    char* data;
    int key;
};

struct Pair{
    char** line;
    size_t size;
    size_t real_size;
};

struct Pair ErrorPair()
{
    struct Pair result;
    result.line = NULL;
    result.size = 0;
    
    return result;
};


// Получение строки
struct Pair getString();
// Удаление пробелов по краям строки
char* SpacesStartEnd(char* text);
// Удаление всех пробелов из строки
char* DeleteAllSpaces(char* text);

/*
 Замена подстроки на символ
 keyword:
 0 -> проходить по всем значениям массива + не строгая проверка
 1 -> проверка только выражения + строго проверять начало и конец(не стоят ли там буквы)
 */
struct Pair replace(struct Pair source, char* key, char value, int keyword);
//Парсинг пользовательских переменных
struct VariableArray ParseValues(char* source);

//Высчитывание выражения
char calculation(char* text);
//Выделяет подстроку со скобками
char* Brackets(char* text);
//Конвертирует все not
char* not(char* text);
//Считает все and
//char* and(char* text);
char and(char op1, char op2);
//Считает or
char or(char op1, char op2);
//считает xor
char xor(char op1, char op2);


struct Pair getString() {
    size_t n = 0, m=0, size_n = 11, size_m = 11;
    char** input = (char**)calloc(size_n,sizeof(char*));
    
    if (input == NULL) {
        printf("[error]");
        return ErrorPair();
    }
    
    input[n] = (char*)calloc(size_m,1);
    if (input[n] == NULL) {
        for (size_t i = 0; i < n; i++)
            free(input[i]);
        free(input);
        printf("[error]");
        return ErrorPair();
    }
    
    
    char c = ' ';
      while (scanf("%c", &c) == 1) {
        if (c != EOF) {
            if (c == '\n') {
                input[n][m] = '\n';
                input[n][m+1] = '\0';
                n++;
                m = 0;
                // input = (char**)realloc(input, (n + 1) * sizeof(char*));
                if (n==size_n-1){
                    char** buffer = (char**)realloc(input, (n*2+1)*sizeof(char*));
                    if (buffer == NULL) {
                        for (size_t i = 0; i < n + 1; i++)
                            free(input[i]);
                        free(input);
                        
                        printf("[error]");
                        return ErrorPair();
                    } else
                        input = buffer;
                    
                    size_n = 2*n+1;
                }
                
                    size_m=10;
                    input[n] = (char*)calloc(size_m,1);
                    if (input[n] == NULL) {
                        for (size_t i = 0; i < n; i++)
                            free(input[i]);
                        free(input);
                        
                        printf("[error]");
                        return ErrorPair();
                    }
            } else {
                input[n][m] = c;
                m++;
                if (m==size_m-1){
                    char* buffer = (char*)realloc(input[n], (2*m+1) );
                    if (buffer == NULL) {
                        for (size_t i = 0; i < n + 1; i++)
                            free(input[i]);
                        free(input);
                        
                        printf("[error]");
                        return ErrorPair();
                    } else{
                        input[n] = buffer;
                        size_m = m*2+1;
                    }
                }
            }
        } else
            break;
    }
    //free(input[n]);
    input[n][m] = '\n';
    input[n][m+1] = '\0';
    
    struct Pair result;
    result.line = input;
    result.size = n+1;
    result.real_size = size_n;
    
    return result;
}

//Удаление символов
char* SpacesStartEnd(char* text) {
    if(text == NULL) {
        return NULL;
    }
    size_t i = 0, j = 0, length = strlen(text);
    // Сначала
    while(text[i] == ' ') {
        i++;
    }
    if(i > 0) {
        for(j = 0; j < length; j++) {
            text[j] = text[j+i];
            if(text[j+i] == '\0') {
                break;
            }
        }
        text[j] = '\0';
    }
    //После
    length = strlen(text);
    if(length > 0) {
        i = length - 1;
        while(text[i] == ' ') {
            i--;
        }
        if(i < length - 1) {
            text[i+1] = '\0';
        }
    }
    return text;
}

char* DeleteAllSpaces(char* text) {
    if(text == NULL) {
        return NULL;
    }
    
    char* result = (char*)calloc(strlen(text)+1,1);
    
    size_t i = 0,j=0;
    for (; text[i]!='\0'; i++)
        if (text[i]!=' ')
            result[j++]=text[i];
    strcpy(text,result);
    free(result);
    return text;
}

struct Pair replace(struct Pair source, char* key, char value, int keyword)
{
    if(source.line== NULL) {
        return ErrorPair();
    }
    size_t length_key = strlen(key);
    for (size_t n=0;n<source.size;n++)
    {
        if (keyword==0 || n==source.size-1){
            if (source.line[n]==NULL)
                return ErrorPair();
            char* text = strdup(source.line[n]);
            size_t length_text = strlen(text);
            for (size_t i=0; i<length_text; i++)
            {
                if (text[i]=='\0')
                    break;
                size_t status = 1;
                size_t isStart = 0,isEnd=0;
                if (i==0 || keyword == 0)
                    isStart=1;
                else
                    if (!(text[i-1]>='a' && text[i-1]<='z'))
                        isStart=1;
                if (i!=0 && ((text[i-1]>='a' && text[i-1]<='z') || (text[i-1]>='A' && text[i-1]<='Z')))
                    isStart = 0;
                if (text[i] == key[0] && isStart==1) {
                    size_t j = i+1;
                    while ((text[j] != '\0') && (j-i < length_key)) {
                        if (text[j] != key[j-i]) {
                            status = 0;
                            break;
                        }
                        j++;
                    }
                    if(text[j]!='\0'){
                        if (keyword == 0)
                            isEnd = 1;
                        else
                            if (!(text[j]>='a' && text[j]<='z'))
                                isEnd=1;
                    }
                    if (i+length_key<=length_text)
                        if ((text[i+length_key]>='a' && text[i+length_key]<='z') || (text[i+length_key]>='A' && text[i+length_key]<='Z'))
                            isEnd=0;
                    if (status == 1  && isEnd==1) {
                        text[i]=value;
                        for(j=i+1;j<length_text;j++){
                            text[j]=text[j+length_key-1];
                            if (text[j+length_key-1]=='\0')
                                break;
                        }
                        i--;
                    }
                }
            }
            free(source.line[n]);
            source.line[n]=strdup(text);
            free(text);
            //source.line[n]=text;
        }
    }
    return source;
}

struct VariableArray ParseValues(char* source)
{
    struct VariableArray result;
    result.key = -1;
    char buffer[1024]="";
    
    size_t i=0;
    while (source[i]!='=')
    {
        buffer[i]=source[i];
        if (!(source[i]>='a' && source[i]<='z') || source[i]=='\n')
        {
            result.data = strdup(buffer);
            return result;
        }
        i++;
    }
    result.data = strdup(buffer);
    if (source[i+1]=='0')
        result.key = 0;
        else if (source[i+1]=='1')
            result.key = 1;
            else
                return result;
    
    if (source[i+2]!=';')
        result.key=-1;
        else if (source[i+3]!='\n')
            result.key=-1;
            
            return result;
}

char* not(char* text)
{
    char* result = (char*)calloc(strlen(text)+1,1);
    
    size_t j=0;
    for (size_t i = 0; i<strlen(text); i++)
        if (text[i]=='!'){
            if (text[i+1]=='0')
                result[j++]='1';
            else
                result[j++]='0';
            i++;
        }
        else
            result[j++]=text[i];
    
    //    if (j!=strlen(text)-1)
    //        result[j]=text[strlen(text)-1];
    
    return result;
}

char and(char op1, char op2)
{
    if ((op1=='1' || op1=='0') && (op2=='1' || op2=='0'))
    {
        if (op1=='1' && op2=='1')
            return '1';
        else
            return '0';
    }
    return '[';
}

char or(char op1, char op2)
{
    if ((op1=='1' || op1=='0') && (op2=='1' || op2=='0')){
        if (op1=='1' || op2=='1')
            return '1';
        else if (op1=='0' || op2=='0')
            return '0';
    }
    return '[';
}

char xor(char op1, char op2)
{
    if ((op1=='1' || op1=='0') && (op2=='1' || op2=='0')){
        if (op1==op2)
            return '0';
        else
            return '1';
    }
    return '[';
}

char checkDouble(char* source)
{
    for (size_t i = 1; i<strlen(source); i++)
        if ((source[i]=='1' || source[i]=='0') && (source[i-1]=='1' || source[i-1]=='0'))
            return '0';
    return '1';
}

char calculation(char* source1)
{
    //    char* text = (char*)calloc(strlen(source)+1,1);
    //    strcpy(text,source);
    char * source = strdup(source1);
    char* tmp_brack = strdup(source);
    while (1)
    {
        size_t i=0;
        for (;i<strlen(tmp_brack);i++){
            if (tmp_brack[i]==')')
            {
                free(source);
                free(tmp_brack);
                return '[';
            }
            if (tmp_brack[i]=='('){
                char *tmp_tmp = Brackets(tmp_brack);
                free(tmp_brack);
                tmp_brack = strdup(tmp_tmp);
                free(tmp_tmp);
                break;
            }
            if (!(tmp_brack[i]=='1' || tmp_brack[i]=='0' || tmp_brack[i]=='!' || tmp_brack[i]=='^' || tmp_brack[i]=='+' || tmp_brack[i]=='*' || tmp_brack[i]=='(' || tmp_brack[i]==')'))
            {
                free(source);
                free(tmp_brack);
                return '[';
            }
        }
        if (i==strlen(tmp_brack))
        {
            free(source);
            source = strdup(tmp_brack);
            free(tmp_brack);
            break;
        }
    }
    
    char* result = strdup(source);
    if (result[0]=='['){
        //   free(result);
        //free(text);
        free(result);
        free(source);
        return '[';
    }
    
    if (checkDouble(result)=='0')
    {
        free(result);
        free(source);
        return '[';
    }
    
    char* tmp = not(result);
    strcpy(result, tmp);
    free(tmp);
    
    tmp = (char*)calloc(strlen(result)+1,1);
    size_t j = 0;
    for (size_t i=0;result[i]!='\0';i++)
    {
        if (result[i+1]=='*'){
            if (strlen(result)<=i+2)
            {
                free(result);
                // free(text);
                free(tmp);
                free(source);
                return '[';
            }
            tmp[j++] = and(result[i],result[i+2]);
            i=i+2;
        }else
            tmp[j++] = result[i];
    }
    
    strcpy(result,tmp);
    free(tmp);
    
    char res = result[0];
    for (size_t i = 1; result[i]!='\0'; i++)
    {
        if (result[i]=='+'){
            res = or(res,result[++i]);
        }
        else if (result[i]=='^'){
            res = xor(res,result[++i]);
        }
        if (res=='[')
            break;
    }
    free(result);
    free(source);
    //    free(text);
    return res;
}

char* Brackets(char* source)
{
    char* text = (char*)calloc(strlen(source)+1,1);
    strcpy(text,source);
    size_t start=0,end=0;
    for (size_t i=0;i<strlen(text);i++)
    {
        if (text[i]=='('){
            start = i;
            end = 0;
        }
        else if (text[i]==')' && end==0)
            end = i;
    }
    if (start>end || (text[start]=='(' && end==0)){
        free(text);
        return strdup("[error]");
    }
    if (end-start==1)
    {
        free(text);
        return strdup(" ");
    }
    if (start==end){
        return text;
    }
    else{
        char* result = (char*)calloc(strlen(text)+1,1);
        char* calc = (char*)calloc((end-start),1);
        
        size_t j=0;
        for (size_t i = start+1; i<=end-1; i++){
            calc[j++] = text[i];
        }
        
        j=0;
        for (size_t i=0;i<start;i++)
            result[j++]=text[i];
        result[j++]=calculation(calc);
        for (size_t i=end+1; i<strlen(text);i++)
            result[j++]=text[i];
        
        free(calc);
        free(text);
        return result;
    }
}

int main() {
    // ввод текста
    struct Pair input = getString();
    
    input = replace(input, "True", '1',0);
    input = replace(input, "False", '0',0);
    //лишние пробелы
    input = replace(input, "  ", ' ',0); //2 пробела->1 пробел
    for (size_t i=0; i<input.size;i++){ //-пробелы с конца и начала
        input.line[i]=SpacesStartEnd(input.line[i]);
        //split(input.line[i]);
    }
    
    input = replace(input, " )", ')',0);
    input = replace(input, ") ", ')',0);
    input = replace(input, "( ", '(',0);
    input = replace(input, " (", '(',0);
    
    //окончания переменных
    input = replace(input, " ;", ';',0);
    input = replace(input, "; ", ';',0);
    //  input = replace(input, ";\n", '\n',0);
    
    
    //-пробелы между rvalue и lvalue
    input = replace(input, "= ", '=',0);
    input = replace(input, " =", '=',0);
    //   input = replace(input, "=", ' ',0);
    
    
    //Ключевые слова
    input = replace(input, " xor ",'^',0);
    input = replace(input, "xor", '^',1);
    
    input = replace(input, " and ",'*',0);
    input = replace(input, "and", '*',1);
    
    input = replace(input, " or ",'+',0);
    input = replace(input, "or", '+',1);
    
    input = replace(input, "not ",'!',0);
    input = replace(input, "not",'!',1);
    
    //    новые пробелы
    input.line[input.size-1] = DeleteAllSpaces(input.line[input.size-1]);
    
    //    if (input.size==1 && (input.line[0]==NULL || input.line[0]==" "))
    //        {
    //            printf("False");
    //            for (size_t er=0;er<input.size;er++)
    //                free(input.line[er]);
    //            free(input.line);
    //            return 0;
    //        }
    
    if (input.size>1){
        //        unsigned long size = input.size-2;
        for (int i = (int)(input.size-2); i>=0; i--)
        {
            struct VariableArray data = ParseValues(input.line[i]);
            if (data.key!=-1){
                input = replace(input, data.data, data.key+'0',1);
                free(data.data);
            }else
            {
                printf("[error]");
                for (size_t er=0;er<input.real_size;er++)
                    free(input.line[er]);
                free(input.line);
                free(data.data);
                return 0;
            }
        }
    }
    // тест: нет выражения, только объявление переменных
    struct VariableArray data = ParseValues(input.line[input.size-1]);
    if (data.key!=-1){
        //input = replace(input, data.data, data.key+'0',1);
        printf("False");
        for (size_t er=0;er<input.real_size;er++)
            free(input.line[er]);
        free(input.line);
        free(data.data);
        return 0;
    }
    
    free(data.data);
    char* text = input.line[input.size-1];
    //    printf("%s",text);
    
    text[strlen(text)-1]='\0';
    char result = calculation(text);
    if (result =='[')
    {
        printf("[error]");
        for (size_t er=0;er<input.real_size;er++)
            free(input.line[er]);
        free(input.line);
        return 0;
    }
    else
    {
        if (result=='1')
            printf("True");
        else if (result=='0')
            printf("False");
        else
            printf("[error]");
    }
    
    for (size_t er=0;er<input.real_size;er++)
        free(input.line[er]);
    free(input.line);
    //    free(text);
    return 0;
}
