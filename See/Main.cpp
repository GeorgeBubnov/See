#define _CRT_SECURE_NO_WARNINGS
//#include "stdafx.h"
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <cstdio>
#include <stdio.h>
#include <windows.h>

typedef struct {
	char types_of_products[30];
	char units[30];
	float y1913, y1928, y1940, y1955;
} table;

char input_way[150];

bool isalpha_rus(char c) {
	return c >= 'а' && c <= 'я' || c >= 'А' && c <= 'Я'
		|| c == 'ё' || c == 'Ё' || c >= 'A' && c <= 'Z';
}

int vvod(FILE* input)
{
	int key;
	int i = 0;
	table product;

	do {
		do {
			printf("\nВведите название продукта: ");
			scanf("%s", &product.types_of_products);
			fflush(stdin);
		} while (isalpha_rus(*product.types_of_products) == 0);
		do {
			printf("Введите единицы измерения продукта '%s': ", product.types_of_products);
			scanf("%s", &product.units);
			fflush(stdin);
		} while (isalpha_rus(*product.units) == 0);
		do {
			printf("Введите количество продукта '%s' в 1913 году: ", product.types_of_products);
			fflush(stdin);
		} while (scanf("%f", &product.y1913) != 1);
		do {
			printf("Введите количество продукта '%s' в 1928 году: ", product.types_of_products);
		} while (scanf("%f", &product.y1928) != 1);
		do {
			printf("Введите количество продукта '%s' в 1940 году: ", product.types_of_products);
		} while (scanf("%f", &product.y1940) != 1);
		do {
			printf("Введите количество продукта '%s' в 1955 году: ", product.types_of_products);
		} while (scanf("%f", &product.y1955) != 1);

		fwrite(&product, sizeof(table), 1, input);

		do {
			printf("\nХотите ввести еще одну строку? (y/n): \n");
			key = _getch();
		} while (key != 'y' && key != 'Y' && key != 'n' && key != 'N');
		if (key == 'N' || key == 'n') {
			fputs("\n", input);
			return 0;
		}
	} while (true);
}

FILE* file_input(void)
{
	int key, key1;
	FILE* input;

	do {
		printf("Вы хотите ввести путь к файлу? (y/n): ");
		key = _getch();
	} while (key != 'Y' && key != 'y' && key != 'n' && key != 'N');
	if (key == 'y' || key == 'Y') {
		printf("\nУкажите путь к файлу: ");
		gets_s(input_way);
		if ((input = fopen(input_way, "r")) == NULL) {
			printf("Ошибка при открытии файла %s", input_way);
			do {
				printf("\nХотите создать файл с таким именем? (y/n)");
				key1 = _getch();
			} while (key1 != 'Y' && key1 != 'y' && key1 != 'n' && key1 != 'N');
			if (key == 'y' || key == 'Y') {
				if ((input = fopen(input_way, "w")) == NULL) {
					printf("Ошибка при открытии файла %s", input_way);
				}
				vvod(input);
				return input;
			}
			fclose(input);
		}
		else
			return input;
	}
	if ((input = fopen("in.txt", "w")) == NULL) {
		printf("Ошибка при открытии файла in.txt");
		return input;
	}

	vvod(input);
	strcpy(input_way, "in.txt");
	return input;
}

int file_output(FILE* input)
{
	FILE* output;
	table line, output_line;
	char name_max[60], out[100];
	double prir;
	float firstGrowth = 0, secondGrowth = 0;

	if ((input = fopen(input_way, "r")) == NULL) {
		printf("Ошибка при открытии файла %s", input_way);
		exit(1);
	}

	if ((output = fopen("out.txt", "w")) == NULL) {
		printf("Ошибка при открытии файла out.txt");
		exit(1);
	}

	fread(&line, sizeof(table), 1, input);
	strcpy(name_max, "   ");
	int i = 0;
	while (i < 7) {
		name_max[i] = line.types_of_products[i];
		i++;
	}
	while (!feof(input)) {
		if (strstr(line.units, "тыс.штук") != NULL) {
			if (line.y1928 == 0) {
				if (line.y1940 == 0)
					firstGrowth = 0;
				else
					firstGrowth = 100;
				if (line.y1955 == 0)
					secondGrowth = 0;
				else
					secondGrowth = 100;
			}
			else {
				firstGrowth = line.y1940 / line.y1928 * 100;
				secondGrowth = line.y1955 / line.y1928 * 100;
			}
			strcpy(name_max, line.types_of_products);
		}
		fread(&line, sizeof(table), 1, input);
	}
	fprintf(output, "Рост продукта %s с 1928 года по 1940 год: %lf\nРост продукта %s с 1928 года по 1955 год: %lf\n\n", name_max, firstGrowth, name_max, secondGrowth);

	fclose(output);

	if ((output = fopen("out.txt", "r")) == NULL) {
		printf("Ошибка при открытии файла out.txt");
		exit(1);
	}

	rewind(input);

	printf("Продукция\t| Единицы измерения |\t1913 год\t|\t1928 год\t|\t1940 год\t|\t1955 год\n");
	while (fread(&output_line, sizeof(table), 1, input)) {
		printf("\n%s\t|   %s   |\t%f\t|\t%f\t|\t%f\t|\t%f\t", output_line.types_of_products, output_line.units, output_line.y1913, output_line.y1928, output_line.y1940, output_line.y1955);
	}
	printf("\n");

	do {
		fgets(out, 100, output);
		printf("%s", out);
	} while (!feof(output));

	fclose(input);
	fclose(output);
	return 0;
}

int main(void)
{
	system("color F0");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("chcp 1251");
	system("cls");
	FILE* table;

	table = file_input();

	if (table == NULL) {
		printf("Ошибка при открытии файла %s\n", input_way);
		return 1;
	}

	fclose(table);

	file_output(table);

	system("pause");
	return 0;
}
