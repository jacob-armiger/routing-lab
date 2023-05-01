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
/*------- costs[DESTINATION][VIA] --------*/

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
	// These are the costs to go from node 0 to node[destination] through node[via]
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
		// Don't send to self or if the cost is infinity
		if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
			continue;
		}

		// Send packet to each nieghbor
		tolayer2(packet);
	}
}


void rtupdate0(rcvdpkt)
	struct rtpkt *rcvdpkt;
{
	printf("\nUPDATE 0 FROM %d\n", rcvdpkt->sourceid);
	int sendUpdate = 0; //initialize false

	int sender = rcvdpkt->sourceid;

	// Cost between node 0 and the sender
	int cost_to_source = dt0.costs[sender][sender];

	// Loop through destinations
	for(int i = 0; i < 4; i++) {
		// Current cost from sender to other destinations via the sender
		int cost_to_destination = rcvdpkt->mincost[i];

		// Cost to go directly to destination from node 0
		int direct_cost = dt0.costs[i][i];

		int path_cost = cost_to_source + cost_to_destination;

		// Update cost table if the cost is less
		if(path_cost < direct_cost) {
			// Update node 0 cost table since a more efficient route is found
			dt0.costs[i][sender] = path_cost;
			// Send update since there has been a change
			sendUpdate = 1;
		}
	}
	
	if(sendUpdate) {
		// create routing packet to send
		struct rtpkt packet;
		packet.sourceid = 0;
		packet.destid = NULL;
		// Get the min costs from node 0 to the sender via other destinations
		packet.mincost[0] = dt0.costs[0][sender];
		packet.mincost[1] = dt0.costs[1][sender];
		packet.mincost[2] = dt0.costs[2][sender];
		packet.mincost[3] = dt0.costs[3][sender];

		for(int i = 0; i < 4; i++) {
			packet.destid = i;
			// Don't send to self or if the cost is infinity
			if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
				continue;
			}

			// Send packet to each nieghbor
			tolayer2(packet);
		}
	}
	// Print updated table
	printdt0(&dt0);
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

linkhandler0(int linkid, int newcost) {
  	printf("\nLINKHANDLER0 %d\n", newcost);

	//compute distances for each row for linkid
	// for (int i=0; i<4; i++) {
	// 	if (dt0.costs[i][linkid] != 999) {
	// 		//if cost is not infinity, subtract old cost at index and add the new cost for that index
	// 		dt0.costs[i][linkid] -= dt0.costs[linkid][linkid];
	// 	}
	// }

	dt0.costs[linkid][linkid] = newcost;

	struct rtpkt packet;
	packet.sourceid = 0;
	packet.destid = NULL;
	packet.mincost[0] = dt0.costs[0][linkid];
	packet.mincost[1] = dt0.costs[1][linkid];
	packet.mincost[2] = dt0.costs[2][linkid];
	packet.mincost[3] = dt0.costs[3][linkid];

	for (int i=0; i<4; i++) {
		packet.destid = i;

		if(packet.sourceid == packet.destid || packet.mincost[i] == 999) {
			continue;
		}

		tolayer2(packet);
	}
	printdt0(&dt0);
}

