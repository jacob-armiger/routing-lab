#include <stdio.h>

struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt0;
/*------- costs[VIA][DESTINATION] --------*/

/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
  printf("\nRINIT0\n");
  // Set all values to infinity in distance table
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      dt0.costs[i][j] = 999; 
    }
  }
  printf("\n");

  // Set initial values in distance table
  dt0.costs[0][0] = 0;
  dt0.costs[1][1] = 1;
  dt0.costs[2][2] = 3;
  dt0.costs[3][3] = 7;
  printdt0(&dt0);

  // create routing packet to send
  struct rtpkt packet;
  packet.sourceid = 0;
  packet.destid = NULL;
  packet.mincost[0] = 0;
  packet.mincost[1] = 1;
  packet.mincost[2] = 3;
  packet.mincost[3] = 7;

  for(int i = 0; i < 4; i++) {
    packet.destid = i;
    if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
      continue;
    }

    // Send packet to nieghbors
    tolayer2(packet);
  }
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  printf("\nUPDATE 0 FROM %d\n", rcvdpkt->sourceid);
  int sendUpdate = 0; //initialize false

  int sender = rcvdpkt->sourceid;
  for(int i = 0; i < 0; i++) {
    // dt0.costs[0][i]
    // rcvdpkt->mincost[i]
  }


}


printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

