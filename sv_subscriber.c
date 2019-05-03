/*
 * sv_subscriber_example.c
 *
 * programme pour l'acquistion de Sampled Values (SV) : subscriber
 *
 */

#include <iec61850_server.h>
#include <sv_subscriber.h>
#include <hal_thread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <pthread.h>

typedef struct data_{

   SVReceiver receiver;
 
}data_;

/**
 *  la fonction quality prend en paramètre la qualité (entier) de la donnée reçu 
 * elle renvoie une chaîne de caractère correpondant  la qualité
**/
char* quality (Quality Q){
  
    char* q = 0;
    switch(Q){

	  case  8192 :
		  q = "DERIVED"; 
		  break;
	  case  64 :
		  q = "DETAIL:FAILURE"; 
		  break;
	  case 16  :	
		  q = "DETAIL:INACCURATE";
		  break;
	  case 512 :	
		  q = "DETAIL:INACCURATE";
		  break;
	  case 256 :	
		  q = "DETAIL:INCONSISTENT";
		  break;
	  case 128 :	
		  q = "DETAIL:OLD_DATA";
		  break;
	  case 32:
		  q = "DETAIL:OSCILLATORY";
		  break;
	  case 8:
		  q = "DETAIL:OUT_OF_RANGE";
		  break;
	  case 4:
		  q = "DETAIL:OVERFLOW";
		  break;
	  case 4096 :	
		  q = "OPERATOR_BLOCKED";
		  break;
	  case 1024 :	
		  q = "SOURCE_SUBSTITUTED";
		  break;
	  case 2048 :	
		  q = "TEST";
		  break;
	  case 0:
		  q = "VALIDITY:GOOD";
		  break;
	  case 2	:
		  q = "VALIDITY:INVALID";
		  break;
	  case 3	:
		  q = "VALIDITY:QUESTIONABLE";
		  break;
	  case 1	:
		  q = "VALIDITY:RESERVED";
		  break;
	 
    }
    return(q);
  
}

void time_add_ns( struct timespec *t,int ns)
{

    t->tv_nsec += ns;

    if(t->tv_nsec > 1000000000) {
        t->tv_nsec -= 1000000000;
        t->tv_sec += 1;
    }

}

