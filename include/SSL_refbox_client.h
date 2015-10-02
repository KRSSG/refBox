#ifndef REF_BOX_CLIENT_H
#define REF_BOX_CLIENT_H
#include "netraw.h"
#include <string>
#include <QMutex>
#include "referee.pb.h"

using namespace std;

class SSLRefboxClient{
protected:
    static const int MaxDataGramSize = 65536;
	char *in_buffer;
	Net::UDP mc;
	QMutex mutex;
	int _port;
	string _net_address;
	string _net_interface;
public:
    SSLRefboxClient(int port = 10003,
                     string net_ref_address="224.5.23.1",
                     string net_ref_interface="");

    ~SSLRefboxClient();
    bool open(bool blocking=false);
    void close();
    bool receive(SSL_Referee & packet);
};

#endif
