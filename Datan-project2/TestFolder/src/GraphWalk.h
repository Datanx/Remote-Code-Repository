#pragma once
/////////////////////////////////////////////////////////////////
// Graph.h - Demonstrate how to do Graph walks                 //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////
/*
* This demo displays several Graph traversal strategies.
* Any of them will work for Repository searches.
*
*/
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_map>

namespace GraphProcessing
{
  /////////////////////////////////////////////////////////////////////
  // Node<V> class
  // - represents graph vertices

  template<typename V>
  class Node;

  template<typename V>
  using Sptr = std::shared_ptr < Node<V> >;
  using Key = std::string;

  template<typename V>
  class Node
  {
  public:
    Node(const std::string& name) : name_(name) { }
    V value() const { return value_; }
    void value(V v) { value_ = v; }
    void addChild(Sptr<V> pNode) { children_.push_back(pNode); }
    std::vector<Sptr<V>>& children() { return children_; }
    std::string& name() { return name_; }
    void mark() { visited_ = true; }
    void unmark() { visited_ = false; }
    bool& marked() { return visited_; }
  private:
    V value_;
    std::string name_;
    std::vector<Sptr<V>> children_;
    bool visited_;
  };

  /////////////////////////////////////////////////////////////////////
  // Graph<V> class
  // - represents graph of Nodes connected by edges
  // - this class has the limitation that there is no convenient
  //   way to store information held by edges
  // - many applications don't need that, some do

  template<typename V>
  class Graph
  {
  public:
    using iterator = typename std::unordered_map<Key, Sptr<V>>::iterator;

    Graph(const std::string& gName = "") : name_(gName) {}
    void name(const std::string& name) { name_ = name; }
    std::string name() { return name_; }
    void addNode(Sptr<V> pNode);
    void addNode(const Key& name);
    bool containsNode(Sptr<V> pNode);
    bool containsNode(const Key& name);
    bool addEdge(Sptr<V> pSrc, Sptr<V> pDst);
    bool addEdge(const Key& srcName, const Key& dstName);
    iterator begin() { return nodeMap_.begin(); }
    iterator end() { return nodeMap_.end(); }
    Sptr<V> find(const Key& name);
    void unmarkAll();
    std::vector<Sptr<V>> findUnMarked();
    template<typename T, typename P>
    void walker(T t, Sptr<V> pNode, P p, bool walkAll = false);
  private:
    std::unordered_map<Key, Sptr<V>> nodeMap_;
    std::string name_;
  };

  //----< graph traversal >--------------------------------------------
  /*
  * - V represents node value type
  * - T is a traversal type, e.g., DFS or BFS
  * - P is the type of a callable object invoked on each node during walk
  */
  template<typename V>
  template<typename T, typename P>
  void Graph<V>::walker(T t, Sptr<V> pNode, P p, bool walkAll)
  {
    // traverse the graph, applying processing p to each node

    t(pNode, p);

    // a graph may have more than one node with no parents,
    // so t may not have visited all nodes

    if (walkAll)
    {
      std::vector<Sptr<V>> unmarked = findUnMarked();
      for (auto pNode : unmarked)
        t(pNode, p);
    }
    unmarkAll();
  }
  //----< add a node to the graph >------------------------------------

  template<typename V>
  void Graph<V>::addNode(const Key& name)
  {
    if (containsNode(name))
      return;
    nodeMap_[name] = Sptr<V>(new Node<V>(name));
  }
  //----< add a node to the graph >------------------------------------

  template<typename V>
  void Graph<V>::addNode(Sptr<V> pNode)
  {
    addNode(pNode->name());
  }
  //----< is this node in the graph? >---------------------------------

  template<typename V>
  bool Graph<V>::containsNode(const Key& name)
  {
    iterator iter = nodeMap_.find(name);
    return iter != nodeMap_.end();
  }
  //----< is this node in the graph? >---------------------------------

  template<typename V>
  bool Graph<V>::containsNode(Sptr<V> pNode)
  {
    return containsNode(pNode->name());
  }
  //----< find node with specified key >-------------------------------
  /*
  * - returns nullptr if not found in graph
  */
  template<typename V>
  Sptr<V> Graph<V>::find(const Key& name)
  {
    iterator iter = nodeMap_.find(name);
    if (iter != nodeMap_.end())
      return iter->second;
    return nullptr;
  }
  //----< adds edge from node with srcName to node with dstName >------

  template<typename V>
  bool Graph<V>::addEdge(const Key& srcName, const Key& dstName)
  {
    if (containsNode(srcName) && containsNode(dstName))
    {
      Sptr<V> pSrc = find(srcName);
      pSrc->addChild(find(dstName));
      return true;
    }
    return false;
  }
  //----< adds edge from src node to dst node >------------------------

  template<typename V>
  bool Graph<V>::addEdge(Sptr<V> pSrc, Sptr<V> pDst)
  {
    if (containsNode(pSrc) && containsNode(pDst))
    {
      pSrc->addChild(pDst);
      return true;
    }
    return false;
  }
  //----< remove all visitation marks from graph nodes >---------------

  template<typename V>
  void Graph<V>::unmarkAll()
  {
    for (auto item : nodeMap_)
    {
      item.second->unmark();
    }
  }
  //----< find any unmarked nodes >------------------------------------

