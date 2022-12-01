#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7;

struct PDB{
	int pid;
	char** Code;
	int Line_NUM;
	int current_instruction_num;
	int age;
	struct PDB* nextPDB;
};
typedef struct PDB pdb;  




int help();
int quit();
int badcommand();
int badcommandTooManyTokens();
int badcommandWrongAmountProgs();
int badcommandWrongPolicy();
int badcommandFileDoesNotExist();
int badcommandDuplicateProgram();
int ErrorFailLoadingCode();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int my_ls();
int echo();
int exec();


int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		if (strcmp(command_args[0], "set")==0 && args_size > MAX_ARGS_SIZE) {
			return badcommandTooManyTokens();
		}
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand();
		char* value = (char*)calloc(1,150);
		char spaceChar = ' ';

		for(int i = 2; i < args_size; i++){
			strncat(value, command_args[i], 30);
			if(i < args_size-1){
				strncat(value, &spaceChar, 1);
			}
		}
		return set(command_args[1], value);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
		
	} else if (strcmp(command_args[0], "exec")==0) {
		if (args_size < 2 || args_size > 5){return badcommandWrongAmountProgs();}
		if ((strcmp(command_args[args_size-1],"FCFS")== 0)
				|| (strcmp(command_args[args_size-1],"RR")== 0)
						|| (strcmp(command_args[args_size-1],"AGING")== 0)
								|| (strcmp(command_args[args_size-1],"SJF")== 0))
		{
			char** proglist = (char**)malloc((args_size-1) * sizeof(char*));
			int icounter;

			for( icounter = 1; icounter < args_size-1; icounter++){
				proglist[icounter-1]=malloc(100 * sizeof(char));
				char* address=malloc(100 * sizeof(char));
				strcpy(address,"./" );
						strcat(address,command_args[icounter]);
						strcpy(proglist[icounter-1],address );
						//printf("%s\n",address);
						
					}

			return exec(proglist,command_args[args_size-1],args_size-2);
			
		}
		else{return badcommandWrongPolicy();}
		
	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size > 2) return badcommand();
		return my_ls();
	
	}else if (strcmp(command_args[0], "echo")==0) {
		if (args_size > 2) return badcommand();
		return echo(command_args[1]);
	
	} else {
		return badcommand();
	}
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many tokens");
	return 2;
}
int badcommandWrongAmountProgs(){
	printf("%s\n", "Bad command: Too many Programs or No programe is Given!");
	return 2;
}
int badcommandWrongPolicy(){
	printf("%s\n", "Bad command: Policy doesnot exist!");
	return 2;
}
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}
int badcommandDuplicateProgram(){
	printf("%s\n", "Bad command: same file name");
	return 3;
}
int ErrorFailLoadingCode(){
	printf("%s\n", "Error: Fail to load code");
	return 4;
}
int set(char* var, char* value){

	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);

	mem_set_value(var, value);

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	int errCode = 0;
	
	FILE *p = fopen(script,"rt");  // the program is in a file
	if(p == NULL){
		return badcommandFileDoesNotExist();
	}
	
	
	pdb pdb_jarry1;
	pdb *pdbcurrent = &pdb_jarry1;
	
	int linemarker =0;
	pdb_jarry1.Code =(char**)malloc(300 * sizeof(char*));
	
	if(pdb_jarry1.Code==NULL){ErrorFailLoadingCode();}
	
	pdb_jarry1.Code[linemarker]=malloc(100 * sizeof(char));
	
	if(pdb_jarry1.Code[linemarker]==NULL){ErrorFailLoadingCode();}
	
	fgets(pdb_jarry1.Code[linemarker],99,p);
	while(1){
		linemarker+=1;
		pdb_jarry1.Code[linemarker]=malloc(100 * sizeof(char));
		if(pdb_jarry1.Code[linemarker]==NULL){ErrorFailLoadingCode();}
		if(feof(p)){
			pdb_jarry1.Code[linemarker]=NULL;
			pdb_jarry1.Line_NUM=linemarker;
			break;
		}
			
		fgets(pdb_jarry1.Code[linemarker],99,p);
	}
	
    fclose(p);
    
    pdb_jarry1.current_instruction_num=0;
   
    int PIDpool[1000] ;
    int counter1;

    for (counter1 = 0; counter1 < 1000; counter1++)
    {
    	PIDpool[counter1]=counter1;
    }
    
    int counter2;
        for (counter2 = 0; counter2 < 1000; counter2++)
            {
            	if(PIDpool[counter2]>0){pdb_jarry1.pid=PIDpool[counter2];
            	PIDpool[counter2]=-1;}
            }
            	
        
    /*int counter3=0;
    while(pdbcurrent->Code[counter3]!=NULL){
    	printf("%s",pdbcurrent->Code[counter3]);
    	counter3++;
    }*/
    while(1){
    	if(pdbcurrent->Code[(*(pdbcurrent)).current_instruction_num]==NULL)
    	{	
    		if ((*pdbcurrent).pid == (*(pdbcurrent->nextPDB)).pid){
    			//printf("HERE33!\n");
    			break;
    		}
    		pdb* buffer ;
    		buffer = pdbcurrent;
    		pdbcurrent=(pdbcurrent->nextPDB);
    		PIDpool[buffer->pid]=buffer->pid;
    		free(buffer);
    	}
    	//printf("%s\n ccccc",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
    	errCode = parseInput(pdbcurrent->Code[pdbcurrent->current_instruction_num]);
    	pdbcurrent->current_instruction_num+=1;
    	pdbcurrent->nextPDB=&pdb_jarry1;
    }
        PIDpool[pdbcurrent->pid]=pdbcurrent->pid;
        free(pdbcurrent->Code);
	return errCode;
}
int exec(char** progLIST,char* POLICY,int progamount){
	int errCode = 0; 
	if(progamount==1){return run(progLIST[0]);} 
	
	if((strcmp(progLIST[0],progLIST[1]) == 0) 
			|| (strcmp(progLIST[1],progLIST[2])== 0) 
			|| (strcmp(progLIST[0],progLIST[2])== 0)){
		return badcommandDuplicateProgram();
	}
	//printf("%s\n","FCFS");
	int PIDpool[1000] ;
	int counter1;
	for (counter1 = 0; counter1 < 1000; counter1++)
	{
	    PIDpool[counter1]=counter1;
	}
	//printf("%s\n","FCFS");
	pdb PDBLIST[progamount];
	int counter_prog;
	for (counter_prog = 0; counter_prog < progamount; counter_prog++)
	{
		FILE *p = fopen(progLIST[counter_prog],"rt");  // the program is in a file
		//printf("%s\n","FCFS");
		if(p == NULL){
			return badcommandFileDoesNotExist();
		}
		
		int linemarker =0;
		
		PDBLIST[counter_prog].Code =(char**)malloc(300 * sizeof(char*));
		
		if(PDBLIST[counter_prog].Code==NULL){return ErrorFailLoadingCode();}
		
		PDBLIST[counter_prog].Code[linemarker]=malloc(100 * sizeof(char));
		
		if(PDBLIST[counter_prog].Code[linemarker]==NULL){return ErrorFailLoadingCode();}
		
		fgets(PDBLIST[counter_prog].Code[linemarker],99,p);
		while(1){
			linemarker+=1;
			PDBLIST[counter_prog].Code[linemarker]=malloc(100 * sizeof(char));
			if(PDBLIST[counter_prog].Code[linemarker]==NULL){return ErrorFailLoadingCode();}
			if(feof(p)){
				PDBLIST[counter_prog].Code[linemarker]=NULL;
				PDBLIST[counter_prog].Line_NUM=linemarker;
				break;
			}
				
			fgets(PDBLIST[counter_prog].Code[linemarker],99,p);
		}
		fclose(p);
		PDBLIST[counter_prog].pid=PIDpool[counter_prog];
		PDBLIST[counter_prog].age=linemarker;
	}
	//printf("%s\n",PDBLIST[2].Code[0]);
	
		
	if (strcmp(POLICY,"FCFS") ==0 )
		{	
		int progcount =progamount;
		//printf("HERE33!\n");
		pdb *pdbcurrent = &(PDBLIST[0]);
			for (counter_prog = 0; counter_prog < progamount; counter_prog++)
				{
				
				if (counter_prog==progamount-1){PDBLIST[counter_prog].nextPDB=NULL;}
				PDBLIST[counter_prog].nextPDB=(pdb *) malloc(sizeof(pdb));
				PDBLIST[counter_prog].nextPDB=&(PDBLIST[counter_prog+1]);
				}
			
			
			while(1){
				if(pdbcurrent->Code[(*(pdbcurrent)).current_instruction_num]==NULL)
				{	
					if (progcount == 1){
						//printf("HERE33!\n");
						break;
					}
					//printf("%d\n",(*pdbcurrent).pid);
					//printf("%d\n",PDBLIST[2].nextPDB==NULL);
					//printf("%d\n",(PDBLIST[2].nextPDB)->pid);
					pdb* buffer ;
					buffer = pdbcurrent;
					
					pdbcurrent=(pdbcurrent->nextPDB);
					//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
					PIDpool[buffer->pid]=buffer->pid;
					for(int i=0;i<buffer->Line_NUM;i++){
						free(buffer->Code[i]);
					}
					free(buffer->Code);
					
					buffer->nextPDB=NULL;
					//free(buffer);
					//printf("FREE!\n");
					progcount--;
					
				}
				//printf("%s\n ccccc",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
				errCode = parseInput(pdbcurrent->Code[pdbcurrent->current_instruction_num]);
				pdbcurrent->current_instruction_num+=1;
			}
				PIDpool[pdbcurrent->pid]=pdbcurrent->pid;
				for(int i=0;i<pdbcurrent->Line_NUM;i++){
										free(pdbcurrent->Code[i]);
									}
				free(pdbcurrent->Code);
				return errCode;
		}
	else if (strcmp(POLICY,"SJF") ==0 )
		{
		//printf("HERE33!\n");

		int i, j;
		struct PDB temp;
		    
		    for (i = 0; i < progamount - 1; i++)
		    {
		        for (j = 0; j < (progamount - 1-i); j++)
		        {
		            if (PDBLIST[j].Line_NUM < PDBLIST[j + 1].Line_NUM)
		            {
		                temp = PDBLIST[j];
		                PDBLIST[j] = PDBLIST[j + 1];
		                PDBLIST[j + 1] = temp;
		            } 
		        }
		    }
		    //printf("HERE33!\n");
		    
		    for (counter_prog = progamount-1; counter_prog > 0; counter_prog--)
				{
		    	if (counter_prog==progamount-1){PDBLIST[counter_prog].nextPDB=NULL;}
				PDBLIST[counter_prog].nextPDB=(pdb *) malloc(sizeof(pdb*));
				PDBLIST[counter_prog].nextPDB=&(PDBLIST[counter_prog-1]);
				}
		    pdb *pdbcurrent = &(PDBLIST[progamount-1]);

		    while(1){
		    	//printf("HERE33!\n");
		    	//printf("%s\n ",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
				if(pdbcurrent->Code[pdbcurrent->current_instruction_num]==NULL)
				{	
					if ((*pdbcurrent).nextPDB == NULL){
						//printf("HERE33!\n");
						break;
					}
					//printf("%d\n",(*pdbcurrent).pid);
					//printf("%d\n",PDBLIST[2].nextPDB==NULL);
					//printf("%d\n",(PDBLIST[2].nextPDB)->pid);
					pdb* buffer ;
					buffer = pdbcurrent;
					
					pdbcurrent=(pdbcurrent->nextPDB);
					//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
					PIDpool[buffer->pid]=buffer->pid;
					for(int i=0;i<buffer->Line_NUM;i++){
						free(buffer->Code[i]);
					}
					free(buffer->Code);
					
					buffer->nextPDB=NULL;
					//free(buffer);
					//printf("FREE!\n");
					
				}
				//printf("%s\n ",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
				errCode = parseInput(pdbcurrent->Code[pdbcurrent->current_instruction_num]);
				pdbcurrent->current_instruction_num+=1;
			}
				PIDpool[pdbcurrent->pid]=pdbcurrent->pid;
				for(int i=0;i<pdbcurrent->Line_NUM;i++){
										free(pdbcurrent->Code[i]);
									}
				free(pdbcurrent->Code);
				return errCode;
		}
	else if (strcmp(POLICY,"RR") ==0 )
		{
		int progamount_count=progamount;
		pdb *pdbcurrent = &(PDBLIST[0]);
		for (counter_prog = 0; counter_prog < progamount; counter_prog++)
			{
			
			if (counter_prog==progamount-1){PDBLIST[counter_prog].nextPDB=NULL;}
			PDBLIST[counter_prog].nextPDB=(pdb *) malloc(sizeof(pdb));
			PDBLIST[counter_prog].nextPDB=&(PDBLIST[counter_prog+1]);
			}
		
		int command_counter=0;
		while(1){
			if(pdbcurrent->Code[(*(pdbcurrent)).current_instruction_num]==NULL)
			{	
				
				//printf("%d\n",(*pdbcurrent).pid);
				//printf("%d\n",PDBLIST[2].nextPDB==NULL);
				//printf("%d\n",(PDBLIST[2].nextPDB)->pid);
				command_counter=0;
				//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
				pdb* buffer ;
				buffer = pdbcurrent;
				if(progamount_count==3){
					pdbcurrent->nextPDB->nextPDB->nextPDB=(pdbcurrent->nextPDB);
					//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
					pdbcurrent=(pdbcurrent->nextPDB);
					progamount_count--;
					//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
				}
				else if(progamount_count==2){
					pdbcurrent=(pdbcurrent->nextPDB);
					progamount_count--;
				}
				else{
					break;
				}

				PIDpool[buffer->pid]=buffer->pid;
				//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
				for(int i=0;i<buffer->Line_NUM-1;i++){
					free(buffer->Code[i]);
				}
				//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
				free(buffer->Code);
				
				
				//free(buffer);
				//printf("FREE!\n");
				
			}
			//printf("%s\n ccccc",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
			if(command_counter>=2){
				if(progamount_count==3){
					pdbcurrent->nextPDB->nextPDB->nextPDB=(pdb *) malloc(sizeof(pdb*));
					pdbcurrent->nextPDB->nextPDB->nextPDB=pdbcurrent;
					pdbcurrent=(pdbcurrent->nextPDB);
					command_counter=0;
				}
			
			else{
				pdbcurrent->nextPDB->nextPDB=(pdb *) malloc(sizeof(pdb*));
					pdbcurrent->nextPDB->nextPDB=pdbcurrent;
					pdbcurrent=(pdbcurrent->nextPDB);
					command_counter=0;
			}
			}
			else{
			errCode = parseInput(pdbcurrent->Code[pdbcurrent->current_instruction_num]);
			command_counter+=1;
			pdbcurrent->current_instruction_num+=1;
			}
		}
			PIDpool[pdbcurrent->pid]=pdbcurrent->pid;
			for(int i=0;i<pdbcurrent->Line_NUM;i++){
									free(pdbcurrent->Code[i]);
								}
			free(pdbcurrent->Code);
			return errCode;
		}
	else if (strcmp(POLICY,"AGING") ==0 )
		{
		int progamount_count2=progamount;
		int aged=0;
		int i, j;
		struct PDB temp;
				    
		for (i = 0; i < progamount - 1; i++)
		{
			for (j = 0; j < (progamount - 1-i); j++)
			{
				if (PDBLIST[j].Line_NUM < PDBLIST[j + 1].Line_NUM)
				{
					temp = PDBLIST[j];
					PDBLIST[j] = PDBLIST[j + 1];
					PDBLIST[j + 1] = temp;
				} 
			}
		}
		//printf("HERE33!\n");
		
		for (counter_prog = progamount-1; counter_prog > 0; counter_prog--)
			{
			if (counter_prog==progamount-1){PDBLIST[counter_prog].nextPDB=NULL;}
			PDBLIST[counter_prog].nextPDB=(pdb *) malloc(sizeof(pdb*));
			PDBLIST[counter_prog].nextPDB=&(PDBLIST[counter_prog-1]);
			}
		pdb *pdbcurrent = &(PDBLIST[progamount-1]);

		while(1){
			//printf("HERE33!\n");
			//printf("%s\n ",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
			if(pdbcurrent->Code[pdbcurrent->current_instruction_num]==NULL)
			{	
				if ((*pdbcurrent).nextPDB == NULL){
					//printf("HERE33!\n");
					break;
				}
				//printf("%d\n",(*pdbcurrent).pid);
				//printf("%d\n",PDBLIST[2].nextPDB==NULL);
				//printf("%d\n",(PDBLIST[2].nextPDB)->pid);
				pdb* buffer ;
				buffer = pdbcurrent;
				
				pdbcurrent=(pdbcurrent->nextPDB);
				//printf("%s\n",(pdbcurrent->nextPDB)->Code[0]);
				PIDpool[buffer->pid]=buffer->pid;
				for(int i=0;i<buffer->Line_NUM;i++){
					free(buffer->Code[i]);
				}
				free(buffer->Code);
				
				buffer->nextPDB=NULL;
				//free(buffer);
				//printf("FREE!\n");
				
			}
			if (aged==1 || progamount_count2==1){
				errCode = parseInput(pdbcurrent->Code[pdbcurrent->current_instruction_num]);
				pdbcurrent->current_instruction_num+=1;
				aged=0;
				continue;
			}
			else if(aged==0){
				for(int i=0;i<progamount;i++){
				if(PDBLIST[counter_prog].pid==pdbcurrent->pid){continue;}
				else{if(PDBLIST[counter_prog].age>0){PDBLIST[counter_prog].age-=1;}}
				//printf("FREE!\n");
				}
				aged=1;
				if(progamount_count2==3){
					if ((pdbcurrent->nextPDB)->age<pdbcurrent->age){
					pdbcurrent->nextPDB->nextPDB->nextPDB=(pdb *) malloc(sizeof(pdb*));
					pdbcurrent->nextPDB->nextPDB->nextPDB=pdbcurrent;
					pdbcurrent=(pdbcurrent->nextPDB);
					printf("FREE!\n");
					continue;
					}
					else if ((pdbcurrent->nextPDB->nextPDB)->age<pdbcurrent->age){
						pdbcurrent->nextPDB->nextPDB->nextPDB=(pdb *) malloc(sizeof(pdb*));
						pdbcurrent->nextPDB->nextPDB->nextPDB=pdbcurrent;
						pdbcurrent=(pdbcurrent->nextPDB->nextPDB);
						continue;
					}
					else{continue;}
				}
				else{
					if ((pdbcurrent->nextPDB)->age<pdbcurrent->age){
						pdbcurrent->nextPDB->nextPDB=(pdb *) malloc(sizeof(pdb*));
						pdbcurrent->nextPDB->nextPDB=pdbcurrent;
						pdbcurrent=(pdbcurrent->nextPDB);
						continue;
						}
					else{continue;}
									}
				
					
					

			//printf("%s\n ",pdbcurrent->Code[pdbcurrent->current_instruction_num]);
			
		}
			PIDpool[pdbcurrent->pid]=pdbcurrent->pid;
			
			for(int i=0;i<pdbcurrent->Line_NUM;i++){
									free(pdbcurrent->Code[i]);
								}
			free(pdbcurrent->Code);
			return errCode;
		}
		}
	
									
									
									
									
		
	
	
}
int my_ls(){
	int errCode = system("ls | sort");
	return errCode;
}

int echo(char* var){
	if(var[0] == '$'){
		var++;
		printf("%s\n", mem_get_value(var)); 
	}else{
		printf("%s\n", var); 
	}
	return 0; 
}