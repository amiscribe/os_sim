#include <stdio.h>
#include <string.h>
#include "lib/c_ins_queue.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"

int runOS(OS* opSys){
    //variables
    pcbQueue readyQ;
    PCB running;
    insQueue q;
    float f = 0.0;

    //constructions
    constructQueue(&q);
    constructPcbQueue(&readyQ, 10);
    
    processmdf(opSys, &readyQ); 

    while(pcbq_dequeue(&readyQ, &running))
       {
        runPCB(opSys, &running, &f);
       }

}



int main(int argC, char* argv[]){
    
    OS opSys;
    
    constructOS(&opSys);
    configOS(&opSys, argv[1]);

    runOS(&opSys);

    return 0;
}
