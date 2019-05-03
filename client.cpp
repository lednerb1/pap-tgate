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
    map<string, string> permitidos;
    vector<pair<string, string>> passadas;
    vector<string> rejeitadas;
public:
    Catraca(int id, vector<string> types);
    ~Catraca();
    void Passar(string cpf);
    void HttpGetList();
    void HttpSendBuffer();
    bool inList(string cpf){
      return permitidos.find(cpf) != permitidos.end();
    }
};

Catraca::Catraca(int id, vector<string> types){
    this->id = id;
    this->types = types;
    HttpGetList();
}

void Catraca::Passar(string cpf){
    map<string,string>::iterator i = this->permitidos.find(cpf);

    if(i != this->permitidos.end()){
        this->passadas.push_back({i->first, i->second});
    }else{
        this->rejeitadas.push_back(cpf);
    }

    if(this->passadas.size() + this->rejeitadas.size() > 9){
      this->HttpSendBuffer();
    }
}

string data;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    for (int c = 0; c<size*nmemb; c++) {
        data.push_back(buf[c]);
    }
    return size*nmemb;
}

void Catraca::HttpGetList(){
    CURL *curl;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/getRegisters");
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_perform(curl);

    //cout << endl << data << endl;
    //cin.get();

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    //cout << data << endl;
    Document d;
    cout << data << endl;
    d.Parse(data.c_str());
    assert(d.IsObject());
    for(string permitido : this->types){
        Value& attr = d[permitido.c_str()];
        assert(attr.IsArray());
        for(Value::ConstValueIterator itr = attr.Begin(); itr != attr.End(); itr++){
            const Value& itr2 = *itr;
            assert(itr2.IsObject());
            Value::ConstMemberIterator item = itr2.MemberBegin();
            permitidos.insert({item->value.GetString(), (item+1)->value.GetString()});
        }
    }

}

void Catraca::HttpSendBuffer() {
    Document d;
    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();
    writer.String("Passaram");
    writer.StartArray();
    for(auto& s : this->passadas){
        string a = s.first + " " + s.second;
        writer.String(a.c_str());
    }
    writer.EndArray();
    writer.String("Rejeitados");
    writer.StartArray();
    for(string a : this->rejeitadas){
        writer.String(a.c_str());
    }
    writer.EndArray();
    writer.EndObject();
    d.Parse(s.GetString());
    cout << s.GetString() << endl;

    CURL * curl;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/sendRegisters");
    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, s.GetString());
    curl_easy_perform(curl);


}

class Pessoa {
private:
    string name;
    lli cpf;
public:
    Pessoa(string name, lli cpf);
    ~Pessoa();
    string getData(){
      return this->name + "\n" + to_string(cpf);
    }
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


    cout << "Vai mandando os CPF ai bro\n";
    string cpf;
    while(cin >> cpf) {
        catraca->Passar(cpf);
    }

    delete catraca;

}
