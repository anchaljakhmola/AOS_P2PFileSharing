// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include<pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <bits/stdc++.h>
#define BUFFER 2048
using namespace std;
vector<pair<string,string>> userport_username;
vector<pair<string,string>>connected_clients;
map<string,int>login_check;
map<string,vector<string>>filename_userport;
vector<pair<string,string>> alldownloads;
//this is my user class to store details of registered users;
class Registered_user{
	public:
	string user_name;
	string user_password;
	string user_port;
	//bool  user_login;
};

// for storing all registered users;
vector<Registered_user> vec_userinfo;

//class for storing group information;
class Registered_group{
	public:
	string group_id;
	string group_owner;
	string owner_port;
	vector<pair<string,string>>group_files;
	vector<string>group_users; //can do this to store all users in the group;
	vector<string>pending_users;
};

//for storing all groups;
vector<Registered_group>vec_groupinfo;

string CreateUser(vector <string>vec_input, string cp){
	for(auto u:vec_userinfo)
	{
		if(u.user_name==vec_input[1])
		{
			return "username already exists";
		}
	}
	
	Registered_user u1;
	u1.user_name=vec_input[1];
	u1.user_password=vec_input[2];
	//u1.user_login=0;
	u1.user_port=cp;
	cout<<"user "<<u1.user_name<< " registered successfully\n";
	vec_userinfo.push_back(u1);
	userport_username.push_back({cp,vec_input[1]});
	return "user registered successfully";
	//cout<<"at end port"<<u1.user_port;
}

string LoginUser(vector <string>vec_input){
	for(auto u:vec_userinfo)
	{
		if(u.user_name==vec_input[1]  && u.user_password==vec_input[2])
		{
			cout<<"user "<<u.user_name<<" is logged in now\n";
			login_check[u.user_port]=1;
			//return;
			return "user is logged in now";
		}
	}
	//cout<<"Please create an account first";
	return "Please check username/password or create an account first";
}

string CreateGroup(vector <string>vec_input,string cp){
	for(auto u:vec_groupinfo)
	{
		if(u.group_id==vec_input[1])
		{
			return "group already exists";
		}
	}

	Registered_group g1;
	g1.group_id=vec_input[1];
	//need to add more info;
	g1.owner_port=cp;
	for(auto x:userport_username)
	{
		if(x.first==cp)
		g1.group_owner=x.second;
	}
	g1.group_users.push_back(cp);
	cout<<"Group "<<g1.group_id<< " has been created\n";//<<userport_username[cp];
	vec_groupinfo.push_back(g1);
	return "Group has been created";
}


string JoinGroup(vector <string>vec_input,string cp){
for(auto it=vec_groupinfo.begin();it!=vec_groupinfo.end();it++)
	{
		if(it->group_id==vec_input[1])
		{
			for(auto u:it->pending_users)
			{
				if(u==cp)
				return "join request is already sent to the admin";
			}
			for(auto u:it->group_users)
			{
				if(u==cp)
				return "you are aleady member of this group";
			}
		}
	}
	
	//cout<<vec_groupinfo.size();
	// for(auto u:vec_groupinfo)
	// {
	// 	if(u.group_id==vec_input[1])
	// 	{
	// 		cout<<"join request is sent to the admin";
	// 		u.pending_users.push_back(cp);
	// 		//cout<<u.pending_users.size();
	// 		//u.group_users.push_back(cp);
	// 		// code to add this user to users of that group;
	// 		return;
	// 	}
	// }

	for(auto it=vec_groupinfo.begin();it!=vec_groupinfo.end();it++)
	{
		if(it->group_id==vec_input[1])
		{
			//cout<<"join request is sent to the admin\n";
			it->pending_users.push_back(cp);
			//cout<<u.pending_users.size();
			//u.group_users.push_back(cp);
			// code to add this user to users of that group;
			return "join request is sent to the admin";
		}
	}
	//cout<<"no such group exist\n";
	return "no such group exist";
}

