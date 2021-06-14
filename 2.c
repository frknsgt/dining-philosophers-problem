#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define N_FILOZOFLAR 5

enum pdurum {DUSUNME,AC,YIYOR};  
enum pdurum durum[5];


int filozof_id[N_FILOZOFLAR];     
pthread_cond_t condition[N_FILOZOFLAR];
pthread_mutex_t mutex_lock;              
pthread_t filozoflar[N_FILOZOF]; 


void *filozof(void *prm);
void test(int i);           
void cubuk_almak(int philo_id); 
void cubuk_birakmak(int philo_id); 
extern inline int sol_kontrol(int philo_id); 
extern inline int sag_kontrol(int philo_id);


int main(void)
{
  int i;
  for(i=0; i<N_FILOZOFLAR; i++){ 
     durum[i]=DUSUNME;
     printf("Filozof %d dusunuyor\n",i+1); 
     filozof_id[i]=i;          
     pthread_cond_init(&condition[i],NULL);
  }
  
  pthread_mutex_init(&mutex_lock,NULL); 
  
  for(i=0; i<N_FILOZOFLAR; i++){
    pthread_create(&filozoflar[i],0,filozof,(void*)&filozof_id[i]); 
  }
  
  for(i=0; i<N_FILOZOFLAR; i++)
     pthread_join(filozoflar[i],NULL); 
     
  return 0;
} 



inline int sol_kontrol(int philo_id) 
{
  return ((philo_id == 0) ? N_FILOZOFLAR-1 : philo_id-1);
}

inline int sag_kontrol(int philo_id)
{
  return ((philo_id == N_FILOZOFLAR-1) ? 0 : philo_id+1);
}


void test(int i)
{
  if(   (durum[sol_kontrol(i)] != YIYOR)       
    &&(durum[sag_kontrol(i)] !=YIYOR) 
    && (durum[i] !=YIYOR ) ) {
       durum[i]=YIYOR;                   
       pthread_cond_signal(&condition[i]);
     }
}

void cubuk_almak(int philo_id)
{
  pthread_mutex_lock(&mutex_lock);
  
 
  test(philo_id);
  
  while(durum[philo_id] !=YIYOR) { 
  
       pthread_cond_wait(&condition[philo_id],&mutex_lock); 
    }
    
    printf("Filozof %d yiyor\n",philo_id+1); 
      
    pthread_mutex_unlock(&mutex_lock); 
 

}

void cubuk_birakmak(int philo_id)
{
   pthread_mutex_lock(&mutex_lock);
   durum[philo_id]=DUSUNME;  

   printf("Filozof %d dusunuyor\n",philo_id+1); 
  
   test(sol_kontrol(philo_id));
   test(sag_kontrol(philo_id));
   
   pthread_mutex_unlock(&mutex_lock); 
}


void *filozof(void *prm)
{
  int *philo_id = prm; 
  int uyuma_zaman;
  
  srandom((unsigned) time(NULL)); 
  
  while(1){
  
    
      
      uyuma_zaman=(int)((random() % 5)+1); 
      sleep(uyuma_zaman);
      
      cubuk_almak(*philo_id);  
      uyuma_zaman=(int)((random() % 3)+1); 
      sleep(uyuma_zaman);
      
      cubuk_birakmak(*philo_id);
      
      
      }
 }