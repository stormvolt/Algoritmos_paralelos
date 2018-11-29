#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>

#define BLUR_SIZE 1

using namespace std;

//leer imagen
unsigned char* readBMP(char* filename, int &my_width, int &my_height)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
    my_width = width;
	my_height = height;

    return data;
}

//guardar imagen
void writeBMP(unsigned char* img, int w, int h)
{
    FILE *f;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};
    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);
    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);
    f = fopen("img.bmp","wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    //for(int i=0; i<h; i++)
    for(int i=h-1; i>=0; i--)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }
    free(img);
    fclose(f);
}


__global__
void blurKernel(unsigned char *in, unsigned char *out, int w, int h)
{
	int Col = blockIdx.x * blockDim.x + threadIdx.x;
	int Row = blockIdx.y * blockDim.y + threadIdx.y;
	
	if(Col<w && Row<h)
	{
		int pixVal = 0;
        int pixels = 0;
        
        for(int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE+1; ++blurRow)
        {
            for(int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE+1; ++blurCol)
            {
                int curRow = Row + blurRow;
                int curCol = Col + blurCol;
                
                if(curRow>-1 && curRow<h && curCol>-1 && curCol<w)
                {
                    pixVal += in[curRow * w +curCol];
                    pixels++;
                }
            }
            out[Row * w + Col] = (unsigned char)(pixVal/pixels);
        }
	}
	
	
}

int main()
{
    unsigned char* imagen_in;
	unsigned char* imagen_out;
	int width = 0;
	int height = 0;
	string archivo;
	//cout<<"Ingrese nombre de la imagen: ";
	//cin>>archivo;
	
	imagen_in = readBMP("Tulips.bmp", width, height);
	
	dim3 dimGrid(ceil(width/16.0), ceil(height/16.0), 1);
	dim3 dimBlock(16, 16, 1);
	blurKernel<<<dimGrid,dimBlock>>>(imagen_in,imagen_out,width,height);
	
	writeBMP(imagen_out, width, height);

    return 0;
}