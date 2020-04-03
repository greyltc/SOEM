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

char IOmap[4096];

void simpletest(char *ifname){
  int num_bytes = 0;
  uint16 write_value = 0x0000;
  uint32 counter = 1e3;
  volatile uint32 i = 0;
  volatile float temp = 0.0;
  volatile int16 read_value = 0;

  printf("Starting simple test\n");

  /* initialise SOEM, bind socket to ifname */
  if (ec_init(ifname)){
    printf("ec_init on %s succeeded.\n",ifname);
    /* find and auto-config slaves */

    if ( ec_config_init(FALSE) > 0 ){
      printf("%d slaves found and configured.\n",ec_slavecount);
	  
      ec_configdc();
      ec_config_map_group(&IOmap, 0);
      
      write_value = 2;
      num_bytes = sizeof(write_value);
      //set channel 1 to pt1000
      ec_SDOwrite(2, 0x8000, 0x19, FALSE, num_bytes, &write_value, EC_TIMEOUTSAFE);

      //set channel 1 to four-wire mode
      ec_SDOwrite(2, 0x8000, 0x1a, FALSE, num_bytes, &write_value, EC_TIMEOUTSAFE);

      //set channel 2 to pt1000
      ec_SDOwrite(2, 0x8010, 0x19, FALSE, num_bytes, &write_value, EC_TIMEOUTSAFE);

      //set channel 2 to four wire mode
      ec_SDOwrite(2, 0x8010, 0x1a, FALSE, num_bytes, &write_value, EC_TIMEOUTSAFE);
	  
      printf("Temperature: ");
      i = 0;
      num_bytes = sizeof(read_value);
      while(i < counter){
        ec_SDOread(2, 0x6010, 0x11, FALSE, &num_bytes, (void*) &read_value, EC_TIMEOUTSAFE);
        temp = read_value / 100.0;
        printf("%2.2f [deg C]\n", temp);
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
