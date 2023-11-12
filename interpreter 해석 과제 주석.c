/* Basic Interpreter by H�seyin Uslu raistlinthewiz@hotmail.com */
/* Code licenced under GPL */

//라이브러리 헤더파일 import

#include <stdio.h> // 입출력 함수 제공
#include <conio.h> // 화면 출력 및 조작 함수 제공
#include <string.h> // 문자열 비교, 복사 및 조작 함수 제공
#include <stdlib.h> // 메모리 할당 및 관리 함수 제공
#include <ctype.h> // 문자 검사 및 조작 함수 제공

struct node{ //node 구조체를 정의한다
	int type; // int형태로 노드의 type 저장 (변수= 1, 함수= 2, 함수 호출= 3, 시작= 4, 끝= 5)
/* system stack-> 1 for variable, 2 for function , 3 for function call
4 for begin 5 for end */

	char exp_data; // char형태로 변수,함수의 이름 등을 저장
	int val; // int형태로 값 또는 데이터를 저장
	int line; // int 형태로 node의 line 저장
	struct node * next; // 포인터 형식으로 연결리스트에서 다음 node 구조체를 포인팅

};

typedef struct node Node; //struct node로 새로운 자료형 Node 생성

struct stack{ //stack 구조체를 정의한다.
	Node * top; // stack에서의 top을 포인팅
};

typedef  struct stack Stack; //struct stack으로 새로운 자료형 Stack 생성


struct opnode{ // 연산자 노드 구조체를 정의한다.
	char op; // char형태로 연산자를 저장
	struct opnode * next; // 다음 opnode 구조체를 포인팅
};
typedef struct opnode opNode; //struct opnode로 새로운 자료형 opNode 생성

struct opstack{ // 연산자 스택 구조체를 정의한다.
	opNode * top; //opstack에서의 top을 포인팅
};

typedef struct opstack OpStack; //struct opstack으로 새로운 자료형 opStack 생성

struct postfixnode{ //후위표기 노드 구조체를 정의한다.
	int val; // int형태로 연산할 정수값 저장
	struct postfixnode * next; // 다음 postfixnode 구조체를 포인팅
};

typedef struct postfixnode Postfixnode; //struct postfixnode로 새로운 자료형 Postfixnode 생성

struct postfixstack{ // 후위표기 스택 구조체를 정의한다.
	Postfixnode * top; // postfixstack에서의 top을 포인팅
	};

typedef struct postfixstack PostfixStack; //struct postfixstack으로 새로운 자료형 Postfixstack 생성





int GetVal(char,int *,Stack *); // 매개변수를 이용해 char, int*, stack *를 검색
int GetLastFunctionCall(Stack*); // 스택에서 마지막으로 호출한 함수를 검색하고 해당 함수 호출이 발생한 줄 번호를 반환

Stack* FreeAll(Stack *); //FreeAll 함수를 통해 할당한 메모리를 해제

Stack * Push(Node sNode,Stack *stck) //stck 스택에 sNode를 push하는 함수 정의
{
Node *newnode; //새로운 노드 포인터 변수 newnode 생성

if ((newnode=(Node*)malloc(sizeof(Node)))==NULL) { // malloc함수를 통해 newnode에 메모리 할당에 실패했다면
printf("ERROR, Couldn't allocate memory..."); //오류 메시지 출력
return NULL; // 오류가 일어나면 NULL값 반환
}
else //메모리 할당에 성공했다면
{
newnode->type=sNode.type; // Snode의 type을 newnode의 type에 할당
newnode->val=sNode.val; // Snode의 val을 newnode의 val에 할당
newnode->exp_data=sNode.exp_data; // Snode의 exp_data을 newnode의 exp_data에 할당
newnode->line=sNode.line; // Snode의 line을 newnode의 line에 할당
newnode->next=stck->top; // newnode의 next를 현재 스택의 top으로 할당
stck->top=newnode; //스택의 top을 newnode로 설정
return stck; // 스택을 반환
}
}

