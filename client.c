#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

int	Port;
//#define URL ""
// #define URL ""
// #define URL ""
#define URL "http://localhost"
#define ADDR_SIZE 125
#define NAME_SIZE 125
#define ADM_SIZE  10
#define JIBUN_SIZE 12
#define NAME_SIZE 125
#define TEL_SIZE 15
#define COORD_LEN 17
#define CATE_SIZE 7
#define PATH_SIZE 125

char *qURLencode(char *str) {
	char *encstr, buf[2+1];
	unsigned char c;
	int i, j;

	if(str == NULL) return NULL;
	if((encstr = (char *)malloc((strlen(str) * 3) + 1)) == NULL) return NULL;
	memset(encstr, 0, (strlen(str) * 3) + 1) ;

	for(i = j = 0; str[i]; i++) {
		c = (unsigned char)str[i];

		if((c >= '0') && (c <= '9')) {
			encstr[j++] = c;
		} else if((c >= 'A') && (c <= 'Z')) {
			encstr[j++] = c;
		} else if((c >= 'a') && (c <= 'z')) {
			encstr[j++] = c;
		} else if((c == '@') || (c == '.') || (c == '/') || (c == '-') || (c == '_') || (c == ':') )  {
			encstr[j++] = c;
		} else {
			memset(buf, 0, sizeof(buf)) ;
			sprintf(buf, "%02x", c);
			encstr[j++] = '%';
			encstr[j++] = buf[0];
			encstr[j++] = buf[1];
		}
	}

	return encstr;
}


void pr_times(clock_t real, struct tms *b_tms, struct tms *e_tms, int succ, int fail, int total)
{
	static long clktck = 0;

	if(clktck == 0)
		if((clktck = sysconf(_SC_CLK_TCK)) < 0) {
			printf(" sysconf error");
			return;
		}

	printf( "total: %d, succ:%d, fail:%d\n", total, succ, fail);
	printf( "[%7.3f]\n", real / (double) clktck);
	printf( "[%7.3f]\n", (e_tms->tms_utime - b_tms->tms_utime) / (double) clktck);
}

void InputInt(const char* prompt, int* variable, const int def_val)
{
	char buf[128];
	printf("%s[%d]=", prompt, def_val);

	fgets(buf, 128, stdin);

	if (strcmp(buf, "\n")==0) *variable=def_val;
	else *variable = atoi(buf);
}

void InputString(const char* prompt, char* variable, const int length, const char* def_val)
{           
	if((int)strlen(def_val)>=length) { 
		fprintf(stderr, "length(%d) of variable (%s) is too long than length(%d)\n", (int)strlen(def_val), def_val, length);
		exit(1);
	}           

	printf("%s[%s]=", prompt, def_val);
	fgets(variable, length+1, stdin);

	if (variable[strlen((char*)variable)-1]=='\n') 
		variable[strlen(variable)-1]='\0';

	if (strcmp(variable, "")==0)
		strcpy(variable, def_val);
}               

void client_request( char *request)
{
	CURL *curl;
	CURLcode res;
	clock_t s_clock, e_clock;
	struct tms s_tms, e_tms;
	int succ = 0, fail = 0;

	curl = curl_easy_init();

	if(curl)
	{
		printf("req:%s\n", request);
		curl_easy_setopt(curl, CURLOPT_URL, request);

		s_clock = times(&s_tms);

		if((res = curl_easy_perform(curl)) != CURLE_OK)	{
			printf("Err..(%d)\n", res);
		}

		e_clock = times(&e_tms);

		curl_easy_cleanup(curl);

	}
	pr_times(e_clock - s_clock, &s_tms, &e_tms, succ, fail, 1);
}

