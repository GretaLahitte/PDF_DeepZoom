/* 		
 *		PDF_DeepZoom
 *
 *		Save a PDF file as a set of tiles at various resolutions with libvips. 
 * 		compile with:
 *		gcc -Wall -o PDF_DeepZoom PDF_DeepZoom.c `pkg-config vips --cflags --libs`
 * 		usage: ./PDF_DeepZoom pdfFile outputFolder
 * 		with pdfFile the pdf you want to process ant outputFolder the folder name where to store the result
 * 
 * 		by M.LAHITTE  02/10/2018
 */

#include <stdio.h>
#include <vips/vips.h>
#include <unistd.h>
#include <stdlib.h>

int mat_len(char *s) 
{ 
	int i = 0; while(s[i]) i++; return i; 
}

int main( int argc, char **argv )
{
	VipsImage *in;
	VipsImage *out;
	
		
	const char *prefix= "./public/images/dzi/";
	const char *extension = ".png";
	const gdouble dpi= 300;

	char *path;
	if((path=malloc(sizeof(path)*(strlen(prefix)+strlen(argv[2])+1)))==NULL)
	{
		exit (EXIT_FAILURE);
	}

	char *thumb;
	if((thumb=malloc(sizeof(thumb)*(strlen(extension)+1)))==NULL)
	{
		exit (EXIT_FAILURE);
	};

	
	if( VIPS_INIT( argv[0] ) )//open the vips library
		vips_error_exit( NULL ); 

	if( argc != 3 )//check for arguments
		vips_error_exit( "usage: %s infile outfile", argv[0] ); 
	
	//load pdf and resample it to 300 dpi
	vips_pdfload(argv[1],&in,"dpi",dpi,NULL);
	
	
	//create folder name and folder
	strcpy(path,prefix);
	strcat(path,argv[2]);
	mkdir(path,0700);
	strcat(path,"/");
	strcat(path,argv[2]);
	
	
	//generate tiles
	if((vips_dzsave(in,path ,NULL))){
		vips_error_exit( NULL );
	}else{
		printf("PDF created,moving temp files.\n");
		
	
		strcpy(thumb,extension);
		strcat(path,thumb);
		printf("thumbnail path:%s\n",path);
			
		//generate thumbnail
		vips_thumbnail(argv[1],&out,128,NULL);
		
		if(!(vips_image_write_to_file(out,path,NULL)))
			printf("Thumbnail created.\n");
		
	
	}
	vips_shutdown(); //close the vips library
	
	free(thumb);
	free(path);
	
	return( 0 );
}


