#include <bits/stdc++.h>
using namespace std;
int main()
{
    double tgt[8]={1610,2020,3323,937,2085,2707,1272,6859};
    string s;
    ifstream arq("bm.txt");
    ofstream arq2("bm_avg.txt");
    for (int i=0;i<8;i++){
        getline(arq,s);
        arq2<<s;
        double custo=0,tempo=0;
        for (int j=0;j<10;j++){
            getline(arq,s);
            stringstream r;
            double t;
            r<<s.substr(7,s.size()-7);
            r>>t;
            tempo+=t;
            getline(arq,s);
            stringstream p;
            double c;
            p<<s.substr(7,s.size()-7);
            p>>c;
            custo+=c;
            getline(arq,s);
        }
       double pct = ( (custo/10 - tgt[i]) / tgt[i] ) * 100;
       arq2<<"\nTempo: "<<tempo/10<<"\nCusto: "<<custo/10<<endl<<pct<<"%"<<endl<<endl;

    }
    arq.close(); 
    arq2.close();
    return 0;
}
