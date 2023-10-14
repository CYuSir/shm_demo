#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

using namespace std;

int main() {
    // create shm
    int shmId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmId == -1) {
        cerr << "Failed to create shared memory." << endl;
        return -1;
    }

    int* sharedDate = (int*)shmat(shmId, nullptr, 0);
    if (sharedDate == (int*)-1) {
        cerr << "Failed to attach shared memory." << endl;
        return -1;
    }

    *sharedDate = 42;

    // create child pid
    pid_t pid = fork();
    if (pid == -1) {
        cerr << "Failed to fork." << endl;
    }
    if (pid == 0) {
        // child
        cout << "Child process: get Shared data is " << *sharedDate << endl;

        // child write data to shm
        *sharedDate = 100;
        // del link
        shmdt(sharedDate);
    } else {
        // father
        // wait child
        wait(nullptr);

        cout << "Parent process: get Shared data is " << *sharedDate << endl;
        // del link
        shmdt(sharedDate);

        // del shm
        shmctl(shmId, IPC_RMID, nullptr);
    }

    return 0;
}