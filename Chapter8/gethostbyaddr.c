#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
void error_handling(char *message);

/*
gethostbyaddr()

struct hostent * gethostbyaddr(const char * addr, socklen_t len, int family);
- addr : IP주소를 지니는 in_addr 구조체 변수의 포인터 전달 
	 * IPv4 이외 다양한 정보를 전달받도록 일반화 하기 위해 매개변수를 char형 포인터로 선언 
- len : 첫 번째 인자로 전달된 주소 정보의 길이, IPv4 : 4, IPv6 : 16 전달
- family : 주소체계 정보 전달.
	IPv4: AF_INET
	IPv6 : AF_INET6
*/
int main(int argc, char *argv[])
{
	int i;
	struct hostent *host;
	/*
		struct hostent{
			char *h_name; // 공식 도메인 이름
			char **h_aliase; // 별칭 도메인 이름
			int h_addrtype; // 반환된 IP 정보, IPv4 : AF_INET 
			int h_length; // 반환된 IP 정보의 크기 
			char ** h_addr_list; // IP의 주소 정보(둘 이상일 경우 반환) 
	*/
	struct sockaddr_in addr;
	if(argc!=2) {
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	// 주소 정보 할당 
	memset(&addr, 0, sizeof(addr));

	// 문자열 주소 정보를 네트워크 바이트 순서의 정수로 변환한다.
	addr.sin_addr.s_addr=inet_addr(argv[1]);

	// gethostbyaddr 함수 사용 
	// 위에서 addr 구조체 변수 addr의 sin_addr 주소정보를 인자로 전달 
	host=gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	if(!host)
		error_handling("gethost... error");

	printf("Official name: %s \n", host->h_name);

	for(i=0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i+1, host->h_aliases[i]);
	
	printf("Address type: %s \n", 
		(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");

	// inet_ntoa() : 네트워크 바이트 순서 IP주소를 문자열 정보로 변환한다.
	for(i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1,
					inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}