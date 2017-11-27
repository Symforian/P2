#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <time.h>
using namespace std;
int kostka()
{
	return rand()%6+1;
}
int mysliwi, kucharze, zwierzyna, pozywienie;
pthread_mutex_t zwierzynaLock;
pthread_mutex_t pozywienieLock;
void *poluj(void*)
{
	bool sleep = false;
	while(!sleep)
	{
	pthread_mutex_lock(&zwierzynaLock);
	int mys = kostka();
	int zwierz = kostka();
	if(mys>zwierz)
	{
	zwierzyna--;
	pozywienie++;
	}
	sleep = true;
	pthread_mutex_unlock(&zwierzynaLock);
	}

	return NULL;
}
/*void *what_to_tell_a(void*)
{
	cout<<"Hello world, thread a there!\n";
	return NULL;
}
void *what_to_tell_b(void*)
{
	cout<<"Hello world, thread b there!\n";
	return NULL;
}
void *what_to_tell_c(void*)
{
	cout<<"Hello world, thread c there!\n";
	return NULL;
}
void *make_some_threads(void*)
{
	pthread_t a,b,c;
	pthread_create(&a, NULL, &what_to_tell_a,NULL);
	pthread_create(&b, NULL, &what_to_tell_b,NULL);
	pthread_create(&c, NULL, &what_to_tell_c,NULL);
	pthread_join(a,NULL);
	pthread_join(b,NULL);
	pthread_join(c,NULL);
	cout<<"Hello world, parent thread there!\n";
	void* result=NULL;
	return result;
}*/

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
	if (pthread_mutex_init(&zwierzynaLock,NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return 1;
	}
	cout<<"Myśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	pthread_t pthread_mysliwi[mysliwi];
	for(int i=0; i<mysliwi; i++)
	{
		pthread_create(&pthread_mysliwi[i],NULL,&poluj,NULL);
	}
	//pthread_t parent_thread;
	//pthread_create(&parent_thread, NULL, &make_some_threads,NULL);
	//pthread_join(parent_thread,NULL);
	//cout<<"Hello world, main there!\n";
	for(int i =0; i<365; i++)
	{
	cout<<"Dzień "<<i+1<<"\nMyśliwych jest "<<mysliwi<<"\n Kucharzy jest "<<kucharze<<"\n Zwierzyny jest "<<zwierzyna<<"\n Pożywienia jest "<<pozywienie<<endl;
	}
	//for(int i = 0; i < 10; i++)
	//	cout<<kostka()<<endl;
	for(int i=0; i<mysliwi; i++)
	{
		pthread_join(pthread_mysliwi[i],NULL);
	}
	pthread_mutex_destroy(&zwierzynaLock);
	return 0;
}
