//gcc -o DogLang main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DEBUG
/**
 * @brief 프로그램 안에서 사용할 정수형 변수의 최대 개수
 */
#define TOTAL_VARIABLE_SIZE 100

/**
 * @brief REPL 모드에서 입력되는 명령어 문자열 크기
 */
#define REPL_CMD_LENGTH 0x1024

/**
 * @brief REPL 모드에서 보이는 prompt 문자열 크기
 */
#define REPL_PROMPT_LENGTH 100

/**
 * @brief 실행할 때 REPL 모드로 할지 스크립트 실행 모드로 할지 결정
 */
#define RUN_NOTHING 0
#define RUN_REPL 1
#define RUN_SCRIPT 2
#define RUN_EMBED_C_SRC 3

/**
 * @brief REPL 모드에서 보이는 PROMPT 문자열
 */
char REPL_PROMPT[REPL_PROMPT_LENGTH] = " > ";

/**
 * @brief 스크립트 실행 파일 위치
 */
char SCRIPT_PATH[1024] = "";

int if_checker = 0;

void print_dog_ascii_art();
int repl(unsigned char *VARS);
void arg_help();
char arg_parser(int argc, char** argv);
void error(char *error_msg, int line, char *error_function);
void run_script(unsigned char *VARS);
int white_space(char *s);
void m_int_parser(unsigned char *VARS, char *s, int line, char *function_name, unsigned char left);
void p_int_parser(unsigned char *VARS, char *s, int line, char *function_name, unsigned char left);
int get_parser(char *s, int line, char *function_name);
int compare_parser(unsigned char *VARS, char *s, int line, char *function_name, unsigned char left);
void parser(unsigned char *VARS, char *s, int line, char *function_name);

int main(int argc, char** argv) {
  
  unsigned char VARS[1024];

  switch (arg_parser(argc, argv)) {
  case RUN_NOTHING:
    printf("아무것도 실행되지 않았습니다.\n");
    break;

  case RUN_REPL:
    repl(VARS);
    break;

  case RUN_SCRIPT:
    run_script(VARS);
    break;


  case RUN_EMBED_C_SRC:
    printf("아직 개발중인 기능입니다.");
    break;
  }

  return 0;
}

/**
 * @brief 강아지 아스키 아트 출력
 */
void print_dog_ascii_art() {
  printf("\n    /\\\n");
  printf("   /  \\_____\n");
  printf("  /    \\   *|_____\n");
  printf("  \\        _______|\n");
  printf("  /        \\_____    개소리 (DogSoundLang)\n");
  printf("  /       ______/    version 0.1\n");
  printf("  /______/\n\n");

}

/**
 * @brief REPL 모드
 * @return program exit code (0: 정상 종료)
 */
int repl(unsigned char *VARS) {
  print_dog_ascii_art();

  while (1) {
    // Prompt
    printf("%s", REPL_PROMPT);

    char *cmd = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&cmd, &len, stdin);

    if (cmd[0] == '\n') {
      free(cmd);
      continue;
    }
    
    cmd[strlen(cmd) - 1] = '\0';
    
    if (strcmp(cmd, "그만해") == 0) {
      free(cmd);
      printf("뫙뫙\n");
      break;
    }

    // 명령어 실행
    parser(VARS, cmd, 1, "Main");

    free(cmd);
    
    printf("\n");
  }

  return 0;
}

void error(char *error_msg, int line, char *error_function)
{
    printf("야 개짖는 소리좀 안나게해라\n");
    printf("%s, %d Line From %s\n", error_msg, line, error_function);
    exit(-1);
}

void arg_help() {
  printf(" -h               \t\t 사용법 출력\n");
  printf(" -v               \t\t 버전 출력\n");
  printf(" -s <SCRIPT_PATH> \t\t 스크립트 파일 위치\n");
  printf(" -E               \t\t C언어로 실행가능한 소스코드 출력");
}

char arg_parser(int argc, char** argv) {
  int running_mode = RUN_REPL;

  for (int arg = 0; arg < argc; arg++) {
    if (strcmp(argv[arg], "-h") == 0) {
      arg_help();
      running_mode = RUN_NOTHING;

    } else if (strcmp(argv[arg], "-v") == 0) {
      printf("개소리 (DogSoundLang)\n");
      printf("version 0.1\n");
      running_mode = RUN_NOTHING;

    } else if (strcmp(argv[arg], "-s") == 0) {
      strcpy(SCRIPT_PATH, argv[++arg]);
      running_mode = RUN_SCRIPT;
    } else if (strcmp(argv[arg], "-E") == 0) {
      running_mode = RUN_EMBED_C_SRC;
    }
  }

  return running_mode;
}

void run_script(unsigned char *VARS) {
  char buffer[REPL_CMD_LENGTH];
  char ch;
  FILE *fp;
  size_t line = 0;
  size_t length = 0;

  fp = fopen(SCRIPT_PATH, "r");

  if (fp == NULL) {
    printf("[run_script][%s] 스크립트 파일을 열 수 없습니다.\n", SCRIPT_PATH);
    exit(1);
  }

  while(1) {
    line++;
    length = 0;

    while (1) {
      ch = (char)fgetc(fp);
      if (ch == EOF || ch == '\n') break;
      buffer[length++] = ch;
    }

    if (ch == EOF) break;
    if (length == 0) continue;

    buffer[length] = '\0';
    parser(VARS, buffer, line, "Main");
  }

  fclose(fp);
}

