/***********************/
/*    FTE calculator   */
/* Dr. Sherif F. Fahmy */
/***********************/ 

#include <stdio.h>
#include <string.h>
/*Without the include below, code will not work*/
#include <stdlib.h>

typedef struct{
  char rank[50];
  int hours;
}ranks;

typedef struct{
  char name[60];
  char level[50];
  int hours;
  int FT;
  char dept[3];
  int tenure;
}faculty_m;


float FTE=0; //the final FTE value, total
ranks x[5]; //table of ranks


float PT_tenure_FTE=0;
float PT_nonten_FTE=0;

float FT_tenure_FTE=0;
float FT_nonten_FTE=0;

float tenure_FTE=0;
float nonten_FTE=0;

int tenure = 0;
int non_tenure =0;

/************************/
/*  fill ranks array    */
/************************/

void fillranks(ranks *input){
  strcpy(input[0].rank,"Prof.");
  input[0].hours=10;
  strcpy(input[1].rank,"Teacher");
  input[1].hours=16;
  strcpy(input[2].rank,"Asc. Prof");
  input[2].hours=14;
  strcpy(input[3].rank,"TA");
  input[3].hours=18;
  strcpy(input[4].rank,"MM");
  input[4].hours=4;
}

/***********************/
/*  get hours for rank */
/***********************/

int gethours(char *rank,ranks *x){
  int i;  
  for(i=0; i<5; i++){
    if (strcmp(rank,x[i].rank)==0){
      return x[i].hours;
      }
  }
  return -1;
}

/************************/
/*  process one line    */
/************************/

void processline(char *line){
  int i=0;
  char *token,sep[3]=",\n";
  faculty_m a;
  token=strtok(line,sep);
  while(token!=NULL){
    i++;
    switch(i){
    case 1: strcpy(a.name,token);
      break;
    case 2: strcpy(a.level,token);
      break;
    case 3: a.hours=atoi(token);
      break;
    case 5: if (strcmp(token,"FT")==0)
        a.FT=1;
      else
	a.FT=0;
      break;
    case 6: strcpy(a.dept,token);
      break;
    case 7: if (strcmp(token,"NT")!=0)
	a.tenure=1;
      else
	a.tenure=0;
      
    }
    token=strtok(NULL,sep);
  }
  if (a.FT==1 && strcmp(a.dept,"CE")==0){ //full time faculty member in department is one FTE
    FTE+=1; //total FTE

    //this part is just being added, mind blown

    if (a.tenure==1){ //because not all fulltimers are tenured, eg Wahdan & Ashraf
      tenure_FTE+=1;   
      FT_tenure_FTE++;
      tenure++;
    }else{
      nonten_FTE++;
      FT_nonten_FTE++;
      non_tenure++;
    }
  }else{ //none CE faculty members, less than one FTE
    if (a.hours>0){ //teaches in CE program    
      float value=(float)a.hours/gethours(a.level,x); //get fraction of FTE
      //here we handle the many new cases that I discovered we need in the report
      if (a.tenure==1 && a.FT==1){
	FT_tenure_FTE+=value;
	//tenure++;
      } else if (a.tenure==0 && a.FT==1){
	FT_nonten_FTE+=value;
	//non_tenure++;
      } else if (a.tenure==1 && a.FT==0){
	PT_tenure_FTE+=value;
	//tenure++;
	puts(a.name);
	puts("panic!!!");
      }
      else if (a.tenure==0 && a.FT==0){
	PT_nonten_FTE+=value;
	//non_tenure++;
      }

      FTE+=value; //add fraction to total FTE
      if(a.tenure==1){ //distribute to either tenured or none tenured
	tenure_FTE+=value; //eg, Hadi, tenured but not 1 FTE in CE
       
	tenure++;
      }else{
	nonten_FTE+=value; //example Mahmoud Khaleel
	non_tenure++;
	
      }
    }
  }
}

int main(){
  char filename[50]; //filename
  FILE *p; //file pointer
  char line[90]; //line read from file

  fillranks(x);
  strcpy(filename,"data.txt");

  //open the file
  p=fopen(filename,"r");
  if(p==NULL){
    puts("Could not find file, exiting program\n");
    return -1;
  }

  while(!feof(p)){
    fgets(line,90,p); //read a line
    processline(line); //process it
  }

  printf("The total FTE so far is %f\n",FTE);
  printf("No of tenured faculty %d, with FTE %f\n",tenure,tenure_FTE);
  printf("No of non-tenured faculty %d, with FTE %f\n",non_tenure,nonten_FTE);

  printf("Pt tenured %f\n",PT_tenure_FTE);
  printf("Pt non-tensure %f\n",PT_nonten_FTE);
  printf("Ft tenured %f\n",FT_tenure_FTE);
  printf("Ft non-tenured %f\n",FT_nonten_FTE);

  fclose(p);
  return 0;
}
