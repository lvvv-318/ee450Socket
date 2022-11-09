# include <iostream>
# include <sys/types.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <string.h>
# include <string>
# include <netinet/in.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <sstream>
#include <climits>
#include <string>

using namespace std;

const std::string LOCAL_IP = "127.0.0.1";
const int TCP_PORT = 25060;
const int MAXBUFLEN = 4096;

struct UserInfo {
    string username;
    string password;
};

vector<string> convert_string_to_vector(string input_string) {
    vector<string> result;
    istringstream ss(input_string);

    do {
        string word;
        ss >> word;
        if (word == "") continue;
        result.push_back(word);
    }while (ss);

    return result;
}

bool setupTCP(int &server_socket, sockaddr_in &serverAddr, int port, string IP)
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cout << "Failed to create server socket.\n";
		return false;
	}

	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(IP.c_str());
	
    if(bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		cout << "[-]Error in binding.\n";
		return false;
	}

	if(listen(server_socket, 10) != 0) {
		cout << "[-]Error in listening.\n";
        return false;
	}

	return true;
}

int main() 
{
	int server_socket, bind_return;
	sockaddr_in serverAddr;

	if (setupTCP(server_socket, serverAddr, TCP_PORT, LOCAL_IP) == false) {
		return -1;
	}

	int newSocket;
	while (true) {
		sockaddr_in newAddr; 

		socklen_t addr_size = sizeof(newAddr);
		pid_t childpid;

		//accept request
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0) {
			return -1;
		}
		cout << "[+] Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": "
			<< ntohs(newAddr.sin_port) << "\n";

		if((childpid = fork()) == 0) {
			close(server_socket);

			while (true) {
				char buffer[MAXBUFLEN];
    			memset(&buffer, '\0', sizeof(buffer));

				if (recv(newSocket, buffer, MAXBUFLEN, 0) == 0){
					cout << "recv wrong" << endl;
					continue;
				}

				vector<string> message_list = convert_string_to_vector(string(buffer));
				string username = message_list[0];
				string password = message_list[1];
				string reply;

				cout << "that's the request:" << username << " and password : " << password << endl;

				reply = "All right";
				send(newSocket, reply.c_str(),reply.size() + 1, 0);
				cout << "[+] <" << TCP_PORT	 << "> Sending to Client <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
					 << "> : " <<  reply << endl << endl;
			}
		}
		
	}

	close(newSocket);

	return 0;
}