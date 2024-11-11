#ifndef NEOS_JOB
#define NEOS_JOB

#include <string>
#include <vector>

using namespace std;

class NEOSJob {
  public:
    NEOSJob();
    ~NEOSJob();
    void setServerUrl(string serverUrl);
    void setCategory(string category);
    void setSolver(string solver);
    void setInputMethod(string inputMethod);
    void setEmail(string email);
    void setDat1(string dat1);
    void setDat2(string dat2);
    void setTsp(string tsp);
    void setAlgType(string algType);
    void setRDType(string rdType);
    void setPLType(string plType);
    void setComment(string comment);

    string getServerUrl();
    string getCategory();
    string getSolver();
    string getInputMethod();
    string getEmail();
    string getDat1();
    string getDat2();
    string getTsp();
    string getAlgType();
    string getRDType();
    string getPLType();
    string getComment();

    vector<int> submit();
  private:
    bool serverUp;

    string serverUrl;
    string category;
    string solver;
    string inputMethod;
    string email;
    string dat1;
    string dat2;
    string tsp;
    string algType;
    string rdType;
    string plType;
    string comment;
};

#endif