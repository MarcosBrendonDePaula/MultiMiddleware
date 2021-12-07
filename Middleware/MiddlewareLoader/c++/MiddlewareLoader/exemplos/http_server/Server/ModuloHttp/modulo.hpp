#pragma once
#include <iostream>
#include <fstream>
#include <map>

#include <Tcp/AsyncServer.h>
#include "./bodyparser.hpp"
#include "http/json.hpp"
#include "http/Utility.hpp"


using namespace std;
using json = nlohmann::json;
using Memi = easy::http::utility::Memi;

string Path = "public";

auto MeMiList = Memi::get();

class HttpServer : public MiddlewareModule{
    public:
        void Main(map<string,void *>&Args) {
            
            if((Args["answered"] != nullptr)){
                cout<<"HttpServer: algum modulo respondeu primeiro"<<endl;
                return;
            }

            auto client = (ServerClient*)Args["Client"];
            easy::Header::Request req(((Buffer*)Args["Buffer"])->toString());
            uint64_t size=0;
            if(req.method=="GET"){
                easy::Header::Response res;
                //req.path.erase(req.path.begin());
                fstream File;
                File.open(Path+req.path,ios::in|ios::ate|ios::binary);
                if(!File.is_open()) {
                    res.body =  "  ";
                    res.stats = "404";
                    res.version = "HTTP/1.1";
                    res.Headers["content-length"] = "0";
                }
                else 
                {
                    size = File.tellp();
                    File.seekp(0,ios::beg);
                    vector<char> file(size);
                    //char *file = new char[size];
                    File.read(&file[0],size);

                    vector<string> Parts;
                    string StrFile(&file[0],size);
                    File.close();
                    
                    res.Headers["Content-Transfer-Encoding"] = "binary";
                    res.Headers["charset"] = "ISO-8859-4";
                    res.Headers["content-length"] = to_string(size);
                    res.body = StrFile+"\r\n";
                    res.stats = (unsigned int)easy::http::utility::status::ok;
                    res.version = "HTTP/2";
                    res.Headers["Content-Type"] = MeMiList->ftm(req.path.substr(req.path.find(".")+1));
                    StrFile.clear();
                }
                cout<<req.path<<" "<<res.Headers["Content-Type"]<<" "<<size<<endl;
                auto buf = Buffer(res.to_str(),res.to_str().size());
                client->sendBuffer(buf);
            }else 
            if(req.method=="POST") {
                cout<<"files:"<<req.Files.size()<<endl;
                for(auto file:req.Files) {
                    cout<<"Create File: "<<Path+"/"+file.File_name<<"\n";
                    ofstream nfile(Path+"/"+file.File_name,ios::out|ios::binary);
                    cout<<nfile.is_open()<<"\n";
                    nfile.write(file.Content.c_str(),file.bytes);
                    nfile.close();
                }
                
                //cout<<req.to_str()<<endl;
                easy::Header::Response res;
                string body = /*((json*)Args["body"])->dump()*/"{}";
                //cout<<((json*)Args["body"])->dump()<<endl;
                res.Headers["Content-Transfer-Encoding"] = "binary";
                res.Headers["charset"] = "ISO-8859-4";
                res.Headers["content-length"] = to_string(body.size());
                res.body = body+"\r\n";
                res.stats = (unsigned int)easy::http::utility::status::ok;
                res.version = "HTTP/2";
                res.Headers["Content-Type"] = MeMiList->ftm("json");
                auto buf = Buffer(res.to_str(),res.to_str().size());
                client->sendBuffer(buf);
            }
        }
};

class RemoveInstances:public MiddlewareModule {
    public:
    void Main(map<string,void *>&Args) {
        delete ((json*)Args["body"]);
    }
};