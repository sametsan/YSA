#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <png.h>
#include <string.h>
#include "image.h"
#include "config.h"

#ifndef YSA_H
#define YSA_H


typedef struct 
{
		// Weights
	//int wsize,wosize;
	float w[YSA_CELL][YSA_INPUT];
	float wo[YSA_OUTPUT][YSA_CELL];
}ysa_network;


typedef struct{
	
	char name[30];
	int id;
	ysa_network net;

}ysa_data;

typedef struct{

	int inputsize;
	int outputsize;
	int cellsize;
	int offset;

}ysa_data_info;



typedef struct 
{
	int inputsize;
	int outputsize;
	int cellsize;

	// Input Output
	float *input;
	//={0.1,0.2,0.3,0.4,0.5};
	float *output;
	float *fo;
	float *o;


	// Errors
	float *eo;
	float *e;

	// Weights
	ysa_network net;
	///float **w;
	//float **wo;


}ysa;


void ysa_init(ysa *ys);
void ysa_free(ysa *ys);
void ysa_convertMapToInput(image *img,ysa *ys);
void ysa_run(ysa *ys);
int ysa_isOk(ysa *ys);
void ysa_training(ysa *ys);

void ysa_network_reset(ysa *ys);
void ysa_output_set(ysa *ys,int id);
void ysa_data_create(ysa *ys);
void ysa_data_add(ysa *ys,int id,char *name);
int ysa_data_load(ysa_data *dt,ysa_data_info *info);
void ysa_data_info_load(ysa_data_info *info);
void ysa_convertHashToInput(image *img,ysa *ys);
void ysa_convertGrayToInput(image *img,ysa *ys);
void ysa_convertTrimToInput(image *img,ysa *ys);

#endif