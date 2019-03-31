#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t mutex, writeblock;
int data = 0, rcount = 0;

void *reader(void *arg)
{
    int f;
    f = ((int) arg);
    sem_wait(&mutex);
    rcount = rcount + 1;
    if (rcount == 1)
      sem_wait(&writeblock);
    sem_post(&mutex);
    printf("Data read by the reader%d is %d\n", f, data);
    sleep(1);
    sem_wait(&mutex);
    rcount = rcount - 1;
    if (rcount == 0)
      sem_post(&writeblock);
    sem_post(&mutex);
}

void *writer(void *arg)
{

    int f;
    f = ((int) arg);
    sem_wait(&writeblock);
    data++;
    printf("Data written by the writer%d is %d\n", f, data);
    sleep(1);
    sem_post(&writeblock);

}

int main()
{
  int i,b;
  pthread_t readThread[5],writeThread[5];
  sem_init(&mutex,0,1);
  sem_init(&writeblock,0,1);

  for(i=0;i<=3;i++) {
    pthread_create(&writeThread[i], NULL, writer, (void *) i);
    pthread_create(&readThread[i], NULL, reader, (void *) i);
  }
  for(i=0;i<=3;i++)
  {
    pthread_join(writeThread[i],NULL);
    pthread_join(readThread[i],NULL);
  }
  return 0;
}