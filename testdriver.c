#include <stdio.h>
#include <string.h>
#include "lib/c_ins_queue.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"

int main(int argC, char* argv[]){
    
    OS opSys;
    pcbQueue readyQ;
    insQueue q;
    instruction output;
    float f = 0.0;
    char* s;
    FILE* fin;

    alloStr(&s, 50);
    constructOS(&opSys);
    constructIns(&output);
    constructQueue(&q);
    constructPcbQueue(&readyQ, 10);

    configOS(&opSys, "config.cnf");
    
    processmdf(&readyQ, opSys.metaDatFile); 


    return 0;
}
