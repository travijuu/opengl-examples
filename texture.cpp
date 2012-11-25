#include <stdio.h>
#include <GL/glaux.h>
#include "texture.h"


void texture::Create( char *filename )
{
	

	::LoadBMPWithAlpha( &img, filename);
	
	glGenTextures( 1, &texName ) ;

	glBindTexture( GL_TEXTURE_2D, texName  );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height,
		          0, GL_RGBA, GL_UNSIGNED_BYTE, img.data );

}

void texture::Draw( )
{
	glBindTexture( GL_TEXTURE_2D, texName ) ;
	
	glBegin( GL_QUADS ) ;
	  glTexCoord2f(0.0, 0.0 ) ; glVertex2i( 0,0 );
	  glTexCoord2f(0.0, 1.0 );  glVertex2i( 0, img.height );
	  glTexCoord2f(1.0, 1.0 );  glVertex2i( img.width , img.height );
	  glTexCoord2f(1.0, 0.0 );  glVertex2i( img.width , 0 );
	glEnd();

}


AUX_RGBImageRec *AUXLoadBMP(char *Filename)                // Loads A Bitmap Image
{
        FILE *File=NULL;                                // File Handle

        if (!Filename)                                  // Make Sure A Filename Was Given
        {
                return NULL;                            // If Not Return NULL
        }

        File=fopen(Filename,"r");                       // Check To See If The File Exists

        if (File)                                       // Does The File Exist?
        {
                fclose(File);                           // Close The Handle
                return auxDIBImageLoad(Filename);       // Load The Bitmap And Return A Pointer
        }
        return NULL;                                    // If Load Failed Return NULL
}

void LoadBMP( IMAGE *image, char *Filename )
{
	    AUX_RGBImageRec *Image; 
        Image = AUXLoadBMP(Filename);       
				
		if ( Image ) 
		{
		  image->data = Image->data;
		  image->height = Image->sizeY;
		  image->width = Image->sizeX;
		  image->type =  BMP_RGB ;
		}
		else
		{ 
		  image->data = NULL;
		  image->height = 0;
		  image->width = 0;
		  image->type =  BMP_RGB ;
		}
}


void LoadBMPWithAlpha( IMAGE *alphaimage, char *filename)
{
	AUX_RGBImageRec *Image;              

    Image = AUXLoadBMP( filename );
  
	if ( Image == NULL )
		return  ;
    
	int w = alphaimage->width  = Image->sizeX ;
	
	int h = alphaimage->height = Image->sizeY ;
	
	int size = w * h * 3 ;
	
	unsigned char *data = Image->data;

	int alphatotal = w * h * 4 ;
	
	unsigned char *alpha = ( unsigned char *)malloc( alphatotal ) ;

    for (int src = 0, dst = 0; src < size; src +=3, dst +=4)
	{

      if (data[src] == alphaimage->r && data[src+1] == alphaimage->g && data[src+2] == alphaimage->b)
	  {        
		alpha[dst+3] = 0;
	  }
       else 
	   alpha[dst+3] = 255 ;

      alpha[dst] = data[src];
      alpha[dst+1] = data[src+1];
      alpha[dst+2] = data[src+2];
	}

	free( Image->data );
	free( Image );

	alphaimage->type = BMP_RGBA ;
	alphaimage->data = alpha ;
}


