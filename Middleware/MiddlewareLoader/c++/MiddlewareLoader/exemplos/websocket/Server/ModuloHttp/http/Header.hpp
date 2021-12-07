#ifndef INCLUDE_EASY_HTTP_HEADER
#define INCLUDE_EASY_HTTP_HEADER
#pragma once

#include <map>
#include <iostream>
#include <vector>
#include <regex>

#include "Utility.hpp"

namespace easy::Header {
    using string = std::string;

    class Response {
        public:
        string version;
        string stats;
        string body;
        std::map<string,string> Headers;

        Response();
        string to_str();
        private:
    };

    Response::Response() {
        body = "\0";
        version = "HTTP/1.1";
        stats = "0";
    }
    
    string Response::to_str() {
        string header = (version+" "+stats+"\r\n");
        
        for(auto i:Headers) {
            header += (i.first+": "+i.second+"\r\n");
        }

        header +="\r\n";
        if(!body.empty())
            header += body;
        return header;
    }
    
    class RequestFile {
        public:
            string Field_Name;
            string ContentType;
            string File_name;
            string Content;
            ULONG64 bytes;
    };

    class Request{
        public:
            Request(string req);
            string to_str();
            string path;
            string method;
            string version;
            string body;
            std::map<string,string> Header;
            std::map<string,string> FormFields;
            std::vector<RequestFile> Files;
            bool is_multipart_form;
    };

    Request::Request(string req="") {
        is_multipart_form = false;
        if(req.empty()){
            return;
        }

        std::vector<string> HeaderBody{easy::http::utility::split(req,"\r\n\r\n")};
        //std::cout<<HeaderBody.size()<<std::endl;
       
        auto Headers = easy::http::utility::split(HeaderBody[0],"\r\n");
        
        //Method Path Version
        auto Control = easy::http::utility::split(Headers[0]," ");
        this->method = Control[0];
        
        if(easy::http::utility::IsUrlEncoded(Control[1])){
            this->path = easy::http::utility::UrlDecode(Control[1]);
        }else
            this->path = Control[1];

        this->version = Control[2];
        Control.clear();

        //all headers
        for(int i=1;i<Headers.size();i++) {
            string Atual = Headers[i];
            Control = easy::http::utility::split(Atual,": ");
            this->Header[Control[0]] = Control[1];
            Control.clear();
        }
        int multipart_pos = Header["Content-Type"].find_last_of("multipart\\s=");

        if(multipart_pos!=string::npos) {
            std::cout<<"Incompleto!"<<std::endl;
        }
        else
        //basic form
        if(Header.find("Content-Length")!=Header.end()) {
            body = (easy::http::utility::IsUrlEncoded(req.substr(req.find("\r\n\r\n")+4)))?easy::http::utility::UrlDecode(req.substr(req.find("\r\n\r\n")+4)):req.substr(req.find("\r\n\r\n")+4);
        } else {
            body = "";
        }
        return;
    }

    string Request::to_str() {
        string strheader = "";
        strheader+=(method+" "+path+" "+version);
        for(auto hd:Header) {
            strheader+=(hd.first+":"+hd.second+"\r\n");
        }
        return strheader;
    }
}
#endif