int main(int argc, char *argv[])
{
	int  Mode;
	int  FetchCount;
	int  StartPosition;
	char Address[ADDR_SIZE+1];
	char request[1024];
	char RoadName[NAME_SIZE+1];
	char AdmCode[ADM_SIZE+1];
	char Jibun[JIBUN_SIZE+1];
	char CateCode[CATE_SIZE+1];
	char xpos[COORD_LEN+1];
	char ypos[COORD_LEN+1];
	char xpos1[COORD_LEN+1];
	char ypos1[COORD_LEN+1];
	char query[NAME_SIZE+1];
	char tel[TEL_SIZE+1];
	char icode[TEL_SIZE+1];
	char Source[15];
	char ConfigPath[PATH_SIZE+1];	
	char *buff = NULL;

	int Depth, nameopt, admopt, SpOpt, Radius, sortopt, Bcode, Level, mid, opt;
	int cutflag, CoordType;

	if(argc == 1)
		Port = ##;
	else if(argc == 2)	{
		if((Port = atoi(argv[1])) <= 0)
			Port = ##;
	}
	else	{
		printf("Usage : %s Port[default:##]\n", argv[0]);
		exit(-1);
	}

	curl_global_init(CURL_GLOBAL_ALL);

	while(1)    {
		printf("===============================\n");
		printf("1.OA\n");
		printf("2.NA \n") ;
		printf("3.SubAdm \n");
		printf("4.RoadList \n");
		printf("5.RoadJibun\n");
		printf("6.BL\n");
		printf("7.POIT_NA \n");
		printf("8.SQE  \n");
		printf("9.MULTI_YPI \n");
		printf("10.New SRDP_NA\n") ;
		printf("11.RSPOI \n");
		printf("12.ICODE \n");
		printf("13.Neibor \n") ;
		printf("14.KAKAOPOI \n") ;
		printf("15.CONVICODE \n") ;
		printf("17.REF_NEIGHBOR \n") ;
		printf("19.REVERSE_GEOCODING \n") ;
		printf("100.OIL UPDATE\n");
		printf("900.MANAGEMENT\n");
		printf("901.INDEX UPDATE\n");
		printf("0.exit\n");
		printf("===============================\n");
		InputInt("Enter Service", &Mode, 0) ;

		if(Mode == 0) return (1) ;

		Depth = 0 , nameopt = 0, admopt = 0, SpOpt = 0, Radius = 0;
		sortopt = 0, Bcode = 0, Level = 0, mid = 0, opt = 0, cutflag = 0, CoordType = 0;

		memset(query, 0x00, sizeof(query));
		memset(request, 0x00, sizeof(request));
		memset(Address, 0x00, sizeof(Address));
		memset(RoadName, 0x00, sizeof(RoadName));
		memset(xpos, 0x00, sizeof(xpos));
		memset(ypos, 0x00, sizeof(ypos));
		memset(xpos1, 0x00, sizeof(xpos1));
		memset(ypos1, 0x00, sizeof(ypos1));
		memset(tel, 0x00, sizeof(tel));
		memset(CateCode, 0x00, sizeof(CateCode));
		memset(AdmCode, 0x00, sizeof(AdmCode));
		memset(icode, 0x00, sizeof(icode));
		memset(Jibun, 0x00, sizeof(Jibun));

		InputString("EnterSource", Source, 14, "SERVER");
		InputInt("CutFlag[0:Non,1:Cut]", &cutflag, 0);
		InputInt("CoordType[0:정규화,1:WGS84]", &CoordType, 0);
		InputInt("StartPosition", &StartPosition, 0);
		InputInt("FetchCount", &FetchCount, 10);


		switch (Mode)   {
			case 1 :
				InputInt("Option[0:admcode+jibun / 1:addr+jibun]", &opt, 3);

				if( opt == 1)
				{
					InputString("Enter Address", Address, ADDR_SIZE+1, "남항동");
					buff = qURLencode(Address);
					memset(Address, 0x00, sizeof(Address));
					strncpy(Address, buff, strlen(buff));
					free(buff);
				} else {
					InputString("Enter AdmCode", AdmCode, ADM_SIZE+1, "11680");
				}

				InputString("Enter Jibun", Jibun, JIBUN_SIZE+1, " ");

				printf("Enter Mode[1:Law (if Not Law return Adm), 2:Law 3:Law+Adm] ::\n");
				InputInt("Mode", &Mode, 1);
				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, opt,  Address, AdmCode, Jibun, Mode);
				break;

			case 2 :
				InputInt("Mode[1:admcode + addr ,2:addr", &Mode, 2) ;
				if( Mode == 2 )
					InputString("Enter Address", Address, ADDR_SIZE+1, "삼성동");
				else
					InputString("Enter AdmCode", Address, ADDR_SIZE+1, "11680");

				InputString("Enter Jibun", Jibun, JIBUN_SIZE+1, " ");

				buff = qURLencode(Address);
                memset(Address, 0x00, sizeof(Address));
                strncpy(Address, buff, strlen(buff));
                free(buff);
				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, Mode, Address, Address, Jibun);
				break;
			case 3 :
				InputInt("Mode[1:admcode, 2:addr, 3.admcode->addr", &Mode, 1) ;

				if( Mode == 2 )
				{
					InputString("Enter Address", Address, ADDR_SIZE+1, "강남구");
					buff = qURLencode(Address);
					memset(Address, 0x00, sizeof(Address));
					strncpy(Address, buff, strlen(buff));
					free(buff);
				}
				else
					InputString("Enter AdmCode", Address, ADDR_SIZE+1, "11680");

				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, Mode, Address);
				break;
			case 4 :
				InputString("AdmCode", AdmCode, ADM_SIZE+1, "1141011700");
				InputString("Enter Jibun", Jibun, JIBUN_SIZE, "77-25");
				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount,AdmCode, Jibun);
				break;
			case 5 :
				InputString("Enter query", Address, ADDR_SIZE +1, "양재동+275-7");
				buff = qURLencode(Address);
				memset(Address, 0x00, sizeof(Address));
				strncpy(Address, buff, strlen(buff));
				free(buff);
				InputInt("Mode[1,2,3]", &Mode, 1);
				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, Address, Mode);
			
				break;
			case 6 :
				InputInt("BCODE", &Bcode, -1) ;
				InputInt("Level", &Level, -1) ;
				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, Bcode, Level);
				break;
			case 7 :
				InputInt("Depth",&Depth,2);
				InputString("Enter query", query, NAME_SIZE+1, "팅크웨어");
				buff = qURLencode(query);
				memset(query, 0x00, sizeof(query));
				strncpy(query, buff, strlen(buff));
				free(buff);
				InputInt("Option[1:admcode,2:ragne,4:bcode,8:name]",&opt,8);
				InputInt("NameOption",&nameopt,7);
				InputInt("AdmOption",&admopt,0);
				if( admopt == 1 )
					InputString("Enter AdmCode", AdmCode, ADM_SIZE+1, "11");
				else if( admopt == 2 || admopt == 3 )
					InputString("Enter AdmName", AdmCode, ADM_SIZE+1, "서울");

				InputString("Enter CATECODE", CateCode, CATE_SIZE+1, "");
				InputString("Enter Tel", tel, TEL_SIZE+1, "");
				InputInt("SpOption[0:Non,1:Extent,2:K-NN, 3:Range]", &SpOpt, 0 );
				InputString("Enter XPos1", xpos, COORD_LEN+1, "169174");
				InputString("Enter YPos1", ypos, COORD_LEN+1, "531035");

				if( SpOpt == 0 ) {
					InputInt("Radius", &Radius, 0 );
				} else if( SpOpt == 1 ) {
					InputString("Enter XPos2", xpos1, COORD_LEN+1, "174174");
					InputString("Enter YPos2", ypos1, COORD_LEN+1, "536035");

				} else if( SpOpt == 2 ) {
					InputInt("Enter Mid", &mid, 4717891);
				} 

				InputInt("SortOpt[1:Name,2:dis,3:NAME->dis, 4:Weight 5:Weight+length]", &sortopt, 4);

				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount,Depth, query, opt, nameopt, admopt, AdmCode, CateCode, tel, SpOpt, Radius, xpos, ypos, xpos1, ypos1, mid, sortopt );
				break;
			case 8:
				InputInt("Depth",&Depth,2);
				InputInt("Mode[1:poi검색,2:adm검색,3:poi+adm검색]", &Mode, 1);
				InputString("Enter query", query, NAME_SIZE+1, "팅크웨어");
				buff = qURLencode(query);
				memset(query, 0x00, sizeof(query));
				strncpy(query, buff, strlen(buff));
				printf("buff:%s %s %d\n", buff, query, strlen(query));
				free(buff);

				InputInt("SpOption[0:Non,1:Extent,2:K-NN, 3:Range]", &SpOpt, 0 );

				if( SpOpt != 0 )
				{
					InputString("Enter XPos1", xpos, COORD_LEN+1, "169174");
					InputString("Enter YPos1", ypos, COORD_LEN+1, "531035");
				}

				InputString("Enter AdmCode", AdmCode, ADM_SIZE+1, "");
				InputString("Enter CateCode", CateCode, CATE_SIZE+1, "");
				InputInt("SortOpt[1:Name,2:dis,3:NAME->dis, 4:Weight 5:Weight+length]", &sortopt, 4);

				if( SpOpt == 3 ) {
					InputInt("Radius", &Radius, 0 );
				} else if( SpOpt == 1 ) {
					InputString("Enter XPos2", xpos1, COORD_LEN+1, "174174");
					InputString("Enter YPos2", ypos1, COORD_LEN+1, "536035");
				}

				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, Depth, Mode, Radius, SpOpt, sortopt, query, xpos, ypos, xpos1,ypos1,AdmCode, CateCode);
				break;
			case 9:
				InputInt("MID", &mid, 1240659);
				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, mid);
				break;
			case 10:
				InputInt("MID", &mid, 1240659);
				InputString("Enter XPos", xpos, COORD_LEN+1, "169174");
				InputString("Enter YPos", ypos, COORD_LEN+1, "531035");

				sprintf(request, "", URL, Port, Source, cutflag, CoordType, StartPosition, FetchCount, mid, xpos, ypos);
				break;
		}

		client_request(request);
	}

	curl_global_cleanup();
}
