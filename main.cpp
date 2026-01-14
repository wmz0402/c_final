#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define PassWord_Len 30
#define freeze_time 10
#define MaxCount 4
#define MAXUSER 100
typedef struct
{
	char username[11];
	char password[10];
	char question[50];
	char anwers[50];
	int attemp_count;
	time_t start_time;
}User;

typedef struct UserNode{
	User user;
	struct UserNode* next;
}UserNode;

UserNode* userHead = NULL;
int user_count = 0;
//登陆状态标记（-1未登录，>=0为已登录用户索引）
int loggedInUserIndex = -1;
void clean()
{
	int a;
	while((a = getchar()) != '\n'&& a != EOF);
}

int loginAttempt = 0;
time_t freezeStarTime = 0;
//学生信息
typedef struct _Student
{
	int stuNum;
	char name[50];
	char dept[50];
	char className[50];
	float math;
	float english;
	float computer;
	float total;
	float avg;
}Student;
//节点信息
typedef struct _Node
{
	Student student;
	struct _Node* next;
}Node;

typedef enum
{
	ADMIN,
	USER,
	NONE
}UserType;


const char adminPassWord[] = "admin2007";
const char userPassWord[] = "user0402";
void initUser();
void saveUser();
UserNode* findUserByName(const char* username);
void addUser(const User* newUser);
void stars(char *password);
void random_captchar(char* captcha);
void forget_password();
void new_user();
void old();
void welcome();
UserType selectSystem();
void countdown(int seconds);
int isAccountFrozen();
int checkPassWord(const char *password);
int inputAndVerifyPassWord(const char*password,const char*systemName);
void download();
void welcome1();
void welcome2();
void inputStudent(Node* head);
void printStudent(Node* head);
void countStudent(Node* head);
void findStudent(Node* head);
void saveStudent(Node* head);
void loadStudent(Node* head);
void modifyStudent(Node* head);
void deleteStudent(Node* head);
void sortStudent(Node* head);
void analyzeStudentScore(Node* head);
float calculateAverageScore(Node* head,const char* subject);
float findMaxScore(Node* head,const char* subject);
float findMinScore(Node* head,const char* subject);
int countScoreRange(Node* head,const char* subject,float min,float max);
void quitStudent();


int main(){
	//创建头节点
	Node* head =(Node*) malloc(sizeof(Node));
	head->next = NULL;
	loadStudent(head);
	initUser();
	welcome();
	while(1)
	{
	printf("\n");
    download();
	char a = _getch();
	printf("\n");
	switch(a)
	{
		case '1':
			new_user();
			break;
		case '2':
			old();
			if(loggedInUserIndex != -1)
			goto end_login_loop;
			break;
		case '3':
			forget_password();
			break;
		case '4':
			printf("感谢选择 king 系统\n");
			exit(0);
		default:
			printf("没有该选项！\n");
			_getch();
			system("cls");
			break;
	}
	}
	end_login_loop:;
	UserType sysType = selectSystem();
	while(1){
		if(sysType == ADMIN)
		{
		welcome1();
		char c = _getch();
		switch (c)
		{
		case '1':
	 	    inputStudent(head);
	 		break;
	 	case '2':
	 		printStudent(head);
	    	break;
		case '3':
		   	countStudent(head);
			break;
	 	case '4':
	 		findStudent(head);
	 		break;
		case '5':
			modifyStudent(head);
	    	break;
	    case '6':
	    	deleteStudent(head);
			break;
	 	case '7':
	 		sortStudent(head);
	 		break;
	 	case '8':
	 		analyzeStudentScore(head);
	 		break;
	 	case '0':
	 		quitStudent();
	    	break;
	    default:
	    	printf("没有该选项，请重新输入！\n");
	    	system("pause");
	    	system("cls");
			break;
    	}
		}else if(sysType == USER)
		{
			welcome2();

			char ch = _getch();
		switch (ch)
		{
	 	case '1':
	 		printStudent(head);
	    	break;
		case '2':
		   	countStudent(head);
			break;
	 	case '3':
	 		findStudent(head);
	 		break;
	 	case '4':
	 		analyzeStudentScore(head);
	 		break;

	 	case '0':
	 		quitStudent();
	    	break;
	    default:
	    	printf("没有该选项，请重新输入！\n");
			break;
		}
		}
	}
	free(head);
	UserNode* temp = userHead;
	while(temp !=NULL)
	{
		UserNode* del = temp;
		temp = temp->next;
		free(del);
	}
	return 0;
}

