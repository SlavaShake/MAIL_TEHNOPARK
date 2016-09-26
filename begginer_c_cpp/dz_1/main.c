#include <stdio.h>
#include <stdlib.h>
#include <string.h>



FILE* file_open(const char *name_file);
FILE* file_write(const char *name_file);
int file_close(FILE* file);
long long how_much_symbols_in_file(FILE* file);
int sequal(const char *s1,const char *s2);
void write_to_massive(char *A,const long long N,FILE* file);
char to_lower(char A);
void to_litle_registr(char *A);


int main(int argc, char *argv[])
{
    //колличество символов в файле
    long long N = 0;

    //счетчик файлов коллекции
    int i = 2;

    if (argc < 3){
        printf("Usage: %s file_in file_1 and i.e.\n", argv[0]);
        return EXIT_FAILURE;
        }

    const char *file_input = argv[1];
    printf("File to write words: %s\n",file_input);

    while(argv[i] != '\0'){
        //обнуляем колличество символов
        N = 0;

        //место для хранения текста
        char *A;

        //берем файл из коллекции
        const char *file_collection = argv[i++];
        printf("File collection is %s\n",file_collection);

        //открываем его
        FILE* read_file = file_open(file_collection);

        //считаем сколько в нем символов
        N = how_much_symbols_in_file(read_file);

        //выделяем память под фаил
        A = malloc (sizeof (char) * (N + 1));

        //записываем все из файла в массив
        write_to_massive(A,N,read_file);


        file_close(read_file);
        }





    return 0;
}

//приведение к нижнему регистру
char to_lower(char A){
    if((A >= 'A') && (A <= 'Z'))
        return A + ('a' - 'A');
    return A;
}
//приведение к нижнему регистру
void to_litle_registr(char *A){

    int i = 0;
        while (A[i]!='\0')
        {
            A[i] = to_lower (A[i]);
            i++;
        }

}

void write_to_massive(char *A,const long long N,FILE* file){


    for (long long i = 0; i < N; i++){
            fscanf(file,"%c",&A[i]); /* Записываем символы из файла в массив */
            printf("%c", A[i]);
        }
}

long long how_much_symbols_in_file(FILE* file){

    long long N = 0;
    while(!feof(file)){
            char tmp;
            if((fscanf(file,"%c", &tmp)) == 1){
                N++;
            }
        }
    printf("how_much_word_in_file = %lli\n",N);
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

FILE* file_open(const char *name_file){
    FILE* file;
    file = fopen(name_file,"r");
    if(file == NULL){
        printf("Failed to open file: %s \n",name_file);
        return NULL;
        }

    return file;

}

FILE* file_write(const char *name_file){
    FILE* file = fopen(name_file,"wb");
    if(file == NULL){
        printf("Failed to open file: %s \n",name_file);
        return NULL;
        }

    return file;
}

int file_close(FILE* file){

    int close_file = fclose(file);

    if(close_file == EOF){
        printf("Failed close file!!!!");
        return EXIT_FAILURE;
        }
    return close_file;
}
