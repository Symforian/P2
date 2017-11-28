#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;
volatile int Czas = 365;
//sem_t Time;
int kostka()
{
	return rand()%6+1;
}
volatile int mysliwi, kucharze, zwierzyna, pozywienie;
volatile int blocked=0;
volatile int today = 0;
pthread_mutex_t zwierzynaLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pozywienieLock = PTHREAD_MUTEX_INITIALIZER; ;
pthread_mutex_t BlockLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t warunekCzekania = PTHREAD_COND_INITIALIZER;
pthread_mutex_t OsadnicyLock = PTHREAD_MUTEX_INITIALIZER;
void *poluj(void*)
{

	cout<<"TEST ILE WATKOW M"<<endl;
	for(int i =0; i<Czas; i++)
	{

	pthread_mutex_lock(&BlockLock);
	blocked++;
	while(i==today)
	{
	if(pthread_cond_wait(&warunekCzekania,&BlockLock)!=0)
	{perror("Warunek czekania mysliwi error");
		break;
		}

	}
	pthread_mutex_unlock(&BlockLock);


	//cout<<"\nMyśliwy wstał.\n";
	//cout<<"Myśliwy żyje dzień "<<i+1<<endl;
	int mys = kostka();
	int zwierz = kostka();
	if(mys>zwierz)
	{
	pthread_mutex_lock(&zwierzynaLock);
	if(zwierzyna>0)
		{
		zwierzyna--;
	//	cout<<"\nMyśliwy:UPOLOWAŁ.\n";
		}
	pthread_mutex_unlock(&zwierzynaLock);
	}
	pthread_mutex_lock(&pozywienieLock);
	if(pozywienie>0)
	{	
	pozywienie--;
	pthread_mutex_unlock(&pozywienieLock);
	//cout<<"\nMyśliwy:ZJADŁ.\n";
	}	
	else
	{
	pthread_mutex_unlock(&pozywienieLock);
	//cout<<"\nMyśliwy:DEAD.\n";
	pthread_mutex_lock(&OsadnicyLock);
	mysliwi--;
	pthread_mutex_unlock(&OsadnicyLock);
	pthread_exit(NULL);
	}


	//cout<<"\nMyśliwy:ZzZzZz.\n";


	}

	return NULL;
}
void *gotuj(void*)
{

	cout<<"TEST ILE WATKOW K"<<endl;
	for(int i =0; i<Czas; i++)
	{


	
	if(pthread_mutex_lock(&BlockLock)!=0)
	perror("Mutex lock BL kucharz error");
	blocked++;
	while(i==today)
	{
	if(pthread_cond_wait(&warunekCzekania,&BlockLock)!=0)
	{
perror("Warunek czekania kucharze error");

}

	}
	if(pthread_mutex_unlock(&BlockLock)!=0)
	perror("Mutex unlock BL kucharz error");

	//cout<<"\nKucharz wstał.\n";
	//cout<<"Kucharz żyje dzień "<<i+1<<endl;
	
	int talent = kostka();
	pthread_mutex_lock(&zwierzynaLock);
	if(zwierzyna>0)
	{	zwierzyna--;
		pthread_mutex_unlock(&zwierzynaLock);
	//	cout<<"\nKucharz:Ugotował:"<<talent<<"\n";
		pthread_mutex_lock(&pozywienieLock);
		pozywienie+=talent;
		pthread_mutex_unlock(&pozywienieLock);
	}
	else
	pthread_mutex_unlock(&zwierzynaLock);
	pthread_mutex_lock(&pozywienieLock);
	if(pozywienie>0)
	{
	pozywienie--;
	pthread_mutex_unlock(&pozywienieLock);
	//cout<<"Kucharz:ZJADŁ."<<endl;
	}
	else
	{
	pthread_mutex_unlock(&pozywienieLock);
	//cout<<"\nKucharz:DEAD.\n";
	pthread_mutex_lock(&OsadnicyLock);
	kucharze--;
	pthread_mutex_unlock(&OsadnicyLock);
	pthread_exit(NULL);
	}



	//cout<<"\nKucharz:ZzZzZz.\n";

	
	}

	return NULL;
}

