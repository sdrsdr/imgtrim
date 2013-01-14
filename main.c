#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <magick/MagickCore.h>

int main(int argc, char **argv) {

	ExceptionInfo *exception;
	Image *image;
	ImageInfo *image_info;

	/* Initialize the image info structure and read an image. */
	MagickCoreGenesis(*argv,MagickTrue);

	if (argc != 2) {
		printf("Usage: %s file_to_trim\n We're using ImageMagick from %s v:%s\n",argv[0],GetMagickPackageName(),GetMagickVersion(NULL));
		
		exit(0);
	}
	
	exception=AcquireExceptionInfo();
	image_info=CloneImageInfo((ImageInfo *) NULL);
	strcpy(image_info->filename,argv[1]);
	image=ReadImage(image_info,exception);
	if (exception->severity != UndefinedException) CatchException(exception);
	if (image == (Image *) NULL) exit(1);
	printf("%s %zu x %zu\n",argv[1],image->columns,image->rows);

	return 0;
}
