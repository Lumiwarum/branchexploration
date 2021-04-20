#include <iostream>
#include <vector>
#include <memory>
using namespace std;
template <typename T>
class vertex {
public:
    int id;
    T storedObject;
    vertex(){}
    vertex(T object){
        storedObject=object;
    }
    void replaceObject(T replaceTo){
        storedObject=replaceTo;
    }
    bool equalTo(vertex<T> compareWith){
        if (compareWith.storedObject==storedObject){ return true;}
        return false;
    }
};
template<typename T,typename W>
class edge{
public:
    vertex<T>* from;
    vertex<T>* to;
    W weight;
    edge(vertex<T>* from,vertex<T>* to,W weight){
        this->from=from;
        this->to=to;
        this->weight=weight;
    }
    ~edge(){}
};

template<typename V,typename W>
class graphADT{
public:
virtual vertex<V>* addVertex(V value){};
virtual void removeVertex(vertex<V>* vertex){};
virtual edge<V,W>* addEdge(vertex<V>* from,vertex<V>* to,W weight){};
virtual void removeEdge(edge<V,W>* edge){};
virtual vector<edge<V,W>> edgesFrom(vertex<V>* Vertex){};
virtual vector<edge<V,W>> edgesTo(vertex<V>* Vertex){};
virtual vertex<V>* findVertex(V value){};
virtual edge<V,W>* findEdge(V valueFrom,V valueTo){};
virtual bool hasEdge(vertex<V>* from,vertex<V>* to){};
};

template<typename V,typename W>
class  adjacencyMatrixGraph: public graphADT<V,W>{
private:
    vector<vertex<V>> vertexes;
    vector<vector<edge<V,W>*>> matrix;
public:
    adjacencyMatrixGraph(){
    }
    void changeIds(){
        for (int i=0;i<vertexes.size();i++){
            vertexes[i].id=i;
        }
    }

    vertex<V>* addVertex(V value) override {
        vertexes.push_back(vertex(value));
        for (int i=0;i<matrix.size();i++){
            matrix[i].push_back(nullptr);
        }
        vector<edge<V,W>*> vectorToAdd;
        for (int i = 0; i< vertexes.size();i++){
            vectorToAdd.push_back(nullptr);
        }
        matrix.push_back(vectorToAdd);
        changeIds();
    }

    void removeVertex(vertex<V>* vertex) override{
        int remove;
        for (int i=0;i<vertexes.size();i++){
            if (vertex->equalTo(vertexes[i])){
                remove=i;
            }
        }
        int n=matrix.size();
        matrix.erase(matrix.begin()+remove);
        for (int i=0;i<n-1;i++){
            matrix[i].erase(matrix[i].begin()+remove);
        }
        vertexes.erase(vertexes.begin()+remove);
        changeIds();
    }

    edge<V,W>* addEdge(vertex<V>* from,vertex<V>* to,W weight) override{
        auto edgeToAdd = new edge<V,W>(from,to,weight);
        matrix[from->id][to->id] = edgeToAdd;
        return matrix[from->id][to->id];
    }

    void removeEdge(edge<V,W>* edge) override {
        matrix[edge->from->id][edge->to->id]= nullptr;
        delete matrix[edge->from->id][edge->to->id];
    }

    vector<edge<V,W>> edgesFrom(vertex<V>* Vertex) override {
        vector<edge<V,W>> returnVector;
        for (int i=0;i<matrix[Vertex->id].size();i++){
            if (matrix[Vertex->id][i]!= nullptr){
                returnVector.push_back(*matrix[Vertex->id][i]);
            }
        }
        return returnVector;
    }

    vector<edge<V,W>> edgesTo(vertex<V>* Vertex) override {
        vector<edge<V,W>> returnVector;
        for (int i=0;i<matrix.size();i++){
            if (matrix[i][Vertex->id]!= nullptr){
                returnVector.push_back(*matrix[i][Vertex->id]);
            }
        }
        return returnVector;
    }

    vertex<V>* findVertex(V value) override {
        for (int i=0;i<vertexes.size();i++){
            if (vertexes[i].storedObject==value){
                return &vertexes[i];
            }
        }
        return nullptr;
    }

    edge<V,W>* findEdge(V valueFrom,V valueTo) override {
        vertex<V>* from=findVertex(valueFrom);
        vertex<V>* to=findVertex(valueTo);
        if ((from != nullptr)&&(to!= nullptr)){
            return matrix[from->id][to->id];
        }
        return nullptr;
    }

    bool hasEdge(vertex<V>* from,vertex<V>* to) override {
        bool hasEdge=false;
        if (matrix[from->id][to->id]!= nullptr){hasEdge= true;}
        return hasEdge;
    }

};
int main() {
    adjacencyMatrixGraph<int,int> matrix;
    for (int i=0;i<5;i++){
        matrix.addVertex(i);
    }
    for (int i=1;i<5;i++){
        matrix.addEdge(matrix.findVertex(0),matrix.findVertex(i),i);
    }
    matrix.removeEdge(matrix.findEdge(0,1));

    vector<edge<int,int>> edges=matrix.edgesFrom(matrix.findVertex(0));
    for (int i=0;i<edges.size();i++){
       cout <<edges[i].weight<<endl;
    }
    return 0;
}
