#ifndef WEB_SERVER_EXECUTOR_HPP
#define WEB_SERVER_EXECUTOR_HPP

#include <list>
#include <iostream>
#include <unistd.h>
#include <vector>

class Executor {
private:
    char **arrayEnv;
    char **launch;
    int fdIn;
    int fdOut;
    std::string body;
    Client _client;
public:
    Executor(char **arrayEnv, const std::string &launcher, const std::string &scrypt, const std::string &body,
             const Client &client) :
            arrayEnv(arrayEnv), body(body), _client(client) {
        launch = (char **) malloc(sizeof(char *) * 3);
        launch[0] = strdup(launcher.c_str());
        launch[1] = strdup(scrypt.c_str());
        launch[2] = NULL;
    }

    ~Executor() {
        free(launch[0]);
        free(launch[1]);
        free(launch);
        for (int i = 0; arrayEnv[i]; ++i) {
            free(arrayEnv[i]);
        }
        free(arrayEnv);
    }

    char **getArrayEnv() const {
        return arrayEnv;
    }

    Executor &launcher();

    Executor &putBody();

    Executor &outputBody();

    const std::string &getBody() const;

    char **getLaunch() const;
};

Executor &Executor::launcher() {
    int status = 0;

    pid_t pid = fork();
    if (pid == 0) {
        fdIn = open("cgi_file_input.txt", O_RDWR);
        dup2(fdIn, 0);
        fdOut = open("cgi_file_output.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
        dup2(fdOut, 1);
        chdir(_client.getHttp().getStartLine().find("change_location")->second.c_str());
        std::cerr << "dir=\"" << _client.getHttp().getStartLine().find("change_location")->second << "\"" << std::endl;
        std::cerr << "launch=\"" << getLaunch()[0] << " " << getLaunch()[1] << "\"" << std::endl;
        status = execve(getLaunch()[0], getLaunch(), getArrayEnv());
        exit(status);
    } else if (pid < 0)
        write(2, "Error Fork", 10);
    pid = waitpid(pid, &status, WUNTRACED);
    close(fdIn);
    return *this;
}

Executor &Executor::putBody() {
    fdIn = open("cgi_file_input.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (!body.empty()) {
        write(fdIn, body.c_str(), body.length());
    }
    close(fdIn);
    return *this;
}

Executor &Executor::outputBody() {
    char *buf = new char[5048577];
    int len = 0;
    body.clear();
    fdOut = open("cgi_file_output.txt", O_RDWR);
    while ((len = read(fdOut, buf, 1048576)) > 0) {
        buf[len] = '\0';
        body += buf;
    }
    delete[]buf;
    close(fdOut);
    return *this;
}

const std::string &Executor::getBody() const {
    return body;
}

char **Executor::getLaunch() const {
    return launch;
}

#endif
