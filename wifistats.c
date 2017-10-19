/*
   CITS2002 Project 1 2017
   Name(s):             Hodaka Kubo
   Student number(s):   22121415
   Date:                12noon Fri 22nd September
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

#define TEMPODATA "tempodat.txt"

int numlines(char *filename);
FILE *readData(char *argv);
char *converStr(char *sixDigits);
int main(int argc, char *argv[]);


int numlines(char *filename){
	FILE *fp = fopen(filename,"r");
	char ch;
	int lines = 0;
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n'){
			lines++;
		}
	}
	lines--;
	return lines;
}

FILE *readData(char *argv){
	FILE *data;
	data = fopen(argv,"r");
	if(data == NULL){
		printf("cannot open datafile %s\n",argv);
		exit(EXIT_FAILURE);
	}
	return data;
}

char *converStr(char *sixDigits){
	for(int i = 0; sixDigits[i]; i++){
		sixDigits[i] = tolower(sixDigits[i]);
		if(sixDigits[i] == '-'){
			sixDigits[i] = ':';
		}
	}	
	return sixDigits;
}

 
int main(int argc, char *argv[]){

	/*----if the single character is 't', 1 is assign to column ----*/
	/*----if the single character is 'r', 2 is assign to column ----*/
	int column;
	if( strcmp(argv[1],"t") == 0) column = 1;
	else column = 2;
	
/*-----This section creates ------------------------------------------------*/
/*--------datastr = "68:94:23:4b:e8:35\t100\n-------------------------------*/
/*-------------------00:11:d9:20:aa:4e\t951\n-------------------------------*/
/*-------------------cc:3a:61:df:4b:61\t259\n-------------------------------*/
/*-------------------84:1b:5e:a8:bf:7f\t82\n--------------------------------*/
/*-------------------74:e2:f5:17:96:89\t829\n-------------------------------*/
/*-------------------84:1b:5e:a8:bf:7c\t56\n\0"-----------------------------*/
/*-----from "sample-packets".-----------------------------------------------*/

	/*-- The number of lines in the file(argv[2]) is assign to numOfLines---*/
	int numOfLines;
	numOfLines = numlines(argv[2]);
	
	/*-- OUIs and numbers will be assign to this array --*/
	char ansArray[numOfLines][2][18];

	/*- read the file here -*/
	FILE *data;
	data = readData(argv[2]);

	/*--------Allocate OUIs and numbers in the file to ansArray------------*/
	/*--------ansArray[0] -> "68:94:23:4b:e8:35"		"100"----------*/
	/*--------ansArray[1] -> "00:11:d9:20:aa:4e"<-same	"285"----------*/
	/*--------ansArray[2] -> "cc:3a:61:df:4b:61"		"259"----------*/
	/*--------ansArray[3] -> "00:11:d9:20:aa:4e"<-same	"683"----------*/
	/*--------ansArray[4] ->          .            		  . -----------*/
	/*------------------           .            		  . -----------*/
	int index = 0;
	char line[60];
	char *cutLine;
	const char cut[1] = "\t";	//string file is cut with '\t'
	while(fgets(line, sizeof(line),data) != NULL){
		//let's say the line = 1503251369.691375 84:1b:5e:a8:bf:7f 68:94:23:4b:e8:35 100 	 
		if(index == numOfLines){break;}		
		cutLine = strtok(line,cut);	//cutLine here is "1503251369.691375"
 
		cutLine = strtok(NULL,cut);	//cutLine here is "84:1b:5e:a8:bf:7f"
		//if column is 1, then the element of the second column is allocated to ansArray[index][0]
		if(column == 1)	strcpy(ansArray[index][0],cutLine);

		cutLine = strtok(NULL,cut);	//cutLine here is "68:94:23:4b:e8:35"
		//if column is 2, then the element of the third column is allocated to ansArray[index][0]
		if(column == 2)	strcpy(ansArray[index][0],cutLine);

		cutLine = strtok(NULL,cut);	//cutLine here is "100"
		cutLine[strlen(cutLine) - 1] = '\0'; //cut '\n' at the end of each line in the file
		strcpy(ansArray[index][1],cutLine);
		index++;
	}

	/*--------Add two numbers if both two OUIs are the same--------------------------*/
	/*---------and the added numbers and its OUI is replaced by "X" in this section.-*/	
	/*--------ansArray[0] -> "68:94:23:4b:e8:35"	       "100"---------------------*/
	/*--------ansArray[1] -> "00:11:d9:20:aa:4e"	added->"968" <-(638+285) --------*/
	/*--------ansArray[2] -> "cc:3a:61:df:4b:61"	       "259"---------------------*/
	/*--------ansArray[3] ->       "X"    <- replaced ->	"X" <-(here was 285) ----*/
	/*------------------           .      		         . ----------------------*/
	/*------------------           .                	 . ----------------------*/
	int inum,jnum,sum; 
	char str[10];
	for(int i = numOfLines - 1; i >= 1; i--){ 	//check from the back side
		for(int j = i - 1; j >= 0;j--){
			//the first comparison is "ansArray[lastrow][0]" and "ansArray[lastrow - 1][0]"
			if(strcmp(ansArray[i][0],ansArray[j][0]) == 0){
				inum = atoi(ansArray[i][1]);	//convert the string number to integer
				jnum = atoi(ansArray[j][1]);	//convert the string number to integer
				sum = inum + jnum;		//add the two numbers and assign to sum
				sprintf(str,"%d",sum);		//convert the integer "sum" to string
				strcpy(ansArray[j][1],str);	//replace the number to the added number
				strcpy(ansArray[i][1],"X");	//replace the added number to "X"
				strcpy(ansArray[i][0],"X");	//replace the added number's OUT to "X"
				break;
			}
		}
	}
	
	/*------Make string through concaternating each element of ansArray-------*/	
	char datastr[12000];
	datastr[0] = '\0';	//the last char of the datastr has to be '\0'
	for(int i = 0; i < numOfLines ; i++){
		if(strcmp(ansArray[i][0],"X") != 0){	//if ansArray[i][0] is "X", this elelemt is not concatenated
			strcat(datastr,ansArray[i][0]); //concatenate datastr and ansArray[i][0]
			strcat(datastr,"\t");		//concatenate datastr and "\t"
			strcat(datastr,ansArray[i][1]);	//concatenate datastr and ansArray[i][1]
			strcat(datastr,"\n");		//concatenate datastr and "\n"
		}
	}