OpStack * PushOp(char op,OpStack *opstck)//opstck 스택에 op를 push하는 함수 정의
{
opNode *newnode; //새로운 opnode 포인터 변수 newnode 생성
if ((newnode=(opNode*)malloc(sizeof(opNode)))==NULL) { // malloc함수를 통해 newnode에 메모리 할당에 실패했다면
printf("ERROR, Couldn't allocate memory..."); //오류 메시지 출력
return NULL; // 오류가 일어나면 NULL값 반환
}
else //메모리 할당에 성공했다면
{
newnode->op=op; //op값을 newnode의 op에 할당
newnode->next=opstck->top; //newnode의 next를 현재 스택의 top으로 할당
opstck->top=newnode; // 스택의 top을 newnode로 설정
return opstck; // 스택을 반환
}
}

char PopOp(OpStack *opstck) //op 스택에서 op를 pop하는 함수 정의
{
opNode *temp; // 새로운 opnode 포인터 변수 temp 생성
char op; // char 형태의 op 변수 선언
if (opstck->top == NULL ) // 스택의 top이 NULL값이면
{
printf("ERROR, empty stack..."); // 스택이 비어있다는 오류 메시지 출력
}
else // 스택의 top이 NULL이 아니라면
{
op=opstck->top->op; // 스택의 top의 op 값을 op변수에 할당
temp=opstck->top; // 스택의 top을 temp에 할당
opstck->top=opstck->top->next; //스택의 top의 next 값을 스택의 top으로 할당
free(temp); // temp의 메모리를 해제
return op; // op값을 반환
}
return NULL; // NULL값을 반환
}


PostfixStack * PushPostfix(int val,PostfixStack *poststck) // postfix스택에 값을 push하는 함수 정의
{
Postfixnode *newnode; // 새로운 Postfixnode 포인터 변수 newnode 생성
if ((newnode=(Postfixnode*)malloc(sizeof(Postfixnode)))==NULL) { // malloc함수를 통해 newnode에 메모리 할당에 실패했다면
printf("ERROR, Couldn't allocate memory..."); // 오류 메시지 출력
return NULL; // NULL값 반환
}
else // 메모리 할당에 성공했다면
{
newnode->val=val; // 값을 newnode의 val에 할당
newnode->next=poststck->top; // 스택의 top을 newnode의 next에 할당
poststck->top=newnode; // newnode를 스택의 top으로 설정
return poststck; // 스택을 반환
}
}

char PopPostfix(PostfixStack *poststck) // postfix 스택에서 값을 pop하는 함수 정의
{
Postfixnode *temp; // 새로운 postfixnode 포인터 함수 temp 생성
int val; // int형의 변수 val 선언
if (poststck->top == NULL ) // 스택의 top이 NULL값이라면
{
printf("ERROR, empty stack..."); // 오류 메시지 출력
}
else // 스택의 top이 NULL이 아니라면
{
val=poststck->top->val; // 스택의 top의 val값을 val에 할당
temp=poststck->top; // 스택의 top을 temp 변수에 할당
poststck->top=poststck->top->next; // 스택의 top의 next값을 스택의 top에 할당
free(temp); // temp의 메모리를 해제
return val; // val값을 반환
}
return NULL; // NULL값을 반환
}





void Pop(Node * sNode,Stack *stck) // 스택에서 노드를 pop하는 함수 정의
{
Node *temp; // Node 포인터 변수 temp 선언

if (stck->top == NULL ) // 스택의 top이 NULL값이라면
{
printf("ERROR, empty stack..."); // 오류 메시지 출력
}
else // 스택의 top이 NULL이 아니라면
{
sNode->exp_data=stck->top->exp_data; // 스택의 top의 exp_data를 sNode의 exp_data에 할당
sNode->type=stck->top->type;// 스택의 top의 type을 sNode의 type에 할당
sNode->line=stck->top->line;// 스택의 top의 line을 sNode의 line에 할당
sNode->val=stck->top->val;// 스택의 top의 val를 sNode의 val에 할당
temp=stck->top; // 스택의 top을 temp에 할당
stck->top=stck->top->next; // 스택의 top의 next 값을 스택의 top에 할당
free(temp); // temp의 메모리를 해제
}
}
int isStackEmpty(OpStack *stck) // 스택의 empty여부를 확인하는 함수 정의
{
	if (stck->top==0) //스택의 top이 비어있다면  
		return 1; //1을 반환
return 0; // 스택의 top이 비어있지 않다면 0을 반환
}

