#ifndef INCLUDE_EASY_HTTP_UTILITY
#define INCLUDE_EASY_HTTP_UTILITY
#pragma once

#include <iostream>
#include "Memi.hpp"

namespace easy::http::utility {

    std::vector<std::string> split(std::string str,std::string delimiter) {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> strs;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            strs.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        
        if (str.size() > 0) {
            strs.push_back(str);
        }

        return strs;
    }

    bool IsUrlEncoded(std::string value)
    {
        for (auto c : value)
        {
            if (c == '%' || c == '+')
            {
                return true;
            }
        }
        
        return false;
    }

    std::string UrlEncode(const std::string& value)
    {
        static auto hex_digt = "0123456789ABCDEF";
        
        std::string result;
        result.reserve(value.size() << 1);
        
        for (auto ch : value)
        {
            if ((ch >= '0' && ch <= '9')
                || (ch >= 'A' && ch <= 'Z')
                || (ch >= 'a' && ch <= 'z')
                || ch == '-' || ch == '_' || ch == '!'
                || ch == '\'' || ch == '(' || ch == ')'
                || ch == '*' || ch == '~' || ch == '.')  // !'()*-._~
            {
                result.push_back(ch);
            }
            else
            {
                result += std::string("%") +
                        hex_digt[static_cast<unsigned char>(ch) >> 4]
                        +  hex_digt[static_cast<unsigned char>(ch) & 15];
            }
        }
        
        return result;
    }

    std::string UrlDecode(const std::string& value)
    {
        std::string result;
        result.reserve(value.size());
        
        for (std::size_t i = 0; i < value.size(); ++i)
        {
            auto ch = value[i];
            
            if (ch == '%' && (i + 2) < value.size())
            {
                auto hex = value.substr(i + 1, 2);
                auto dec = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
                result.push_back(dec);
                i += 2;
            }
            else if (ch == '+')
            {
                result.push_back(' ');
            }
            else
            {
                result.push_back(ch);
            }
        }
        
        return result;
    }

    enum class status : unsigned int
    {
        /** An unknown status-code.

            This value indicates that the value for the status code
            is not in the list of commonly recognized status codes.
            Callers interested in the exactly value should use the
            interface which provides the raw integer.
        */
        unknown = 0,

        continue_                           = 100,

        /** Switching Protocols

            This status indicates that a request to switch to a new
            protocol was accepted and applied by the server. A successful
            response to a WebSocket Upgrade HTTP request will have this
            code.
        */
        switching_protocols                 = 101,

        processing                          = 102,

        ok                                  = 200,
        created                             = 201,
        accepted                            = 202,
        non_authoritative_information       = 203,
        no_content                          = 204,
        reset_content                       = 205,
        partial_content                     = 206,
        multi_status                        = 207,
        already_reported                    = 208,
        im_used                             = 226,

        multiple_choices                    = 300,
        moved_permanently                   = 301,
        found                               = 302,
        see_other                           = 303,
        not_modified                        = 304,
        use_proxy                           = 305,
        temporary_redirect                  = 307,
        permanent_redirect                  = 308,

        bad_request                         = 400,
        unauthorized                        = 401,
        payment_required                    = 402,
        forbidden                           = 403,
        not_found                           = 404,
        method_not_allowed                  = 405,
        not_acceptable                      = 406,
        proxy_authentication_required       = 407,
        request_timeout                     = 408,
        conflict                            = 409,
        gone                                = 410,
        length_required                     = 411,
        precondition_failed                 = 412,
        payload_too_large                   = 413,
        uri_too_long                        = 414,
        unsupported_media_type              = 415,
        range_not_satisfiable               = 416,
        expectation_failed                  = 417,
        misdirected_request                 = 421,
        unprocessable_entity                = 422,
        locked                              = 423,
        failed_dependency                   = 424,
        upgrade_required                    = 426,
        precondition_required               = 428,
        too_many_requests                   = 429,
        request_header_fields_too_large     = 431,
        connection_closed_without_response  = 444,
        unavailable_for_legal_reasons       = 451,
        client_closed_request               = 499,

        internal_server_error               = 500,
        not_implemented                     = 501,
        bad_gateway                         = 502,
        service_unavailable                 = 503,
        gateway_timeout                     = 504,
        http_version_not_supported          = 505,
        variant_also_negotiates             = 506,
        insufficient_storage                = 507,
        loop_detected                       = 508,
        not_extended                        = 510,
        network_authentication_required     = 511,
        network_connect_timeout_error       = 599
    };

    /** Represents the class of a status-code.
    */
    enum class status_class : unsigned
    {
        /// Unknown status-class
        unknown = 0,

        /// The request was received, continuing processing.
        informational = 1,

        /// The request was successfully received, understood, and accepted.
        successful = 2,

        /// Further action needs to be taken in order to complete the request.
        redirection = 3,

        /// The request contains bad syntax or cannot be fulfilled.
        client_error = 4,

        /// The server failed to fulfill an apparently valid request.
        server_error = 5,
    };

}
#endif