
#include "ysa.h"


void ysa_init(ysa *ys){

	ys->inputsize = YSA_INPUT;
	ys->outputsize = YSA_OUTPUT;
	ys->cellsize = YSA_CELL;


	ys->input = (float*) calloc(sizeof(float),ys->inputsize);
	ys->output = (float*) calloc(sizeof(float),ys->outputsize);
	ys->fo = (float*) calloc(sizeof(float),ys->cellsize);
	ys->o = (float*) calloc(sizeof(float),ys->outputsize);
	ys->eo = (float*) calloc(sizeof(float),ys->outputsize);
	ys->e = (float*) calloc(sizeof(float),ys->cellsize);

	/*net.w = (float**) calloc(sizeof(float*),ys->cellsize);
	for(int i=0;i<ys->cellsize;i++)
		net.w[i] = (float*) calloc(sizeof(float),ys->inputsize);

	net.wsize = ys->cellsize * ys->inputsize;

	net.wo = (float**) calloc(sizeof(float*),ys->outputsize);
	for(int i=0;i<ys->outputsize;i++)
		net.wo[i] = (float*) calloc(sizeof(float),ys->cellsize);

	net.wosize = ys->cellsize * ys->outputsize;
*/


	ysa_network_reset(ys);

}	

void ysa_network_reset(ysa *ys){
		// Ağ ağırlıkları tanımlama
	for(int i=0;i<ys->cellsize;i++){
		for(int j=0;j<ys->inputsize;j++){
			ys->net.w[i][j]=(float)1/YSA_INPUT;//(((0.001*j)+(i*0.001)))/YSA_CELL;
			//printf("%f\n",ys->net.w[i][j] );
		}
	}


	for(int i=0;i<ys->outputsize;i++){
		for(int j=0;j<ys->cellsize;j++){
			ys->net.wo[i][j]=(float)1/YSA_CELL;//(((0.001*j)+(i*0.001)))/YSA_CELL;
			ys->output[i]=0;
		
		}	
	}

}

void ysa_free(ysa *ys){

	free(ys->input);
	free(ys->output);
	free(ys->fo);
	free(ys->o);
	free(ys->e);
	free(ys->eo);
	//free(ys->net.w);
	//free(ys->net.wo);

/*	for(int i=0;i<ys->cellsize;i++)
		free(ys->net.w[i]); 

	for(int i=0;i<ys->outputsize;i++)
		free(ys->net.wo[i]); */

}

void ysa_output_set(ysa *ys,int id){
	ys->output[id]=YSA_OUTPUT_VALUE;
}


float ysa_calcE(float value,float errorX,float weight){
	return  value * (1 - value) * weight * errorX ;
}

float ysa_calcEo(float value,float b){
	float e = b - value;
	return value * (1- value) *e;
}

float ysa_calcNewW(float value,float error,float weight){
	return weight + (0.6 * error * value);
}

float ysa_total(float *i,float *w,int len){
	float s=0;	
	for(int j=0;j<len;j++){
		s = s+(i[j]*w[j]);
	}
	return s;
}

float ysa_func(float x){

	float y;
	//x= x * 0.5;
	//y = (exp(x) - exp(-1*x)) / (exp(x) + exp(-1*x)) ; // Hiperbolik Tanjant
	if(x<0) y=0; else y=x; // Leakly RELU
    //y= 2*x*(1/(1+exp(-1*x))); // Swish 
   // y= (1/(1+exp(-1*x))); // Sigmoid 
	//printf("Aktivasyon : %f => %f \n", x,y);
	  return y;
}

void ysa_convertGrayToInput(image *img,ysa *ys){

	int index =0;

	for(int i=0;i<img->height;i++){
		for(int j=0;j<img->width;j++){
			index = (i*img->height)+j;
			ys->input[index] =1-((float)(img->gray[i][j])/255); // ysa_func((float)img->map[i][j]/255);
			//printf("%d %d %f\n",index,img->gray[i][j],ys->input[index] );
		}
	}

}

void ysa_convertMapToInput(image *img,ysa *ys){

	int index =0;

	for(int i=0;i<img->height;i++){
		for(int j=0;j<img->width;j++){
			index = (i*img->height)+j;
			ys->input[index] =(float)(img->map[i][j]); // ysa_func((float)img->map[i][j]/255);
			//printf("%d %d %f\n",index,img->map[i][j],ys->input[index] );
		}
	}

}


void ysa_convertTrimToInput(image *img,ysa *ys){

	int index =0;

	for(int i=0;i<img->trim_height;i++){
		for(int j=0;j<img->trim_width;j++){
			index = (i*img->trim_height)+j;
			ys->input[index] =(float)(img->trim[i][j])/img->trim_height; // ysa_func((float)img->map[i][j]/255);
			//printf("%d %d %f|",index,img->trim[i][j],ys->input[index] );
		}
	}
}



void ysa_convertHashToInput(image *img,ysa *ys){

	printf("%d size hash -> input\nGiriş : ", img->hashsize);
	for(int i=0;i<img->hashsize;i++){
			ys->input[i] =(float)(img->hash[i])/45; // ysa_func((float)img->map[i][j]/255);
			printf("%f|", ys->input[i]);
		
	}

}



