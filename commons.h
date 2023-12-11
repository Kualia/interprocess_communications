#include <iostream>
#include <string>
#include <list>
#include <cstring>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

const long int SERVER = 1L; 
const string NONEEDUPDATE = "no need to update";
const string DOWNLOADFROMPRIVATECHANNEL = "download  from  private  channel";
int KEY = 12345;
const int SHM_SIZE = 100;

struct Program
{
  int id;
  string version;
  string name;
  string link;
};

struct Message
{
  long int msg_to;
  long int msg_fm;
  char buffer[256];
};

list<Program> loadApplications(string path)
{

  list<Program> applications = list<Program>();
  list<Program>::iterator it = applications.begin();
  ifstream file("updates_list.txt");
  string str_line;
  int i = 0;

  if (!file.is_open())
  {
    cout << "Error while reading programs list" << endl;
    return applications;
  }

  while (getline(file, str_line))
  {
    Program p;

    int idPos = str_line.find("Program ID: ") + 12;
    int versionPos = str_line.find("Program Version: ") + 17;
    int namePos = str_line.find("Program Name: ") + 14;
    int urlPos = str_line.find("Download URL: ") + 14;

    p.id = stoi(str_line.substr(idPos, str_line.find(',', idPos) - idPos));
    p.version = str_line.substr(versionPos, str_line.find(',', versionPos) - versionPos);
    p.name = str_line.substr(namePos, str_line.find(',', namePos) - namePos);
    p.link = str_line.substr(urlPos);

    cout << "Program ID: " << p.id << " Program Version: " << p.version << " Program Name: " << p.name << " Download URL: " << p.link << '\n';
    
    applications.push_front(p);
  }

  file.close();

  return applications;
}