/*-----This section creates ------------------------------------------------*/
/*-------datastr2 = "??:??:??\tUNKNOWN-VENDOR\t100\n------------------------*/
/*-------------------00:11:d9\tTiVo\t951\n----------------------------------*/
/*-------------------cc:3a:61\tMAMSUNG ELECTRO MECHANICS CO., LTD\t259\n----*/
/*-------------------84:1b:5e\tNETGEAR\t138\n-------------------------------*/
/*-------------------74:e2:f5\tApple\t829\n\0"------------------------------*/
/*-----from "sample-OUIfile-small", if argv equals 4.-----------------------*/


	char datastr2[12000];
	if(argc == 4){
		/*-- The number of lines in the file(argv[3]) is assign to numOfLines2---*/
	        int numOfLines2;
	        numOfLines2 = numlines(argv[3]);

		/*-- OUIs and its company's name will be assign to this array --*/
	        char ansArray2[numOfLines2][2][91];
	       
		/*- read the file here -*/
		FILE *data2;
		data2 = readData(argv[3]);

		/*--------Allocate OUIs and its company's name in the file to ansArray2---*/
		/*--------ansArray2[0] -> "00:01:24"	"Acer Incorporated"---------------*/
		/*--------ansArray2[1] -> "74:e2:f5"	"Apple"---------------------------*/
		/*--------ansArray2[2] -> "00:0b:6a"	"Asiarock Incorporation"----------*/
		/*--------ansArray2[3] -> "00:13:8f"	"Asiarock Incorporation"----------*/
		/*--------ansArray2[4] ->      .    	      .                -----------*/
		/*--------ansArray2[5] ->      .    	      .                -----------*/
	        int index2 = 0;
	        char line2[100];
	        char *cutLine2;
		char *converted;
	        const char cut2[1] = "\t";
	        while(fgets(line2, 100,data2)){			//let's say line2 = 00-01-24	Acer Incorporated
			if(index2 == numOfLines2){break;}	
			cutLine2 = strtok(line2,cut2);		//cutLine2 here is "00-01-24"
			converted = converStr(cutLine2); 	//convert "00-01-24" to "00:01:24"
	                strcpy(ansArray2[index2][0],converted); //copy "00:01:24" to ansArray[index2][0]
	                cutLine2 = strtok(NULL,cut2);		//cutLine2 here is "Acer Incorporated"
			cutLine2[strlen(cutLine2) - 1] = '\0';	//cut '\0' at the end of each line
	                strcpy(ansArray2[index2][1],cutLine2);	//copy "Acer Incorporated" to ansArray[index2][1]
	                index2++;
	        }
		
		
		/*-- OUIs and its company's name and its number will be assign to this array --*/
	        char ansArray3[numOfLines][3][91];
		
		/*--------Allocate OUIs and its company's name and its number in the file to ansArray3 ---------*/
		/*--------ansArray3[0] -> "??:??:??"	"UNKNOWN-VENDOR"	"100"                  ---------*/
		/*--------ansArray3[1] -> "00:11:d9"	"TiVo"	829                                    ---------*/
		/*--------ansArray3[2] -> "cc:3a:61"	"SAMSUNG ELECTRO MECHANICS CO., LTD."	"259"  ---------*/
		/*--------ansArray3[3] -> "84:1b:5e"	"NETGEAR"	"138"	                       ---------*/
		/*--------ansArray3[4] -> "74:e2:f5"	"Apple"	"658" <-same OUI still exists!         ---------*/
		/*--------ansArray3[5] -> "74:e2:f5"	"Apple"	"171" <-same OUI still exists!         ---------*/
		/*--------ansArray3[6] ->    "X"    	  "X"    "X"                                   ---------*/
		/*--------ansArray3[7] ->     .     	   .      .                                    ---------*/
		/*--------ansArray3[8] ->     .     	   .      .                                    ---------*/
		char key[18];
		int index3 = 0;
		for(int i = 0; i < numOfLines; i++){
			if(strcmp(ansArray[i][0],"X") != 0){
				strncpy(key,ansArray[i][0],8);	//key is "68:94:23:4b:e8:35" here for instance
				key[8] = '\0';			//key is "68:94:23'\0'4b:e8:35" here for instance
				bool judge = true;
				for(int j = 0; j < numOfLines2; j++){
					if(strcmp(ansArray2[j][0],key) == 0){	//if the key is same as ansArray2[j][0](ig.00:11:d9)
						//allocate ansArray2[j][0](ig.00:11:d9) to ansArray3[index3][0]
						strcpy(ansArray3[index3][0],ansArray2[j][0]);
						
						//allocate ansArray2[j][1](ig."Acer Incorporated") to ansArray3[index3][1]
						strcpy(ansArray3[index3][1],ansArray2[j][1]);

						//allocate ansArray2[i][1](ig."951") to ansArray3[index3][1]
						strcpy(ansArray3[index3][2],ansArray[i][1]);
						index3++;
						judge = false;
					}
				}
				if(judge){
					//if there in no collesponding data, then allocate ??:??:?? "UNKNOWN-VENDOR" and its number to ansArray3 
					strcpy(ansArray3[index3][0],"??:??:??");
					strcpy(ansArray3[index3][1],"UNKNOWN-VENDOR");
					strcpy(ansArray3[index3][2],ansArray[i][1]);
					index3++;
				}
			}
		}
		//assign X to the rest element of ansArray3
		for(int i = index3; i < numOfLines; i++){
			strcpy(ansArray3[i][0],"X");
			strcpy(ansArray3[i][1],"X");
			strcpy(ansArray3[i][2],"X");
		}
		
		/*--------Add numbers which have the same OUI---------------------------------------------------*/
		/*--------ansArray3[0] -> "??:??:??"	"UNKNOWN-VENDOR"	"100"                  ---------*/
		/*--------ansArray3[1] -> "00:11:d9"	"TiVo"	829                                    ---------*/
		/*--------ansArray3[2] -> "cc:3a:61"	"SAMSUNG ELECTRO MECHANICS CO., LTD."	"259"  ---------*/
		/*--------ansArray3[3] -> "84:1b:5e"	"NETGEAR"	"138"	                       ---------*/
		/*--------ansArray3[4] -> "74:e2:f5"	"Apple"	"829" <-added!!!                       ---------*/
		/*--------ansArray3[5] ->    "X"    	  "X"    "X"  <-replace by "X"                 ---------*/
		/*--------ansArray3[6] ->    "X"    	  "X"    "X"                                   ---------*/
		/*--------ansArray3[7] ->     .     	   .      .                                    ---------*/
		/*--------ansArray3[8] ->     .     	   .      .                                    ---------*/
		int inum2,jnum2,sum2; 
		int length = 0;
		char str2[10];
		for(int i = 0; i < numOfLines; i++){	//search the number of rows whose first row is not "X".
			if(strcmp(ansArray3[i][0],"X") == 0){
				length = i;
				break;
			}
		}
		for(int i = length - 1; i >= 0; i--){	//check from the back side whether two OUIs are the same or not
			for(int j = i - 1; j >= 0;j--){
				if(strcmp(ansArray3[i][0],ansArray3[j][0]) == 0){
					inum2 = atoi(ansArray3[i][2]);
					jnum2 = atoi(ansArray3[j][2]);
					sum2 = inum2 + jnum2;
					sprintf(str2,"%d",sum2);
					strcpy(ansArray3[j][2],str2);
					strcpy(ansArray3[i][0],"X");
					strcpy(ansArray3[i][1],"X");
					strcpy(ansArray3[i][2],"X");
				}
			}
		}
		/*------Make string through concaternating each element of ansArray3-------*/	
		datastr2[0] = '\0';
		for(int i = 0; i < numOfLines ; i++){
			if(strcmp(ansArray3[i][0],"X") != 0){
				strcat(datastr2,ansArray3[i][0]);
				strcat(datastr2,"\t");
				strcat(datastr2,ansArray3[i][1]);
				strcat(datastr2,"\t");
				strcat(datastr2,ansArray3[i][2]);
				strcat(datastr2,"\n");
			}
		}
	}

