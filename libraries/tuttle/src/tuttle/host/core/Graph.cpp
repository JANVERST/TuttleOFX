#include "Graph.hpp"
#include "ProcessGraph.hpp"
#include <tuttle/host/ofx/OfxhImageEffectPlugin.hpp>
#include <tuttle/host/ofx/OfxhImageEffectNode.hpp>
#include <tuttle/host/ofx/attribute/OfxhClipImage.hpp>
#include <tuttle/host/graph/GraphExporter.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>

namespace tuttle {
namespace host {
namespace core {

Graph::Graph()
{
}

Graph::~Graph()
{}

Graph::Node& Graph::createNode( const std::string& id )
{
	ofx::imageEffect::OfxhImageEffectPlugin* plug = Core::instance().getImageEffectPluginById( id );

	if( !plug )
		BOOST_THROW_EXCEPTION( exception::LogicError( "Plugin not found. plug (" + id + ")" ) );

	plug->loadAndDescribeActions();

	ofx::imageEffect::OfxhImageEffectNode* plugInst = NULL;
	if( plug->supportsContext( kOfxImageEffectContextReader ) )
	{
		plugInst = plug->createInstance( kOfxImageEffectContextReader );
	}
	else if( plug->supportsContext( kOfxImageEffectContextWriter ) )
	{
		plugInst = plug->createInstance( kOfxImageEffectContextWriter );
	}
	else if( plug->supportsContext( kOfxImageEffectContextGenerator ) )
	{
		plugInst = plug->createInstance( kOfxImageEffectContextGenerator );
	}
	else if( plug->supportsContext( kOfxImageEffectContextFilter ) )
	{
		plugInst = plug->createInstance( kOfxImageEffectContextFilter );
	}
	else if( plug->supportsContext( kOfxImageEffectContextGeneral ) )
	{
		plugInst = plug->createInstance( kOfxImageEffectContextGeneral );
	}
	else
	{
		BOOST_THROW_EXCEPTION( exception::LogicError( "Plugin contexts not supported by the host. (" + id + ")" ) );
	}

	if( !plugInst )
		BOOST_THROW_EXCEPTION( exception::LogicError( "Plugin not found. plugInst (" + id + ")" ) );
	ImageEffectNode* node = dynamic_cast<ImageEffectNode*>( plugInst );
	if( !node )
		BOOST_THROW_EXCEPTION( exception::LogicError( "Plugin not found (" + id + ")." ) );

	std::stringstream uniqueName;
	uniqueName << node->getLabel() << ++_instanceCount[node->getLabel()];
	node->setName( uniqueName.str() );

	std::string key( node->getName() ); // for constness
	_nodes.insert( key, node );
	addToGraph( *node );

	return *node;
}


void Graph::addToGraph( Node& node )
{
	graph::Vertex v( node.getName(), node );

	COUT_DEBUG(node.getName());
	
	_graph.addVertex( v );
}

void Graph::removeFromGraph( Node& node ) throw( exception::LogicError )
{
	//	graph::Vertex v( node.getName(), &node );
	//
	//	_nodesList.find( &node );
	//	_nodes[node.getName()] = node;
	//	_graph.addVertex( v );
}

void Graph::deleteNode( const Node& node ) //throw( exception::LogicError )
{}

void Graph::connect( const Node& out, const Node& in ) //throw( exception::LogicError )
{
	connect( out, in.getSingleInputAttribute() );
}

void Graph::connect( const Node& out, const Attribute& inAttr ) //throw( exception::LogicError )
{
	_graph.connect( out.getName(), inAttr.getNode().getName(), inAttr.getName() );
}

//void Graph::unconnectNode( const Node& node ) //throw( exception::LogicError )
//{}

void Graph::compute( const std::list<std::string>& nodes, const int tBegin, const int tEnd )
{
	graph::exportAsDOT( _graph, "graph.dot" );

	COUT( "ProcessGraph constructor" );
	ProcessGraph process( *this, nodes );
	COUT( "ProcessGraph process" );
	process.process( tBegin, tEnd );
}

std::list<Graph::Node*> Graph::getNodesByContext( const std::string& context )
{
	std::list<Node*> selectedNodes;
	for( NodeMap::iterator it = getNodes().begin(), itEnd = getNodes().end();
		 it != itEnd;
		 ++it )
	{
		if( it->second->getContext() == context )
			selectedNodes.push_back( it->second );
	}
	return selectedNodes;
}

std::list<Graph::Node*> Graph::getNodesByPlugin( const std::string& pluginId )
{
	std::list<Node*> selectedNodes;
	for( NodeMap::iterator it = getNodes().begin(), itEnd = getNodes().end();
		 it != itEnd;
		 ++it )
	{
		if( boost::iequals( it->second->getPlugin().getIdentifier(), pluginId) )
			selectedNodes.push_back( it->second );
	}
	return selectedNodes;
}

std::ostream& operator<<( std::ostream& os, const Graph& g )
{
	os << "Graph" << std::endl
	   << g.getGraph();
	return os;
}

}
}
}

