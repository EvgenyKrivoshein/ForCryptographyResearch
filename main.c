#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_LEN 1000000

void removeComments(char *str) 
{
	typedef enum { TEXT, C_COMMENT, CPP_COMMENT, S_LITERAL } state_t;
	char cur, prev;
	size_t length, cmntBegin;
	state_t state = TEXT;
	
	if (str == NULL)
		return;

	length = strlen(str);
	++length;				// we want look at '\0'
	
	cmntBegin = 0;
	prev = str[0];
	for (size_t i = 1; i < length; ++i)
	{
		cur = str[i];

		switch (cur)
		{
		case '"':				//to avoid comment patterns inside string literals
			if (state == TEXT && prev != '\'' && prev != '\\')
			{
				state = S_LITERAL;
			}
			else if (state == S_LITERAL && prev != '\\' && prev != '\'')
			{
				state = TEXT;
			}
			break;

		case '/':
			if (state == TEXT && prev == '/')
			{
				state = CPP_COMMENT;
				cmntBegin = i - 1;
			}
			else if (state == C_COMMENT && prev == '*' && (i - cmntBegin > 2)) // last statement for avoiding /*/ pattern
			{
				state = TEXT;
				memmove(str + cmntBegin, str + i + 1, length - i);
				length -= i - cmntBegin;
				i = cmntBegin;
				cur = str[i];
			}
			break;

		case '*':
			if (state == TEXT && prev == '/')
			{
				state = C_COMMENT;
				cmntBegin = i - 1;
			}
			break;

		case '\0':
		case '\n':
			if (state == CPP_COMMENT)
			{
				state = TEXT;
				memmove(str + cmntBegin, str + i, length - i);
				length -= i - cmntBegin;
				i = cmntBegin;
				cur = str[i];
			}
			break;
		
		default:
			break;
		}

		prev = cur;
	}
}

/*
* Removes comments from this source file
* and prints result to the "mainNoComments.c"
*/
void removeCommentsFromThisFile()
{
	char str[MAX_LEN];
	char c;
	int i;

	FILE * pFile = fopen("Task.c", "r");
	for (i = 0; (c = getc(pFile)) != EOF; ++i)
	{
		str[i] = c;
	}
	str[i] = '\0';
	fclose(pFile);

	removeComments(str);

	pFile = fopen("TaskNoComments.c", "w");
	fprintf(pFile, "%s", str);
	fclose(pFile);
}

void test()
{
	char testString0[] = "";
	removeComments(testString0);
	assert(!strcmp(testString0, ""));

	char testString1[] = "//";
	removeComments(testString1);
	assert(!strcmp(testString1, ""));

	char testString2[] = "/ / ";
	removeComments(testString2);
	assert(!strcmp(testString2, "/ / "));

	char testString3[] = "//////\n";
	removeComments(testString3);
	assert(!strcmp(testString3, "\n"));

	char testString4[] = "abc//def\nghi";
	removeComments(testString4);
	assert(!strcmp(testString4, "abc\nghi"));
	
	char testString5[] = " \" inside /* a string literal */ it is not //a comment \" ";
	removeComments(testString5);
	assert(!strcmp(testString5, " \" inside /* a string literal */ it is not //a comment \" "));

	char testString6[] = "/**/";
	removeComments(testString6);
	assert(!strcmp(testString6, ""));

	char testString7[] = "/*\n*\n*\n*/";
	removeComments(testString7);
	assert(!strcmp(testString7, ""));

	char testString8[] = "ab/*c \n d \n */ e";
	removeComments(testString8);
	assert(!strcmp(testString8, "ab e"));

	char testString9[] = "/*/";
	removeComments(testString9);
	assert(!strcmp(testString9, "/*/"));

	char testString10[] = "comment\n/*inside \n //another \n*/\ncomment";
	removeComments(testString10);
	assert(!strcmp(testString10, "comment\n\ncomment"));

	char testString11[] = "comments//comments//again/*here*/\n!";
	removeComments(testString11);
	assert(!strcmp(testString11, "comments\n!"));

	char testString12[] = "ab \n/*\n//one more\n*/\ncd//and more\n";
	removeComments(testString12);
	assert(!strcmp(testString12, "ab \n\ncd\n"));

	char testString13[MAX_LEN];
	testString13[0] = '/';
	testString13[1] = '*';
	for (int i = 2; i < MAX_LEN; ++i)
	{
		testString13[i] = 'c';
	}
	testString13[MAX_LEN - 3] = '*';
	testString13[MAX_LEN - 2] = '/';
	testString13[MAX_LEN - 1] = '\0';
	removeComments(testString13);
	assert(!strcmp(testString13, ""));	//still works
}

main(void)
{
	test();
	removeCommentsFromThisFile();
}