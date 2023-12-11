#include "commons.h"

int main(int argc, char *argv[]) {

  string path = "./updates_list.txt";
  list<Program> installedApps = loadApplications(path);
  int msgid;
  pid_t pid = getpid();
  int n;

  Message message;
  memset(message.buffer, 0x0, sizeof(message.buffer));
  message.msg_to = SERVER;
  message.msg_fm = pid;

  Program p;
  p.id = 43;
  p.name = "CinsBrowser";
  p.version = "12";
  p.link = "";
  
  string msg = to_string(p.id) + "," + p.name + "," + p.version; 
  memcpy(message.buffer, msg.c_str(), strlen(msg.c_str()));

  if((msgid = msgget(KEY, 0)) == -1)
    msgid = msgget(KEY, IPC_CREAT | 0666);

  
  n = msgsnd(msgid, (void *) &message, sizeof(Message), 0);


  n = msgrcv(msgid, (void *) &message, sizeof(Message), pid, 0);
  cout << "Message from: " << message.msg_fm << ":" << endl;
  cout << "Message to: " << message.msg_to << ":" << endl;
  cout << "Message char buffer: " << message.buffer << ":" << endl;

  cout << "returned message: " << message.buffer << endl;
  msg = message.buffer;

  // if
  int shmid = stoi(msg.substr(DOWNLOADFROMPRIVATECHANNEL.length()));
  cout << shmid << endl;
  char* shm = (char *)shmat(shmid, 0, 0);

  cout << shm[0] << endl;
  cout << shm[1] << endl;
  cout << shm[2] << endl;
  cout << shm[3] << endl;


  shmctl(shmid, IPC_RMID, 0);
  return 0;
}