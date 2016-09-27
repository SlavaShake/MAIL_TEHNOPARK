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


#define MAX_WORDS 2000
#define MAX_FILE 15

FILE* open_file(const char *name_file);
FILE* write_to_file(const char *name_file);
int close_file(FILE* file);
long long how_much_symbols_in_file(FILE* file);
int sequal(const char *s1,const char *s2);
void write_to_array(char *A,const long long N,FILE* file);
char to_lower(char A);
void char_to_lowercase(char A[]);
long long how_much_words_in_file(const char *A,const long long N);
void squeeze(char s1[]);
void add_word_to_array_of_word(char words[MAX_WORDS][100],const char A[],const long long N);
void delete_arr(char *c);
int check_word_in_matrix(const char struct_word[MAX_WORDS][100],const char word[]);




int main(int argc, char *argv[])
{    
    if (argc < 3){
        printf("Usage: %s file_in file_1 and i.e.\n", argv[0]);
        return EXIT_FAILURE;
        }

    const char *file_input = argv[1];
    printf("File to write words: %s\n",file_input);

    //счетчик файлов коллекции
    int i = 1;

    //может содержать до 2000 слов для максимального
    //колличества файлов в коллекции равным 15
    char        word[MAX_WORDS][100];

    //колличество пападаний слова в файле
    long long   number_of_words[MAX_WORDS][MAX_FILE];

    //колличество всех слов в отдельном файле
    long long   number_of_words_in_file[MAX_FILE];

    //частота попаданий слова в отдельном файле
    double      TFi[MAX_WORDS][MAX_FILE];

    while(argv[i] != '\0'){


        //берем файл
        const char *file_collection = argv[i++];
        printf("File collection is %s\n",file_collection);

        //открываем его
        FILE* read_file = open_file(file_collection);
        if(read_file == NULL) return 0;

        //считаем сколько в нем символов
        long long N = how_much_symbols_in_file(read_file);

        //выделяем память место для хранения текста из фаила
        char *A = (char *)malloc (N + 1);
        if (A == NULL) return EXIT_FAILURE;

        //открываем его
        read_file = open_file(file_collection);

        //записываем все из файла в массив
        write_to_array(A,N,read_file);
        printf("write_to_array: %s\n\n", A);

        close_file(read_file);

        //Удаляет из строки s1 символы и повторяющиеся пробелы
        squeeze(A);
        printf("squeeze: %s\n\n", A);

        //приводим к нижнему регистру
        char_to_lowercase(A);
        printf("char_to_lowercase: %s\n", A);


        //сколько всего слов было в фале
        number_of_words_in_file[i-1] = how_much_words_in_file(A,N);
        printf("number_of_words_in_file = %lli\n",number_of_words_in_file[i-1]);

        //заполняем массив
        add_word_to_array_of_word(word,A,N);




        }





    return 0;
}


//заполняем массив не повторяющимися словами
void add_word_to_array_of_word(char words[MAX_WORDS][100],const char A[],const long long N){
    int j = 0;
    char word[100];
    for(long long i = 0; (i < N) || (A[i] != '\0');i++){
        if (A[i] == ' ' ){
            printf("A[%lli] = %s\n",i,word);
            if(check_word_in_matrix(words,word) == 1)
            strcpy(words[i],word);
            delete_arr(word);
        }
        else{
            word[j] = A[i];
            j++;
        }
    }

}

//проверяем на наличие слова в матрице
int check_word_in_matrix(const char struct_word[MAX_WORDS][100],const char word[]){

    for(int i = 0; (i < MAX_WORDS) || (struct_word[i] != '\0');i++){
        if(strcmp(struct_word[i],word) == 0){
            return 0;
        }
    }
    return 1;
}

//очистка промежуточного массива
void delete_arr(char *c){
    int i = 0;
    while(c[i] != '\0')
    {
        c[i++] = '\0';
    }
}

/* Подсчитываем количество слов в тексте */
long long how_much_words_in_file(const char *A,const long long N){

    long long i;
    long long k = 0;
    for (i = 0; (i < N) || (A[i] != '\0'); i++){
        if (isspace(A[i]) && !isspace(A[i+1]))
            k++;
    }
    printf("Kolichestvo slov v texste: %lli \n", k);

    return k;

}

//приведение к нижнему регистру
char to_lower(char A){
    if((A >= 'A') && (A <= 'Z'))
        return A + ('a' - 'A');
    return A;
}

//приведение к нижнему регистру
void char_to_lowercase(char A[]){

    int i = 0;
        while (A[i]!='\0')
        {
            A[i] = to_lower(A[i]);
            i++;
        }

}

//Удаляет из строки s1 символы
// и повторяющиеся пробелы
void squeeze(char s1[]){
     int i, k;
     for (i = k = 0; s1[i] != '\0'; i++){
         if(isspace(s1[i]) || ispunct(s1[i]) || ((48 <= s1[i]) && (s1[i] <= 57)))
             s1[k++] = ' ';
         else
             s1[k++] = s1[i];
     }
     s1[k] = '\0';

     for(i = k = 0;s1[k] != '\0';i++){
         if(i==0 && isspace(s1[i]))
             continue;
         if(isspace(s1[i]) && isspace(s1[i+1]))
             continue;
         else
             s1[k++] = s1[i];
         }
     s1[k]='\0';
}

/* Записываем символы из файла в массив */
void write_to_array(char *A,const long long N,FILE* file){

    while(!feof(file)){
        for (long long i = 0; i < N; i++){
                //fscanf(file,"%c",&A[i]);
                A[i] = fgetc(file);
//                printf("A[%lli] = %c\n",i, A[i]);
            }
    }

}

long long how_much_symbols_in_file(FILE* file){

    long long N = 0;
    while(!feof(file)){
            char tmp;
            tmp = fgetc(file);
            //if((fscanf(file,"%c", &tmp)) == 1){
            if(tmp){
                N++;
            }
        }
    printf("how_much_symbols_in_file = %lli\n",N);
    return N;
}

//сравнение строк по алфавиту
int sequal(const char *s1,const char *s2){
    while ( *s1 && *s2 ){
    if( *s1 < *s2 )
        return -1;
    if( *s1 > *s2 )
        return 1;
    ++s1;
    ++s2;
    }

return *s1? -1 : *s2? 1 : 0;//если s1 то s2 иначе 0
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
    FILE* file = fopen(name_file,"wb");
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
