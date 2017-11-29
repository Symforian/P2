#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
using namespace std;

int mysliwi, kucharze, zwierzyna, pozywienie;
int M, K;
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
			zwierzyna++;
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
			M--;
			pthread_mutex_unlock(&Lock);
		//	cout<<"\nMyśliwy:DEAD.\n";
			pthread_exit(NULL);
		}
		usleep(100);
	}
	//cout<<"\nMyśliwy:FIN.\n";
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
			K--;
			pthread_mutex_unlock(&Lock);
		//	cout<<"\nKucharz:DEAD.\n";
			pthread_exit(NULL);
		}
		usleep(100);
	}
	//cout<<"\nKucharz:FIN.\n";
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
	M = mysliwi;
	K = kucharze;
	for(int i=0; i<mysliwi; i++)
	{
		if(pthread_create(&pthread_mysliwi[i],NULL,&poluj,NULL)!=0)
			cout<<"ERROR creating mysliwy "<<i<<endl;
	}

	for(int i=0; i<kucharze; i++)
	{
		if(pthread_create(&pthread_kucharze[i],NULL,&gotuj,NULL)!=0)
			cout<<"ERROR creating kucharz "<<i<<endl;
	}

	for(int i=0; i<mysliwi; i++)
	{
		if(pthread_join(pthread_mysliwi[i],NULL)!=0)
			cout<<"ERROR joining mysliwy "<<i<<endl;
	}

	for(int j=0; j<kucharze; j++)
	{
		if(pthread_join(pthread_kucharze[j],NULL)!=0)
			cout<<"ERROR joining kucharz "<<j<<endl;
	}


	pthread_mutex_destroy(&Lock);
	cout<<"------------------------\nPo wszystkim:\nMyśliwych jest "<<M<<"\n Kucharzy jest "<<K<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	//cout<<"Myśliwych bylo "<<mysliwi<<"\n Kucharzy bylo "<<kucharze<<endl;
	return 0;
}
