#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vips/vips.h>


/**
 * PDF_DeepZoom
 *
 *		Save a PDF file as a set of tiles at various resolutions with libvips. 
 * 		A thumbnail is also created
 * 		compile with:
 *		gcc -Wall -o PDF_DeepZoom PDF_DeepZoom.c `pkg-config vips --cflags --libs`
 * 		usage: ./PDF_DeepZoom pdfFile outputFolder
 * 		with pdfFile the pdf you want to process ant outputFolder the folder name where to store the result
 * 
 * 		by M.LAHITTE  02/10/2018
 * 		Special thanks to Mathieu Dorville!
 * 
 * 		Use of pipe for parallelising the thumbnail creation. 
 * 
 * 
 *		un tube est défini par une entrée et une sortie (0 et 1)
 * 		Il faut s'assurer de fermer l'entrée non utilisée de chaque pipe
 *  
 * **/
 
#define TAILLE_MESSAGE 256

int main(int argc, char **argv){
	
		VipsImage *in;
		VipsImage *out;
		
		const char *prefix= "./";
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
		}
		
		
		if( VIPS_INIT( argv[0] ) )//open the vips library
			vips_error_exit( NULL ); 
		
		if( argc != 3 )//check for arguments
			vips_error_exit( "usage: %s infile outfile", argv[0] ); 
		
		//load pdf and resample it to 300 dpi
		vips_pdfload(argv[1],&in,"dpi",dpi,NULL);
		
			
		pid_t pid_fils;
		int descripteurTube[2];
		char messageLire[2][TAILLE_MESSAGE],messageEcrire[2][TAILLE_MESSAGE];
		
		printf("Création du tube.\n");
		if(pipe(descripteurTube) !=0){
			fprintf(stderr, "\033[31mErreur de création du tube.\033[0m\n");
			return EXIT_FAILURE;
		}
		pid_fils = fork();
		
		if(pid_fils == -1){
			fprintf(stderr,"\033[31mErreur de création de processus.\033[0m\n");
			return 1;
		}
		if(pid_fils == 0){
			close(descripteurTube[1]);
			
			read(descripteurTube[0],messageLire,TAILLE_MESSAGE*2);
			printf("Une commande a été reçue via le pipe\n");

			strcpy(thumb,extension);
			strcat(messageLire[1],thumb);

			//generate thumbnail
			vips_thumbnail(messageLire[0],&out,128,NULL);

			if(!(vips_image_write_to_file(out,messageLire[1],NULL)))
				printf("Thumbnail created.\n");
		}

		else{
			
			//create folder name and folder
			strcpy(path,prefix);
			strcat(path,argv[2]);
			mkdir(path,0700);
			strcat(path,"/");
			strcat(path,argv[2]);
			
			strcpy(messageEcrire[0],argv[1]);		
			strcpy(messageEcrire[1],path);		
			
			close(descripteurTube[0]);

			write(descripteurTube[1], messageEcrire, TAILLE_MESSAGE*2+2);
		//generate tiles
			if((vips_dzsave(in,path ,NULL)))
			{
				vips_error_exit( NULL );
			}else
			{
				printf("PDF created,moving temp files.\n");
			}
				
			wait(NULL);
		}
	
	vips_shutdown(); //close the vips library
	
	free(thumb);
	free(path);
	
    return 0;			
}
