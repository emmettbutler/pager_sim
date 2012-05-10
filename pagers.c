//memory paging simulator by emmett butler
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t memoryLock = PTHREAD_MUTEX_INITIALIZER;
int *physicalMemory;
int *lookupTable;
int *metaTable;
int *disk;
int seq, k, m, n, maxTasks;
int LOGLEVEL;

void *f_thread3(void *x){
    if(LOGLEVEL > 1)
        printf("In thread 1\n");
    int num_added = 0;
    int *sum, nextAdd;
    int new, pageNum, tasksCompleted = 0, algorithm = 0;

    sum = (int *)malloc(sizeof(int*));
    *sum = 0;

    while(algorithm <= 2){
        while(tasksCompleted < maxTasks){
            while(num_added < k){
                srand ( (unsigned)time ( NULL ) );
                nextAdd = random() % n;
                if(LOGLEVEL > 1)
                    printf("Thread3: Fetching variable i = %d\n", nextAdd);
                new = fetch(nextAdd);
                if(new >= 0){
                    if(LOGLEVEL > 1)
                        printf("Thread3 sum = %d = %d + %d\n", *sum+new, *sum, new);
                    *sum += new;
                    num_added++;
                } else {
                    if(LOGLEVEL > 1)
                        printf("Thread3: Lookup failed, paging from disk[%d]\n", nextAdd);
                    switch(algorithm){
                        case 0: pageNum = page_in_random(nextAdd); break;
                        case 1: pageNum = page_in_MRU(nextAdd); break;
                        case 2: pageNum = page_in_LRU(nextAdd); break;
                    }
                }
                seq++;
                new = 0;
            }
            tasksCompleted++;
            if(LOGLEVEL >= 1)
                printf("3, %d, %d\n", tasksCompleted, seq);
            num_added = *sum = 0;
            if(LOGLEVEL > 1)
                printf("Thread3 completed %d sum tasks.\n", tasksCompleted);
        }
        if(LOGLEVEL >= 1){
            switch(algorithm){
                case 0: printf("Thread 3 Tested random algorithm\n"); break;
                case 1: printf("Thread 3 Tested MRU algorithm\n"); break;
                case 2: printf("Thread 3 Tested LRU algorithm\n"); break;
            }
        }
        algorithm++;
        tasksCompleted = 0;
    }
    if(LOGLEVEL > 1)
        printf("Thread3 sum before return: %d\n", *sum);
    return (void*)*sum;
}

void *f_thread2(void *x){
    if(LOGLEVEL > 1)
        printf("In thread 2\n");
    int num_added = 0, variableWindowBase = 0;
    int *sum, nextAdd;
    int new, pageNum, tasksCompleted = 0, algorithm = 0;

    sum = (int *)malloc(sizeof(int*));
    *sum = 0;

    while(algorithm <= 2){
        while(tasksCompleted < maxTasks){
            while(num_added < k){
                nextAdd = num_added + variableWindowBase;
                if(nextAdd > k)
                    nextAdd = 0;
                if(LOGLEVEL > 1)
                    printf("Thread2: Fetching variable i = %d\n", nextAdd);
                new = fetch(nextAdd);
                if(new >= 0){
                    if(LOGLEVEL > 1)
                        printf("Thread2 sum = %d = %d + %d\n", *sum+new, *sum, new);
                    *sum += new;
                    num_added++;
                } else {
                    if(LOGLEVEL > 1)
                        printf("Thread2: Lookup failed, paging from disk[%d]\n", nextAdd);
                    switch(algorithm){
                        case 0: pageNum = page_in_random(nextAdd); break;
                        case 1: pageNum = page_in_MRU(nextAdd); break;
                        case 2: pageNum = page_in_LRU(nextAdd); break;
                    }
                }
                seq++;
                new = 0;
            }
            tasksCompleted++;
            if(LOGLEVEL >= 1)
                printf("2, %d, %d\n", tasksCompleted, seq);
            if(variableWindowBase == k)
                variableWindowBase = 0;
            else
                variableWindowBase++;
            num_added = *sum = 0;
            if(LOGLEVEL > 1)
                printf("Thread2 completed %d sum tasks.\n", tasksCompleted);
        }
        if(LOGLEVEL >= 1){
            switch(algorithm){
                case 0: printf("Thread 2 Tested random algorithm\n"); break;
                case 1: printf("Thread 2 Tested MRU algorithm\n"); break;
                case 2: printf("Thread 2 Tested LRU algorithm\n"); break;
            }
        }
        algorithm++;
        tasksCompleted = 0;
    }
    void *ret = (void*)*sum;
    return ret;
}

