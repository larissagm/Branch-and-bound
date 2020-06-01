#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <chrono>
using namespace std;

#include "data.h"
#include "hungarian.h"

struct Node {

	vector<vector<int>> subtours;
	vector<pair<int,int>> arcos_proibidos;
	double lower_bound;
	int escolhido;
	bool podar=0;

};

void calcularSolucao(Node &node, hungarian_problem_t &p){

	for (int i=0;i<node.arcos_proibidos.size();i++)	{
		p.cost[node.arcos_proibidos[i].first][node.arcos_proibidos[i].second]=999999999;
	}

	node.lower_bound = hungarian_solve(&p);

	//construir subtours

	int **m = p.assignment;
	int &n = p.num_rows;

	int menorTam = n+2;
	int x=-1;
	vector<int> ch(n,0);
	for (int i=0;i<n;i++){
		if (ch[i]==0){
			vector<int> init = {i};
			int k=i;
			x++;
			while (ch[i]==0){
				for (int j=0;j<n;j++){
					if (m[k][j]==1){
						ch[j]=1;
						k=j;
						init.push_back(j);
						break;
					}
				}
			}
			node.subtours.push_back(init);
			if (init.size()<menorTam){
				menorTam=init.size();
				node.escolhido=x;
			}
		}
	}
	if (node.subtours.size()==1) node.podar=1;

}

int main(int argc, char** argv) {

	auto start = chrono::system_clock::now();

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}

	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode);

	Node best,node;
	best.lower_bound=numeric_limits<double>::infinity();
	list<Node> arvore={node};

	while (!arvore.empty()){
		
		node = arvore.front();
	
		hungarian_free(&p);
		hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode);

		calcularSolucao(node,p);

		if (node.podar){
			if (node.lower_bound<best.lower_bound){
				best=node;
			}
		}
		else if (node.lower_bound<best.lower_bound){
			for (int i=0;i<node.subtours[node.escolhido].size()-1;i++){
				Node n;
				n.arcos_proibidos=node.arcos_proibidos;
				n.arcos_proibidos.push_back({node.subtours[node.escolhido][i],node.subtours[node.escolhido][i+1]});
				arvore.push_back(n);
			}
		}
		arvore.pop_front();
	}

	auto end = chrono::system_clock::now();
	chrono::duration<double> time=end-start;

	/*printf("Solution: ");
	for (int i=0;i<=data->getDimension();i++){
		printf("%i ",best.subtours[0][i]);
	}*/

	printf("Tempo: %lf\nCusto: %lf\n\n",time,best.lower_bound);

	hungarian_free(&p);
	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}
 