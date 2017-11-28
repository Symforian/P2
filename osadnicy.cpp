#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;
int Czas = 365;
sem_t Time;
int kostka()
{
	return rand()%6+1;
}
int mysliwi, kucharze, zwierzyna, pozywienie;
//int sleeping=0;
int blocked=0;
int today = 0;
pthread_mutex_t zwierzynaLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pozywienieLock = PTHREAD_MUTEX_INITIALIZER; ;//, 
pthread_mutex_t BlockLock = PTHREAD_MUTEX_INITIALIZER;
void *poluj(void*)
{
	//bool sleep = false;
	for(int i =0; i<Czas; i++)
	{
	cout<<"Myśliwy żyje dzień "<<i<<endl;
	int mys = kostka();
	int zwierz = kostka();
	if(mys>zwierz)
	{
	pthread_mutex_lock(&zwierzynaLock);
	if(zwierzyna>0)
		{
		zwierzyna--;
		pthread_mutex_unlock(&zwierzynaLock);
		pthread_mutex_lock(&pozywienieLock);
		pozywienie++;
		pthread_mutex_unlock(&pozywienieLock);
		cout<<"\nMyśliwy:UPOLOWAŁ.\n";
		}
	else
	pthread_mutex_unlock(&zwierzynaLock);
	}
	pthread_mutex_lock(&pozywienieLock);
	if(pozywienie>0)
	{	
	pozywienie--;
	pthread_mutex_unlock(&pozywienieLock);
	cout<<"\nMyśliwy:ZJADŁ.\n";
	}	
	else
	{
	pthread_mutex_unlock(&pozywienieLock);
	cout<<"\nMyśliwy:DEAD.\n";
	mysliwi--;
	break;
	}
	int wut;

	cout<<"\nMyśliwy:ZzZzZz.\n";//<<wut<<endl;
	//while(sem_wait(&Time)!=-1)
	//if(sem_wait(&Time)==-1 && errno == EINTR)
	//while(
	//while(blocked==-1)
	//usleep(1);
	blocked++;
	//for(int i=0; i<mysliwi;i++)
	//sem_getvalue(&Time,&wut);
	//cout<<wut<<endl;
	if(sem_wait(&Time)==-1)
	perror("Error sem wait");
	
	while(true)
	{
	//cout<<"zzzz"<<endl;
	if(today>i)
	break;
	}

	//else 
	//	break;
	//sem_getvalue(&Time,&wut);
	cout<<"\nMyśliwy wstał.\n";//<<wut<<endl;
	}

	return NULL;
}


int main( int argc, char * argv[] )
{
	srand(time(NULL));
	//argv[0] mysliwi 
	//argv[1] kucharze
	//argv[2] zwierzyna
	//argv[3] pozywienie
	mysliwi = 10;
	kucharze = 11;
	zwierzyna = 3;
	pozywienie = 7;

	if(argc!=1)
	{
		 for(;argc>0;argc--)
		{
			if(argc==1)
			mysliwi = atoi(argv[argc]);
			else if(argc==2)
			kucharze = atoi(argv[argc]);
			else if(argc==3)
			zwierzyna = atoi(argv[argc]);
			else if(argc==4)
			pozywienie = atoi(argv[argc]);

		}
	}
	/*if (pthread_mutex_init(&zwierzynaLock,NULL) != 0)
	{
		printf("\n zwierzyna mutex init failed\n");
		return 1;
	}
	if (pthread_mutex_init(&pozywienieLock,NULL) != 0)
	{
		printf("\n pozywienie mutex init failed\n");
		return 1;
	}*/
	//if (pthread_mutex_init(&BlockLock,NULL) != 0)
	//{
	//	printf("\n BlockLock mutex init failed\n");
	//	return 1;
	//}
	cout<<"Myśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	int byloM = mysliwi;
	//int byloK = kucharze; 
	pthread_t pthread_mysliwi[mysliwi];
	//cout<<sleeping<<"<"<<endl;
	if(sem_init(&Time, 0, 0)==-1)
	{
		printf("\n Semafor init error\n");
		return 1;
	}
	//cout<<sleeping<<"<"<<endl;
	//if(sem_open("Time",5)==SEM_FAILED)
	//{
	//	cout<<"\n Semafor init error\n"<<sem_open("Time",O_CREAT);
	//	return 1;
	//}
	//else
	//Time = *sem_open("Times",O_CREAT);
	

	//sem_getvalue(&Time,&sleeping);
	//cout<<sleeping<<endl;
	for(int i=0; i<mysliwi; i++)
	{
		pthread_create(&pthread_mysliwi[i],NULL,&poluj,NULL);
	}

	while(mysliwi!=0)
	{
		//sem_getvalue(&Time,&sleeping);
		if(blocked==mysliwi)//nowy dzien
		{


			cout<<"---Dzień "<<today+1<<"---"<<endl;
			cout<<"Pożywienie: "<<pozywienie<<"\tZwierzyna: "<<zwierzyna<<endl;
			//pthread_mutex_lock(&BlockLock);
			
			
			for(int i = 0; i<=blocked; i++)
				{if(sem_post(&Time)==-1)
perror("Error sem post");}
//sem_getvalue(&Time,&sleeping);
//cout<<"WSTAWAC! "<<sleeping<<endl;}
			//pthread_mutex_unlock(&BlockLock);
			today++;
			blocked=0;
		}
	//cout<<blocked<<endl;




	}
	/*for(int i =0; i<Czas; i++)
	{
	//if(sleeping+mysliwi==-1)
	//for(int i=0; i<mysliwi;)
	//{
	//	sem_post(&Time);
	//}
	sem_getvalue(&Time,&sleeping);
	cout<<"Dzień "<<i+1<<"\n Myśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl<<sleeping<<endl;
	
	if(mysliwi == 0)
	{
	cout<<"\nWszyscy myśliwi zmarli\n";
	break;
	}
	sem_getvalue(&Time,&sleeping);
	//while(sleeping+mysliwi<-1)
	//{
	//	sem_getvalue(&Time,&sleeping);
	//}

	}*/

	for(int i=0; i<byloM; i++)
	{
		pthread_join(pthread_mysliwi[i],NULL);
	}
	pthread_mutex_destroy(&zwierzynaLock);
	pthread_mutex_destroy(&pozywienieLock);
	//pthread_mutex_destroy(&BlockLock);
	sem_destroy(&Time);
	return 0;
}
