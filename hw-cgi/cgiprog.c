/*
1) jfran33 2454224 2438086    1 2454224 echoserveri
2) only 1 and only one thread, looking at the code for echoserveri I believe this is because it is not set up to run multiple treads or process yet
3) the second window recieved it's message back and the server window show a new connection and more bytes recieved, the thrid window remains unchanged
4.
jfran33 2459429 2458732    1 2459429 S echoserverp
jfran33 2459533 2459429    1 2459533 S echoserverp
jfran33 2461259 2459429    1 2461259 S echoserverp
jfran33 2461697 2459429    1 2461697 S echoserverp

5. 4 processes and 4 threads with 3 of the threads sharing a pgid. This is because for each incoming connection the parent forks a new child process to work with that connection.

6. 
jfran33 2464368 2458732    4 2464368 S echoservert
jfran33 2464368 2458732    4 2464508 S echoservert
jfran33 2464368 2458732    4 2464706 S echoservert
jfran33 2464368 2458732    4 2464896 S echoservert

7. 1 process and 4 threads. This is because the program allocates memory for the thread and then creates the thread from the same original process.

8. 
jfran33 2466539 24587329 2466539 S echoservert_pre
jfran33 2466539 24587329 2468924 S echoservert_pre
jfran33 2466539 24587329 2468925 S echoservert_pre
jfran33 2466539 24587329 2468926 S echoservert_pre
jfran33 2466539 24587329 2468927 S echoservert_pre
jfran33 2466539 24587329 2468929 S echoservert_pre
jfran33 2466539 24587329 2468930 S echoservert_pre
jfran33 2466539 24587329 2468931 S echoservert_pre
jfran33 2466539 24587329 2468932 S echoservert_pre

9) With this server there is a total of 9 threads, I think this is because the code creates a fixed number of new worker threads which is 8 and then sends the connection to one of these threads
10) I see 8 before wait (items) so I believe there are 0 producer threads
11) there are 8 for reasons stated previously
12) while there are no print statments, I assume they are waiting on a connection
13) they are waiting on items
14) the connection, since there are open slots it is not waiting on those
15) a producer posting an item
16) one
17) a producer posting an item
18) there are no statements for waiting on a slot so I assume another connection or something to put into a slot.
19) echoserverp, echoservert, echoservert_pre
20. echoservert_pre
21. t_pre and t
22. p seems to be the simplest because you just call fork and then call echo.

*/


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int main (void) {
    int MAX = 500;
    char* query;
    char content[MAX];
    
    query = getenv("QUERY_STRING");
    
    sprintf(content, "The query string is: %s", query);
    printf("Content-type: text/plain\r\n");
    printf("Content-length: %d\r\n\r\n", (int)strlen(content));

    printf("%s", content);
    fflush(stdout);

    return 0;
}