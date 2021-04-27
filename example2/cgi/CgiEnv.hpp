//
// Created by Elwin Babaew on 3/6/21.
//

#ifndef WEBSERVER_CGIENV_HPP
#define WEBSERVER_CGIENV_HPP

#include <list>
#include <utility>
#include <unistd.h>
#include "../Client.hpp"

class CgiEnv {
private:
    Client client;
    t_locations locations;
    std::string cgi_path;
    std::list<std::string> env;
    t_server_config_data config;
public:
    CgiEnv(const Client &_client, t_locations &_locations, const std::string &cgi_path, t_server_config_data &config) :
            client(_client), locations(_locations), cgi_path(cgi_path), config(config) {
    }

    const std::string &getCgiPath() const;

    const std::list<std::string> &getEnv() const {
        return env;
    }

    CgiEnv &createEnv();

    char **getEnvArray();

    const Client &getClient() const;

private:
    std::string getAuthType();

    std::string getContentLength();

    std::string getContentType();

    std::string getGatewayInterface();

    std::string getPathInfo();

    std::string getPathTranslated();

    std::string getQueryString();

    std::string getRemoteAddr();

    std::string getRemoteIdent();

    std::string getRemoteUser();

    std::string getRequestMethod();

    std::string getRequestUri();

    std::string getScriptName();

    std::string getFileName();

    std::string getServerName();

    std::string getServerPort();

    std::string getServerProtocol();

    std::string getServerSoftware();
};

CgiEnv &CgiEnv::createEnv() {
    env.push_back(getAuthType());
    env.push_back(getContentLength());
    env.push_back(getContentType());
    env.push_back(getGatewayInterface());
    env.push_back(getPathInfo());
    env.push_back(getPathTranslated());
    env.push_back(getQueryString());
    env.push_back(getRemoteAddr());
    env.push_back(getRemoteIdent());
    env.push_back(getRemoteUser());
    env.push_back(getRequestMethod());
    env.push_back(getRequestUri());
    env.push_back(getServerName());
    env.push_back(getServerPort());
    env.push_back(getServerProtocol());
    env.push_back(getServerSoftware());
    for (std::map<std::string, std::string>::const_iterator i = client.getHttp().getHeadMap().begin();
         i != client.getHttp().getHeadMap().end(); i++)
        env.push_back("HTTP_" + i->first + "=" + i->second);
    return *this;
}

std::string CgiEnv::getAuthType() {
    return std::string("AUTH_TYPE=" + locations.auth_data.AuthType);
}

std::string CgiEnv::getContentLength() {
    return std::string("CONTENT_LENGTH=" + std::to_string(client.getHttp().getBody().length()));
}

std::string CgiEnv::getContentType() {
    if (client.getHttp().getHeadMap().find("content-type") != client.getHttp().getHeadMap().end())
        return std::string("CONTENT_TYPE=" + client.getHttp().getHeadMap().find("content-type")->second);
    else
        return std::string("CONTENT_TYPE=");
}

std::string CgiEnv::getGatewayInterface() {
    return std::string("GATEWAY_INTERFACE=CGI/1.1");
}

std::string CgiEnv::getPathInfo() {
    return std::string("PATH_INFO=" + client.getHttp().getStartLine().find("path_info")->second);
}

std::string CgiEnv::getPathTranslated() {
    return std::string("PATH_TRANSLATED=" + client.getHttp().getStartLine().find("path_info")->second);
}

std::string CgiEnv::getQueryString() {
    if (client.getHttp().getStartLine().find("query_string") != client.getHttp().getStartLine().end())
        return std::string("QUERY_STRING=" + client.getHttp().getStartLine().find("query_string")->second);
    else
        return std::string("QUERY_STRING=");

}

std::string CgiEnv::getRemoteAddr() {
    return std::string("REMOTE_ADDR=" + client.get_remote_addr());
}

std::string CgiEnv::getRemoteIdent() {
    return std::string("REMOTE_IDENT=" + locations.auth_data.password);
}

std::string CgiEnv::getRemoteUser() {
    return std::string("REMOTE_USER=" + locations.auth_data.login);
}

std::string CgiEnv::getRequestMethod() {
    if (client.getHttp().getStartLine().find("method") != client.getHttp().getStartLine().end())
        return std::string("REQUEST_METHOD=" + client.getHttp().getStartLine().find("method")->second);
    else
        return std::string("REQUEST_METHOD=");
}

std::string CgiEnv::getRequestUri() {
    return std::string("RЕQUEST_URI=" + client.getHttp().getStartLine().find("uri")->second);
}

std::string CgiEnv::getServerName() {
    return std::string("SERVER_NAME=" + *(config.server_name.begin()));
}

std::string CgiEnv::getServerPort() {
    return std::string("SERVER_PORT=" + config.port);
}

std::string CgiEnv::getServerProtocol() {
    return std::string("SERVER_PROTOCOL=" + client.getHttp().getStartLine().find("http_version")->second);
}

std::string CgiEnv::getServerSoftware() {
    return std::string("SERVER_SOFTWARE=web_server");
}

char **CgiEnv::getEnvArray() {
    char **pString = (char **) malloc(sizeof(char **) * (getEnv().size() + 1));
    std::list<std::string>::const_iterator j = getEnv().begin();
    std::string::size_type i = 0;
    for (; i < getEnv().size(); ++i) {
        pString[i] = strdup(j->c_str());
        j++;
    }
    pString[i] = NULL;
    return pString;
}

const std::string &CgiEnv::getCgiPath() const {
    return cgi_path;
}

const Client &CgiEnv::getClient() const {
    return client;
}

#endif