/*void printAllStack(Stack * stck) // 스택을 출력하는 함수 정의 (주석처리)
{
	Node tempNode; // Node 변수 tempNode 선언
	printf("\n------------------\n");  // 구분선 출력
	printf("dumping the stack...\n"); // stack을 출력한다는 알림 출력
	Pop(&tempNode,stck); // 스택의 top을 pop
	printf("exp=%c type=%d val=%d 	line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line); // pop한 node의 정보들을 출력
	while( (stck->top!=NULL) ) { // 스택의 top이 NULL값이 아니면 실행한다.
		Pop(&tempNode,stck); // 스택의 node를 pop한다.
		printf("exp=%c type=%d val=%d 				line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line); // pop한 node들의 정보들을 출력한다.
	}
} */

int Priotry(char operator) //연산자의 우선순위를 지정하는 함수 정의
{
	if ((operator=='+') | (operator=='-')) // 연산자가 +이거나 -라면
		return 1; // 1을 반환
	else if ((operator=='/') | (operator=='*')) //연산자가 /거나 *라면
		return 2; // 2를 반환
return 0; // 0을 반환
}

int main(int argc,char ** argv) //main함수의 시작점, argc와 argv를 정의한다.
{
	char line[4096]; //문자열을 저장하는 line 배열 선언
	char dummy[4096]; //문자열을 저장하는 dummy 배열 선언
	char lineyedek[4096]; //문자열을 저장하는 lineyedek 배열 선언

	char postfix[4096]; //문자열을 저장하는 postfix 배열 선언

	char * firstword; // 문자열 포인터 변수 firstword 선언



//	int i; // int형 변수 i선언 (주석처리)
	int val1; // int형 변수 val1선언
	int val2; // int형 변수 val2선언


	int LastExpReturn; // int형 변수 LastExpReturn 선언
	int LastFunctionReturn=-999; // int형 변수 LastFunctionReturn 선언 후 -999로 초기화
	int CalingFunctionArgVal; // int형 변수 CalingFunctionArgVal선언

	Node tempNode;  // Node형식의 tempNode 변수 선언


	OpStack * MathStack; // OpStack 포인터 변수 MathStack 선언

	FILE *filePtr; // FILE 포인터 변수 filePtr 선언

	PostfixStack * CalcStack; // PostfixStack 포인터 변수 CalcStack 선언

	int resultVal; // int형 변수 resultVal 선언

	Stack * STACK; // Stack 포인터 변수 STACK 선언

	int curLine=0; // int형 변수 curLine 선언 후 0으로 초기화
	int foundMain=0; // int형 변수 foundMain 선언 후 0으로 초기화
        int WillBreak=0; // int형 변수 WillBreak 선언 후 0으로 초기화


	MathStack->top=NULL; //MathStack의 top에 NULL값을 할당
	CalcStack->top=NULL; //CalcStack의 top에 NULL값을 할당
	STACK->top=NULL; //Stack의 top에 NULL값을 할당
	clrscr(); // 화면을 클리어

	if (argc!=2) //argc 값이 2가 아니라면
	{
		/* if argument count is =1 */  
		printf("Incorrect arguments!\n"); // 오류 메시지 출력
		printf("Usage: %s <inputfile.spl>",argv[0]); // 경로 메시지 출력
		return 1; //1을 반환
	}





	/* open the file */

	if ( ( filePtr=fopen(argv[1],"r") ) == NULL ) // 파일을 열었을 때 값이 NULL이라면
	{
		printf("Can't open %s. Check the file please",argv[1]); // 오류 메시지 출력
		return 2; // 2를 반환
	}


	while (!feof(filePtr)) // 파일 포인터가 파일의 끝까지 읽지 못했을 때 
	{

		int k=0; // int형 변수 k를 정의하고 0으로 초기화

		fgets(line,4096,filePtr); // 코드를 한 줄씩 읽어 line에 저장 
		/* read the file by Line by Line */ 
		/* scan for /t characters. get rid of them! */
		while(line[k]!='\0') // 읽어온 코드가 NULL이 아닐 때
		{
		 if (line[k]=='\t') // line에 tab문자가 있다면
		 {
			line[k]=' '; // 스페이스 문자로 변환
		 }

		 k++; // k를 증가시킨다 (다음문자로 이동)
		}

		strcpy(lineyedek,line); // line 변수에 저장되어있는 정보를 lineyedek변수에 복사한다.

		curLine++; //현재 작업중인 line 정보를 1 증가시킨다
		tempNode.val=-999; //tempNode의 val를 -999로 초기화 한다.
		tempNode.exp_data=' ';  //tempNode의 exp_data을 ' '로 초기화 한다.
		tempNode.line=-999; //tempNode의 line을 -999로 초기화 한다.
		tempNode.type=-999; //tempNode의 type을 -999로 초기화 한다.



		if (!strcmpi("begin\n",line) | !strcmpi("begin",line)) // line에서 begin을 찾았을 때 (begin/n이거나)
		{
			if (foundMain) // main 함수를 찾았다면
			{
				tempNode.type=4; // tempNode의 type을 4로 설정한다.
				STACK=Push(tempNode,STACK); // 설정한 tempNode를 Stack에 Push한다.
			}
		}
		else if (!strcmpi("end\n",line) | !strcmpi("end",line) ) // line에서 end를 찾았을 때 (end/n이거나)
		{
			if (foundMain) // main 함수를 찾았다면
			{
				int sline; // int형 변수 sline 선언


				tempNode.type=5; // Node의 type을 5로 설정한다.
				STACK=Push(tempNode,STACK); // STACK에 tempNode를 push한다.

				sline=GetLastFunctionCall(STACK); // 마지막으로 호출된 함수 호출의 라인 번호를 sline 변수에 할당한다.
				if (sline==0) // 만약 호출된 함수의 라인 번호가 0이라면 (호출이 끝났다면)
				{
					/* WE FOUND THE RESULT! */
					printf("Output=%d",LastExpReturn); // LastExpReturn에 저장된 결과 값을 출력한다.

				}
				else // 마지막으로 호출된 함수 호출의 라인 번호가 0이 아니라면 (끝나지 않았다면)
				{
					int j; // int형 변수 j를 선언한다.
					int foundCall=0; // int형 변수 foundCall을 0으로 초기화한다.
					LastFunctionReturn=LastExpReturn; // 마지막 ExpReturn에 할당된 값을 LastFunctionRerun변수에 다시 할당한다.
					/* get to the last line that have been a function calling */

						fclose(filePtr); // 열려있던 파일을 닫는다.
							filePtr=fopen(argv[1],"r"); // 닫았던 파일을 다시 읽기 모드로 연다.
							curLine=0; // 현재 라인 번호를 0으로 초기화한다.
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */
							for(j=1;j<sline;j++) //  마지막 함수 호출이 있었던 라인까지
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */ //파일을 읽어 dummy변수에 저장한다. 
								curLine++; // 현재 라인 번호를 1 늘린다.
							}


						/* clear all the stack up to the last function call */
						while(foundCall==0) // foundcall 변수가 0일 때(아직 type이 3인 Node를 찾지 못했을 때)
						{
							Pop(&tempNode,STACK); //스택에서 Node를 pop한다.
							if (tempNode.type==3) //만약 Node의 type이 3인 Node를 찾았다면
							{
								foundCall=1; // foundCall에 1을 할당하여 반복문을 중단한다.
							}
						}

				}


			}
		}
		else // line에서 begin과 end모두 찾지 못했다면
		{
			//we need to tokinize
			firstword=strtok(line," "); //line을 공백을 기준으로 하여 토큰화해 firstword변수에 할당한다.


			if (!strcmpi("int",firstword)) // 만약 firstword에서 int를 찾았다면 (변수 선언 line을 찾았다면)
			{
				if (foundMain) // main함수를 찾았을 때
				{
					tempNode.type=1; /*integer*/ // tempNode의 type을 1로 설정한다(int형)
					firstword=strtok(NULL," "); // 다음 문자열을 firstword 변수에 할당한다. 
					tempNode.exp_data=firstword[0]; // fistword의 0번 list에 저장되어 있는 값을 tempNode의 exp_data에 할당한다. (변수 이름 저장)


					firstword=strtok(NULL," "); // 다음 문자열을 firstword 변수에 할당한다.

					/* check for = */
					if (!strcmpi("=",firstword)) // 만약 firstword에 저장되어 있는 값이 "="라면
					{
						firstword=strtok(NULL," "); // 다음 문자열을 firstword 변수에 할당한다.
					}

					tempNode.val=atoi(firstword); // firstword에 저장되어 있는 값을 정수형으로 변환하여 tempNode의 val에 할당한다. (변수의 값 저장)
					tempNode.line=0; // tempNode의 line번호를 0으로 한다.
					STACK=Push(tempNode,STACK); //tempNode를 STACK에 push한다.
				}
			}
			else if (!strcmpi("function",firstword)) // 만약 fisrtword에 저장된 값이 function이었다면
			{
				tempNode.type=2; // tempNode의 type을 2로 설정한다(함수)
				firstword=strtok(NULL," "); // 다음 문자열을 firstword에 할당한다.
				tempNode.exp_data=firstword[0]; // firstword의 0번 list에 저장되어 있는 값을 temNode의 exp_data에 할당한다. (함수 이름 저장)
				tempNode.line=curLine; // 현재 line을 tempNode의 line에 할당한다.
				tempNode.val=0; // tempNode의 val은 0으로 설정한다.
				STACK=Push(tempNode,STACK); // 설정한 tempNode를 STACK에 push한다.

				if ( (firstword[0]=='m') & (firstword[1]=='a') & (firstword[2]=='i') & (firstword[3]=='n')) // 만약 firstword에서 main 함수를 찾았다면
				{
					/*printf("Found function main() in line %d. Starting to running the script...\n",curLine);*/
					foundMain=1; // foundmain 변수에 1을 할당한다.


				}
				else // main함수가 아니라면
				{
					if (foundMain) // 이전에 main함수를 찾았다면
					{
						firstword=strtok(NULL," "); // 다음 문자열을 firstword에 할당한다.
						tempNode.type=1; // tempNode의 type을 1로 설정한다.
						tempNode.exp_data=firstword[0]; // firstword의 0번 list에 저장되어 있는 값을 tempNode의 exp_data에 할당한다.(변수 이름 저장)
						tempNode.val=CalingFunctionArgVal; // 함수 호출에서 인수의 값을 tempNode의 val에 저장한다. (인수 저장)
						tempNode.line=0; // tempNode의 line을 0으로 설정한다
						STACK=Push(tempNode,STACK); // tempNode를 STACK에 push한다.



					}

				}


			}
			else if (firstword[0]=='(') // 만약 firstword의 0번 list에 저장된 값이 '('라면
			{

			if (foundMain) // 메인 함수를 찾았다면
				{

				int i=0; // int형 변수 i를 0으로 초기화한다.
				int y=0; // int형 변수 y를 0으로 초기화한다.


				MathStack->top=NULL; // MathStack의 top에 NULL값을 할당하여 Stack을 비운다.
				/* now make the postfix calculcation */





				while(lineyedek[i]!='\x0') // line을 복사한 lineyedek 배열이 종료문자를 만날 때까지
				{
					/* evulate the function */
					if (isdigit(lineyedek[i])) { // 만약 lineyedek 배열에 저장된 값이 숫자라면
						postfix[y]=lineyedek[i]; // 해당값을 postfix 배열에 저장한다.
						y++; // y값을 1 증가시킨다.
					}
				     /*	else if (lineyedek[i]=='(')  // 만약 lineyedek 배열에 저장되어 있는 값이 '('라면 (주석 처리)
					{
						MathStack=PushOp(lineyedek[i],MathStack); // 해당 값을 MathStack에 push한다.
					}*/

					else if (lineyedek[i]==')') // 만약 lineyedek 배열에 저장되어 있는 값이 ')'라면
					{
						if (!isStackEmpty(MathStack)) // 만약 MathStack이 비어있지 않다면
						{
							postfix[y]=PopOp(MathStack); // 연산자를 pop하여 후위표기 배열에 할당한다.
							y++; // y값을 1 증가시킨다.
						}
					}
					else if ((lineyedek[i]=='+') | (lineyedek[i]=='-') | (lineyedek[i]=='*') | (lineyedek[i]=='/')) // 만약 lineyedek 배열에 저장되어 있는 값이 + , -, *, / 중 하나라면
					{
						/*operators*/
						if (isStackEmpty(MathStack)) // 만약 MathStack이 비어있다면
						{
					/* if stack empty push the operator to stack */

							MathStack=PushOp(lineyedek[i],MathStack); // lineyedek 배열의 값을 MathStack에 push한다.
						}
						else // 만약 MathStack이 비어있지 않다면
						{
							/* check for presedence */
							if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op) ) // 만약 MathStack의 top에 저장된 연산자의 우선순위가 lineyedek 배열의 연산자 우선순위보다 더 높거나 같으면
							{
								/* higher presedence for example + < *  */
								/* pop the last operator */

								/* add the poped operator to the postfix */
								postfix[y]=PopOp(MathStack); // MathStack에 저장되어 있던 산자를 pop하고 후위표기 배열에 할당한다.
								y++; // y값을 1 증가시킨다.


								MathStack=PushOp(lineyedek[i],MathStack); // lineyedek 배열의 연산자를 MathStack에 push한다.


							}
							else // 약 MathStack의 top에 저장된 연산자의 우선순위가 lineyedek 배열의 연산자 우선순위보다 낮다면
							{
								/* lower presedence for example / > + */
								/* push to stack */

								MathStack=PushOp(lineyedek[i],MathStack); // lineyedek 배열의 연산자를 MathStack에 push한다.
							}

						}
					}
					else if (isalpha(lineyedek[i])>0) //lineyedek 배열에 저장되어 있는 값이 알파벳이라면
					{
						int codeline=0; // int형 변수 codeline값을 0으로 초기화
						int dummyint=0; // int형 변수 dummyint값을 0으로 초기화
						/*look if it's a variable or function call 						*/
						int retVal=0; // int형 변수 retVal값을 0으로 초기화
						retVal=GetVal(lineyedek[i],&codeline,STACK); // Getval함수를 실행하여 반환값을 retval 변수에 할당한다.
						if ((retVal!=-1) & (retVal!=-999)) // 만약 retval에 저장되어 있는 값이 -1과 -999가 아니라면(유효한 값이라면)
						{
							/* if variable */
							postfix[y]=retVal+48; /* in ascii table numeric values start from 48 */ // retval에 저장된 값을 ASCII code형태로 변환(48을 더해서)하여 후위연산 배열에 저장
							y++; // y값을 1 증가시킨다.

						}
						else // 만약 retval에 저장된 값이 유효한 값이 아니라면
						{

						    if (LastFunctionReturn==-999) // 만약 직전 함수 호출이 없었다면 (-999) 
						    {

							/* if function */
							/* add to our system stack that we are making a call to function */
							int j; // int형 변수 j 선언
							tempNode.type=3; // tempNode의 type을 3으로 설정
							tempNode.line=curLine; // tempNode의 line을 현재 처리중인 line으로 설정
							STACK=Push(tempNode,STACK); // tempNode를 STACK에 push

							/* get function's arguments value */
							CalingFunctionArgVal=GetVal(lineyedek[i+2],&dummyint,STACK); //Getval함수를 실행하고 반환값을 CalingFunctionArgVal 변수에 저장



							fclose(filePtr); // 파일을 닫는다.
							filePtr=fopen(argv[1],"r"); // 파일을 읽기모드로 다시 연다.
							curLine=0; // 현재 line을 0으로 설정한다.
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */


/* !!!!!!!!!!!!!!! j <= codeline olabilir */
							for(j=1;j<codeline;j++) // for 반복문을 통해 codeline까지
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */ //파일을 읽어 dummy변수에 할당한다.
								curLine++; // 현재 line을 1증가시킨다.
							}

							WillBreak=1; // WillBreak 변수에 1을 할당한다.
							break; // 반복문을 빠져나온다.
						    }
						    else // 직전 함수 호출이 있었다면

						    {


							postfix[y]=LastFunctionReturn+48; /* in ascii table numeric values start from 48 */ //함수 호출 반환값을 ASCII code로 변환(48을 더한다)하여 후위 연산자 배열에 할당한다.
							y++; // y값을 1 증가시킨다.
							i=i+3; // i의 값을 3 증가시킨다.
							LastFunctionReturn=-999; // LastFunctionReturn값을 -999로 초기화한다.


						    }
						}
					}

					i++; // i값을 1 증가시킨다.
				}

				if (WillBreak==0) // WillBreak 변수가 0이라면
				{
				/* get out items left in the mathstack */
				while (isStackEmpty(MathStack)==0) // MathStack이 비어있지 않을 때 
				{

					/* add the poped operator to the postfix */
					postfix[y]=PopOp(MathStack); // MathStack의 연산자를 pop하고 연산자 배열에 할당한다.
					y++; // y값을 1 증가시킨다.
				}

				postfix[y]='\0'; // 후위연산자 배열에 null 문자를 넣어 배열을 마무리한다.

				//MathStack=FreeAll(MathStack); // MatchStack의 메모리를 해제한다.(주석 처리)

				/* now calculate the postfix */
				/*printf("\nCURRENT POSTFIX=%s\n",postfix);*/

				i=0; // i에 0을 할당한다.

				CalcStack->top=NULL; // CalcStack의 top에 NULL값을 할당한다.(초기화)
				while(postfix[i]!='\x0') // 후위연산 배열이 종료되지 않았을 때
				{
					if (isdigit(postfix[i])) { // 만약 후위연산 배열이 숫자라면
						/* push to stack */
						CalcStack=PushPostfix(postfix[i]-'0',CalcStack); // 숫자 0의 ASCII code == 48임을 이용해 정수값을 획득하여 반환된 정수값을 CalcStack에 push한다.
					}
					else if ((postfix[i]=='+') | (postfix[i]=='-') | (postfix[i]=='*') | (postfix[i]=='/')) // 만약 postfix 배열 값이 +, -, *, / 라면
					{
						val1=PopPostfix(CalcStack); // ClacStack을 pop하여 val1 변수에 할당한다.

						val2=PopPostfix(CalcStack); // ClacStack을 pop하여 val2 변수에 할당한다.


						switch (postfix[i]) //postfix 배열에 switch문을 이용하여
						{
							case '+': resultVal=val2+val1;break; // '+'라면 val2와 val1을 더하여 resultVal변수에 할당한다.
							case '-': resultVal=val2-val1;break; // '-'라면 val2와 val1을 뺄셈여 resultVal변수에 할당한다.
							case '/': resultVal=val2/val1;break; // '/'라면 val2에서 val1을 나누어 resultVal변수에 할당한다.
							case '*': resultVal=val2*val1;break; // '*'라면 val2와 val1을 곱하여 resultVal변수에 할당한다.
						}

						CalcStack=PushPostfix(resultVal,CalcStack); // CalcStack에 resultVal을 push한다.
					}
					i++; // i 를 1 증가시킨다.
				}

				//CalcStack=FreeAll(CalcStack); // CalcStack의 메모리를 해제한다. (주석처리)
				LastExpReturn=CalcStack->top->val; // LastExpReturn에 CalcStack의 Top이 val값을 할당한다.(연산된 결과값을 저장)


				}
				WillBreak=0; // WillBreak변수에 0으로 초기화한다.
				}
			}
		}


	}

	fclose(filePtr); //열려있는 파일을 닫는다.

	//printAllStack(STACK); // Stack을 모두 출력한다 (주석처리)
	STACK=FreeAll(STACK); // Stack을 모두 초기화한다.

	printf("\nPress a key to exit..."); // 종료 안내 문구를 출력한다.
	getch(); // 키보드로 부터 입력값을 받으면
	return  0; // 0을 반환하여 종료한다.
}

