#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
pass1(){
	FILE *input,*sym,*temp;
	int i,j,k,line=1;
	char c,str[100];
	input=fopen("code.txt","r");						//Opens Code File
	if(input==NULL){									//Checks code file exist or not
		printf("Code File Not Exist\n");
		exit(0);
	}
	temp=fopen("temp.txt","w");							//creates a temporary file temp.txt
	sym=fopen("symbol.txt","w");						//creates file symbol.txt to store lables
	c=getc(input);
	while(c==' ' || c=='\n')							//removes spaces or empty line at starting of code
		c=getc(input);				
	if(c==EOF){											//Checks if the code file in empty or not
		printf("Code File Is Empty\n");
		exit(0);
	}
	while(c!=EOF){										//converts small case to large case and writes in temp.txt
		if(c>96 && c<123)
		c-=32;
		putc(c,temp);
		c=getc(input);
	}
	fclose(temp);
	fclose(input);
	temp=fopen("temp.txt","r"); 
	input=fopen("input.txt","w");						//creates file input.txt
	while(1){
		c=getc(temp);								
		while(c==' ' || c=='\n')						//removes spaces at starting of each line //removes empty lines between code
			c=getc(temp);	
		if(c==EOF)										//terminates while loop at the end of the file
			break;
		for(k=0;c!='\n' && c!=EOF;k++)					//stores a line of code in str
		{
			str[k]=c;
			c=getc(temp);
		}
		str[k]='\0';									//writes null char at end of str
		for(i=0;i<k;i++){								//loop executes for each char in str
			if(str[i]==':')								//if : found writes the label and line no. in symbol.txt
			{
				for(j=0;j<i;j++){
					if(str[j]!=' ')
						putc(str[j],sym);
				}
				fprintf(sym," %d ",line);
				putc('\n',sym);
				break;
			}
		}
		if(str[i]==':')									//writes the remaining string in input.txt
		{
			for(i++;str[i]==' ';i++);
			fprintf(input,"%s\n",str+i);
		}
		else{
			fprintf(input,"%s\n",str);
		}
		line++;
	}
	fclose(sym);
	fclose(input);
	fclose(temp);
	remove("temp.txt");									//Deletes the temp.txt
	checkLable();
}
pass2(){
	FILE *input,*output;
	int line=1,n,i,j;
	char str[100],opcode[32],c,opr[5][20];
	input=fopen("input.txt","r");
	output=fopen("output.txt","w");						//creates the output file output.txt
	while(1){
		c=getc(input);
		while(c==' ' || c=='\n')
			c=getc(input);
		if(c==EOF){										//checks for end of file
			printf("Task Completed Succesfully\n");
			exit(0);
		}
		for(i=0;c!=' ' && c!='\n';i++){					//stores first keyword of instruction line in str
			str[i]=c;
			c=getc(input);
		}
		str[i]='\0';
		while(c==' ')
				c=getc(input);
		for(j=0,n=0;c!='\n';n++,j=0){					//stores the operands in a 2-D char array opr[][]
			while(c!=' ' && c!='\n'){					//N will store the no. of operands
				opr[n][j]=c;
				c=getc(input);
				j++;
			}
			opr[n][j]='\0';
			while(c==' ')
				c=getc(input);	
		}
		sopcode(str,opcode,line,0);						
		if(opcode[0]=='0'){
			if(n!=2){
				printf("Invalid no. of operands in Line No. %d\n No. of operands should be Two",line);
				exit(0);
			}
			fprintf(output,"%s",opcode);
			sopcode(opr[0],opcode,line,1);
			fprintf(output,"%s",opcode);
			sopcode(opr[1],opcode,line,1);
			fprintf(output,"%s",opcode);
			fprintf(output,"0000000000000000\n");
		}
		else if(opcode[4]=='0'){
			if(opcode[5]=='0')
			{
				if(n!=1){
				printf("Invalid no. of operands in Line No. %d\nNo. of operands should be One",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);
				sopcode(opr[0],opcode,line,1);
				fprintf(output,"%s",opcode);
				fprintf(output,"0000000000000000\n");	
			}
			else{
				if(n!=2){
				printf("Invalid no. of operands in Line No. %d\nNo. of operands should be Two",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);
				sopcode(opr[0],opcode,line,1);
				fprintf(output,"%s",opcode);
				strbin(opr[1],opcode,line);
				fprintf(output,"%s\n",opcode);
			}
		}
		else{
			if(opcode[8]=='0')
			{
				if(n!=0){
				printf("Invalid no. of operands in Line No. %d\nNo. of operands should be zero",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);	
				fprintf(output,"0000000000000000\n");
			}
			else{
				if(n!=1){
				printf("Invalid no. of operands in Line No. %d\nNo. of operands should be one",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);
				ssymbol(opr[0],opcode,line);
				fprintf(output,"%s\n",opcode);
			}
		}
        line++;
	}
}
sopcode(char str[],char opcode[],int line,int s){
	FILE *inst;
	if(s==0)
		inst=fopen("instruction.txt","r");
	else
		inst=fopen("register.txt","r");
	if(inst==NULL && s==0){
		printf("Instruction File Not Exist");
		exit(0);
	}
	else if(inst==NULL && s==1){
		printf("Register File Not Exist");
		exit(0);
	}
	char temp1[20],temp2[33];
	while(1){
		fscanf(inst,"%s%s",temp1,temp2);
		if(strcmp(temp1,str)==0){
			strcpy(opcode,temp2);
			fclose(inst);
			break;
		}
		if(feof(inst)){
			fclose(inst);
		    printf("Instruction or operand %s in line no. %d dont Exist",str,line);
		    exit(0);
		} 
	}
}
ssymbol(char str[],char opcode[],int line){
	FILE *sym=fopen("symbol.txt","r");
	char temp1[10];
	int temp2=0;
	while(1){
		fscanf(sym,"%s %d",temp1,&temp2);
		if(strcmp(temp1,str)==0){
			fclose(sym);
			break;
		}
		if(feof(sym)){
			fclose(sym);
			printf("Label %s in line no. %d dont Exist",str,line);
		    exit(0);
		} 
	}
	intbin(temp2,opcode,line);
}
strbin(char str[],char temp[],int line){
	int a=strlen(str),i,n=0,j;
	if(a!=4)
	{
		printf("Invalid hexadecimal data %s in line no. %d",str,line);
		exit(0);
	}
	for(i=0,j=3;i<4;i++,j--){
		if(str[i]>47 && str[i]<58)
		n+=pow(16,j)*(str[i]-48);
		else if(str[1]>64 && str[i]<71)
		n+=pow(16,j)*(str[i]-55);
		else{
			printf("Invalid hexadecimal char %c in line no. %d",str[i],line);
			exit(0);
		}
	}
	intbin(n,temp,line);
}
intbin(int n,char temp[],int line){
	int i;
	if(n<0 || n>65535){
		printf("Data out of bounds in line no. %d",line);
		exit(0);
	}
	temp[16]='\0';
	for(i=15;i>=0;i--)
	{
		if(n%2==1)
			temp[i]='1';
		else
			temp[i]='0';
		n/=2;
	}
}
checkLable(){													//Checks for Duplicates lables
	FILE *sym=fopen("symbol.txt","r");
	char str[20][20];
	int arr[20],count,i,j,k;
	for(count=0;1;count++){
		if(feof(sym))
			break;
		fscanf(sym,"%s %d",str[count],&arr[count]);
	}
	fclose(sym);
	if(count==0)
		return;
	for(i=0;i<count;i++){
		for(j=i+1;j<count;j++){
			if(strcmp(str[i],str[j])==0){
				printf("Duplicate Lables in Line %d and %d\n",arr[i],arr[j]);
				exit(0);
			}
		}
	}
}
int main(){
	pass1();
	pass2();
	return 0;
}
