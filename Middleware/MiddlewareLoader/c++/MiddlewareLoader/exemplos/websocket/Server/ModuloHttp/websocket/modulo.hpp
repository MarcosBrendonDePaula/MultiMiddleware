#pragma once

#include <iostream>
#include <fstream>
#include <map>

#include <Tcp/AsyncServer.h>
#include "../bodyparser.hpp"
#include "../http/json.hpp"
#include "../http/Utility.hpp"
#include "../http/sha1/sha1.hpp"
#include "../http/base64/base64.hpp"

using namespace std;

class WebsocketInstance{
    public:
        WebsocketInstance();
        Socket* client;
        bool connected;
        std::pair<int,std::string> DecodeDataGrama(Buffer datagram);
        int SendDataGram(std::string str);
        std::string path;
};

WebsocketInstance::WebsocketInstance() {
    this->connected = false;
}


class WebSocket : public MiddlewareModule{
    private:
        bool wbClient;
        bool answered;
    public:
        static std::map<Socket*, WebsocketInstance> Instances;
        static bool Debug;
        void Main(map<string,void *>&Args);
        WebSocket() {
        }
};

std::map<Socket*, WebsocketInstance> WebSocket::Instances;

void WebSocket::Main(map<string,void *>&Args) {
    auto client = (ServerClient*)Args["Client"];
   
    if(this->Instances[client].connected) {
        Args["wbclient"] = &wbClient;
        Args["instance"] = &this->Instances[client];
        return;
    }

    easy::Header::Request req(((Buffer*)Args["Buffer"])->toString());
    if(req.Header["Upgrade"]=="websocket"){
        
        if(WebSocket::Debug) {
            std::cout<<"WebSocket HandShake"<<std::endl;
        }
        
        Args["answered"] = &answered;

        std::string Key = req.Header["Sec-WebSocket-Key"];

        auto newKey = Key+ "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

        char hex[SHA1_HEX_SIZE];
        char base64[SHA1_BASE64_SIZE];
        auto sh = sha1(newKey.c_str());
        sh.finalize();
        sh.print_base64(base64);

        std::string header = 
        std::string("HTTP/1.1 101 Switching Protocols\r\n")+
        std::string("Upgrade: websocket\r\n")+
        std::string("Connection: Upgrade\r\n")+
        std::string("Sec-WebSocket-Accept: "+std::string(base64)+"\r\n\r\n");

        client->sendBuffer(Buffer(header));

        this->Instances[client].connected = true;
        this->Instances[client].client    = client;
        this->Instances[client].path      = req.path;
        if(WebSocket::Debug) {
            std::cout<<"WebSocket Base64 key:"<<base64<<" Accepted"<<std::endl;
        }

        if(WebSocket::Debug) {
            std::cout<<"WebSocket is connected"<<std::endl;
            
        }

    }
}

std::pair<int,std::string> WebsocketInstance::DecodeDataGrama(Buffer datagram) {
    unsigned char* data = (unsigned char*) datagram.getDataAccess();
    unsigned int i, j;
    unsigned char mask[4];
    unsigned int packet_length = 0;
    unsigned int length_code = 0;
    int index_first_mask  = 0;
    int index_first_data_byte  = 0;

    if( ( unsigned char )data[0] != 129 ) 
    {
        //dst = NULL;
        if( ( unsigned char )data[0] == 136 ) 
        {
            /* WebSocket client disconnected */
            return std::make_pair(-2,"");
        }
        /* Unknown error */
        return std::make_pair(-1,"");
    }

    length_code = ((unsigned char) data[1]) & 127;

    if( length_code <= 125 ) 
    {
            index_first_mask = 2;
            mask[0] = data[2];
            mask[1] = data[3];
            mask[2] = data[4];
            mask[3] = data[5];
        } 
    else 
    if( length_code == 126 ) 
    {
        index_first_mask = 4;

        mask[0] = data[4];
        mask[1] = data[5];
        mask[2] = data[6];
        mask[3] = data[7];
    } 
    else 
    if( length_code == 127 ) 
    {
        index_first_mask = 10;
        mask[0] = data[10];
        mask[1] = data[11];
        mask[2] = data[12];
        mask[3] = data[13];
    }

    index_first_data_byte = index_first_mask + 4;

    packet_length = datagram.getActualSize() - index_first_data_byte;

    std::vector<char> temp(packet_length,0x0);

    for( i = index_first_data_byte, j = 0; i < datagram.getActualSize(); i++, j++ ) 
    {
        temp[ j ] = ( unsigned char )data[ i ] ^ mask[ j % 4];
    }

    return std::make_pair(packet_length,std::string(&temp[0],temp.size()));
}

int WebsocketInstance::SendDataGram(std::string str) {
    
    unsigned char* data = (unsigned char*) str.c_str();
    int data_length     = str.size(); 
    
    unsigned char *message = ( unsigned char * )malloc( 65535 * sizeof( char ) );
    int i;
    int data_start_index;

    message[0] = 129;

    if( data_length <= 125 ) 
    {
        message[1] = ( unsigned char) data_length;
        data_start_index = 2;
    } 
    else 
    if( data_length > 125 && data_length <= 65535 ) 
    {
        message[1] = 126;
        message[2] = ( unsigned char )( ( data_length >> 8 ) & 255 );
        message[3] = ( unsigned char )( ( data_length ) & 255 );
        data_start_index = 4;
    } 
    else 
    {
        message[1] = 127;
        message[2] = ( unsigned char )( ( data_length >> 56 ) & 255 );
        message[3] = ( unsigned char )( ( data_length >> 48 ) & 255 );
        message[4] = ( unsigned char )( ( data_length >> 40 ) & 255 );
        message[5] = ( unsigned char )( ( data_length >> 32 ) & 255 );
        message[6] = ( unsigned char )( ( data_length >> 24 ) & 255 );
        message[7] = ( unsigned char )( ( data_length >> 16 ) & 255 );
        message[8] = ( unsigned char )( ( data_length >> 8 ) & 255 );
        message[9] = ( unsigned char )( ( data_length ) & 255 );
        data_start_index = 10;
    }
    
    for( i = 0; i < data_length; i++ ) 
    {
        message[ data_start_index + i ] = ( unsigned char )data[i];
    }
    
    
    unsigned char *temp = new unsigned char[data_length+ data_start_index];

    for( i = 0; i < data_length+ data_start_index; i++ ) 
    {
        temp[i] = ( unsigned char )message[ i ];
    }
    
    send(this->client->_socket,(char*)temp,i,0);
    
    if( message ) 
    {
        free( message );
        message = NULL;
    }

    return i;
}

bool WebSocket::Debug = false;