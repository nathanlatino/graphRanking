#pragma once
#include <vector>
#include <stack>
#include <list>
#include <queue>
#include <set>
#include <iomanip>
#include <math.h>

template <typename VVALUE, typename EVALUE>
class Graph {

public:
	class Iterator {
	public:
		virtual int get_vertex() = 0;
		virtual void next() = 0;
		virtual bool valid() = 0;
		virtual ~Iterator() {}
	};

private:
	class Edge;

	class Vertex {
	private:
		int _index;
		VVALUE _value;
		std::vector<Edge*> _edges;
	public:
	    float pageRank = 0.25;
		VVALUE get_value() { return _value; }
		int get_index() { return _index; }
		Vertex(int index, VVALUE& value) { _index = index; _value = value; }
		void add_edge(Edge* edge) { _edges.push_back(edge); }
		Edge* get_edge(int i) { return _edges[i]; }
		int get_degree() { return _edges.size(); }
		Vertex* get_neighbor(int i) { return _edges[i]->get_target(this); }
	};

	class Edge {
	private:
		int _index;
		Vertex* _source;
		Vertex* _target;
		EVALUE _weight;
	public:
		EVALUE get_weight() { return _weight; }
		Vertex* get_target(Vertex* source) {
			if (source == _source) return _target;
			if (source == _target) return _source;
			return 0;
		}
		Vertex* get_source() { return _source; }
		Vertex* get_target() { return _target; }
		int get_index() { return _index; }
		Edge(int index, Vertex* source, Vertex* target, EVALUE& weight)
		{
			_index = index;
			_source = source;
			_target = target;
			_weight = weight;
		}
	};

	class AllVertexIterator : public Iterator {
	private:
		int _position;
		int _size;
	public:
		AllVertexIterator(int size) { _size = size; _position = 0; }
		virtual int get_vertex() { return _position; }
		virtual void next() { if (valid()) _position ++; }
		virtual bool valid() { return _position < _size; }
		~AllVertexIterator() {}
	};

	class DepthFirstIterator : public Iterator {
	private:
		Vertex* _position;
		std::stack<Vertex*> _stack;
		std::set<int> _visited;

		bool visited(Vertex* v) {return _visited.count(v->get_index()) > 0;}
		void visit()
		{
			_visited.insert(_position->get_index());
			for (int i=_position->get_degree()-1; i>=0; i--) {
				Vertex* neighbor = _position->get_neighbor(i);
				if (!visited(neighbor)) {
					_stack.push(neighbor);
				}
			}
		}
	public:
		DepthFirstIterator(Vertex* start)
		{
			_position = start;
			visit();
		}
		virtual int get_vertex() { return _position ? _position->get_index() : -1; }
		virtual void next()
		{
			if (!valid()) return;
			Vertex* top = 0;
			while (_stack.size()) {
				top = _stack.top();
				_stack.pop();
				if (!visited(top)) break;
				top = 0;
			}
			_position = top;
			if (top)
				visit();
		}
		virtual bool valid() { return _position != 0; }
		~DepthFirstIterator() {}
	};

	class WidthFirstIterator : public Iterator {
	private:
		Vertex* _position;
		std::list<Vertex*> _queue;
		std::set<int> _visited;
		
		bool visited(Vertex* v) {return _visited.count(v->get_index()) > 0;}
		void visit()
		{
			_visited.insert(_position->get_index());
			for (int i=_position->get_degree()-1; i>=0; i--) {
				Vertex* neighbor = _position->get_neighbor(i);
				if (!visited(neighbor)) {
					_queue.push_back(neighbor);
				}
			}
		}
	public:
		WidthFirstIterator(Vertex* start)
		{
			_position = start;
			visit();
		}
		virtual int get_vertex() { return _position ? _position->get_index() : -1; }
		virtual void next()
		{
			if (!valid()) return;
			Vertex* top = 0;
			while (_queue.size()) {
				top = _queue.front();
				_queue.pop_front();
				if (!visited(top)) break;
				top = 0;
			}
			_position = top;
			if (top)
				visit();
		}
		virtual bool valid() { return _position != 0; }
		~WidthFirstIterator() {}
	};

	struct MSTEdge {
		int source;
		int target;
		EVALUE weight;
		MSTEdge(int s, int t, EVALUE w) { source = s; target = t; weight = w; }
		bool operator<(const MSTEdge& e2) const { return e2.weight < weight; }
	};

	bool _directed;
	std::vector<Vertex*> _vertices;
	std::vector<Edge*> _edges;

	void MST_visit(int current, std::set<int>* visited, std::priority_queue<MSTEdge>* queue)
	{
		visited->insert(current);
		for (int i=0; i<get_degree(current); i++) {
			int neigh = get_neighbor(current, i);
			if (visited->count(neigh) == 0)
				queue->push(MSTEdge(current, neigh, get_weight(current, i)));
		}
	}

public:
	Graph(bool directed) { _directed = directed; }
	~Graph()
	{
		for (Edge* e: _edges)
			delete e;
		for (Vertex* v: _vertices)
			delete v;
	}

	int add_vertex(VVALUE value)
	{
		int index = _vertices.size();
		Vertex* vertex = new Vertex(index, value);
		_vertices.push_back(vertex);
		return index;
	}