string LeaveGroup(vector <string>vec_input,string cp){
	vector<Registered_group>::iterator p;
	int flag=0;
	for(p=vec_groupinfo.begin();p!=vec_groupinfo.end();p++)
	{
		if(p->group_id==vec_input[1])
		break;
	}
	for(auto u=vec_groupinfo.begin();u!=vec_groupinfo.end();u++)
	{
		if(u->group_id==vec_input[1] && u->owner_port!=cp)
		{
			// code to add this user to users of that group;
			//vector<string> myvector=u.group_users;
			//vector<string>::iterator it;
    		auto it = u->group_users.begin();
			while(it!=u->group_users.end())
			{
				if(*it==cp)
				{
					u->group_users.erase(it);
					//cout<<"user is removed from the group\n";
					flag=1;
					break;
				}
				it++;
			}
			if(flag==1)
			return "user is removed from the group";
		}
		else if(u->group_id==vec_input[1] && u->owner_port==cp)
		{
			//cout<<"group is deleted as admin left the group\n";
			vec_groupinfo.erase(p);
			return "group is deleted as admin left the group";
		}
	}
	//cout<<"no such group exist for this user\n";
	return "no such group exist for this user";
}

string ListPendingGroup(vector <string>vec_input,string cp){
	string s="";
	for(auto u:vec_groupinfo)
	{
		if(u.group_id==vec_input[1])
		{
			for(auto m:u.pending_users)
			{
				for(auto p:userport_username)
				{
					if(m==p.first)
					{
						//cout<<p.second<<" ";
						s=s+p.second+" ";
						break;
					}	
					//cout<<"user is added in the group";
				}
			}
			//cout<<"\n";
			if(s=="")
			return "no pending requests";
			return s;
		}
	}
	//cout<<"no such group exist";
	return "no such group exist";
}

string AcceptRequest(vector <string>vec_input,string cp)
{
	string s="";
	for(auto x:userport_username)
	{
		if(x.second==vec_input[2])
		{
			s=x.first;
			break;
		}
	}
	for(auto u=vec_groupinfo.begin();u!=vec_groupinfo.end();u++)
	{
		if(u->group_id==vec_input[1] && u->owner_port==cp)
		{
			for(auto it=u->pending_users.begin();it!=u->pending_users.end();it++)
			{
				if(*it==s)
				{
					u->pending_users.erase(it);
					u->group_users.push_back(*it);
					return "user is added to group";
				}
			}
			return "user is not present in group";
		}
	}
	return "no such group exist for user";
}

string ListGroups(vector <string>vec_input,string cp){
	string s="";
	for(auto u:vec_groupinfo)
	{
		for(auto m:u.group_users)
		{
			if(m==cp)
			{
			//cout<<"user is logged in now";
				// for(auto it = userport_username.begin(); it!=userport_username.end(); it++ ){ 
				// 	if(it->first == cp)
					{s=s+u.group_id+" ";}
				// }
			//return;
			}
		}
	}
	//cout<<"Please create an account first";
	if(s=="")
	return "you are not in network of group";
	else
	return s;
}
///YE ISLIYE H KI SB GROUPS SHOW KREGA OR UPR WALA USER SPECIFIC
string ListGroups1(vector <string>vec_input,string cp){
	string s="";
	for(auto u:vec_groupinfo)
	{
			//cout<<"user is logged in now";
				// for(auto it = userport_username.begin(); it!=userport_username.end(); it++ ){ 
				// 	if(it->first == cp)
		s=s+u.group_id+" ";
				// }
			//return;
	}
	//cout<<"Please create an account first";
	if(s=="")
	return "you are not in network of group";
	else
	return s;
}