/* Callback handler for received SV messages */
static void
svUpdateListener (SVSubscriber subscriber, void* parameter, SVSubscriber_ASDU asdu)
{
    printf("svUpdateListener called\n");
    const char* svID = SVSubscriber_ASDU_getSvId(asdu);

    if (svID != NULL)
        printf("  svID=(%s)\n", svID);

    printf("  smpCnt: %i\n",  SVSubscriber_ASDU_getSmpCnt(asdu));
   // printf("  smpRate: %i\n",  SVSubscriber_ASDU_getSmpRate(asdu));
    printf("  confRev: %u\n", SVSubscriber_ASDU_getConfRev(asdu));

    /*
     * Accéder aux données requiert a priori une connaissance sur les données sur lesquels on travaille.

     * Une valeur INT32 est encodée en 4 octets. on peut trouver la première valeur 
     * INT 32 à l'octet de position 0, la seconde valeur à l'octet de position 4, 
     * la troisième valeur à l'octet de position 8,etc.
     * Pour prévenir les erreurs due à la configuration,il faut relever la taille du bloc de 
     * données des messages SV avant d'accéder aux données.
     */
    if (SVSubscriber_ASDU_getDataSize(asdu) >= 8) {
      
        printf("  ia: %i A\t", SVSubscriber_ASDU_getINT32(asdu, 0)/1000);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,4)));
        printf("  ib: %i A\t", SVSubscriber_ASDU_getINT32(asdu, 8)/1000);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu, 12)));
        printf("  ic: %i A\t", SVSubscriber_ASDU_getINT32(asdu, 16)/1000);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,20)));
        printf("  in: %i A\t", SVSubscriber_ASDU_getINT32(asdu,24)/1000);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,28)));
	printf("  Va: %i V\t", SVSubscriber_ASDU_getINT32(asdu, 32)/100);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,36)));
        printf("  Vb: %i V\t", SVSubscriber_ASDU_getINT32(asdu, 40)/100);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,44)));
        printf("  Vc: %i V\t", SVSubscriber_ASDU_getINT32(asdu, 48)/100);
	printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,52)));
        printf("  Vn: %i V\t", SVSubscriber_ASDU_getINT32(asdu, 56)/100);
	printf("  quality: %s\n\n",quality( SVSubscriber_ASDU_getQuality(asdu,60)));
    }
}
void *acquisition(void *donnees)
{

     /* allocation mémoire */
    data_* param = malloc(sizeof(data_));
    param  = donnees;
    /* Créer un subscriber écoutant les messages SV  avec un APPID 4000h */
    SVSubscriber subscriber = SVSubscriber_create(NULL, 0x4000);

    /* Installer un callback handler pour le subscriber */
    SVSubscriber_setListener(subscriber, svUpdateListener, NULL);

    /* Connecter le subscriber au receiver */
    SVReceiver_addSubscriber(param->receiver, subscriber);

    /* Commencer à écouter les messages SV - commence une nouvelle tâche de receiver en arrière-plan */
    SVReceiver_start(param->receiver);
    struct timespec t;
    struct timeval avant_boucle,apres_boucle,debut_programme;
    long int duration ;
    int i=0,j;
    int n=0;
    int taille = 60;
    long int duree [taille];
    clock_gettime(CLOCK_MONOTONIC, &t);
    gettimeofday(&debut_programme,NULL);
    while (1) {
	    gettimeofday(&avant_boucle,NULL);
	    while(n<4000){
		/*calcul du temps de cycle*/
		/*reveil de l'horloge */
		clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME, &t, NULL);
		/* attendre le temps d'avoir 250us entre chaque échantillon*/
		time_add_ns(&t,250000); 
		n+=1;
	    }
	    gettimeofday(&apres_boucle,NULL);
	      duration  = (apres_boucle.tv_sec*1000000 + apres_boucle.tv_usec);
	      duration -= (avant_boucle.tv_sec*1000000 + avant_boucle.tv_usec);
	      duree [i] = duration;
	     if (apres_boucle.tv_sec - debut_programme.tv_sec > taille){ // supérieur à une durée fixée dans la variable taille    
	      FILE *fichier;
	      /* enregistrement dans un dossier sur la machine physique */
	      fichier = fopen("./temps_cycle_subscribe.csv", "w+");
	      /* docker */
	     // fichier = fopen("/log/temps_cycle_subscribe.csv", "w+");
	      if (fichier != NULL){
		for(j=0;j<taille;j++){ 
		    printf ("4000 échantillons reçu en %lu us\n",duree[j]);
		    fprintf (fichier," %lu\n",duree[j]);
		  }
		  printf("\t");
		  fprintf(fichier,"\t");
		  fclose(fichier);  
	      }  
	      break;
	    }
        i+=1;
	n=0;
    }
    
    /* Arrête de l'écoute les messages SV */
    SVReceiver_stop(param->receiver);
    /* Netoyyage et libération des ressources */
    SVReceiver_destroy(param->receiver);
    pthread_exit(NULL);
}

int
main(int argc, char** argv)
{
    SVReceiver receiver = SVReceiver_create();

    if (argc > 1) {
        SVReceiver_setInterfaceId(receiver, argv[1]);
		printf("Set interface id: %s\n", argv[1]);
    }
    else {
        printf("Using interface eth0\n");
        SVReceiver_setInterfaceId(receiver, "eth0");
    }
    
    /* création d'un bloc de données de thread de type data_ */
    data_ thread_data;
    thread_data.receiver= receiver;
    /* décalaration de la thread */
    pthread_t thread_acquisition;
    /* création de la thread d'acquisition*/
    pthread_create(&thread_acquisition,NULL,acquisition,&thread_data);
    /* le programme principal attend la fin de la tâche*/
    pthread_join(thread_acquisition,NULL); 
    return 0 ;
}















