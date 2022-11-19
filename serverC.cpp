// using g++ -o serverC -std=c++11 serverC.cpp
# include <iostream>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sstream>
# include <vector>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <netdb.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include <iostream>
# include <fstream>
# include <string>
# include <set>
# include <vector>
# include <sstream>
# include <map>

using namespace std;

const std::string LOCAL_IP = "127.0.0.1";
const std::string INPUT = "cred.txt";
const int SERVER_C = 21060;
const int SERVER_CS = 22060;
const int SERVER_EE = 23060;
const int UDP_MAIN_SERVER = 24060;
const int TCP_PORT = 25060;
const int MAXBUFLEN = 4096;

vector<string> convert_string_to_vector(string input_string)
{
	vector<string> result;
	if (input_string == "") return result;
	string token;
	stringstream ss(input_string);
	while (getline(ss, token, ',')) {
		result.push_back(token);
	}

	return result;
}

void create_UDP(int &sockfd_UDP, sockaddr_in &serverAddr_UDP, int port, string ip) 
{ 
	sockfd_UDP = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr_UDP, '\0', sizeof(serverAddr_UDP));

    serverAddr_UDP.sin_family = AF_INET;
    serverAddr_UDP.sin_port = htons(port);
    serverAddr_UDP.sin_addr.s_addr = inet_addr(ip.c_str());
}

string UDP_send_receive(int &sockfd_UDP, sockaddr_in &serverAddr_UDP, string message)
{
	sendto(sockfd_UDP, message.c_str(), message.size() + 1, 0, (struct sockaddr*)&serverAddr_UDP, sizeof(serverAddr_UDP));
	cout << "[+] <" << UDP_MAIN_SERVER << "> Sending to UDP <" << inet_ntoa(serverAddr_UDP.sin_addr) << ": " << ntohs(serverAddr_UDP.sin_port) 	
		 << "> : " <<  message << endl << endl;
    socklen_t  serverAddr_UDP_length = sizeof(serverAddr_UDP);

	char buffer[MAXBUFLEN];
	memset(&buffer, '\0', sizeof(buffer));
	
	recvfrom(sockfd_UDP, buffer, MAXBUFLEN, 0, (sockaddr*) &serverAddr_UDP, &serverAddr_UDP_length);
	cout << "[+] <" << UDP_MAIN_SERVER << "> Receiving from UDP <" << inet_ntoa(serverAddr_UDP.sin_addr) << ": " << ntohs(serverAddr_UDP.sin_port) 	
		 << "> : " <<  string(buffer) << endl << endl;
	
	return string(buffer);
}

int main()
{
    map<string, string> cred_map;

    ifstream ins(INPUT);

    // read data from the txt file
    string line;
    while(getline(ins, line)) {
        stringstream ss(line);
        string username, password;
        getline(ss, username, ',');
        ss >> password;
        cred_map.insert(make_pair(username, password));
    }

    // for(map<string,string>::iterator itr=cred_map.begin();itr!=cred_map.end();itr++){
    //     cout<<"Username: "<<itr->first<<" Password: "<<itr->second<<endl;
    // }

    int sockfd;
    sockaddr_in serverAddr, clientAddr;
    char buffer[MAXBUFLEN];
    socklen_t addr_size;
    string message = "null";

    create_UDP(sockfd, serverAddr, SERVER_C, LOCAL_IP);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))){
        cout << SERVER_C << " Error in binding." << endl;
		return -1;
    }

    cout << "The ServerC is up and running using UDP on port " << SERVER_C << endl;

    while (true) {

        memset(&buffer, '\0', sizeof(buffer));
        memset(&clientAddr, '\0', sizeof(clientAddr));
        
        socklen_t  siaddr_size = sizeof(clientAddr);

        recvfrom(sockfd, buffer, MAXBUFLEN, 0, (sockaddr*) &clientAddr, &siaddr_size);

        cout << "The ServerC received an authentication request from the Main Server." << endl;
        vector<string> message_list = convert_string_to_vector(string(buffer));
        string username = message_list[0];
        string password = message_list[1];
        // cout << "Username: " << username << endl;
        // cout << "Password: " << password << endl;

        // Figure out the correct message
        auto iter = cred_map.find(username);
        if (iter == cred_map.end()) {
            message = "FAIL_NO_USER";
        } else {         
            if (iter->second.compare(password) != 0) {
                message = "FAIL_PASS_NO_MATCH";
            } else {
                message = "PASS";
            }
        }

        sendto(sockfd, message.c_str(), message.size() + 1, 0, (sockaddr*) &clientAddr, sizeof(clientAddr));
        cout << "The ServerC finished sending the response to the Main Server." << endl;

    }

    close(sockfd);
    
    return 0;

}