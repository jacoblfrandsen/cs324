// 1. What are the numbers associated with the manual sections for executable
//     programs, system calls, and library calls, respectively?

//        1   Executable programs or shell commands
//        2   System calls (functions provided by the kernel)
//        3   Library calls (functions within program libraries)
//        4   Special files (usually found in /dev)
//        5   File formats and conventions, e.g. /etc/passwd
//        6   Games
//        7   Miscellaneous (including macro packages and conventions), e.g. man(7), groff(7)
//        8   System administration commands (usually only for root)
//        9   Kernel routines [Non standard]
    
//  2. Which section number(s) contain a man page for `kill`?
//    section 1,2
//  3. Which section number(s) contain a man page for `exit`?
//    section 3,2
//  4. Which section number(s) contain a man page for `open`?
//    section 2
//  5. What three `#include` lines should be included to use the `open()` system
//     call?
//    #include <sys/types.h>
//    #include <sys/stat.h>
//    #include <fcntl.h>
//  6. Which section number(s) contain a man page for `socket`?
//    section 7,2
//  7. Which `socket` option "Returns a value indicating whether or not this
//     socket has been marked to accept connections with `listen(2)`"?
//     SO_ACCEPTCONN
//  8. How many man pages (in any section) contain keyword references to
//     `getaddrinfo`?
//     3 pages
//  9. According to the "DESCRIPTION" section of the man page for `string`, the
//     functions described in that man page are used to perform operations on
//     strings that are ________________. (fill in the blank)
//     null-terminated
//  10. What is the return value of `strcmp()` if the value of its first argument
//      (i.e., `s1`) is _greater than_ the value of its second argument (i.e.,
//      `s2`)? 
//      it returns greater than 0


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
   FILE *fp;
   char str[1024];
    

   fp = fopen ("lorem-ipsum.txt","r");
   
   fprintf(stderr, "%d", getpid());
   
   
   while (fgets(str, 1024, fp)) {
      /* writing content to stdout */
      char *ret;
      char *pattern = getenv("CATMATCH_PATTERN");
      if(pattern){

         ret = strstr(str, pattern);
         if(ret){
            printf("1 %s", str);
         }
         else{
            printf("0 %s", str);
         }
      }
      else{
         printf("0 %s", str);
      }

      
   }

   fclose(fp);

   // I did TMUX
   



}