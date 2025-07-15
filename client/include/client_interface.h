#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

namespace transfer {

class ClientInterface {
public:
    virtual void Run() = 0;
    virtual void Stop() = 0;

};

}   //namespace transfer

#endif // CLIENT_INTERFACE_H
