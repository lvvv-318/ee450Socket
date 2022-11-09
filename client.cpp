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

using namespace std;

#define SERVERPORT "25060"
#define MAXBUFLEN 100

struct UserInfo {
    string username;
    string password;
};

bool usernameCheck(string username) {
	return true;
}

bool passwordCheck(string password) {
	return true;
}

int main(int argc, char* argv[])
{
    int sockfd, numbytes;
    struct addrinfo hints, *serveinfo, *p;
    int rv;

    // if (arge != 2) {
    //     fprintf(stderr,"usage: client hostname\n");
    //     exit(1);
    // }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

    // loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

    cout << "The client is up and running." << endl;

    UserInfo userInfo;

	while(true) {
		
		cout << "Please enter the user ID: " << endl;
		cin >> userInfo.username;
		if (!usernameCheck(userInfo.username)) {
			continue;
		}
		cout << "Please enter the password: " << endl;
		cin >> userInfo.password;
		if (!passwordCheck(userInfo.password)) {
			continue;
		}

		if (numbytes = send(sockfd, &userInfo, sizeof(userInfo, 0)) == -1) {
			cout << userInfo.username << " could not send to server!" << endl;
			continue;
		}
		cout << userInfo.username << " sent an authentication request to the main server." << endl;

		char msgRecv[MAXBUFLEN];
		if ((numbytes = recv(sockfd, &msgRecv, sizeof(msgRecv), 0)) == -1) {
			cout << "There was an error getting response from server." << endl;
		} else {
			cout << "That's the message received: " << endl << msgRecv << endl;
		}

	}

	close(sockfd);
	return 0;

}