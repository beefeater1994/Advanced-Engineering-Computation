#include <stdio.h>
#include <stdlib.h>
#include "simplebitmap.h"
#include "yspng.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
	SimpleBitmap test;
	if (false == test.LoadPng(argv[1]))
	{
		printf("Error: Failed to read a .PNG file.\n");
	}
	if (argc == 1) {
		printf("Usage: ps3_1 <pngFileName.png> \n");
	}
	else {
		SimpleBitmap cut;
		int width = 0;
		int height = 0;
		if (test.GetWidth() % 40 == 0) {
			width = test.GetWidth() / 40;
		}
		else {
			width = test.GetWidth() / 40 + 1;
		}
		if (test.GetHeight() % 40 == 0) {
			height = test.GetHeight() / 40;
		}
		else {
			height = test.GetHeight() / 40 + 1;
		}

		int number = 0;
		int flag = 0;
		for (int i = 0; i < height; i++) {
			if (flag == 1) {
				break;
			}
			for (int j = 0; j < width; j++) {
				if (number >= 200) {
					flag = 1;
					break;
				}
				cut = test.CutOut(40 * j, 40 * i, 40, 40);
				char buffer[10];
				sprintf(buffer, "%d.png", number);
				FILE *fp = fopen(buffer, "wb");
				cut.SavePng(fp);
				fclose(fp);
				number++;
			}
		}
		printf("%d", height);
		printf("%d", width);
	}
	
	
	return 0;
}