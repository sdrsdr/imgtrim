#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <magick/MagickCore.h>

int main(int argc, char **argv) {

	ExceptionInfo *exception;
	Image *image,*oimage;
	ImageInfo *image_info,*oimage_info,*mimage_info;

	/* Initialize the image info structure and read an image. */
	MagickCoreGenesis(*argv,MagickTrue);

	if (argc != 4) {
		printf("Usage: %s <file_to_trim> <rows_to_keep> <trimmed_file>\n We're using Image Magick from %s v:%s\n",argv[0],GetMagickPackageName(),GetMagickVersion(NULL));
		exit(0);
	}

	int keeprows=atoi(argv[2]);
	
	exception=AcquireExceptionInfo();
	image_info=CloneImageInfo((ImageInfo *) NULL);
	strcpy(image_info->filename,argv[1]);
	
	oimage_info=CloneImageInfo((ImageInfo *) NULL);
	strcpy(oimage_info->filename,argv[3]);


	mimage_info=CloneImageInfo((ImageInfo *) NULL);
	mimage_info->depth=8;
	strcpy(mimage_info->filename,"rgb:mem.raw");

	
	image=ReadImage(image_info,exception);
	if (exception->severity != UndefinedException) CatchException(exception);
	if (image == (Image *) NULL) exit(1);
	
	size_t bufsize=image->columns * image->rows *3;
	//char *pixels=AcquireMagickMemory(bufsize);

	unsigned char *pixels=ImageToBlob(mimage_info,image, &bufsize,exception);
	if (exception->severity != UndefinedException) CatchException(exception);

	int row,px;
	unsigned char ref_r=pixels[0];
	unsigned char ref_g=pixels[1];
	unsigned char ref_b=pixels[2];
	unsigned char *cpixel=pixels;
	int rmrows=-keeprows,stop=0;
	for (row=0; row< image->rows-1 ;row ++){
		for (px=0;px<image->columns; px ++){
			unsigned char r=cpixel[0];
			unsigned char g=cpixel[1];
			unsigned char b=cpixel[2];
			#define dist(a,b) (a>b?a-b:b-a)
			if (dist(r,ref_r)>10 ||dist(g,ref_g)>10 || dist(b,ref_b)>10) {
				stop=1;
				break;
			} else {
				//cpixel[0]=0;
				//cpixel[1]=255;
				//cpixel[2]=0;
			}
			cpixel+=3;
		}
		if (stop) break;
		rmrows++;
	}
	printf("%s %zux%zu kr:%d rmr:%d\n",argv[1],image->columns,image->rows,keeprows,rmrows);
	if (rmrows>0){
		oimage=ConstituteImage(image->columns,image->rows-rmrows,"RGB",CharPixel,pixels+rmrows*3*image->columns,exception);
		if (exception->severity != UndefinedException) CatchException(exception);
		strcpy(oimage->filename,argv[3]);
		WriteImage(oimage_info,oimage);
	} else {
		strcpy(image->filename,argv[3]);
		WriteImage(oimage_info,image);
	}
	
	//FILE * rawfile;
	//rawfile=fopen("tmp.raw","wb");
	//fwrite(pixels,bufsize,1,rawfile);
	//fclose(rawfile);
	
	return 0;
}
