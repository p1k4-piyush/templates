
using namespace std;

#ifdef GRACIE
template <typename T>
struct min_cut_result : public vector<bool> {
    const flow_graph<T>* g_ptr;
    
    std::string graphviz() const {
        std::ostringstream os;
        os << "digraph G {\n";
        os << "  node [shape=circle, fontname=\"Courier\"];\n";
        
        const flow_graph<T>& g = *g_ptr;
        for (int i = 0; i < g.n; ++i) {
            os << "  " << i << " [label=\"" << i << "\"";
            if ((*this)[i]) {
                os << ", style=filled, fillcolor=\"#a2ff9c\""; 
