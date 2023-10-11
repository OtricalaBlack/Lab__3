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

struct node* front = NULL, * rear = NULL; // Указатели на начало и конец очереди

void enqueue(void); // Добавление элемента в очередь
int dequeue(char* name); // Удаление элемента из очереди
void display(void); // Просмотр содержимого очереди
void saveToFile(void); // Сохранение данных очереди в файл
void loadFromFile(void); // Загрузка данных очереди из файла

struct node* get_struct(void)
{
	struct node* p = NULL;
	char s[256];

	if ((p = (struct node*)malloc(sizeof(struct node))) == NULL) // Выделяем память под новый элемент очереди
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

/* Добавление элемента в очередь */
void enqueue(void)
{
	struct node* p = NULL;
	p = get_struct();
	if (rear == NULL && p != NULL) // Если очередь пуста, то устанавливаем начало и конец очереди
	{
		front = p;
		rear = p;
	}
	else if (rear != NULL && p != NULL) // В очереди уже есть элементы, добавляем в конец
	{
		rear->next = p;
		rear = p;
	}
	return;
}

/* Удаление элемента из очереди (из начала) */
int dequeue(char* name)
{
	struct node* temp = front;
	struct node* prev = NULL;
	int flag = 0;

	if (front == NULL) // Если начало очереди равно NULL, то очередь пуста
	{
		printf("Очередь пуста\n");
		return 0;
	}
	if (strcmp(name, temp->inf) == 0)
	{
		flag = 1;
		front = temp->next; // мб тумп заменить на фронт
		free(temp);
	}
	else {
		prev = temp;
		temp = temp->next;
	}

	while (temp)
	{
		if (strcmp(name, temp->inf) == 0)
		{
			flag = 1;
			if (temp->next)
			{
				prev->next = temp->next;
				free(temp);
				temp = prev->next;
			}
			else
			{
				prev->next = NULL;
				free(temp);
				return 0;
			}
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}

	if (flag == 0)
	{
		printf("Элемент не найден");
		return 0;
	}
}

/* Просмотр содержимого очереди */
void display(void)
{
	struct node* struc = front;
	if (front == NULL)
	{
		printf("Очередь пуста\n");
	}
	while (struc)
	{
		printf("%s \n", struc->inf);
		struc = struc->next;
	}
	return;
}

/* Сохранение данных очереди в файл */
void saveToFile(void)
{
	FILE* file;
	struct node* struc = front;

	if (front == NULL)
	{
		printf("Очередь пуста, нет данных для сохранения\n");
		return;
	}

	file = fopen("queue.txt", "w");
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
	printf("Данные очереди успешно сохранены в файле\n");
}

/* Загрузка данных очереди из файла */
void loadFromFile(void)
{
	FILE* file;
	char s[256];

	file = fopen("queue.txt", "r");
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
		p->next = NULL;

		if (rear == NULL)
		{
			front = p;
			rear = p;
		}
		else
		{
			rear->next = p;
			rear = p;
		}
	}

	fclose(file);
	printf("Данные из файла успешно загружены в очередь\n");
}

int main()
{
	setlocale(LC_ALL, "Rus");
	int choice;
	char name[256];

	while (1)
	{
		printf("\nОперации над очередью:\n");
		printf("1. Добавить элемент в очередь\n");
		printf("2. Удалить элемент из очереди\n");
		printf("3. Просмотреть содержимое очереди\n");
		printf("4. Сохранить данные очереди в файл\n");
		printf("5. Загрузить данные очереди из файла\n");
		printf("6. Выход\n");
		printf("Введите выбор: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			enqueue();
			break;
		case 2:
			printf("Какой элемент хотите удалить?: ");
			scanf("%s", &name);
			dequeue(name);
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