void initUser()
{
	UserNode* temp = userHead;
	while(temp != NULL)
	{
		UserNode* del = temp;
		temp = temp->next;
		free(del);
	}
	userHead = (UserNode*)malloc(sizeof(UserNode));
	userHead->next = NULL;
	user_count = 0;
	FILE* fp = fopen("user_date.txt","r");
	if(fp == NULL)
	{
		printf("未找到用户文件，将自动创建！\n");
		return ;
	}
	User tempUser;
	while(fscanf(fp,"%s %s %s %s %d %ld",tempUser.username,tempUser.password,
	tempUser.question,tempUser.anwers,&tempUser.attemp_count,&tempUser.start_time)!=EOF)
	{
		UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
		newNode->user = tempUser;
		newNode->next = NULL;
		UserNode* move = userHead;
		while(move->next!=NULL)
		{
			move = move->next;
		}
		move->next = newNode;
		user_count++;
	}
	fclose(fp);
	printf("用户数据读取成功！\n");
}

void saveUser()
{
	FILE* fp = fopen("user_date.txt","w");
	if(fp == NULL)
	{
		printf("打开文件失败，数据无法保存！\n");
		return ;
	}
	UserNode* temp = userHead->next;
	while(temp!=NULL)
	{
		fprintf(fp,"%s %s %s %s %d %ld\n",temp->user.username,temp->user.password,
		temp->user.question,temp->user.anwers,temp->user.attemp_count,temp->user.start_time);
		temp = temp->next;
	}
	fclose(fp);
}

