USC EE450 Socket Programming Project
Fall 2022

a.  Full Name: Zhiyuan Lv

b.  Student ID: 9954199060

c.  What I have done in the assignment:

        In this project, I implemented a student request system to search for course information
    based on users' queries. The system consists of a main server, a credential server, two back-
    end department servers, and a client.
        The credential server stores the authentication information of the system, which is user-
    names and passwords. And the department servers, server CS and server EE, store the course info
    of corresponding courses held by these two departments. The main server receive the queries from
    client and forward to credential server and department servers, then receive the responses and
    forward them to client.

d.  My code files:

    - client.cpp
        - Connect with main server using TCP
        - Send authentication query and search query to main server
        - Display the responses to user
    
    - serverM.cpp
        - Connect with client using TCP
        - Connect with backend servers using UDP
        - Process queries and forward them to corresponding servers
    
    - serverC.cpp
        - Connect with main server using UDP
        - Process authentication query
        - Send response to main server
    
    - serverCS.cpp
        - Connect with main server using UDP
        - Process search query
        - Send response to main server
    
    - serverEE.cpp
        - Connect with main server using UDP
        - Process search query
        - Send response to main server

e.  Format of all the messages exchanged:

    - client to main server:
        - string of username and password
        - string of course code and category
    
    - main server to credential server:
        - string of encrypted username and password
    
    - main server to department server:
        - string of course code and category
    
    - credential/department server to main server:
        - string of response information

g.  Idiosyncrasy

    Based on the test results run on given Ubuntu environment, there is no idiosyncrasy.

h.  Reference code:

    Beej's guide to network programming
        - Generate TCP/UDP connections
        - Bind a socket
        - Send and receive messages

