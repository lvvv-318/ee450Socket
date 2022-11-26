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
const std::string INPUT = "cs.txt";
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

int main() 
{
    map<string, vector<string>> cs_map;

    ifstream ins(INPUT);

    // read data from the txt file
    string line;
    while(getline(ins, line)) {
        stringstream ss(line);
        string col1, col2, col3, col4, col5;  
        getline(ss, col1, ',');
        getline(ss, col2, ',');
        getline(ss, col3, ',');
        getline(ss, col4, ',');
        getline(ss, col5);
        vector<string> values;
        values.push_back(col2);
        values.push_back(col3);
        values.push_back(col4);
        values.push_back(col5);
        cs_map.insert(make_pair(col1, values));
    }

    int sockfd;
    sockaddr_in serverAddr, clientAddr;
    char buffer[MAXBUFLEN];
    socklen_t addr_size;
    string message = "null";

    create_UDP(sockfd, serverAddr, SERVER_CS, LOCAL_IP);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))){
        cout << SERVER_CS << " Error in binding." << endl;
		return -1;
    }

    cout << "The ServerCS is up and running using UDP on port " << SERVER_CS << endl;

    while (true) {

        memset(&buffer, '\0', sizeof(buffer));
        memset(&clientAddr, '\0', sizeof(clientAddr));
        
        socklen_t  siaddr_size = sizeof(clientAddr);

        recvfrom(sockfd, buffer, MAXBUFLEN, 0, (sockaddr*) &clientAddr, &siaddr_size);

        vector<string> message_list = convert_string_to_vector(string(buffer));
        string courseCode = message_list[0];
        string category = message_list[1];
        cout << "The ServerCS received a request from the Main Server about the " <<
                category << " of " << courseCode << "." << endl;

        auto iter = cs_map.find(courseCode);
        if (iter == cs_map.end()) {
            message = "NOT_FOUND";
            cout << "Didn't find the course: " << courseCode << endl;
        } else {
            if (category.compare("Credit") == 0) {
                message = iter->second[0];
            } else if (category.compare("Professor") == 0) {
                message = iter->second[1];
            } else if (category.compare("Days") == 0) {
                message = iter->second[2];
            } else if (category.compare("CourseName") == 0) {
                message = iter->second[3];
            }
            cout << "The course information has been founded: The " << category <<
                    " of " << courseCode << " is " << message << endl;
        }
        sendto(sockfd, message.c_str(), message.size() + 1, 0, (sockaddr*) &clientAddr, sizeof(clientAddr));
        cout << "The ServerCS finished sending the response to the Main Server." << endl;
    }
    
    return 0;

}