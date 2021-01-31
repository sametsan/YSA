#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <png.h>
#include <string.h>


void img_init(image *img){
	
	remove(INPUTS_FILE);
}

void img_free(image *img){

	free(img->hash);
	free(img->data);
	free(img->gray);
}


void img_read(image *img){

	char imgfile[150];
	BMPHeader header;
	unsigned char info[56];

	strcpy(imgfile,img->folder);
	strcat(imgfile,"/");
	strcat(imgfile,img->file);

	printf("%s file openning ...\n", imgfile);

	FILE* f = fopen(imgfile, "rb");

	fread(&info[2], sizeof(unsigned char), 54, f); // read the 54-byte header

	memcpy(&header,info,56);

	img->width = header.width_px;
	img->height = header.height_px;
	img->datasize = header.image_size_bytes;

	printf("Görüntü genişlik %d yükseklik %d kayıt başlangıcı %d boyut %d \n",img->width,img->height,header.offset,img->datasize);

	fseek(f,header.offset,SEEK_SET);

	img->data = (unsigned char*)calloc(img->datasize,sizeof(unsigned char)); // allocate 3 bytes per pixel
	fread(img->data, sizeof(unsigned char), img->datasize, f); // read the rest of the data at once
	fclose(f);


}


void img_to_gray(image *img){

	printf("Görüntü griye çevriliyor...\n");
	int R,G,B,C,A;

	img->gray = (unsigned char**) calloc(img->height,sizeof(unsigned char*));
	for(int i=0;i<img->width;i++)
		img->gray[i] = (unsigned char*) calloc(img->width,sizeof(unsigned char));

	int a=0;

	for(int i=0;i< img->height;i++){
		for(int j=0;j<img->width;j++){

			B = 0.0722 * img->data[a]; a++; 
			G = 0.7152 * img->data[a]; a++; 
			R = 0.2126 * img->data[a]; a++; 
			A = 0 * img->data[a]; a++;

			C = R + G + B + A;

			img->gray[i][j] = C;


		}
	}
   
}


void img_trim(image *img){

	img_map(img);
	int total;
	int top,left,right,bottom;


	for(int i=0;i< img->height;i++){
		total=0;
		for(int j=0;j< img->width;j++){
			total+= img->map[i][j];
		}

		if(total!=0){
			top=i;
			break;
		}

	}


	for(int i=img->height-1 ;i >=0 ;i--){
		total=0;
		for(int j=0;j< img->width;j++){
			total+= img->map[i][j];
		}

		if(total!=0){
			bottom=i;
			break;
		}

	}

	for(int i=0;i< img->width;i++){
		total=0;
		for(int j=0;j<img->height;j++){
			total+= img->map[j][i];
		}


		if(total!=0){
			left = i;
			break;
		}

	}

	for(int i=img->width-1;i>=0;i--){
		total=0;
		for(int j=0;j<img->height;j++){
			total+= img->map[j][i];
		}

		if(total!=0){
			right = i;
			break;
		}

	}

	img->trim_width = right- left;
	img->trim_height = bottom - top;

	img->trim = (unsigned char **) calloc(img->trim_height,sizeof(unsigned char*));

	for(int i=0;i< img->trim_height;i++)
		img->trim[i] = (unsigned char *) calloc(img->trim_width,sizeof(unsigned char));



	for(int i=0; i <img->trim_height; i++){
		for(int j=0;j< img->trim_width;j++){

			img->trim[i][j] = img->map[i+top][j+left];
		}
	}
/*
	for(int i=0; i <img->trim_height; i++){
		for(int j=0;j< img->trim_width;j++){

			printf("%d|",img->trim[i][j]);
		}
		printf("\n");
	}


*/
	unsigned char temp[IMG_HEIGHT][IMG_WIDTH];
	
	float oranH = img->height / (float)img->trim_height;
	float oranW = img->width / (float)img->trim_width;
	int h,w;

	for(int i=0; i < img->height; i++){
		for(int j=0;j< img->width;j++){

			if((h=i/oranH) >= img->height) break;
			if((w=j/oranW) >= img->width) break;
			temp[i][j]= img->trim[h][w];
			//printf("%d|",temp[i][j]);
		}		//printf("\n");
	}


	for(int i=0; i < img->height; i++){
		memcpy(img->map[i],temp[i],img->width);
	}


	for(int i=0;i< img->height;i++){

		for(int j=0;j<img->width;j++){

			printf("%d|", img->map[i][j]);			
		}	printf("\n");
	}



	//printf("\033[%d;%dH", (y), (x));


}



