

const char* ENDOFSET = "A(end)0";
const char* ENDOFSETS = "S(end)0";


void getInstruction(char** insStr, FILE* fin)
   {
    //variables
    char buffer[1];

    //seed loop
    buffer[0] = fgetc(fin);

    //while input char != ; or '.' 
    while((buffer[0] != ';') && (buffer[0] != '.'))
       {
        //strcat
        appendChar(insStr, buffer[0]);        

        //get char
        buffer[0] = fgetc(fin);
       }
   }


instruction parseIns(char* strIns)
   {
    //variables
    char* tok;
    char  comp;
    char* descrip;
    int   cycles;
    instruction rins;    

    //construct
    alloStr(&tok, 12);
    alloStr(&descrip, 12);

    tok = strtok(strIns, "(");
    comp = tok[0];

    tok = strtok(NULL, ")");
    strcpy(descrip, tok);

    tok = strtok(NULL, " \n");
    cycles = atoi(tok);

    rins = loadInstruction(comp, descrip, cycles);

    return rins;
   }

int getProgram(insQueue *programQ, FILE* fin)
   {
    //variables
    char* buffer;
    instruction tins;
    int x = 0;

    //construct
    alloStr(&buffer, 20);
    constructIns(&tins);

    //read in instructions until returned value is "A(end)0"
    do
       {
        clearStr(&buffer);
        getInstruction(&buffer, fin);
               
        while(!isalpha(buffer[0]))
          {
           removeLeadingChar(&buffer);
          }

        if(buffer[0] == 'S')
           {
            return 0;
           }    
    
        tins = parseIns(buffer);
        enqueue(programQ, tins);
       } while(strcmp(tins.descriptor, "end"));

    return 1;
    
   }
int processmdf(char* fpath)
   {
    //variables
    FILE* mdfFile;
    PCB pcbArr[10];     //temp until I make a PCB vector
    int pid = 0;
    char* garbage; 
    insQueue buff_q;
    
    int x = 0;

    //construction et al
    alloStr(&garbage, 50);
    constructQueue(&buff_q);

    for(x=0; x < 10; x++){
        constructPCB(&(pcbArr[x]), &buff_q, -1);
    }


    mdfFile = fopen(fpath, "r");

    /*add some error checking
     *
     */

    
    //get first line as garbage
    fgets(garbage, 50, mdfFile);
    clearStr(&garbage);
 
    //get first instruction as garbage
    getInstruction(&garbage, mdfFile);


    while(getProgram(&buff_q, mdfFile))
       {
        //linking queue with new pcb
        constructPCB(&(pcbArr[pid]), &buff_q, pid);
        pid++;
        
        //clear queue for next iteration
        clearQueue(&buff_q);    
             
       }

    fclose(mdfFile);
    
    for(pid = 0; pcbArr[pid].pid != -1; pid++){
        printQueue(&(pcbArr[pid].instructions));
        puts("");
    }
    
    return 1;
   }
