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



FILE* open_file(const char *name_file);
FILE* write_in_file(const char *name_file);
int close_file(FILE* file);
long long how_much_symbols_in_file(FILE* file);
int sequal(const char *s1,const char *s2);
void write_to_array(char *A,const long long N,FILE* file);
char to_lower(char A);
void char_to_lowercase(char A[]);
long long how_much_words_in_file(const char *A,const long long N);
void squeeze (char s1[],const char s2[]);


int main(int argc, char *argv[])
{
    //символы на удаление из массива
    char sym[260] = { '.', ',', ':', ';', '!', '^', '?', '#', '$', '%',
        '*', '|', '/', '\\', '~', '`', '"', '<', '>', '_', '-', '+', '=', '(', ')',
        '{', '}', '[', ']', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\n','\t','\v','\f','\r'};

    if (argc < 3){
        printf("Usage: %s file_in file_1 and i.e.\n", argv[0]);
        return EXIT_FAILURE;
        }

    const char *file_input = argv[1];
    printf("File to write words: %s\n",file_input);

    //счетчик файлов коллекции
    int i = 2;

    while(argv[i] != '\0'){


        //берем файл из коллекции
        const char *file_collection = argv[i++];
        printf("File collection is %s\n",file_collection);

        //открываем его
        FILE* read_file = open_file(file_collection);

        //считаем сколько в нем символов
        long long N = how_much_symbols_in_file(read_file);

        //выделяем память место для хранения текста из фаила
        char *A = (char *)malloc (N + 1);
        if (A == NULL) return EXIT_FAILURE;

        //открываем его
        read_file = open_file(file_collection);

        //записываем все из файла в массив
        write_to_array(A,N,read_file);
        printf("%s\n", A);

        //Удаляет из строки s1 все символы встречающиеся в строке s2
        squeeze(A, sym);
        printf("%s\n", A);

        //приводим к нижнему регистру
        char_to_lowercase(A);
        printf("%s\n", A);



        //сколько всего слов было в фале
        how_much_words_in_file(A,N);


        close_file(read_file);
        }





    return 0;
}



/* Подсчитываем количество слов в тексте */
long long how_much_words_in_file(const char *A,const long long N){

    long long i;
    long long k = 0;
    for (i = 0; i < N; i++){
        if (isspace(A[i]) && (isspace(A[i-1]) == 0))
            k++;
    }
    printf("Kolichestvo slov v texste: %lli \n", k - 1);

    return k-1;

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
            A[i] = to_lower (A[i]);
            i++;
        }

}

//Удаляет из строки s1 все символы встречающиеся в строке s2
void squeeze(char s1[],const char s2[]){

     int i, j, k;
     for (i = k = 0; s1[i] != '\0'; i++) {
         for (j = 0; s2[j] != '\0' && s2[j] != s1[i]; j++);
         if (s2[j] == '\0')
             s1[k++] = s1[i];
     }
     s1[k] = '\0';
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

FILE* write_in_file(const char *name_file){
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
