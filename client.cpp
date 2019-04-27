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
};

Catraca::Catraca(int id, vector<string> types){
    this->id = id;
    this->types = types;
    HttpGetList();
}

void Catraca::Passar(string cpf){
    map<string,string>::iterator i;
    if((i = this->permitidos.find(cpf)) != this->permitidos.end()){
        this->passadas.push_back({i->first, i->second});
    }else{
        this->rejeitadas.push_back(cpf);
    }

    //if(this->passadas.size() + this->rejeitadas.size() > 100){
    this->HttpSendBuffer();
    //}
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

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9999/getRegisters");
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_perform(curl);

    //cout << endl << data << endl;
    //cin.get();

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    //cout << data << endl;
    Document d;
    d.Parse(data.c_str());
    assert(d.IsObject());
    for(string permitido : this->types){
        Value& attr = d[permitido.c_str()];
        assert(attr.IsArray());
        for(Value::ConstValueIterator itr = attr.Begin(); itr != attr.End(); itr++){
            const Value& itr2 = *itr;
            assert(itr2.IsObject());
            for(Value::ConstMemberIterator item = itr2.MemberBegin(); item != itr2.MemberEnd(); item++)
                permitidos.insert({ item->name.GetString(), item->value.GetString() });
        }
    }

}

void Catraca::HttpSendBuffer() {
    Document d;
    d.SetObject();

    Value array(kArrayType);
    Document::AllocatorType& allocator = d.GetAllocator();
    for(auto& s : this->passadas){
        //string tmp = s.first + " : " + s.second;
        Value obj(kObject);
        Value tmp;
        obj.SetObject();
        //tmp.SetString(s.second.c_str(), allocator);
        obj.AddMember("name", s.second.c_str(), allocator);
        //tmp.SetString(s.first.c_str(), allocator);
        obj.AddMember("cpf", s.first.c_str(), allocator);
        array.PushBack(obj, allocator);
    }

    d.AddMember("Passaram", array, allocator);

    Value arr(kArrayType);
    for(string s : this->rejeitadas){
        Value obj;
        obj.SetObject();
        obj.AddMember("cpf", s.c_str(),allocator);
        arr.PushBack(obj, allocator);
    }

    d.AddMember("Falharam", arr, allocator);

    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    d.Accept(writer);

    cout << strbuf.GetString() << endl;

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

    cout << "Vai mandando os CPF ai bro\n";
    string cpf;
    while(cin >> cpf) {
        catraca->Passar(cpf);
    }


}