Stack * FreeAll(Stack * stck) // FreeAll 함수 정의 (매개변수로 받은 stck(Stack)에 대한)
{
Node * temp; // Node 포인터 변수 temp를 선언한다.
Node * head; // Node 포인터 변수 head를 선언한다.

	if (stck->top != NULL ) // 만약 stck의 top이 NULL값이 아니라면
	{
		head=stck->top; // head변수에 stck의 top을 할당
		do // 반복문 시작
		{

			temp=head; // head를 temp에 할당한다.
			head=head->next; // head의 next를 head에 할당한다.
			free(temp); // temp 변수의 메모리를 해제한다.

		} while (head->next!=NULL); // head의 next가 NULL값이 될 때까지 반복한다.
	}

return NULL; // NULL값을 반환한다.
}
int GetLastFunctionCall(Stack *stck) // 마지막 호출된 함수 라인번호를 반환하는 함수를 정의한다.
{
Node * head; // Node 포인터 변수 head를 선언한다.

	if (stck->top == NULL ) // 만약 stck의 top이 NULL값이라면
	{
		printf("ERROR, empty stack..."); // 에러문구를 출력한다.
	}
	else // 만약 stck의 top이 NULL이 아니라면
	{
		head=stck->top; // stck의 top을 head변수에 할당한다.
		do // 반복문 시작
		{
			if ( head->type==3 ) // 만약 head의 type이 3이라면
			{
				return head->line; // head의 line값을 반환한다.
			}
			else // 만약 head의 type이 3이 아니라면
			{
				head=head->next; // head의 next를 head에 할당한다.
			}
		} while (head->next!=NULL); // head의 next가 NULL일 때까지 반복한다.
	}

	return 0; // 0을 반환한다.



}