/*---------------------------------------------------------------------------------*/
/*------Copy either datastr or datastr2 to TEMPODATA(text file is made here) ------*/
/*------and make child process and sort either datastr ot datastr2-----------------*/
/*---------------------------------------------------------------------------------*/
	
	FILE *fptr;
	fptr = fopen(TEMPODATA,"w");
	if(fptr == NULL){
		printf("Error is occured while creating to the file.");
	}
	
	int  pid;
        switch (pid = fork()) {
	case -1 :
                perror("fork()");     // process creation failed
                exit(1);
                break;
        case 0:     					
		if(argc == 3){
			fprintf(fptr,"%s\n",datastr);	//print datastr to TEMPODATA if argc = 3
		}
		if(argc == 4){
			fprintf(fptr,"%s\n",datastr2); 	//print datastr2 to TEMPODATA if argc = 4
		}
		fclose(fptr);
		fclose(data);
		exit(EXIT_FAILURE);
		break;
	default:
		if(argc == 3){	
			char *newargs[7] = {"sort","-k","2","-n","-r",TEMPODATA,NULL};
			execv("/usr/bin/sort",newargs);	//sort TEMPODATA by the second column
		}
		if(argc == 4){
			char *newargs[9] = {"sort","-t\t","-k","3","-n","-r",TEMPODATA,NULL};
			execv("/usr/bin/sort",newargs);	//sort TEMPODATA by the third column
		}
		sleep(1);
		break;
	}
	fflush(stdout);
	return 0;
}
