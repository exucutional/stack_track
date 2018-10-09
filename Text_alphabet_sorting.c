/*
 ============================================================================
 Name        : Text_alphabet_sorting.c
 Author      : Exdev
 Version     : 0.5
 ============================================================================
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>

#define MAX_LINES 50000
#define MAX_SIZE_LINE 100

char** Read_text(FILE *fin, int *nlines);
void Str_sort(char **text, int nlines);
void Text_out(FILE *fout, char **text, int nlines);
int Str_compare(const void *p1, const void *p2);


int main()
{
	int nlines = 0;

	FILE* fin;
	FILE* fout;
	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	char** text = NULL;
	text = Read_text(fin, &nlines);

	if (fin == NULL)
	{
		printf("Couldn't open input file\n");
		assert(1);
	}
	if(fout == NULL)
	{
		printf("Couldn't open output file\n");
		assert(1);
	}

	qsort(text, nlines, sizeof(char*), Str_compare);

	Text_out(fout, text, nlines);

	fclose(fin);
	return 0;
}

char** Read_text(FILE *fin, int *nlines)
{

	char** text = NULL;
	text = (char**) calloc(MAX_LINES, sizeof(char*) );


	char* line = NULL;
	line = (char*) calloc(MAX_SIZE_LINE, sizeof(char));

	while ( fgets(line, MAX_SIZE_LINE, fin) != NULL )
	{

		text[*nlines] = line;
		*nlines+= 1;
		line = (char*) calloc(MAX_SIZE_LINE, sizeof(char));
	}

	return text;
}

void Text_out(FILE *fout, char **text, int nlines)
{
	for (int i = 0; i < nlines; i++)
	{
		printf("%s", text[i]);
		fprintf(fout ,"%s", text[i]);
	}
}

void Str_sort(char **text, int nlines)
{
	char *ch_temp = NULL;
	bool sort = false;
	while (!sort)
	{
		sort = true;
		for (int i = 0; i < nlines - 1; i++)
		{
			if(!Str_compare(text[i], text[i + 1]))
			{
				ch_temp = text[i];
				text[i] = text[i + 1];
				text[i + 1] = ch_temp;
				sort = false;
			}
		}
	}
}

int Str_compare(const void *p1,const void *p2)
{
	int i = 0;
	int j = 0;
	const char *string1 = *(char**) p1;
	const char *string2 = *(char**) p2;
	while (string1[i] != '\0' && string2[j] != '\0')
	{
		while (!isalpha(string1[i]) ) {i++;}
		while (!isalpha(string2[j]) ) {j++;}
		if (string1[i] == string2[j])
		{
			i++;
			j++;
		} else
			return(string1[i] - string2[j]);
	}
	return 0;
}
