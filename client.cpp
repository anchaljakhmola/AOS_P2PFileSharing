// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>
#define BUFFER 2048
using namespace std;
string myport;
map<string,string>filename_path;

void* download_function(void* args){
	char buffer[BUFFER];
    int client_socket = *((int*)args);
	//read( client_socket , buffer, BUFFER);
// while(1){
		memset(buffer,0,BUFFER);
		read( client_socket , buffer, BUFFER);
        printf("%s\n",buffer );
//////////////////////
//char *filename = buffer;
string st= filename_path[buffer];
cout<<st<<endl;
FILE *fp = fopen(st.c_str(),"r");
// fscanf(f,"%s",buff);
// write(client_socket,buff,100);
while(1)
 {
     unsigned char buff[256]={0};
 int nread = fread(buff,1,256,fp);
 printf("Bytes read %d \n", nread);
 if(nread > 0)
 {
 printf("Sending \n");
 write(client_socket, buff, nread);
 }
 else
 {
     write(client_socket, buff, nread);
      fclose(fp);
     break;
 }
 }
//////////////////////
		fflush(stdout);
		//send(client_socket , buffer , BUFFER , 0 );
		printf("file sent\n");
	// }
	// close(client_socket);
	//
}

int act_as_sender(){
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFFER] = {0};
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(myport.c_str()));
	
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
			{
				perror("bind failed");
				exit(EXIT_FAILURE);
			}
	if (listen(server_fd, 3) < 0)
	{
		perror("------server is not listening----------");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
	
		pthread_t pid;
		pthread_create(&pid,NULL,download_function,(void *)&new_socket);
	}
	close(server_fd);
	return 0;
}


int Download(vector <string>vec_input,string reciever_port)
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	int x=atoi(reciever_port.c_str());
	serv_addr.sin_port = htons(x);
	//serv_addr.sin_port=htons(argv[1]);

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	
	
	//string s=argv[1];
	//cout<<s<<"\n";
	string s="starting download...";
	cout<<s<<"\n";
	s=vec_input[2];
	int i;
    for (i = 0; i < s.size(); i++)
         buffer[i] = s[i];
	send(sock , buffer , BUFFER , 0 );

	///////////////////////////
	char recvBuff[256];
	int bytesReceived = 0;
 memset(recvBuff, '0', sizeof(recvBuff));
	FILE *fp;
	string str=vec_input[3]+"/"+vec_input[2];
 fp = fopen(str.c_str(),"w");
 while(1)
 {
	 bytesReceived = read(sock, recvBuff, 256);
	 if(bytesReceived > 0)
 	{
		 printf("Bytes received %d\n",bytesReceived);
		//cout<<recvBuff<<endl;
 		fwrite(recvBuff, 1,bytesReceived,fp);
		break;
	}
	else
	{
	shutdown(sock, SHUT_WR);
	close(sock);
	break;
	}
 // printf("%s \n", recvBuff);
 }
	return 0;

}


int main(int argc, char *argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8888);
	//serv_addr.sin_port=htons(argv[1]);
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	string s=argv[1];
	cout<<s<<"\n";
	int i;
    for (i = 0; i < s.size(); i++)
        buffer[i] = s[i];
	send(sock , buffer , BUFFER , 0 );
	//send(sock,s,BUFFER,0);
		thread t1(act_as_sender);
	string inp_start=buffer;
	for(i=0;i<inp_start.length();i++)
	{
	if(inp_start[i]==':') 
		{
			myport=inp_start.substr(i+1);
		}
	}
	
	while(1)
	{
		memset(buffer,0,BUFFER);
		fgets(buffer,BUFFER,stdin);
		send(sock , buffer , BUFFER , 0 );
		
		string inp=buffer;
	    string word;
	    vector<string> vec_input;
	    istringstream ss(inp);
	    while (ss >> word) 
        {
        vec_input.push_back(word);
        }
		
		memset(buffer,0,BUFFER);
		read( sock , buffer, BUFFER);
		cout<<buffer<<"\n";
		
		if(vec_input[0]=="download_file" && buffer[0]!='u' && buffer[0]!='n' && buffer[0]!='I')
        {
			string inp1=buffer;
			string word1;
			vector<string> vec_input1;
			istringstream ss(inp1);
			while (ss >> word1) 
			{
			vec_input1.push_back(word1);
			}
			string p=vec_input1[0];
            int m=Download(vec_input,p);
        }
		if(vec_input[0]=="upload_file" && buffer[0]=='f')
        {
            string st=vec_input[1],final="";
	        int i=vec_input[1].size()-1;
	        for(i;i>0;i--)
	        {
	        	if(st[i]=='/' || st[i]==' ')
		            break;
		        final=st[i]+final;
	        }
            filename_path[final]=vec_input[1];
            cout<<filename_path[final]; 
			//cout<<"hello";
        }
		if(buffer=="sockclosed")
		break;
		//printf("above command execution completed\n");
		fflush(stdout);
	}
	shutdown(sock,SHUT_RDWR);
	close(sock);
	return 0;
}


