#include <iostream> //cout, endl, cin
/*
*   Implementação do servidor
*   Ao ter alguma conexão estabelecida no servidor o servidor 
*   poderá enviar e receber mensagens atravez desta conexão.
*/

// Possibilita a criação de servidores asincronos.
#include <Tcp/AsyncServer.h>
// Possibilita a criação de modulos para a incorporação em servidores assincronos.
#include <MiddlewareModule.h>

/*
* Modulo de conexão 
*/

class ConnectionModule : public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        Socket* client = (Socket*)Args["Client"];
        Buffer temp("");
        std::string inp;
        while(temp.getActualSize()>=0) {
            temp = client->recvBuffer();
            std::cout<<"Recebido: "<<std::endl;
            std::cout<<temp.toString()<<std::endl;
            std::cout<<"Digite a resposta: "<<std::endl;
            std::cin>>inp;
            client->sendBuffer(Buffer(inp));
        }  
    }
};

int main() {

    AsyncServer serv(25565);
    serv.Use(new ConnectionModule(),EventTypes::Connected,5);
    serv.Start(false);
    return 0;
}