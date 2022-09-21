#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
	int pid;

	printf("Starting program; process has pid %d\n", getpid());
	
	FILE *fp;
	fp = fopen ("fork-output.txt", "w+");
	fprintf(fp,"BEFORE FORK\n");
	fflush(fp); 
	// fclose (fp);

	// int fd1[2];
	// pipe(fd1);

	if ((pid = fork()) < 0) {
		fprintf(stderr, "Could not fork()");
		exit(1);
	}

	/* BEGIN SECTION A */

	printf("Section A;  pid %d\n", getpid());
	// sleep(30);
	fprintf(fp,"SECTION A\n");
	fflush(fp); 
	/* END SECTION A */
	if (pid == 0) {
		/* BEGIN SECTION B */

		printf("Section B\n");
		fprintf(fp,"SECTION B\n");
		fflush(fp); 
		int fd1 = fileno(fp);
		
		

		
		char *newenviron[] = { NULL };

		printf("Program \"%s\" has pid %d. Sleeping.\n", argv[0], getpid());
		// sleep(30);

		if (argc <= 1) {
			printf("No program to exec.  Exiting...\n");
			exit(0);
		}

		printf("Running exec of \"%s\"\n", argv[1]);
		// dup2(fd1, STDOUT_FILENO);
		execve(argv[1], &argv[1], newenviron);
		printf("End of program \"%s\".\n", argv[0]);

		
		// close(fd1[0]);
		// write(fd1[1],"hello from Section B\n", 22);
		// sleep(30);
		// sleep(30);
		// printf("Section B done sleeping\n");

		exit(0);

		/* END SECTION B */
	} else {
		/* BEGIN SECTION C */
		
		printf("Section C\n");




		fprintf(fp,"SECTION C\n");
		fflush(fp); 

		// char buff [50];
		// close(fd1[1]);
		// int s = read(fd1[0], buff, 50);
		// buff[s]='\0';
		// printf("%s",buff);

		// sleep(30);
		// printf("Section C done sleeping\n");

		exit(0);

		/* END SECTION C */
	}
	/* BEGIN SECTION D */

	printf("Section D\n");
	// sleep(30);
	fprintf(fp,"SECTION D\n");
	fflush(fp); 
	/* END SECTION D */
}

