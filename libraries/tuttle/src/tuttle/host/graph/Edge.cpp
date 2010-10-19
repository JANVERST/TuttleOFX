#include "Edge.hpp"
#include "GraphExporter.hpp"

namespace tuttle {
namespace host {
namespace graph {

Edge::Edge()
{}

Edge::Edge( const Vertex::Key& out, const Vertex::Key& in, const std::string& inAttrName )
: IEdge( out, in, inAttrName )
{
}

Edge::Edge( const Edge& e )
: IEdge( e )
{}

Edge::~Edge()
{}


std::ostream& Edge::exportDotDebug( std::ostream& os ) const
{
	std::ostringstream s;
	s << subDotEntry( "label", getName() );
	s << subDotEntry( "id", _localId );
	std::ostringstream timesNeeded;
	BOOST_FOREACH( const TimeMap::value_type& m, _timesNeeded )
	{
		timesNeeded << "(" << m.first << ":";
		std::copy(
			m.second.begin(),
			m.second.end(),
			std::ostream_iterator<OfxTime>(timesNeeded, "," ) );
		timesNeeded << ")";
	}
	s << subDotEntry( "timesNeeded", timesNeeded.str() );

	os << "[" << std::endl;
	os << dotEntry( "type", "Edge" ) << ", " << std::endl;
	os << dotEntry( "label", s.str() ) << std::endl;
	os << "]" << std::endl;
	return os;
}

}
}
}