void *f_thread1(void *x){
    if(LOGLEVEL > 1)
        printf("In thread 1\n");
    int num_added = 0;
    int *sum, nextAdd = 0;
    int new, pageNum, tasksCompleted = 0, algorithm = 0;

    sum = (int *)malloc(sizeof(int*));
    *sum = 0;

    while(algorithm <= 2){
        while(tasksCompleted < maxTasks){
            while(num_added < k){
                if(num_added < k)
                    nextAdd++;
                if(num_added == k)
                    srand ( (unsigned)time ( NULL ) );
                    nextAdd = random() % n;
                if(LOGLEVEL > 1)
                    printf("Thread1: Fetching variable i = %d\n", nextAdd);
                new = fetch(nextAdd);
                if(new >= 0){
                    if(LOGLEVEL > 1)
                        printf("Thread1 sum = %d = %d + %d\n", *sum+new, *sum, new);
                    *sum += new;
                    num_added++;
                } else {
                    if(LOGLEVEL > 1)
                        printf("Thread1: Lookup failed, paging from disk[%d]\n", nextAdd);
                    switch(algorithm){
                        case 0: pageNum = page_in_random(nextAdd); break;
                        case 1: pageNum = page_in_MRU(nextAdd); break;
                        case 2: pageNum = page_in_LRU(nextAdd); break;
                    }
                }
                seq++;
                new = 0;
            }
            tasksCompleted++;
            if(LOGLEVEL >= 1)
                printf("1, %d, %d\n", tasksCompleted, seq);
            num_added = *sum = 0;
            if(LOGLEVEL > 1)
                printf("Thread1 completed %d sum tasks.\n", tasksCompleted);
        }
        if(LOGLEVEL >= 1){
            switch(algorithm){
                case 0: printf("Thread 1 Tested random algorithm\n"); break;
                case 1: printf("Thread 1 Tested MRU algorithm\n"); break;
                case 2: printf("Thread 1 Tested LRU algorithm\n"); break;
            }
        }
        algorithm++;
        tasksCompleted = 0;
    }
    void *ret = (void*)*sum;
    return ret;
}

int fetch(int i){
    if(i < 0 || i > n)
        return -2;
    // look up variable i in memory
    if(LOGLEVEL > 1)
        printf("Fetch: lookupTable[%d] = %d\n", i, lookupTable[i]);
    metaTable[i] = seq;
    if(lookupTable[i] != -1){
        if(LOGLEVEL > 1)
            printf("Retrieved value %d from physicalmemory[%d]\n", physicalMemory[lookupTable[i]], lookupTable[i]);
        return physicalMemory[lookupTable[i]];
    }
    return -1;
}

