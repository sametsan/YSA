#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <png.h>
#include <string.h>
#include "image.h"
#include "ysa.h"
#include <sys/types.h>
#include <dirent.h>



int main(){

	image img;
	ysa ys;
	DIR *dp,*rp;
	struct dirent *dc,*rdc;  
	char raw_folder[200],datafile[200];

	// Starting 


	printf("Ön yüklemeler...\n");
	// Inıtialize
	img_init(&img);
	ysa_init(&ys);
	ysa_data_create(&ys);


	printf("Liste açılıyor...\n");

	dp = opendir (RAWS_FOLDER);
	int id=0;

	while((dc=readdir(dp))){
		if(strcmp(dc->d_name,".")!=0 && strcmp(dc->d_name,"..")!=0){

			strcpy(raw_folder,RAWS_FOLDER);
			strcat(raw_folder,dc->d_name);
			strcat(raw_folder,"/");
			printf("%s\n", raw_folder);

			rp = opendir (raw_folder);
			while((rdc=readdir(rp))){
				if(strcmp(rdc->d_name,".")!=0 && strcmp(rdc->d_name,"..")!=0){

					printf("%d Başlıyoruz...-------------------------------------------\n",id);

					//Network reset
					ysa_network_reset(&ys);

					// Output set
					ysa_output_set(&ys,id);

					// Image open and read
					printf("Resim okunuyor...\n");
					strcpy(img.folder,raw_folder);
					strcpy(img.file,rdc->d_name);
					img_read(&img);

					// Do Image hash
					printf("Map oluşturmaya başla.\n");
					img_trim(&img);

					printf("Hash kaydediliyor...\n");
					img_mapsave(&img);
					img_hashsave(&img);

					// Image hash convert to input data
					printf("Hash giriş olarak ayarlanıyor...\n");
					ysa_convertMapToInput(&img,&ys);

					// network calculating
					printf("Ağ ağırlıkları hesaplanıyor...\n");
					ysa_training(&ys);
					printf("Ağ eğitimi tamamlandı. \n");

					// YSA data save
					printf("Data kaydediliyor...\n");
					strcpy(datafile,DATA_FOLDER);
					strcat(datafile,img.file);
					ysa_data_add(&ys,id,dc->d_name);


				}
			}
			id++;
		}	
	}


	printf("Bellekler bırakılıyor...\n");
	img_free(&img);printf("-");
	ysa_free(&ys);
	printf("Bellekler bırakıldı.\n");

	return 0;
}