	int add_edge(int v1, int v2, EVALUE weight)
	{
		if (v1 >= _vertices.size() || v2 >= _vertices.size() || v1 < 0 || v2 < 0)
			return -1;
		Vertex* source = _vertices[v1];
		Vertex* target = _vertices[v2];
		int index = _edges.size();
		Edge* edge = new Edge(index, source, target, weight);
		_edges.push_back(edge);
		source->add_edge(edge);
		if (!_directed)
			target->add_edge(edge);
		return index;
	}

	int get_degree(int vindex)
	{
		if (vindex >= _vertices.size() || vindex < 0)
			return -1;
		return _vertices[vindex]->get_degree();
	}

	int get_neighbor(int vindex, int eindex)
	{
		if (vindex >= _vertices.size() || vindex < 0)
			return -1;
		return _vertices[vindex]->get_neighbor(eindex)->get_index();
	}

	int get_weight(int vindex, int eindex)
	{
		if (vindex >= _vertices.size() || vindex < 0)
			return -1;
		return _vertices[vindex]->get_edge(eindex)->get_weight();
	}

	VVALUE get_value(int vindex)
	{
		VVALUE def;
		if (vindex > _vertices.size() || vindex < 0)
			return def;
		return _vertices[vindex]->get_value();
	}

	int num_vertices() { return _vertices.size(); }
	int num_edges() { return _edges.size(); }

	Iterator* all_vertex_iterator() { return new AllVertexIterator(_vertices.size()); }
	Iterator* depth_first_iterator(int start = 0) { return new DepthFirstIterator(_vertices[start]); }
	Iterator* width_first_iterator(int start = 0) { return new WidthFirstIterator(_vertices[start]); }

	Graph<VVALUE,EVALUE> minimum_spanning_tree(int start = 0)
	{
		Graph<VVALUE,EVALUE> mst(true);
		for (int i=0; i<_vertices.size(); i++)
			mst.add_vertex(get_value(i));

		std::set<int> visited;
		std::priority_queue<MSTEdge> queue;

		MST_visit(start, &visited, &queue);
		while (queue.size()) {
			MSTEdge me = queue.top();
			queue.pop();
			if (visited.count(me.target) == 0) {
				mst.add_edge(me.source, me.target, me.weight);
				MST_visit(me.target, &visited, &queue);
			}
		}
		return mst;
	}

	void print(std::ostream& os)
	{
		if (_directed)
			os << "di";
		os << "graph {\n";
		for (int i=0; i<_vertices.size(); i++) {
			VVALUE name = get_value(i);
			os << "//    node " << i << " = "<< name << "\n";
			for (int n=0; n < get_degree(i); n++) {
				int nn = get_neighbor(i, n);
				if (_directed || nn > i) {
					os << "      " << name
						<< (_directed ? " -> " : " -- ")
						<< get_value(nn)
						<< " [label=" << get_weight(i, n) << "];\n";
				}
			}
		}
		os << "}\n";
	}

public:
    void calculatePageRank(std::ostream& os) {
	    auto invertedGraph = this->invertGraph();
        float dampingFactor = 0.85;
        bool unstable = true;
        int iteration = 0;
        float epsilon = 0.0001;

        while(unstable)
        {
            os << std::setprecision(5) << std::fixed;
            os << iteration << "\t";

            unstable = false;

            for (Vertex* vertex : _vertices){
                float oldPageRank = vertex->pageRank;
                float pageRankTemp = (1.0 - dampingFactor) / static_cast<float>(this->num_vertices());
                float pageRankOther = 0;

                for (int i = 0; i < invertedGraph->get_degree(vertex->get_index()); i++) {
                    Vertex* pageP = invertedGraph->_vertices.at(vertex->get_index())->get_neighbor(i);
                    pageRankOther += (this->_vertices.at(pageP->get_index())->pageRank / (float)(this->get_degree(pageP->get_index())));
                }

                vertex->pageRank = pageRankTemp + dampingFactor * pageRankOther;
                float delta = (oldPageRank - vertex->pageRank);

                if(abs(delta) > epsilon)
                {
                    unstable = true;
                }

                os << vertex->get_value() << " =" << vertex->pageRank << "\t";
            }
            os << std::endl;
            iteration++;
        }

        int nbVertex = 0;
        for(Vertex* vertex : this->_vertices)
        {
            os << vertex->get_value() << " =" << vertex->pageRank << std::endl;
            nbVertex++;
        }

        os << std::endl;
        os << "Found pagerank in " << iteration << " iteration (number of web site: " << nbVertex << ")" << std::endl;

    }

    Graph<VVALUE, EVALUE>* invertGraph()
    {
        auto invertedGraph = new Graph<VVALUE, EVALUE>(_directed);

        for(int i = 0; i < this->_vertices.size();i++)
        {
            invertedGraph->add_vertex(this->get_value(i));
        }

        for(Edge* edge : this->_edges)
        {
            invertedGraph->add_edge(edge->get_target()->get_index(), edge->get_source()->get_index(), edge->get_weight());
        }

        return invertedGraph;
    }

    void printGraphViz(std::ostream& os)
    {
        if (_directed)
            os << "di";
        os << "graph sites{\n";
        for (int i=0; i<_vertices.size(); i++) {
            VVALUE name = get_value(i);
            os << "//\tnode P" << i << " [label="<< name << "]\n";

        }

        for (int i=0; i<_vertices.size(); i++) {
            for (int n = 0; n < get_degree(i); n++) {
                int nn = get_neighbor(i, n);
                if (_directed || nn > i) {
                    os << "\t" << i
                       << (_directed ? " -> " : " -- ")
                       << nn << "\n";
                }
            }
        }
        os << "}\n";
    }
};

