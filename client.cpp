#include <bits/stdc++.h>
#include <curl/curl.h>
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

typedef long long int lli;

class Catraca {
private:
    int id;
    vector<string> types;
    map<string,int> permitidos;
    vector<int> passadas;
    vector<int> rejeitadas;
public:
    Catraca(int id, vector<string> types);
    ~Catraca();
    void Passar(lli cpf);
    void HttpGetList();
};

Catraca::Catraca(int id, vector<string> types){
    this->id = id;
    this->types = types;
    HttpGetList();
}

string data = "*";
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    for (int c = 0; c<size*nmemb; c++) {
        data.push_back(buf[c]);
    }
    return size*nmemb;
}

void Catraca::HttpGetList(){
    CURL *curl;
    if(data[0] != '*'){
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9999/getRegisters");
        // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        curl_easy_perform(curl);

        cout << endl << data << endl;
        cin.get();

        curl_easy_cleanup(curl);
        curl_global_cleanup();

    }

    Document d;
    d.Parse(data.c_str());
    for(string permitido : this->types){
        Value& attr = d[permitido.c_str()];
        for(ConstValueIterator itr = attr.Begin(); itr != attr.End(); itr++){
            Value& item = *ittr;
            cout << item
        }
    }

}

class Pessoa {
private:
    string name;
    lli cpf;
public:
    Pessoa(string name, lli cpf);
    ~Pessoa();
};

Pessoa::Pessoa(string name, lli cpf) {
    this->name = name;
    this->cpf = cpf;
}

class Estudante : public Pessoa {
public:
    Estudante(string name, lli cpf) : Pessoa(name, cpf) {}
};


class Professor : public Pessoa {
public:
    Professor(string name, lli cpf) : Pessoa(name, cpf) {}
};

class Servidor : public Pessoa {
public:
    Servidor(string name, lli cpf) : Pessoa(name, cpf) {}
};

int main(){
    vector<string> permitidos;
    permitidos.push_back("teachers");
    Catraca * catraca = new Catraca(1, permitidos);


}