int page_in_random(int i){
    // page variable i from disk into a free memory location
    // if there is no free location, randomly choose a location to evict
    // and place variable i there
    int newPageNum, swappingMem, freeMem = -1, j;
    pthread_mutex_lock(&memoryLock);

    int requestedVar = disk[i];
    for(j = 0; j < m; j++){
        if(physicalMemory[j] <= 0){
            freeMem = j;
            physicalMemory[freeMem] = requestedVar;
            if(LOGLEVEL > 1)
                printf("Requested value %d and placed it in physicalMemory[%d]\n", requestedVar, freeMem);
            lookupTable[i] = freeMem;
            newPageNum = freeMem;
            break;
        }
    }
    // if no open memory cell was found
    if(freeMem == -1){
        // choose randomly
        swappingMem = random() % m;
        physicalMemory[swappingMem] = requestedVar;
        lookupTable[i] = swappingMem;
        if(LOGLEVEL > 1)
            printf("Requested value %d and swapped it to physicalMemory[%d]\nWrote value %d to lookupTable[%d]\n", requestedVar, swappingMem, swappingMem, i);
        newPageNum = swappingMem;
    }

    pthread_mutex_unlock(&memoryLock);
    return newPageNum;
}

int page_in_LRU(int i){
    // pages in variable i from disk to a free memory location
    // if there is no free memory, evicts the least recently used memory location
    // and replace with variable i
    int newPageNum, swappingMem, freeMem = -1, j;
    pthread_mutex_lock(&memoryLock);

    int requestedVar = disk[i];
    for(j = 0; j < m; j++){
        // this conditional indicates a free memory slot
        if(physicalMemory[j] <= 0){
            freeMem = j;
            physicalMemory[freeMem] = requestedVar;
            if(LOGLEVEL > 1)
                printf("Requested value %d and placed it in physicalMemory[%d]\n", requestedVar, freeMem);
            lookupTable[i] = freeMem;
            newPageNum = freeMem;
            // update meta table for this variable with the current sequential counter
            metaTable[freeMem] = seq;
            break;
        }
    }
    // if no open memory cell was found
    if(freeMem == -1){
        // figure out the least recently used variable using metaTable values
        int iterSeq = 99999;
        for(j = 0; j < m; j++){
            if(metaTable[j] < iterSeq){
                swappingMem = j;
                iterSeq = metaTable[j];
            }
        }
        if(LOGLEVEL > 1)
            printf("physicalMemory[%d] is least recently used, swapping it out\n", swappingMem);
        // perform the swap
        physicalMemory[swappingMem] = requestedVar;
        lookupTable[i] = swappingMem;
        if(LOGLEVEL > 1)
            printf("Requested value %d and swapped it to physicalMemory[%d]\nWrote value %d to lookupTable[%d]\n", requestedVar, swappingMem, swappingMem, i);
        newPageNum = swappingMem;
        metaTable[swappingMem] = seq;
    }
    pthread_mutex_unlock(&memoryLock);
    return newPageNum;
}

int page_in_MRU(int i){
    // pages in variable i from disk to a free memory location
    // if there is no free memory, evicts the most recently used memory location
    // and replace with variable i
    int newPageNum, swappingMem, freeMem = -1, j;
    pthread_mutex_lock(&memoryLock);

    int requestedVar = disk[i];
    for(j = 0; j < m; j++){
        // this conditional indicates a free memory slot
        if(physicalMemory[j] <= 0){
            freeMem = j;
            physicalMemory[freeMem] = requestedVar;
            if(LOGLEVEL > 1)
                printf("Requested value %d and placed it in physicalMemory[%d]\n", requestedVar, freeMem);
            lookupTable[i] = freeMem;
            newPageNum = freeMem;
            // update meta table for this variable with the current sequential counter
            metaTable[freeMem] = seq;
            break;
        }
    }
    // if no open memory cell was found
    if(freeMem == -1){
        // figure out the most recently used variable using metaTable values
        int iterSeq = 0;
        for(j = 0; j < m; j++){
            if(metaTable[j] > iterSeq){
                swappingMem = j;
                iterSeq = metaTable[j];
            }
        }
        if(LOGLEVEL > 1)
            printf("physicalMemory[%d] is most recently used, swapping it out\n", swappingMem);
        // perform the swap
        physicalMemory[swappingMem] = requestedVar;
        lookupTable[i] = swappingMem;
        if(LOGLEVEL > 1)
            printf("Requested value %d and swapped it to physicalMemory[%d]\nWrote value %d to lookupTable[%d]\n", requestedVar, swappingMem, swappingMem, i);
        newPageNum = swappingMem;
        metaTable[swappingMem] = seq;
    }
    pthread_mutex_unlock(&memoryLock);
    return newPageNum;
}

