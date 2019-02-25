/////////////////////////////////////////////////////////////////
// Graph.cpp - Demonstrate how to do Graph walks               //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////

#include "Graph.h"

//----< utilities >----------------------------------------------------------

void title(const std::string& t, char underline = '-')
{
  std::cout << "\n  " << t;
  std::cout << "\n " << std::string(t.size() + 2, underline);
}

void putLine(size_t lines = 1)
{
  for (size_t i = 0; i < lines; ++i)
    std::cout << "\n";
}

void putLine_Indent()
{
  std::cout << "\n  ";
}
//----< demonstration >------------------------------------------------------

int main()
{
  title("Demonstrate Graph Traversal Strategies", '=');
  putLine();

  using Graph = Graph<std::string>;
  using Node = Node<std::string>;
  using Sptr = Sptr<std::string>;

  Graph graph("testGraph 2-23-2018");
  graph.addNode(Sptr(new Node("n1")));
  graph.addNode(Sptr(new Node("n2")));
  graph.addNode(Sptr(new Node("n3")));
  graph.addNode(Sptr(new Node("n4")));
  graph.addNode(Sptr(new Node("n5")));
  graph.addNode(Sptr(new Node("n6")));
  graph.addEdge("n1", "n2");
  graph.addEdge("n2", "n1");
  graph.addEdge("n2", "n3");
  graph.addEdge("n2", "n4");
  graph.addEdge("n3", "n1");
  graph.addEdge("n3", "n4");
  graph.addEdge("n5", "n4");
  graph.addEdge("n6", "n2");

  showGraph<std::string>(graph);
  putLine();

  auto proc = [](Sptr pNode) { std::cout << " --> " << pNode->name(); };
  using P = decltype(proc);
  using V = std::string;

  title("DFS_r_ltr starting at node n1");
  putLine_Indent();
  Sptr pNode = graph.find("n1");
  graph.walker(DFS_r_ltr<V, P>, pNode, proc);
  // equivalent to:
  // DFS_r_ltr(pNode, proc);
  // graph.unmarkAll();
  putLine();

  title("DFS_r_ltr starting at node n1 and walking all");
  putLine_Indent();
  graph.walker(DFS_r_ltr<V, P>, pNode, proc, WALKALL);
  putLine();

  title("DFS_r_ltr starting at node n3");
  putLine_Indent();
  pNode = graph.find("n3");
  graph.walker(DFS_r_ltr<V, P>, pNode, proc);
  putLine();

  title("DFS_r_ltr starting at node n3 and walking all");
  putLine_Indent();
  graph.walker(DFS_r_ltr<V, P>, pNode, proc, WALKALL);
  putLine();

  title("DFS_r_ltrbtt starting at node n3");
  putLine_Indent();
  graph.walker(DFS_r_ltrbtt<V, P>, pNode, proc);
  putLine();

  title("DFS_r_rtl starting at node n3");
  putLine_Indent();
  graph.walker(DFS_r_rtl<V, P>, pNode, proc);
  putLine();

  title("DFS_s_ltr starting at node n3");
  putLine_Indent();
  graph.walker(DFS_s_ltr<V, P>, pNode, proc);
  putLine();

  title("DFS_s_rtl starting at node n3");
  putLine_Indent();
  graph.walker(DFS_s_rtl<V, P>, pNode, proc);
  putLine();

  title("BFS_q starting at node n3");
  putLine_Indent();
  graph.walker(BFS_q<V, P>, pNode, proc);

  putLine(3);
}