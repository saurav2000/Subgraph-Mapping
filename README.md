# Subgraph-Mapping
## Statement
> There are two directed graphs G and G' without any self edges. This finds a one-one mapping
> M from nodes of G to G', such that there is an edge from v1 to v2 in G iff there is an edge from M(v1)
> to M(v2) in G'

## Execution
> This is done by converting the problem to a Boolean satisfiability problem which is in turn
> solved using [MiniSAT a complete SAT solver](minisat.se/MiniSat.html)

## Running
- generate.py generates a random graph with input of the no. of nodes and edges 
in G' and G. This is run by `python generate.py [NAME] n1 e1 n2 e2` which stores the graphs 
in [NAME].graphs with n1 and e1 being the nodes and edges of G'.
- check.py checks if the mapping is correct or not. It is run by `python check.py [NAME].graphs [NAME].mapping`
- run1.sh and run2.sh can be run by `./run1.sh [NAME]` and `./run2.sh [NAME]`. run1 generates the input for minisat in [NAME].satinput and run2 takes the output of the minisat ([NAME].satoutput) to generate the mapping.
- The minisat executable can be run by `./minisat [NAME].satinput [NAME].satoutput`
- The entire list of commands is present in overallrun.sh, which is run by `./overallrun.sh [NAME] n1 e1 n2 e2`
