#include<cv.h>
#include<highgui.h>
#include"cxcore.h"
#define LCD_COLS 144
#define LCD_ROWS 168
#define LCD_BYTES_inLine 18
#define BINARY_THR 210
uchar arr[LCD_ROWS][LCD_BYTES_inLine];	//[168][18]
char eraseFileFlag= 0;
char fileNameString[10];

using namespace cv;

void makeArray(char *imgPtr)
{
	int i,j,k;
	uchar byteVal ;
	uchar val;
	for (i=0; i<LCD_ROWS; i++)
	{
		for(j=0; j<LCD_BYTES_inLine; j++)
		{
			byteVal = 0x00;
			for (k=0;k<8;k++)
			{
				val = (uchar)*((i*LCD_BYTES_inLine*24)+imgPtr+(j*24)+(k*3));
				val = (val > BINARY_THR) ? 1 : 0;
				byteVal |= val << (7-k); 
			}
			arr[i][j] = byteVal;
		}
	}
}

void MakeHeader(char *imgPtr,int Size)
{
	FILE *fp;
	int i;
	uchar* arrPtr;
	arrPtr = &arr[0][0] ;

	makeArray(imgPtr);

	if ((fp = fopen(fileNameString,"w")) == NULL)
		printf("error openig file");

	fprintf(fp,"\n/* AUTHOR : SANDEEP RAVI */\n");
	fprintf(fp,"unsigned char ImageData[] = {\n");
	fprintf(fp," 0x%x" ,*arrPtr);
	for(i=2;i<(LCD_ROWS*LCD_BYTES_inLine);i++)
	{
		fprintf(fp,",0x%x" ,*(arrPtr+i));
		if (!(i%18))
			fprintf(fp,"\n");
	}
	fprintf(fp,"};\n");
	fclose(fp);

}


void MakeSequenceHeader(char *imgPtr,int Size,int seqNo)
{
	FILE *fp;
	int i;
	uchar* arrPtr;
	arrPtr = &arr[0][0] ;

	makeArray(imgPtr);
	if (eraseFileFlag == 0)
	{
		if ((fp = fopen(fileNameString,"w")) == NULL)		// to erase the contents of the file 
			printf("error openig file");
		fclose(fp);
		eraseFileFlag = 1;
	}
	if ((fp = fopen(fileNameString,"a")) == NULL)
			printf("error openig file");
	fprintf(fp,"\n/* AUTHOR : SANDEEP RAVI */\n");
	fprintf(fp,"\n const unsigned char A%d[] = {\n",seqNo);
	fprintf(fp," 0x%x" ,*arrPtr);
	for(i=2;i<(LCD_ROWS*LCD_BYTES_inLine);i++)
	{
		fprintf(fp,",0x%x" ,*(arrPtr+i));
		if (!(i%18))
			fprintf(fp,"\n");
	}
	fprintf(fp,"};\n");
	fclose(fp);

}

void main()
{
	char choice;
	char str[50],tempStr[20];
	char str1[10];
	char GenString[20];
	int filename,noOfFiles;
	int i,j;
	
	char locationBackup[100]= "D:\\DispImages\\";
	char location[100];
	IplImage* newImage;
	Mat A,B;
	printf("************* WELCOME **************\n");
	printf("Please Select your choice \n 1 -> Single image conversion \n 2 -> Image Sequence Conversion \n");
	printf("Please Enter your Choice : ");
	scanf("%c",&choice);
	switch(choice)
	{
		case '1':

			printf("\nPlease Enter filename with extension\n");
			scanf("%s",&str);
			printf("\nPlease enter name of the .h file :");
			scanf("%s",&fileNameString);
			strcat(location,str);
			newImage = cvLoadImage(location,1);
			
			if (!newImage)
				printf("Error Opening Image");
			
			cvNamedWindow("Main Image",1);
			cvShowImage("Main Image",newImage);
			MakeHeader(newImage->imageData,newImage->imageSize);
			printf("Image Size is: %d", newImage->imageSize);
			cvWaitKey(0);
			cvDestroyWindow("Main Image");
			cvReleaseImage(&newImage);
			break;
		case '2':
			printf("\nPlease Enter the First .png filename without extension: ");
			scanf("%s",&str1);
			printf("\nEnter number of images: ");
			scanf("%d",&noOfFiles);
			printf("\nPlease enter name of the .h file :");
			scanf("%s",&fileNameString);

			filename = atoi(str1);
			
			for(i=0;i<noOfFiles;i++)
			{
				sprintf(GenString,"Animate\\%d.png",filename+i);
				strcpy(location,locationBackup);
				strcat(location,GenString);
				newImage = cvLoadImage(location,1);
				if (!newImage)
					printf("Error Opening Image %d",filename+i);
				cvNamedWindow("Main Image",1);
				cvShowImage("Main Image",newImage);
				MakeSequenceHeader(newImage->imageData,newImage->imageSize,i);
								
				cvDestroyWindow("Main Image");
				cvReleaseImage(&newImage);
			}
			break;

	}



}
