// Удалить заданное ребро и в модифицированном графе найти вершину с максимальной степенью
// визуализировать граф

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct line //структура смежных вершин (т.е. структура рёбер), сюды записываем из файлика graph
{
    int node1; // вершина 1
    int node2; // вершина 2, смежная вершине 1
};

struct node //структура уникальных вершин
{
    int number;   // вершина
    int deg; // степень вершины
};

int length = 0, uniq_count = 0; // длинна массива структур ребер и длмнна массива структур уникальных вершин

// считывание количества строк в прикреплённом файлике
size_t get_length (const char *filename)
{
    size_t size = 0;
    FILE *file;
    file = fopen(filename, "r");
    if ((file = fopen(filename, "r")) == NULL)
    {
        printf("Foner read error!\n");
        exit(1);
    }
    while (!feof(file))
    {
        if (fgetc(file) == '\n')
            size++;
    }
    size++;
    return size;
}

// заполнение массива структуры смежных вершин чиселками из прикреплённого файлика
void get_lines(struct line *lines, const char *filename)
{
    int l = 0;
    FILE *file;
    file = fopen(filename, "r");
    while (fscanf(file, "%d--%d", &(lines[l].node1), &(lines[l].node2)) != EOF)
    {
        l++;
    }
}

// удаление ребра
void removing_edge (int l, struct line *lines, int v1, int v2, bool* fl) //длинна масива, масив, две вершины, образующих ребро, которое надо удалить
{
    *fl = 0;
    for (int i=0; i<l; i++)
    if (((lines[i].node1 == v1) && (lines[i].node2 == v2)) || ((lines[i].node1 == v2) && (lines[i].node2 == v1)))
    {
        for (i; i < l-1; i++)
        {
            lines[i].node1=lines[i+1].node1;
            lines[i].node2=lines[i+1].node2;
        }
        *fl = 1;
    }

}

// запись модифицированного графа в новый файл
void new_graph (int l, struct line *lines, const char *filename)
{
    FILE *file; 
    file = fopen("filename", "w"); //открыли файл для записи модифицированного графа

    //проверка на то открылся ли файл
    if (
        (file = fopen("filename", "w+")) == NULL)
    {
        printf("Foner read error!\n"); //если ошибка, то печатаем ошибку
        exit(1);
    }
    else //если файл успешно открыт, то записываем массив структур в файл
    {
        for (int i=0; i<(l-1); i++)
        {
            fprintf(file, "%d", lines[i].node1);
            fprintf(file, "%s", "--");
            fprintf(file, "%d\n", lines[i].node2);
        }
        fprintf(file, "%d", lines[l].node1);
        fprintf(file, "%s", "--");
        fprintf(file, "%d", lines[l].node2);
    }
    fclose(file);  

    //выведем в консоль файлик c модифицированным графом  
    char c[10];
    file = fopen("filename", "r"); //открыли файл для записи модифицированного графа
    printf("graph after edge removal\n");
    while (!feof(file))
        {
            if ((fgets(c, 100, file)) != NULL) //функция fgets возвращает 0 в случае ошибки чтения, иначе - адресс массива символос, которые считались из файла
            printf("%s", c);
        }
    fclose(file);
}



// заполнение массива структур уникальных вершин, степень каждой вершины примем равной нулю
int get_uniq_node(int l, struct line *lines, struct node *nodes)
{
    int i = 0, c = 0; // с - количество уникальных вершин, т.е. длина массива структур уникальных вершин
    for (i = 0; i < l; i++)
    {
        int j;
        for (j = 0; j < c; j++)
        {
            if (lines[i].node1 == nodes[j].number)
                break;
        }
        if (j == c)
        {
            nodes[c].number = lines[i].node1;
            nodes[c].deg = 0;
            c++;
        }
    }
    for (i = 0; i < l; i++)
    {
        int j;
        for (j = 0; j < c; j++)
        {
            if (lines[i].node2 == nodes[j].number)
                break;
        }
        if ((j == c) && (lines[i].node2 != 0))
        {
            nodes[c].number = lines[i].node2;
            nodes[c].deg = 0;
            c++;
        }
    }
    return c;
}

