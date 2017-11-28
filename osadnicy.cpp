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
int blocked=0;
int today = 0;
pthread_mutex_t zwierzynaLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pozywienieLock = PTHREAD_MUTEX_INITIALIZER; ;
pthread_mutex_t BlockLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t warunekCzekania = PTHREAD_COND_INITIALIZER;
void *poluj(void*)
{

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


	cout<<"\nMyśliwy:ZzZzZz.\n";

	blocked++;

	//if(sem_wait(&Time)==-1)
	//perror("Error sem wait myśliwy");
		
	pthread_mutex_lock(&BlockLock);
	while(i==today)
	{
	pthread_cond_wait(&warunekCzekania,&BlockLock);
//	cout<<"mys"<<endl;
	//if(today>i)
	//break;
	}
	pthread_mutex_unlock(&BlockLock);


	cout<<"\nMyśliwy wstał.\n";
	}

	return NULL;
}
void *gotuj(void*)
{

	for(int i =0; i<Czas; i++)
	{
	cout<<"Kucharz żyje dzień "<<i<<endl;
	
	int talent = kostka();
	pthread_mutex_lock(&zwierzynaLock);
	

	if(zwierzyna>0)
	{	zwierzyna--;
		pthread_mutex_unlock(&zwierzynaLock);
		cout<<"\nKucharz:Ugotował:"<<talent<<"\n";
	}
	else
	pthread_mutex_unlock(&zwierzynaLock);
	
	pozywienie+=talent;
	if(pozywienie>0)
	{
	pozywienie--;
	pthread_mutex_unlock(&pozywienieLock);
	cout<<"Kucharz:ZJADŁ."<<endl;
	}
	else
	{
	pthread_mutex_unlock(&pozywienieLock);
	cout<<"\nKucharz:DEAD.\n";
	kucharze--;
	break;
	}



	cout<<"\nKucharz:ZzZzZz.\n";

	blocked++;

	//if(sem_wait(&Time)==-1)
	//perror("Error sem wait kucharz");
	
	pthread_mutex_lock(&BlockLock);
	while(i==today)
	{
	pthread_cond_wait(&warunekCzekania,&BlockLock);
//	cout<<"mys"<<endl;
	//if(today>i)
	//break;
	}
	pthread_mutex_unlock(&BlockLock);


	cout<<"\nKucharz wstał.\n";
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
cout<<"Hello "<<argc<<endl;
	if(argc>1)
	mysliwi = atoi(argv[1]);
	if(argc>2)
	kucharze = atoi(argv[2]);//cout<<"seg"<<endl;}
	if(argc>3)
	zwierzyna = atoi(argv[3]);
	if(argc>4)
	pozywienie = atoi(argv[4]);


	cout<<"Myśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	int byloM = mysliwi;
	int byloK = kucharze; 
	pthread_t pthread_mysliwi[mysliwi];
	pthread_t pthread_kucharze[mysliwi];

	if(sem_init(&Time, 0, 0)==-1)
	{
		printf("\n Semafor init error\n");
		return 1;
	}


	for(int i=0; i<mysliwi; i++)
	{
		pthread_create(&pthread_mysliwi[i],NULL,&poluj,NULL);
	}
	for(int i=0; i<kucharze; i++)
	{
		pthread_create(&pthread_kucharze[i],NULL,&gotuj,NULL);
	}
	//int tick = 0;
	while(mysliwi+kucharze!=0 && today<Czas)
	{

		if(blocked==mysliwi+kucharze)//nowy dzien
		{

pthread_mutex_lock(&BlockLock);
			cout<<"---Dzień "<<today+1<<"---"<<endl;
			cout<<"Pożywienie: "<<pozywienie<<"\tZwierzyna: "<<zwierzyna<<endl;
			cout<<"Myśliwi: "<<mysliwi<<"\tKucharze: "<<kucharze<<endl;
			blocked=0;
			pthread_cond_broadcast(&warunekCzekania);
			//for(int i = 0; i<=blocked; i++)
			//	{
			//	if(sem_post(&Time)==-1)
			//	perror("Error sem post");
			//	}

			today++;

pthread_mutex_unlock(&BlockLock);
		}
	//	if(tick%200==0)
		//cout<<blocked<<"\t"<<mysliwi<<"\t"<<kucharze<<endl;
	//	tick++;

	}
	

	for(int i=0; i<byloM; i++)
	{
		pthread_join(pthread_mysliwi[i],NULL);
	}
	for(int i=0; i<byloK; i++)
	{
		pthread_join(pthread_kucharze[i],NULL);
	}
	pthread_mutex_destroy(&zwierzynaLock);
	pthread_mutex_destroy(&pozywienieLock);
	sem_destroy(&Time);
	return 0;
}
