/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ksala
 * Creation Date: 02/01/2017 at 09:59:14
 *********************************************/
/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ksala
 * Creation Date: 29/12/2016 at 20:36:45
 *********************************************/

 int nLocations = ...;
 int ls = ...;
 int fromPoints = ...;
 
 range L = 1..nLocations;
 
 int points[i in L][x in 1..2] = ...;
 float dist[i in L][j in L] = ...;
 int t[l in L] = ...;
 int minWin[l in L] = ...;
 int maxWin[l in L] = ...;
 
 dvar boolean travel[i in L][j in L];
 dvar float+ arrive[l in L];
 dvar int+ value[l in L];
 dvar int+ ncars;
 
 execute {
 	 if (fromPoints == 1) {
 	 	for(var l1 = 1; l1 <= nLocations; ++l1) {
 	 		for (var l2 = 1; l2 <= nLocations; ++l2) {
 	 		 	var mydist = Opl.pow(points[l1][1]-points[l2][1], 2) + Opl.pow(points[l1][2]-points[l2][2], 2);
 	 			dist[l1][l2] = Opl.sqrt(mydist);
 	 		}
 	 	}
 	 }
 }
 
 minimize 720 * ncars + arrive[ls];
 
 subject to {
 	 
 	forall(i in L) {
		if (i != ls) {
			sum(j in L) travel[i][j] == 1;
			sum(j in L) travel[j][i] == 1;
		}
	}
	
	ncars == sum(i in L) travel[ls][i];
	
	ncars == sum(i in L) travel[i][ls];
	
 	forall(j in L)
 		arrive[j] >= dist[ls][j] * travel[ls][j];

 	forall(i in L, j in L) {
 		if (i != ls)
 	  		arrive[j] >= (t[i] + dist[i][j]) * travel[i][j] + arrive[i] - 720 * (1 - travel[i][j]);
  	}

 	forall(i in L) {
 		arrive[i] >= minWin[i];
 		arrive[i] <= maxWin[i];
 	}

 	forall(i in L, j in L)
		if (i != ls)
 			value[i] >= value[j] + 1 - nLocations * (1 - travel[i][j]);

 	forall(i in L)
 		1 <= value[i] <= nLocations;
 }
 
 execute {
 	writeln("We need " + ncars + " cars and the last one arrives at " + arrive[ls]);
 	
 	for (var i = 1; i <= nLocations; i++) {
 		if (travel[ls][i] == 1) {
 			write("Travel: 1 -> " + i +" (" + arrive[i] + ") -> ");
 			var prev = i;
 			var end = 0;
 			while (end == 0) {
 				var found = 0;
 				var next;
 				for (var j = 1; j <= nLocations; ++j) {
 					if (travel[prev][j] == 1 && found == 0) {
 						next = j;
 						found = 1;
 					}
 					else if (travel[prev][j] == 1) writeln("Error! Loop broken!");
 				}
 				
 				if (found == 0) writeln("Error! Next point not found!");
 				else if (next == 1) {
 					end = 1;
 					writeln("1");
 				}
 				else {
 					if (arrive[next] > maxWin[next] || arrive[next] < minWin[next]) writeln("Error! Time window not satisfied!");
 					else write(next + " (" + arrive[next] + ") -> ");
 				}
 				prev = next;
 			}
 		}
 	}
 };
