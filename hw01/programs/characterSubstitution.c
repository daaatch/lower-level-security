#include<stdio.h>
#include<stdlib.h>

int main(){
  FILE *sourceFile, *destFile;
  char sourcePath[100], destPath[100];
  char ch;
  char sub;
  
  printf("enter source file path: ");
  scanf("%s", sourcePath);
  
  sourceFile=fopen(sourcePath, "r");
  if(sourceFile==NULL){
    printf("Cannot open file %s\n", sourcePath);
    exit(EXIT_FAILURE);
  }
  
  printf("enter destination file path: ");
  scanf("%s", destPath);
  
  destFile=fopen(destPath, "w");
  if(destPath==NULL){
    printf("Cannot open file %s\n", destPath);
    fclose(sourceFile);
    exit(EXIT_FAILURE);
  }
  
  while((ch=fgetc(sourceFile))!=EOF){
    if(ch=='T') sub='E';
    else if(ch=='E') sub='T';
    else if(ch=='O') sub='A';
    else if(ch=='U') sub='O';
    else if(ch=='I') sub='I';
    else if(ch=='H') sub='N';
    else if(ch=='J') sub='S';
    else if(ch=='W') sub='H';
    else if(ch=='S') sub='R';
    else if(ch=='A') sub='D';
    else if(ch=='B') sub='L';
    else if(ch=='C') sub='C';
    else if(ch=='F') sub='U';
    else if(ch=='L') sub='M';
    else if(ch=='Y') sub='W';
    else if(ch=='Q') sub='F';
    else if(ch=='R') sub='G';
    else if(ch=='G') sub='Y';
    else if(ch=='D') sub='P';
    else if(ch=='N') sub='B';
    else if(ch=='X') sub='V';
    else if(ch=='P') sub='K';
    else if(ch=='K') sub='J';
    else if(ch=='M') sub='Q';
    else sub=ch;
    fputc(sub, destFile);
  }
  
  fclose(sourceFile);
  fclose(destFile);
  return 0;
}
