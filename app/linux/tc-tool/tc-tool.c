/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : tc-tool [ifname] [-sdo] [-map]
 * Ifname is NIC interface, f.e. eth0.
 * Optional -sdo to display CoE object dictionary.
 * Optional -map to display slave PDO mapping
 *
 * This shows the configured slave data.
 *
 * (c)Arthur Ketels 2010 - 2011
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "ethercat.h"

#define EC_TIMEOUTMON 500

char IOmap[4096];
char usdo[128];

void simpletest(char *ifname){
  int l = 2;
  int16 *i16;
  uint16 val = 2;
  uint32 counter = 1e3;
  uint32 i = 0;

  printf("Starting simple test\n");

  /* initialise SOEM, bind socket to ifname */
  if (ec_init(ifname)){
    printf("ec_init on %s succeeded.\n",ifname);
    /* find and auto-config slaves */

    if ( ec_config_init(FALSE) > 0 ){
      printf("%d slaves found and configured.\n",ec_slavecount);
	  
      ec_configdc();
      ec_config_map_group(&IOmap, 0);
      
      //set channel 1 to pt1000
      ec_SDOwrite(2, 0x8000, 0x19, FALSE, 2, &val, EC_TIMEOUTSAFE);

      //set channel 1 to four-wire mode
      ec_SDOwrite(2, 0x8000, 0x1a, FALSE, 2, &val, EC_TIMEOUTSAFE);

      //set channel 2 to pt1000
      ec_SDOwrite(2, 0x8010, 0x19, FALSE, 2, &val, EC_TIMEOUTSAFE);

      //set channel 2 to four wire mode
      ec_SDOwrite(2, 0x8010, 0x1a, FALSE, 2, &val, EC_TIMEOUTSAFE);
	  
      printf("tmp: ");
      i = 0;
      while(i < counter){
        ec_SDOread(2, 0x6010, 0x11, FALSE, &l, &usdo, EC_TIMEOUTRXM);
        i16 = (int16*) &usdo[0];
        printf("0x%4.4x %d\n", *i16, *i16);
        i++;
      }
    }
  }
}


int main(int argc, char *argv[])
{
   printf("tc-tool\n");

   if (argc > 1)
   {
      simpletest(argv[1]);
   }
   else
   {
      printf("Usage: tc-tool ifname1\nifname = eth0 for example\n");
   }

   printf("End program\n");
   return (0);
}
