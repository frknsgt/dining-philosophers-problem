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
pthread_t filozoflar[N_FILOZOFLAR]; 

void *filozof(void *prm);
void test(int i);           
void cubuklari_alma(int filo_id); 
void cubuklari_birakma(int filo_id); 
extern inline int sol_kontrol(int filo_id); 
extern inline int sag_kontrol(int filo_id);


int main(void)
{
  int i;
  for(i=0; i<N_FILOZOFLAR; i++){ 
     durum[i]=DUSUNME;
     printf("Filozof %d  dusunuyor\n",i+1); 
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


inline int sol_kontrol(int filo_id) 
{
  return ((filo_id == 0) ? N_FILOZOFLAR-1 : filo_id-1);
}

inline int sag_kontrol(int filo_id)
{
  return ((filo_id == N_FILOZOFLAR-1) ? 0 : filo_id+1);
}


void test(int i)
{
    if(i%2==0){
  if( (durum[sol_kontrol(i)] !=YIYOR) 
    && (durum[i] !=YIYOR ) ) {
     
       durum[i]=YIYOR;                   
       pthread_cond_signal(&condition[i]);
     }
    }
    else{
    if( (durum[sag_kontrol(i)] !=YIYOR) 
    && (durum[i] !=YIYOR ) ) {
   
       durum[i]=YIYOR;                
       pthread_cond_signal(&condition[i]);
     }
    }

}

void cubuklari_alma(int filo_id)
{
  pthread_mutex_lock(&mutex_lock);
  
 
  test(filo_id);
  
  while(durum[filo_id] !=YIYOR) { 
  
       pthread_cond_wait(&condition[filo_id],&mutex_lock); 
    }
   
    printf("Filozof %d is yiyor\n",filo_id+1); 
      
    pthread_mutex_unlock(&mutex_lock); 

}

void cubuklari_birakma(int filo_id)
{
   pthread_mutex_lock(&mutex_lock);
   durum[filo_id]=DUSUNME;  
   printf("Filozof %d  dusunuyor\n",filo_id+1); 
  
   test(sol_kontrol(filo_id));
   test(sag_kontrol(filo_id));

   pthread_mutex_unlock(&mutex_lock); 
}


void *filozof(void *prm)
{
  int *filo_id = prm; 
  int uyuma_zaman;
  
  srandom((unsigned) time(NULL)); 
  
  while(1){
    
      uyuma_zaman=(int)((random() % 2)+1); 
      sleep(uyuma_zaman);
      
      cubuklari_alma(*filo_id);  
      uyuma_zaman=(int)((random() % 2)+1); 
      sleep(uyuma_zaman);
      
      cubuklari_birakma(*filo_id);
      
     
      }
 }