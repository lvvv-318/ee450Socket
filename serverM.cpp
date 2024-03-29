# include <iostream>
# include <sys/types.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <string.h>
# include <string>
# include <netinet/in.h>
# include <string>
# include <unordered_map>
# include <unordered_set>
# include <iostream>
# include <fstream>
# include <vector>
# include <ctype.h>
# include <sstream>
# include <climits>
# include <string>

using namespace std;

const std::string LOCAL_IP = "127.0.0.1";
const int SERVER_C = 21060;
const int SERVER_CS = 22060;
const int SERVER_EE = 23060;
const int UDP_MAIN_SERVER = 24060;
const int TCP_PORT = 25060;
const int MAXBUFLEN = 4096;

string encrypt(string input)
{
	string cipher = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] >= 'a' && input[i] <= 'z') {
			cipher.push_back((char)((input[i] - 'a' + 4) % 26 + 'a'));
		} else if (input[i] >= 'A' && input[i] <= 'Z') {
			cipher.push_back((char)((input[i] - 'A' + 4) % 26 + 'A'));
		} else if (input[i] >= '0' && input[i] <= '9') {
			cipher.push_back((char)((input[i] - '0' + 4) % 10 + '0'));
		} else {
			cipher.push_back(input[i]);
		}
	}
	return cipher;
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
	
    socklen_t  serverAddr_UDP_length = sizeof(serverAddr_UDP);

	char buffer[MAXBUFLEN];
	memset(&buffer, '\0', sizeof(buffer));
	
	recvfrom(sockfd_UDP, buffer, MAXBUFLEN, 0, (sockaddr*) &serverAddr_UDP, &serverAddr_UDP_length);
	
	return string(buffer);
}

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
		cout << "Error in binding.\n";
		return false;
	}

	if(listen(server_socket, 10) != 0) {
		cout << "Error in listening.\n";
        return false;
	}

	return true;
}

int main() 
{
	// UDP Setup
	int sockfd_UDP;
    sockaddr_in main_serverAddr_UDP, serverAddr_UDP_C, serverAddr_UDP_CS, serverAddr_UDP_EE;
    create_UDP(sockfd_UDP, main_serverAddr_UDP, UDP_MAIN_SERVER, LOCAL_IP);

    if (bind(sockfd_UDP, (sockaddr *) &main_serverAddr_UDP, sizeof(main_serverAddr_UDP)) < 0) {
        cout << "Error in binding in UDP ServerC" << endl;
        return -1;
    }

	memset(&serverAddr_UDP_C, 0, sizeof(serverAddr_UDP_C));
    serverAddr_UDP_C.sin_family = AF_INET;
    serverAddr_UDP_C.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());
    serverAddr_UDP_C.sin_port = htons(SERVER_C);

	memset(&serverAddr_UDP_CS, 0, sizeof(serverAddr_UDP_CS));
	serverAddr_UDP_CS.sin_family = AF_INET;
	serverAddr_UDP_CS.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());
	serverAddr_UDP_CS.sin_port = htons(SERVER_CS);

	memset(&serverAddr_UDP_EE, 0, sizeof(serverAddr_UDP_EE));
	serverAddr_UDP_EE.sin_family = AF_INET;
	serverAddr_UDP_EE.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());
	serverAddr_UDP_EE.sin_port = htons(SERVER_EE);

	// TCP Setup
	int server_socket, bind_return;
	sockaddr_in serverAddr;

	if (setupTCP(server_socket, serverAddr, TCP_PORT, LOCAL_IP) == false) {
		return -1;
	}

	cout << "The main server is up and running." << endl;

	int newSocket;

	while (true) {

		sockaddr_in newAddr; 

		socklen_t addr_size = sizeof(newAddr);
		pid_t childpid;

		//accept request
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if (newSocket < 0) {
			return -1;
		}

		if ((childpid = fork()) == 0) {
			close(server_socket);

			string username;

			while (true) {

				char buffer[MAXBUFLEN];
    			memset(&buffer, '\0', sizeof(buffer));

				if (recv(newSocket, buffer, MAXBUFLEN, 0) == 0){
					cout << "recv wrong" << endl;
					continue;
				}

				if (buffer[0] != 'E' && buffer[0] != 'C') {

					vector<string> message_list = convert_string_to_vector(string(buffer));
					username = message_list[0];
					string password = message_list[1];
					string en_username = encrypt(username);
					password = encrypt(password);
					string toCren = en_username + "," + password;
					memset(&buffer, '\0', sizeof(buffer));

					cout << "The main server received the authentication for " << username <<
							" using TCP over port " << TCP_PORT << "." << endl;

					cout << "The main server sent an authentication request to serverC." << endl;
					string response = UDP_send_receive(sockfd_UDP, serverAddr_UDP_C, toCren);
					cout << "The main server received the result of the authentication request " \
							"from ServerC using UDP over port " << UDP_MAIN_SERVER << endl;

					send(newSocket, response.c_str(), response.size() + 1, 0);

					cout << "The main server sent the authentication result to the client." << endl;

				} else if (buffer[0] == 'E') {

					vector<string> message_list = convert_string_to_vector(string(buffer));
					string courseCode = message_list[0];
					string category = message_list[1];
					string toEE = courseCode + "," + category;

					cout << "The main server received from " << username << " to query course " <<
							courseCode << " about " << category << " using TCP over port " <<
							TCP_PORT << endl;
					string ee_response = UDP_send_receive(sockfd_UDP, serverAddr_UDP_EE, toEE);
					cout << "The main server sent a request to serverEE." << endl;

					cout << "The main server received the response from serverEE using UDP " <<
							"over port " << UDP_MAIN_SERVER << "." << endl;
					send(newSocket, ee_response.c_str(), ee_response.size() + 1, 0);
					cout << "The main server sent the query information to the client." << endl;

				} else if (buffer[0] == 'C') {

					vector<string> message_list = convert_string_to_vector(string(buffer));
					string courseCode = message_list[0];
					string category = message_list[1];
					string toCS = courseCode + "," + category;

					cout << "The main server received from " << username << " to query course " <<
							courseCode << " about " << category << " using TCP over port " <<
							TCP_PORT << endl;
					string cs_response = UDP_send_receive(sockfd_UDP, serverAddr_UDP_CS, toCS);
					cout << "The main server sent a request to serverCS." << endl;
					
					cout << "The main server received the response from serverCS using UDP " <<
							"over port " << UDP_MAIN_SERVER << "." << endl;
					send(newSocket, cs_response.c_str(), cs_response.size() + 1, 0);
					cout << "The main server sent the query information to the client." << endl;

				}
			}
		}
	}

	close(newSocket);
	close(sockfd_UDP);

	return 0;
}