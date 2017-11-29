#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
using namespace std;

int mysliwi, kucharze, zwierzyna, pozywienie;
pthread_mutex_t Lock = PTHREAD_MUTEX_INITIALIZER;

int kostka()
{
	return rand()%6+1;
}

void *poluj(void*)
{

	for(int i =0; i<365; i++)
	{
		int mys = kostka();
		int zwierz = kostka();
		if(mys>zwierz)
		{
			pthread_mutex_lock(&Lock);
			if(zwierzyna>0)
				{
				zwierzyna--;
				//cout<<"\nMyśliwy:UPOLOWAŁ.\n";
				}
			pthread_mutex_unlock(&Lock);
		}

		pthread_mutex_lock(&Lock);
		if(pozywienie>0)
		{	
			pozywienie--;
			pthread_mutex_unlock(&Lock);
		//	cout<<"\nMyśliwy:ZJADŁ.\n";
		}	
		else
		{
			mysliwi--;
			pthread_mutex_unlock(&Lock);
		//	cout<<"\nMyśliwy:DEAD.\n";
			pthread_exit(NULL);
		}
		usleep(100);
	}
	//cout<<"\nMyśliwy:FIN--------------------.\n";
	return NULL;
}
void *gotuj(void*)
{


	for(int i =0; i<365; i++)
	{
		int talent = kostka();
		pthread_mutex_lock(&Lock);
		if(zwierzyna>0)
		{	zwierzyna--;
			//cout<<"\nKucharz:Ugotował:"<<talent<<"\n";
			pozywienie+=talent;
		}
		pthread_mutex_unlock(&Lock);

		pthread_mutex_lock(&Lock);
		if(pozywienie>0)
		{
			pozywienie--;
			pthread_mutex_unlock(&Lock);
			//cout<<"Kucharz:ZJADŁ."<<endl;
		}
		else
		{
			kucharze--;
			pthread_mutex_unlock(&Lock);
		//	cout<<"\nKucharz:DEAD.\n";
			pthread_exit(NULL);
		}
		usleep(100);
	}
	//cout<<"\nKucharz:FIN-----------------.\n";
	return NULL;
}

int main( int argc, char * argv[] )
{
	srand(time(NULL));
	mysliwi = 0;
	kucharze = 0;
	zwierzyna = 0;
	pozywienie = 0;

	if(argc>1)
	mysliwi = atoi(argv[1]);
	if(argc>2)
	kucharze = atoi(argv[2]);
	if(argc>3)
	zwierzyna = atoi(argv[3]);
	if(argc>4)
	pozywienie = atoi(argv[4]);

	cout<<"Myśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;

	pthread_t pthread_mysliwi[mysliwi];
	pthread_t pthread_kucharze[kucharze];
	int M = mysliwi;
	int K = kucharze;
	for(int i=0; i<mysliwi; i++)
	{
		if(pthread_create(&pthread_mysliwi[i],NULL,&poluj,NULL)!=0)
			cout<<"ERROR"<<endl;
	}

	for(int i=0; i<kucharze; i++)
	{
		if(pthread_create(&pthread_kucharze[i],NULL,&gotuj,NULL)!=0)
			cout<<"ERROR"<<endl;
	}

	for(int i=0; i<M; i++)
	{
		cout<<"M"<<i<<endl;
		pthread_join(pthread_mysliwi[i],NULL);
		cout<<"M"<<i<<endl;
	}

	for(int i=0; i<K; i++)
	{
		cout<<"K"<<i<<endl;
		pthread_join(pthread_kucharze[i],NULL);
		cout<<"K"<<i<<endl;
	}


	pthread_mutex_destroy(&Lock);
	cout<<"------------------------\nPo wszystkim:\nMyśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	return 0;
}