int printUsage(char *executable){
    printf("Usage: %s <disk size> <number of variables> <memory size> <number of tasks to run> <LOGLEVEL>\nLog levels:\n1=INFO\n2=DEBUG\n", executable);
    return 0;
}

int main(int argc, char *argv[]){
    if(argc != 6){
        printUsage(argv[0]);
        return 1;
    }

    int i = 0;
    n = atoi(argv[1]); // disk size
    k = atoi(argv[2]); // number of variables to sum
    m = atoi(argv[3]); // memory size
    maxTasks = atoi(argv[4]); // number of tasks to run
    LOGLEVEL = atoi(argv[5]); // output level - 1=INFO, 2=DEBUG

    if(n < m || n < k){
        printf("Disk must be larger than memory, and large enough for all variables!\n");
        printUsage(argv[0]);
        return 1;
    }
    if(LOGLEVEL > 2 || LOGLEVEL < 1){
        printf("Invalid log level");
        printUsage(argv[0]);
        return 1;
    }

    pthread_t thread1, thread2, thread3;
    int *thread1_ret, *thread2_ret, *thread3_ret;
    int t1, t2, t3;

    thread1_ret = (int *)malloc(sizeof(int *));
    thread2_ret = (int *)malloc(sizeof(int *));
    thread3_ret = (int *)malloc(sizeof(int *));

    physicalMemory = (int *)malloc(m*sizeof(int));
    lookupTable = (int *)malloc(n*sizeof(int));
    metaTable = (int *)malloc(m*sizeof(int));
    disk = (int *)malloc(n*sizeof(int));

    srand ( (unsigned)time ( NULL ) );
    // initialize shared buffers with sentinel values
    for(i = 0; i <= n; i++){
        // disk[i] = rand() % 100;
        disk[i] = i;
        lookupTable[i] = -1;
        if(LOGLEVEL > 1)
            printf("disk[%d]: %d\n", i, disk[i]);
    }
    for(i = 0; i <= m; i++){
        physicalMemory[i] = -1;
        metaTable[i] = -1;
    }

    // run the threads
    t1 = pthread_create(&thread1, NULL, f_thread1, (void*)10);
    t2 = pthread_create(&thread2, NULL, f_thread2, (void*)0);
    t3 = pthread_create(&thread3, NULL, f_thread3, (void*)0);

    if(LOGLEVEL > 1)
        printf("Thread 1 create returned %d\nThread 2 create returned %d\nThread 3 create returned %d\n", t1, t2, t3);
    else if(LOGLEVEL >= 1) {
        printf("Memory paging simulation\nEmmett Butler\n");
        printf("CONFIG:\ndisk size=%d\nnumber of variables=%d\nmemory size=%d\ntasks to run=%d\nLOGLEVEL=%d\n", n, k, m, maxTasks, LOGLEVEL);
        printf("Running trials...\n");
    }

    pthread_join(thread1, (void *)thread1_ret);
    pthread_join(thread2, (void *)thread2_ret);
    pthread_join(thread3, (void *)thread3_ret);

    if(LOGLEVEL > 1){
        printf("Starting exit sequence\n");
        printf("Thread 1 completed with return value %d\n", *thread1_ret);
        printf("Thread 2 completed with return value %d\n", *thread2_ret);
        printf("Thread 3 completed with return value %d\n", *thread3_ret);
    } else {
        printf("Completed successfully.\nExiting...\n");
    }

    return 0;
}
