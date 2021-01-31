#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <png.h>
#include <string.h>
#include "image.h"
#include "ysa.h"
#include <dirent.h>


char data_file[50];

int main(){
	char name[5];
	float f=0.5,k;
	//FILE *fp;
	//DIR *dir;
 	//struct dirent *dirent;
	image img;
	ysa ys;
	img_init(&img);
	ysa_init(&ys);

	ysa_data dt;
	ysa_data_info dt_info;

	// Başlıyoruz

	// Resim ismini al
	printf("Resim adını girin:");
	scanf("%s",img.file);

	// Resim dosyasını aç ve oku
	printf("Resim okunuyor...\n");
	strcpy(img.folder ,EXAMPLE_FOLDER);
	strcat(img.file,".bmp");
	img_read(&img);


	// Resim hash işlemi
	printf("Resim hash işlemi...\n");
	img_trim(&img);


	// Image hash convert to input data
	printf("Hash giriş olarak ayarlanıyor...\n");
	ysa_convertMapToInput(&img,&ys);

   //	dir = opendir (data_folder);

	ysa_data_info_load(&dt_info);


   	while(ysa_data_load(&dt,&dt_info)==1){

		ysa_network_reset(&ys);
		
   		memcpy(&ys.net,&dt.net,sizeof(ysa_network));

		printf("[%s]Ağ çalışıyor...\n",dt.name);
		ysa_run(&ys);


		for(int j=0;j<ys.outputsize;j++){

			k = (1 - YSA_ERROR ) - ys.o[j];

			if(k < 0) k = k * -1;

			if(k < f) { strcpy(name,dt.name); f=k; printf("|||||||||||||||||--------->>>>>>");}

			printf("%s Output %d => %f [%f] \n",data_file,j,ys.o[j],k ); 

		}

   	}

   	printf("------------------------------------------------\n");
    printf("%s dosyasındaki sayı : %s \nHata payı : %f \n",img.file, name,f);

	img_free(&img);
	ysa_free(&ys);
	return 0;
}