#ifndef SAPPHIRE_IDMAP_H
#define SAPPHIRE_IDMAP_H

#include <unordered_map>
#include <boost/shared_ptr.hpp>

namespace Core {
namespace Util {

   template< class T >
   class IdMap
   {
   public:
      IdMap() {};
      virtual ~IdMap() {};

      void insert( boost::shared_ptr< T > entry )
      {
         m_idMap[id] = entry;
      }

      boost::shared_ptr< T > get( uint32_t id )
      {
         auto it = m_idMap.find( id );
         if( it != m_idMap.end() )
         {
            return it->second;
         }
         return nullptr;
      }

      void remove( uint32_t id )
      {
         m_idMap.erase( id );
      }

   private:
      std::unordered_map< uint32_t, boost::shared_ptr< T > > m_idMap;

   };


}
}


#endif //SAPPHIRE_IDMAP_H
