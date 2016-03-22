#include <stdio.h>
#include <string.h>
#include "lib/c_ins_queue.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"

int main(int argC, char* argv[]){
    
    OS opSys;
    PCB pcb;
    insQueue q;
    instruction output;
    float f = 0.0;
    char* s;
    FILE* fin;

    alloStr(&s, 50);
    constructOS(&opSys);
    constructIns(&output);
    constructQueue(&q);

    fin = fopen("t1.mdf","r");
    
    fgets(s, 50, fin);
    clearStr(&s);

    getInstruction(&s, fin);

    getProgram(&q, fin);

    fclose(fin);

    configOS(&opSys, "config.cnf");
    
    constructPCB(&pcb, &q, 1);
    
    runPCB(&opSys, &pcb, &f);

    return 0;
}
