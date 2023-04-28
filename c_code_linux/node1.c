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

int connectcosts1[4] = { 1,  0,  1, 999 };

struct distance_table 
{
	int costs[4][4];
} dt1;


/* students to write the following two routines, and maybe some others */


rtinit1() 
{
	printf("\nRINIT1\n");
	// Set all values to infinity in distance table
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			dt1.costs[i][j] = 999; 
		}
	}

	// Set initial values in distance table
	dt1.costs[0][0] = 1;
	dt1.costs[1][1] = 0;
	dt1.costs[2][2] = 1;
	dt1.costs[3][3] = 999; // Still infinity
	printdt1(&dt1);
	printf("\n");

	// create routing packet to send
	struct rtpkt packet;
	packet.sourceid = 1;
	packet.destid = NULL;
	packet.mincost[0] = 1;
	packet.mincost[1] = 0;
	packet.mincost[2] = 1;
	packet.mincost[3] = 999;

	for(int i = 0; i < 4; i++) {
		packet.destid = i;
		if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
			continue;
		}
		// Send packet to nieghbors
		tolayer2(packet);
	}
}


rtupdate1(rcvdpkt)
	struct rtpkt *rcvdpkt;

{
	printf("\nUPDATE1\n");
	int sendUpdate = 0;
	int sender = rcvdpkt->sourceid;

	//cost between node 1 and the sender
	int cost_to_source = dt1.costs[sender][sender];

	//loop through destinations
	for (int i = 0; i < 4; i++) {
		//current cost from sender to other destinations via the sender
		int cost_to_destination = rcvdpkt->mincost[i];

		//cost to go directly to destination from node 1
		int direct_cost = dt1.costs[i][i];

		int path_cost = cost_to_source + cost_to_destination;

		//update cost table if the cost is less
		if (path_cost < direct_cost) {
			//update node 1 cost table since a more efficient route is found
			dt1.costs[i][sender] = path_cost;
			//send update since there has been a change
			sendUpdate = 1;
		}

		if (sendUpdate) {
			//create routing packet to send
			struct rtpkt packet;
			packet.sourceid = 0;
			packet.destid = NULL;
			// get the min costs from node 0 to the sender via other destinations
			packet.mincost[0] = dt1.costs[0][sender];
			packet.mincost[1] = dt1.costs[1][sender];;
			packet.mincost[2] = dt1.costs[2][sender];;
			packet.mincost[3] = dt1.costs[3][sender];;
			
			for (int i = 0; i < 4; i++) {
				packet.destid = i;
				//don't send to self or if the cost is infinity
				if (packet.sourceid == packet.destid || packet.mincost[i] == 999) continue; 

				//send packet to each neighbor
				tolayer2(packet);
			}
		}
	}
	printdt1(&dt1);
}


printdt1(dtptr)
	struct distance_table *dtptr;

{
	printf("             via   \n");
	printf("   D1 |    0     2 \n");
	printf("  ----|-----------\n");
	printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
	printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
	printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



linkhandler1(linkid, newcost)   
	int linkid, newcost;   
	/* called when cost from 1 to linkid changes from current value to newcost*/
	/* You can leave this routine empty if you're an undergrad. If you want */
	/* to use this routine, you'll need to change the value of the LINKCHANGE */
	/* constant definition in prog3.c from 0 to 1 */

{
}