int GetVal(char exp_name,int * line,Stack *stck) // line 값을 설정하고 변수나 함수 값을 반환하는 GetVal함수 정의
{

Node * head; // Node 포인터 변수 head를 선언한다.
*line=0; // *line을 0으로 설정한다.
	if (stck->top == NULL ) // 만약 stck의 top이 NULL값이라면
	{
		printf("ERROR, empty stack..."); // 에러문구를 출력한다.
	}
	else // stck의 top이 NULL이 아니라면
	{
		head=stck->top; // stck의 top을 head 변수에 할당한다.
		do // 반복문 시작
		{
			if ( head->exp_data==exp_name ) // 만약 head의 exp_data와 exp_name이 같다면
			{

				if (head->type==1) // 만약 head의 type이 1라면(int라면)
				{
					/* return the variables value */
					return head->val; // head의 val값을 반환한다.
				}
				else if (head->type==2) // 만약 head의 type이 2라면 (함수라면)
				{
					*line=head->line; // head의 line을 *line에 할당한다.
					return -1; // -1을 반환한다.
					/* it's a function so return -1 */ //(함수라면 -1을 반환하기로 정의)
				}

			}
			else // 만약 head의 exp_data와 exp_name이 같지 않다면
			{
				head=head->next; // head의 next를 head로 설정한다.
			}
		} while (head->next!=NULL); // head의 next가 NULL값이 될 때까지 실행한다.
		/* check agin once more */ // line 699부터 714를 재실행해서 다시 확인한다.
		if (head->exp_data==exp_name) // 만약 head의 exp_data와 exp_name이 같다면
		{

				if (head->type==1) // 만약 head의 type이 1라면(int라면)
				{
					/* return the variables value */
					return head->val; // head의 val값을 반환한다.
				}
				else if (head->type==2) // 만약 head의 type이 2라면 (함수라면)
				{
					*line=head->line; // head의 line을 *line에 할당한다.
					return -1; // -1을 반환한다.
					/* it's a function so return -1 */
				}
	

		}
	}

	return -999; // -999를 반환한다.
}

