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
} dt2;


/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
	printf("\nRINIT2\n");
	// Set all values to infinity in distance table
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			dt2.costs[i][j] = 999; 
		}
	}

	// Set initial values in distance table
	dt2.costs[0][0] = 3;
	dt2.costs[1][1] = 1;
	dt2.costs[2][2] = 0;
	dt2.costs[3][3] = 2;
	printdt2(&dt2);
	printf("\n");

	// create routing packet to send
	struct rtpkt packet;
	packet.sourceid = 2;
	packet.destid = NULL;
	packet.mincost[0] = 3;
	packet.mincost[1] = 1;
	packet.mincost[2] = 0;
	packet.mincost[3] = 2;

	for(int i = 0; i < 4; i++) {
		packet.destid = i;
		if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
			continue;
		}
		// Send packet to nieghbors
		tolayer2(packet);
	}
}


void rtupdate2(rcvdpkt)
	struct rtpkt *rcvdpkt;

{
	printf("\nUPDATE 2 FROM %d\n", rcvdpkt->sourceid);
	int sendUpdate = 0;
	int sender = rcvdpkt->sourceid;

	//cost between node 2 and the sender
	int cost_to_source = dt2.costs[sender][sender];

	 //if sender is 0 or 1, check if 0 to 1 cost is 20 inside the packet
    if (sender == 0 || sender == 1) {
        //basically, just check if the number 20 is in one of the indexes
        int updated = 0;
        for (int i = 0; i<4; i++) {

            if (rcvdpkt->mincost[i] == 20 && dt2.costs[0][1] == 1) {
                updated = 1;
            }
        }

        //update node 1 via 0 and node 0 via 1
        if (dt2.costs[0][1] != 999 && updated == 1) {
            dt2.costs[0][1] -= 1;
            dt2.costs[0][1] += 20;
        }

        if (dt2.costs[1][0] != 999 && updated == 1) {
            dt2.costs[1][0] -= 1;
            dt2.costs[1][0] += 20;
        }

    }
	

	//loop through destinations
	for (int i = 0; i < 4; i++) {
		//current cost from sender to other destinations via the sender
		int cost_to_destination = rcvdpkt->mincost[i];

		//cost to go directly to destination from node 2
		int direct_cost = dt2.costs[i][i];

		int path_cost = cost_to_source + cost_to_destination;

		//update cost table if the cost is less
		if (path_cost < direct_cost) {
			//update node 2 cost table since a more efficient route is found
			dt2.costs[i][sender] = path_cost;
			//send update since there has been a change
			sendUpdate = 1;
		}
	}
	
	if (sendUpdate) {
		//create routing packet to send
		struct rtpkt packet;
		packet.sourceid = 0;
		packet.destid = NULL;
		// get the min costs from node 0 to the sender via other destinations
		packet.mincost[0] = dt2.costs[0][sender];
		packet.mincost[1] = dt2.costs[1][sender];
		packet.mincost[2] = dt2.costs[2][sender];
		packet.mincost[3] = dt2.costs[3][sender];

		for (int i = 0; i < 4; i++) {
			packet.destid = i;
			//don't send to self or if the cost is infinity
			if (packet.sourceid == packet.destid || packet.mincost[i] == 999) continue;   

			//send packet to each neighbor
			tolayer2(packet);
		}
	}
	printdt2(&dt2);

}


printdt2(dtptr)
	struct distance_table *dtptr;

{
	printf("                via     \n");
	printf("   D2 |    0     1    3 \n");
	printf("  ----|-----------------\n");
	printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
			dtptr->costs[0][1],dtptr->costs[0][3]);
	printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
			dtptr->costs[1][1],dtptr->costs[1][3]);
	printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
			dtptr->costs[3][1],dtptr->costs[3][3]);
}