//поиск степени каждой вершины, вывод всех степеней
void deg_seach (struct line *lines, struct node *nodes)
{
    for (int i = 0; i < uniq_count; i++)
    {
        for (int j = 0; j < length; j++)
        if ((nodes[i].number == lines[j].node1) && (nodes[i].number == lines[j].node2))
            nodes[i].deg+=2;
        else if ((nodes[i].number == lines[j].node1) || (nodes[i].number == lines[j].node2))
            nodes[i].deg++;
    }

    printf ("vertexs: ");
    for (int i = 0; i < uniq_count; i++)
    printf ("%4d", nodes[i].number);
    printf ("\n");

    printf ("degrees: ");
    for (int i = 0; i < uniq_count; i++)
    printf ("%4d", nodes[i].deg);
    printf ("\n");
}

//поиск максимальной степени вершины
int max_deg_seach (struct node *nodes, int* max)
{
    int ver = 0;
    *max = nodes[0].deg;
    for (int i = 0; i < uniq_count; i++)
    if (nodes[i].deg > *max)
    {
        *max = nodes[i].deg;
        ver = nodes[i].number;
    }

    return ver;
}


// cоздание и заполнение Dotfile
void create_dot_file(const char *from_file, const char *to_file)
{
    char c;
    FILE *from, *to;
    from = fopen(from_file, "r"); 
    to = fopen(to_file, "w"); // открыли файл Dotfile.txt для записи
    // проверка открылся ли файл
    if (
        (to = fopen(to_file, "w+")) == NULL)
    {
        printf("Foner read error!\n"); // если ошибка, то печатаем ошибку
        exit(1);
    }
    else
    {
        fputs("graph G{\n", to);
        while ((c = getc(from)) != EOF)
        {
            if (c == '\n')
                putc(';', to);
            putc(c, to);
        }
        fputs(";\n}", to);
    }

    fclose(to);
    fclose(from);
}




int main(void)
{
    int i = 0;
    length = get_length("graph"); // считаем количество строк в файле graph и звписываем в переменную length
    bool fl = 0;

    struct line list_lines[length]; // соездаем массив структур с длинной равной количеству строк в файле
    get_lines(list_lines, "graph"); // заполняем его

    // вывод исходного графа для проверки
    printf("Тhis graph was read from the file 'graph' \n");
    for (i = 0; i < length; i++)
    {
        printf("%d--%d\n", list_lines[i].node1, list_lines[i].node2);
    }
    printf("\n");

    // ввод ребра, которое надо удалить (ввод двух вершин, образующих это ребро)
    int v1, v2;
    printf ("enter the vertices of the edge you want to delete\n");
    scanf ("%d%d", &v1, &v2);
    printf ("\n");
    printf ("the edge you want to remove: %d -- %d \n", v1, v2);

    // удаление ребра
    removing_edge (length, list_lines, v1, v2, &fl);
    if (fl == 1) //проверка на то, было ли произведено удаление или такого ребра в графе нет
    {
        // если удаление было, то создадим новый файл с модифицированным графом и выведем его
        length--; // уменьшим длинну массива структур ребер на 1, тк одно ребро удалили (последнее ребро повторяется)
        new_graph (length, list_lines, "filename"); 
        printf("\n");

    }
    else printf ("there is no such edge in the graph\n"); 

    
    // создаём массив структур уникальных вершин
    struct node uniq_nodes[2 * length];                         // длиной 2n т.к. это максимально возможная длина если все вершины разные
    uniq_count = get_uniq_node(length, list_lines, uniq_nodes); //заполняем его
    //выводим его
    printf("Unique vertices of this graph\n");
    for (i = 0; i < uniq_count; i++)
    {
        printf("%d ", uniq_nodes[i].number);
    }
    printf("\n");

    // найдём степень каждой вершины, выведем
    deg_seach (list_lines, uniq_nodes);
    int maxdeg = 0, ver = 0;
    ver = max_deg_seach (uniq_nodes, &maxdeg);
    printf ("the vertex %d has a maximum degree equal to %d\n", ver, maxdeg);


    //визуализация
    //создадим и заполним файл Dotfile.txt
    create_dot_file("graph", "Dotfile.gv");
    if (fl != 0)
    create_dot_file("filename", "Dotfile1.gv");

    return 0;
}
