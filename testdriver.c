#include <stdio.h>
#include <string.h>
#include "lib/c_ins_queue.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"

int runOS(OS* opSys, SimpleTimer* sysTime){
    //variables
    pcbQueue readyQ;
    PCB running;
    insQueue q;
    char* elapTime;
    char* outBuff; 
    float f = 0.0;

    //constructions
    constructQueue(&q);
    constructPcbQueue(&readyQ, 10);
    alloStr(&elapTime, 10);
    alloStr(&outBuff, 50);

    stop(sysTime);

    getElapsedTime(&elapTime, sysTime);

    f += atof(elapTime);

    strcat(outBuff, elapTime);
    strcat(outBuff, " - Simulator Program Starting");

    outputHandler(opSys, outBuff);


    start(sysTime);
    //begin processing
    processmdf(opSys, &readyQ); 
    
    stop(sysTime);

    getElapsedTime(&elapTime, sysTime);

    f += atof(elapTime);

    start(sysTime);

    heapsort(opSys, &readyQ);

    while(pcbq_dequeue(&readyQ, &running))
       {
        runPCB(opSys, &running, &f);
       }

    stop(sysTime);

    getElapsedTime(&elapTime, sysTime);

    f += atof(elapTime);
    
    clearStr(&outBuff);
    strcat(outBuff, elapTime);
    strcat(outBuff, " - Simulator Program Ending");

    outputHandler(opSys, outBuff);
}



int main(int argC, char* argv[]){
    
    OS opSys;
    SimpleTimer clock;

    makeSimpleTimer(&clock);
    
    start(&clock);

    constructOS(&opSys);
    configOS(&opSys, argv[1]);

    runOS(&opSys, &clock);

    return 0;
}