void ysa_run(ysa *ys){

	// calc network
	for(int i=0;i<ys->cellsize;i++){
		ys->fo[i] = ysa_func(ysa_total(ys->input,ys->net.w[i],ys->inputsize));
		//printf("Cell %d => %f  \n",i,fo[i]);
	}


	// calc output
	for(int i=0;i<ys->outputsize;i++){
		ys->o[i] = ysa_total(ys->fo,ys->net.wo[i],ys->cellsize);
		//printf("Output %d => %f  \n",i,ys->o[i]);
	}

}


int ysa_isOk(ysa *ys){
	int OK;
	for(int i=0;i<ys->outputsize;i++){
		OK=1;
		if((ys->o[i] < (ys->output[i] - YSA_ERROR ) )  ||  (ys->o[i] > (ys->output[i] + YSA_ERROR ))   ){
			OK=0;
			break;
		}

	}

	return OK;
}


void ysa_training(ysa *ys){

	int a=0;
	while(1){

		//system("clear");

		ysa_run(ys);

		for(int j=0;j<ys->outputsize;j++){
			printf("%f \n",ys->o[j] ); 
	
		}	


		for(int io=0;io<ys->outputsize;io++){
			ys->eo[io] = ysa_calcEo(ys->o[io],ys->output[io]);
			for(int ic=0;ic<ys->cellsize;ic++){
				ys->net.wo[io][ic] = ysa_calcNewW(ys->fo[ic],ys->eo[io],ys->net.wo[io][ic]);
				//printf("%d => %d : %f | ",ic,io,ys->net.wo[io][ic]);

					for(int ii=0;ii<ys->inputsize;ii++){
						ys->e[ic] = ysa_calcE(ys->fo[ic],ys->eo[io],ys->net.w[ic][ii]); 
						ys->net.w[ic][ii]=ysa_calcNewW(ys->input[ii],ys->e[ic],ys->net.w[ic][ii]);
						//printf("%d => %d : %f | ",ii,ic,ys->net.w[ic][ii] );
					}
			}

		}

		if(ysa_isOk(ys)){
		
			printf("Çıkış : ");
/*
			for(int i=0;i<ys->cellsize;i++){
				for(int j=0;j<ys->inputsize;j++){
					printf("%f|",ys->net.w[i][j] ); 
				}
			}	
			printf("------------\n");
			for(int i=0;i<ys->outputsize;i++){
				for(int j=0;j<ys->cellsize;j++){
					printf("%f|",ys->net.wo[i][j] ); 
				}
			}	*/		

			for(int j=0;j<ys->outputsize;j++){

				printf("%f|",ys->o[j] ); 
		
			}		

			printf("%d deneme...\n",a);

			return;
		} 	
		printf("\033[10A"); 

		a++;

	}



}


void ysa_data_info_load(ysa_data_info *info){

	FILE *fp;
	fp = fopen(DATA_FILE,"rb");
	fread(info,sizeof(ysa_data_info),1,fp);
	fclose(fp);	
}


int ysa_data_load(ysa_data *dt,ysa_data_info *info){

	if(info->offset == 0) {
		printf("please run ysa_data_info_load() function.\n");
		return 0;
	}

	FILE *fp;
	printf("Open file %s\n", DATA_FILE);
	fp = fopen(DATA_FILE,"rb");

	fseek(fp,info->offset,SEEK_SET);
	printf("Point %ld\n", ftell(fp));

	fread(dt,sizeof(ysa_data),1,fp);
	printf("Veri ismi : %s \n",dt->name);
	printf("Veri ID : %d\n", dt->id );
	info->offset = ftell(fp);
	if(feof(fp)) return 0;
	fclose(fp);

	return 1;
}


void ysa_data_create(ysa *ys){

	FILE *fp;
	printf("Open file %s\n", DATA_FILE);
	fp = fopen(DATA_FILE,"wb");

	ysa_data_info info;
	info.inputsize = ys->inputsize;
	info.outputsize = ys->outputsize;
	info.cellsize = ys->cellsize;
	info.offset = sizeof(ysa_data_info);

	fwrite(&info,sizeof(ysa_data_info),1,fp);

	fclose(fp);

}


void ysa_data_add(ysa *ys,int id,char *name){

	ysa_data dt;
	FILE *fp;	


	dt.id=id;
	memcpy(&dt.net,&ys->net,sizeof(ysa_network));
	strncpy(dt.name,name,sizeof(dt.name));

	printf("Open file %s -> %s \n", DATA_FILE,dt.name);

	fp = fopen(DATA_FILE,"ab");
	printf("Data boyutu %ld\n",sizeof(ysa_data));
	fwrite(&dt,sizeof(ysa_data),1,fp);
	fclose(fp);



	/*printf("%ld bytes data writing...\n", sizeof(float) * ys->inputsize*ys->cellsize);
	for(int i=0;i<(ys->cellsize);i++)
		fwrite(ys->net.w[i],sizeof(float),ys->inputsize,ys->data_fp);

	printf("%ld bytes data writing...\n", sizeof(float) * ys->outputsize*ys->cellsize);
	for(int i=0;i<(ys->outputsize);i++)
		fwrite(ys->net.wo[i],sizeof(float),ys->cellsize,ys->data_fp);

*/

}