#ifndef SOCKET_H
#define SOCKET_H

#include <WinSock2.h>

#include <string>

enum TypeSocket {BlockingSocket, NonBlockingSocket};

class SocketBase 
{
public:

	virtual ~SocketBase();
	SocketBase(const SocketBase&);
	SocketBase& operator=(SocketBase&);

	std::string ReceiveLine();
	std::string ReceiveBytes();

	void   Close();

	// The parameter of SendLine is not a const reference
	// because SendLine modifes the std::string passed.
	//����SendLine����һ��const����
	//��ΪSendLine modifes��std :: string���ݡ�
	void   SendLine (std::string);

	// The parameter of SendBytes is a const reference
	// because SendBytes does not modify the std::string passed 
	// (in contrast to SendLine).
	//����SendBytes����һ��const����
	//����ΪSendBytes���޸�ͨ����std :: string
	//���ڶԱ�SendLine����
	void   SendBytes(const std::string&);

protected:
	friend class SocketServer;
	friend class SocketSelect;

	SocketBase(SOCKET s);
	SocketBase();


	SOCKET s_;

	int* refCounter_;

private:
	static void Start();
	static void End();
	static int  nofSockets_;
};

class SocketClient : public SocketBase 
{
public:
	SocketClient(const std::string& host, int port);
};

class SocketServer : public SocketBase 
{
public:
	SocketServer(int port, int connections, TypeSocket type=BlockingSocket);

	SocketBase* Accept();
};

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
class SocketSelect 
{
public:
	SocketSelect(SocketBase const * const s1, SocketBase const * const s2=NULL, TypeSocket type=BlockingSocket);

	bool Readable(SocketBase const * const s);

private:
	fd_set fds_;
}; 



#endif