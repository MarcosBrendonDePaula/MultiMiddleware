#include <Tcp/AsyncClient.h>
#include <MiddlewareModule.h>
#include <iostream>
using namespace std;
class OnConnect:public MiddlewareModule{
    public:
        void Main(std::map<std::string,void*>& Args) {
            Socket* socket = (Socket*) Args["Client"];
            Buffer temp("");
            while (temp.getActualSize() >= 0 )
            {
                cout<<"Digite uma mensagem a ser enviada"<<endl;
                string inp;
                cin>>inp;
                socket->sendBuffer(Buffer(inp));
                temp = socket->recvBuffer();
                cout<<"Respondeu:"<<temp.toString()<<endl;
            }
        }
};

int main() {
    AsyncClient cliente;
    cliente.Use(new OnConnect(),EventTypes::Connected,5);
    cliente.connect_("127.0.0.1",25565,1500,false);
    return 0;
}
