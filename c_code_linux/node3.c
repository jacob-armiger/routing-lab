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
} dt3;

/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  printf("\nRINIT3\n");
  // Set all values to infinity in distance table
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      dt3.costs[i][j] = 999; 
    }
  }
  printdt3(&dt3);
  printf("\n");

  // Set initial values in distance table
  dt3.costs[3][0] = 7;
  dt3.costs[3][1] = 999; // Still infinity
  dt3.costs[3][2] = 2;
  dt3.costs[3][3] = 0;

  // create routing packet to send
  struct rtpkt packet;
  packet.sourceid = 3;
  packet.destid = NULL;
  packet.mincost[0] = 7;
  packet.mincost[1] = 999;
  packet.mincost[2] = 2;
  packet.mincost[3] = 0;

  for(int i = 0; i < 4; i++) {
    packet.destid = i;
    if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
      continue;
    }
    // Send packet to nieghbors
    tolayer2(packet);
  }
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  printf("\nUPDATE3\n");

}


printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