int main( int argc, char * argv[] )
{
	srand(time(NULL));
	//argv[1] mysliwi 
	//argv[2] kucharze
	//argv[3] zwierzyna
	//argv[4] pozywienie
	mysliwi = 10;
	kucharze = 11;
	zwierzyna = 3;
	pozywienie = 7;

	if(argc>1)
	mysliwi = atoi(argv[1]);
	if(argc>2)
	kucharze = atoi(argv[2]);//cout<<"seg"<<endl;}
	if(argc>3)
	zwierzyna = atoi(argv[3]);
	if(argc>4)
	pozywienie = atoi(argv[4]);
	int byloM = mysliwi;
	int byloK = kucharze;

	cout<<"Myśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	pthread_t pthread_mysliwi[mysliwi];
	pthread_t pthread_kucharze[mysliwi];


	for(int i=0; i<byloM; i++)
	{
		if(pthread_create(&pthread_mysliwi[i],NULL,&poluj,NULL)!=0)
		perror("Mysliwy create error");
	}
	for(int i=0; i<byloK; i++)
	{
		//cout<<"Kolejny kucharz"<<endl;
		if(pthread_create(&pthread_kucharze[i],NULL,&gotuj,NULL)!=0)
		perror("Kucharz create error");
	}
	//cout<<"M"<<endl;
	/*	for(int i=0; i<byloM; i++)
	{
		cout<<&pthread_mysliwi[i]<<"\t"<<i<<endl;
	}
	//cout<<"K"<<endl;
	for(int i=0; i<byloK; i++)
	{
		cout<<&pthread_kucharze[i]<<"\t"<<i<<endl;
	}*/

	while(today<Czas)
	{
		cout<<blocked<<" "<<mysliwi+kucharze<<endl;
		pthread_mutex_lock(&OsadnicyLock);
		if(blocked==mysliwi+kucharze)//nowy dzien
		{

			cout<<"---Dzień "<<today+1<<"---"<<endl;
			cout<<"Pożywienie: "<<pozywienie<<"\tZwierzyna: "<<zwierzyna<<endl;
			cout<<"Myśliwi: "<<mysliwi<<"\tKucharze: "<<kucharze<<endl<<endl;
			if(mysliwi+kucharze==0)
				{
pthread_mutex_unlock(&OsadnicyLock);

				today=Czas;}

			pthread_mutex_unlock(&OsadnicyLock);

			today++;
			blocked=0;
			if(
			pthread_cond_broadcast(&warunekCzekania)!=0)
			perror("Condition broadcast error");





		}
		pthread_mutex_unlock(&OsadnicyLock);



	}
	
//	cout<<byloM<<" "<<byloK<<endl;
	for(int i=0; i<byloM; i++)
	{
		//if(pthread_mysliwi[i]!=)
		//if(
pthread_join(pthread_mysliwi[i],NULL);
//!=0)
//			perror("Error join mysliwi");
	}

	for(int i=0; i<byloK; i++)
	{
		//if(pthread_kucharze[i]!=NULL)
		//if(
	cout<<"i "<<i<<endl;
pthread_join(pthread_kucharze[i],NULL);
		cout<<"i "<<i<<endl;
//!=0)
//			perror("Error join kucharze");
	}
	
//	cout<<byloM<<" "<<byloK<<endl;
	pthread_mutex_destroy(&zwierzynaLock);
	pthread_mutex_destroy(&pozywienieLock);
	pthread_mutex_destroy(&BlockLock);
	pthread_mutex_destroy(&OsadnicyLock);
	pthread_cond_destroy(&warunekCzekania);
	//sem_destroy(&Time);
	return 0;
}
