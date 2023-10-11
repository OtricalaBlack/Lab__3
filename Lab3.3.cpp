#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 

struct node
{
	char inf[256];     // Полезная информация
	struct node* next; // Ссылка на следующий элемент
};

struct node* top = NULL; // Указатель на вершину стека

void push(void);      // Добавление элемента в стек
void pop(void);       // Удаление элемента из стека
void display(void);   // Просмотр содержимого стека
void saveToFile(void); // Сохранение данных стека в файл
void loadFromFile(void); // Загрузка данных стека из файла

struct node* get_struct(void)
{
	struct node* p = NULL;
	char s[256];

	if ((p = (struct node*)malloc(sizeof(struct node))) == NULL) // Выделяем память под новый элемент стека
	{
		printf("Ошибка при распределении памяти\n");
		exit(1);
	}

	printf("Введите название объекта: \n"); // Вводим данные
	scanf("%s", s);
	if (*s == 0)
	{
		printf("Запись не была произведена\n");
		return NULL;
	}
	strcpy(p->inf, s);

	p->next = NULL;

	return p; // Возвращаем указатель на созданный элемент
}

/* Добавление элемента в стек */
void push(void)
{
	struct node* p = NULL;
	p = get_struct();
	if (p != NULL)
	{
		p->next = top;
		top = p;
	}
}

/* Удаление элемента из стека (с вершины) */
void pop(void)
{
	if (top == NULL)
	{
		printf("Стек пуст\n");
		return;
	}

	struct node* temp = top;
	top = top->next;
	free(temp);
}

/* Просмотр содержимого стека */
void display(void)
{
	struct node* struc = top;
	if (top == NULL)
	{
		printf("Стек пуст\n");
	}
	while (struc)
	{
		printf("%s \n", struc->inf);
		struc = struc->next;
	}
}

/* Сохранение данных стека в файл */
void saveToFile(void)
{
	FILE* file;
	struct node* struc = top;

	if (top == NULL)
	{
		printf("Стек пуст, нет данных для сохранения\n");
		return;
	}

	file = fopen("stack.txt", "w");
	if (file == NULL)
	{
		printf("Ошибка при открытии файла\n");
		return;
	}

	while (struc)
	{
		fprintf(file, "%s\n", struc->inf);
		struc = struc->next;
	}

	fclose(file);
	printf("Данные стека успешно сохранены в файле\n");
}

/* Загрузка данных стека из файла */
void loadFromFile(void)
{
	FILE* file;
	char s[256];

	file = fopen("stack.txt", "r");
	if (file == NULL)
	{
		printf("Файл не найден или ошибка при открытии\n");
		return;
	}

	while (fscanf(file, "%s", s) != EOF)
	{
		struct node* p = (struct node*)malloc(sizeof(struct node));
		if (p == NULL)
		{
			printf("Ошибка при распределении памяти\n");
			exit(1);
		}
		strcpy(p->inf, s);
		p->next = top;
		top = p;
	}

	fclose(file);
	printf("Данные из файла успешно загружены в стек\n");
}

int main()
{
	setlocale(LC_ALL, "Rus");
	int choice;

	while (1)
	{
		printf("\nОперации над стеком:\n");
		printf("1. Добавить элемент в стек\n");
		printf("2. Удалить элемент из стека\n");
		printf("3. Просмотреть содержимое стека\n");
		printf("4. Сохранить данные стека в файл\n");
		printf("5. Загрузить данные стека из файла\n");
		printf("6. Выход\n");
		printf("Введите выбор: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			push();
			break;
		case 2:
			pop();
			break;
		case 3:
			display();
			break;
		case 4:
			saveToFile();
			break;
		case 5:
			loadFromFile();
			break;
		case 6:
			exit(0);
		default:
			printf("Неправильный выбор\n");
		}
	}
	return 0;
}
