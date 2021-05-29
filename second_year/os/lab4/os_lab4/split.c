#pragma once
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include"split.h"
char** split(char* str, char sep) {
	char** res = NULL;
	if (res == NULL) {
		if (str == NULL)
			return res;
		int str_size = -1, substring_count = 0, i = 0, j = 0, was_sep = 1, substring_length = 0, start_index = 0;

		do {
			str_size++;
			if ((str[str_size] == sep || str[str_size] == '\0') && was_sep == 0) {
				was_sep = 1;
				substring_count++;
			}
			else if (str[str_size] != sep && str[str_size] != '\0')
				was_sep = 0;
		} while (str[str_size] != '\0');
		str_size++;

		if (substring_count == 0) return res;

		substring_count++;
		//printf("count:%d size:%d len:%d\n", substring_count, str_size, strlen(str));
		res = (char**)malloc(substring_count * sizeof(char*));
		//res = new char*[substring_count];
		res[substring_count - 1] = (char*)malloc(sizeof(char));
		//res[substring_count - 1] = new char[1];
		res[substring_count - 1][0] = '\0';
		for (int i = 0; i < str_size; i++) {
			//	printf("i:%d\n", i);
			substring_length++;
			if ((str[i] == sep || str[i] == '\0') && substring_length > 1) {
				//		printf("sub_length:%d j:%d ", substring_length, j);
				res[j] = (char*)malloc(substring_length * sizeof(char));
				//		res[j] = new char[substring_length];
				for (int k = i - substring_length + 1, l = 0; k < i; ++k, ++l) {
					res[j][l] = str[k];
				}
				res[j][substring_length - 1] = '\0';
				substring_length = 0;
				j++;
			}
			else if (str[i] == sep || str[i] == '\0') substring_length = 0;
		}
	}
	return res;
}