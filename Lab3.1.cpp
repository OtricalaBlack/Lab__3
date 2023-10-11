#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char inputFileName[] = "lear.txt"; // Файл для загрузки очереди 
char outputFileName[] = "lear.txt"; // Файл для сохранения очереди

struct node {
	char inf[256];      // полезная информация
	int priority;       // приоритет
	struct node* next;  // ссылка на следующий элемент
};

struct node* head = NULL, * last = NULL, * f = NULL; // указатели на первый и последний элементы списка
int len = 0;

struct node* get_struct(void) {
	struct node* p = NULL;
	char s[256];
	int priority;

	if ((p = (struct node*)malloc(sizeof(struct node))) == NULL) {  // выделяем память под новый элемент списка
		printf("Ошибка при распределении памяти\n");
		exit(1);
	}

	printf("Введите название объекта: \n");   // вводим данные
	scanf("%s", s);
	if (*s == 0) {
		printf("Запись не была произведена\n");
		return NULL;
	}
	strcpy(p->inf, s);

	printf("\nВведите приоритет объекта: \n");   // вводим приоритет
	scanf("%d", &priority);
	p->priority = priority;

	p->next = NULL;

	return p;       // возвращаем указатель на созданный элемент
}

/* Последовательное добавление в список элемента (в соответствии с приоритетом) */
void spstore(void) {
	struct node* p = NULL;
	p = get_struct();
	if (head == NULL && p != NULL) {    // если списка нет, то устанавливаем голову списка
		head = p;
		last = p;
	}
	else if (head != NULL && p != NULL) { // список уже есть, то вставляем в соответствии с приоритетом
		struct node* current = head;
		struct node* prev = NULL;

		while (current != NULL && p->priority >= current->priority) {
			prev = current;
			current = current->next;
		}

		if (prev == NULL) {
			// Вставляем в начало списка
			p->next = head;
			head = p;
		}
		else {
			// Вставляем между prev и current
			prev->next = p;
			p->next = current;
			if (current == NULL) {
				last = p; // Если p вставлен в конец списка, обновляем last
			}
		}
	}
}

/* Просмотр содержимого списка. */
void review(void) {
	struct node* struc = head;
	if (head == NULL) {
		printf("\nСписок пуст\n");
	}
	while (struc) {

		printf("Имя - %s, Приоритет - %d\n", struc->inf, struc->priority);
		struc = struc->next;
	}
}

/* Поиск элемента по содержимому. */
struct node* find(char* name) {
	struct node* struc = head;
	if (head == NULL) {
		printf("\nСписок пуст\n");
	}
	while (struc) {
		if (strcmp(name, struc->inf) == 0) {
			return struc;
		}
		struc = struc->next;
	}
	printf("\nЭлемент не найден\n");
	return NULL;
}

/* Удаление элемента по содержимому. */
void del(char* name) {
	struct node* struc = head; // указатель, проходящий по списку установлен на начало списка
	struct node* prev = NULL;  // указатель на предшествующий удаляемому элементу
	int flag = 0;             // индикатор отсутствия удаляемого элемента в списке

	if (head == NULL) // если голова списка равна NULL, то список пуст
	{
		printf("\nСписок пуст\n");
		return;
	}

	if (strcmp(name, struc->inf) == 0) // если удаляемый элемент - первый
	{
		flag = 1;
		head = struc->next; // устанавливаем голову на следующий элемент
		free(struc);  // удаляем первый элемент
		struc = head; // устанавливаем указатель для продолжения поиска
	}
	else {
		prev = struc;
		struc = struc->next;
	}

	while (struc) // проход по списку и поиск удаляемого элемента
	{
		if (strcmp(name, struc->inf) == 0) // если нашли, то
		{
			flag = 1;         // выставляем индикатор
			if (struc->next)  // если найденный элемент не последний в списке
			{
				prev->next = struc->next; // меняем указатели
				free(struc);        // удаляем  элемент
				struc = prev->next; // устанавливаем указатель для продолжения поиска
			}
			else            // если найденный элемент последний в списке
			{
				prev->next = NULL; // обнуляем указатель предшествующего элемента
				free(struc);    // удаляем  элемент
				last = prev;    // обновляем last
				return;
			}
		}
		else    // если не нашли, то
		{
			prev = struc; // устанавливаем указатели для продолжения поиска
			struc = struc->next;
		}
	}

	if (flag == 0) // если флаг = 0, значит нужный элемент не найден
	{
		printf("\nЭлемент не найден\n");
		return;
	}
}

void SaveFile(const char* filename) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("\nОшибка при открытии файла для записи\n");
		return;
	}

	struct node* struc = head;
	while (struc) {
		fprintf(file, "%s %d\n", struc->inf, struc->priority);
		struc = struc->next;
	}

	fclose(file);
	printf("\nОчередь успешно сохранена в файл %s\n", filename);
}

void LoadFile(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("\nОшибка при открытии файла для чтения\n");
		return;
	}

	// Очистка существующей очереди
	struct node* current = head;
	while (current != NULL) {
		struct node* next = current->next;
		free(current);
		current = next;
	}
	head = last = NULL;

	char inf[256];
	int priority;
	while (fscanf(file, "%s %d", inf, &priority) != EOF) {
		struct node* p = (struct node*)malloc(sizeof(struct node));
		if (p == NULL) {
			printf("\nОшибка при распределении памяти\n");
			fclose(file);
			exit(1);
		}
		strcpy(p->inf, inf);
		p->priority = priority;
		p->next = NULL;

		// Вставка элемента в отсортированную очередь
		if (head == NULL || priority < head->priority) {
			// Вставка в начало
			p->next = head;
			head = p;
		}
		else {
			struct node* current = head;
			struct node* prev = NULL;
			while (current != NULL && priority >= current->priority) {
				prev = current;
				current = current->next;
			}
			// Вставка между prev и current
			prev->next = p;
			p->next = current;
			if (current == NULL) {
				last = p; // Обновление last, если p вставлен в конец
			}
		}
	}

	fclose(file);
	printf("\nОчередь загружена из файла %s\n", filename);
}

int main() {
	setlocale(LC_ALL, "Rus");
	int choice;
	char name[256];
	struct node* found = NULL;

	while (true) 
	{
		printf("\n1. Просмотреть очередь\n");
		printf("2. Добавить элемент в очередь\n");
		printf("3. Найти элемент\n");
		printf("4. Удалить элемент\n");
		printf("5. Сохранить очередь в файл\n");
		printf("6. Загрузить очередь из файла\n");
		printf("7. Выход\n");
		printf("\nВыберите операцию: ");
		scanf("%d", &choice);
		printf("\n");

		switch (choice) 
		{
		case 1:
			review();
			break;
		case 2:
			spstore();
			break;
		case 3:
			printf("Введите название объекта для поиска: ");
			scanf("%s", name);
			found = find(name);
			if (found != NULL) {
				printf("Найденный элемент: Имя - %s, Приоритет - %d\n", found->inf, found->priority);
			}
			break;
		case 4:
			printf("Какой объект удалит?: ");
			scanf("%s", name);
			del(name);
			break;
		case 5:
			SaveFile(outputFileName); // Функция сохранения очереди в файл
			break;
		case 6:
			LoadFile(inputFileName); // Функция загрузки очереди из файла
			break;
		case 7:
			exit(0);
		default:
			printf("Неверный выбор. Пожалуйста, выберите снова.\n");
		}
	}
	return 0;
}
