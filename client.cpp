// using g++ -o client -std=c++11 client.cpp
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

using namespace std;

const std::string LOCAL_IP = "127.0.0.1";
const int TCP_PORT = 25060;
const int MAXBUFLEN = 4096;

bool usernameCheck(string username) {
	return true;
}

bool passwordCheck(string password) {
	return true;
}

int main()
{
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		cout << "socket wrong" << endl;
		return -1;
	}

	sockaddr_in server_addr, my_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(TCP_PORT);	
	server_addr.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());

	int connectRes = connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	if (connectRes == -1) {
		cout << "connect wrong" << endl;
		return -1;
	}

	int myPort;
    char myIP[16];
    bzero(&my_addr, sizeof(my_addr));
	socklen_t len = sizeof(my_addr);
	getsockname(client_socket, (struct sockaddr *) &my_addr, &len);
	inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
	myPort = ntohs(my_addr.sin_port);

    //	While loop:
    char buf[4096];
    string userInput;
    int count = 2;

    cout << "The client is up and running at port <" << myPort << ">" << endl <<endl;

	while (true) {

        if (count < 0) {
            cout << "Authentication Failed for 3 attempts. Client will shut down." << endl;
            break;
        }

        string username, password;

        
        cout << "Please enter the username: ";
        getline(cin, username);
        if (!usernameCheck(username)) {
            continue;
        }

        cout << "Please enter the password: ";
        getline(cin, password);
        if (!passwordCheck(password)) {
            continue;
        }

        userInput = username + "," + password;
        //  Send to server
        int sendRes = send(client_socket, userInput.c_str(), userInput.size() + 1, 0);
        cout << username << " sent an authentication request to the main server." << endl;
        if (sendRes == -1) {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        //	Wait for response
        memset(buf, 0, 4096);
        
        int bytesReceived = recv(client_socket, buf, 4096, 0);
        string receivedInfo(buf);
        if (bytesReceived == -1) {
            cout << "[-] <" << myPort << "> There was an error getting response from server" << endl << endl;
        }
        else {
            if (receivedInfo.compare("PASS") == 0) {
                cout << username << " received the result of authentication using TCP over "
                     << TCP_PORT << ". Authentication is successful" << endl;
            } else if (receivedInfo.compare("FAIL_NO_USER") == 0) {
                cout << username << " received the result of authentication using TCP over "
                     << TCP_PORT << ". Authentication failed: Username Does not exist" << endl
                     << "Attempts remaining:" << count << endl;
            } else if (receivedInfo.compare("FAIL_PASS_NO_MATCH") == 0) {
                cout << username << " received the result of authentication using TCP over "
                     << TCP_PORT << ". Authentication failed: Password Does not exist" << endl
                     << "Attempts remaining:" << count << endl;
            }
        }
        count--;
    }

	close(client_socket);

	return 0;

}