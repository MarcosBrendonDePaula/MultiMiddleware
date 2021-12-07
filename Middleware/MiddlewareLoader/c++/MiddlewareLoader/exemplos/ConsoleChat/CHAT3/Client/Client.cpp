#include <Tcp/AsyncClient.h>
#include <MiddlewareModule.h>
#include <iostream>
using namespace std;

class OnConnect:public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
            
    }
};

class OnReceive:public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto buffer = (Buffer*)Args["Buffer"];
        cout<<buffer->toString()<<endl;  
    }
};



int main() {
    AsyncClient cliente;
    cliente.Use(new OnReceive,EventTypes::Received,5);
    cliente.connect_("127.0.0.1",25565,1500,true);
    while(cliente.isConnected()) {
        string msg;
        cin>>msg;
        cliente.sendBuffer(Buffer(msg));
    }
}
