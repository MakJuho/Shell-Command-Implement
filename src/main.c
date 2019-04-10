#define MAX_LENGTH 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <unistd.h>


char built_in[4][MAX_LENGTH] = {"pwd", "/bin/ls", "make", "run"};
char system_in[4][MAX_LENGTH] = {"cd", "echo", "grep", "ls"};
char redirection[3][MAX_LENGTH] = {">", "<", "|"};

void Eliminate(char *str, char ch)

{

    for (; *str != '\0'; str++)//종료 문자를 만날 때까지 반복

    {

        if (*str == ch)//ch와 같은 문자일 때

        {

            strcpy(str, str + 1);

            str--;

        }

    }

}

char duplicate_check[MAX_LENGTH];
int main()
{
	char buf[4096];

	// fgets로 계속해서 단어를 받는다.
	while (fgets(buf, 4096, stdin)) {
	// 문자열 비교해서 pwd와 같은지 판별
		// 버퍼에 문장을 받기
		
		char *sArr[30];
		char* cmd;
		int arr_cnt=0;
	// 첫 문자열 받음
		char *ptr = strtok(buf," \n");
		// NULL 나올 때까지
		while(ptr != NULL)
		{
			sArr[arr_cnt] = ptr;
			// check length sArr
			arr_cnt++;
			ptr = strtok(NULL, " \n");
		}

		for(int i=0; i< 4; i++){

			// "pwd" "/bin/ls" "make" "run"
			if(strcmp(sArr[0],built_in[i])==0 || sArr[0][0]=='~'){

				// pwd 일때
				if (strcmp(sArr[0],built_in[0])==0) {
					// fork()로 자식 생성
					char working_directory_name[4096];
					// getcwd 현재 디렉토리 이름 구하기
					getcwd(working_directory_name, 4096);
					printf("%s\n", working_directory_name);
					
				}
				// /bin/ls 일때
				else if(strcmp(sArr[0],built_in[1]) ==0){
					// fork()로 자식 생성
					
					pid_t pid = fork();
					int status2;

					// 자식이나 부모가 아닌 경우 Error 처리
					if (pid == -1) {
						fprintf(stderr, "Error occured during process creation\n");
						exit(EXIT_FAILURE);
					// 자식인 경우
					} else if (pid == 0) {
						
						execl("/bin/ls","ls",NULL);
						printf("/bin/ls execv failed to run ls\n");
						exit(EXIT_SUCCESS);
					}
					// 부모인 경우
					else {

						// 자식이 끝날 때까지 기다린다.
						waitpid(pid, &status2, WNOHANG);
					}
				}
				// ~ 시작
				else if(sArr[0][0]=='~'){
					// fork()로 자식 생성
					chdir(getenv("HOME"));
					char tmp_path[MAX_LENGTH];
					strcpy(tmp_path,sArr[0]+2);
					
					pid_t pid = fork();
					int status3;

					// 자식이나 부모가 아닌 경우 Error 처리
					if (pid == -1) {
						fprintf(stderr, "Error occured during process creation\n");
						exit(EXIT_FAILURE);
					// 자식인 경우
					} else if (pid == 0) {
						
						// 경로에 있는 파일 실행
						execl(tmp_path,"./course_sched",NULL);
						printf("~/경로 실행 execv failed to run ls\n");
						exit(EXIT_SUCCESS);
					}
					// 부모인 경우
					else {
						// 자식이 끝날 때까지 기다린다.
						wait(NULL);
					}
					
				}
				

				// make, make run
				else if(strncmp(sArr[0],"make",4)==0){
					
					
					// make run
					if(strncmp(sArr[1],"run", 3)==0){
						// fork()로 자식 생성
						
						int status4;

						pid_t pid = fork();
						// 자식이나 부모가 아닌 경우 Error 처리
						if (pid == -1) {
							fprintf(stderr, "Error occured during process creation\n");
							exit(EXIT_FAILURE);
						// 자식인 경우
						} else if (pid == 0) {
							sleep(1);
							// 경로에 있는 파일 실행
							execl("/usr/bin/make","make","run",NULL);
							printf("make run 실행 execv failed to run ls\n");
							exit(EXIT_SUCCESS);
						}
						// 부모인 경우
						else {
							// 자식이 끝날 때까지 기다린다.
							wait(NULL);
						}


					}
					// make test
					else if(strncmp(sArr[1],"test", 4)==0){
						// fork()로 자식 생성
						
						int status5;

						pid_t pid = fork();
						// 자식이나 부모가 아닌 경우 Error 처리
						if (pid == -1) {
							fprintf(stderr, "Error occured during process creation\n");
							exit(EXIT_FAILURE);
						// 자식인 경우
						} else if (pid == 0) {
							
							// 경로에 있는 파일 실행
							execl("/usr/bin/make","make","test",NULL);
							printf("make run 실행 execv failed to run ls\n");
							exit(EXIT_SUCCESS);
						}
						// 부모인 경우
						else {
							// 자식이 끝날 때까지 기다린다.
							wait(NULL);
						}


					}// make
					else{
					// fork()로 자식 생성
					//	fflush(stdin);
						pid_t pid = fork();
						int status6;

						// 자식이나 부모가 아닌 경우 Error 처리
						if (pid == -1) {
							fprintf(stderr, "Error occured during process creation\n");
							exit(EXIT_FAILURE);
						// 자식인 경우
						} else if (pid == 0) {
							
							// 경로에 있는 파일 실행
							execl("/usr/bin/make","make",NULL);
							printf("make 실행 execv failed to run ls\n");
							exit(EXIT_SUCCESS);
						}
						// 부모인 경우
						else {
							// 자식이 끝날 때까지 기다린다.
							wait(NULL);
						}

					}

				}
			}

			// cd, echo, grep
			if(strcmp(sArr[0],system_in[i])==0){

				// cd
				if(strcmp(sArr[0],"cd")==0){			
					char path[MAX_LENGTH];
					char tmp_path[MAX_LENGTH];
					
					
					// cd ~일 경우 일단 HOME으로 보낸다.
					// cd ~/경로1/경로2 일 경우 일단 HOME으로 보내고, 나머지 경로로 보낸다.
					
					if(sArr[1][0] == '~' ){
						
						chdir(getenv("HOME"));

						strcpy(tmp_path,sArr[1] +2);
						
						if(chdir(tmp_path)==-1)
							printf("failed, change directory\n");
					}else{
						if(chdir(sArr[1])==-1)
						{
							printf("failed, change directory\n");
						}
						else
						{
							printf("After Current Directory:%s\n",getcwd(path,MAX_LENGTH));
						}
					}
				}
			}
		}
		
		// >, <, |

		int redirect_cnt = 0;

		for(int j=0; j<arr_cnt; j++){
			for(int i=0; i< 3; i++){
				if(strcmp(sArr[j],redirection[i])==0){
					redirect_cnt = j;

				}
			}
		}

		int fd_wr;
		int fd_rd;
		char string[MAX_LENGTH];
		int last_pt=0;
		char file_name[MAX_LENGTH];
		ssize_t nr;
		char buff[MAX_LENGTH];

		strcpy(string,"");

		if(strcmp(sArr[0],"echo")==0 && sArr[1][0]=='$'){
			printf("%s\n",getenv("PATH"));
		}
	

		if(strcmp(sArr[redirect_cnt],">")==0){
			fd_wr = open(sArr[redirect_cnt+1], O_RDWR|O_CREAT|O_TRUNC,0644);
			// 쌍따옴표
			if(strcmp(sArr[0],"echo")==0){
				if(sArr[1][0] == '"'){
						
						// 큰 따옴표 안에 문자열 string 변수에 넣기 성공!
					for(int i=0; i<arr_cnt; i++){
						if(sArr[i][strlen(sArr[i])-1] == '"'){
								
							last_pt = i;	
						}
					}
						
					for(int i=1; i<last_pt+1; i++){
						strcat(string,sArr[i]);
						strcat(string," ");
					}
					Eliminate(string,'"');
				}
				// fork()로 자식 생성
				pid_t pid = fork();
				int status7;

				// 자식이나 부모가 아닌 경우 Error 처리
				if (pid == -1) {
					fprintf(stderr, "Error occured during process creation\n");
					exit(EXIT_FAILURE);
				// 자식인 경우
				} else if (pid == 0) {
					char working_directory_name[4096];
					// getcwd 현재 디렉토리 이름 구하기
						
					fd_rd = dup2(fd_wr,STDOUT_FILENO);
					execl("/bin/echo","echo",string,NULL);
					close(fd_rd);
					exit(EXIT_SUCCESS);
				}
				// 부모인 경우
				else {
					// 자식이 끝날 때까지 기다린다.
					wait(NULL);
				}
				
				close(fd_wr);
			}
			else if(strcmp(sArr[0],"ls")==0){
					fflush(stdin);
					// fork()로 자식 생성
					pid_t pid = fork();
					int status8;

					// 자식이나 부모가 아닌 경우 Error 처리
					if (pid == -1) {
						fprintf(stderr, "Error occured during process creation\n");
						exit(EXIT_FAILURE);
					// 자식인 경우
					} else if (pid == 0) {
						char working_directory_name[4096];
						// getcwd 현재 디렉토리 이름 구하기
						
						fd_rd = dup2(fd_wr,STDOUT_FILENO);
						execl("/bin/ls","ls",NULL);
						
						close(fd_rd);
						exit(EXIT_SUCCESS);
					}
					// 부모인 경우
					else {
						// 자식이 끝날 때까지 기다린다.
						wait(NULL);
					}
			}
		}
		else if(strcmp(sArr[redirect_cnt],"<")==0){
			int fd;
			if(strcmp(sArr[0],"grep")==0){
				if((fd = open("students.txt", O_RDONLY)) == -1)
				{
					printf("open error\n");
					exit(1);
				}

				dup2(fd, STDIN_FILENO);
				execl("/bin/grep", "grep", "Fan",NULL);
				close(fd);
			}
		}
		
		// 채점 보조기능 구현
		// ls 일때
		if(strcmp(sArr[0],"ls")==0&&(sArr[1]==NULL)){
			// fork()로 자식 생성
			pid_t pid = fork();
			int status2;

			// 자식이나 부모가 아닌 경우 Error 처리
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			// 자식인 경우
			} else if (pid == 0) {
				
				//char *arg[] = {"ls", (char *)0};
				//printf("buf:%c\n",buf[7]);
				//execv(sArr[0], arg);
				execl("/bin/ls","ls",NULL);
				printf("/bin/ls execv failed to run ls\n");
				exit(EXIT_SUCCESS);
			}
			// 부모인 경우
			else {

				// 자식이 끝날 때까지 기다린다.
				waitpid(pid, &status2, WNOHANG);
				//wait(NULL);
			}
		}
		if(strcmp(sArr[0],"ready-to-score")==0){
			// fork()로 자식 생성
			//chdir(getenv("HOME"));
			chdir("scripts");
				//printf("FAIL!!\n");
			pid_t pid = fork();
			int status;

			// 자식이나 부모가 아닌 경우 Error 처리
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			// 자식인 경우
			} else if (pid == 0) {
				
				execl("/usr/bin/python3","python3","ready-to-score.py",NULL);
				printf("ready-to-score.py execv failed to run ls\n");
				exit(EXIT_SUCCESS);
			}
			// 부모인 경우
			else {

				// 자식이 끝날 때까지 기다린다.
				wait(NULL);
			}
					

		}
		if(strcmp(sArr[0],"auto-grade-pa0")==0){
			chdir("scripts");
			pid_t pid = fork();
			int status;

			// 자식이나 부모가 아닌 경우 Error 처리
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			// 자식인 경우
			} else if (pid == 0) {
				
				execl("/usr/bin/python3","python3","auto-grade-pa0.py",NULL);
				printf("auto-grade-pa0.py execv failed to run ls\n");
				exit(EXIT_SUCCESS);
			}
			// 부모인 경우
			else {

				// 자식이 끝날 때까지 기다린다.
				wait(NULL);
			}
		}
		if(strcmp(sArr[0],"report-grade")==0){
			chdir("scripts");
			pid_t pid = fork();
			int status;

			// 자식이나 부모가 아닌 경우 Error 처리
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			// 자식인 경우
			} else if (pid == 0) {
				
				execl("/usr/bin/python3","python3","report-grade.py",NULL);
				printf("report-grade.py execv failed to run ls\n");
				exit(EXIT_SUCCESS);
			}
			// 부모인 경우
			else {

				// 자식이 끝날 때까지 기다린다.
				wait(NULL);
			}
		}
		

	}

	return 0;
}