string UploadFile(vector <string>vec_input,string cp){
	int flag=0;
	for(auto it=vec_groupinfo.begin();it!=vec_groupinfo.end();it++)
	{
		if(it->group_id==vec_input[2])
		{
			for(auto u:it->group_users)
			{
				if(u==cp)
				{
					flag=1;
					break;
				}
			}
		}
	}

	if(flag!=1)
		return "user is not registered in group so cannot upload file";
	if(access( vec_input[1].c_str(), F_OK ) == -1)
	return "no file  exist at given path";

	string fn="",st=vec_input[1],final="";
	int i=vec_input[1].size()-1;
	for(i;i>=0;i--)
	{
		if(st[i]=='/' || st[i]==' ')
		break;
		final=st[i]+final;
	}
	for(auto u=vec_groupinfo.begin();u!=vec_groupinfo.end();u++)
	{
		if(u->group_id==vec_input[2])
		{
			//cout<<"user is logged in now";
			u->group_files.push_back({vec_input[1],final});
			//return;
			filename_userport[final].push_back(cp);
			return "file is uploaded";
		}
	}
	//cout<<"Please create an account first";
	return "can not upload or no such path exist";
}

string DownloadFile(vector <string>vec_input,string cp){
	int flag=0;
	for(auto it=vec_groupinfo.begin();it!=vec_groupinfo.end();it++)
	{
		if(it->group_id==vec_input[1])
		{
			for(auto u:it->group_users)
			{
				if(u==cp)
				{
					flag=1;
					break;
				}
			}
		}
	}
	if(flag!=1)
		return "user is not registered in group so cannot download file";
	
	string c=vec_input[2];
	string d=vec_input[1];
	string s="";
	for(auto m:filename_userport[c])
	s=s+m+" ";
	if(s=="")
		return "no such file in group";
	alldownloads.push_back({d,c});
	return s;
	}

string ListFiles(vector <string>vec_input,string cp){
	string s="";
	for(auto u:vec_groupinfo)
	{
		if(u.group_id==vec_input[1])
		for(auto m:u.group_files)
		{
			//cout<<"user is logged in now";
				// for(auto it = userport_username.begin(); it!=userport_username.end(); it++ ){ 
				// 	if(it->first == cp)
					{s=s+m.second+" ";}
				// }
			//return;
		}
	}
	//cout<<"Please create an account first";
	if(s=="")
	return "no files in group";
	else
	return s;
}


string Logout(vector <string>vec_input,string cp){
	for(auto u:vec_userinfo)
	{
		if(u.user_port==cp)
		{
			//cout<<"user is logged in now";
			login_check[u.user_port]=0;
			//return;
			return "user is logged out now";
		}
	}
	//cout<<"Please create an account first";
	return "you are not logged in";
}

