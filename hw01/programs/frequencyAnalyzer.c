#include<stdio.h>
#include<stdlib.h>

/* a simple structure to store information about the frequency analysis of the text */
struct frequencyAnalysis {
    int index;
    int total;
    char characters[27];
    int occurrences[27];
    float percentages[27];
};

/* a simple function that tells me if some character is already stored */
int find(char c, char chars[27]){
  int pos=-1;
  for(int i=0; i<27; i++) if(chars[i]==c) pos=i;
  return pos;
}

/* a simple function to sort the resulting percentages */
void structSort(struct frequencyAnalysis f){
  int i,j;
  float ftemp;
  char ctemp;
  int itemp;
  for(i=0; i<27; i++){
    for(j=0; j<27-i; j++){
      if(f.occurrences[j]<f.occurrences[j+1]){
        ftemp=f.percentages[j];
        f.percentages[j]=f.percentages[j+1];
        f.percentages[j+1]=ftemp;
        itemp=f.occurrences[j];
        f.occurrences[j]=f.occurrences[j+1];
        f.occurrences[j+1]=itemp;
        ctemp=f.characters[j];
        f.characters[j]=f.characters[j+1];
        f.characters[j+1]=ctemp;
      }
    }
  }
}

/* a simple function to dump on screen the resulting frequency analysis */
void structDump(struct frequencyAnalysis f){
  printf("\n|----------------------------------------|\n");
  printf("| CHARACTER |  OCCURRENCES |  FREQUENCY  |\n");
  printf("|----------------------------------------|\n");
  for(int i=0; i<(f.index-1); i++){
    if((f.occurrences[i]>=10)&&(f.percentages[i]>=10)){
      printf("|     %c     |      %d      |  %f%         |\n", 
      f.characters[i], f.occurrences[i], f.percentages[i]);
    } else if(f.occurrences[i]>=10){
        printf("|     %c     |      %d      |  0%f%         |\n", 
        f.characters[i], f.occurrences[i], f.percentages[i]);
    } else if(f.percentages[i]>=10){
        printf("|     %c     |      0%d      |  %f%         |\n", 
        f.characters[i], f.occurrences[i], f.percentages[i]);
    } else {
      printf("|     %c     |      0%d      |  0%f%         |\n", 
      f.characters[i], f.occurrences[i], f.percentages[i]);
    }
  }
  printf("|----------------------------------------|\n\n");
}

int main(){

  FILE *sourceFile;
  char sourcePath[100];
  char c;
  struct frequencyAnalysis f={0, 0, {(char) 0}, {0}, {0}};
  
  printf("enter source file path: ");
  scanf("%s", sourcePath);
  
  sourceFile=fopen(sourcePath, "r");
  if(sourceFile==NULL){
    printf("Cannot open file %s\n", sourcePath);
    exit(EXIT_FAILURE);
  }
 
  while((c=fgetc(sourceFile))!=EOF){
    int pos=find(c, f.characters);
    f.total+=1;
    if(pos==-1){
        f.characters[f.index]=c;
        f.occurrences[f.index]=1;
        f.index+=1;
    } else f.occurrences[pos]+=1;
  }

  f.total--;
  
  for(int i=0; i<(f.index-1); i++){ 
    f.percentages[i]=((float) f.occurrences[i]/(float) f.total)*100;
  }
  
  structSort(f);
  structDump(f);
  fclose(sourceFile);
  
  return 0;
}
