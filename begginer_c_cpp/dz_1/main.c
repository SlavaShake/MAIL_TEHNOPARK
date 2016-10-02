/*
 *
Написать программу, которая в качестве аргументов командной строки принимает имена текстовых
файлов (первое - имя целевого документа, далее - имена остальных документов в коллекции), а
на выходной поток выдаёт различные слова в целевом файле с указанием их частоты встречаемости
в тексте целевого документа. Слова должны выводиться в порядке убывания их частоты встречаемости.
При одинаковой частоте встречаемости выдавать слова в лексикографическом порядке.
Частоту встречаемости считать по формуле TF-IDF:
где tfi — частота встречаемости i-го слова в данном документе (term frequency),
— логарифм отношения количества всех документов в коллекции к количеству документов, в
которых встречается i-ое слово (inverse document frequency).
Слова во входных файлах разделяются символами, для которых библиотечные функции isspace() или ispunct()
возвращают ненулевое значение. Считать, что словарь различных слов можно хранить в оперативной памяти.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//для isspace и ispunct
#include <math.h>
#include <stdio.h>

#define MAX_WORDS 2000
#define MAX_FILE 15

FILE* open_file(const char *name_file);
FILE* write_to_file(const char *name_file);
int close_file(FILE* file);
char to_lower(char A);
void char_to_lowercase(char A[]);
void delete_arr(char c[]);
int get_words_from_collection(FILE* open_file, char *word, long long *N);
int check_in_matrix(char words[MAX_WORDS][32], char *word, long long check_point[MAX_WORDS][MAX_FILE], int N_file);
void calculate_TFi(long long N[MAX_FILE],long long check_point[MAX_WORDS][MAX_FILE],double TFi[MAX_WORDS][MAX_FILE],int argc,double *res_TFi,int num_word);
double summ(double TFi[MAX_FILE],int argc);
void print_matrix(long long N[][MAX_FILE]);



int main(int argc, char *argv[])
{
    if (argc < 3){
           printf("Usage: %s file_in file_1 and i.e.\n", argv[0]);
           return EXIT_FAILURE;
           }

    const char *file_input = argv[1];
    printf("File to write words: %s\n",file_input);


    //может содержать до 2000 слов для максимального
    //колличества файлов в коллекции равным 15
    char        words[MAX_WORDS][32];

    //колличество пападаний слова в файле
    long long   number_of_words[MAX_WORDS][MAX_FILE];

   //частота попаданий слова в отдельном файле
    double      TFi[MAX_WORDS][MAX_FILE];

    //результирующий TFi
    double      res_TFi[MAX_WORDS];

    //сколько всего слов в файле
    long long N[MAX_FILE];

    //счетчик слов которые есть в матрице
    int num_word = 0;

    for(int i = 1; i < argc; i++){

        if(i == 1){

            FILE* file = open_file(argv[i]);

            char word[32];

            for(num_word = 0; !feof(file); ){

                if(get_words_from_collection(file,word,&N[i-1]) == 1){
                    if(check_in_matrix(words,word,number_of_words,i-1) == 0){
                        strcpy(words[num_word],word);
                        number_of_words[num_word][i-1]++;
                        num_word++;

                    }
                }

            }
            close_file(file);
        }
        else{
            FILE* file = open_file(argv[i]);

            char word[32];

            for(int j = 0; !feof(file); j++){

                if(get_words_from_collection(file,word,&N[i-1]) == 1)
                    check_in_matrix(words,word,number_of_words,i-1);

            }
            close_file(file);
        }
    }

    for(int i = 0;i<num_word;i++){
        printf("words[%d] = %s\n",i,words[i]);
    }

    FILE* write_file = write_to_file(file_input);

    calculate_TFi(N,number_of_words,TFi,argc,res_TFi,num_word);

    for(int i = 0; i < num_word; i++){

//        fprintf(write_file,"RED%15s = BLUE%15.9f \n\n",words[i],res_TFi[i]);
        printf("RED%15s = BLUE%15.9f \n\n",words[i],res_TFi[i]);
    }
    close_file(write_file);
    printf("Hello World!\n");
    return 0;
}

//подсчет частоты попадания файла
void calculate_TFi(long long N[MAX_FILE], long long check_point[MAX_WORDS][MAX_FILE], double TFi[MAX_WORDS][MAX_FILE], int argc, double *res_TFi, int num_word){
    argc -= 1;
    for(int i = 0; i < num_word; i++){
        for(int j = 0; j < argc;j++){
            TFi[i][j] = (double)check_point[i][j]/N[j];
        }
        double sum = summ(TFi[i],argc);
        res_TFi[i] = TFi[i][0]/sum;
    }
}

//Сумма всех TFi
double summ(double TFi[MAX_FILE],int argc){
    double sum = 0;
    for(int i = 0; i < argc; i++){
        sum += (double)pow(TFi[i],2);
    }
    return sum;
}

//проверка слова на совпадение в матрице
int check_in_matrix( char words[MAX_WORDS][32], char *word, long long check_point[MAX_WORDS][MAX_FILE], int N_file){

    for(int i = 0; words[i][0]!='\0';i++){
        if(strcmp(words[i],word) == 0){
            check_point[i][N_file] += 1;
            return 1;
        }
    }
    return 0;
}
//вытаскиваем слово из потока чтения файла
int get_words_from_collection(FILE* open_file, char *word, long long *N){

    int c = fgetc(open_file);
    int i = 0;
    for( ;!isspace(c) && !ispunct(c) && (c != EOF); i++, c = fgetc(open_file))
        word[i] = c;

    word[i] = '\0';

    if(word[0] != '\0'){
        *N += 1;
        char_to_lowercase(word);
//        printf("get_words_from_collection_word = %s\n",word);
        return 1;

    }
    else
        return 0;
}

//приведение к нижнему регистру
char to_lower(char A){
    if((A >= 'A') && (A <= 'Z'))
        return A + ('a' - 'A');
    return A;
}

//приведение к нижнему регистру
void char_to_lowercase(char A[]){

    for(int i = 0; A[i] != '\0';i++){
        A[i] = to_lower(A[i]);
    }
}

FILE* open_file(const char *name_file){
    FILE* file;
    file = fopen(name_file,"r");
    if(file == NULL){
        printf("Failed to open file: %s \n",name_file);
        return NULL;
        }

    return file;

}

FILE* write_to_file(const char *name_file){
    FILE* file = fopen(name_file,"ab");
    if(file == NULL){
        printf("Failed to open file: %s \n",name_file);
        return NULL;
        }

    return file;
}

int close_file(FILE* file){

    int close_file = fclose(file);

    if(close_file == EOF){
        printf("Failed close file!!!!");
        return EXIT_FAILURE;
        }
    return close_file;
}

void print_matrix(long long N[][MAX_FILE]){
    for(int i = 0; i < 15;i++){
        for(int j = 0;j<MAX_FILE;j++){
            printf("N[%d][%d] = %lli\n",i,j,N[i][j]);
        }
    }
}