void* myfunction(void* args){
	char buffer[BUFFER];

    int client_socket = *((int*)args);
	read( client_socket , buffer, BUFFER);
	string inp_start=buffer;
	string client_ip;
	string client_port;
	//cout<<inp_start<<"\n";
	int i;
	for(i=0;i<inp_start.length();i++)
	{
		if(inp_start[i]==':')
		{
			client_ip=inp_start.substr(0,i);
			client_port=inp_start.substr(i+1);
			//cout<<client_ip<<"\n"<<client_port<<"\n";
			//connected_clients.push_back({client_port,client_ip});
		}
	}

	
	while(1){
	memset(buffer,0,BUFFER);
	read( client_socket , buffer, BUFFER);
	string inp=buffer;
	string buf;
	string word;
	vector<string> vec_input;
	istringstream ss(inp);
	 while (ss >> word) 
    {
        vec_input.push_back(word);
    }
	cout<<"My input command is: "<<vec_input[0]<<"\n";
	if(vec_input[0]=="create_user")
	{
		if(vec_input.size()!=3)
			buf="Invalid no. of arguments";
		else
			buf=CreateUser(vec_input,client_port);
		//"user_is_registered";
	}
	
	else if(vec_input[0]=="login")
	{
		if(vec_input.size()!=3)
			buf="Invalid no. of arguments";
		else
			buf=LoginUser(vec_input);
	}

	else if(vec_input[0]=="create_group" && login_check[client_port]==1)
	{
		if(vec_input.size()!=2)
			buf="Invalid no. of arguments";
		else
			buf=CreateGroup(vec_input,client_port);
	}
	
	else if(vec_input[0]=="join_group" && login_check[client_port]==1)
		{
		if(vec_input.size()!=2)
			buf="Invalid no. of arguments";
		else
			buf=JoinGroup(vec_input,client_port);
			// for(auto u:vec_groupinfo)
			// cout<<u.pending_users.size();
			// cout<<"no";
		}
	
	else if(vec_input[0]=="leave_group" && login_check[client_port]==1)
		{
		if(vec_input.size()!=2)
			buf="Invalid no. of arguments";
		else
			buf=LeaveGroup(vec_input,client_port);
		}

	else if(vec_input[0]=="list_requests" && login_check[client_port]==1)
		{
		if(vec_input.size()!=2)
			buf="Invalid no. of arguments";
		else
			buf=ListPendingGroup(vec_input,client_port);
		}

	else if(vec_input[0]=="accept_request" && login_check[client_port]==1)
		{
		if(vec_input.size()!=3)
			buf="Invalid no. of arguments";
		else
			buf=AcceptRequest(vec_input,client_port);
		}
	else if(vec_input[0]=="list_groups" && login_check[client_port]==1)
		{
		if(vec_input.size()!=1)
			buf="Invalid no. of arguments";
		else
			buf=ListGroups1(vec_input,client_port);
			//ye abi all groups ka h user specific k liye ListGroups fn hai
		}	
		
		else if(vec_input[0]=="list_groups_me" && login_check[client_port]==1)
		{
		if(vec_input.size()!=1)
			buf="Invalid no. of arguments";
		else
			buf=ListGroups(vec_input,client_port);
		}	

	else if(vec_input[0]=="upload_file" && login_check[client_port]==1)
		{
		if(vec_input.size()!=3)
			buf="Invalid no. of arguments";
		else
			buf=UploadFile(vec_input,client_port);
		}

	else if(vec_input[0]=="download_file" && login_check[client_port]==1)
		{
		if(vec_input.size()!=4)
			buf="Invalid no. of arguments";
		else
			buf=DownloadFile(vec_input,client_port);
		}

	else if(vec_input[0]=="list_files" && login_check[client_port]==1)
		{
		if(vec_input.size()!=2)
			buf="Invalid no. of arguments";
		else
			buf=ListFiles(vec_input,client_port);
		}

	else if(vec_input[0]=="logout" && login_check[client_port]==1)
		{
		if(vec_input.size()!=1)
			buf="Invalid no. of arguments";
		else
			buf=Logout(vec_input,client_port);
		}	
		
		else if(vec_input[0]=="show_downloads" && login_check[client_port]==1)
		{
			string s="";
			if(alldownloads.size()==0)
				buf="no downloads done";
			else
			{
				for(auto u:alldownloads)
				{
				s=s+ "C"+" " + u.first + " " +u.second+"\n";
				}
				buf=s;
			}
		}	
		
	else if(vec_input[0]=="stop_share" && login_check[client_port]==1)
		{
		if(vec_input.size()!=3)
			buf="Invalid no. of arguments";
		else
			buf="stopped share";
		}	
		
		
	else if(vec_input[0]=="exit")
		{
			buf="sockclosed";
			send(client_socket , buf.c_str() , buf.size() , 0 );
			shutdown(client_socket,SHUT_RDWR);
			close(client_socket);
			pthread_exit(NULL);
		}
	else
		{
			//cout<<"Invalid command or you are not logged in\n";
			buf="Either Invalid command or you are not logged in\n";
		}
	fflush(stdout);
	send(client_socket , buf.c_str() , buf.size() , 0 );
	//printf("\ntask completed\n");
	}
	close(client_socket);
	pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFFER] = {0};
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	// This is for reusing socket, you will not get frequest bind error after this
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8888);
	
	// Forcefully attaching socket to the port 8888
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
	//char buffer[BUFFER];
    //int client_socket = *((int*)args);
	
		pthread_t pid;
		pthread_create(&pid,NULL,myfunction,(void *)&new_socket);
	}
	close(server_fd);
	return 0;
}