void img_hash(image *img){

	img_map(img);

	int filter[5][5]={
		{3,2,1,2,3},
		{2,2,1,2,2},
		{1,1,1,1,1},
		{2,2,1,2,2},
		{3,2,1,2,3}
	};

	int h=5;
	int w=5;
	int a=0;

	int count = (img->width/w) *(img->height/h);

	img->hashsize = count;

	img->hash = (int *) calloc(count,sizeof(int));

	for(int i=0;i< img->height-h;i+=h){

		for(int j=0;j<img->width-w;j+=w){


				for(int fi=0;fi< h;fi++){

					for(int fj=0;fj<w;fj++){

						
						img->hash[a] += img->map[i+fi][j+fj] * filter[fi][fj];
					}
				}
				a++;
		}
	}
}


void img_maptagram(image *img){

	img_map(img);

	printf("Maptagram oluşturuluyor...\n" );
	img->maptagramX = (int*) calloc(img->width,sizeof(int));
	img->maptagramY = (int*) calloc(img->height,sizeof(int));

	for(int i=0;i< img->height;i++){
		for(int j=0;j< img->width;j++){
			if(img->map[i][j]){
				img->maptagramX[i]++;
				img->maptagramY[j]++;
			}
		}
	}

	printf("Maptagram kaydediliyor...\n");

	char path[70];
	strcpy(path,MAPTAGRAM_FOLDER);
	strncat(path,img->file,strlen(img->file)-4);

	FILE* f = fopen(path, "wb");
	fwrite(img->maptagramY, sizeof(int),img->height, f);
	fwrite(img->maptagramX, sizeof(int),img->width, f);
	fclose(f);
}


void img_map(image *img){

	img_to_gray(img);
	printf("Map oluşturuluyor...\n");

	img->map = (char  **) calloc(img->height ,sizeof(char *));
	for(int i=0;i<img->width;i++)
		img->map[i] = (char *) calloc(img->width,sizeof(char ));

	int a=0;

	for(int i=0;i< img->height;i++){
			
		a++;

		for(int j=0;j<img->width;j++){
	
			if(img->gray[i][j] < (255/2)){ 
				img->map[i][j]=1;
			}
			else
				img->map[i][j]=0;
		}
	}


	printf("Map oluşturuldu.\n");
	/*

	float* temp = (float *) calloc(img->width ,sizeof(float));

	for(int i=0;i< img->hashsize;i+=img->width){

		memcpy(temp,&img->hash[i],img->width);

		if(temp > 0) printf("\n %d-",i); else printf("\n0-");

		for(int j=0;j<img->width;j++)
			printf("%f ", temp[j]);

	}

	free(temp);
*/


	//while(1);

}


void img_mapsave(image *img){
	char path[70];
	strcpy(path,MAPS_FOLDER);
	strncat(path,img->file,strlen(img->file)-4);
/*
	for (int i = 0; i < img->hashsize; ++i)
	{
	printf("%d-%d|",i, img->hash[i]);	}
*/
	FILE* f = fopen(path, "wb");
	for(int i=0;i< img->height;i++)
		fwrite(img->map[i], sizeof(char),img->width, f);
	fclose(f);

}


void img_hashsave(image *img){

	char path[70];
	strcpy(path,HASH_FOLDER);
	strncat(path,img->file,strlen(img->file)-4);

	FILE* f = fopen(path, "wb");
	fwrite(img->hash, sizeof(int),img->hashsize, f);
	fclose(f);

}