int white_space(char *s)
{
  int index = 0;
  while (s[index] == ' ')
  {
    index++;
  }
  return index;
}

void m_int_parser(unsigned char *VARS, char *s, int line, char *function_name, unsigned char left){
  int index = 0;
  int value = 0;
  int flag = 0;
  while (strncmp(s, "망", 3) == 0) {
    index++;
    s += 3; //한글 1글자는 3Byte
  }
  while (s[0] == '!')
  {
    value++;
    s++;
  }
  if (s[0] == '?')
  {
    s++;
    flag = 1;
  }
  if(s[0] != NULL)
  {
    error("SyntaxError: invalid syntax", line, function_name);
  }
  if (flag){
    VARS[index] = VARS[index]-value-VARS[left];
  }
  else {
    VARS[index] = -value;
  }
  #ifndef DEBUG
    printf("[m_int_parser][망%d]VARS[%d] = %d\n", flag, index, VARS[index]);
  #endif
}

/**
 * @brief int형 변수를 선언하는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void
 */
void p_int_parser(unsigned char *VARS, char *s, int line, char *function_name, unsigned char left){
  int index = 0;
  int value = 0;
  int flag = 0;
  while (strncmp(s, "멍", 3) == 0) {
    index++;
    s += 3; //한글 1글자는 3Byte
  }
  while (s[0] == '!')
  {
    value++;
    s++;
  }
  if (s[0] == '?')
  {
    s++;
    flag = 1;
  }
  if(s[0] != NULL)
  {
    error("SyntaxError: invalid syntax", line, function_name);
  }
  if (flag){
    VARS[index] = VARS[index]+value+VARS[left];
  }
  else {
    VARS[index] = value;
  }
  #ifndef DEBUG
    printf("[p_int_parser][멍%d]VARS[%d] = %d\n", flag, index, VARS[index]);
  #endif
}

/**
 * @brief int형 변수의 문자열 인덱스를 가져오는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void
 */
int get_parser(char *s, int line, char *function_name){
  int index = 0;

  s += 3;

  while (s[0] == '!')
  {
    index++;
    s++;
  }

  #ifndef DEBUG
    printf("[get_parser][손]VARS[%d] = %d\n", index, VARS[index]);
  #endif
  if (s[0] == ' ' || s[0] == NULL)
    return index;
  
  error("SyntaxError: invalid syntax", line, function_name);

}

int compare_parser(unsigned char *VARS, char *s, int line, char *function_name, unsigned char left) {
  int right = 0, flag;
  s += 30;
  s += white_space(s);
  
  if(strncmp(s, "엄마?", 7) == 0) {
    flag = 0;
  }
  else if(strncmp(s, "아빠?", 7) == 0) {
    flag = 1;
  }
  else if(strncmp(s, "둘다?", 7) == 0) {
    flag = 2;
  }
  else {
    error("SyntaxError: for invalid condition", line, function_name);
  }
  s += 7;
  s += white_space(s);

  if(strncmp(s, "손", 3) != 0)
    error("SyntaxError: for invalid syntax", line, function_name);
  s+= 3;
  while(s[0] == '!')
  {
    s++;
    right++;
  }
  #ifndef DEBUG
    printf("left : %d right : %d flag : %d\n", left, right, flag);
  #endif
  if (flag == 0) {
    return VARS[left] > VARS[right];
  }
  else if(flag == 1){
    return VARS[left] < VARS[right];
  }else if (flag == 2){
    return VARS[left] == VARS[right];
  }
}

/**
 * @brief int형 변수를 선언하는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @param[in] value 출력할 값
 * @param[in] format_string 포맷스트링
 * @return void
 */
void print_parser(char *s, int line, unsigned char value, char *format_string, char *function_name){

  int index = 0;

  s += 3*4; //한글 1글자는 3Byte
  if(s[0] != NULL)
  {
    error("SyntaxError: invalid syntax", line, function_name);
  }
  printf(format_string, value);
}

/**
 * @brief 문자열과 라인 번호를 받아 해당 라인에 맞는 행위 실행
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void 
 */
void parser(unsigned char *VARS, char *s, int line, char *function_name) {
  int i=0;
  unsigned char first_parameter = 0;

  s += white_space(s);

  if(if_checker){
    if (strncmp(s, "산책가자", 12) == 0)
    {
      if_checker = 0;
      s+= 3*4;
      return ;
    }
    if(if_checker == 2)
      return ;
  }
  
  
  if (strncmp(s, "손", 3) == 0) {
      i = get_parser(s, line, function_name); //index
      s += i + 3; //3을 더해주는 이유는 손[3Byte] + !개수
      s += white_space(s);
      first_parameter = VARS[i];
  }
  
  if (strncmp(s, "멍", 3) == 0) {
      p_int_parser(VARS, s, line, function_name, first_parameter);
  }else if(strncmp(s, "망", 3) == 0) {
      m_int_parser(VARS, s, line, function_name, first_parameter);
  }
  else if (strncmp(s, "크게짖어", 12) == 0) {
      print_parser(s, line, first_parameter, "%c", function_name);
  }
  else if (strncmp(s, "작게짖어", 12) == 0) {
      print_parser(s, line, first_parameter, "%d", function_name);
  }
  else if (strncmp(s, "엄마가좋아아빠가좋아", 30) == 0) {
    if_checker = 1 + compare_parser(VARS, s, line, function_name, first_parameter);
  }
  else {
      error("SyntaxError: invalid syntax", line, function_name);
  }


}