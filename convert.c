#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include <sys/types.h>
#include <dirent.h>


typedef struct
{
	unsigned char id;
	int size;
}input;


int main(int argc, char const *argv[])
{
	
	int id=0;
	struct dirent *dc,*rdc;  
	image img;
	input inp;
	DIR *dp,*rp;
	char raw_folder[200];

	strcpy(img.inputs_file,INPUTS_FILE);
	img_init(&img);

	dp = opendir (RAWS_FOLDER);
	remove(INPUTS_FILE);
	
	while((dc=readdir(dp))){
		if(strcmp(dc->d_name,".")!=0 && strcmp(dc->d_name,"..")!=0){

			strcpy(raw_folder,RAWS_FOLDER);
			strcat(raw_folder,dc->d_name);
			strcat(raw_folder,"/");
			printf("%s\n", raw_folder);

			rp = opendir (raw_folder);
			while((rdc=readdir(rp))){
				if(strcmp(rdc->d_name,".")!=0 && strcmp(rdc->d_name,"..")!=0){

					// Image open and read
					printf("Resim okunuyor...\n");
					strcpy(img.folder,raw_folder);
					strcpy(img.file,rdc->d_name);
					img_read(&img);

					// Do Image map
					printf("Map oluşturmaya başla.\n");
					img_map(&img);

					// Do Image hash
					printf("Hash oluşturmaya başla.\n");
					img_hash(&img);

					// Hash Kaydediliyor
					printf("Hash kaydediliyor...\n");
					FILE* f = fopen(INPUTS_FILE, "ab");

					inp.id = atoi(rdc->d_name);
					inp.size = img.hashsize;
					//inp.input_data = (float*) calloc(img.hashsize,sizeof(float));
					//memcpy(&inp.input_data,&img.hash,inp.size);

					fwrite(&inp, sizeof(inp),1, f);
					fwrite(&img.hash,img.hashsize,1,f);
					fclose(f);

					printf("%d %d\n",img.hashsize,inp.id);


				}
			}
			id++;
		}
	}






	return 0;
}