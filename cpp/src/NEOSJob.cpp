#include "NEOSJob.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "GraphReader.h"
#include <xmlrpc-c/client_simple.hpp>

using namespace std;

NEOSJob::NEOSJob() {
    serverUrl = "https://neos-server.org:3333";
    category = "";
    solver = "";
    inputMethod = "";
    email = "";
    dat1 = "";
    dat2 = "";
    tsp = "";
    algType = "";
    rdType = "";
    plType = "";
    comment = "";

    xmlrpc_c::clientSimple client;
    xmlrpc_c::value result;

    client.call(serverUrl, "ping", &result);

    string response = xmlrpc_c::value_string(result);

    if (response == "NeosServer is alive\n") {
        cout << "NEOS server is up and running" << endl;
        serverUp = true;
    } else {
        cout << "NEOS server is down" << endl;
        serverUp = false;
    }
}

NEOSJob::~NEOSJob() {}

void NEOSJob::setServerUrl(string serverUrl) {
    this->serverUrl = serverUrl;
}

void NEOSJob::setCategory(string category) {
    this->category = category;
}

void NEOSJob::setSolver(string solver) {
    this->solver = solver;
}

void NEOSJob::setInputMethod(string inputMethod) {
    this->inputMethod = inputMethod;
}

void NEOSJob::setEmail(string email) {
    this->email = email;
}

void NEOSJob::setDat1(string dat1) {
    this->dat1 = dat1;
}

void NEOSJob::setDat2(string dat2) {
    this->dat2 = dat2;
}

void NEOSJob::setTsp(string tsp) {
    this->tsp = tsp;
}

void NEOSJob::setAlgType(string algType) {
    this->algType = algType;
}

void NEOSJob::setRDType(string rdType) {
    this->rdType = rdType;
}

void NEOSJob::setPLType(string plType) {
    this->plType = plType;
}

void NEOSJob::setComment(string comment) {
    this->comment = comment;
}

string NEOSJob::getServerUrl() {
    return serverUrl;
}

string NEOSJob::getCategory() {
    return category;
}

string NEOSJob::getSolver() {
    return solver;
}

string NEOSJob::getInputMethod() {
    return inputMethod;
}

string NEOSJob::getEmail() {
    return email;
}

string NEOSJob::getDat1() {
    return dat1;
}

string NEOSJob::getDat2() {
    return dat2;
}

string NEOSJob::getTsp() {
    return tsp;
}

string NEOSJob::getAlgType() {
    return algType;
}

string NEOSJob::getRDType() {
    return rdType;
}

string NEOSJob::getPLType() {
    return plType;
}

string NEOSJob::getComment() {
    return comment;
}

vector<int> NEOSJob::submit() {
    string xml = "";
    xml += "<document>\n";
    xml += "<category>" + category + "</category>\n";
    xml += "<solver>" + solver + "</solver>\n";
    xml += "<inputMethod>" + inputMethod + "</inputMethod>\n";
    xml += "<email><![CDATA[" + email + "]]></email>\n";
    xml += "<dat1><![CDATA[" + dat1 + "]]></dat1>\n";
    xml += "<dat2><![CDATA[" + dat1 + "]]></dat2>\n";
    xml += "<tsp><![CDATA[" + tsp + "]]></tsp>\n";
    xml += "<ALGTYPE><![CDATA[" + algType + "]]></ALGTYPE>\n";
    xml += "<RDTYPE><![CDATA[" + rdType + "]]></RDTYPE>\n";
    xml += "<PLTYPE><![CDATA[" + plType + "]]></PLTYPE>\n";
    xml += "<comment><![CDATA[" + comment + "]]></comment>\n";
    xml += "</document>";

    xmlrpc_c::clientSimple client;
    xmlrpc_c::value result_submit;

    client.call(serverUrl, "submitJob", "s", &result_submit, xml.c_str());

    auto result_value = xmlrpc_c::value_array(result_submit);
    int job_number = xmlrpc_c::value_int(result_value.vectorValueValue()[0]);
    string password = xmlrpc_c::value_string(result_value.vectorValueValue()[1]);

    cout << "Job number: " << job_number << " " << "Password: " << password << endl;

    if (job_number == 0) {
        cerr << "NEOS Server error: " << password << endl;
        return vector<int>();
    }

    cout << "Job processing \\" << flush;
    string status = "";
    int i = 0;
    while (status != "Done") {
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << "\b" << flush;
        if (i % 4 == 0) {
            cout << "|" << flush;
        } else if (i % 4 == 1) {
            cout << "/" << flush;
        } else if (i % 4 == 2) {
            cout << "-" << flush;
        } else if (i % 4 == 3) {
            cout << "\\" << flush;
        }

        if (i % 4 == 0) {
            xmlrpc_c::value result_interm;
            client.call(serverUrl, "getJobStatus", "is", &result_interm, job_number, password.c_str());
            status = xmlrpc_c::value_string(result_interm);
        }

        i++;
    }
    cout << "\b" << "Done" << endl;

    xmlrpc_c::value result_final;
    client.call(serverUrl, "getFinalResults", "is", &result_final, job_number, password.c_str());
    auto final_results_bytes = xmlrpc_c::value_bytestring(result_final).cvalue();

    string final_results_string(final_results_bytes.begin(), final_results_bytes.end());
    final_results_string = final_results_string.substr(
        final_results_string.find("integers***\n\n") + 13,
        final_results_string.length() - final_results_string.find("integers***\n\n") - 13);

    return GraphReader::readNeosTour(final_results_string);
}