UserNode* findUserByName(const char* username)
{
	UserNode* temp = userHead->next;
	while(temp != NULL)
	{
		if(strcmp(temp->user.username,username) == 0)
		{
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void addUser(const User* newUser)
{
	if(user_count >= MAXUSER)
	{
		printf("注册人数已达到上线，无法注册！\n");
		return ;
	}
	//创建新节点
	UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
	newNode->user = *newUser;
	newNode->next = NULL;
	UserNode* move = userHead;
	while(move->next != NULL)
	{
		move = move->next;
	}
	move->next = newNode;
	user_count++;
	saveUser();
}


void welcome()
{
	system("color 0A");
	char arr1[] = "Welcome to the King's World!";
	char arr2[] = "############################";
	int left = 0;
	int right = strlen(arr2)-1;
	while(left <= right)
	{
		arr2[left] = arr1[left];
		arr2[right] = arr1[right];
		printf("%s\n",arr2);
		Sleep(500);
		system("cls");
		left++;
		right--;
	}
	printf("%s\n",arr2);
}

void random_captchar(char* captcha)
{
	const char content[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	srand (time(NULL));
	for(int i = 0;i < 6;i++)
	{
		int index = rand() % (sizeof(content) - 1);
		captcha[i] = content[index];
	}
	captcha[6] = '\0';
}

void stars(char *password){
	int i=0;
	char ch;
	while(1){
		ch=getch();
		if(ch=='\r'||ch=='\n'){
			password[i]='\0';
			printf("\n");
			break;
		}
		else if(ch=='\b'){
			if(i>0){
				i--;
				printf("\b \b");
			}
		}
		else if(i<10){
			password[i]=ch;
			i++;
			printf("*");
		}
	}
}
void forget_password()
{
	char username[11];
	char reply[50];
	char new_password[11];
	char confirm_password[11];
	UserNode* userNode = NULL;
	printf("====== 找回密码 =====\n");
	printf("请输入你的用户名>\n");
	fgets(username,sizeof(username),stdin);
	username[strcspn(username,"\n")] = 0;
	clean();
	userNode = findUserByName(username);
	if(userNode == NULL)
	{
		printf("该用户名不存在！\n");
		system("cls");
		return ;
	}
	printf("安全问题：%s\n",userNode->user.question);
	printf("请输入答案：");
	fgets(reply,sizeof(reply),stdin);
	reply[strcspn(reply,"\n")] = 0;
	if(strcmp(userNode->user.anwers,reply) != 0)
	{
		printf("答案错误！\n");
		printf("请按任意键继续.......\n");
		getchar();
		system("cls");
		return ;
	}else{
		printf("回答正确\n");
	}

	int nice_password =0;
	do{
		printf("请输入您的新密码（必须包含数字和字母最多十个）：\n");
		stars(new_password);
		if(strlen(new_password) < 1)
		{
			printf("密码不能为空！\n");
			continue;
		}
		if(!checkPassWord(new_password))
		{
			printf("密码必须包含数字和字母！\n");
			continue;
		}
		printf("请再次输入你的密码：\n");
		stars(confirm_password);
		if(strcmp(new_password,confirm_password)!=0)
		{
			printf("两次输入的密码不一样请重新设置！\n");
			continue;
		}
		nice_password = 1;
	}while(!nice_password);
	strcpy(userNode->user.password,new_password);
	userNode->user.attemp_count = 0;
	printf("密码重置成功！\n");
	Sleep(1000);
	system("cls");
}


void new_user()
{
	if(user_count >= 100)
	{
		printf("注册数量已达到最大值，无法注册！\n");
		Sleep(1000);
		system("cls");
		return ;
	}
	char username[11];
	char password[11];
	char confirm_password[11];
	char captcha[7];
	char user_captcha[7];
	User newUser;
	system("cls");
	printf("\n==== 用户注册 ====\n");
	printf("请输入用户姓名：\n");
	fgets(username,sizeof(username),stdin);
	username[strcspn(username,"\n")] = 0;
	clean();
	if(findUserByName(username) != NULL)
	{
		printf("该用户已存在，请输入其他用户名！\n");
		printf("请按任意键继续......\n");
		_getch();
		system("cls");
		return;
	}
	random_captchar(captcha);
	printf("请输入验证码(%s)：",captcha);
	scanf("%6s",user_captcha);
	clean();
	if(strcmp(captcha,user_captcha) != 0)
	{
		printf("验证码错误，你是人机！\n");
		Sleep(1000);
		system("cls");
		return ;
	}
	int nice_password = 0;
	do{
		printf("请设置您的密码(最多10位)：");
		stars(password);
		if(strlen(password) < 1)
		{
			printf("密码不能为空！\n");
			continue;
		}
		if(!checkPassWord(password))
		{
			printf("密码必须包含数字和字母！\n");
			continue;
		}
		if(strlen(password)>10)
		{
			printf("密码不能超过十位！\n");
			continue;
		}
		printf("请再次输入密码：");
		stars(confirm_password);
		if(strcmp(password,confirm_password) != 0)
		{
			printf("两次输入的密码不一样，请重新输入！\n");
			continue;
		}
		nice_password = 1;
	}while(!nice_password);
	clean();
	printf("请设置安全问题：\n");
	fgets(newUser.question,sizeof(newUser.question),stdin);
	newUser.question[strcspn(newUser.question,"\n")] = 0;
	printf("请设置答案：\n");
	fgets(newUser.anwers,sizeof(newUser.anwers),stdin);
	newUser.anwers[strcspn(newUser.anwers,"\n")] = 0;
	strcpy(newUser.username,username);
	strcpy(newUser.password,password);
	newUser.attemp_count = 0;
	newUser.start_time = 0;
	addUser(&newUser);
	printf("注册成功！\n");
	Sleep(1000);
	system("cls");
}

void old()
{
	char username[10];
	char password[10];
	UserNode* userNode = NULL;
	printf("请输入姓名：\n");
	scanf("%10s",username);
	clean();
	userNode = findUserByName(username);
	if(userNode == NULL)
	{
		printf("该用户名不存在！\n");
		system("pause");
		system("cls");
		return ;
	}
	time_t now = time(NULL);
	if(userNode->user.start_time != 0&&(now - userNode->user.start_time) < freeze_time)
	{
		printf("账号冻结中，剩余%ld秒\n",freeze_time - (now - userNode->user.start_time));
		system("pause");
		system("cls");
		return ;
	}
	while(1)
	{
		printf("请输入密码：");
		stars(password);
		if(strcmp(password,userNode->user.password) == 0)
		{
			printf("登陆成功,欢迎回来%s!\n",username);
			userNode->user.attemp_count = 0;
			loggedInUserIndex = 0;
			saveUser();
			Sleep(1000);
			system("cls");
			break;
		}else{
			userNode->user.attemp_count++;
			if(userNode->user.attemp_count >= MaxCount)
			{
				userNode->user.start_time = time(NULL);
				printf("\033[1;31m");
				printf("密码错误，账号将被冻结10秒\n");
				printf("\033[0m");
				countdown(freeze_time);
				break;
			}else{
				int remaining_attempts = MaxCount - userNode->user.attemp_count;
				printf("\033[1;31m");
				printf("密码错误还省%d次机会\n",remaining_attempts);
				printf("\033[0m");
				printf("请重新输入密码......\n");
				Sleep(500);
				system("cls");
			}
		}
	}
}

void download()
{
	printf("================ 请选择登陆方式 =================\n");
	printf("================ 1. 注册新用户  =================\n");
	printf("================ 2. 登录账号    =================\n");
	printf("================ 3. 忘记密码    =================\n");
	printf("================ 4. 退出系统    =================\n");
	printf("=================================================\n");
}

UserType selectSystem()
{
	while(1){
		printf("\n**************请选择系统**************\n");
		printf("**********   1.管理员系统   **********\n");
		printf("**********   2.学生系统     **********\n");
		printf("**************************************\n");
		char choice = _getch();
		printf("你的选择是：%c\n",choice);
		if(choice == '1')
		{
			if(inputAndVerifyPassWord(adminPassWord,"管理员系统"))
			{
				return ADMIN;
			}
		}else if(choice == '2')
		{
			if(inputAndVerifyPassWord(userPassWord,"学生系统"))
			{
				return USER;
			}
		}else{
			printf("无效选择，请重新选择！\n");
			Sleep(500);
			system("cls");
		}
	}
	return NONE;
}
//倒计时提示
void countdown(int seconds)
{
	system("color 0C");
	for(int i = seconds;i>0;i--)
	{
		printf("\r账号已冻结！请在%d秒后重试......",i);

	fflush(stdout);//强制刷新输出
	Sleep(1000);
	}
	printf("\r账号已解冻，请重新登录！");
	system("color 0A");
}

int isAccountFrozen()
{
	if(freezeStarTime == 0)
	return 0;
	time_t now = time(NULL);
	if(now - freezeStarTime < freeze_time)
	{
		int remaining = (int)(freeze_time-(now - freezeStarTime));
		countdown(remaining);
		return 1;
	}else{
		freezeStarTime = 0;
		loginAttempt = 0;
		return 0;
	}
}
//必须由字母和数字组成
int checkPassWord(const char *password)
{
	int letter = 0,degit = 0;
	for(int i = 0;password[i] != '\0';i++)
	{
		if((password[i] >= 'A'&&password[i] <= 'Z') || (password[i] >= 'a'&&password[i] <= 'z') )
		{
			letter = 1;
		}else if((password[i] >= '0'&&password[i] <= '9'))
		{
			degit = 1;
		}
	}
	return letter && degit;
}
//密码输入
int inputAndVerifyPassWord(const char*correctPassWord,const char*systemName)
{
	if(isAccountFrozen())
	return 0;
	char inputPassword[PassWord_Len];
	int i = 0;
	char ch;
	printf("请输入密码：\n");
	while(1)
	{
		ch = _getch();
		if(ch == '\r' || ch == '\n')
		{
			inputPassword[i] = '\0';
			printf("\n");
			break;
		}else if(ch == '\b')
		{
			if(i > 0)
			{
				i--;
				printf("\b \b");
			}
		}else if(i < PassWord_Len - 1)
		{
			inputPassword[i++] = ch;
			printf("*");
		}
	}
	//校验密码的复杂度和正确性
	if(!checkPassWord(inputPassword))
	{
		loginAttempt++;
		printf("密码复杂度不够，剩余尝试次数：%d\n",MaxCount - loginAttempt);
		Sleep(1000);
		system("cls");
	}else if(strcmp(inputPassword,correctPassWord)==0)
	{
		printf("%s登录成功！\n",systemName);
		loginAttempt = 0;
		Sleep(1000);
		system("cls");
		return 1;
	}else
	{
		loginAttempt++;
		printf("密码错误，剩余尝试次数：%d\n",MaxCount - loginAttempt);
		Sleep(1000);
		system("cls");
	}
	if(loginAttempt >= MaxCount)
	{
		freezeStarTime = time(NULL);
		printf("登陆失败次数太多，账号冻结 %d 秒\n",freeze_time);
	}
	return 0;
}

void welcome1()
{
	    system("cls");
    	printf("*******************************************************\n");
		printf("*             欢迎使用学生成绩管理系统               *\n");
		printf("*******************************************************\n");
		printf("*                  请选择功能                        *\n");
		printf("*                1.录入学生信息                      *\n");
		printf("*                2.打印学生信息                      *\n");
		printf("*                3.统计学生信息                      *\n");
		printf("*                4.查找学生信息                      *\n");
		printf("*                5.修改学生信息                      *\n");
		printf("*                6.删除学生信息                      *\n");
		printf("*                7.按成绩排序                        *\n");
		printf("*                8.成绩分析                          *\n");
		printf("*                0.退出系统                          *\n");
		printf("*******************************************************\n");
		printf("请选择：");
}

void welcome2()
{
	    system("cls");
    	printf("*******************************************************\n");
		printf("*             欢迎使用学生成绩管理系统               *\n");
		printf("*******************************************************\n");
		printf("*                  请选择功能                        *\n");
		printf("*                1.打印学生信息                      *\n");
		printf("*                2.统计学生信息                      *\n");
		printf("*                3.查找学生信息                      *\n");
		printf("*                4.成绩分析                          *\n");
		printf("*                0.退出系统                          *\n");
		printf("*******************************************************\n");
		printf("请选择：");
}

int checkStudent(Node* head,int num)
{
	Node* move = head->next;
	while(move!=NULL)
	{
		if(move->student.stuNum == num)
		return 1;
		move=move->next;
	}
	return 0;
}

void inputStudent(Node* head)
{
    Node* fresh =(Node*)malloc(sizeof(Node));//申请一块空间
    fresh->next = NULL;
    Student* s =&(fresh->student);
    int num;
    while(1)
    {
		printf("请输入学生学号：");
		scanf("%d",&num);
		if(checkStudent(head,num)==1)
		{
			printf("该学号已存在，请重新输入！\n");
			continue;
		}
		s->stuNum = num;
		break;
	}
	printf("请输入学生姓名：");
	scanf("%s",s->name);
	printf("请输入学生系别：");
	scanf("%s",s->dept);
	printf("请输入学生班级：");
	scanf("%s",s->className);
	printf("请输入学生数学成绩：");
	scanf("%f",&s->math);
	printf("请输入学生英语成绩：");
	scanf("%f",&s->english);
	printf("请输入学生计算机成绩：");
	scanf("%f",&s->computer);
	s->total=s->computer+s->english+s->math;
	s->avg=s->total/3.0;
	clean();
	Node* move = head;
	while(move->next!=NULL)
	{
		move = move->next;
	}
	move->next = fresh;
	saveStudent(head);
	printf("学生信息录入成功！\n");
	system("pause");
	system("cls");
}

void printStudent(Node* head)
{
	if(head->next==NULL)
	{
		printf("暂无学生信息！\n");
		system("pause");
		return ;
	}
	printf("请选择要打印的方式：\n");
	printf("1.打印全部学生信息\n");
	printf("2.按系打印\n");
	printf("3.按班级打印\n");
	printf("请输入选择：");
	char choice = _getch();
	system("cls");
	char target[10]="";
	if(choice == '2'||choice == '3')
	{
		if(choice == '2')
		printf("请输入要打印的系别：");
		else
		printf("请输入要打印的班级：");
		scanf("%s",target);
		clean();
		system("cls");
	}
	printf("==========================================================================================\n");
	printf("%-8s %-10s %-15s %-8s %-8s %-8s %-8s %-8s %-8s\n",
	"学号","姓名","系别","班级","数学成绩","英语成绩","计算机成绩","总成绩","平均成绩");
	printf("==========================================================================================\n");
	Node* move = head->next;
	int printCount = 0;
	while(move != NULL)
	{
		Student* s = &(move->student);
		int isMatch = 0;
		if(choice == '1')
		{
			isMatch = 1;
		}else if(choice == '2')
		{
			isMatch = (strcmp(s->dept,target) == 0);
		}else if(choice == '3')
		{
			isMatch = (strcmp(s->className,target) == 0);
		}
		if(isMatch)
		{
			printf("%-8d %-10s %-15s %-10s %-8.1f %-8.1f %-8.1f %-8.1f %-8.1f\n",
			s->stuNum,s->name,s->dept,s->className,s->math,s->english,s->computer,s->total,s->avg);
			printCount++;
		}
		move = move->next;
	}
	printf("==========================================================================================\n");
	if(printCount == 0)
		printf("未找到相关信息");
	else
		printf("共打印%d名学生信息\n",printCount);
	system("pause");
	system("cls");
}

void countStudent(Node* head){
	int count = 0;
	Node* move = head->next;
	while(move !=NULL){
		count++;
		move = move->next;
	}
	printf("学生的总人数为:%d\n",count);
	system("pause");
	system("cls");
}

float getMathScore(Student* s)
{
	return s->math;
}
float getEnglishScore(Student* s)
{
	return s->english;
}
float getComputer(Student* s)
{
	return s->computer;
}

void findStudent(Node* head)
{
	if(head->next == NULL)
	{
		printf("暂无学生信息！\n");
		system("pause");
		system("cls");
		return ;
	}
	printf("请选择查找的方式：\n");
	printf("1.按学号查找学生\n");
	printf("2.查找学生成绩\n");
	printf("请输入选择: ");
	char findType = _getch();
	system("cls");
	if(findType == '1')
	{
		printf("请输入查找的学生学号：");
		int stuNum;
		scanf("%d",&stuNum);
		clean();
		Node* move = head->next;
		while(move != NULL)
		{
			if(stuNum == move->student.stuNum)
			{
				Student* s =&(move->student);

			printf("==================================== 学生信息 =================================\n");
			printf("学号：%d\n姓名：%s\n系别：%s\n班级：%s\n",s->stuNum,s->name,s->dept,s->className);
			printf("数学成绩：%.1f | 英语成绩：%.1f | 计算机成绩：%.1f\n",s->math,s->english,s->computer);
			printf("总成绩：%.1f | 平均成绩：%.1f\n",s->total,s->avg);
			printf("===============================================================================\n");
			system("pause");
			system("cls");
			return ;
			}
			move = move->next;
		}
	}
	else if (findType == '2')
	{
		printf("请选择科目：1.数学 2.英语 3.计算机\n");
		printf("请输入选择：");
		char subjType = _getch();
		system("cls");
		const char* subjName = "";
		float (*getScore)(Student*) = NULL;
		if(subjType == '1')
		{
			subjName = "数学";
			getScore = getMathScore;
		}
		else if(subjType == '2')
		{
			subjName = "英语";
			getScore = getEnglishScore;
		}
		else if(subjType == '3')
		{
			subjName = "计算机";
			getScore = getComputer;
		}
		else{
			printf("选择错误！\n");
			system("pause");
			system("cls");
			return ;
		}
		printf("=============================%s成绩列表===============================\n",subjName);
		printf("%-8s %-10s %-15s %-8s %-8s\n","学号","姓名","系别","班级",subjName);
		printf("======================================================================\n");
		Node* move = head->next;
		int printCount = 0;
		while(move != NULL)
		{
			Student* s = &(move->student);
			printf("%-8d %-10s %-15s %-8s %-8.1f\n ",
			s->stuNum,s->name,s->dept,s->className,getScore(s));
			printCount++;
			move = move->next;
		}
		printf("=========================================================================\n");
		printf("共查询到%d名学生的%s成绩\n",printCount,subjName);
	}
	else{
	printf("未找到学生信息\n");
	}
	system("pause");
	system("cls");
}

void saveStudent(Node* head)
{
	FILE* file = fopen("student_data.txt","w");
	if(file == NULL)
	{
		printf("打开文件失败\n");
		return ;
	}
	Node* move=head->next;
	while(move!=NULL)
	{
		Student* s = &(move->student);
		fprintf(file,"%d %s %s %s %.1f %.1f %.1f %.1f %.1f\n",
		s->stuNum,s->name,s->dept,s->className,s->math,s->english,s->computer,s->total,s->avg);
		move = move->next;
	}
	fclose(file);
	printf("数据已保存到文件!\n");
}

void loadStudent(Node* head)
{
	FILE* file = fopen("student_data.txt","r");
	if(file == NULL)
	{
		printf("未找到学生文件，跳过读取\n");
		return ;
	}
	Node* temp = head->next;
	while(temp !=NULL)
	{
		Node* del = temp;
		temp = temp->next;
		free(del);
	}
	head->next=NULL;
	Student s;
	while(fscanf(file,"%d %s %s %s %f %f %f %f %f",
	&s.stuNum,s.name,s.dept,s.className,&s.math,&s.english,&s.computer,&s.total,&s.avg) !=EOF)
	{
		Node* fresh = (Node*)malloc(sizeof(Node));//创建新节点
		fresh->student = s;
		fresh->next = NULL;

		Node* move = head;
		while(move->next!=NULL)move = move->next;
		move->next = fresh;
	}

		fclose(file);
		printf("读取成功\n");
}

void modifyStudent(Node* head)
{
	if(head->next == NULL)
	{
		printf("暂无学生信息，无法修改！\n");
		system("pause");
		system("cls");
		return ;
	}
	printf("请输入要修改的学生的学号：");
	int stuNum;
	scanf("%d",&stuNum);
	clean();
	Node* move = head->next;
	while(move !=NULL){
		if(move->student.stuNum == stuNum)
		{
		Student* s = &(move->student);
		printf("============================= 当前学生信息 =======================\n");
		printf("学号：%d\n姓名：%s\n系别：%s\n班级：%s\n",s->stuNum,s->name,s->dept,s->className);
		printf("数学成绩：%.1f\n英语成绩；%.1f\n计算机成绩：%.1f\n",s->math,s->english,s->computer);
		printf("总成绩：%.1f\n平均成绩：%.1f\n",s->total,s->avg);
		printf("===================================================================\n");
		printf("请选择要修改的字段: \n");
		printf("1.姓名\n2.系别\n3.班级\n4.数学成绩\n5.英语成绩\n6.计算机成绩\n");
		printf("请输入你的选择：");
		char choice = _getch();
		system("cls");
		switch(choice)
		{
			case '1':
				printf("请输入新姓名：");
				scanf("%s",s->name);
				break;
			case '2':
				printf("请输入新系别：");
				scanf("%s",s->dept);
				break;
			case '3':
				printf("请输入新班级：");
				scanf("%s",s->className);
				break;
			case '4':
				printf("请输入新的数学成绩：");
				scanf("%f",&s->math);
				break;
			case '5':
				printf("请输入新的英语成绩：");
				scanf("%f",&s->english);
				break;
			case '6':
				printf("请输入新的计算机成绩：");
				scanf("%f",&s->computer);
				break;
			default:
				printf("选择错误，修改取消！\n");
				system("pause");
				system("cls");
				return;
		}
		s->total=s->english+s->math+s->computer;
		s->avg = s->total / 3.0;
		saveStudent(head);
		printf("修改成功\n");
		system("pause");
		system("cls");
		return ;
		}
		move = move->next;
	}
	printf("未找到学生信息\n");
	system("pause");
	system("cls");
}

void deleteStudent(Node* head)
{
	printf("请输入删除的学生学号：");
	int stuNum = 0;
	scanf("%d",&stuNum);

	Node* move=head;
	while(move->next !=NULL)
	{

		if(move->next->student.stuNum == stuNum)
		{
			Node* tmp = move->next;
			move->next = move->next->next;
			free(tmp);
			tmp = NULL;
			saveStudent(head);
			printf("删除成功\n");
			system("pause");
			system("cls");
			return ;
		}

		move = move->next;
	}
	printf("未找到学生\n");
	system("pause");
	system("cls");
}

void sortStudent(Node* head)
{
	if(head->next==NULL||head->next->next==NULL)
	{
		printf("无需排序（学生数量不足两人）\n");
		system("pause");
		system("cls");
		return ;
	}
	printf("请选择排序方式：\n");
	printf("1.按总分降序\n2.按数学升序\n3.按英语升序\n4.按计算机升序\n");
	printf("请输入你的选择：");
	char c = _getch();
	system("cls");
	Node* save = NULL;
	while(head->next !=save)
	{
		Node* move=head->next;
		while(move->next!=save)
		{
			Student* s1 = &(move->student);
			Student* s2 = &(move->next->student);
			int needSwap = 0;
			switch(c)
			{
				case '1':
					needSwap = (s1->total<s2->total);
					break;
				case '2':
					needSwap = (s1->math>s2->math);
					break;
				case '3':
					needSwap = (s1->english>s2->english);
					break;
				case '4':
					needSwap = (s1->computer>s2->computer);
					break;
				default:
					printf("无该选项，请重新输入\n");
					break;
			}
			if(needSwap)
			{
				Student temp = *s1;
				*s1 = *s2;
				*s2 = temp;
			}
			move = move->next;
		}
		save = move;
	}
	printf("排序完成,结果如下：\n");
	printStudent(head);
}

float calculateAverageScore(Node* head,const char*subject)
{
	Node* move = head ->next;
	int count = 0;
	float sum = 0;
	while(move != NULL)
	{
		if(strcmp(subject,"数学") == 0)
		{
			sum+=move->student.math;
		}else if(strcmp(subject,"英语") == 0)
		{
			sum+=move ->student.english;
		}else if(strcmp(subject,"计算机") == 0)
		{
			sum += move->student.computer;
		}else if(strcmp(subject,"总分") == 0)
		{
			sum+=move->student.total;
		}else{
			printf("未知学科！\n");
			move = move->next;
			continue;
		}
		count++;
		move=move->next;
	}
	return count == 0 ? 0:sum / count;
}

float findMaxScore(Node* head,const char* subject)
{
	Node* move = head->next;
	if(move == NULL)
	return 0;
	float max = 0;
	if(strcmp(subject,"数学") == 0)
	{
		max = move->student.math;
	}else if(strcmp(subject,"英语") == 0)
	{
		max = move->student.english;
	}else if(strcmp(subject,"计算机") == 0)
	{
		max = move->student.computer;
	}else if(strcmp(subject,"总分") == 0)
	{
		max = move->student.total;
	}else{
		printf("科目不存在！\n");
		return 0;
	}
	while(move!=NULL)
	{
		if(strcmp(subject,"数学") == 0&&move->student.math > max)
		{
			max = move->student.math;
		}else if(strcmp(subject,"英语") == 0&& move->student.english > max)
		{
			max = move->student.english;
		}else if(strcmp(subject,"计算机") == 0&& move->student.computer > max)
		{
			max = move->student.computer;
		}else if(strcmp(subject,"总分") == 0&&move->student.total > max)
		{
			max = move->student.total;
		}
		move = move->next;
	}
	return max;
}

float findMinScore(Node* head,const char* subject)
{
	Node* move = head->next;
	if(move == NULL)
	{
		printf("暂无学生信息，请录入！\n");
		return 0;
	}
	float min = 0;
	if(strcmp(subject,"数学") == 0)
	{
		min = move->student.math;
	}else if(strcmp(subject,"英语") == 0)
	{
		min = move->student.english;
	}else if(strcmp(subject,"计算机") == 0)
	{
		min = move->student.computer;
	}else if(strcmp(subject,"总分") == 0)
	{
		min = move->student.total;
	}else{
		printf("该科目不存在！\n");
		return 0;
	}
	while(move != NULL)
	{
		if(strcmp(subject,"数学") == 0&&move ->student.math < min)
		{
			min = move->student.math;
		}else if(strcmp(subject,"英语") == 0&&move->student.english < min)
		{
			min = move->student.english;
		}else if(strcmp(subject,"计算机") == 0&&move->student.computer < min)
		{
			min = move->student.computer;
		}else if(strcmp(subject,"总分") == 0&&move->student.total < min)
		{
			min = move->student.total;
		}
		move = move->next;
	}
	return min;
}

int countScoreRange(Node* head,const char*subject,float min,float max)
{
	Node* move =head->next;
	int count = 0;
	while(move!=NULL)
	{
		float score = 0;
		if(strcmp(subject,"数学") == 0)
		{
			score = move->student.math;
		}else if(strcmp(subject,"英语") == 0)
		{
			score = move->student.english;
		}else if(strcmp(subject,"计算机") == 0)
		{
			score = move ->student.computer;
		}else if(strcmp(subject,"总分") == 0)
		{
			score = move ->student.total;
		}
		if(score >= min&&score <= max)
		{
			count++;
		}
		move = move->next;
	}
	return count;
}

void analyzeStudentScore(Node* head)
{
	if(head->next == NULL)
	{
		printf("暂无学生信息，无法进行成绩分析！\n");
		system("pause");
		system("cls");
		return ;
	}
	system("cls");
	printf("============================================\n");
	printf("=                请选择分析维度            =\n");
	printf("=    1 .科目基础分析（平均分/最高分/最低分）\n");
	printf("=    2 .优秀率/及格率                       \n");
	printf("============================================\n");
	printf("请选择：");
	char choice = _getch();
	printf("\n");
	if(choice == '1')
	{
		const char*subjects[] = {"数学","英语","计算机","总分"};
		printf("========================科目基础分析结果=========================\n");
		printf("%-8s %-10s %-10s %-10s\n","科目","平均分","最高分","最低分");
		printf("=================================================================\n");
		for(int i = 0;i < 4;i++)
		{
			float avg = calculateAverageScore(head,subjects[i]);
			float max = findMaxScore(head,subjects[i]);
			float min = findMinScore(head,subjects[i]);
			printf("%-8s %-10.1f %-10.1f %-10.1f\n",subjects[i],avg,max,min);
		}
	}else if(choice == '2'){
		printf("请选择统计的科目：\n1.数学\n2.英语\n3.计算机\n4.总分\n");
		printf("请选择：");
		char subChoice = _getch();
		printf("\n");
		const char* subject = "";
		switch(subChoice)
		{
			case '1':
				subject = "数学";
				break;
			case '2':
				subject = "英语";
				break;
			case '3':
				subject = "计算机";
				break;
			case '4':
				subject = "总分";
				break;
			default:
				printf("选择错误！\n");
				system("pause");
				system("cls");
				return ;
		}
		int totalStudent = 0;
		Node* move = head->next;
		while(move!=NULL)
		{
			totalStudent++;
			move = move->next;
		}
		if(totalStudent == 0)
		{
			printf("暂无学生数据!\n");
			system("pause");
			system("cls");
			return ;
		}
		float ranges[3][2] = {{90,100},{60,100},{0,59}};
		const char* rangeName[] = {"优秀","及格","不及格"};
		int count[3] = {0};
		for(int i = 0;i < 3;i++)
		{
			count[i] = countScoreRange(head,subject,ranges[i][0],ranges[i][1]);
		}
		float excellentRate = (count[0] * 100.0f) / totalStudent;
		float passRate = (count[1] * 100.0f) / totalStudent;
		printf("======================统计结果===================\n");
		printf("总人数：%d\n",totalStudent);
		printf("%-8s %-8s %-10s\n","类别","人数","占比");
		printf("=================================================\n");
		printf("%-8s %-8d %.1f%%\n",rangeName[0],count[0],excellentRate);
		printf("%-8s %-8d %.1f%%\n",rangeName[1],count[1],passRate);
		printf("%-8s %-8d %.1f%%\n",rangeName[2],count[2],100.0f - passRate);
		printf("==================================================\n");
		printf("优秀（>=90） | 及格（>=60）\n");
	}else{
		printf("选择错误，分析取消！\n");
	}
	printf("=======================================================\n");
	system("pause");
	system("cls");
}

void quitStudent()
{
	system("cls");
	system("color 7c");
	float x, y,a;
	for(y=1.5f;y>-1;y-=0.1f)
	{
	   	for(x=-1.5f;x<1.5f;x+=.05f){
	 		a=x*x+y*y-1;
	 		putchar(a*a*a-x*x*y*y*y<0.0f?'*':' ');
	 	}
	 	Sleep(30);
	 	putchar('\n');
	}
	printf("\n\t欢迎下次使用，祝您生活愉快事业有成学业进步！！！\t\n");
	exit(0);
}