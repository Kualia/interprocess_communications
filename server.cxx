#include "commons.h"

int main(int argc, char *argv[])
{
  string path = "./updates_list.txt";
  list<Program> installedApps = loadApplications(path);
  Message message;

  Program p;
  int msgid;

  if ((msgid = msgget(KEY, 0)) == -1)
    msgid = msgget(KEY, IPC_CREAT | 0660);

  if(msgid == -1){
    cout << "Error while creating message queue\n";
  }


  while (true)
  {
    memset(message.buffer, 0x0, sizeof(message.buffer));
    int n = msgrcv(msgid, (void *) &message, sizeof(Message), SERVER, 0);
    cout << "N: " << n <<endl;

    if(n>0){
      /* REMOVE */
      cout << "Message from: " << message.msg_fm << ":" << endl;
      cout << "Message to: " << message.msg_to << ":" << endl;
      cout << "Message char buffer: " << message.buffer << ":" << endl;
      string msg = (string) message.buffer;
      
      p.id = stoi(msg.substr(0, msg.find(',')));
      p.name = msg.substr(msg.find(',')+1, msg.find_last_of(',') - msg.find(',') -1);
      p.version = msg.substr(msg.find_last_of(',')+1);

      message.msg_to = message.msg_fm;
      message.msg_fm = SERVER;
      
      for(Program pr: installedApps){
        if(pr.id != p.id) continue;

        int shmid;
        char c, *shm;
        
        shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0660);
        shm = (char *)shmat(shmid, 0, 0);
        memcpy(shm, &pr.link, sizeof(shm));

        msg = (pr.version.c_str() == p.version.c_str())? NONEEDUPDATE: DOWNLOADFROMPRIVATECHANNEL + to_string(shmid);
        cout << msg << endl;
        memcpy(message.buffer, msg.c_str(), sizeof(message.buffer));
        msgsnd(msgid, (void *) &message, sizeof(Message), 0);
      }

    }
  }
  
  msgctl(msgid, IPC_RMID, 0);
  return 0;
}