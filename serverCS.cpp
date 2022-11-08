// using g++ -o serverCS -std=c++11 serverCS.cpp
# include <iostream>
# include <sstream>
# include <iomanip>
# include <list>
# include <set>
# include <vector>
# include <unordered_map>
# include <cstring>
# include <fstream>
# include <queue>
# include <limits>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
#include <map>

using namespace std;

const string INPUT = "cs.txt";

int main(int argc, const char* argv[]) {

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
        ss >> col5;
    }
    
    return 0;

}