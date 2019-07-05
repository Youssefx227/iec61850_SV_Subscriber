/* @Youssef Mounkaila Rte-France
	youssef.mounkaila@rte-france.com
 * sv_subscriber_example.c
 * programme pour l'acquistion de Sampled Values (SV) : subscriber
 */
/*
#include <iec61850_server.h>
#include <iec61850_model.h>
#include <sv_subscriber.h>
#include <hal_thread.h>
#include <static_model.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <mms_value.h>
#include <sched.h>

#include <goose_publisher.h>
*/
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sv_subscriber.h>
#include <time.h>
#include <sys/time.h>
static bool running = true;
void sigint_handler(int signalId)
{
    running = 0;
}

long long int Ts_ms = 0;
int i =0;
struct timeval maintenant;
struct timeval debut_programme;
int position =0;
uint64_t TableauTimeStamp[4000*30];
int min=0,max=0;


/**
 *  la fonction quality prend en paramètre la qualité (entier) de la donnée reçu
 * elle renvoi une chaîne de caractère correpondant à la qualité
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
/*
	foncton ajout délai
*/
void time_add_ns( struct timespec *t,int ns)
{

    t->tv_nsec += ns;

    if(t->tv_nsec > 1000000000) {
        t->tv_nsec -= 1000000000;
        t->tv_sec += 1;
    }

}


/* Callback handler pour les messages SV reçus */
static bool depart=true;
struct timeval before_usec;
uint64_t before; /* timestamp in microsecond */
Timestamp ts;
//Timestamp_clearFlags(&ts);
struct timeval timer_usec;
uint64_t timestamp_usec; /* timestamp in microsecond */

static void
svUpdateListener (SVSubscriber subscriber, void* parameter, SVSubscriber_ASDU asdu)
{

    //printf("svUpdateListener called\n");
    if(depart==true){
      gettimeofday(&before_usec, NULL);
      before  = ((uint64_t) before_usec.tv_sec) * 1000000 +
                       (uint64_t) before_usec.tv_usec;
      depart=false;
    }
    const char* svID = SVSubscriber_ASDU_getSvId(asdu);
    if (svID != NULL)
    //printf("  svID=(%s)\n", svID);

    //printf("  smpCnt:  %i\n",  SVSubscriber_ASDU_getSmpCnt(asdu));
  //  printf("  confRev: %u\n",  SVSubscriber_ASDU_getConfRev(asdu));

    /*
     * Accéder aux données requiert à priori une connaissance sur la structure de données sur laquelle on travaille.

     * Une valeur INT32 est encodée en 4 octets. on peut trouver la première valeur
     * INT 32 à l'octet de position 0, la seconde valeur à l'octet de position 4,
     * la troisième valeur à l'octet de position 8,etc.
     * Pour prévenir les erreurs due à la configuration,il faut relever la taille du bloc de
     * données des messages SV avant d'accéder aux données.
     */

    if (SVSubscriber_ASDU_getDataSize(asdu) >= 8) {

    /*
      printf("  ia: %i A\n", SVSubscriber_ASDU_getINT32(asdu, 0)/1000);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,4)));
      fprintf("  ib: %i A\n", SVSubscriber_ASDU_getINT32(asdu, 8)/1000);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu, 12)));
      printf("  ic: %i A\n", SVSubscriber_ASDU_getINT32(asdu, 16)/1000);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,20)));
      printf("  in: %i A\n", SVSubscriber_ASDU_getINT32(asdu,24)/1000);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,28)));
      printf("  Va: %i V\n", SVSubscriber_ASDU_getINT32(asdu, 32)/100);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,36)));
      printf("  Vb: %i V\n", SVSubscriber_ASDU_getINT32(asdu, 40)/100);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,44)));
      printf("  Vc: %i V\n", SVSubscriber_ASDU_getINT32(asdu, 48)/100);
      printf("  quality: %s\n", quality(SVSubscriber_ASDU_getQuality(asdu,52)));
      printf("  Vn: %i V\n", SVSubscriber_ASDU_getINT32(asdu, 56)/100);
      printf("  quality: %s\n\n",quality( SVSubscriber_ASDU_getQuality(asdu,60)));
    */

    gettimeofday(&timer_usec, NULL);
    timestamp_usec = ((uint64_t) timer_usec.tv_sec) * 1000000 +
                     (uint64_t) timer_usec.tv_usec;

    //printf(" Timestamp: %lld\n",(long long int) timestamp_usec);
    //printf("time µs since the last sample : %lld\n",timestamp_usec-A);
    TableauTimeStamp[position] = (timestamp_usec-before );
    position+= 1;
    before  = timestamp_usec; // sauvegarde du timestamp précédent

  	}

}

int main(int argc, char** argv)
{

  SVReceiver receiver = SVReceiver_create();

  //uint16_t appid;

  if (argc > 1)
      SVReceiver_setInterfaceId(receiver, argv[1]);

  else {
      printf("Using interface eth0\n");
      SVReceiver_setInterfaceId(receiver, "eth0");
  }

    /* Créer un subscriber écoutant les messages SV  avec un APPID 4000h */
    SVSubscriber subscriber = SVSubscriber_create(NULL,0x4000);

    SVSubscriber_setListener(subscriber, svUpdateListener, NULL);
    /* Connecter le subscriber au receiver */
    SVReceiver_addSubscriber(receiver, subscriber);

    /* Commencer à écouter les messages SV - commence une nouvelle tâche de receiver en arrière-plan */
    SVReceiver_start(receiver);
    //clock_gettime(CLOCK_MONOTONIC, &t);
    signal(SIGINT, sigint_handler);

    gettimeofday(&debut_programme,NULL);
    while (running) {

    //  Thread_sleep(1);
      gettimeofday(&maintenant,NULL);
      if (maintenant.tv_sec - debut_programme.tv_sec > 5){ // supérieur à une durée fixé dans la variable taille
          int j;
          /*création d'un fichier de stockage des valeurs */
          FILE *fichier;
        fichier = fopen("./timestamp_subscribe.csv", "w+");
          /* chemin du fichier dans le docker */
      // fichier = fopen("/log/temps_cycle.csv", "w+");

          if(fichier != NULL) {
            /*-- affichage après un certains temps des temps de cycle --*/
            for (j=0;j<position;j++){
                fprintf(fichier,"%i",j);
                fprintf(fichier,"\t%ld\n",TableauTimeStamp[j]);
                //printf("\t%ld\n",TableauTimeStamp[j]);
            }
          fclose(fichier);
          }
          break;
        }

    }

/* Arrête de l'écoute les messages SV */
 SVReceiver_stop(receiver);
/* Netoyyage et libération des ressources */
 SVReceiver_destroy(receiver);
}
