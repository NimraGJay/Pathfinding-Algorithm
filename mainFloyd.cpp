#include <iostream>
#include <fstream>
#include <cfloat>
#include <cmath>

using namespace std;

//===================Variables========================
string FileName;
int V; //V for Vertices
double **Adj, **Next; //2d Dynamic Arrays to store the matrices
int water_capacity,H2O;

//===================Functions========================

void ReadFileName(){
	char c;
	while (c!='a' && c!='b')
	{
	  cout << "Select File:\nType a for Shortest.to.post.100.txt\nType b for Shortest-to-post-300.txt\n";
	  cin>>c;
	  if(c=='a'){
	  	FileName=string("Shortest-to-post-100.txt");
		V= 100;
	  	}
	else{
	  	FileName=string("Shortest-to-post-300.txt");
		V= 300;
		}
	}	
}
bool ReadFile(string FileName){
	ifstream fin;
	int u,v;
	double distance;
	Adj=new double*[V];
	Next=new double*[V];
	
	
	for(int i = 0; i < V; ++i){
      Adj[i] = new double[V];
	  Next[i] = new double[V];      
    }
    for(int i = 0 ; i < V ; i++){
    	for(int j = 0 ; j<V; j++){
    		Next[i][j] = -1;
		}
	}
	fin.open(FileName);
	if(!fin.good()){
		cout<<"File not good";
		return false;	
	}else{
		while(fin>>u>>v>>distance){

			
			if(distance==0)
				distance=(double)INT_MAX;
			else if (distance<0)
				distance=2;	
			else if(u==v)
				distance=0;
			
			Adj[u][v]=distance;
			
		}

		
	fin.close();
	}
}
void Destruct(){
	for(int i = 0; i < V; ++i){
    	delete [] Adj[i];
    	delete [] Next[i];	
    }
  delete [] Adj;
  delete [] Next;
}
void printMatrix(double **arr){
	for(int i = 0; i < V; ++i){
    	for(int j = 0; j < V; ++j){
     		 std::cout <<arr[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;
}

void floyds(double **Adj)
{
    int i, j, k;
    for (k = 0; k < V; k++)
    {
        for (i = 0; i < V; i++)
        {
            for (j = 0; j < V; j++)
            {

                    if ((Adj[i][k] + Adj[k][j] < Adj[i][j]) )
                    {
                    	//cout<<Adj[i][k]<<" "<<Adj[k][j]<<" "<<Adj[i][j]<<"\n";
                        Adj[i][j] = Adj[i][k] + Adj[k][j];
                        Next[i][j]=k;
                    }

            }
        }
    }
}

string GetPath (int i, int j) {
	
    if (Adj[i][j] == INT_MAX) return "\nNo Path\n";
    if(Adj[i][j]!=INT_MAX && Adj[i][j]>water_capacity){
    	return "\nThe path length from " + to_string(i) + " to " + to_string(j)+ 
		" ("+to_string((int)Adj[i][j])+" km) is more than the bottle capacity. hence this path is not possible.\n";
	}
    
    int intermediate = Next[i][j];
    
    if (intermediate < 0) { //if its a direct path
    	
	
		H2O=H2O-Adj[i][j]; 
		if(H2O==0){ //all water used
			H2O=water_capacity;
			return "\nAll Water used: Refill at the next village: "+to_string(j)+" \n";
			
		}else if(H2O<0){
			//H2O=water_capacity-Adj[i][j];
			return "\nWater will run out. Refill at previous Village: "+ to_string(i)+"\n" ;
		}
		return "\n" + to_string((int)Adj[i][j]) + " kilometres" + " with water capacity of: "+ to_string(H2O)+"\n" + "to ";
		
		}
    return GetPath(i,intermediate) + to_string(intermediate) + GetPath(intermediate,j);
}

int findMinVertex(double distance[], bool visited[], int V){
	int minVertex=-1;
	for(int i = 0; i<V; i++){
		if(visited[i]==false && (minVertex==-1 || distance[i]<distance[minVertex])){
			minVertex=i;
		}
	}	
return minVertex;
}

void djikstra (double **arr){
	
	for(int src=0; src<V;src++){
	
	double distance[V];
	bool visited[V];
	for(int i = 0 ; i<V; i++){
		distance[i]=INT_MAX;
		visited[i]=false;
	}
	distance[src]=0;
	
	for(int i =0; i<V-1; i++){
		
		int minVertex = findMinVertex(distance,visited,V);
		visited[minVertex]=true;
		for(int j = 0 ; j< V; j++){
			if(Adj[minVertex][j]!=INT_MAX && !visited[j]){
				double dist = distance[minVertex] + Adj[minVertex][j];
				if(dist<distance[j]){
					distance[j]=dist;
				}
			}
		}	
	}
	
	for(int i = 0; i<V;i++){
		Adj[src][i]=distance[i];
	}
}
}
//================== Main Function ===================
int main(){
	ReadFileName();
	ReadFile(FileName);
	cout<<"Which Algorithm would you like to run?\nPress a for Floyd\nPress b for Djikstra\n";
	char algo;
	cin>>algo;
	if(algo=='a'){
		floyds(Adj);
		
		cout<<"Enter the water(H2O) Capacity? Pls\n";
		cin>>water_capacity;
		H2O=water_capacity;
		cout<<"Enter Village Source: ";
		int src;
		cin>>src;
		cout<<"Enter Village Destination: ";
		int dest;
		cin>>dest;
		cout<<"Path from source to destination:\n"<<src<<GetPath(src,dest)<<dest;
		
	}else if(algo=='b'){
		djikstra(Adj);
	}
	
	Destruct();
}