  template<typename V>
  std::vector<Sptr<V>> Graph<V>::findUnMarked()
  {
    std::vector<Sptr<V>> unmarked;
    for (auto item : nodeMap_)
    {
      if (!item.second->marked())
        unmarked.push_back(item.second);
    }
    return unmarked;
  }
  //----< display graph >----------------------------------------------

  template<typename V>
  void showGraph(Graph<V>& graph, std::ostream& out = std::cout)
  {
    if (graph.name().size() > 0)
    {
      std::cout << "\n  Graph \"" << graph.name() << "\"";
    }
    for (auto item : graph)
    {
      out << "\n  " << item.first;
      if (item.second->children().size() > 0)
      {
        out << " --> ";
        for (auto pChild : item.second->children())
        {
          out << pChild->name() << " ";
        }
      }
    }
  }
  //----< traversal wrapper >------------------------------------------
  /*
  * - unbound traversal function does same thing as Graph<V>::walker
  * - will probably remove later
  */
  const bool WALKALL = true;

  template<typename T, typename P, typename V>
  void walker(T t, P p, Graph<V> graph, Sptr<V> pNode, bool walkall = false)
  {
    t(pNode, p);
    if (walkall)
    {
      std::vector<Sptr<V>> unmarked = graph.findUnMarked();
      for (auto pNode : unmarked)
        t(pNode, p);
    }
    graph.unmarkAll();
  }
}
/*---------------------------------------------------------------------
  Graph traversal strategies
  ---------------------------------------------------------------------
*Note:
*To accumulate data acquired during graph Walk :
*-recursive call methods require data storage to be defined
*   outside method, e.g., member data in class that defines
*   the recursive walk, or by passing as a parameter.
* -stack and queue based walks can define local data for
*   accumulation because you never leave the starting scope
*   until the search completes.
*----------------------------------------------------------------------
*/

using namespace GraphProcessing;

//----< recursive left-to-right Depth First Search >-------------------
/*
*  Good candidate for Repository searches
*  - V is type data carried by Node<V>
*  - P is the type of a callable object that is invoked on each node
*    during traversal
*/
template<typename V, typename P>
void DFS_r_ltr(Sptr<V> pNode, P p)
{
  if (pNode->marked())
    return;
  pNode->mark();
  p(pNode);
  //std::cout << "\n  " << pNode->name();
  for (auto pChild : pNode->children())
    DFS_r_ltr(pChild, p);
}
//----< recursive left-to-right, bottom-to-top Depth First Search >----------

template<typename V, typename P>
void DFS_r_ltrbtt(Sptr<V> pNode, P p)
{
  if (pNode->marked())
    return;
  pNode->mark();
  for (auto pChild : pNode->children())
    DFS_r_ltrbtt(pChild, p);
  p(pNode);
}
//----< recursive right-to-left Depth First Search >-------------------------

template<typename V, typename P>
void DFS_r_rtl(Sptr<V> pNode, P p)
{
  if (pNode->marked())
    return;
  pNode->mark();

  p(pNode);
  size_t numChildren = pNode->children().size();
  for (size_t i = 0; i < numChildren; ++i)
    DFS_r_rtl(pNode->children()[numChildren - i - 1], p);
}
//----< stack-based right-to-left Depth First Search >-----------------------
/*
*  Good candidate for XmlDocument search operations
*/
template<typename V, typename P>
void DFS_s_rtl(Sptr<V> pNode, P p)
{
  std::stack<Sptr<V>> nodeStack;
  nodeStack.push(pNode);
  while (nodeStack.size() > 0)
  {
    Sptr<V> pCurrentNode = nodeStack.top();
    nodeStack.pop();
    pCurrentNode->mark();
    p(pCurrentNode);
    for (auto pChild : pCurrentNode->children())
    {
      if (pChild->marked())
        continue;
      nodeStack.push(pChild);
    }
  }
}
//----< stack-based left-to-right Depth First Search >-----------------------

template<typename V, typename P>
void DFS_s_ltr(Sptr<V> pNode, P p)
{
  std::stack<Sptr<V>> nodeStack;
  nodeStack.push(pNode);
  while (nodeStack.size() > 0)
  {
    Sptr<V> pCurrentNode = nodeStack.top();
    nodeStack.pop();
    pCurrentNode->mark();
    p(pCurrentNode);
    size_t numChildren = pCurrentNode->children().size();
    for (size_t i = 0; i < numChildren; ++i)
    {
      Sptr<V> pChild = pCurrentNode->children()[numChildren - i - 1];
      if (pChild->marked())
        continue;
      nodeStack.push(pChild);
    }
  }
}
//----< queue-based left-to-right Breadth First Search >---------------------
/*
*   Breadth first search has the same variations as shown
*   above for Depth First Search.  I'll leave it up to you
*   to explore those if you wish.
*/
template<typename V, typename P>
void BFS_q(Sptr<V> pNode, P p)
{
  std::queue<Sptr<V>> nodeQueue;
  nodeQueue.push(pNode);
  while (nodeQueue.size() > 0)
  {

    Sptr<V> pCurrentNode = nodeQueue.front();
    nodeQueue.pop();
    pCurrentNode->mark();
    p(pCurrentNode);
    for (auto pChild : pCurrentNode->children())
    {
      if (pChild->marked())
        continue;
      nodeQueue.push(pChild);
    }
  }
}
