#include <stdio.h>
#include <string.h>
#include "lib/c_ins_queue.h"
#include "lib/oslib.h"
#include "util/fio_util.h"


int main(int argC, char* argv[]){
    
    OS opSys;
    instruction output;
    float f = 0.0;

    constructOS(&opSys);
    constructIns(&output);

    output = loadInstruction('P',"run", 20);
    configOS(&opSys, "config.cnf");
    

    processInstruction(&opSys, &output, &f);

    output = loadInstruction('O',"hard drive", 20);
    processInstruction(&opSys, &output, &f);

    output = loadInstruction('I',"keyboard", 30);
    processInstruction(&opSys, &output, &f);
    return 0;
}
